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
    // use std::sort(vector.begin(), vector.end()), needs include for algorithm

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

    for (pair<int, vector<AttributeType> p1 : at1) {
        for (pair<int, vector<AttributeType> p2 : at2) {
            if (p1.second.at(0) == p2.second.at(0)) {
                mPossibleMatches[p1.second.at(0)].push_back(pair<int,int>(p1.first, p2.first));
            }
        }
    }
}

void Matcher::findSingularMatchings(vector<pair<int, int> >& matches)
{
    for (AttributeType t : vector<AttributeType>({ID, RANGE, LONGSTRING})) {
        if (mPossibleMatches.find(t) != mPossibleMatches.end()) {
            for (pair<int, int> p : mPossibleMatches[t]) {
                matches.push_back(p);
            }
        }
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
