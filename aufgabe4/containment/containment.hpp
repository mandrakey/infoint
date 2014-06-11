#ifndef CONTAINMENT_HPP_
#define CONTAINMENT_HPP_

#include "model/Query.hpp"

#include <memory>
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;

typedef std::unique_ptr<Query> QueryPtr;

class Algorithm;

class Containment
{
public:
    static const char* USAGE_TEXT;

    Containment();
    Containment(const string& inputFile, const string& outputFile,
                string algorithm, bool debug) throw (const char*);

    const string& inputFile() const;
    const string& outputFile() const;
    shared_ptr<Algorithm> algorithm() const;
    bool debug() const;
    void setDebug(bool debug);

private:
    static string INPUT_FILE;
    static string OUTPUT_FILE;
    static shared_ptr<Algorithm> ALGORITHM;
    static bool DEBUG;
    static bool INITIALIZED;
};

#endif // CONTAINMENT_HPP
