#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

const int MAX_INFECTION = 500;
const int PLAYER_STEP_SIZE = 4;

class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
    virtual ~Actor();
    virtual void doSomething() = 0;
    bool isDead() const;
    void die();
    bool isBlockingObject() const;
    
protected:
    void setBlockingObject();
    StudentWorld* getWorld() const;
    
private:
    bool m_isDead;
    StudentWorld* m_world;
    bool m_isBlockingObject;
    
};

class Moveable: public Actor
{
public:
    Moveable(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world);
    virtual ~Moveable();
    void moveSelf(Direction dir, int steps);
};

class Penelope : public Moveable
{
public:
    Penelope(double startX, double startY, StudentWorld* world);
    virtual ~Penelope();
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
    virtual ~Wall();
    virtual void doSomething();
};

#endif // ACTOR_H_
