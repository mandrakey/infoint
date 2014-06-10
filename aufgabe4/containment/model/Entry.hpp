#ifndef ENTRY_HPP_
#define ENTRY_HPP_

#include <vector>
using std::vector;
#include <iostream>
#include <string>
using std::string;

class Entry {
public:
    Entry(string& line);

	char name() const;
	vector<char> variables() const;
	vector<char> constants() const;

private:
	char mName;
	vector<char> mVariables;
	vector<char> mConstants;
};

std::ostream& operator<<(std::ostream& lhs, const Entry& rhs);

#endif /* ENTRY_HPP_ */
