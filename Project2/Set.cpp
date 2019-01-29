//
//  Set.cpp
//  Project2
//
//  Created by Brendon Ng on 1/18/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include "Set.h"

Set::Set()
{
    m_size = 0;
    head=nullptr;
    tail=nullptr;
}

Set::~Set()
{
    Node* cur = head;
    while(cur != nullptr){
        Node* next = cur->m_next;
        delete cur;
        cur = next;
    }
}

Set::Set(const Set& src)
{
    m_size=src.m_size;
    if(src.head==nullptr){
        head = nullptr;
        tail = nullptr;
    }
    else {
        Node* srcCur = src.head;
        head = new Node(srcCur->m_value, nullptr, nullptr);
        Node* cur = head;
        while(srcCur->m_next != nullptr){
            cur->m_next = new Node(srcCur->m_next->m_value, nullptr, cur);
            srcCur = srcCur->m_next;
            cur = cur->m_next;
        }
        tail = cur;
    }
}

Set& Set::operator=(const Set &src)
{
    if(&src == this)    // If this and src's addresses are the same, change nothing and return *this
        return *this;
    
    // Destruct all nodes in the linked list
    Node* cur = head;
    while(cur != nullptr){
        Node* next = cur->m_next;
        delete cur;
        cur = next;
    }
    
    Node* srcCur = src.head;
    head = srcCur;
    while (srcCur != nullptr)   // Fill new linked list
    {
        insert(srcCur->m_value);
        tail = srcCur;
        srcCur = srcCur->m_next;
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
    if(m_size == 0)     // If the linked list has no elements
    {
        head = new Node(value, nullptr, nullptr);
        tail = head;
        m_size++;
        return true;
    }
    else if (value < head->m_value) // If value is before the first node alphabetically, add to front
    {
        Node* temp = head;
        head = new Node(value, temp, nullptr);
        temp->m_prev = head;
        m_size++;
        return true;
    }
    else if (value == head -> m_value)
        return false;
    else {
        Node* cur = head;
        while(cur->m_next != nullptr) { // Traverse to node directly above where to insert
            if(cur->m_next->m_value > value)
                break;
            else if (cur->m_next->m_value == value)
                return false;
            
            cur = cur->m_next;
        }
        Node* temp = new Node(value, cur->m_next, cur);  // Allocate new node
        
        if(cur->m_next != nullptr)  // If we are not at the end of the linked list
            cur->m_next->m_prev = temp;
        else
            tail = temp;
        
        cur->m_next = temp;
        m_size++;
        return true;
    }
}

bool Set::erase(const ItemType& value)
{
    if(m_size == 0)  // If empty linked list
        return false;
    else if (head->m_value == value)    // If we wish to delete the first node
    {
        Node* del = head;
        head = head->m_next;
        
        if(head != nullptr)     // Set previous pointer, keep in mind case of the end of the list
            head->m_prev = nullptr;
        else
            tail = head;
        
        delete del;
        m_size--;
        return true;
    }
    else{
        Node* cur = head;
        while (cur->m_next != nullptr) {
            if(cur->m_next->m_value == value){
                Node* del = cur->m_next;
                cur->m_next = cur->m_next->m_next;
                if(cur->m_next != nullptr)      // Set prev pointer, keep in mind case of end of list
                    cur->m_next->m_prev = cur;
                else
                    tail = cur;
                
                delete del;
                m_size--;
                return true;
            }
            cur = cur->m_next;
        }
        return false;
    }
}

bool Set::contains(const ItemType& value) const
{
    Node* cur = head;
    while(cur != nullptr){
        if(cur->m_value == value)
            return true;
        cur = cur->m_next;
    }
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    if(i < 0 || i >= m_size)
        return false;
    Node* cur = head;
    while (i>0) {
        cur = cur->m_next;
        i--;
    }
    value = cur->m_value;
    return true;
}

void Set::swap(Set& other)
{
    Node* otherHead = other.head;
    Node* otherTail = other.tail;
    int otherSize = other.m_size;
    
    other.head = head;
    other.tail = tail;
    other.m_size = m_size;
    
    head = otherHead;
    tail = otherTail;
    m_size = otherSize;
}



