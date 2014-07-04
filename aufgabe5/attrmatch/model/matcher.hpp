#ifndef MATCHER_HPP
#define MATCHER_HPP

#include "attributetype.hpp"
#include "relation.hpp"
#include "attributeblock.hpp"

#include <memory>
using std::shared_ptr;
#include <map>
using std::map;
#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <string>
using std::string;

typedef map<AttributeType, vector<pair<int, int> > > PossibleMatchMap;

class Matcher
{
public:
    static const char* TAG;
    static const int INPUTBUFFER_SIZE;

    Matcher();

    void reset();
    vector<pair<int, int> > match(Relation* r1, Relation* r2) throw (const string&);

    void onAttributeTypeDetected(int attribute, AttributeType type);
    void onAttributeValueRead(int attribute, int value);

private:
    static shared_ptr<Relation> mCurrentRelation;
    static pair<shared_ptr<Relation>, shared_ptr<Relation> > mRelations;
    static PossibleMatchMap mPossibleMatches;
    static map<int, int> mTemporaryAttributeMean;
    static map<int, map<AttributeType, int> > mTemporaryAttributeTypes;

    void readParseRelation() throw (const string&);
    void findPossibleMatches();
    void findSingularMatchings(vector<pair<int, int> >& matches);
    bool attributesMatch(const AttributeBlock& a, const AttributeBlock& b);
    bool attributesMatch(const vector<string>& a, const vector<string>& b, const AttributeType t);
    int wordDifference(const string& s, const string& t);
};

#endif // MATCHER_HPP
