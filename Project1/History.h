//
//  History.hpp
//  Project1
//
//  Created by Brendon Ng on 1/12/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#ifndef History_h
#define History_h

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    char m_grid[MAXROWS][MAXCOLS];
    int m_nRows;
    int m_nCols;
};

#endif /* History_h */
