#include "tuple.hpp"

#include <cstring>
#include <vector>
#include <sstream>
using std::stringstream;

using std::string;
using std::vector;

const char Tuple::TUPLE_SEPARATOR = ';';

Tuple::Tuple(const char* line) : 
    mAttributes()
{
    parseLine(line);
}

const vector<string>& Tuple::attributes() const
{
    return mAttributes;
}

const string& Tuple::attribute(unsigned int index) const throw (const char*)
{
    if (index > mAttributes.size()) {
        throw "Requested attribute index out of range.";
    }
    
    return mAttributes.at(index);
}

void Tuple::parseLine(const char* line)
{
    if (!line) {
        return;
    }

    bool literal = false;
    vector<string> tokens;
    stringstream token;

    for (unsigned int i = 0; i < strlen(line); ++i) {
        char c = line[i];
        if (literal) {
            token << c;
            if (c == '\"') {
                literal = false;
            }
        } else {
            if (c == TUPLE_SEPARATOR) {
                tokens.push_back(token.str());
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
        tokens.push_back(token.str());
    }

    mAttributes = tokens;
}
