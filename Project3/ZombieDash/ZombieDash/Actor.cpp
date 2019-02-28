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

void Actor::die() {m_isDead = true;}
bool Actor::isDead() const {return m_isDead;}
StudentWorld* Actor::getWorld() const {return m_world;}

bool Actor::isBlockingObject() const {return false;}
bool Actor::blocksFlames() const {return false;}
bool Actor::isZombie() const {return false;}
bool Actor::isHuman() const {return false;}
bool Actor::isDamageable() const {return false;}




//////////////////////////////////
///// MOVABLE implementation /////
//////////////////////////////////
Moveable::Moveable(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, world)
{
    m_paralyze = true;
}

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

bool Moveable::isBlockingObject() const {return true;}
bool Moveable::isDamageable() const {return true;}
bool Moveable::isParalyzed() const {return m_paralyze;}
void Moveable::setParalyze(bool p){m_paralyze = p;}


void Moveable::follow(Actor *a, int step) {
    int a_x = a->getX();
    int a_y = a->getY();
    int col = getX() / LEVEL_WIDTH;
    int row = getY() / LEVEL_HEIGHT;
    int a_col = a_x / LEVEL_WIDTH;
    int a_row = a_y / LEVEL_HEIGHT;
    // If it is in the same column
    if(col == a_col) {
        if(a_row > row){
            if(moveSelf(up, step)) // if move is successful
                return;
        }
        else if (a_row < row) {
            if(moveSelf(down, step)) // if move is successful
                return;
        }
    }
    // If it is in the same row
    else if(row == a_row){
        if(a_col > col){
            if(moveSelf(right, step)) // if move is successful
                return;
        }
        else if (a_col < col) {
            if(moveSelf(left, step)) // if move is successful
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
            if(a_row > row)
                dir = up;
            else
                dir = down;
            
            if(a_col > col)
                backupdir = right;
            else
                backupdir = left;
        }
        else {
            if(a_col > col)
                dir = right;
            else
                dir = left;
            
            if(a_row > row)
                backupdir = up;
            else
                backupdir = down;
        }
        
        // Try moving in first direction, then other, if both fail, function moveso n
        if(moveSelf(dir, step))
            return;
        else{
            if(moveSelf(backupdir, step))
                return;
        }
        
    }
}

//////////////////////////////////
////// HUMAN implementation //////
//////////////////////////////////

Human::Human(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world)
: Moveable(imageID, startX, startY, dir, depth, world)
{
    m_infectionCount = 0;
    m_infectionStatus = false;
}

bool Human::isInfected() const {return m_infectionStatus;}

void Human::infect() {m_infectionStatus = true;}

void Human::uninfect() {m_infectionStatus = false;}

int Human::infectionCount() const {return m_infectionCount;}

void Human::incrementInfectionCount() {
    m_infectionCount++;
}

bool Human::isHuman() const {return true;}

////////////////////////////////
///// PLAYER implementation ////
////////////////////////////////

Penelope::Penelope(double startX, double startY, StudentWorld* world)
: Human(IID_PLAYER, startX, startY, right, 0, world)
{ }

void Penelope::doSomething(){
    // Check to see if she is alive
    if(isDead())
        return;
    
    // Check to see if she is infected
    if(isInfected())
        incrementInfectionCount();
    if(infectionCount() >= MAX_INFECTION){
        die();
        return;
    }
    
    // Check if the player has pressed a key
    int key;
    if(getWorld()->getKey(key)){
        switch (key) {
            case KEY_PRESS_SPACE:
                fireFlame();
                break;
            case KEY_PRESS_TAB:
                if (getWorld()->mines() > 0) {
                    getWorld()->addActor(new Landmine(getX(), getY(), getWorld()));
                    getWorld()->decrementMines();
                }
                
                break;
            case KEY_PRESS_ENTER:
                if(getWorld()->vaccines() > 0){
                    uninfect();
                    getWorld()->decrementVaccines();
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
}

void Penelope::die() {
    getWorld()->playSound(SOUND_PLAYER_DIE);
    Actor::die();
}

void Penelope::fireFlame(){
    if(getWorld()->charges() <= 0)
        return;
    getWorld()->decrementCharges();
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    for(int i = 1; i<=3; i++){
        int x = getX();
        int y = getY();
        Direction dir = getDirection();

        switch (dir) {
            case up:
                y = getY()+i*SPRITE_HEIGHT;
                break;
            case down:
                y = getY()-i*SPRITE_HEIGHT;
                break;
            case left:
                x = getX()-i*SPRITE_WIDTH;
                break;
            case right:
                x = getX()+i*SPRITE_WIDTH;
                break;
            default:
                break;
        }

        if(!getWorld()->flameBlocked(x,y)){
            getWorld()->addActor(new Flame(x, y, getDirection(), getWorld()));
        }
        else{
            break;
        }
    }
}

//////////////////////////////////
///// CITIZEN implementation /////
//////////////////////////////////

Citizen::Citizen(double startX, double startY, StudentWorld* world)
: Human(IID_CITIZEN, startX, startY, right, 0, world)
{
}

void Citizen::doSomething() {
    // Switch paralyze variable every tick
    setParalyze(!isParalyzed());
    
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
        
        int r = rand() % 10;
        if(r < 3)
            getWorld()->addActor(new SmartZombie(getX(), getY(), getWorld()));
        else
            getWorld()->addActor(new DumbZombie(getX(), getY(), getWorld()));
        
        getWorld()->incrementZombies();

        return;
    }
    
    // Paralysis ticks
    if(isParalyzed())
        return;
    
    // Determine Distance to penelope
    double dist_p = getWorld()->getDistance(this, getWorld()->penelope());
    
    // Determine distance to nearest zombie
    Actor* nearest = getWorld()->nearestMoveable(this, false);
    double dist_z;
    if(nearest != NULL)
        dist_z = getWorld()->getDistance(this, nearest);
    else
        dist_z = VIEW_WIDTH; // Handle case where there are no more zombies
    
    
    // If citizen wants to follow Penelope
    if((getWorld()->zombiesLeft()==0 || dist_p < dist_z) && dist_p <= DISTANCE_TO_FOLLOW) {
        follow(getWorld()->penelope(), CITIZEN_STEP_SIZE);
    }
    
    // If Citizen is to run away
    if(dist_z <= DISTANCE_TO_FOLLOW) {
        double upDist;
        double downDist;
        double rightDist;
        double leftDist;
        upDist = downDist = rightDist = leftDist = -1.0;
        
        // Get hypothetical distances to zombies
        if(!getWorld()->isBlocked(this, getX(), getY()+CITIZEN_STEP_SIZE)){
            Actor* nearest = getWorld()->nearestMoveable(getX(), getY()+CITIZEN_STEP_SIZE, false);
            upDist = getWorld()->getDistance(getX(), getY()+CITIZEN_STEP_SIZE, nearest);
        }
        if(!getWorld()->isBlocked(this, getX(), getY()-CITIZEN_STEP_SIZE)){
            Actor* nearest = getWorld()->nearestMoveable(getX(), getY()-CITIZEN_STEP_SIZE, false);
            downDist = getWorld()->getDistance(getX(), getY()-CITIZEN_STEP_SIZE, nearest);
        }
        if(!getWorld()->isBlocked(this, getX()+CITIZEN_STEP_SIZE, getY())){
            Actor* nearest = getWorld()->nearestMoveable(getX()+CITIZEN_STEP_SIZE, getY(), false);
            rightDist = getWorld()->getDistance(getX()+CITIZEN_STEP_SIZE, getY(), nearest);
        }
        if(!getWorld()->isBlocked(this, getX()-CITIZEN_STEP_SIZE, getY())){
            Actor* nearest = getWorld()->nearestMoveable(getX()-CITIZEN_STEP_SIZE, getY(), false);
            leftDist = getWorld()->getDistance(getX()-CITIZEN_STEP_SIZE, getY(), nearest);
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

void Citizen::die() {
    getWorld()->playSound(SOUND_CITIZEN_DIE);
    getWorld()->increaseScore(-1000);
    getWorld()->decrementCitizens();
    Actor::die();
}

/////////////////////////////////
///// ZOMBIE implementation /////
/////////////////////////////////

Zombie::Zombie(double startX, double startY, StudentWorld* world)
: Moveable(IID_ZOMBIE, startX, startY, right, 0, world)
{
    m_movementPlanDistance = 0;
}

void Zombie::doSomething() {
    // Switch paralyze variable every tick
    setParalyze(!isParalyzed());
    
    // Check to see if it is currently alive
    if(isDead())
        return;
    
    // Paralysis ticks
    if(isParalyzed())
        return;
    
    // Check to see if a person is in front
    int vomitX = -1;
    int vomitY = -1;
    if(getDirection() == left){
        vomitX = getX()-SPRITE_WIDTH;
        vomitY = getY();
    }
    else if(getDirection() == right){
        vomitX = getX()+SPRITE_WIDTH;
        vomitY = getY();
    }
    else if(getDirection() == up){
        vomitX = getX();
        vomitY = getY()+SPRITE_HEIGHT;
    }
    else if(getDirection() == down){
        vomitX = getX();
        vomitY = getY()-SPRITE_HEIGHT;
    }
    Actor* vom = new Vomit(vomitX, vomitY, getDirection(), getWorld());
    int randThree = rand() % 3; // One in Three chance it will vomit
    if(getWorld()->getOverlapper(vom, true, true) != NULL && randThree == 0){
        getWorld()->addActor(vom);
        getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
        return;
    }
    else
        delete vom;
    
    // Check to see if it needs a new movement plan
    if(m_movementPlanDistance == 0) {
        pickNewMovementPlan();
    }
    
    // Determine a destination coordinates and check move
    if(moveSelf(getDirection(), ZOMBIE_STEP_SIZE))
        m_movementPlanDistance--;
    else
        m_movementPlanDistance = 0;
    
    
}


int Zombie::movementPlanDistance() const {return m_movementPlanDistance;}
void Zombie::setMovementPlanDistance(int dist) {m_movementPlanDistance = dist;}
bool Zombie::isZombie() const {return true;}



//////////////////////////////////////
///// DUMB ZOMBIE implementation /////
//////////////////////////////////////

DumbZombie::DumbZombie(double startX, double startY, StudentWorld* world)
: Zombie(startX, startY, world)
{ }

void DumbZombie::pickNewMovementPlan(){
    int randDist = (rand() % 8) + 3;
    setMovementPlanDistance(randDist);
    int randDir = rand() % 4;
    switch (randDir) {
        case 0:
            setDirection(up);
            break;
        case 1:
            setDirection(down);
            break;
        case 2:
            setDirection(right);
            break;
        case 3:
            setDirection(left);
            break;
        default:
            break;
    }
}

void DumbZombie::die() {
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
    getWorld()->increaseScore(1000);
    
    // 1 in 10 Zombies drop a vaccine goodie
    int randVac = rand() % 10;
    if(randVac == 0)
        getWorld()->addActor(new VaccineGoodie(getX(),getY(),getWorld()));
    
    // Set isDead to true
    Actor::die();
}


//////////////////////////////////////
///// SMART ZOMBIE implementation ////
//////////////////////////////////////

SmartZombie::SmartZombie(double startX, double startY, StudentWorld* world)
: Zombie(startX, startY, world)
{ }

void SmartZombie::pickNewMovementPlan() {
    int randDist = (rand() % 8) + 3;
    setMovementPlanDistance(randDist);

    Actor* nearest = getWorld()->nearestMoveable(this, true);
    if(nearest == NULL)
        nearest = getWorld()->penelope();
    double dist = getWorld()->getDistance(nearest, this);
    
    if(dist > DISTANCE_TO_FOLLOW){
        int randDir = rand() % 4;
        switch (randDir) {
            case 0:
                setDirection(up);
                break;
            case 1:
                setDirection(down);
                break;
            case 2:
                setDirection(right);
                break;
            case 3:
                setDirection(left);
                break;
            default:
                break;
        }
    }
    else {
        follow(nearest, ZOMBIE_STEP_SIZE);
    }
}

void SmartZombie::die() {
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
    getWorld()->increaseScore(2000);
    Actor::die();
    
    setMovementPlanDistance(0);
}


///////////////////////////////////////
///// OVERLAPPABLE implementation /////
///////////////////////////////////////
Overlappable::Overlappable(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, world)
{ }

bool Overlappable::isOverlappingWithPenelope() const {
    return (getWorld()->isOverlapping(this, getWorld()->penelope()));
}


///////////////////////////////
///// EXIT implementation /////
///////////////////////////////

Exit::Exit(double startX, double startY, StudentWorld* world)
: Overlappable(IID_EXIT, startX, startY, right, 1, world)
{ }

void Exit::doSomething() {
    Actor* cit = NULL;
    cit = getWorld()->getOverlapper(this, true, false);
    if(cit != NULL){
        getWorld()->increaseScore(500);
        cit->Actor::die();
        getWorld()->playSound(SOUND_CITIZEN_SAVED);
        getWorld()->decrementCitizens();
    }
    
    if(isOverlappingWithPenelope() && getWorld()->citizensLeft() == 0) {
        getWorld()->finishLevel();
    }
}

bool Exit::blocksFlames() const {return true;}

//////////////////////////////
///// PIT implementation /////
//////////////////////////////

Pit::Pit(double startX, double startY, StudentWorld* world)
: Overlappable(IID_PIT, startX, startY, right, 0, world)
{ }

void Pit::doSomething() {
    // Get Human overlapping with the pit
    Actor* victim = getWorld()->getOverlapper(this, true, true);
    if(victim != NULL){
        victim->die();
    }
    
    // Get Zombie overlapping with pit
    victim = getWorld()->getOverlapper(this, false, false);
    if(victim != NULL){
        victim->die();
    }
}

///////////////////////////////////
///// LANDMINE implementation /////
///////////////////////////////////

Landmine::Landmine(double startX, double startY, StudentWorld* world)
: Overlappable(IID_LANDMINE, startX, startY, right, 1, world)
{
    m_active = false;
    m_safetyTicks = 30;
}

void Landmine::doSomething() {
    // Check if it's alive
    if(isDead())
        return;
    
    // If it is not yet active
    if(!m_active){
        m_safetyTicks--;
        if(m_safetyTicks<=0)
            m_active = true;
        return;
    }
    
    // check if it overlaps with Penelope, citizen or zombie
    if(isOverlappingWithPenelope() || getWorld()->getOverlapper(this, false, false) != NULL || getWorld()->getOverlapper(this, true, true) != NULL) {
        die();
    }
    
}

// When a landmine "dies" it explodes
void Landmine::die(){
    // Set isDead to true
    Actor::die();
    
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    
    //Introduce flame if its not blocked
    // On location
    if(!getWorld()->flameBlocked(getX(), getY()))
        getWorld()->addActor(new Flame(getX(), getY(), getDirection(), getWorld()));
    //EAST
    if(!getWorld()->flameBlocked(getX()+SPRITE_WIDTH, getY()))
        getWorld()->addActor(new Flame(getX()+SPRITE_WIDTH, getY(), getDirection(), getWorld()));
    //WEST
    if(!getWorld()->flameBlocked(getX()-SPRITE_WIDTH, getY()))
        getWorld()->addActor(new Flame(getX()-SPRITE_WIDTH, getY(), getDirection(), getWorld()));
    //NORTH
    if(!getWorld()->flameBlocked(getX(), getY()+SPRITE_HEIGHT))
        getWorld()->addActor(new Flame(getX(), getY()+SPRITE_HEIGHT, getDirection(), getWorld()));
    //SOUTH
    if(!getWorld()->flameBlocked(getX(), getY()-SPRITE_HEIGHT))
        getWorld()->addActor(new Flame(getX(), getY()-SPRITE_HEIGHT, getDirection(), getWorld()));
    //NORTHEAST
    if(!getWorld()->flameBlocked(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT))
        getWorld()->addActor(new Flame(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT, getDirection(), getWorld()));
    //NORTHWEST
    if(!getWorld()->flameBlocked(getX()-SPRITE_WIDTH, getY()+SPRITE_HEIGHT))
        getWorld()->addActor(new Flame(getX()-SPRITE_WIDTH, getY()+SPRITE_HEIGHT, getDirection(), getWorld()));
    //SOUTHEAST
    if(!getWorld()->flameBlocked(getX()+SPRITE_WIDTH, getY()-SPRITE_HEIGHT))
        getWorld()->addActor(new Flame(getX()+SPRITE_WIDTH, getY()-SPRITE_HEIGHT, getDirection(), getWorld()));
    //SOUTHWEST
    if(!getWorld()->flameBlocked(getX()-SPRITE_WIDTH, getY()-SPRITE_HEIGHT))
        getWorld()->addActor(new Flame(getX()-SPRITE_WIDTH, getY()-SPRITE_HEIGHT, getDirection(), getWorld()));
    
    // Introduce a pit
    getWorld()->addActor(new Pit(getX(), getY(), getWorld()));
    
    
}

bool Landmine::isDamageable() const {return true;}


/////////////////////////////////
///// GOODIE implementation /////
/////////////////////////////////

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* world)
: Overlappable(imageID, startX, startY, right, 1, world)
{ }

void Goodie::doSomething() {
    // check if it's alive
    if(isDead())
        return;
    
    if(isOverlappingWithPenelope()){
        getWorld()->increaseScore(50);
        die();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getGoodie(isVaccine(), isGasCan(), isLandmine());
    }
}

bool Goodie::isDamageable() const {return true;}
bool Goodie::isVaccine() const {return false;}
bool Goodie::isLandmine() const {return false;}
bool Goodie::isGasCan() const {return false;}


/////////////////////////////////////////
///// VACCINE GOODIE implementation /////
/////////////////////////////////////////

VaccineGoodie::VaccineGoodie(double startX, double startY, StudentWorld* world)
: Goodie(IID_VACCINE_GOODIE, startX, startY, world)
{ }

bool VaccineGoodie::isVaccine() const {return true;}

/////////////////////////////////////////
///// GAS CAN GOODIE implementation /////
/////////////////////////////////////////

GasCanGoodie::GasCanGoodie(double startX, double startY, StudentWorld* world)
: Goodie(IID_GAS_CAN_GOODIE, startX, startY, world)
{ }

bool GasCanGoodie::isGasCan() const {return true;}

//////////////////////////////////////////
///// LANDMINE GOODIE implementation /////
//////////////////////////////////////////

LandmineGoodie::LandmineGoodie(double startX, double startY, StudentWorld* world)
: Goodie(IID_LANDMINE_GOODIE, startX, startY, world)
{ }

bool LandmineGoodie::isLandmine() const {return true;}


/////////////////////////////////////
///// PROJECTILE implementation /////
/////////////////////////////////////

Projectile::Projectile(int imageID, double startX,double startY, Direction dir, int depth, StudentWorld* world)
: Overlappable(imageID, startX, startY, dir, depth, world)
{
    m_ticksSinceCreation = 0;
}

void Projectile::incrementTick() {m_ticksSinceCreation++;}
int Projectile::ticksSinceCreation() const {return m_ticksSinceCreation;}


////////////////////////////////
///// FLAME implementation /////
////////////////////////////////

Flame::Flame(double startX, double startY, Direction dir, StudentWorld* world)
: Projectile(IID_FLAME, startX, startY, dir, 0, world)
{ }

void Flame::doSomething() {
    // Check if it is alive
    if(isDead())
        return;
    
    // If it is after two ticks from creation
    if(ticksSinceCreation() >= 2) {
        die();
        return;
    }
    
    // Damage all objects that overlap
    getWorld()->damageVictims(this);
    
    
    incrementTick();
}

////////////////////////////////
///// VOMIT implementation /////
////////////////////////////////

Vomit::Vomit(double startX, double startY, Direction dir, StudentWorld* world)
: Projectile(IID_VOMIT, startX, startY, dir, 0, world)
{ }

void Vomit::doSomething() {
    // Check if it is alive
    if (isDead())
        return;
    
    // If it is after two ticks from creation
    if(ticksSinceCreation() >= 2) {
        die();
        return;
    }
    
    incrementTick();
}


///////////////////////////////
///// WALL implementation /////
///////////////////////////////

Wall::Wall(double startX, double startY, StudentWorld* world)
: Actor(IID_WALL, startX, startY, right, 0, world)
{ }

bool Wall::isBlockingObject() const {return true;}
bool Wall::blocksFlames() const {return true;}

void Wall::doSomething()
{
    // Wall does nothing
}
               
