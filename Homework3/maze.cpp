//
//  maze.cpp
//  Homework3
//
//  Created by Brendon Ng on 2/11/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    if (maze[sr][sc] != '.')
        return false;
    if(sr==er && sc==ec)
        return true;
    maze[sr][sc] = 'D';
    if(maze[sr+1][sc] == '.')   //SOUTH
        if(pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    if(maze[sr][sc-1] == '.')   //WEST
        if(pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    if(maze[sr-1][sc] == '.')   //NORTH
        if(pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
            return true;
    if(maze[sr][sc+1] == '.')   //EAST
        if(pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    return false;
}

