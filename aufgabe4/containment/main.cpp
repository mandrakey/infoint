#include "containment.hpp"
#include "algo/DepthFirst.hpp"
#include "model/Query.hpp"
#include "model/Entry.hpp"
#include "model/FileParser.hpp"

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;

bool parseArgs(int argc, char* argv[]) throw (const char*);

int main(int argc, char* argv[])
{
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

    // Read input file
    while (true) {
        pair<QueryPtr, QueryPtr> p = parser.getQueryPair();
        if (p.first.get() == 0 || p.second.get() == 0) {
            break;
        }

        c.algorithm()->matches(std::move(p.first), std::move(p.second));
        // Note: p.first and p.second are not copied, but MOVED into
        // the algorithm method (necessary because of
        // unique_ptr<Query> (cannot be copied!)
    }

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
