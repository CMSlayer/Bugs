#include "Actor.h"
#include "StudentWorld.h"



//====================================Actor class

//GraphObject::GraphObject(int imageID, int startX, int startY, Direction dir = right, int depth = 0, double size = 0.25)

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, int depth)
:GraphObject(imageID, startX, startY, startDirection, depth)
{
    
}


//====================================EnergyHolder class

EnergyHolder::EnergyHolder(int imageID, int startX, int startY, Direction startDirection, int depth, int startingEnergy):
Actor(imageID, startX, startY, startDirection, depth)
{
    
}

bool EnergyHolder::blocksMovement(Actor* inQuestion) const
{
    return false;
}

void EnergyHolder::changeEnergy(int amt)
{
    m_energy+=amt;
}


//====================================Insect class

Insect::Insect(int imageID, int startX, int startY, Direction startDirection, int depth, int startingEnergy):
EnergyHolder(imageID, startX, startY, startDirection, depth, startingEnergy)
{
    
}

int Insect::getRestCount()const
{
    return m_restCount;
}

void Insect::increaseRestCount(int amt)
{
    m_restCount+= amt;
}

int Insect::getDistance() const
{
    return m_curDist;
}

void Insect::setDistance(int distance)
{
    m_curDist= distance;
}

//====================================GrassHopper class

GrassHopper::GrassHopper(int imageID, int startX, int startY, Direction startDirection, int depth, int startingEnergy):
Insect(imageID, startX, startY, startDirection, depth, startingEnergy)
{
    
}


//====================================BabyGrassHopper class

BabyGrassHopper::BabyGrassHopper(int startX, int startY):
GrassHopper(IID_BABY_GRASSHOPPER, startX, startY, none, BABYHOPPER_START_DEPTH, BABYHOPPER_START_HP)
{
    setDirection(getRandomDir());
    setDistance(randInt(2, 12)); //walking instructions
    
    //m_restCount==0;
    
}

















