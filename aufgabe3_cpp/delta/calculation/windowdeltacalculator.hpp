#ifndef WINDOWDELTACALCULATOR_HPP
#define WINDOWDELTACALCULATOR_HPP

#include "entry.h"

#include <string>
#include <map>
#include <fstream>
#include <vector>

using std::string;
using std::map;
using std::ifstream;
using std::vector;

typedef map<long,Entry> EntryMap;

class WindowDeltaCalculator
{
public:
    static const char* TAG;

    WindowDeltaCalculator();

    map<string, int> calculateDelta(const string& f1, const string& f2);

private:
    static vector<char> DELIMS;

    map<long, Entry> inputBuffer[2];
    map<long, Entry> agingBuffer[2];
    char delim[2];
    map<string, int> results;

    void fillInputBuffer(int bufid, ifstream& f) throw (const string);
    void matchBuffers(EntryMap& buf1, EntryMap& buf2);
    char findDelimiter(ifstream& stream) throw (const string);
};

#endif // WINDOWDELTACALCULATOR_HPP
