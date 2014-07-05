#include "matcher.hpp"
#include "bmlib/log.hpp"


#include <string.h>
#include <stdio.h>
#include <cctype>
#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::pair;
using std::shared_ptr;
using std::map;


//==============================================================================
// STATIC INITIALIZERS

const char* Matcher::TAG = "Matcher";
const int Matcher::INPUTBUFFER_SIZE = 512;

shared_ptr<Relation> Matcher::mCurrentRelation = 0;
pair<shared_ptr<Relation>, shared_ptr<Relation> > Matcher::mRelations;
PossibleMatchMap Matcher::mPossibleMatches;
map<int, int> Matcher::mTemporaryAttributeMean;
map<int, map<AttributeType, int> > Matcher::mTemporaryAttributeTypes;

//==============================================================================
// INSTANCE METHODS

Matcher::Matcher()
{ // Does nothing -> monostate object
}

void Matcher::reset()
{
    // todo: Reset all statics to default state
}

vector<pair<int, int> > Matcher::match(Relation* r1, Relation* r2)
    throw (const string&)
{
    if (!r1) {
        throw string("Input file 1 not set.");
    }
    if (!r2) {
        throw string("Input file 2 not set.");
    }

    if (Log::debug()) {
        cout << "Types are:" << endl;
        cout << "DATE = " << AttributeType::DATE << endl <<
                "INTEGER = " << AttributeType::INTEGER << endl <<
                "DOUBLE = " << AttributeType::DOUBLE << endl <<
                "STRING = " << AttributeType::STRING << endl <<
                "LONGSTRING = " << AttributeType::LONGSTRING << endl <<
                "ID = " << AttributeType::ID << endl <<
                "RANGE = " << AttributeType::RANGE << endl;
    }

    vector<pair<int, int> > matches;
    mRelations.first = shared_ptr<Relation>(r1);
    mRelations.second = shared_ptr<Relation>(r2);

    //----
    // 1. Read and parse relations
    mCurrentRelation = mRelations.first;
    readParseRelation();
    mCurrentRelation->compressAttributeTypes();
    if (Log::debug()) {
        cout << endl << mCurrentRelation->fileName() << endl <<
                "--------------------------------------------------------------------------------" <<
                endl << *mCurrentRelation <<
                "--------------------------------------------------------------------------------" <<
                endl;
    }

    mCurrentRelation = mRelations.second;
    readParseRelation();
    mCurrentRelation->compressAttributeTypes();
    if (Log::debug()) {
        cout << endl << mCurrentRelation->fileName() << endl <<
                "--------------------------------------------------------------------------------" <<
                endl << *mCurrentRelation <<
                "--------------------------------------------------------------------------------" <<
                endl;
    }

    // todo: Implement following steps in matcher

    //----
    // 2. Find non-matching types
    findPossibleMatches();

    //----
    // 3. Find singular matchings (ID, RANGE, LONGSTRING) and say "gotcha!"
    findSingularMatchings(matches);

    //----
    // 4. Sort still-to-match attribute lists
//    pair<vector<int>, vector<int> > sorted;
//    for (pair<const AttributeType, vector<pair<int, int> > >& p : mPossibleMatches) {
//        for (pair<int, int>& rels : p.second) {
//            if (std::find(sorted.first.begin(), sorted.first.end(), rels.first) != sorted.first.end()) {
//                mRelations.first->sortAttributes(rels.first);
//                sorted.first.push_back(rels.first);
//            }
//            if (std::find(sorted.second.begin(), sorted.second.end(), rels.second) != sorted.second.end()) {
//                mRelations.second->sortAttributes(rels.second);
//                sorted.second.push_back(rels.second);
//            }
//        }
//    }

    //----
    // 5. Build blocks over sorted attribute lists
    // Relation should do that, like mRelations.first->buildAttributeBlocks()

    //----
    // 6. Compare attribute-wise for similarity
    // use generated block lists: mRelations.first->attributeBlocks()
    // hint: first block-list = first attribute and so on
    // bool attributesMatch(const AttributeBlock& a, const AttributeBlock& b);
    // count matches somehow ... !!!

    vector<int> matched;
    for (pair<const AttributeType, vector<pair<int, int> > >& p : mPossibleMatches) {
        for (pair<int, int> attrs : p.second) {
            if (std::find(matched.begin(), matched.end(), attrs.first) == matched.end()) {
                const vector<string>* attr1 = 0, *attr2 = 0;
                try {
                    attr1 = mRelations.first->attribute(attrs.first);
                    attr2 = mRelations.second->attribute(attrs.second);
                } catch (const string ex) {
                    Log::e(TAG, string("Error matching attributes: ").append(ex));
                    continue;
                }

                cout << "Trying to match: " << "(" << AttributeTypeToString(p.first) << ") " <<
                                            attrs.first << " against " <<
                                            attrs.second << endl;
                if (attributesMatch(*attr1, *attr2, p.first)) {
                    matches.push_back(pair<int, int>(attrs.first, attrs.second));
                    matched.push_back(attrs.first);
                }
            } else {
                cout << "Not testing: " << attrs.first << " <-> " << attrs.second << endl;
            }
        }
    }

    return matches;
}

void Matcher::readParseRelation() throw (const string&)
{
    if (!mCurrentRelation) {
        throw string("Cannot parse not-set relation.");
    }

    char buf[INPUTBUFFER_SIZE];
    ifstream in(mCurrentRelation->fileName(), std::ios_base::in);
    if (!in.is_open()) {
        throw string("Failed to open input file '").append(string(mCurrentRelation->fileName()))
                .append("'");
    }

    while (!in.eof()) {
        in.getline(buf, INPUTBUFFER_SIZE);
        mCurrentRelation->addTuple(buf);
    }
}

void Matcher::findPossibleMatches()
{
    map<int, vector<AttributeType> >& at1 = mRelations.first->attributeTypes();
    map<int, vector<AttributeType> >& at2 = mRelations.second->attributeTypes();

    for (pair<int, vector<AttributeType> > p1 : at1) {
        for (pair<int, vector<AttributeType> > p2 : at2) {
            if (p1.second.at(0) == p2.second.at(0)) {
                mPossibleMatches[p1.second.at(0)].push_back(pair<int,int>(p1.first, p2.first));
            }
        }
    }
}

void Matcher::findSingularMatchings(vector<pair<int, int> >& matches)
{
    for (AttributeType t : vector<AttributeType>({ID, RANGE, LONGSTRING})) {
        auto it = mPossibleMatches.find(t);
        if (it != mPossibleMatches.end()) {
            for (pair<int, int> p : mPossibleMatches[t]) {
                matches.push_back(p);
            }
            mPossibleMatches.erase(it);
        }
    }
}

bool Matcher::attributesMatch(const vector<std::string>& a, const vector<std::string>& b, const AttributeType t)
{
//    cout << "Matching type " << t << endl;
    switch (t) {
    case INTEGER: {
        int mean1 = 0, mean2 = 0;
        double jaccardCoefficient = 0.0;

        for (const string& s : a) {
            mean1 += atoi(s.c_str());
        }

        for (const string& s : b) {
            mean2 += atoi(s.c_str());
        }

        mean1 = mean1 / a.size();
        mean2 = mean2 / a.size();
        jaccardCoefficient = jaccard(a,b);

        if (((mean1 < 100 && mean2 < 100) ||
                (mean1 < 1500 && mean2 < 1500) ||
                (mean1 >= 1500 && mean2 >= 1500)) &&
                jaccardCoefficient > 0.1) {
            return true;
        } else {
            return false;
        }


        break;
    }
    case DOUBLE: {
        double dmean1 = 0, dmean2 = 0;
        bool maxMoreTen1 = false, maxMoreTen2 = false;
        double jaccardCoefficient = 0.0;

        for (const string& s : a) {
            double d = atof(s.c_str());
            dmean1 += d;
            if (!maxMoreTen1 && d > 10) {
                maxMoreTen1 = true;
            }
        }

        for (const string& s : b) {
            double d = atof(s.c_str());
            dmean2 += d;
            if (!maxMoreTen2 && d > 10) {
                maxMoreTen2 = true;
            }
        }
        dmean1 = dmean1 / a.size();
        dmean2 = dmean2 / b.size();
        jaccardCoefficient = jaccard(a, b);

        if (((dmean1 > 50 && dmean2 > 50) ||
                (dmean1 < 5 && dmean2 < 5) ||
                (maxMoreTen1 == maxMoreTen2)) &&
                jaccardCoefficient > 0.1) {
            return true;
        } else {
            return false;
        }

        break;
    }
    case STRING: {
        //----
        // Calculate string mean length, count empty entries and amount of
        // commas in text
        // empty = "", "-"
        bool calculatedB = false;
        int meanlength1 = 0, meanlength2 = 0;
        int emptyrows1 = 0, emptyrows2 = 0;
        int countcomma1 = 0, countcomma2 = 0;
        double jaccardCoefficient = 0.0;

        for (const string& s : a) {
            meanlength1 += s.size();
            if (s.empty()) {
                ++emptyrows1;
            } else {
                countcomma1 += std::count(s.begin(), s.end(), ',');
            }
        }
        for (const string& s : b) {
            meanlength2 += s.size();
            if (s.empty()) {
                ++emptyrows2;
            } else {
                countcomma2 += std::count(s.begin(), s.end(), ',');
            }
        }

        jaccardCoefficient = jaccard(a, b);
        meanlength1 = meanlength1 / a.size();
        meanlength2 = meanlength2 / b.size();
        countcomma1 = countcomma1 / a.size();
        countcomma2 = countcomma2 / b.size();

        //---
        // Check results

        if (((meanlength1 <= 4 && meanlength2 <= 4) ||
                (countcomma1 < 10 && countcomma2 < 10) ||
                (countcomma1 >= 10 && countcomma2 >= 10) ||
                (emptyrows1 > 4 && emptyrows2 > 4)) &&
                jaccardCoefficient > 0.5) {
            return true;
        } else {
            return false;
        }

        break;
    }
    case DATE: {
        int empty1 = 0, empty2 = 0;
        for (const string& s : a) {
            if (s.empty()) {
                ++empty1;
            }
        }

        for (const string& s : b) {
            if (s.empty()) {
                ++empty2;
            }
        }

        if ((empty1 < 4 && empty2 < 4) || (empty1 >= 4 && empty2 >= 4)) {
            return true;
        } else {
            return false;
        }

        break;
    }
    default:
        break;
    }

    return false;
}

int Matcher::wordDifference(const std::string& s, const std::string& t)
{
    if (s.empty()) {
        return t.size();
    }
    if (t.empty()) {
        return s.size();
    }
    if (s == t) {
        return 0;
    }

    string small((s.size() < t.size()) ? s : t);
    string big((s.size() > t.size()) ? s : t);
    std::transform(small.begin(), small.end(), small.begin(), std::ptr_fun<int, int>(std::tolower));
    std::transform(big.begin(), big.end(), big.begin(), std::ptr_fun<int, int>(std::tolower));

    int diff = 0;
    for (unsigned int i = 0, j = 0; i < small.size(), j < big.size(); ++i, ++j) {
        while (small[i] != big[j] && j < big.size()) {
            ++diff;
            ++j;
        }
    }

    return diff;
}

double Matcher::jaccard(const vector<std::string>& a, const vector<std::string>& b)
{
    vector<string> intersected;
    vector<string> united;

    for (string s : a) {
        if (std::find(united.begin(), united.end(), s) == united.end()) {
            united.push_back(s);
        }

        if (std::find(intersected.begin(), intersected.end(), s) == intersected.end()) {
            for (string s2 : b) {
                if ((s.size() <= 7 && s == s2) ||
                        (s.size() > 7 && wordDifference(s,s2) < 2)) {
                    intersected.push_back(s2);
                    break;
                }
            }
        }
    }

    // Add all values from b to union
    for (string s : b) {
        if (std::find(united.begin(), united.end(), s) == united.end()) {
            united.push_back(s);
        }
    }

    if (Log::debug()) {
        cout << "==== JACCARD" << endl;
        cout << "intersected = [ ";
        for (string s : intersected) {
            cout << s << ", ";
        }
        cout << "]" << endl << "united= [ ";
        for (string s : united) {
            cout << s << ", ";
        }
        cout << "]" << endl << "Result: " <<
                intersected.size() << "/" << united.size() << " = " <<
                (0.0 + intersected.size()) / united.size() << endl <<
                "JACCARD ====" << endl << endl;
    }
    return ((0.0 + intersected.size()) / united.size());
}

//------------------------------------------------------------------------------
// EVENT HANDLER

void Matcher::onAttributeTypeDetected(int attribute, AttributeType type)
{
    ++mTemporaryAttributeTypes[attribute][type];
}

void Matcher::onAttributeValueRead(int attribute, int value)
{
    mTemporaryAttributeMean[attribute] += value;
}
