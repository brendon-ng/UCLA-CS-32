#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>
using namespace std;

#include <math.h>
#include <sstream>
#include <iomanip>


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
    m_vaccines = m_mines = m_charges = 0;
    
    // Get level file
    Level lev(assetPath());
    stringstream levelFile;
    levelFile << "level" << getLevel()/10 << getLevel()%10 << ".txt" ;
// CHANGE THIS
//    Level::LoadResult result = lev.loadLevel(levelFile.str());
    Level::LoadResult result = lev.loadLevel("level04.txt");
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
                        m_actors.push_back(new DumbZombie(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
                        m_zombiesLeft++;
                        break;
                    case Level::smart_zombie:
                        m_actors.push_back(new SmartZombie(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
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
                        m_actors.push_back(new Pit(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
                        break;
                    case Level::vaccine_goodie:
                        m_actors.push_back(new VaccineGoodie(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
                        break;
                    case Level::landmine_goodie:
                        m_actors.push_back(new LandmineGoodie(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
                        break;
                    case Level::gas_can_goodie:
                        m_actors.push_back(new GasCanGoodie(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
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
    else{
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    // If all citizens AND Penelope have used the exit
    if(m_finishedLevel){
        playSound(SOUND_LEVEL_FINISHED);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
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
    
    
    stringstream statText;
    statText << "Score: " ;
    statText.fill('0');
    statText << setw(6) << getScore();
    statText << "  Level: " << getLevel();
    statText << "  Lives: " << getLives();
    statText <<"  Vaccines: " << m_vaccines;
    statText << "  Flames: " << m_charges;
    statText <<"  Mines: " << m_mines;
    statText << "  Infected: " << m_penelope->infectionCount();
    
    statText << " " << m_citizensLeft;
    
    setGameStatText(statText.str());
    
    
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
    if(m_penelope != nullptr){
        delete m_penelope;
        m_penelope = nullptr;
    }
}

void StudentWorld::finishLevel() {
    m_finishedLevel = true;
}

bool StudentWorld::isBlocked(const Actor* actor, int x, int y) const {
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
    // Penelope coords
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

bool StudentWorld::flameBlocked(int x, int y) const {
    
    list<Actor*>::const_iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if((*it)->blocksFlames() && isOverlapping(x, y, *it))
            return true;
        it++;
    }
    return false;
}

Actor* StudentWorld::penelope() const {return m_penelope;}
int StudentWorld::citizensLeft() const {return m_citizensLeft;}
void StudentWorld::decrementCitizens() {m_citizensLeft--;}
int StudentWorld::zombiesLeft() const {return m_zombiesLeft;}
void StudentWorld::incrementZombies() {m_zombiesLeft++;}
int StudentWorld::vaccines() const {return m_vaccines;}
int StudentWorld::charges() const {return m_charges;}
int StudentWorld::mines() const {return m_mines;}
void StudentWorld::decrementVaccines() {m_vaccines--;}
void StudentWorld::decrementCharges() {m_charges--;}
void StudentWorld::decrementMines() {m_mines--;}



void StudentWorld::getGoodie(bool isVaccine, bool isGasCan, bool isLandmine) {
    if(isVaccine)
        m_vaccines += VACCINES_PER_GOODIE;
    else if (isGasCan)
        m_charges += CHARGES_PER_GOODIE;
    else if (isLandmine)
        m_mines += LANDMINES_PER_GOODIE;
}

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

bool StudentWorld::isOverlapping(int x, int y, const Actor* a1) const{
    if(getDistance(x,y, a1) <= EUCLIDEAN_DISTANCE)
        return true;
    else
        return false;
}

Actor* StudentWorld::getOverlapper(const Actor* a, bool human, bool penelope) const {
    list<Actor*>::const_iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if((*it)->isHuman() && human && !(*it)->isDead()){
            if(isOverlapping(*it, a))
                return (*it);
        }
        if((*it)->isZombie() && !human && !(*it)->isDead()){
            if(isOverlapping(*it, a))
                return (*it);
        }
        it++;
    }
    
    if(penelope && isOverlapping(m_penelope, a))
        return m_penelope;
    
    return NULL;
}

void StudentWorld::damageVictims(const Actor* flame) {
    list<Actor*>::iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if(isOverlapping(*it, flame) && (*it)->isDamageable() && !(*it)->isDead())
            (*it)->die();
        it++;
    }
    
    if(isOverlapping(m_penelope, flame) && ! m_penelope->isDead())
        m_penelope->die();
    
}

void StudentWorld::infectVictims(const Actor* vomit) {
    list<Actor*>::iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if(isOverlapping(*it, vomit) && (*it)->isHuman() && !(*it)->isDead()){
            //Human* victim = (*it);
            //victim->infect();
        }
        it++;
    }
}

Actor* StudentWorld::nearestMoveable(const int x, const int y, const bool human) const {
    double dist = -1.0;
    Actor* nearest = NULL;
    
    list<Actor*>::const_iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        if(!(*it)->isDead() && (((*it)->isZombie() && !human) || ((*it)->isHuman() && human))){
            if(dist < 0){
                dist = getDistance(x,y, *it);
                nearest = *it;
            }
            else{
                if(getDistance(x,y, *it) < dist){
                    dist = getDistance(x,y, *it);
                    nearest = *it;
                }
            }
        }
        it++;
    }
    
    // If we are looking for a human, check Penelope's distance
    if(human && getDistance(x,y, m_penelope) < dist) {
        dist = getDistance(x,y, m_penelope);
        nearest = m_penelope;
    }
    
    return nearest;
}

Actor* StudentWorld::nearestMoveable(const Actor* a, const bool human) const {
    return nearestMoveable(a->getX(), a->getY(), human);
}

void StudentWorld::addActor(Actor *a) {
    m_actors.push_back(a);
}

StudentWorld::~StudentWorld(){
    cleanUp();
}


