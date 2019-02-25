#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
class Actor;
class Moveable;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void finishLevel();
    void addZombie(int x, int y);
    
    double getDistance(const Actor* a1, const Actor* a2) const;
    double getDistance(const int x, const int y, const Actor* a) const;
    bool isOverlapping(const Actor* a1, const Actor* a2) const;
    Actor* getOverlapper(const Actor* a, bool human) const;
    bool isBlocked(Actor* actor, int x, int y) const;
    double distToNearestZombie(const Actor* a) const;
    double distToNearestZombie(const int x, const int y) const;
    
    
    //Accessors and Modifiers
    Actor* penelope() const;
    int citizensLeft() const;
    int zombiesLeft() const;
    void decrementCitizens();
    

private:
    bool m_finishedLevel;
    int m_citizensLeft;
    int m_zombiesLeft;
    Actor* m_penelope;
    std::list<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
