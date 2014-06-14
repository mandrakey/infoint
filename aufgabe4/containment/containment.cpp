#include "containment.hpp"
#include "algo/Algorithm.hpp"
#include "algo/DepthFirst.hpp"

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;

const char* Containment::USAGE_TEXT = "Usage: containment -i infile -o outfile [-a algorithm -d]";
string Containment::INPUT_FILE = "";
string Containment::OUTPUT_FILE = "";
shared_ptr<Algorithm> Containment::ALGORITHM = 0;
bool Containment::DEBUG = false;
bool Containment::INITIALIZED = false;

Containment::Containment()
{
}

Containment::Containment(const string& inputFile, const string& outputFile,
                         string algorithm, bool debug) throw (const char*)
{
    if (INITIALIZED) {
        return;
    }

    INPUT_FILE = string(inputFile);
    OUTPUT_FILE = string(outputFile);
    DEBUG = debug;

    // Detect algorithm to use
    if (algorithm == "frozen-facts") {
        throw "Not implemented: FrozenFacts.";
    }
    else {
        ALGORITHM = shared_ptr<Algorithm>(new DepthFirst());
    }

    INITIALIZED = true;
}

bool Containment::isConstant(char c) const
{
    return (c >= 'A' && c < 'a');
}

bool Containment::isVariable(char c) const
{
    return (c >= 'a' && c <= 'z');
}

const string& Containment::inputFile() const
{
    return INPUT_FILE;
}

const string& Containment::outputFile() const
{
    return OUTPUT_FILE;
}

shared_ptr<Algorithm> Containment::algorithm() const
{
    return ALGORITHM;
}

bool Containment::debug() const
{
    return DEBUG;
}

void Containment::setDebug(bool debug)
{
    DEBUG = debug;
}
