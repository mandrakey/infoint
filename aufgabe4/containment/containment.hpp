#ifndef CONTAINMENT_HPP_
#define CONTAINMENT_HPP_

#include <memory>
#include <string>
using std::string;

#include "model/Query.hpp"

typedef std::unique_ptr<Query> QueryPtr;

class Containment
{
public:
    static const char* USAGE_TEXT;

    Containment();
    Containment(const string& inputFile, const string& outputFile, bool debug);

    const string& inputFile() const;
    const string& outputFile() const;
    bool debug() const;
    void setDebug(bool debug);

private:
    static string INPUT_FILE;
    static string OUTPUT_FILE;
    static bool DEBUG;
    static bool INITIALIZED;
};

#endif // CONTAINMENT_HPP
