#ifndef RELATION_HPP
#define RELATION_HPP

#include "attributeblock.hpp"

#include <vector>
using std::vector;
#include <map>
using std::map;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
#include <boost/regex.hpp>
using boost::regex;
#include <iostream>
using std::ostream;

class Relation
{
public:
    static const char* TAG;
    static const char TUPLE_SEPARATOR;
    static const regex RX_ID;
    static const regex RX_DATE;
    static const regex RX_DATE_FOR_THE_STUPID_IDIOTS_WHO_CANNOT_HANDLE_EXCEL;
    static const regex RX_RANGE;
    static const regex RX_DOUBLE;
    static const regex RX_INTEGER;

    Relation(const string& filename);
    Relation(const char* filename);
    
    void compressAttributeTypes();
    void buildAttributeBlocks();
    void sortAttributes(int index);

    string fileName() const;
    void addTuple(const char* line);
    const map<int, vector<string> >& attributes() const;
    const vector<string>* attribute(int index) const throw (const string);
    map<int, vector<AttributeType> >& attributeTypes();
    const map<int, vector<AttributeType> >& attributeTypes_const() const;
    
private:
    string mFileName;
    map<int, vector<string> > mAttributes;
    map<int, vector<AttributeType> > mAttributeTypes;
//    map<int, vector<AttributeBlock> > mAttributeBlocks;

    void parseLine(const char* line);
    void addAttribute(int& index, const string& attribute);
    AttributeType getType(const string& attribute);
};

ostream& operator<<(ostream& lhs, const Relation& rhs);

#endif
