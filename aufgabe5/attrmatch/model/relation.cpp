#include "relation.hpp"

#include <memory>

using std::string;
using std::vector;

using std::shared_ptr;

Relation::Relation(const char* name) :
    mName(name), mTuples()
{
}

Relation::Relation(const string& name) : 
    mName(name), mTuples()
{
}

//==============================================================================
// GETTER / SETTER

string Relation::name() const
{
    return mName;
}

void Relation::addTuple(Tuple* tuple)
{
    if (tuple) {
        mTuples.push_back(shared_ptr<Tuple>(tuple));
    }
}

const vector<shared_ptr<Tuple> >& Relation::tuples() const
{
    return mTuples;
}
