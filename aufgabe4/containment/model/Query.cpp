#include "Query.hpp"
#include "Literal.hpp"

#include <algorithm>
#include <sstream>
using std::stringstream;
#include <iostream>
using std::endl;

Query::Query(const string& line) :
    mRelations(), mLiterals()
{
    string l = line.substr(line.find('-') + 1);
    mOriginal = l;

    bool inEntry = false;
    stringstream tmp;
    for (char c : l) {
        if (!inEntry) {
            switch (c) {
            case ',':
            case '.':
                continue;
                break;
            case '(':
                inEntry = true;
                tmp << c;
                break;
            default:
                tmp << c;
                mRelations.push_back(c);
            }
        } else {
            tmp << c;
            if (c == ')') {
                inEntry = false;
                string s = tmp.str();
                Literal e(s);
                mLiterals.push_back(e);
                tmp.str("");
            }
        }
    }
}

vector<char> Query::relations() const
{
	return vector<char>(mRelations);
}

vector<Literal>& Query::literals()
{
    return mLiterals;
}

std::ostream& operator<<(std::ostream& lhs, Query& rhs)
{
    lhs << "========" << endl << "Query" << endl << "--------" << endl;
    lhs << "Relations: ";
    for (char c : rhs.relations()) {
        lhs << c << ", ";
    }
    lhs << endl << endl << "Entries:" << endl;
    for (const Literal& e : rhs.literals()) {
        lhs << e << endl << endl;
    }
    lhs << "========";

    return lhs;
}
