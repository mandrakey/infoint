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
#include <iostream>
#include <string>
using std::string;

typedef vector<pair<char, char> > ContainmentMapping;

class DepthFirst : public Algorithm
{
public:
    static string TAG;

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

public: // todo: remove this "public"
    vector<Literal*> getPotentialTargets(Literal* l, vector<Literal>& targets) const;
    bool mappingCompatible(ContainmentMapping h, ContainmentMapping g) const;
    ContainmentMapping createMapping(Literal* l, Literal* g) throw(string);
    ContainmentMapping mergeMappings(ContainmentMapping h, ContainmentMapping g) const;

};

std::ostream& operator<<(std::ostream& lhs, const ContainmentMapping& rhs);
string cmToString(const ContainmentMapping& cm);

#endif /* DEPTHFIRST_HPP_ */
