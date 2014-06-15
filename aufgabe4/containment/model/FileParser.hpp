#ifndef FILEPARSER_HPP_
#define FILEPARSER_HPP_

#include "Query.hpp"
#include "containment.hpp"

#include <fstream>
using std::ifstream;
#include <utility>
using std::pair;
#include <memory>
using std::unique_ptr;
#include <string>
using std::string;

class FileParser
{
public:
    static const int ERR_EOF;

    static const int INPUT_BUFFER;

    FileParser(const string& filename);
    ~FileParser();

    pair<QueryPtr, QueryPtr> getQueryPair() throw (int);

    bool isOpen() const;

private:
    ifstream mInputStream;

    string trim(const string& s);
};

#endif // FILEPARSER_HPP
