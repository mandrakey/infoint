#include "entry.h"

#include <iostream>
#include <algorithm>
#include <cstring>

using std::string;
using std::hash;
using std::cerr;
using std::endl;

hash<string> Entry::HASH;

Entry::Entry() :
    mKey(0), mContent(0)
{
}

Entry::Entry(string& line, const char delim) throw (string) :
    mKey(0), mContent(0)
{
    if (std::count(line.begin(), line.end(), delim) < 2) {
        throw string("Line '").append(line).append("' cannot be parsed with delimiter '")
                .append(string(&delim)).append("'.");
    }

    if (delim != ';') {
        std::replace(line.begin(), line.end(), delim, ';');
    }
    std::remove(line.begin(), line.end(), '\"');

    mKey = atol(line.substr(0, line.find_first_of(';')).c_str());
    mContent = HASH(line);

//    if (mKey == 176544849057) {
//        cerr << "Entry from '" << line << "': " << endl <<
//                "{ key: " << mKey << ", content: " << mContent << " }" << endl << endl;
//    }
}
