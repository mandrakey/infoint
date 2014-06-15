#include "FileParser.hpp"
#include "containment.hpp"
#include "Query.hpp"

#include <string>
using std::string;
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;

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
    pair<QueryPtr, QueryPtr> p;
    if (mInputStream.eof()) {
        return p;
    }

    char in[4][INPUT_BUFFER];
    string lines[4];

    //--------------------------------------------------------------------------
    // Read 4 lines

    mInputStream.getline(in[0], INPUT_BUFFER); // Comment
    lines[0] = trim(string(in[0]));
    mInputStream.getline(in[1], INPUT_BUFFER); // q1
    lines[1] = trim(string(in[1]));
    mInputStream.getline(in[2], INPUT_BUFFER); // q2
    lines[2] = trim(string(in[2]));
    mInputStream.getline(in[3], INPUT_BUFFER); // Results
    lines[3] = trim(string(in[3]));

    if (lines[1] == "" || lines[2] == "") {
        return p;
    } /*else {
        cout << "1: \"" << lines[0] << "\"" << endl << "2: \"" << lines[1] << "\"" << endl << "3: \"" << lines[2] << "\"" << endl << "4: \"" << lines[3] << "\"" << endl;
    }*/

    p.first = QueryPtr(new Query(lines[1]));
    p.second = QueryPtr(new Query(lines[2]));

    return p;
}

bool FileParser::isOpen() const
{
    return mInputStream.is_open();
}

string FileParser::trim(const string& s)
{
    size_t start, end;
    start = s.find_first_not_of(" \t\r\n");

    string res = (start == string::npos) ? string() : s.substr(start);
    end = res.find_last_not_of(" \t\r\n");

    return (end == string::npos) ? string() : s.substr(0, end + 1);
}
