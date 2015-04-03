#include "interaction.h"

Interaction::Interaction()     
{

}

Interaction::Interaction( istringstream &ss)
{
    ss >> click;
    ss >> impression;
    ss >> displayURL;
    ss >> adID;
    ss >> advertiserID;
    ss >> depth;
    ss >> position;
    ss >> queryID;
    ss >> titleID;
    ss >> descriptionID;
    ss >> userID;

}


Interaction::~Interaction()
{

}

