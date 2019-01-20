//
//  testnewSet.cpp
//  Homework1
//
//  Created by Brendon Ng on 1/18/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include "newSet.h"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
    
    Set a(1000);   // a can hold at most 1000 distinct items
    Set b(5);      // b can hold at most 5 distinct items
    Set c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
    ItemType v[6] = {"hello","world","test","new","set","class" };
    
    // No failures inserting 5 distinct items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(v[k]));
    
    // Failure if we try to insert a sixth distinct item into b
    assert(!b.insert(v[5]));
    
    // When two Sets' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(v[5])  &&  b.insert(v[5]));

    
    Set ss;
    ss.insert("lavash");
    ss.insert("roti");
    ss.insert("chapati");
    ss.insert("injera");
    ss.insert("roti");
    ss.insert("matzo");
    ss.insert("injera");
    assert(ss.size() == 5);  // duplicate "roti" and "injera" were not added
    string x;
    ss.get(0, x);
    assert(x == "chapati");  // "chapati" is greater than exactly 0 items in ss
    ss.get(4, x);
    assert(x == "roti");  // "roti" is greater than exactly 4 items in ss
    ss.get(2, x);
    assert(x == "lavash");  // "lavash" is greater than exactly 2 items in ss
    
    Set ss1;
    ss1.insert("laobing");
    Set ss2;
    ss2.insert("matzo");
    ss2.insert("pita");
    ss1.swap(ss2);
    assert(ss1.size() == 2);
    assert(ss1.contains("matzo")  &&  ss1.contains("pita"));
    assert(ss2.size() == 1);
    assert(ss2.contains("laobing"));
    
    Set ss3;
    ss3.insert("dosa");
    assert(!ss3.contains(""));
    ss3.insert("tortilla");
    ss3.insert("");
    ss3.insert("focaccia");
    assert(ss3.contains(""));
    ss3.erase("dosa");
    assert(ss3.size() == 3  &&  ss3.contains("focaccia")  &&  ss3.contains("tortilla")  &&
           ss3.contains(""));
    string v0;
    assert(ss3.get(1, v0)  &&  v0 == "focaccia");
    assert(ss3.get(0, v0)  &&  v0 == "");
    
    
    Set s;
    assert(s.empty());
    ItemType y = "arepa";
    assert( !s.get(42, y)  && y == "arepa"); // x unchanged by get failure
    s.insert("chapati");
    assert(s.size() == 1);
    assert(s.get(0, y)  &&  y == "chapati");
    
    
    Set s2;

    ItemType x1[17] = {"a","k","b","l","c","p","i","d","o","e","j","f","g","m","h","n","0"};
    int i=0;
    while(x1[i] != "0"){
        s2.insert(x1[i]);
        i++;
    }
    assert(s2.size()==16);
    assert(s2.insert("p") == false);
    assert(s2.insert("l") == false);
    assert(s2.insert("h") == false);
    assert(s2.erase("z") == false);
    assert(s2.size()==16);
    assert(s2.insert("z") == true);
    assert(s2.size()==17);
    assert(s2.erase("c") == true);
    assert(s2.size() == 16);
    assert(s2.insert("kangaroo") == true);
    assert(s2.contains("kangaroo") == true);
    ItemType v1;
    s2.get(10, v1);
    assert(v1=="kangaroo");

    
    Set s3;
    s3.insert("bruin");
    s3.insert("kangaroo");
    s3.insert("zebra");
    s3.insert("arangatang");
    s3.insert("dodo bird");
    s3.insert("asteroid");
    assert(s3.insert("") == true);
    assert(s3.size()==7);
    assert(s3.contains("") == true);
    s3.erase("dodo bird");
    assert(s3.contains("dodo bird") == false);
    

    
    s3.swap(s2);
    
    assert(s2.size()==6);
    assert(s2.contains("dodo bird") == false);
    

    s3.get(10, v1);
    assert(v1=="kangaroo");
    assert(s3.contains("kangaroo") == true);
    assert(s3.size()==17);
    
    
    cout << "Passed all tests" << endl;
    
}


