#include "attrmatch.hpp"
#include "model/relation.hpp"
#include "model/tuple.hpp"
#include "bmlib/log.hpp"

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

    //--------------------------------------------------------------------------
    // Load relations from input files and parse
    // Note: Tuple::Tuple manages parsing of tuple lines with internal
    // private method Tuple::parseLine()

    // File 1
    ifstream in;
    char buf[255];

    in.open(am.inputFile1(), std::ios_base::in);
    if (!in.is_open()) {
        Log::e("main", string("Failed to open input file 1: ").append(am.inputFile1()));
        return -1;
    }

    Relation r1(am.inputFile1());
    while (!in.eof()) {
        in.getline(buf, 255);
        r1.addTuple(new Tuple(buf));
    }
    in.close();

    // File 2
    in.open(am.inputFile2(), std::ios_base::in);
    if (!in.is_open()) {
        Log::e("main", string("Failed to open input file 2: ").append(am.inputFile2()));
        return -1;
    }

    Relation r2(am.inputFile2());
    while (!in.eof()) {
        in.getline(buf, 255);
        r2.addTuple(new Tuple(buf));
    }
    in.close();
    
    //--------------------------------------------------------------------------
    // Reading complete. Do the magic :)

    // todo: remove demo output
    cout << "Got " << r1.tuples().size() << " tuples from R1:" << endl;
    int tupleNr = 1;
    for (auto t_ptr : r1.tuples()) {
        cout << "Tupel " << tupleNr++ << endl;
        for (const string& t : t_ptr->attributes()) {
            cout << "Attr: " << t << endl;
        }
        cout << endl;
    }
}
