#include "windowdeltacalculator.hpp"
#include "delta.hpp"

#include <iostream>
#include <utility>
#include <algorithm>
#include <string.h>

using std::string;
using std::pair;
using std::ifstream;
using std::cerr;
using std::endl;

const char* WindowDeltaCalculator::TAG = "WindowDeltaCalculator";
vector<char> WindowDeltaCalculator::DELIMS = { ',', ';', ' ', ':' };

WindowDeltaCalculator::WindowDeltaCalculator()
{
}

map<string, int> WindowDeltaCalculator::calculateDelta(const string& f1, const string& f2)
{
    results["UPDATE"] = 0;
    results["INSERT"] = 0;
    results["DELETE"] = 0;

    // Prepare readers
    ifstream in1, in2;
    in1.open(f1, std::ios_base::in);
    in2.open(f2, std::ios_base::in);

    // Load delimiters
    delim[0] = findDelimiter(in1);
    delim[1] = findDelimiter(in2);

    //--------------------------------------------------------------------------
    // Parse files

    fillInputBuffer(0, in1);
    fillInputBuffer(1, in2);
    while (!inputBuffer[0].empty() && !inputBuffer[1].empty()) {
        matchBuffers(inputBuffer[0], inputBuffer[1]);
        matchBuffers(inputBuffer[0], agingBuffer[1]);
        matchBuffers(inputBuffer[1], agingBuffer[0]);

        for (pair<long, Entry> p : inputBuffer[0]) {
            agingBuffer[0][p.second.mKey] = p.second;
        }
        for (pair<long, Entry> p : inputBuffer[1]) {
            agingBuffer[1][p.second.mKey] = p.second;
        }

        fillInputBuffer(0, in1);
        fillInputBuffer(1, in2);
    }

    results["DELETE"] = agingBuffer[0].size();
    results["INSERT"] = agingBuffer[1].size();

    in1.close();
    in2.close();

    return results;
}

void WindowDeltaCalculator::fillInputBuffer(int bufid, ifstream& f) throw (const string)
{
    if (!f.is_open()) {
        throw string("Cannot load data from non-open stream.");
    }

    char line[1024];
    EntryMap& buffer = inputBuffer[bufid];
    buffer.clear();

    f.getline(line, 1024);
    while (buffer.size() < Delta::INPUTBUFFER_SIZE && strlen(line) != 0) {
        string tmp(line);
        Entry e(tmp, delim[bufid]);
        buffer[e.mKey] = e;
        f.getline(line, 1024);
    }
}

void WindowDeltaCalculator::matchBuffers(EntryMap& buf1, EntryMap& buf2)
{
    int updates = 0;
    vector<long> toRemove;

    for (pair<long, Entry> p : buf1) {
        long& key = p.first;
        Entry& e = p.second;

        if (buf2.find(e.mKey) != buf2.end()) {
            if (e.mContent != buf2.at(e.mKey).mContent) {
//                cerr << e.mContent << " != " << buf2.at(e.mKey).mContent << endl;
                ++updates;
            }

            toRemove.push_back(key);
        }
    }

//    cerr << "bufsize: " << buf1.size();
    for (long key : toRemove) {
        buf1.erase(key);
        buf2.erase(key);
    }
//    cerr << " -> " << buf1.size() << endl;

    results["UPDATE"] = results["UPDATE"] + updates;
}

char WindowDeltaCalculator::findDelimiter(ifstream& stream) throw (const string)
{
    if (!stream.is_open()) {
        throw string("Can not determine delimiter from non-open file");
    }
    char buf[1024];
    stream.getline(buf, 1024);
    stream.seekg(0);
    string line(buf);

    bool quoted = false;

    for (unsigned int i = 0; i < line.size(); ++i) {
        if (line[i] == '"') {
            quoted = !quoted;
            continue;
        }

        if (std::find(DELIMS.begin(), DELIMS.end(), line[i]) != DELIMS.end()) {
            return line[i];
        }
    }

    throw string("Could not determine the delimiter");
}
