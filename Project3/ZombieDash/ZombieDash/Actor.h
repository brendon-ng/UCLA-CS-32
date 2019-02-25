#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

const int MAX_INFECTION = 500;
const int PLAYER_STEP_SIZE = 4;
const int CITIZEN_STEP_SIZE = 2;
const double EUCLIDEAN_DISTANCE = 10.0;

class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
    virtual ~Actor();
    
    virtual void doSomething() = 0;
    
    //Accessors and Modifiers
    bool isDead() const;
    void die();
    bool isBlockingObject() const;
    bool isOverlappable() const;
    bool isZombie() const;
    bool isHuman() const;
    
protected:
    void setBlockingObject();
    void setOverlappable();
    void setIsZombie();
    void setHuman();
    StudentWorld* getWorld() const;
    
private:
    bool m_isDead;
    StudentWorld* m_world;
    bool m_isBlockingObject;
    bool m_isOverlappable;
    bool m_isZombie;
    bool m_isHuman;
    
};

class Moveable: public Actor
{
public:
    Moveable(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world);
    virtual ~Moveable();
    bool moveSelf(Direction dir, int steps);
};

class Human : public Moveable
{
public:
    Human(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world);
    virtual ~Human();
    
    bool isInfected() const;
    void uninfect();
    int infectionCount() const;
    void incrementInfectionCount();
private:
    bool m_infectionStatus;
    int m_infectionCount;
    
};

class Penelope : public Human
{
public:
    Penelope(double startX, double startY, StudentWorld* world);
    virtual ~Penelope();
    virtual void doSomething();
private:
    int m_mines;
    int m_charges;
    int m_vaccines;
};

class Citizen : public Human
{
public:
    Citizen(double startX, double startY, StudentWorld* world);
    virtual ~Citizen();
    virtual void doSomething();
private:
    bool m_paralyze;
};

class Overlappable: public Actor
{
public:
    Overlappable(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world);
    virtual ~Overlappable();
    bool isOverlappingWithPenelope() const;
};

class Exit: public Overlappable
{
public:
    Exit(double startX, double startY, StudentWorld* world);
    virtual ~Exit();
    virtual void doSomething();
};

class Wall : public Actor
{
public:
    Wall(double startX, double startY, StudentWorld* world);
    virtual ~Wall();
    virtual void doSomething();
};



#endif // ACTOR_H_
