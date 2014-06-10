#ifndef QUERY_HPP_
#define QUERY_HPP_

#include "Entry.hpp"

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>

class Query {
private:
	vector<char> mRelations;
	vector<Entry> mEntries;

public:
	Query(const string& line);

	vector<char> relations() const;
    vector<Entry>& entries();
};

std::ostream& operator<<(std::ostream& lhs, Query& rhs);

#endif /* QUERY_HPP_ */
