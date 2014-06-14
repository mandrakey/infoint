#ifndef ENTRY_HPP_
#define ENTRY_HPP_

#include <vector>
using std::vector;
#include <iostream>
#include <string>
using std::string;

class Literal {
public:
    static string TAG;

    Literal(string& line);

	char name() const;
	vector<char> variables() const;
    int variableCount() const;
	vector<char> constants() const;
    int constantsCount() const;

private:
	char mName;
	vector<char> mVariables;
	vector<char> mConstants;
};

std::ostream& operator<<(std::ostream& lhs, const Literal& rhs);

#endif /* ENTRY_HPP_ */
