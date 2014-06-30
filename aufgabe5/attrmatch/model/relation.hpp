#ifndef RELATION_HPP
#define RELATION_HPP

#include "tuple.hpp"

#include <vector>
#include <string>
#include <memory>

class Relation
{
public:
    Relation(const std::string& name);
    Relation(const char* name);
    
    std::string name() const;
    void addTuple(Tuple* tuple);
    const std::vector<std::shared_ptr<Tuple> >& tuples() const;
    
private:
    std::string mName;
    std::vector<std::shared_ptr<Tuple> > mTuples;
};

#endif
