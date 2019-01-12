//
//  Game.hpp
//  Project1
//
//  Created by Brendon Ng on 1/12/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#ifndef Game_h
#define Game_h

class Arena;
class Player;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
};

#endif /* Game_h */
