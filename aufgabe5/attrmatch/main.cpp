#include "attrmatch.hpp"
#include "model/relation.hpp"
#include "model/tuple.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <string>
using std::string;

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
    
    Relation r("Test");
    r.addTuple(new Tuple("a;b;\"c und ; so\";d"));
    
    cout << "Got " << r.tuples().size() << " tuples:" << endl;
    for (auto t_ptr : r.tuples()) {
        for (auto t : t_ptr->attributes()) {
            cout << "Attr: " << t << endl;
        }
    }
}
