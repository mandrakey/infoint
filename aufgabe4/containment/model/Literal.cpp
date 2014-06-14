#include "Literal.hpp"
#include "containment.hpp"
#include "../bmlib/log.hpp"

#include <string>
using std::string;
#include <iostream>
using std::endl;
#include <sstream>
using std::stringstream;
#include <iterator>
#include <cstring>

string Literal::TAG("Literal");

Literal::Literal(string& line) :
	mName('-'), mVariables(), mConstants()
{   
    Containment c;
    mName = line[0];
    line = line.substr(line.find("(") + 1, line.find(")") - 2).c_str();
    char* tmp = new char[line.length() + 1];
    strncpy(tmp, line.c_str(), line.length());

    char* buf = strtok(tmp, ",");
    while (buf != 0) {
        if (c.isConstant(*buf)) {
            mConstants.push_back(*buf);
        } else if (c.isVariable(*buf)) {
            mVariables.push_back(*buf);
        } else {
            Log::w(TAG, string("Character '").append(buf).append("' not recognized."));
        }
        buf = strtok(0, ",");
    }

    delete buf;
    delete tmp;
}

char Literal::name() const
{
	return mName;
}

vector<char> Literal::variables() const
{
	return vector<char>(mVariables);
}

int Literal::variableCount() const
{
    return mVariables.size();
}

vector<char> Literal::constants() const
{
	return vector<char>(mConstants);
}

int Literal::constantsCount() const
{
    return mConstants.size();
}

std::ostream& operator<<(std::ostream& lhs, const Literal& rhs)
{
    lhs << "====" << endl << "Literal" << endl << "----" << endl <<
           "Name: " << rhs.name() << endl << endl <<
           "Variables: ";

    for (char c : rhs.variables()) {
        lhs << c << ", ";
    }

    lhs << endl << "Constants: ";
    for (char c : rhs.constants()) {
        lhs << c << ", ";
    }

    lhs << std::endl << "====";

    return lhs;
}

