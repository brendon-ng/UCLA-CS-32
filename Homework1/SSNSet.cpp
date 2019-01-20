//
//  SSNSet.cpp
//  Homework1
//
//  Created by Brendon Ng on 1/19/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include "SSNSet.h"
#include <iostream>
using namespace std;

SSNSet::SSNSet()
{
    
}

bool SSNSet::add(unsigned long ssn)
{
    return m_set.insert(ssn);
}

int SSNSet::size() const
{
    return m_set.size();
}

void SSNSet::print() const
{
    for(int i=0; i<m_set.size(); i++){
        unsigned long value;
        m_set.get(i, value);
        cout << value << endl;
    }
}


