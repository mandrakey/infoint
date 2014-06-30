#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <string>
#include <vector>

typedef std::vector<std::string> AttributeVector;

class Tuple
{
public:
    static const char TUPLE_SEPARATOR;

    Tuple(const char* line);
    
    const std::vector<std::string>& attributes() const;
    const std::string& attribute(unsigned int index) const throw (const char*);
    
private:
    std::vector<std::string> mAttributes;

    void parseLine(const char* line);
};

#endif
