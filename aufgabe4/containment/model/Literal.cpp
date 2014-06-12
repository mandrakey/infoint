#include "Literal.hpp"

#include <string>
using std::string;
#include <iostream>
using std::endl;
#include <sstream>
using std::stringstream;
#include <iterator>
#include <cstring>

Literal::Literal(string& line) :
	mName('-'), mVariables(), mConstants()
{   
    mName = line[0];
    line = line.substr(line.find("(") + 1, line.find(")") - 2).c_str();
    char* tmp = new char[line.length() + 1];
    strncpy(tmp, line.c_str(), line.length());

    char* buf = strtok(tmp, ",");
    while (buf != 0) {
        if (*buf >= 'a') {
            mConstants.push_back(*buf);
        } else {
            mVariables.push_back(*buf);
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

