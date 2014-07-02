#include "relation.hpp"

#include <cstring>
#include <sstream>
using std::stringstream;

using std::string;
using std::vector;
using std::map;

using std::shared_ptr;

const char Relation::TUPLE_SEPARATOR = ';';

Relation::Relation(const char* filename) :
    mFileName(filename), mAttributes(), mAttributeBlocks()
{
}

Relation::Relation(const string& filename) :
    mFileName(filename), mAttributes(), mAttributeBlocks()
{
}

void Relation::buildAttributeBlocks()
{
    // todo: implement Relation::buildAttributeBlocks
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

const map<int, vector<AttributeBlock> >& Relation::attributeBlocks() const
{
    return mAttributeBlocks;
}
