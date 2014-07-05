#include "attributetype.hpp"

const string AttributeTypeToString(const AttributeType t)
{
    switch (t) {
    case DATE:
        return "DATE";
    case INTEGER:
        return "INTEGER";
    case DOUBLE:
        return "DOUBLE";
    case STRING:
        return "STRING";
    case LONGSTRING:
        return "LONGSTRING";
    case ID:
        return "ID";
    case RANGE:
        return "RANGE";
    default:
        return "UNKNOWN";
    }
}
