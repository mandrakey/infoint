#ifndef ENTRY_HPP_
#define ENTRY_HPP_

#include <vector>
using std::vector;

class string;

class Entry {
public:
	Entry(const string& line);

	char name() const;
	vector<char> variables() const;
	vector<char> constants() const;

private:
	char mName;
	vector<char> mVariables;
	vector<char> mConstants;
};

#endif /* ENTRY_HPP_ */
