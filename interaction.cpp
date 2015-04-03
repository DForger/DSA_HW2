#include "interaction.h"

Interaction::Interaction()     
{

}

Interaction::Interaction(const istringstream &ss)
{
    ss>>click;
    ss>>impression;
    ss>>adID;

}


Interaction::~Interaction()
{

}

