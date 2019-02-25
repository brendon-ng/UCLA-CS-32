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
    m_isBlockingObject = false;
    m_isOverlappable = false;
    m_isZombie = false;
    m_isHuman = false;
}

Actor::~Actor(){}
void Actor::die() {m_isDead = true;}
bool Actor::isDead() const {return m_isDead;}
StudentWorld* Actor::getWorld() const {return m_world;}
bool Actor::isBlockingObject() const {return m_isBlockingObject;}
void Actor::setBlockingObject() {m_isBlockingObject = true;}
bool Actor::isOverlappable() const {return m_isOverlappable;}
void Actor::setOverlappable() {m_isOverlappable = true;}
bool Actor::isZombie() const {return m_isZombie;}
void Actor::setIsZombie() {m_isZombie = true;}
bool Actor::isHuman() const {return m_isHuman;}
void Actor::setHuman(){m_isHuman = true;}



//////////////////////////////////
///// MOVABLE implementation /////
//////////////////////////////////
Moveable::Moveable(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, world)
{
    setBlockingObject();
}

Moveable::~Moveable(){}

bool Moveable::moveSelf(Direction dir, int steps) {
    int newX = getX();
    int newY = getY();
    switch (dir) {
        case up:
            newY += steps;
            break;
        case down:
            newY -= steps;
            break;
        case right:
            newX += steps;
            break;
        case left:
            newX -= steps;
            break;

        default:
            break;
    }
    
    // Don't move if its blocked
    if (!getWorld()->isBlocked(this, newX, newY)){
        setDirection(dir);
        moveTo(newX, newY);
        return true;
    }
    
    // return false if it is blocked
    return false;
    
}

//////////////////////////////////
////// HUMAN implementation //////
//////////////////////////////////

Human::Human(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world)
: Moveable(imageID, startX, startY, dir, depth, world)
{
    m_infectionCount = 0;
    m_infectionStatus = false;
    setHuman();
}

Human::~Human() {}

bool Human::isInfected() const {return m_infectionStatus;}

void Human::uninfect() {m_infectionStatus = false;}

int Human::infectionCount() const {return m_infectionCount;}

void Human::incrementInfectionCount() {
    m_infectionCount++;
}

////////////////////////////////
///// PLAYER implementation ////
////////////////////////////////
Penelope::Penelope(double startX, double startY, StudentWorld* world)
: Human(IID_PLAYER, startX, startY, right, 0, world)
{
    m_mines = m_charges = m_vaccines = 0;
}

Penelope::~Penelope() {}

void Penelope::doSomething(){
    // Check to see if she is alive
    //if(isDead())
    //    return;
    
    // Check to see if she is infected
    if(isInfected())
        incrementInfectionCount();
    if(infectionCount() >= MAX_INFECTION){
        die();
        getWorld()->playSound(SOUND_PLAYER_DIE);
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
                    uninfect();
                    m_vaccines--;
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                moveSelf(up, PLAYER_STEP_SIZE);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                moveSelf(down, PLAYER_STEP_SIZE);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                moveSelf(right, PLAYER_STEP_SIZE);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                moveSelf(left, PLAYER_STEP_SIZE);
                break;
                
            default:
                break;
        }
    }
    
    // Check if overlapping vomit
    
    // Check if overlapping flame
}

//////////////////////////////////
///// CITIZEN implementation /////
//////////////////////////////////

Citizen::Citizen(double startX, double startY, StudentWorld* world)
: Human(IID_CITIZEN, startX, startY, right, 0, world)
{
    m_paralyze = true;
}

Citizen::~Citizen() {}

void Citizen::doSomething() {
    // Switch paralyze variable every tick
    m_paralyze = !m_paralyze;
    
    // Check to see if it is currently alive
    if(isDead())
        return;
    
    // If infected
    if(isInfected())
        incrementInfectionCount();
    
    // If infection kills citizen
    if(infectionCount() >= 500){
        die();
        getWorld()->playSound(SOUND_ZOMBIE_BORN);
        getWorld()->increaseScore(-1000);
        getWorld()->addZombie(getX(), getY());
        return;
    }
    
    // Paralysis ticks
    if(m_paralyze)
        return;
    
    // Determine Distance to penelope
    double dist_p = getWorld()->getDistance(this, getWorld()->penelope());
    
    // Determine distance to nearest zombie
    double dist_z = getWorld()->distToNearestZombie(this);
    
    
    // If citizen wants to follow Penelope
    if((getWorld()->zombiesLeft()==0 || dist_p < dist_z) && dist_p <= 80) {

        int p_x = getWorld()->penelope()->getX();
        int p_y = getWorld()->penelope()->getY();
        int col = getX() / LEVEL_WIDTH;
        int row = getY() / LEVEL_HEIGHT;
        int p_col = p_x / LEVEL_WIDTH;
        int p_row = p_y / LEVEL_HEIGHT;
        // If it is in the same column
        if(col == p_col) {
            if(p_row > row){
                if(moveSelf(up, CITIZEN_STEP_SIZE)) // if move is successful
                    return;
            }
            else if (p_row < row) {
                if(moveSelf(down, CITIZEN_STEP_SIZE)) // if move is successful
                    return;
            }
        }
        // If it is in the same row
        else if(row == p_row){
            if(p_col > col){
                if(moveSelf(right, CITIZEN_STEP_SIZE)) // if move is successful
                    return;
            }
            else if (p_col < col) {
                if(moveSelf(left, CITIZEN_STEP_SIZE)) // if move is successful
                    return;
            }
        }
        // Not in the same row or col
        else {
            // randomize vertical or horizontal first
            bool moveVertically = rand() % 2;
            
            Direction dir;
            Direction backupdir;
            
            if(moveVertically){
                if(p_row > row)
                    dir = up;
                else
                    dir = down;
                
                if(p_col > col)
                    backupdir = right;
                else
                    backupdir = left;
            }
            else {
                if(p_col > col)
                    dir = right;
                else
                    dir = left;
                
                if(p_row > row)
                    backupdir = up;
                else
                    backupdir = down;
            }
            
            // Try moving in first direction, then other, if both fail, function moveso n
            if(moveSelf(dir, CITIZEN_STEP_SIZE))
                return;
            else{
                if(moveSelf(backupdir, CITIZEN_STEP_SIZE))
                    return;
            }
            
        }
        
        // If Citizen is to run away
        if(dist_z <= 80) {
            double upDist;
            double downDist;
            double rightDist;
            double leftDist;
            upDist = downDist = rightDist = leftDist = -1.0;
            
            // Get hypothetical distances to zombies
            if(!getWorld()->isBlocked(this, getX(), getY()+CITIZEN_STEP_SIZE)){
                upDist = getWorld()->distToNearestZombie(getX(), getY()+CITIZEN_STEP_SIZE);
            }
            if(!getWorld()->isBlocked(this, getX(), getY()-CITIZEN_STEP_SIZE)){
                downDist = getWorld()->distToNearestZombie(getX(), getY()-CITIZEN_STEP_SIZE);
            }
            if(!getWorld()->isBlocked(this, getX()+CITIZEN_STEP_SIZE, getY())){
                rightDist = getWorld()->distToNearestZombie(getX()+CITIZEN_STEP_SIZE, getY());
            }
            if(!getWorld()->isBlocked(this, getX()-CITIZEN_STEP_SIZE, getY())){
                leftDist = getWorld()->distToNearestZombie(getX()-CITIZEN_STEP_SIZE, getY());
            }
            
            // If none are better than current spot, dont move
            if(upDist <= dist_z && downDist <= dist_z && leftDist <= dist_z && rightDist <= dist_z)
                return;
            else {
                // Find max distance and try to move in that direction
                if(upDist >= downDist && upDist >= leftDist && upDist >= rightDist){
                    if(moveSelf(up, CITIZEN_STEP_SIZE))
                        return;
                }
                else if(downDist >= upDist && downDist >= leftDist && downDist >= rightDist){
                    if(moveSelf(down, CITIZEN_STEP_SIZE))
                        return;
                }
                else if(rightDist >= downDist && rightDist >= leftDist && rightDist >= upDist) {
                    if(moveSelf(right, CITIZEN_STEP_SIZE))
                        return;
                }
                else if(leftDist >= downDist && leftDist >= upDist && leftDist >= rightDist){
                    if(moveSelf(left, CITIZEN_STEP_SIZE))
                        return;
                }
            }
         }
        
    }
}



///////////////////////////////////////
///// OVERLAPPABLE implementation /////
///////////////////////////////////////
Overlappable::Overlappable(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, world)
{
    setOverlappable();
}

Overlappable::~Overlappable() {}

bool Overlappable::isOverlappingWithPenelope() const {
    return (getWorld()->isOverlapping(this, getWorld()->penelope()));
}


///////////////////////////////
///// EXIT implementation /////
///////////////////////////////

Exit::Exit(double startX, double startY, StudentWorld* world)
: Overlappable(IID_EXIT, startX, startY, right, 1, world)
{ }

Exit::~Exit() {}

void Exit::doSomething() {
    Actor* cit = NULL;
    cit = getWorld()->getOverlapper(this, true);
    if(cit != NULL){
        getWorld()->increaseScore(500);
        cit->die();
        getWorld()->playSound(SOUND_CITIZEN_SAVED);
        getWorld()->decrementCitizens();
    }
    
    if(isOverlappingWithPenelope() && getWorld()->citizensLeft() == 0) {
        getWorld()->finishLevel();
    }
        
}

///////////////////////////////
///// WALL implementation /////
///////////////////////////////

Wall::Wall(double startX, double startY, StudentWorld* world)
: Actor(IID_WALL, startX, startY, right, 0, world)
{
    setBlockingObject();
}

Wall::~Wall() {}


void Wall::doSomething()
{
    // Wall does nothing
}
               
