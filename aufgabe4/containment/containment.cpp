#include "algo/DepthFirst.hpp"
#include "model/Query.hpp"
#include "model/Entry.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;

class Algorithm;

int main()
{
    string a = "q(b)-r(b,b),s(c,c),t(i,i).";
    Query q(a);
    cout << q << endl;
	return 0;
}
