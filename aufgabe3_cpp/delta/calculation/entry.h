#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <functional>

class Entry
{
public:
    static std::hash<std::string> HASH;

    Entry();
    Entry(std::string& line, const char delim) throw (std::string);

    long mKey;
    long mContent;
};

#endif // ENTRY_H
