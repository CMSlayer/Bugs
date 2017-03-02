#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;
class Compiler;

const int ANT_TYPE_0 = IID_ANT_TYPE0;
const int ANT_TYPE_1 = IID_ANT_TYPE1;
const int ANT_TYPE_2 = IID_ANT_TYPE2;
const int ANT_TYPE_3 = IID_ANT_TYPE3;

const int START_DEPTH_INSECT    = 1;
const int START_DEPTH_PEBBLE    = 1;
const int START_DEPTH_FOOD      = 2;
const int START_DEPTH_PHEREMONE = 2;
const int START_DEPTH_ANTHILL   = 2;
const int START_DEPTH_WATER     = 2;
const int START_DEPTH_POISON    = 2;

const int BABYHOPPER_START_HP =      500;
const int ADULTHOPPER_START_HP =     1600;
const int ANT_START_HP =             1500;
const int ANTHILL_START_HP =         8999;
const int FOOD_START_ENERGY =        6000;
const int PHEREMONE_START_STRENGTH = 256;
const int DEAD_INSECT_ENERGY =       100;

const int MIN_ANTHILL_BIRTH_ENERGY = 2000;
const int ENERGY_LOST_TO_BIRTH =     1500;
const int MOULTING_ENERGY =          1600;

const int POISON_DAMAGE_POINTS =           150;
const int ANT_BITE_DAMAGE_POINTS =         15;
const int GRASSHOPPER_BITE_DAMAGE_POINTS = 50;

const int MAX_PHEREMONE_STRENGTH =         768;
const int MAX_ANT_HOLDING_CAPACITY =       1800;
const int MAX_ANT_PICKUP_PER_TICK =        400;
const int MAX_ANT_EAT_PER_TICK =           100;

const int GRASSHOPPER_REST_PERIOD = 2;
const int STUN_TIME = 2;

//=============================================Actor
class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);
    
    virtual void doSomething() = 0;
    
    
    virtual bool isDead() const;
    
    virtual bool blocksMovement() const;
    
    virtual void getBitten(int amt);

    virtual void getPoisoned();
    
    virtual void getStunned();
    
    virtual bool isEdible() const;

    virtual bool isPheromone() const;
    
    virtual bool isEnemy(int colony) const;
    
    virtual bool isDangerous(int colony) const;
    
    virtual bool isAntHill(int colony) const;
    
    virtual bool becomesFoodUponDeath() const;
    
    StudentWorld* getWorld() const;
    
    //=====my functions
    virtual int getColony() const;
    
private:
    StudentWorld* m_World;
};

//===========================================Pebble
class Pebble : public Actor
{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
    virtual bool blocksMovement() const;
};

//===========================================EnergyHolder
class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
   
    virtual bool isDead() const;
    
    int getEnergy() const;
    
    void updateEnergy(int amt);
    
    void addFood(int amt);
    
    int pickupFood(int amt);
    
    int pickupAndEatFood(int amt);
    
    virtual bool becomesFoodUponDeath() const;
private:
    int m_energy;
    bool m_dead;
};

//===========================================Food
class Food : public EnergyHolder
{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual void doSomething();
    virtual bool isEdible() const;
};

//===========================================AntHill
class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
    virtual void doSomething();
    virtual bool isMyHill(int colony) const;
private:
    Compiler* m_offspringInstructions;
    int m_colony;
};

//===========================================Pheremone
class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* sw, int startX, int startY, int colony);
    virtual void doSomething();
    virtual bool isPheromone(int colony) const;
    
    // Increase the strength (i.e., energy) of this pheromone.
    void increaseStrength();
};

//===========================================TriggerableActor
class TriggerableActor : public Actor
{
public:
    TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
    virtual bool isDangerous(int colony) const;
};

//===========================================WaterPool
class WaterPool : public TriggerableActor
{
public:
    WaterPool(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

//===========================================Poison
class Poison : public TriggerableActor
{
public:
    Poison(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

//===========================================Insect
class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* world, int startX, int startY, int energy, int imageID);
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual void getPoisoned();
    virtual void getStunned();
    virtual bool isEnemy(int colony);
    virtual bool becomesFoodUponDeath() const;
    
    void getXYInFrontOfMe(int x, int y) const;
    
    virtual bool moveForwardIfPossible();

    void increaseSleepTicks(int amt);
private:
    bool m_lastMoveWasBlocked;
    
};

//===========================================Ant
class Ant : public Insect
{
public:
    Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID);
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual bool isEnemy(int colony) const;
    virtual bool moveForwardIfPossible();
    virtual int getColony() const;
private:
    Compiler* m_myInstructions;
    int m_colony;
};

//===========================================GrassHopper
class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID);
    virtual void doSomething();
    void setNewPath();
};

//===========================================BabyGrasshopper
class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual bool isEnemy(int colony) const;
};

//===========================================AdultGrasshopper
class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual void getBitten(int amt);
};

//===========================================Auxiliary Function declarations

GraphObject::Direction getRandomDir();


#endif // ACTOR_H_
