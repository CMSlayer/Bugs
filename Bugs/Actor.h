#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Compiler;

const int ANT_TYPE_0 = 0;
const int ANT_TYPE_1 = 1;
const int ANT_TYPE_2 = 2;
const int ANT_TYPE_3 = 3;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);
    
    // Action to perform each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    virtual bool isDead() const;
    
    // Does this actor block movement?
    virtual bool blocksMovement() const;
    
    // Cause this actor to be be bitten, suffering an amount of damage.
    virtual void getBitten(int amt);
    
    // Cause this actor to be be poisoned.
    virtual void getPoisoned();
    
    // Cause this actor to be be stunned.
    virtual void getStunned();
    
    // Can this actor be picked up to be eaten?
    virtual bool isEdible() const;
    
    // Is this actor detected by an ant as a pheromone?
    virtual bool isPheromone() const;
    
    // Is this actor an enemy of an ant of the indicated colony?
    virtual bool isEnemy(int colony) const;
    
    // Is this actor detected as dangerous by an ant of the indicated colony?
    virtual bool isDangerous(int colony) const;
    
    // Is this actor the anthill of the indicated colony?
    virtual bool isAntHill(int colony) const;
    
    // Get this actor's world.
    StudentWorld* getWorld() const;
};

class Pebble : public Actor
{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual void doSomething();
    virtual bool blocksMovement() const;
};

class EnergyHolder : public Actor
{
    EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
    virtual bool isDead() const;
    
    // Get this actor's amount of energy (for a Pheromone, same as strength).
    int getEnergy() const;
    
    // Adjust this actor's amount of energy upward or downward.
    void updateEnergy(int amt);
    
    // Add an amount of food to this actor's location.
    void addFood(int amt);
    
    // Have this actor pick up an amount of food.
    int pickupFood(int amt);
    
    // Have this actor pick up an amount of food and eat it.
    int pickupAndEatFood(int amt);
    
    // Does this actor become food when it dies?
    virtual bool becomesFoodUponDeath() const;
};

class Food : public EnergyHolder
{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual void doSomething();
    virtual bool isEdible() const;
};

class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
    virtual void doSomething();
    virtual bool isMyHill(int colony) const;
};

class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* sw, int startX, int startY, int colony);
    virtual void doSomething();
    virtual bool isPheromone(int colony) const;
    
    // Increase the strength (i.e., energy) of this pheromone.
    void increaseStrength();
};

class TriggerableActor : public Actor
{
public:
    TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
    virtual bool isDangerous(int colony) const;
};

class WaterPool : public TriggerableActor
{
public:
    WaterPool(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Poison : public TriggerableActor
{
public:
    Poison(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

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
    
    // Set x,y to the coordinates of the spot one step in front of this insect.
    void getXYInFrontOfMe(int x, int y) const;
    
    // Move this insect one step forward if possible, and return true;
    // otherwise, return false without moving.
    virtual bool moveForwardIfPossible();
    
    // Increase the number of ticks this insect will sleep by the indicated amount.
    void increaseSleepTicks(int amt);
};

class Ant : public Insect
{
public:
    Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID);
    virtual void doSomething();
    virtual void getBitten(int amt);
    virtual bool isEnemy(int colony) const;
    virtual bool moveForwardIfPossible();
};

class Grasshopper : public Insect
{
public:
    Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID);
    virtual void doSomething();
};

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual bool isEnemy(int colony) const;
};

class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
    virtual void getBitten(int amt);
};



#endif // ACTOR_H_
