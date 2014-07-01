#include "relation.hpp"

#include <memory>

using std::string;
using std::vector;

using std::shared_ptr;

Relation::Relation(const char* filename) :
    mFileName(filename), mTuples()
{
}

Relation::Relation(const string& filename) :
    mFileName(filename), mTuples()
{
}

//==============================================================================
// GETTER / SETTER

string Relation::fileName() const
{
    return mFileName;
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
