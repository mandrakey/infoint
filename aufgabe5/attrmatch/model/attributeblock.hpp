#ifndef ATTRIBUTEBLOCK_HPP
#define ATTRIBUTEBLOCK_HPP

#include "attributetype.hpp"

#include <vector>
using std::vector;
#include <string>
using std::string;

class AttributeBlock
{
public:
    AttributeBlock();

    vector<string> attributes() const;
    string attribute(unsigned int index) const throw (const string&);
    int meanValue() const;
    AttributeType type() const;

private:
    vector<string> mAttributes;
    int mMeanValue;
    AttributeType mType;
};

#endif // ATTRIBUTEBLOCK_HPP
