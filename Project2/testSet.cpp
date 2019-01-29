//
//  testSet.cpp
//  Project2
//
//  Created by Brendon Ng on 1/18/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include "Set.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
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
    std::cout << 5 << std::endl;
    ss.printForward();
    ss.printBackward();
    
    Set ss1;
    ss1.insert("laobing");
    std::cout << 1 << std::endl;
    ss1.printForward();
    ss1.printBackward();
    Set ss2;
    ss2.insert("matzo");
    ss2.insert("pita");
    ss1.swap(ss2);
    assert(ss1.size() == 2);
    assert(ss1.contains("matzo")  &&  ss1.contains("pita"));
    assert(ss2.size() == 1);
    assert(ss2.contains("laobing"));
    
    std::cout << 1 << std::endl;
    ss2.printForward();
    ss2.printBackward();
    
    std::cout << 2 << std::endl;
    ss1.printForward();
    ss1.printBackward();
    
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
    string v;
    assert(ss3.get(1, v)  &&  v == "focaccia");
    assert(ss3.get(0, v)  &&  v == "");
    
    
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



