#ifndef ATTRIBUTETYPE_HPP
#define ATTRIBUTETYPE_HPP

#include <string>
using std::string;

enum AttributeType
{
    DATE, INTEGER, DOUBLE, STRING, LONGSTRING, ID, RANGE, UNKNOWN
};

const string AttributeTypeToString(const AttributeType t);

#endif // ATTRIBUTETYPE_HPP
