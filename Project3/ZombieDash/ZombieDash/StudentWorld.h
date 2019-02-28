#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>
class Actor;
class Penelope;

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
    void addActor(Actor* a);
    void damageVictims(const Actor* flame);
    void infectVictims(const Actor* vomit);
    void getGoodie(bool isVaccine, bool isGasCan, bool isLandmine);
    
    // Status functions
    bool isBlocked(const Actor* actor, int x, int y) const;
    bool flameBlocked(int x, int y) const;
    double getDistance(const Actor* a1, const Actor* a2) const;
    double getDistance(const int x, const int y, const Actor* a) const;
    bool isOverlapping(const Actor* a1, const Actor* a2) const;
    bool isOverlapping(int x, int y, const Actor* a1) const;
    Actor* getOverlapper(const Actor* a, bool human, bool penelope) const;
    Actor* nearestMoveable(const Actor* a, const bool human) const;
    Actor* nearestMoveable(const int x, const int y, const bool human) const;
    
    //Accessors and Modifiers
    Actor* penelope() const;
    int citizensLeft() const;
    int zombiesLeft() const;
    void incrementZombies();
    void decrementCitizens();
    int vaccines() const;
    int charges() const;
    int mines() const;
    void decrementVaccines();
    void decrementCharges();
    void decrementMines();

private:
    bool m_finishedLevel;
    int m_citizensLeft;
    int m_zombiesLeft;
    Penelope* m_penelope;
    std::list<Actor*> m_actors;
    int m_vaccines;
    int m_charges;
    int m_mines;
};

#endif // STUDENTWORLD_H_
