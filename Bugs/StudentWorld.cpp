#include "StudentWorld.h"
#include "Field.h"
#include <string>
using namespace std;

class Food;

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
    m_ticks=STARTING_TICKS;
    
    //at the start there is no winner
    m_winningColony = -1;
    
    StudentWorld* ThisIsTheWorld = new StudentWorld(assetDirectory());
    
    string fieldFileName;
    Field f;
    
    string fieldFile = getFieldFilename();
    
    if(f.loadField(fieldFile)!= Field::LoadResult::load_success)
        return GWSTATUS_LEVEL_ERROR;
    
    for(int x=0; x<64; x++)
        for(int y=0; y<64; y++)
        {
            Field::FieldItem item = f.getContentsOf(x,y);
            Actor* a;
            
            switch(item)
            {
                case (Field::FieldItem::anthill0):
                {
                    if(getFilenamesOfAntPrograms().size()>=1)
                    {
                        if(initializeCompiler(ANT_TYPE_0))
                        {
                         a= new AntHill(ThisIsTheWorld, x, y, ANT_TYPE_0, m_antInstructions.at(0));
                         addActor(a);
                            m_colonyScores.push_back(0);
                        }
                        else
                            return GWSTATUS_LEVEL_ERROR;
                    }
                    break;
                }
                case (Field::FieldItem::anthill1):
                {
                    if(getFilenamesOfAntPrograms().size()>=2)
                    {
                        if(initializeCompiler(ANT_TYPE_1))
                        {
                            a= new AntHill(ThisIsTheWorld, x, y, ANT_TYPE_1, m_antInstructions.at(1));
                            addActor(a);
                            m_colonyScores.push_back(0);
                        }
                        else
                            return GWSTATUS_LEVEL_ERROR;
                    }
                    break;
                }
                case (Field::FieldItem::anthill2):
                {
                    if(getFilenamesOfAntPrograms().size()>=3)
                    {
                        if(initializeCompiler(ANT_TYPE_2))
                        {
                            a= new AntHill(ThisIsTheWorld, x, y, ANT_TYPE_2, m_antInstructions.at(2));
                            addActor(a);
                            m_colonyScores.push_back(0);
                        }
                        else
                            return GWSTATUS_LEVEL_ERROR;
                    }
                    break;
                }
                case (Field::FieldItem::anthill3):
                {
                    if(getFilenamesOfAntPrograms().size()>=4)
                    {
                        if(initializeCompiler(ANT_TYPE_3))
                        {
                            a= new AntHill(ThisIsTheWorld, x, y, ANT_TYPE_3, m_antInstructions.at(3));
                            addActor(a);
                            m_colonyScores.push_back(0);
                        }
                        else
                            return GWSTATUS_LEVEL_ERROR;
                    }
                    break;
                }
                case (Field::FieldItem::food):
                {
                    a= new Food(ThisIsTheWorld, x, y, FOOD_START_ENERGY);
                    addActor(a);
                    break;
                }
                case (Field::FieldItem::grasshopper):
                {
                    a= new BabyGrasshopper(ThisIsTheWorld, x, y);
                    addActor(a);
                    break;
                }
                case (Field::FieldItem::water):
                {
                    a= new WaterPool(ThisIsTheWorld, x, y);
                    addActor(a);
                    break;
                }
                case (Field::FieldItem::rock):
                {
                    a= new Pebble(ThisIsTheWorld, x, y);
                    addActor(a);
                    break;
                }
                case (Field::FieldItem::poison):
                {
                    a= new Poison(ThisIsTheWorld, x, y);
                    addActor(a);
                    break;
                }
                case (Field::FieldItem::empty):
                    break;
                default:
                    return GWSTATUS_LEVEL_ERROR;
            }
        }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    decreaseTickCount();
    
    formatGameStatText();
    
    for(int r=0; r<64; r++)
        for(int c=0; c<64; c++)
            for (auto it = m_PlayingField[r][c].begin(); it != m_PlayingField[r][c].end(); it++)
                {
                    //get the actor's current location
                    int oldX= (*it)->getX() , oldY = (*it)->getY();
                    
                    if(!(*it)->isDead())
                        (*it)->doSomething();
                    
                    if((*it)->getX()!= oldX || (*it)->getY()!=oldY)
                        moveActor((*it), oldX, oldY, it);
                }
    
    removeDeadActors();
    
    //update the text at the top of the screen
    formatGameStatText();

    if(getTicks()<=0)
    {
        if(getWinningColony() != -1)
            
            return GWSTATUS_PLAYER_WON;
        else
            return GWSTATUS_NO_WINNER;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    int count=0;
    
    for(int r=0; r<64; r++)
        for(int c=0; c<64; c++)
            for (auto it = m_PlayingField[r][c].begin(); it != m_PlayingField[r][c].end(); it++)
            {
                if(count<1)
                {
                    delete (*it)->getWorld();
                    count++;
                }
                
                delete (*it);
            }
    
    for( auto it = m_antInstructions.begin(); it!=m_antInstructions.end(); it++)
        delete (*it);
    
    
    
}


// Can an insect move to x,y?
bool StudentWorld::canMoveTo(int x, int y) const
{
    if(m_PlayingField[y][x].at(0)->blocksMovement())
        return false;
    return true;
}

// Add an actor to the world
void StudentWorld::addActor(Actor* a)
{
    m_PlayingField[a->getY()][a->getX()].push_back(a);
}

// If an item that can be picked up to be eaten is at x,y, return a
// pointer to it; otherwise, return a null pointer.  (Edible items are
// only ever going be food.)
Actor* StudentWorld::getEdibleAt(int x, int y) const
{
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
        if((*it)->isEdible())
            return (*it);
    return nullptr;
}

// If a pheromone of the indicated colony is at x,y, return a pointer
// to it; otherwise, return a null pointer.
Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
        if((*it)->isPheromone())
            return (*it);
    return nullptr;
}

// Is an enemy of an ant of the indicated colony at x,y?
bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
        if((*it)->isEnemy(colony))
            return true;
    
    return false;
}

// Is something dangerous to an ant of the indicated colony at x,y?
bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
        if((*it)->isDangerous(colony))
            return true;
    
    return false;
}

// Is the anthill of the indicated colony at x,y?
bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
        if((*it)->isAntHill(colony))
            return true;
    
    return false;
}

// Bite an enemy of an ant of the indicated colony at me's location
// (other than me; insects don't bite themselves).  Return true if an
// enemy was bitten.
bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
    vector<Actor*> v = m_PlayingField[me->getY()][me->getX()];
    
    for(auto it = v.begin(); it!= v.end(); it++)
        if((*it)->isEnemy(colony) && (*it)!= me)
        {
            (*it)->getBitten(biteDamage);
            return true;
        }

    return false;
}

// Poison all poisonable actors at x,y.
bool StudentWorld::poisonAllPoisonableAt(int x, int y)
{
    bool poisonedAnActor= false;
    
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
        if((*it)->becomesFoodUponDeath())
        {
            (*it)->getPoisoned();
            poisonedAnActor = true;
        }
    return poisonedAnActor;
}

// Stun all stunnable actors at x,y.
bool StudentWorld::stunAllStunnableAt(int x, int y)
{
    bool stunnedAnActor= false;
    
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
        if((*it)->becomesFoodUponDeath())
        {
            (*it)->getStunned();
            stunnedAnActor = true;
        }
    return stunnedAnActor;
}

// Record another ant birth for the indicated colony.
void StudentWorld::updateScore(int colony, bool increase)
{
    if(increase)
        m_colonyScores[colony]++;
    else
        m_colonyScores[colony]--;
    updateWinner(colony);
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

bool StudentWorld::initializeCompiler(int colony)
{
    
    string s = getFilenamesOfAntPrograms().at(colony);
    
    string ErrorMessage;
    
        Compiler* c = new Compiler();
        if(c->compile(s, ErrorMessage))
        {
            m_antInstructions.push_back(c);
            return true;
        }
        else
            cout << ErrorMessage << endl;
    
    return false;
}


void StudentWorld::decreaseTickCount()
{
    m_ticks--;
}


void StudentWorld::moveActor(Actor* a, int oldX, int oldY, vector<Actor*>::iterator it)
{
    m_PlayingField[oldY][oldX].erase(it);
    addActor(a);
}

void StudentWorld::formatGameStatText()
{
    string s = "";
    s+= "Ticks:";
    s+="";
    setGameStatText("something");
}

int StudentWorld::getTicks() const
{
    return m_ticks;
}

void StudentWorld::removeDeadActors()
{
    for(int r=0; r<64; r++)
        for(int c=0; c<64; c++)
            for (auto it = m_PlayingField[r][c].begin(); it != m_PlayingField[r][c].end();)
            {
                if((*it)->isDead())
                {
                    it= m_PlayingField[r][c].erase(it);
                }
                else
                    it++;
            }
}


int StudentWorld::getWinningColony() const
{
    return m_winningColony;
}

//checks to see if the indicated colony has more ants than
//colony that is currently in the lead,
//If it does, it sets m_winningAnt to colony
//otherwise, it leaves m_winningAnt unchanged
void StudentWorld::updateWinner(int colony)
{
    if(m_colonyScores[colony]>m_winningColony)
        m_winningColony= colony;
    
    if(m_colonyScores[m_winningColony]<MIN_ANTS_NEEDED_TO_WIN)
        m_winningColony=-1;
}



















