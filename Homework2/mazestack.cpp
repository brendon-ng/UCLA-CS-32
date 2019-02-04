//
//  mazestack.cpp
//  Homework2
//
//  Created by Brendon Ng on 2/1/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include <iostream>
#include <stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    stack<Coord> coordStack;
    
    if (maze[sr][sc] != '.')
        return false;
    maze[sr][sc] = 'D';
    coordStack.push(Coord(sr,sc));
    while (!coordStack.empty()) {
        int r = coordStack.top().r();
        int c = coordStack.top().c();
        coordStack.pop();
        if (r==er && c==ec)
            return true;
        if (maze[r+1][c] == '.') { // SOUTH
            coordStack.push(Coord(r+1,c));
            maze[r+1][c] = 'D';
        }
        if (maze[r][c-1] == '.') { // WEST
            coordStack.push(Coord(r,c-1));
            maze[r][c-1] = 'D';
        }
        if (maze[r-1][c] == '.') { // NORTH
            coordStack.push(Coord(r-1,c));
            maze[r-1][c] = 'D';
        }
        if (maze[r][c+1] == '.') { // EAST
            coordStack.push(Coord(r,c+1));
            maze[r][c+1] = 'D';
        }
    }
    return false;
}

int main() {
    string maze[10] = {
        "XXXXXXXXXX",
        "X....X...X",
        "X.XX.XX..X",
        "XXX....X.X",
        "X.XXX.XXXX",
        "X.X...X..X",
        "X...X.X..X",
        "XXXXX.X.XX",
        "X........X",
        "XXXXXXXXXX"
    };
 
    if (pathExists(maze, 10,10, 3,5, 8,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
 
    string maze1[5] = {
        "XXXXXXX",
        "X...X.X",
        "XXX.X.X",
        "X.....X",
        "XXXXXXX",
    };
 
    if (pathExists(maze1, 5,7, 1,1, 1,5))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
 
}


