#include "interaction.h"

Interaction::Interaction()
{

}

Interaction::Interaction( istringstream &ss)
{
    ss >> click;
    ss >> impression;
//    ss >> displayURL;
    ss >> adID;
    ss >> advertiserID;
    ss >> depth;
    ss >> position;
    ss >> queryID;
    ss >> keywordID;
    ss >> titleID;
    ss >> descriptionID;
    ss >> userID;

}

Interaction::Interaction(char *start, char *end)
{
    short sTabPos[12];
    short sTabCnt = 0;
    short sCharCnt = 0;
    char *ptr = start;
    while(ptr != end){
        if(*ptr == '\t'){
            sTabPos[sTabCnt] = sCharCnt;
            ++sTabCnt;
            ++sCharCnt;
        }else{
            ++sCharCnt;
        }
        ++ptr;
    }
    sTabPos[11] = sCharCnt;

    click = str2num(start, start+sTabPos[0]);
    impression = str2num(start+sTabPos[0]+1, start+sTabPos[1]);
    //    displayURL = string(start+sTabPos[1]+1, start+sTabPos[2]);
    displayURL_H = str2num(start+sTabPos[1]+1, start+sTabPos[2]-9);
    displayURL_L = str2num(start+sTabPos[2]-9, start+sTabPos[2]);
    adID = str2num(start+sTabPos[2]+1, start+sTabPos[3]);
    advertiserID = str2num(start+sTabPos[3]+1, start+sTabPos[4]);
    depth = str2num(start+sTabPos[4]+1, start+sTabPos[5]);
    position = str2num(start+sTabPos[5]+1, start+sTabPos[6]);
    queryID = str2num(start+sTabPos[6]+1, start+sTabPos[7]);
    keywordID = str2num(start+sTabPos[7]+1, start+sTabPos[8]);
    titleID = str2num(start+sTabPos[8]+1, start+sTabPos[9]);
    descriptionID = str2num(start+sTabPos[9]+1, start+sTabPos[10]);
    userID = str2num(start+sTabPos[10]+1, start+sTabPos[11]);
}


Interaction::~Interaction()
{

}

Interaction &Interaction::operator=(const Interaction &rhs){
    this->click = rhs.click;
    this->impression = rhs.impression;
//    this->displayURL = rhs.displayURL;
    this->displayURL_H = rhs.displayURL_H;
    this->displayURL_L = rhs.displayURL_L;
    this->adID = rhs.adID;
    this->advertiserID = rhs.advertiserID;
    this->depth = rhs.depth;
    this->position = rhs.position;
    this->queryID = rhs.queryID;
    this->keywordID = rhs.keywordID;
    this->titleID = rhs.titleID;
    this->descriptionID = rhs.descriptionID;
    this->userID = rhs.userID;

    return *this;
}

bool Interaction::operator==(const Interaction &rhs) const{
//    if(this->displayURL != rhs.displayURL){
//        return false;
//    }
    if(     (this->click != rhs.click) ||
            (this->impression != rhs.impression) ||
            (this->adID != rhs.adID) ||
            (this->advertiserID != rhs.advertiserID) ||
            (this->depth != rhs.depth) ||
            (this->position != rhs.position) ||
            (this->queryID != rhs.queryID) ||
            (this->keywordID != rhs.keywordID) ||
            (this->titleID != rhs.titleID) ||
            (this->descriptionID != rhs.descriptionID) ||
            (this->userID != rhs.userID) ||
            (this->displayURL_H != rhs.displayURL_H) ||
            (this->displayURL_L != rhs.displayURL_L) ){
        return false;
    }
    return true;
}


void Interaction::init(char *start, char *end)
{
    short sTabPos[12];
    short sTabCnt = 0;
    short sCharCnt = 0;
    char *ptr = start;
    while(ptr != end){
        if(*ptr == '\t'){
            sTabPos[sTabCnt] = sCharCnt;
            ++sTabCnt;
        }
        ++sCharCnt;
        ++ptr;
    }
    sTabPos[11] = sCharCnt;

    click = str2num(start, start+sTabPos[0]);
    impression = str2num(start+sTabPos[0]+1, start+sTabPos[1]);
//    displayURL = string(start+sTabPos[1]+1, start+sTabPos[2]);
    displayURL_H = str2num(start+sTabPos[1]+1, start+sTabPos[2]-9);
    displayURL_L = str2num(start+sTabPos[2]-9, start+sTabPos[2]);
    adID = str2num(start+sTabPos[2]+1, start+sTabPos[3]);
    advertiserID = str2num(start+sTabPos[3]+1, start+sTabPos[4]);
    depth = str2num(start+sTabPos[4]+1, start+sTabPos[5]);
    position = str2num(start+sTabPos[5]+1, start+sTabPos[6]);
    queryID = str2num(start+sTabPos[6]+1, start+sTabPos[7]);
    keywordID = str2num(start+sTabPos[7]+1, start+sTabPos[8]);
    titleID = str2num(start+sTabPos[8]+1, start+sTabPos[9]);
    descriptionID = str2num(start+sTabPos[9]+1, start+sTabPos[10]);
    userID = str2num(start+sTabPos[10]+1, start+sTabPos[11]);

}

