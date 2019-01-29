//
//  Set.h
//  Project2
//
//  Created by Brendon Ng on 1/18/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#ifndef Set_h
#define Set_h

#include <iostream>
using ItemType = std::string;
//using ItemType = unsigned long;

const int DEFAULT_MAX_ITEMS = 250;

class Set
{
public:
    Set();         // Create an empty set (i.e., one with no items).
    
    ~Set();         // Destructor
    
    Set(const Set& src);    // Copy Constructor
    
    Set& operator= (const Set& src);    // Assignment Operator
    
    bool empty() const;  // Return true if the set is empty, otherwise false.
    
    int size() const;    // Return the number of items in the set.
    
    bool insert(const ItemType& value);
    // Insert value into the set if it is not already present.  Return
    // true if the value was actually inserted.  Leave the set unchanged
    // and return false if the value was not inserted (perhaps because it
    // was already in the set or because the set has a fixed capacity and
    // is full).
    
    bool erase(const ItemType& value);
    // Remove the value from the set if present.  Return true if the
    // value was removed; otherwise, leave the set unchanged and
    // return false.
    
    bool contains(const ItemType& value) const;
    // Return true if the value is in the set, otherwise false.
    
    bool get(int i, ItemType& value) const;
    // If 0 <= i < size(), copy into value the item in the set that is
    // strictly greater than exactly i items in the set and return true.
    // Otherwise, leave value unchanged and return false.
    
    void swap(Set& other);
    // Exchange the contents of this set with the other one.
    
    void printForward();
    void printBackward();
    
private:
    struct Node {
        Node(ItemType value, Node* next, Node* prev) {
            m_value = value;
            m_next = next;
            m_prev = prev;
        }
        ItemType m_value;
        Node* m_next;
        Node* m_prev;
    };
    
    int m_size;
    Node* head;
    Node* tail;
};

#endif /* Set_h */

