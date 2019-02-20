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
StudentWorld* Actor::getWorld() {return m_world;}


////////////////////////////////
///// PLAYER implementation ////
////////////////////////////////
Penelope::Penelope(double startX, double startY, StudentWorld* world)
: Actor(IID_PLAYER, startX, startY, right, 0, world)
{
    m_infectionCount = m_mines = m_charges = m_vaccines = 0;
    m_infectionStatus = false;
}

void Penelope::doSomething(){
    // Check to see if she is alive
    //if(isDead())
    //    return;
    
    // Check to see if she is infected
    if(m_infectionStatus)
        m_infectionCount++;
    if(m_infectionCount >= 500){
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
                m_infectionStatus = false;
                m_vaccines--;
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                moveTo(getX(),getY()+4);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                moveTo(getX(),getY()-4);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                moveTo(getX()+4,getY());
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                moveTo(getX()-4,getY());
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


void Wall::doSomething()
{
    // Wall does nothing
}
