#include "Query.hpp"

Query::Query(const string& line) :
	mRelations(), mEntries()
{
	throw "Not implemented.";
}

vector<char> Query::relations() const
{
	return vector<char>(mRelations);
}

vector<Entry>& Query::entries()
{
	return mEntries;
}
