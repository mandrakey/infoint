#include "attributeblock.hpp"

#include <vector>
using std::vector;
#include <string>
using std::string;

AttributeBlock::AttributeBlock() :
    mAttributes(), mMeanValue(0), mType(UNKNOWN)
{
}

vector<string> AttributeBlock::attributes() const
{
    return vector<string>(mAttributes);
}

string AttributeBlock::attribute(unsigned int index) const throw (const string&)
{
    if (index > mAttributes.size()) {
        throw string("Attribute index out of range.");
    }

    return mAttributes.at(index);
}

int AttributeBlock::meanValue() const
{
    return mMeanValue;
}

AttributeType AttributeBlock::type() const
{
    return mType;
}
