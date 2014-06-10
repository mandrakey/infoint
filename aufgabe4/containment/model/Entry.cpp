#include "Entry.hpp"

#include <string>
using std::string;
#include <iostream>
using std::endl;
#include <sstream>
using std::stringstream;
#include <iterator>
#include <cstring>

Entry::Entry(string& line) :
	mName('-'), mVariables(), mConstants()
{   
    mName = line[0];
    line = line.substr(line.find("(") + 1, line.find(")") - 2).c_str();
    char* tmp = new char[line.length() + 1];
    strncpy(tmp, line.c_str(), line.length());

    char* buf = strtok(tmp, ",");
    while (buf != 0) {
        if (*buf <= 'a') {
            mConstants.push_back(*buf);
        } else {
            mVariables.push_back(*buf);
        }
        buf = strtok(0, ",");
    }

    delete buf;
    delete tmp;
}

char Entry::name() const
{
	return mName;
}

vector<char> Entry::variables() const
{
	return vector<char>(mVariables);
}

vector<char> Entry::constants() const
{
	return vector<char>(mConstants);
}

std::ostream& operator<<(std::ostream& lhs, const Entry& rhs)
{
    lhs << "====" << endl << "Entry" << endl << "----" << endl <<
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

