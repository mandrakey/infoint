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

class FileParser
{
public:
    static const int ERR_EOF;

    static const int INPUT_BUFFER;

    FileParser(const string& filename);
    ~FileParser();

    pair<QueryPtr, QueryPtr> getQueryPair() throw (int);

private:
    ifstream mInputStream;
};

#endif // FILEPARSER_HPP
