#include "Entry.hpp"

#include <string>

Entry::Entry(const string& line) :
	mName('-'), mVariables(), mConstants()
{
	throw "Not implemented";
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

