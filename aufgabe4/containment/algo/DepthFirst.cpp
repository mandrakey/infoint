#include "DepthFirst.hpp"
#include "model/Query.hpp"
#include "model/Literal.hpp"
#include "containment.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <memory>
using std::vector;
#include <algorithm>

//==============================================================================
// StackElement

DepthFirst::StackElement::StackElement() :
    mContainmentMapping(), mTargets(), mSources()
{
}

DepthFirst::StackElement::StackElement(ContainmentMapping cm,
                                       vector<Literal*> t,
                                       vector<Literal*> s) :
    mContainmentMapping(cm), mTargets(t), mSources(s)
{
}

//==============================================================================
// DepthFirst

bool DepthFirst::matches(const QueryPtr q1, const QueryPtr q2)
{
    // Prepare variables
    stack<StackElement> R;
    vector<Literal*> L;
    for (Literal& l : q1->literals()) {
        L.push_back(&l);
    }

    if (L.empty()) {
        return false;
    }

    Literal* l = L.at(0);
    vector<Literal*> H;
    // todo: Implement getPotentialTargets
    //vector<shared_ptr<const Literal> > H = getPotentialTargets(l, q2->literals());

    L.erase(std::find(L.begin(), L.end(), l));
    R.push(StackElement(ContainmentMapping(), H, L));
    L.clear();
    H.clear();

    do {
        StackElement e = R.top();
        R.pop();
        ContainmentMapping h = e.mContainmentMapping;
        L = e.mSources;
        H = e.mTargets;

        if (!H.empty()) {
            Literal* g = H.front();
            vector<Literal*> H_new = H;
            H_new.erase(std::find(H_new.begin(), H_new.end(), g));
            R.push(StackElement(h, H_new, L));

            // Check compatibility of h with mapping for g
            bool compat = true;
            // todo: Implement mappingCompatible
            //bool compat = mappingCompatible(h, l, g);
            if (compat) {
                if (L.empty()) {
                    return true;
                } else {
                    l = L.front();
                    //vector<shared_ptr<Literal> > H1 = getPotentialTargets(l, q2->literals());
                    vector<Literal*> H1;
                    vector<Literal*> Ln(L);
                    Ln.erase(std::find(Ln.begin(), Ln.end(), l));
                    R.push(StackElement(h, H1, Ln));
                }
            }
        }
    } while (!R.empty());

	return false;
}
