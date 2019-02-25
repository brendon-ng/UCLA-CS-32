#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>
using namespace std;

#include <math.h>

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}


int StudentWorld::init()
{
    // Initialize variables
    m_citizensLeft = 0;
    m_zombiesLeft = 0;
    m_finishedLevel = false;
    
    // Get level file
    Level lev(assetPath());
    string levelFile;
    switch (getLevel()) {
        case 1:
            levelFile = "level01.txt";
            break;
        case 2:
            levelFile = "level02.txt";
            break;
        case 3:
            levelFile = "level03.txt";
            break;
        case 4:
            levelFile = "level04.txt";
            break;
        case 5:
            levelFile = "level05.txt";
            break;
        case 6:
            levelFile = "level06.txt";
            break;
        default: // If level file is unfound, the player has won
            return GWSTATUS_PLAYER_WON;
            break;
    }
    Level::LoadResult result = lev.loadLevel(levelFile);
    if(result == Level::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    else if (result == Level::load_fail_bad_format)
        cerr << "Your level was improperly formatted" << endl;
    else if(result == Level::load_success){
        // Iterate through each cell in level file and allocate an Actor object if present
        for(int y = 0 ; y<LEVEL_HEIGHT ; y++){
            for(int x = 0; x<LEVEL_WIDTH; x++){
                Level::MazeEntry ge = lev.getContentsOf(x, y);
                switch (ge) {
                    case Level::empty:
                        break;
                    case Level::exit:
                        m_actors.push_back(new Exit(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
                        break;
                    case Level::player:
                        m_penelope = new Penelope(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this);
                        break;
                    case Level::dumb_zombie:
                        m_zombiesLeft++;
                        break;
                    case Level::smart_zombie:
                        m_zombiesLeft++;
                        break;
                    case Level::citizen:
                        m_citizensLeft++;
                        m_actors.push_back(new Citizen(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
                        break;
                    case Level::wall:
                        m_actors.push_back(new Wall(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
                        break;
                    case Level::pit:
                        break;
                    case Level::vaccine_goodie:
                        break;
                    case Level::landmine_goodie:
                        break;
                    case Level::gas_can_goodie:
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // Ask all actors to do something
        // check is Penelope dies
    list<Actor*>::iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if(! (*it)->isDead() )
            (*it)->doSomething();
        it++;
    }
    if(!m_penelope->isDead())
        m_penelope->doSomething();
    else
        return GWSTATUS_PLAYER_DIED;
    
    // If all citizens AND Penelope have used the exit
    if(m_finishedLevel)
        return GWSTATUS_FINISHED_LEVEL;
    
    // Delete any actors that have died
    it = m_actors.begin();
    while(it != m_actors.end()){
        if((*it)->isDead()){
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }
    
    // Update status text
    
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    // Delete every dynamically allocated Actor object
    list<Actor*>::iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        delete *it;
        it = m_actors.erase(it);
    }
    delete m_penelope;
}

void StudentWorld::finishLevel() {
    m_finishedLevel = true;
}

bool StudentWorld::isBlocked(Actor* actor, int x, int y) const {
    // Top right coordinates
    int maxx = x+SPRITE_WIDTH-1;
    int maxy = y+SPRITE_HEIGHT-1;
    
    // Check every actor
    for(list<Actor*>::const_iterator it = m_actors.begin(); it!=m_actors.end(); it++){
        if(*it == actor)
            continue;
        if(!(*it)->isBlockingObject())
            continue;
        
        // Actor coords
        int actorX = (*it)->getX();
        int actorY = (*it)->getY();
        int actorMaxX = actorX + SPRITE_WIDTH - 1;
        int actorMaxY = actorY + SPRITE_HEIGHT - 1;
        
        // Bottom Left corner
        if(actorX <= x && x <= actorMaxX && actorY <= y && y <= actorMaxY)
            return true;
        // Top Left corner
        if(actorX <= x && x <= actorMaxX && actorY <= maxy && maxy <= actorMaxY)
            return true;
        // Top Right corner
        if(actorX <= maxx && maxx <= actorMaxX && actorY <= maxy && maxy <= actorMaxY)
            return true;
        // Bottom Right corner
        if(actorX <= maxx && maxx <= actorMaxX && actorY <= y && y <= actorMaxY)
            return true;
    }
    
    if(m_penelope == actor)
        return false;
    // Actor coords
    int actorX = m_penelope->getX();
    int actorY = m_penelope->getY();
    int actorMaxX = actorX + SPRITE_WIDTH - 1;
    int actorMaxY = actorY + SPRITE_HEIGHT - 1;
    
    // Bottom Left corner
    if(actorX <= x && x <= actorMaxX && actorY <= y && y <= actorMaxY)
        return true;
    // Top Left corner
    if(actorX <= x && x <= actorMaxX && actorY <= maxy && maxy <= actorMaxY)
        return true;
    // Top Right corner
    if(actorX <= maxx && maxx <= actorMaxX && actorY <= maxy && maxy <= actorMaxY)
        return true;
    // Bottom Right corner
    if(actorX <= maxx && maxx <= actorMaxX && actorY <= y && y <= actorMaxY)
        return true;
    
    return false;
}

Actor* StudentWorld::penelope() const {
    return m_penelope;
}

int StudentWorld::citizensLeft() const {return m_citizensLeft;}
void StudentWorld::decrementCitizens() {m_citizensLeft--;}
int StudentWorld::zombiesLeft() const {return m_zombiesLeft;}

double StudentWorld::getDistance(const Actor* a1, const Actor* a2) const{
    return(sqrt(pow(static_cast<double>(a1->getX() - a2->getX()), 2.0) +
                pow(static_cast<double>(a1->getY() - a2->getY()), 2.0)));
}

double StudentWorld::getDistance(const int x, const int y, const Actor* a) const{
    return(sqrt(pow(static_cast<double>(x - a->getX()), 2.0) +
                pow(static_cast<double>(y - a->getY()), 2.0)));
}

bool StudentWorld::isOverlapping(const Actor* a1, const Actor* a2) const{

    if(getDistance(a1, a2) <= EUCLIDEAN_DISTANCE)
        return true;
    else
        return false;
}

double StudentWorld::distToNearestZombie(const Actor* a) const {
    double dist = -1.0;
    
    list<Actor*>::const_iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if((*it)->isZombie() && !(*it)->isDead() ){
            if(dist < 0)
                dist = getDistance(a, *it);
            else{
                if(getDistance(a, *it) < dist)
                    dist = getDistance(a, *it);
            }
        }
        it++;
    }
    
    return dist;
}

double StudentWorld::distToNearestZombie(const int x, const int y) const {
    double dist = -1.0;
    
    list<Actor*>::const_iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if((*it)->isZombie() && !(*it)->isDead() ){
            if(dist < 0)
                dist = getDistance(x,y, *it);
            else{
                if(getDistance(x,y, *it) < dist)
                    dist = getDistance(x,y, *it);
            }
        }
        it++;
    }
    
    return dist;
}

Actor* StudentWorld::getOverlapper(const Actor* a, bool human) const {
    list<Actor*>::const_iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if((*it)->isHuman() && human ){
            if(isOverlapping(*it, a))
                return (*it);
        }
        if((*it)->isZombie() && !human){
            if(isOverlapping(*it, a))
                return (*it);
        }
        it++;
    }
    return NULL;
}

void StudentWorld::addZombie(int x, int y) {
    int r = rand() % 10;
    //if(r < 3)
        //Add smart Zombie
    //else
        //add dumb Zombie
    
    m_zombiesLeft++;

}

StudentWorld::~StudentWorld(){
    cleanUp();
}


