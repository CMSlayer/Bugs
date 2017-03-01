#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir)
:GameWorld(assetDir)
{
    
}

int StudentWorld::init()
{
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter.
		  // Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.
    return GWSTATUS_NO_WINNER;
}

void StudentWorld::cleanUp()
{
}


// Can an insect move to x,y?
bool StudentWorld::canMoveTo(int x, int y) const
{
    return true;
}

// Add an actor to the world
void StudentWorld::addActor(Actor* a)
{
    
}

// If an item that can be picked up to be eaten is at x,y, return a
// pointer to it; otherwise, return a null pointer.  (Edible items are
// only ever going be food.)
Actor* StudentWorld::getEdibleAt(int x, int y) const
{
    return new Ant(this, 0, 0, 0, m_ant0instructions, IID_ANT_TYPE0);
}

// If a pheromone of the indicated colony is at x,y, return a pointer
// to it; otherwise, return a null pointer.
Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
    return new Ant(this, 0, 0, 0, m_ant0instructions, IID_ANT_TYPE0);
}

// Is an enemy of an ant of the indicated colony at x,y?
bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
    return true;
}

// Is something dangerous to an ant of the indicated colony at x,y?
bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
    return true;
}

// Is the anthill of the indicated colony at x,y?
bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
    return true;
}

// Bite an enemy of an ant of the indicated colony at me's location
// (other than me; insects don't bite themselves).  Return true if an
// enemy was bitten.
bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
    return true;
}

// Poison all poisonable actors at x,y.
bool StudentWorld::poisonAllPoisonableAt(int x, int y)
{
    return true;
}

// Stun all stunnable actors at x,y.
bool StudentWorld::stunAllStunnableAt(int x, int y)
{
    return true;
}

// Record another ant birth for the indicated colony.
void StudentWorld::increaseScore(int colony)
{
    
}








