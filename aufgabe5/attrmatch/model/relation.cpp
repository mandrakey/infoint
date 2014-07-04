#include "relation.hpp"

#include <algorithm>
#include <cstring>
#include <sstream>
using std::stringstream;
#include <utility>
using std::pair;

using std::string;
using std::vector;
using std::map;
using std::shared_ptr;

using boost::regex;

const char Relation::TUPLE_SEPARATOR = ';';
const regex Relation::RX_ID("[0-9]{2,3}[a-zA-Z]{2,3}[0-9]{2,3}");
const regex Relation::RX_DATE("[0-9]{1,2}\\-[a-zA-Z]{3}\\-[0-9]{2}");
const regex Relation::RX_RANGE("[0-9\\.]+ ?(\\-|to) ?[0-9\\.]+");
const regex Relation::RX_DOUBLE("\\-?[0-9]+\\.[0-9]+");
const regex Relation::RX_INTEGER("\\-?[0-9]+");

Relation::Relation(const char* filename) :
    mFileName(filename), mAttributes(), mAttributeTypes(), mAttributeBlocks()
{
}

Relation::Relation(const string& filename) :
    mFileName(filename), mAttributes(), mAttributeTypes(), mAttributeBlocks()
{
}

void Relation::compressAttributeTypes()
{
    map<AttributeType, int> typeCounts;
    for (pair<int, vector<AttributeType> > p : mAttributeTypes) {
        vector<AttributeType>& v = p.second;
        unsigned int sum = 0;

        for (AttributeType at : v) {
            int count = std::count(v.begin(), v.end(), at);
            sum += count;

            typeCounts[at] = count;
            if (sum >= v.size()) {
                break;
            }
        }

        auto it = typeCounts.begin();
        AttributeType max = (*it).first;
        int maxCount = (*it).second;
        while (it != typeCounts.end()) {
            if (maxCount < (*it).second) {
                max = (*it).first;
            }
            ++it;
        }

        // Now, clear v and set max as single element
        v.clear();
        v.push_back(max);
    }
}

void Relation::buildAttributeBlocks()
{
    // todo: implement Relation::buildAttributeBlocks
}

void Relation::sortAttributes(int index)
{
    std::sort(mAttributes[index].begin(), mAttributes[index].end());
}

void Relation::parseLine(const char* line)
{
    if (!line) {
        return;
    }

    bool literal = false;
    vector<string> tokens;
    stringstream token;
    int attributeIndex = 0;

    // todo: Refactor tokens.push_back into method
    // -> enables calls to Matcher's event methods
    // !important: count attribute number!

    for (unsigned int i = 0; i < strlen(line); ++i) {
        char c = line[i];
        if (literal) {
            token << c;
            if (c == '\"') {
                literal = false;
            }
        } else {
            if (c == TUPLE_SEPARATOR) {
                addAttribute(attributeIndex, token.str());
                token.str("");
            } else if (c == '\"') {
                token << c;
                literal = true;
            } else {
                token << c;
            }
        }
    }

    if (!token.str().empty()) {
        addAttribute(attributeIndex, token.str());
    }
}

void Relation::addAttribute(int& index, const string& attribute)
{
    mAttributes[index].push_back(attribute);
    mAttributeTypes[index].push_back(getType(attribute));
    ++index;
}

AttributeType Relation::getType(const string& attribute)
{
    // Is it an ID?
    const char* exp = attribute.c_str();
    if (boost::regex_match(exp, RX_ID)) {
        return AttributeType::ID;
    }

    // Is it a date value?
    if (boost::regex_match(exp, RX_DATE)) {
        return AttributeType::DATE;
    }

    // Is it a range value?
    if (boost::regex_match(exp, RX_RANGE)) {
        return AttributeType::RANGE;
    }

    // Is it a double value?
    if (boost::regex_match(exp, RX_DOUBLE)) {
        return AttributeType::DOUBLE;
    }

    // Is it an integer value?
    if (boost::regex_match(exp, RX_INTEGER)) {
        return AttributeType::INTEGER;
    }

    // Ok, must be a string. Long?
    return (attribute.size() > 20) ? AttributeType::LONGSTRING : AttributeType::STRING;
}

//==============================================================================
// GETTER / SETTER

string Relation::fileName() const
{
    return mFileName;
}

// todo: refactor addTuple to do tuple parsing; remote Tuple class
// build column based attribute lists -> easier for later
// access
void Relation::addTuple(const char* line)
{
    if (line) {
        parseLine(line);
    }
}

const map<int, vector<string> >& Relation::attributes() const
{
    return mAttributes;
}

const vector<string>* Relation::attribute(int index) const throw (const string)
{
    if (mAttributes.find(index) == mAttributes.end()) {
        throw string("Attribute index out of range");
    }

    return &mAttributes.at(index);
}

map<int, vector<AttributeType> >& Relation::attributeTypes()
{
    return mAttributeTypes;
}

const map<int, vector<AttributeBlock> >& Relation::attributeBlocks() const
{
    return mAttributeBlocks;
}
