#ifndef RELATION_HPP
#define RELATION_HPP

#include "tuple.hpp"

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;

class Relation
{
public:
    Relation(const string& filename);
    Relation(const char* filename);
    
    string fileName() const;
    void addTuple(Tuple* tuple);
    const vector<shared_ptr<Tuple> >& tuples() const;
    
private:
    string mFileName;
    vector<shared_ptr<Tuple> > mTuples;
};

#endif
