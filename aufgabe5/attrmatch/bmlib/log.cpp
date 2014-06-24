#include "log.hpp"

#include <iostream>

bool Log::mDebug = false;

void Log::i(const string& tag, const string& msg)
{
    print(std::cout, "I", tag, msg);
}

void Log::e(const string& tag, const string& msg)
{
    print(std::cerr, "E", tag, msg);
}

void Log::w(const string& tag, const string& msg)
{
    print(std::cout, "W", tag, msg);
}

void Log::d(const string& tag, const string& msg)
{
    if (mDebug) {
        print(std::cout, "D", tag, msg);
    }
}

bool Log::debug()
{
    return mDebug;
}

void Log::setDebug(bool debug)
{
    mDebug = debug;
}

void Log::print(std::ostream& stream, const char* type,
                const string& tag,
                const string& msg)
{
    stream << type << "\t" << tag << "\t" << msg << std::endl;
}
