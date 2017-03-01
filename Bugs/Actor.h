#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

const int BABYHOPPER_START_HP= 500;
const int BABYHOPPER_START_DEPTH= 1;
const int GRASSHOPPER_REST_PERIOD= 2;

//====================================Actor class (abstract)
class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction startDirection, int depth);
    virtual void doSomething()=0;
    virtual bool blocksMovement(Actor* inQuestion) const=0;
    
};

//====================================EnergyHolder class (abstract)
class EnergyHolder: public Actor
{
public:
    EnergyHolder(int imageID, int startX, int startY, Direction startDirection, int depth, int startingEnergy);
    virtual void changeEnergy(int amt);
    virtual bool blocksMovement(Actor* inQuestion) const; //returns false
    
private:
    int m_energy;
};

//====================================Insect class (abstract)
class Insect: public EnergyHolder
{
public:
    Insect(int imageID, int startX, int startY, Direction startDirection, int depth, int startingEnergy);
    int getRestCount() const;
    void increaseRestCount(int amt);
    int getDistance() const;
    void setDistance(int distance);
    
    
    
private:
    int m_restCount=0;
    int m_curDist; //current walking distance
    
};

//====================================GrassHopper class (abstract)
class GrassHopper: public Insect
{
public:
    GrassHopper(int imageID, int startX, int startY, Direction startDirection, int depth, int startingEnergy);
    virtual void doSomething();

    
private:
    
};


//====================================BabyGrassHopper class
class BabyGrassHopper: public GrassHopper
{
public:
    BabyGrassHopper(int startX, int startY);
    virtual void doSomething();
    void move();
    
private:
    
    
    
};


//====================================Auxilliary functions

//randInt(): Return a uniformly distributed random int from min to max, inclusive
GraphObject::Direction getRandomDir() //return a random direction: up, right, down, or left
{
    int d= randInt(1, 4);
    
    switch (d)
    {
        case 1:
            return GraphObject::Direction::up;
            break;
        case 2:
            return GraphObject::Direction::right;
            break;
        case 3:
            return GraphObject::Direction::down;
            break;
        case 4:
            return GraphObject::Direction::left;
            break;
        default:
            std::cerr << "This is bad\n";
            exit(1);
            break;
    }
}



#endif // ACTOR_H_
