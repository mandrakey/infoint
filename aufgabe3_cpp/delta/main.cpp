#include "delta.hpp"
#include "calculation/windowdeltacalculator.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <map>
#include <mutex>
#include <sstream>
#include <cmath>
#include <thread>
#include <queue>
#include <dirent.h>
#include <error.h>
#include <string.h>
#include <unistd.h>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::pair;
using std::map;
using std::stringstream;
using std::thread;
using std::queue;

int main()
{
    vector<string> files = Delta::getCsvFilesInDir(Delta::WORKDIR);
    if (files.size() == 0) {
        cerr << "No input files. Exiting." << endl;
        return Delta::EXIT_NO_INPUT_FILES;
    }

    std::sort(files.begin(), files.end());

    //--------------------------------------------------------------------------
    // Run delta computation for all files

    vector<pair<string, string> > resultsToCopy;
    for (const string& f1 : files) {
        for (const string& f2 : files) {

            if (Delta::resultsExist(f1, f2)) {
                cout << "Results for " << Delta::resultKey(f1, f2) << " already done." << endl;
                resultsToCopy.push_back(pair<string, string>(f1, f2));
                continue;
            }

            if (f1 == f2) {
                Delta::addResultsForEquals(f1, f2);
            } else {
                //executeDeltaCalculatorThread
                Delta::JOBQUEUE.push(pair<string, string>(f1, f2));
                ++Delta::DELTA_COUNT;
            }

            Delta::DONE_FILES.push_back(Delta::resultKey(f1, f2));

        }
    }

    // Run the threads
    vector<thread*> threads;
    for (int i = 0; i < Delta::NUM_THREADS; ++i) {
        thread* t = new thread(Delta::startCalculator);
        threads.push_back(t);
    }
    for (thread* t : threads) {
        t->join();
    }

    // Copy missing results
    for (pair<string, string> p : resultsToCopy) {
        Delta::addFromExistingResults(p.first, p.second, Delta::resultKey(p.second, p.first));
    }

    // Print results
    for (string& f1 : files) {
        for (string& f2 : files) {
            cout << Delta::printResult(f1, f2, Delta::RESULTS[Delta::resultKey(f1, f2)]) << endl;
        }
    }

    cout << "Calculated " << Delta::DELTA_COUNT << " out of " <<
            pow(files.size(), 2) << " deltas." << endl;
}

//==============================================================================
// UTILITY FUNCTIONS

vector<string> Delta::getCsvFilesInDir(const string& path)
{
    if (path.empty()) {
        throw string("Cannot use empty path.");
    }

    DIR* d = opendir(path.c_str());
    if (!d) {
        perror("opendir");
        return vector<string>();
    }

    struct dirent* de;
    vector<string> files;
    while ((de = readdir(d))) {
        char* pos = strstr(de->d_name, ".csv");
        if (pos == (de->d_name + strlen(de->d_name) - 4)) {
            files.push_back(string(Delta::WORKDIR).append(string(de->d_name)));
        }
    }

    return files;
}

bool Delta::resultsExist(const std::string& f1, const std::string& f2)
{
    auto pos = std::find(Delta::DONE_FILES.begin(), Delta::DONE_FILES.end(), Delta::resultKey(f1, f2));
    return (pos != Delta::DONE_FILES.end()) ? true : false;
}

string Delta::resultKey(const string& f1, const string& f2)
{
    vector<string> tmp = { f1, f2 };
    std::sort(tmp.begin(), tmp.end());
    return tmp[0].append(",").append(tmp[1]);
}

void Delta::addResultsForEquals(const string& f1, const string& f2)
{
    map<string, int> res;
    res["UPDATE"] = 0;
    res["INSERT"] = 0;
    res["DELETE"] = 0;
    Delta::addResults(f1, f2, res);
}

void Delta::addResults(const string& f1, const string& f2, const map<string, int>& result)
{
    Delta::RESULT_LOCK.lock();
    Delta::RESULTS[Delta::resultKey(f1, f2)] = map<string, int>(result);
    Delta::RESULT_LOCK.unlock();
}

void Delta::addFromExistingResults(const string& f1, const string& f2, const string& key)
{
    const map<string, int>& original = Delta::RESULTS.at(key);
    map<string, int> add(original);
    Delta::RESULTS[Delta::resultKey(f1, f2)] = add;
}

string Delta::printResult(const string& f1, const string& f2, map<string, int>& result)
{
    string fn1 = f1.substr(0, f1.find(".csv"));
    string fn2 = f2.substr(0, f2.find(".csv"));

    stringstream s;
    s << fn1 << "," << fn2 << "," <<
         result["INSERT"] << "," <<
         result["DELETE"] << "," <<
         result["UPDATE"];
    return s.str();
}

void Delta::startCalculator()
{
    while (1) {
        Delta::JOBQUEUE_LOCK.lock();
        if (Delta::JOBQUEUE.size() == 0) {
            Delta::JOBQUEUE_LOCK.unlock();
            break;
        }

        pair<string, string>& task = Delta::JOBQUEUE.front();
        Delta::JOBQUEUE.pop();
        Delta::JOBQUEUE_LOCK.unlock();

        WindowDeltaCalculator calc;
        map<string, int> res = calc.calculateDelta(task.first, task.second);
        Delta::addResults(task.first, task.second, res);
    }
}
