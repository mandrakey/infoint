#include "attrmatch.hpp"
#include "model/relation.hpp"
#include "model/matcher.hpp"
#include "bmlib/log.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <utility>
using std::pair;

int main(int argc, char* argv[])
{
    if (argc != 4) {
        cout << "usage: attrmatch relation1 relation2 outfile" << endl;
        return 1;
    }
    
    AttrMatch am(argv[1], argv[2], argv[3]);
    
    cout << "Running with params:" << endl << 
            "inputFile1 = " << am.inputFile1() << endl << 
            "inputFile2 = " << am.inputFile2() << endl <<
            "outputFile = " << am.outputFile() << endl;

    //--------------------------------------------------------------------------
    // Load relations from input files and parse

    Matcher m;
    Relation r1(am.inputFile1());
    Relation r2(am.inputFile2());
    vector<pair<int, int> > matches;

    try {
        matches = m.match(&r1, &r2);
    } catch (const string& ex) {
        Log::e("main", string("Error while running matcher: ").append(ex));
        return -1;
    }

    for (pair<int, int> p : matches) {
        cout << "Match: " << p.first << " -> " << p.second << endl;
    }
    
    //--------------------------------------------------------------------------
    // Reading complete. Do the magic :)
}
