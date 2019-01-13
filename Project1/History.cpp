//
//  History.cpp
//  Project1
//
//  Created by Brendon Ng on 1/12/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include "History.h"
#include <iostream>

using namespace std;


History::History(int nRows, int nCols){
    m_nCols = nCols;
    m_nRows = nRows;
    for(int r = 0; r<nRows; r++)
        for(int c =0; c<nCols; c++)
            m_grid[r][c] = '.';
    
}

bool History::record(int r, int c){
    if(r<1 || r>m_nRows || c<1 || c>m_nCols)
        return false;
    
    if(m_grid[r-1][c-1] == '.')
        m_grid[r-1][c-1] = 'A';
    else if (m_grid[r-1][c-1] == 'Z')
        return true;
    else
        m_grid[r-1][c-1]++;
    
    return true;
}

void History::display() const{
    clearScreen();
    for(int r = 0; r<m_nRows; r++){
        for(int c =0; c<m_nCols; c++)
            cout << m_grid[r][c];
        cout << endl;
    }
    cout << endl;
}
