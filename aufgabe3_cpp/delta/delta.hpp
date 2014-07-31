#ifndef DELTA_HPP
#define DELTA_HPP

#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <queue>
#include <utility>

using std::string;
using std::map;
using std::vector;
using std::mutex;
using std::queue;
using std::pair;

namespace Delta
{
    //==========================================================================
    // RETURN CODES

    const int EXIT_OK = 0;
    const int EXIT_NO_INPUT_FILES = 1;
    const int EXIT_ERROR_WORKDIR = 11;
    const int EXIT_ERROR_OUTFILE = 12;

    //==========================================================================
    // CONFIGURATION

    const string WORKDIR("./");
//    const string WORKDIR("/local/II2014/uebung3/");
    const string OUTFILE(WORKDIR + "horst.txt");
    const int INPUTBUFFER_SIZE = 1000;
    const int NUM_THREADS = 8;

    //==========================================================================
    // VARIABLES

    extern map<string, map<string, int> > RESULTS;
    extern vector<string> DONE_FILES;
    extern queue<pair<string, string> > JOBQUEUE;
    extern int DELTA_COUNT;
    extern mutex RESULT_LOCK;
    extern mutex JOBQUEUE_LOCK;

    //==========================================================================
    // FUNCTIONS

    vector<string> getCsvFilesInDir(const string& path);
    bool resultsExist(const string& f1, const string& f2);
    string resultKey(const string& f1, const string& f2);
    void addResultsForEquals(const string& f1, const string& f2);
    void addResults(const string& f1, const string& f2, const map<string, int>& result);
    void addFromExistingResults(const string& f1, const string& f2, const string& key);
    string printResult(const string& f1, const string& f2, map<string, int>& result);
    void startCalculator();
}

#endif // DELTA_HPP
