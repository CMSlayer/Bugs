
#include "Actor.h"
#include "StudentWorld.h"


//==========================================class Actor : public GraphObject

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth)
:GraphObject(imageID, startX, startY, startDir, depth)
{
    m_World= world;
}

// Is this actor dead?
bool Actor::isDead() const //
{
    return false;
}
    
// Does this actor block movement?
bool Actor::blocksMovement() const //
{
    return false;
}
    
// Cause this actor to be be bitten, suffering an amount of damage.
void Actor::getBitten(int amt)
{
  
}
    
// Cause this actor to be be poisoned.
void Actor::getPoisoned()
{
    
}
    
// Cause this actor to be be stunned.
void Actor::getStunned()
{
    
}
    
// Can this actor be picked up to be eaten?
bool Actor::isEdible() const
{
    return false;
}
    
// Is this actor detected by an ant as a pheromone?
bool Actor::isPheromone() const
{
    return false;
}
    
// Is this actor an enemy of an ant of the indicated colony?
bool Actor::isEnemy(int colony) const
{
    return this->getColony() == colony;
}
    
// Is this  detected as dangerous by an ant of the indicated colony?
bool Actor::isDangerous(int colony) const
{
    return true;
}
    
// Is this actor the anthill of the indicated colony?
bool Actor::isAntHill(int colony) const
{
    return false;
}

bool Actor::becomesFoodUponDeath() const
{
    return false;
}
    
// Get this actor's world.
StudentWorld* Actor::getWorld() const
{
    return m_World;
}

int Actor::getColony() const
{
    return 0;
}


//==============================================class Pebble : public Actor

Pebble::Pebble(StudentWorld* sw, int startX, int startY)
:Actor(sw, startX, startY, right, IID_ROCK, START_DEPTH_PEBBLE)
{
    
}
void Pebble::doSomething()
{
    
}
bool Pebble::blocksMovement() const
{
    return true;
}

//==============================================class EnergyHolder : public Actor

EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth)
:Actor(sw, startX, startY, startDir, imageID, depth)
{
    
}

bool EnergyHolder::isDead() const
{
    return m_dead;
}
    
// Get this actor's amount of energy (for a Pheromone, same as strength).
int EnergyHolder::getEnergy() const
{
    return m_energy;
}
    
// Adjust this actor's amount of energy upward or downward.
void EnergyHolder::updateEnergy(int amt)
{
    m_energy += amt;
}

// Add an amount of food to this actor's location.
void EnergyHolder::addFood(int amt)
{
    getWorld()->getEdibleAt(0,0);
}
    
// Have this actor pick up an amount of food.
int EnergyHolder::pickupFood(int amt)
{
    m_energy-=amt;
    return 0;
}
    
// Have this actor pick up an amount of food and eat it.
int EnergyHolder::pickupAndEatFood(int amt)
{
    return 0;
}
    
// Does this actor become food when it dies?
bool EnergyHolder::becomesFoodUponDeath() const
{
    return 0;
}

//==============================================class Food : public EnergyHolder

Food::Food(StudentWorld* sw, int startX, int startY, int energy)
:EnergyHolder(sw, startX, startY, right, energy, IID_FOOD, START_DEPTH_FOOD)
{
    
}
void Food::doSomething()
{
    
}
bool Food::isEdible() const
{
    return true;
}

//==============================================class AntHill : public EnergyHolder

AntHill::AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program)
:EnergyHolder(sw, startX, startY, right, ANTHILL_START_HP, colony, START_DEPTH_INSECT)
{
    m_offspringInstructions= program;
}
void AntHill::doSomething()
{
    
}
bool AntHill::isMyHill(int colony) const
{
    return false;
}


//==============================================class Pheromone : public EnergyHolder

Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony)
:EnergyHolder(sw, startX, startY, right, PHEREMONE_START_STRENGTH , colony + 11, START_DEPTH_PHEREMONE)
{
    
}
void Pheromone::doSomething()
{
    
}

bool Pheromone::isPheromone(int colony) const
{
    return true;
}
    
// Increase the strength (i.e., energy) of this pheromone.
void Pheromone::increaseStrength()
{
    updateEnergy(256);
}

//==============================================class TriggerableActor : public Actor


TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID)
:Actor(sw, x, y, right, imageID, START_DEPTH_WATER)
{
    
}

bool TriggerableActor::isDangerous(int colony) const
{
    return true;
}

//==============================================class WaterPool : public TriggerableActor

WaterPool::WaterPool(StudentWorld* sw, int x, int y)
:TriggerableActor(sw, x, y, IID_WATER_POOL)
{
    
}

void WaterPool::doSomething()
{
    
}

//==============================================class Poison : public TriggerableActor

Poison::Poison(StudentWorld* sw, int x, int y)
:TriggerableActor(sw, x, y, IID_POISON)
{
    
}

void Poison::doSomething()
{
    
}

//==============================================class Insect : public EnergyHolder

Insect::Insect(StudentWorld* world, int startX, int startY, int energy, int imageID)
:EnergyHolder(world, startX, startY, getRandomDir(), energy, imageID, START_DEPTH_INSECT)
{
    
}
void Insect::doSomething()
{
    
}
void Insect::getBitten(int amt)
{
    updateEnergy(-amt);
}
void Insect::getPoisoned()
{
    updateEnergy(-150);
}
void Insect::getStunned()
{
    
}
bool Insect::isEnemy(int colony)
{
    return true;
}
bool Insect::becomesFoodUponDeath() const
{
    return true;
}

// Set x,y to the coordinates of the spot one step in front of this insect.
void Insect::getXYInFrontOfMe(int x, int y) const
{
    
}
    
// Move this insect one step forward if possible, and return true;
// otherwise, return false without moving.
bool Insect::moveForwardIfPossible()
{
    return false;
}
    
// Increase the number of ticks this insect will sleep by the indicated amount.
void Insect::increaseSleepTicks(int amt)
{
    
}

//==============================================class Ant : public Insect

Ant::Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID)
:Insect(sw, startX, startY, ANT_START_HP, imageID)
{
    m_myInstructions = program;
}

void Ant::doSomething()
{
    
}
void Ant::getBitten(int amt)
{
    Insect::getBitten(amt);
}
bool Ant::isEnemy(int colony) const
{
    return true;
}
bool Ant::moveForwardIfPossible()
{
    return false;
}

int Ant::getColony() const
{
    return m_colony;
}

//==============================================class Grasshopper : public Insect

Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID)
:Insect(sw, startX, startY, energy, imageID)
{
    
}
void Grasshopper::doSomething()
{
    
}
void setNewPath()
{
    
}

//==============================================class BabyGrasshopper : public Grasshopper


BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
:Grasshopper(sw, startX, startY, BABYHOPPER_START_HP, IID_BABY_GRASSHOPPER)
{
    
}
bool BabyGrasshopper::isEnemy(int colony) const
{
    return false;
}

//==============================================class AdultGrasshopper : public Grasshopper

AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY)
:Grasshopper(sw, startX, startY, ADULTHOPPER_START_HP, IID_ADULT_GRASSHOPPER)
{
    
}
void AdultGrasshopper::getBitten(int amt)
{
    Insect::getBitten(amt);
}

//==============================================Auxiliary Functions

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






