#ifndef LOG_HPP
#define LOG_HPP

#include <vector>
using std::vector;
#include <iostream>
#include <string>
using std::string;

class Log
{
public:
    static void i(const string& tag, const string& msg);
    static void e(const string& tag, const string& msg);
    static void w(const string& tag, const string& msg);
    static void d(const string& tag, const string& msg);

    static bool debug();
    static void setDebug(bool debug);

private:
    static void print(std::ostream& stream, const char* type,
                      const string& tag,
                      const string& msg);

    static bool mDebug;
};

#endif // LOG_HPP
