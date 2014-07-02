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

class Relation
{
public:
    static const char TUPLE_SEPARATOR;

    Relation(const string& filename);
    Relation(const char* filename);
    
    void buildAttributeBlocks();

    string fileName() const;
    void addTuple(const char* line);
    const map<int, vector<string> >& attributes() const;
    const map<int, vector<AttributeBlock> >& attributeBlocks() const;
    
private:
    string mFileName;
    map<int, vector<string> > mAttributes;
    map<int, vector<AttributeBlock> > mAttributeBlocks;

    void parseLine(const char* line);
    void addAttribute(int& index, const string& attribute);
};

#endif
