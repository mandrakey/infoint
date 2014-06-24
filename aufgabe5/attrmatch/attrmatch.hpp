#ifndef ATTRMATCH_HPP
#define ATTRMATCH_HPP

#include <string>

class AttrMatch
{
public:
    AttrMatch();
    AttrMatch(const char* in1, const char* in2, const char* out);
    
    const std::string& inputFile1() const;
    const std::string& inputFile2() const;
    const std::string& outputFile() const;

private:
    static std::string mInputFile1;
    static std::string mInputFile2;
    static std::string mOutputFile;
};

#endif
