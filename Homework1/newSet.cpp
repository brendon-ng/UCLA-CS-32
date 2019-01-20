//
//  newSet.cpp
//  Homework1
//
//  Created by Brendon Ng on 1/18/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include "newSet.h"

Set::Set()
{
    m_size = 0;
    m_capacity = DEFAULT_MAX_ITEMS;
    m_array = new ItemType[DEFAULT_MAX_ITEMS];
}

Set::Set(int capacity)
{
    if(capacity<0){
        // END PROGRAM SOMEHOW
    }
        
    m_capacity = capacity;
    m_size = 0;
    m_array = new ItemType[m_capacity];
}

Set::Set(const Set& src) // Copy Constructor
{
    m_size = src.m_size;
    m_capacity = src.m_capacity;
    m_array = new ItemType[m_capacity];
    for(int i=0; i<m_size; i++){
        m_array[i] = src.m_array[i];
    }
}

Set::~Set()  // Destructor
{
    delete [] m_array;
}

Set& Set::operator= (const Set& src)  // Assignment Operator
{
    if(&src == this)
        return *this;
    
    delete [] m_array;
    m_size = src.m_size;
    m_capacity = src.m_capacity;
    m_array = new ItemType[m_capacity];
    for(int i=0; i<m_size; i++){
        m_array[i] = src.m_array[i];
    }
    return *this;
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
    if (m_size >= m_capacity)
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

    ItemType* otherArray = other.m_array;
    int otherSize = other.size();
    int otherCapacity = other.m_capacity;

    other.m_array = m_array;
    other.m_size = m_size;
    other.m_capacity = m_capacity;
    
    m_array = otherArray;
    m_size = otherSize;
    m_capacity = otherCapacity;


}

void Set::print(){
    for(int i=0; i< m_size;i++){
        std::cout<<m_array[i]<< std::endl;
    }
}


