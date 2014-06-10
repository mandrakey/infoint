#include "containment.hpp"

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;

const char* Containment::USAGE_TEXT = "Usage: containment -i infile -o outfile [-d(ebug)]";
string Containment::INPUT_FILE = "";
string Containment::OUTPUT_FILE = "";
bool Containment::DEBUG = false;
bool Containment::INITIALIZED = false;

Containment::Containment()
{
}

Containment::Containment(const string& inputFile, const string& outputFile, bool debug)
{
    if (INITIALIZED) {
        return;
    }

    INPUT_FILE = string(inputFile);
    OUTPUT_FILE = string(outputFile);
    DEBUG = debug;
    INITIALIZED = true;
}

const string& Containment::inputFile() const
{
    return INPUT_FILE;
}

const string& Containment::outputFile() const
{
    return OUTPUT_FILE;
}

bool Containment::debug() const
{
    return DEBUG;
}

void Containment::setDebug(bool debug)
{
    DEBUG = debug;
}
