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
const int PICK_MOVEMENT_PLAN_MOVE_SUCCESS = 0;
const int PICK_MOVEMENT_PLAN_NO_MOVE = 1;
const int PICK_MOVEMENT_PLAN_MOVE_FAILED = 2;
const int PROJECTILE_LIFE_SPAN = 2;
const int INCREASE_SCORE_GOODIE = 50;
const int INCREASE_SCORE_DUMB_ZOMBIE_KILLED = 1000;
const int INCREASE_SCORE_SMART_ZOMBIE_KILLED = 2000;
const int INCREASE_SCORE_CITIZEN_SAVED = 500;
const int INCREASE_SCORE_CITIZEN_KILLED = -1000;


//////////////////
///// ACTOR  /////
//////////////////
class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world);
    
    virtual void doSomething() = 0;
    
    //Accessors and Modifiers
    virtual void die();
    bool isDead() const;
    virtual bool isBlockingObject() const;
    virtual bool blocksFlames() const;
    virtual bool isInfectable() const;
    virtual bool isDamageable() const;
    virtual void infect();
    virtual bool canStepOn() const;
    

    StudentWorld* getWorld() const;
    
private:
    bool m_isDead;
    StudentWorld* m_world;
    
};


/////////////////////
///// MOVEABLE  /////
/////////////////////
class Moveable: public Actor
{
public:
    Moveable(int imageID, double startX,double startY, StudentWorld* world);

protected:
    bool moveSelf(Direction dir, int steps);
    bool follow(Actor* a, int step);
    
    //Accessors and Modifiers
    bool isParalyzed() const;
    void setParalyze(bool p);
    
    // Override boolean methods
    virtual bool isBlockingObject() const;
    virtual bool isDamageable() const;
    virtual bool canStepOn() const;
    
private:
    bool m_paralyze;
};


//////////////////
///// HUMAN  /////
//////////////////
class Human : public Moveable
{
public:
    Human(int imageID, double startX, double startY, StudentWorld* world);
    
    // Accessor and Modifer
    int infectionCount() const;
    virtual void infect();
    
protected:
    //Accessors and Modifiers
    bool isInfected() const;
    void uninfect();
    void incrementInfectionCount();
    
    // Override boolean methods
    virtual bool isInfectable() const;
    
private:
    bool m_infectionStatus;
    int m_infectionCount;
    
};


/////////////////////
///// PENELOPE  /////
/////////////////////
class Penelope : public Human
{
public:
    Penelope(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual void die();
private:
    void fireFlame();
};


////////////////////
///// CITIZEN  /////
////////////////////
class Citizen : public Human
{
public:
    Citizen(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual void die();
    virtual void infect();
private:
    void morph();
};


///////////////////
///// ZOMBIE  /////
///////////////////
class Zombie : public Moveable
{
public:
    Zombie(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    
protected:
    virtual int pickNewMovementPlan() = 0;  // Returns true if the move was performed, false otherwise
    
    // Accessors and Modifiers
    int movementPlanDistance() const;
    void setMovementPlanDistance(int dist);

private:
    int m_movementPlanDistance;
};


////////////////////////
///// DUMB ZOMBIE  /////
////////////////////////
class DumbZombie : public Zombie
{
public:
    DumbZombie(double startX, double startY, StudentWorld* world);
    virtual void die();
protected:
    virtual int pickNewMovementPlan();
};


/////////////////////////
///// SMART ZOMBIE  /////
/////////////////////////
class SmartZombie : public Zombie
{
public:
    SmartZombie(double startX, double startY, StudentWorld* world);
    virtual void die();
protected:
    virtual int pickNewMovementPlan();
};


/////////////////////////
///// OVERLAPPABLE  /////
/////////////////////////
class Overlappable: public Actor
{
public:
    Overlappable(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world);
    bool isOverlappingWithPenelope() const;
};


/////////////////
///// EXIT  /////
/////////////////
class Exit: public Overlappable
{
public:
    Exit(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
protected:
    // Override Boolean Methods
    virtual bool blocksFlames() const;
};


////////////////
///// PIT  /////
////////////////
class Pit : public Overlappable
{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
};


/////////////////////
///// LANDMINE  /////
/////////////////////
class Landmine : public Overlappable
{
public:
    Landmine(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual void die();
protected:
    // Override Boolean Methods
    virtual bool isDamageable() const;
private:
    bool m_active;
    int m_safetyTicks;
};


///////////////////
///// GOODIE  /////
///////////////////
class Goodie : public Overlappable
{
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    
protected:
    virtual bool isVaccine() const;
    virtual bool isGasCan() const;
    virtual bool isLandmine() const;
    
    // Override Boolean Methods
    virtual bool isDamageable() const;
};

///////////////////////////
///// VACCINE GOODIE  /////
///////////////////////////
class VaccineGoodie : public Goodie
{
public:
    VaccineGoodie(double startX, double startY, StudentWorld* world);
protected:
    virtual bool isVaccine() const;
};


///////////////////////////
///// GAS CAN GOODIE  /////
///////////////////////////
class GasCanGoodie : public Goodie
{
public:
    GasCanGoodie(double startX, double startY, StudentWorld* world);
protected:
    virtual bool isGasCan() const;
};


////////////////////////////
///// LANDMINE GOODIE  /////
////////////////////////////
class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(double startX, double startY, StudentWorld* world);
protected:
    virtual bool isLandmine() const;
};


///////////////////////
///// PROJECTILE  /////
///////////////////////
class Projectile : public Overlappable
{
public:
    Projectile(int imageID, double startX,double startY, Direction dir, StudentWorld* world);
protected:
    int ticksSinceCreation() const;
    void incrementTick();
private:
    int m_ticksSinceCreation;
};


//////////////////
///// FLAME  /////
//////////////////
class Flame : public Projectile
{
public:
    Flame(double startX, double startY, Direction dir, StudentWorld* world);
    virtual void doSomething();
};


/////////////////
///// VOMIT /////
/////////////////
class Vomit : public Projectile
{
public:
    Vomit(double startX, double startY, Direction dir, StudentWorld* world);
    virtual void doSomething();
    
};


/////////////////
///// WALL  /////
/////////////////
class Wall : public Actor
{
public:
    Wall(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isBlockingObject() const;
    virtual bool blocksFlames() const;
};


#endif // ACTOR_H_
