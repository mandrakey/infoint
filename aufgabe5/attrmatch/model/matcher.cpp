#include "matcher.hpp"

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

    vector<pair<int, int> > matches;
    mRelations.first = shared_ptr<Relation>(r1);
    mRelations.second = shared_ptr<Relation>(r2);

    //----
    // 1. Read and parse relations
    mCurrentRelation = mRelations.first;
    readParseRelation();
    mCurrentRelation->compressAttributeTypes();

    mCurrentRelation = mRelations.second;
    readParseRelation();
    mCurrentRelation->compressAttributeTypes();

    // todo: Implement following steps in matcher

    //----
    // 2. Find non-matching types
    findPossibleMatches();

    //----
    // 3. Find singular matchings (ID, RANGE, LONGSTRING) and say "gotcha!"
    findSingularMatchings(matches);

    //----
    // 4. Sort still-to-match attribute lists
    pair<vector<int>, vector<int> > sorted;
    for (pair<const AttributeType, vector<pair<int, int> > >& p : mPossibleMatches) {
        for (pair<int, int>& rels : p.second) {
            if (std::find(sorted.first.begin(), sorted.first.end(), rels.first) != sorted.first.end()) {
                mRelations.first->sortAttributes(rels.first);
                sorted.first.push_back(rels.first);
            }
            if (std::find(sorted.second.begin(), sorted.second.end(), rels.second) != sorted.second.end()) {
                mRelations.second->sortAttributes(rels.second);
                sorted.second.push_back(rels.second);
            }
        }
    }

    //----
    // 5. Build blocks over sorted attribute lists
    // Relation should do that, like mRelations.first->buildAttributeBlocks()

    //----
    // 6. Compare attribute-wise for similarity
    // use generated block lists: mRelations.first->attributeBlocks()
    // hint: first block-list = first attribute and so on
    // bool attributesMatch(const AttributeBlock& a, const AttributeBlock& b);
    // count matches somehow ... !!!

    //----
    // 7. Look through produces matches list
    // if, for two attributes, there is not match -> do nothing
    // if, for two attributes, there is exactly one match -> add to matches
    // if, for two attributes, there are 2+ matches -> add the on with highest block match count to matches

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
    switch (t) {
    case INTEGER:
        int mean1 = 0, mean2 = 0;
        for (string& s : a) {
            mean1 += atoi(s.c_str());
        }
        mean1 = mean1 / a.size();

        for (string& s : b) {
            mean2 += atoi(s.c_str());
        }
        mean2 = mean2 / a.size();

        if ((mean1 < 100 && mean2 < 100) ||
                (mean1 < 1500 && mean2 < 1500) ||
                (mean1 >= 1500 && mean2 >= 1500)) {
            return true;
        } else {
            return false;
        }

        break;
    case DOUBLE:
        double mean1 = 0, mean2 = 0;
        bool maxMoreTen1 = false, maxMoreTen2 = false;
        for (string& s : a) {
            double d = atof(s.c_str());
            mean1 += atof(d);
            if (d > 10) {
                maxMoreTen1 = true;
            }
        }
        mean1 = mean1 / a.size();

        for (string& s : b) {
            double d = atof(s.c_str());
            mean2 += atof(d);
            if (d > 10) {
                maxMoreTen2 = true;
            }
        }
        mean2 = mean2 / a.size();

        if ((mean1 > 50 && mean2 > 50) ||
                (mean1 < 5 && mean2 < 5) ||
                (maxMoreTen1 == maxMoreTen2)) {
            return true;
        }

        break;
    case STRING:
        //looks for meanlength of strings
        int meanlength1 = 0, meanlength2 = 0;

        //looks for "NULL" in a string
        int nullrows1 = 0, nullrows2 = 0;

        //counts NULLS and calculates the meanlength for attrib 1
        for (string& s : a){
            meanlength1 += s.size();
            if (s.empty()){
                ++nullrows1;
            }
        }
        meanlength1 = meanlength1 / a.size();

        //counts NULLS and calculates the meanlength for attrib 2
        for (string& s : b){
            meanlength2 += s.size();
            if (s.empty()){
                ++nullrows2;
            }
        }
        meanlength2 = meanlength2 / b.size();

        //matches the long string and the very short stings
        if (meanlength1 > 20 && meanlength2 > 20 || meanlength1 <=4 && meanlength2 <= 4){
            return true;
        }

        /*todo: look for "," in strings... stings with more "," than
         * 10 matches and with less than 10! The strings differ in the sum
         * of "," in strings...*/
        else if(){

        }
        /* looks for "NULL" rows and differs between sets with much NULL rows.
         * this sets are posible candidates for a match. see origin attribute
         * three, no other so much NULL  rows in it! Only the last Attrib in origin
         * File has NULL rows, but only 4! */

        else if(nullrows1 > 5 && nullrows2 > 5){
            return true;
        }


        break;
    case DATE:
        int empty1 = 0, empty2 = 0;
        for (string& s : a) {
            if (s.empty()) {
                ++empty1;
            }
        }

        for (string& s : b) {
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
    default:
        return false;
    }
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
