#include "FileParser.hpp"
#include "containment.hpp"
#include "Query.hpp"

#include <string>
using std::string;
#include <fstream>

const int FileParser::ERR_EOF = 0;

const int FileParser::INPUT_BUFFER = 1024;

FileParser::FileParser(const string& filename) :
    mInputStream(filename, std::ios_base::in)
{
}

FileParser::~FileParser()
{
    if (mInputStream.is_open()) {
        mInputStream.close();
    }
}

pair<QueryPtr, QueryPtr> FileParser::getQueryPair() throw (int)
{
    if (mInputStream.eof()) {
        throw ERR_EOF;
    }

    pair<QueryPtr, QueryPtr> p;
    char in[INPUT_BUFFER];

    //--------------------------------------------------------------------------
    // Read 4 lines

    // 1. Comment
    mInputStream.getline(in, INPUT_BUFFER);

    // 2. + 3. : Two queries
    Query* qtmp = 0;
    mInputStream.getline(in, INPUT_BUFFER);
    qtmp = new Query(string(in));
    p.first = QueryPtr(qtmp);

    mInputStream.getline(in, INPUT_BUFFER);
    qtmp = new Query(string(in));
    p.second = QueryPtr(qtmp);

    // todo: Skip 4th line...
    mInputStream.getline(in, INPUT_BUFFER);

    return p;
}
