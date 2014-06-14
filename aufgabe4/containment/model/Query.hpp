#ifndef QUERY_HPP_
#define QUERY_HPP_

#include "Literal.hpp"

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>

class Query {
private:
	vector<char> mRelations;
    vector<Literal> mLiterals;

public:
	Query(const string& line);

	vector<char> relations() const;
    vector<Literal>& literals();

    string mOriginal;
};

std::ostream& operator<<(std::ostream& lhs, Query& rhs);

#endif /* QUERY_HPP_ */
