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
const int LANDMINE_SAFETY_TICKS = 30;
const int VACCINES_PER_GOODIE = 1;
const int CHARGES_PER_GOODIE = 5;
const int LANDMINES_PER_GOODIE = 2;


class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
    
    virtual void doSomething() = 0;
    

    //Accessors and Modifiers
    bool isDead() const;
    virtual void die();
    virtual bool isBlockingObject() const;
    virtual bool blocksFlames() const;
    virtual bool isZombie() const;
    virtual bool isHuman() const;
    virtual bool isDamageable() const;
    
protected:
    StudentWorld* getWorld() const;
    
private:
    bool m_isDead;
    StudentWorld* m_world;
    
};


class Moveable: public Actor
{
public:
    Moveable(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world);

    bool moveSelf(Direction dir, int steps);
    void follow(Actor* a, int step);
    
    virtual bool isBlockingObject() const;
    virtual bool isDamageable() const;
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
    void infect();
    virtual bool isHuman() const;
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
    void fireFlame();
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
    virtual bool isZombie() const;
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
    virtual bool blocksFlames() const;
};

class Pit : public Overlappable
{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
};

class Landmine : public Overlappable
{
public:
    Landmine(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual void die();
    virtual bool isDamageable() const;
private:
    bool m_active;
    int m_safetyTicks;
};

class Goodie : public Overlappable
{
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isDamageable() const;
protected:
    virtual bool isVaccine() const;
    virtual bool isGasCan() const;
    virtual bool isLandmine() const;
};

class VaccineGoodie : public Goodie
{
public:
    VaccineGoodie(double startX, double startY, StudentWorld* world);
protected:
    virtual bool isVaccine() const;
};

class GasCanGoodie : public Goodie
{
public:
    GasCanGoodie(double startX, double startY, StudentWorld* world);
protected:
    virtual bool isGasCan() const;
};

class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(double startX, double startY, StudentWorld* world);
protected:
    virtual bool isLandmine() const;
};

class Projectile : public Overlappable
{
public:
    Projectile(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world);
protected:
    int ticksSinceCreation() const;
    void incrementTick();
private:
    int m_ticksSinceCreation;
};

class Flame : public Projectile
{
public:
    Flame(double startX, double startY, Direction dir, StudentWorld* world);
    virtual void doSomething();
};

class Vomit : public Projectile
{
public:
    Vomit(double startX, double startY, Direction dir, StudentWorld* world);
    virtual void doSomething();
    
};

class Wall : public Actor
{
public:
    Wall(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isBlockingObject() const;
    virtual bool blocksFlames() const;
};


#endif // ACTOR_H_
