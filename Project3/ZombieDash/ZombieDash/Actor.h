#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

const int MAX_INFECTION = 500;
const int PLAYER_STEP_SIZE = 4;
const int CITIZEN_STEP_SIZE = 2;
const int ZOMBIE_STEP_SIZE = 1;
const double EUCLIDEAN_DISTANCE = 10.0;
const double DISTANCE_TO_FOLLOW = 80.0;


class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
    
    virtual void doSomething() = 0;
    

    //Accessors and Modifiers
    bool isDead() const;
    virtual void die();
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

    bool moveSelf(Direction dir, int steps);
    void follow(Actor* a, int step);
    
    bool isParalyzed() const;
    void setParalyze(bool p);
    
private:
    bool m_paralyze;
};


class Human : public Moveable
{
public:
    Human(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world);
    
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
    virtual void doSomething();
    virtual void die();
private:
    int m_mines;
    int m_charges;
    int m_vaccines;
};

class Citizen : public Human
{
public:
    Citizen(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual void die();
};

class Zombie : public Moveable
{
public:
    Zombie(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual void pickNewMovementPlan() = 0;
protected:
    int movementPlanDistance() const;
    void setMovementPlanDistance(int dist);
private:
    int m_movementPlanDistance;
};

class DumbZombie : public Zombie
{
public:
    DumbZombie(double startX, double startY, StudentWorld* world);
    virtual void pickNewMovementPlan();
    virtual void die();
};

class SmartZombie : public Zombie
{
public:
    SmartZombie(double startX, double startY, StudentWorld* world);
    virtual void pickNewMovementPlan();
    virtual void die();
};

class Overlappable: public Actor
{
public:
    Overlappable(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world);
    bool isOverlappingWithPenelope() const;
};

class Exit: public Overlappable
{
public:
    Exit(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
};

class Pit : public Overlappable
{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
};

class Wall : public Actor
{
public:
    Wall(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
};

class Vomit : public Actor
{
public:
    Vomit(double startX, double startY, StudentWorld* world)
    : Actor(IID_VOMIT, startX, startY, right, 0, world)
    {
        
    }
    virtual void doSomething() {}
};

class VaccineGoodie : public Actor
{
public:
    VaccineGoodie(double startX, double startY, StudentWorld* world)
    : Actor(IID_VACCINE_GOODIE, startX, startY, right, 0, world)
    {
        
    }
    virtual void doSomething() {}
};


#endif // ACTOR_H_
