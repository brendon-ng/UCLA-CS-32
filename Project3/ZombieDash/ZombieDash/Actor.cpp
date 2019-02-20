#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////
///// ACTOR implementation /////
////////////////////////////////
Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, depth)
{
    m_isDead = false;
    m_world = world;
}

Actor::~Actor(){}
void Actor::die() {m_isDead = true;}
bool Actor::isDead() const {return m_isDead;}
StudentWorld* Actor::getWorld() const {return m_world;}



//////////////////////////////////
///// MOVABLE implementation /////
//////////////////////////////////
Moveable::Moveable(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, world)
{
}

Moveable::~Moveable(){}

void Moveable::moveSelf(Direction dir, int steps) {
    setDirection(dir);
    switch (dir) {
        case up:
            moveTo(getX(), getY()+steps);
            break;
        case down:
            moveTo(getX(), getY()-steps);
            break;
        case right:
            moveTo(getX()+steps, getY());
            break;
        case left:
            moveTo(getX()-steps, getY());
            break;
            
        default:
            break;
    }
    
}


////////////////////////////////
///// PLAYER implementation ////
////////////////////////////////
Penelope::Penelope(double startX, double startY, StudentWorld* world)
: Moveable(IID_PLAYER, startX, startY, right, 0, world)
{
    m_infectionCount = m_mines = m_charges = m_vaccines = 0;
    m_infectionStatus = false;
}

Penelope::~Penelope() {}

void Penelope::doSomething(){
    // Check to see if she is alive
    //if(isDead())
    //    return;
    
    // Check to see if she is infected
    if(m_infectionStatus)
        m_infectionCount++;
    if(m_infectionCount >= MAX_INFECTION){
        die();
        //play a sound SOUND_PLAYER_DIE
        return;
    }
    
    // Check if the player has pressed a key
    int key;
    if(getWorld()->getKey(key)){
        switch (key) {
            case KEY_PRESS_SPACE:
                // throw flames
                break;
            case KEY_PRESS_TAB:
                // introduce landmine
                break;
            case KEY_PRESS_ENTER:
                if(m_vaccines>0){
                    m_infectionStatus = false;
                    m_vaccines--;
                }
                break;
            case KEY_PRESS_UP:
                moveSelf(up, PLAYER_STEP_SIZE);
                break;
            case KEY_PRESS_DOWN:
                moveSelf(down, PLAYER_STEP_SIZE);
                break;
            case KEY_PRESS_RIGHT:
                moveSelf(right, PLAYER_STEP_SIZE);
                break;
            case KEY_PRESS_LEFT:
                moveSelf(left, PLAYER_STEP_SIZE);
                break;
                
            default:
                break;
        }
    }
    
    // Check if overlapping vomit
    
    // Check if overlapping flame
}


///////////////////////////////
///// WALL implementation /////
///////////////////////////////

Wall::Wall(double startX, double startY, StudentWorld* world)
: Actor(IID_WALL, startX, startY, right, 0, world)
{
}

Wall::~Wall() {}


void Wall::doSomething()
{
    // Wall does nothing
}
