#include "attrmatch.hpp"

using std::string;

string AttrMatch::mInputFile1;
string AttrMatch::mInputFile2;
string AttrMatch::mOutputFile;

AttrMatch::AttrMatch()
{
}

AttrMatch::AttrMatch(const char* in1, const char* in2, const char* out)
{
    mInputFile1 = string(in1);
    mInputFile2 = string(in2);
    mOutputFile = string(out);
}

const string& AttrMatch::inputFile1() const
{
    return mInputFile1;
}

const string& AttrMatch::inputFile2() const
{
    return mInputFile2;
}

const string& AttrMatch::outputFile() const
{
    return mOutputFile;
}
