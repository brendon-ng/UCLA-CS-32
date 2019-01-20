//
//  Set.cpp
//  Homework1
//
//  Created by Brendon Ng on 1/18/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include "Set.h"

Set::Set()
{
    m_size = 0;
}

bool Set::empty() const
{
    return (m_size==0);
}

int Set::size() const
{
    return m_size;
}

bool Set::insert(const ItemType& value)
{
    if (DEFAULT_MAX_ITEMS==m_size)
        return false;
    for(int i=0; i<m_size; i++)
    {
        if(value > m_array[i])
            continue;
        else if(value == m_array[i])
            return false;
        else{
            for(int j=m_size; j>i; j--)
            {
                m_array[j] = m_array[j-1];
            }
            m_array[i] = value;
            m_size++;
            return true;
        }
    }
    m_array[m_size] = value;
    m_size++;
    return true;
}

bool Set::erase(const ItemType& value)
{
    int hi = m_size-1;
    int lo = 0;
    int i;
    while(hi >= lo)
    {
        i = (hi+lo)/2;
        if(value > m_array[i])
            lo = i+1;
        else if(value < m_array[i])
            hi = i-1;
        else // if they are equal
        {
            for(int j=i; j<m_size-1; j++)
                m_array[j] = m_array[j+1];
            m_size--;
            return true;
        }
    }
    return false;
}

bool Set::contains(const ItemType& value) const
{
    int hi = m_size-1;
    int lo = 0;
    while(hi >= lo)
    {
        int i = (hi+lo)/2;
        if(value > m_array[i])
            lo = i+1;
        else if(value < m_array[i])
            hi = i-1;
        else // if they are equal
            return true;
    }
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    if(i < 0 || i >= m_size)
        return false;
    value = m_array[i];
    return true;
}

void Set::swap(Set& other)
{
    Set temp = other;
    other = *this;
    *this = temp;
//    ItemType* otherArray = other.m_array;
//    int otherSize = other.size();
//
//    ItemType* thisArray = m_array;
//    int thisSize = m_size;
//
//    ItemType* temp = otherArray;
//    otherArray = thisArray;
//    thisArray = temp;
//    m_size = otherSize;
//    other.m_size = thisSize;
}

void Set::print(){
    for(int i=0; i< m_size;i++){
        std::cout<<m_array[i]<< std::endl;
    }
}


