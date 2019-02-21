#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>
using namespace std;

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
                        break;
                    case Level::player:
                        m_actors.push_back(new Penelope(SPRITE_WIDTH*x,SPRITE_HEIGHT*y, this));
                        break;
                    case Level::dumb_zombie:
                        break;
                    case Level::smart_zombie:
                        break;
                    case Level::citizen:
                        m_citizensLeft++;
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
    
    // If all citizens AND Penelope have used the exit
    //if()
    //    return GWSTATUS_FINISHED_LEVEL;
    
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
    return false;
}

StudentWorld::~StudentWorld(){
    cleanUp();
}


