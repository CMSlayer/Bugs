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
    
    //StudentWorld* this = new StudentWorld(assetDirectory());
    
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
                         a= new AntHill(this, x, y, ANT_TYPE_0, m_antInstructions[0]);
                         addActor(a);
                            m_colonyScores[0]=0;
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
                            a= new AntHill(this, x, y, ANT_TYPE_1, m_antInstructions[1]);
                            addActor(a);
                            m_colonyScores[1]=0;
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
                            a= new AntHill(this, x, y, ANT_TYPE_2, m_antInstructions[2]);
                            addActor(a);
                            m_colonyScores[2]=0;
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
                            a= new AntHill(this, x, y, ANT_TYPE_3, m_antInstructions[3]);
                            addActor(a);
                            m_colonyScores[3]=0;
                        }
                        else
                            return GWSTATUS_LEVEL_ERROR;
                    }
                    break;
                }
                case (Field::FieldItem::food):
                {
                    a= new Food(this, x, y, FOOD_START_ENERGY);
                    addActor(a);
                    break;
                }
                case (Field::FieldItem::grasshopper):
                {
                    a= new BabyGrasshopper(this, x, y);
                    addActor(a);
                    break;
                }
                case (Field::FieldItem::water):
                {
                    a= new WaterPool(this, x, y);
                    addActor(a);
                    break;
                }
                case (Field::FieldItem::rock):
                {
                    a= new Pebble(this, x, y);
                    addActor(a);
                    break;
                }
                case (Field::FieldItem::poison):
                {
                    a= new Poison(this, x, y);
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
                    if((*it)!=nullptr)
                    {
                        //get the actor's current location
                        int oldX= (*it)->getX() , oldY = (*it)->getY();
                    
                        if(!(*it)->isDead())
                        (*it)->doSomething();
                    
                        if((*it)->getX()!= oldX || (*it)->getY()!=oldY)
                        addActor(*it);
                    }
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
                if((*it)!= nullptr)
                {
                    if(count<1)
                    {
                        delete (*it)->getWorld();
                        count++;
                    }
                
                    delete (*it);
                }
            }
    
    for(auto i = 0; i<4; i++)
        delete m_antInstructions[i];
    
    
    
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
    {
        if((*it)!=nullptr)
        {
            if((*it)->isEdible())
                return (*it);
        }
    }
    return nullptr;
}

// If a pheromone of the indicated colony is at x,y, return a pointer
// to it; otherwise, return a null pointer.
Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
    {
        if((*it)!=nullptr)
        {
            if((*it)->isPheromone())
            return (*it);
        }
    }
    return nullptr;
}

// Is an enemy of an ant of the indicated colony at x,y?
bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
    {
        if((*it)!=nullptr)
        {
            if((*it)->isEnemy(colony))
            return true;
        }
    }
    return false;
}

// Is something dangerous to an ant of the indicated colony at x,y?
bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
    {
        if((*it)!=nullptr)
        {
            if((*it)->isDangerous(colony))
            return true;
        }
    }
    
    return false;
}

// Is the anthill of the indicated colony at x,y?
bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
    {
        if((*it)!=nullptr)
        {
            
            if((*it)->isAntHill(colony))
            return true;
        }
    }
    
    return false;
}

// Bite an enemy of an ant of the indicated colony at me's location
// (other than me; insects don't bite themselves).  Return true if an
// enemy was bitten.
bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
    vector<Actor*> v = m_PlayingField[me->getY()][me->getX()];
    
    for(auto it = v.begin(); it!= v.end(); it++)
    {
        if((*it)!=nullptr)
        {
            if((*it)->isEnemy(colony) && (*it)!= me)
            {
                (*it)->getBitten(biteDamage);
                return true;
            }
        }
    }
    return false;
}

// Poison all poisonable actors at x,y.
bool StudentWorld::poisonAllPoisonableAt(int x, int y)
{
    bool poisonedAnActor= false;
    
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
    {
        if((*it)!=nullptr)
        {
            if((*it)->becomesFoodUponDeath())
            {
                (*it)->getPoisoned();
                poisonedAnActor = true;
            }
        }
    }
    return poisonedAnActor;
}

// Stun all stunnable actors at x,y.
bool StudentWorld::stunAllStunnableAt(int x, int y)
{
    bool stunnedAnActor= false;
    
    vector<Actor*> v = m_PlayingField[y][x];
    for(auto it = v.begin(); it!= v.end(); it++)
        if((*it)!=nullptr)
        {
            if((*it)->becomesFoodUponDeath())
            {
                (*it)->getStunned();
                stunnedAnActor = true;
            }
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
            m_antInstructions[colony]=c;
            return true;
        }
        else
        {
            cout << ErrorMessage << endl;
            return false;
        }
    
}


void StudentWorld::decreaseTickCount()
{
    m_ticks--;
}

void StudentWorld::formatGameStatText()
{
    string simulationHeader = "";
    simulationHeader+= "Ticks:";
    
    ostringstream ticks;
    ticks << setw(5) << getTicks() << endl;
    ticks.fill('0');
    string s1 = ticks.str();
    simulationHeader+= s1;
    
    vector<string> v = getFilenamesOfAntPrograms();
    int colony= 0;
    for(auto it = v.begin(); it!= v.end(); it++,colony++)
    {
        string s2;
        if(colony == getWinningColony())
        {
            ostringstream ants;
            ants << setw(2) << m_colonyScores[colony] << endl;
            ants.fill('0');
            string temp = ants.str();
            
            //concatenating each player's status
            s2+= (*it);
            s2+= "*: "; s2+= temp; 
            s2+= m_colonyScores[colony]; s2+= "  ";
            simulationHeader+=s2;
        }
        else
        {
            ostringstream ants;
            ants << setw(2) << m_colonyScores[colony] << endl;
            ants.fill('0');
            string temp = ants.str();
            
            //concatenating each player's status
            s2+= (*it);
            s2+= ": "; s2+= temp;
            s2+= m_colonyScores[colony]; s2+= "  ";
            simulationHeader+=s2;
        }
    }
    
    setGameStatText(simulationHeader);
}

int StudentWorld::getTicks() const
{
    return m_ticks;
}

void StudentWorld::removeDeadActors()
{
    for(int r=0; r<64; r++)
        for(int c=0; c<64; c++)
            for (auto it = m_PlayingField[r][c].begin(); it != m_PlayingField[r][c].end();it++)
            {
                if((*it)!=nullptr)
                {
                    if((*it)->isDead())
                    {
                        if((*it)->becomesFoodUponDeath())
                        {
                            if(getEdibleAt(c,r)!=nullptr)
                            {
                                Food* a = (Food *)getEdibleAt(c, r);
                                a->updateEnergy(100);
                                delete (*it);
                            }
                            else
                            {
                                Actor* a = new Food(this, c , r, 100 );
                                addActor(a);
                                delete (*it);
                            }
                        }
                    
                    }
                }
                
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



















