#include "DepthFirst.hpp"
#include "model/Query.hpp"
#include "model/Literal.hpp"
#include "containment.hpp"
#include "bmlib/log.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <memory>
using std::vector;
#include <algorithm>
#include <sstream>

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

string DepthFirst::TAG("DepthFirst");

bool DepthFirst::matches(const QueryPtr q1, const QueryPtr q2)
{
    cout << q1->mOriginal << " teil von " << q2->mOriginal << endl;
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
    vector<Literal*> H = getPotentialTargets(l, q2->literals());

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
            Log::d(TAG, string("h = ").append(cmToString(h)));
            ContainmentMapping gmap = createMapping(l,g);
            bool compat = mappingCompatible(h, gmap);
            if (compat) {
                if (L.empty()) {
                    return true;
                } else {
                    l = L.front();
                    vector<Literal*> H1 = getPotentialTargets(l, q2->literals());
                    vector<Literal*> Ln(L);
                    Ln.erase(std::find(Ln.begin(), Ln.end(), l));
                    ContainmentMapping hnew = mergeMappings(h, gmap);
                    R.push(StackElement(hnew, H1, Ln));
                }
            }
        }
    } while (!R.empty());

	return false;
}

vector<Literal*> DepthFirst::getPotentialTargets(Literal* l,
                                                 vector<Literal>& targets) const
{
    vector<Literal*> v;
    for(Literal& t : targets) {
        if (t.constantsCount() == l->constantsCount()
                && t.variableCount() == l->variableCount()) {
            v.push_back(&t);
        }
    }

    return v;
}

bool DepthFirst::mappingCompatible(ContainmentMapping h, ContainmentMapping g) const
{
    Containment c;

    for (pair<char, char> ph : h) {
        for (pair<char, char> pg : g) {
            if (c.isVariable(ph.first) && c.isVariable(pg.first)
                    && (ph.first == pg.first && ph.second != pg.second)) {
                // Prevents: a->b, a->c
                return false;
            } else if (c.isConstant(ph.first) && c.isConstant(pg.first)
                       && (ph.first != ph.second
                           || ph.first != pg.first || ph.second != pg.second)) {
                /*cout << ph.first << " != " << ph.second << " || " <<
                        ph.first << " != " << plg.first << " || " <<
                        ph.second << " != " << plg.second << endl;*/
                // Prevents: A->B
                return false;
            }
        }
    }

    return true;
}

ContainmentMapping DepthFirst::createMapping(Literal* l, Literal* g) const
{
    ContainmentMapping m;
    for (size_t i = 0; i < l->variables().size(); ++i) {
        pair<char,char> p;
        p.first = l->variables().at(i);
        p.second = g->variables().at(i);
        m.push_back(p);
    }

    for (size_t i = 0; i < l->constants().size(); ++i) {
        pair<char,char> p;
        p.first = l->constants().at(i);
        p.second = g->constants().at(i);
        m.push_back(p);
    }

    return m;
}

ContainmentMapping DepthFirst::mergeMappings(ContainmentMapping h, ContainmentMapping g) const
{
    if (h.empty()) {
        return g;
    } else if (g.empty()) {
        return h;
    }

    ContainmentMapping n;
    for (auto m : h) {
        n.push_back(m);
    }
    for (auto m : g) {
        n.push_back(m);
    }
    return n;
}

//==============================================================================
// NON-CLASS FUNCTIONS

std::ostream& operator<<(std::ostream& lhs, const ContainmentMapping& rhs)
{
    lhs << cmToString(rhs).c_str();
    return lhs;
}

string cmToString(const ContainmentMapping& cm)
{
    std::stringstream str;
    str << "[ ";
    for (pair<char, char> p : cm) {
        str << p.first << "->" << p.second << ", ";
    }
    str << "]";
    return str.str();
}
