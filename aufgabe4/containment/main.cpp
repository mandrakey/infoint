#include "containment.hpp"
#include "algo/DepthFirst.hpp"
#include "model/Query.hpp"
#include "model/Literal.hpp"
#include "model/FileParser.hpp"
#include "bmlib/log.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;
#include <string>
using std::string;
#include <vector>
using std::vector;

void test();
bool parseArgs(int argc, char* argv[]) throw (const char*);

int main(int argc, char* argv[])
{
//    Log::setDebug(true);
//    test();
//    return -99;

    try {
        parseArgs(argc, argv);
    } catch (const char* s) {
        cout << s << endl;
        return -1;
    }

    Containment c;

    // Check input file
    FileParser parser(c.inputFile());
    if (!parser.isOpen()) {
        cout << "Failed to open input file at '" << c.inputFile() << "'." << endl;
        return -1;
    }

    // Prepare writing output file
    ofstream out(c.outputFile());
    if (!out.is_open()) {
        Log::e(string("main"), string("Failed to open output file at '")
               .append(c.outputFile()).append("'."));
        return -1;
    }

    // Read input file
    int qcount = 0;
    while (true) {
        ++qcount;
        pair<QueryPtr, QueryPtr> p = parser.getQueryPair();
        if (p.first.get() == 0 || p.second.get() == 0) {
            break;
        }

        bool match = c.algorithm()->matches(std::move(p.first), std::move(p.second));
//        cout << "Match? " << match << endl;
        out << ((match) ? "true" : "false") << endl;
        // Note: p.first and p.second are not copied, but MOVED into
        // the algorithm method (necessary because of
        // unique_ptr<Query> (cannot be copied!)
    }

    out << endl;
    out.close();
    cout << "Processed " << qcount << " Queries." << endl;

    return 0;
}

bool parseArgs(int argc, char* argv[]) throw (const char*)
{
    if (argc < 5) {
        throw Containment::USAGE_TEXT;
    }

    string inputFile;
    string outputFile;
    string algorithm;
    bool debug = false;

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            continue;
        }

        switch (argv[i][1]) {
        case 'i':
            if (argc < i+1 || argv[i+1][0] == '-') {
                throw Containment::USAGE_TEXT;
            }
            inputFile = string(argv[i+1]);
            break;
        case 'o':
            if (argc < i+1 || argv[i+1][0] == '-') {
                throw Containment::USAGE_TEXT;
            }
            outputFile = string(argv[i+1]);
            break;
        case 'a':
            algorithm = (argc > i && argv[i+1][0] != '-')
                    ? string(argv[i+1]) : "";
            break;
        case 'd':
            debug = true;
            break;
        default:
            throw Containment::USAGE_TEXT;
        }
    }

    const Containment c(inputFile, outputFile, algorithm, debug);
    return true;
}

void test()
{
    Log::d("main", "Test output");
    DepthFirst df;
    string l1("a(a,b,C)");
    string l2("b(x,y,C)");
    string l3("c(e,b,C)");
    string l4("d(d,w,C)");
    Literal a(l1);
    Literal b(l2);
    Literal c(l3);
    Literal d(l4);

    ContainmentMapping m = df.createMapping(&a,&b);
    ContainmentMapping m2 = df.createMapping(&c, &d);
    cout << "Map 1: " << m << endl;
    cout << "Map 2: " << m2 << endl;

    cout << "Compatible? " << df.mappingCompatible(m, m2) << endl;
}
