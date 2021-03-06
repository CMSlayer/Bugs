#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Compiler.h"
#include <string>
#include <vector>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;


class Compiler;
class Actor;

const int STARTING_TICKS = 2000;
const int MIN_ANTS_NEEDED_TO_WIN = 6;




class StudentWorld : public GameWorld
{
public:
    StudentWorld(string assetDir);

    virtual int init();

    virtual int move();

    virtual void cleanUp();
    
    virtual ~StudentWorld();
    
    bool canMoveTo(int x, int y) const;
    
    void addActor(Actor* a);
    
    Actor* getEdibleAt(int x, int y) const;
    
    Actor* getPheromoneAt(int x, int y, int colony) const;
    
    bool isEnemyAt(int x, int y, int colony) const;

    bool isDangerAt(int x, int y, int colony) const;
    
    bool isAntHillAt(int x, int y, int colony) const;
    
    bool biteEnemyAt(Actor* me, int colony, int biteDamage);
    
    bool poisonAllPoisonableAt(int x, int y);
    
    bool stunAllStunnableAt(int x, int y);
    
    void updateScore(int colony, bool increase);
    
    bool initializeCompiler(int colony);
    
    void decreaseTickCount();
    
    void formatGameStatText();
    
    int getTicks() const;
    
    void removeDeadActors();
    
    void updateWinner(int colony);
    
    int getWinningColony() const;
    
private:
    vector<Actor*> m_PlayingField[64][64];
    
    Compiler* m_antInstructions[4];
    int m_colonyScores[4];
    
    int m_winningColony;
    int m_ticks;
    
};

//=============================================Auxiliary Functions





















#endif // STUDENTWORLD_H_
