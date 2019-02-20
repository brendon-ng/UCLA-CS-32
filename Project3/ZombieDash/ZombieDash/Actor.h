#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
    virtual ~Actor();
    virtual void doSomething() = 0;
    bool isDead() const;
    void die();
    StudentWorld* getWorld();
private:
    bool m_isDead;
    StudentWorld* m_world;
    
};

class Penelope : public Actor
{
public:
    Penelope(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
private:
    bool m_infectionStatus;
    int m_infectionCount;
    int m_mines;
    int m_charges;
    int m_vaccines;
};

class Wall : public Actor
{
public:
    Wall(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
};

#endif // ACTOR_H_
