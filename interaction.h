#ifndef INTERACTION_H
#define INTERACTION_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Interaction
{
public:
    Interaction();

    Interaction(const std::istringstream &ss);

    ~Interaction();

    //data member
    size_t click;
    size_t impression;
    string displayURL;
    unsigned long adID;
    unsigned long advertiserID;
    size_t depth;
    size_t position;
    unsigned long queryID;
    unsigned long titleID;
    unsigned long descriptionID;
    unsigned long userID;

};

#endif // INTERACTION_H
