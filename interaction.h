#ifndef INTERACTION_H
#define INTERACTION_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Interaction
{
    Interaction();

    Interaction( std::istringstream &ss);

    Interaction(char *start, char *end);

    ~Interaction();

    Interaction& operator=(const Interaction &rhs);

    bool operator==(const Interaction &rhs) const;

    void init(char *start, char *end);

    inline size_t str2num(char *start, char *end){
        char *ptr = start;
        size_t value = 0;
        while(ptr != end){
            value = value*10 +(*ptr++ - '0');
        }
        return value;
    }

    inline size_t str2num(const char *start, int length){
        size_t value = 0;
        int nCnt = 0;
        while(nCnt < length){
            value = value*10 + (start[nCnt] - '0');
            ++nCnt;
        }
        return value;
    }

    //data member
    short click;
    short impression;
//    string displayURL;
    size_t displayURL_H;
    size_t displayURL_L;
    size_t adID;
    size_t advertiserID;
    short depth;
    short position;
    size_t queryID;
    size_t keywordID;
    size_t titleID;
    size_t descriptionID;
    size_t userID;
};

#endif // INTERACTION_H
