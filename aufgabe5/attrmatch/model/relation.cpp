#include "relation.hpp"
#include "bmlib/log.hpp"

#include <algorithm>
#include <cstring>
#include <sstream>
using std::stringstream;
#include <utility>
using std::pair;

using std::string;
using std::vector;
using std::map;
using std::shared_ptr;
using std::ostream;
using std::cout;
using std::endl;

using boost::regex;

const char* Relation::TAG = "Relation";
const char Relation::TUPLE_SEPARATOR = ';';
const regex Relation::RX_ID("[0-9]{1,3}[a-zA-Z]{2,3}[0-9]{2,3}");
const regex Relation::RX_DATE("([0-9]{1,2}\\-)?[a-zA-Z]{2,3}\\-[0-9]{2}");
const regex Relation::RX_DATE_FOR_THE_STUPID_IDIOTS_WHO_CANNOT_HANDLE_EXCEL("[0-9]{1,2}\\. [a-zA-Z]{2,3} [0-9]{2}");
const regex Relation::RX_RANGE("[0-9\\.]+ ?(\\-|to) ?[0-9\\.]+");
const regex Relation::RX_DOUBLE("\\-?[0-9]+\\.[0-9]+");
const regex Relation::RX_INTEGER("\\-?[0-9]+");

Relation::Relation(const char* filename) :
    mFileName(filename), mAttributes(), mAttributeTypes()
{
}

Relation::Relation(const string& filename) :
    mFileName(filename), mAttributes(), mAttributeTypes()
{
}

void Relation::compressAttributeTypes()
{
    map<AttributeType, int> typeCounts;
    for (pair<int, vector<AttributeType> > p : mAttributeTypes) {
        vector<AttributeType>& v = p.second;
        unsigned int sum = 0;

        for (AttributeType at : v) {
            int count = std::count(v.begin(), v.end(), at);
            sum += count;

            typeCounts[at] = count;
            if (sum >= v.size()) {
                break;
            }
        }

        auto it = typeCounts.begin();
        AttributeType max = (*it).first;
        int maxCount = (*it).second;
        while (it != typeCounts.end()) {
            if (maxCount < (*it).second) {
                max = (*it).first;
            }
            ++it;
        }

        // Now, clear v and set max as single element
        v.clear();
        v.push_back(max);
    }
}

void Relation::buildAttributeBlocks()
{
    // todo: implement Relation::buildAttributeBlocks
}

void Relation::sortAttributes(int index)
{
    std::sort(mAttributes[index].begin(), mAttributes[index].end());
}

void Relation::parseLine(const char* line)
{
    if (!line) {
        return;
    }

    bool literal = false;
    vector<string> tokens;
    stringstream token;
    int attributeIndex = 0;

    // todo: Refactor tokens.push_back into method
    // -> enables calls to Matcher's event methods
    // !important: count attribute number!

    for (size_t i = 0; i < strlen(line); ++i) {
        char c = line[i];
        string tmp(&c);
        if (c == 13 || c == 10) {
            continue;
        }

        if (literal) {
            if (c == '\"') {
                literal = false;
            } else {
                token << c;
            }
        } else {
            if (c == TUPLE_SEPARATOR) {
                addAttribute(attributeIndex, token.str());
                token.str("");
            } else if (c == '\"') {
//                token << c;
                literal = true;
            } else {
                token << c;
            }
        }
    }

    cout << "\"" << token.str() << "\"" << endl;
    if (!token.str().empty()) {
        addAttribute(attributeIndex, token.str());
    }
}

void Relation::addAttribute(int& index, const string& attribute)
{
//    cout << "Adding " << attribute << "(" << index << ")" << endl;
    string attr(attribute);
    attr.erase(0, attr.find_first_not_of(' '));
    attr.erase(attr.find_last_not_of(' ') + 1);

    // Delete some chars we do not like...
    unsigned long int pos = 0;
    while ((pos = attr.find_first_of("\\r")) != string::npos) {
        attr.replace(pos, 1, "");
    }
    while ((pos = attr.find_first_of("\\n")) != string::npos) {
        attr.replace(pos, 1, "");
    }

    mAttributes[index].push_back(attr);
    mAttributeTypes[index].push_back(getType(attr));
    ++index;
}

AttributeType Relation::getType(const string& attribute)
{
    // Is it an ID?
    const char* exp = attribute.c_str();
//    Log::d(TAG, string("Match '").append(attribute).append("' as ... "));
    if (boost::regex_match(exp, RX_ID)) {
//        Log::d(TAG, "ID\n");
        return AttributeType::ID;
    }

    // Is it a date value?
    if (boost::regex_match(exp, RX_DATE) ||
            boost::regex_match(exp, RX_DATE_FOR_THE_STUPID_IDIOTS_WHO_CANNOT_HANDLE_EXCEL)) {
//        Log::d(TAG, "DATE\n");
        return AttributeType::DATE;
    }

    // Is it a range value?
    if (boost::regex_match(exp, RX_RANGE)) {
//        Log::d(TAG, "RANGE\n");
        return AttributeType::RANGE;
    }

    // Is it a double value?
    if (boost::regex_match(exp, RX_DOUBLE)) {
//        Log::d(TAG, "DOUBLE\n");
        return AttributeType::DOUBLE;
    }

    // Is it an integer value?
    if (boost::regex_match(exp, RX_INTEGER)) {
//        Log::d(TAG, "INTEGER\n");
        return AttributeType::INTEGER;
    }

    // Ok, must be a string. Long?
    if (attribute.size() > 40) {
//        Log::d(TAG, "LONGSTRING\n");
        return AttributeType::LONGSTRING;
    } else {
//        Log::d(TAG, "STRING\n");
        return AttributeType::STRING;
    }
}

//==============================================================================
// GETTER / SETTER

string Relation::fileName() const
{
    return mFileName;
}

// todo: refactor addTuple to do tuple parsing; remote Tuple class
// build column based attribute lists -> easier for later
// access
void Relation::addTuple(const char* line)
{
    if (line) {
        parseLine(line);
    }
}

const map<int, vector<string> >& Relation::attributes() const
{
    return mAttributes;
}

const vector<string>* Relation::attribute(int index) const throw (const string)
{
    if (mAttributes.find(index) == mAttributes.end()) {
        throw string("Attribute index out of range");
    }

    return &mAttributes.at(index);
}

map<int, vector<AttributeType> >& Relation::attributeTypes()
{
    return mAttributeTypes;
}

const map<int, vector<AttributeType> >& Relation::attributeTypes_const() const
{
    return mAttributeTypes;
}

ostream& operator<<(ostream& lhs, const Relation& rhs)
{
    const map<int, vector<string> >& attrs = rhs.attributes();
    const map<int, vector<AttributeType> >& types = rhs.attributeTypes_const();

    for (pair<int, vector<string> > p : attrs) {
        cout << "(" << AttributeTypeToString(types.at(p.first).at(0)) << ") " <<
                "#" << p.first << " [ ";
        for (string& s : p.second) {
            cout << s << ", ";
        }
        cout << "];" << endl << endl;
    }

    return lhs;
}
