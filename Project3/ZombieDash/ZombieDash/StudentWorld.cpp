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
    m_citizensLeft = 0;
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
            levelFile = "level01.txt";
            break;
        case 4:
            levelFile = "level01.txt";
            break;
        case 5:
            levelFile = "level01.txt";
            break;
        case 6:
            levelFile = "level01.txt";
            break;
        default:
            return GWSTATUS_PLAYER_WON;
            break;
    }
    Level::LoadResult result = lev.loadLevel(levelFile);
    if(result == Level::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    else if (result == Level::load_fail_bad_format)
        cerr << "Your level was improperly formatted" << endl;
    else if(result == Level::load_success){
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
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    list<Actor*>::iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()){
        (*it)->doSomething();
        it++;
    }
    //decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}


