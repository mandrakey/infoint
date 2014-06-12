#ifndef DEPTHFIRST_HPP_
#define DEPTHFIRST_HPP_

#include "Algorithm.hpp"
#include "model/Literal.hpp"
#include "model/Query.hpp"

#include <stack>
using std::stack;
#include <vector>
using std::vector;
#include <utility>
using std::pair;

typedef vector<pair<char, char> > ContainmentMapping;

class DepthFirst : public Algorithm
{
public:
    virtual bool matches(const QueryPtr q1, const QueryPtr q2);

private:

    class StackElement {
    public:
        StackElement();
        StackElement(ContainmentMapping cm, vector<Literal*> t, vector<Literal*> s);

        ContainmentMapping mContainmentMapping;
        vector<Literal*> mTargets;
        vector<Literal*> mSources;
    };

};

#endif /* DEPTHFIRST_HPP_ */
