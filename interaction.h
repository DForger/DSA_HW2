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

    Interaction( std::istringstream &ss);

    ~Interaction();

    Interaction& operator=(const Interaction &rhs){
        this->click = rhs.click;
        this->impression = rhs.impression;
        this->displayURL = rhs.displayURL;
        this->adID = rhs.adID;
        this->advertiserID = rhs.advertiserID;
        this->depth = rhs.depth;
        this->position = rhs.position;
        this->queryID = rhs.queryID;
        this->keywordID = rhs.keywordID;
        this->titleID = rhs.titleID;
        this->descriptionID = rhs.descriptionID;
        this->userID = rhs.userID;
    }

    bool operator==(const Interaction &rhs){
        if(this->click != rhs.click){
            return false;
        }
        if(this->impression != rhs.impression){
            return false;
        }
        if(this->adID != rhs.adID){
            return false;
        }
        if(this->advertiserID != rhs.advertiserID){
            return false;
        }
        if(this->depth != rhs.depth){
            return false;
        }
        if(this->position != rhs.position){
            return false;
        }
        if(this->queryID != rhs.queryID){
            return false;
        }
        if(this->keywordID != rhs.keywordID){
            return false;
        }
        if(this->titleID != rhs.titleID){
            return false;
        }
        if(this->descriptionID != rhs.descriptionID){
            return false;
        }
        if(this->userID != rhs.userID){
            return false;
        }
        if(this->displayURL != rhs.displayURL){
            return false;
        }
        return true;
    }

    //data member
    short click;
    short impression;
    string displayURL;
    unsigned long adID;
    unsigned long advertiserID;
    short depth;
    short position;
    size_t queryID;
    size_t keywordID;
    unsigned long titleID;
    size_t descriptionID;
    unsigned long userID;

};

#endif // INTERACTION_H
