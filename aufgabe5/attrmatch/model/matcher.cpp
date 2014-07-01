#include "matcher.hpp"

#include <fstream>
using std::ifstream;
using std::ofstream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <memory>
using std::shared_ptr;

//==============================================================================
// STATIC INITIALIZERS

const int Matcher::INPUTBUFFER_SIZE = 512;

shared_ptr<Relation> Matcher::mCurrentRelation = 0;
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

    //----
    // 1. Read and parse relations
    mCurrentRelation = shared_ptr<Relation>(r1);
    readParseRelation();

    mCurrentRelation = shared_ptr<Relation>(r2);
    readParseRelation();

    //----
    // 2. Find non-matching types
    // todo: Implement step2 in matcher

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
        mCurrentRelation->addTuple(new Tuple(buf));
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
