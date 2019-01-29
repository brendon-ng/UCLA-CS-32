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
    
    
    Set numbers1;
    assert(numbers1.empty());

    ItemType numbers[17] = {"1","8","4","7","1","9","0","0","9","2","3","6","5","\0"};
    int j=0;
    while(numbers[j] != "\0"){
        numbers1.insert(numbers[j]);
        j++;
    }
    assert(numbers1.size() == 10);

    
    Set numbers2;
    ItemType numbersTens[17] = {"10","80","40","70","10","90","90","20","30","60","50","\0"};
    j=0;
    while(numbersTens[j] != "\0"){
        numbers2.insert(numbersTens[j]);
        j++;
    }
    assert(numbers2.size() == 9);
   
    
    Set numbers3 = numbers1;
    for(int i=0; i<10 ;i++){
        ItemType one;
        numbers1.get(i, one);
        ItemType two;
        numbers3.get(i, two);
        assert(one==two);
    }
    
    assert(numbers1.size() == numbers3.size());
    assert(numbers1.erase("0"));
    assert(numbers1.size() == 9);
    assert(numbers1.erase("9"));
    assert(numbers1.size() == 8);
    assert(numbers1.size() != numbers3.size());

   
    numbers3 = numbers2;
    for(int i=0; i<9 ;i++){
        ItemType two;
        numbers2.get(i, two);
        ItemType three;
        numbers3.get(i, three);
        assert(two==three);
    }
    
    assert(numbers2.size() == numbers3.size());
    assert(numbers2.erase("10"));
    assert(numbers2.size() == 8);
    assert(numbers2.erase("90"));
    assert(numbers2.size() == 7);
    assert(numbers2.size() != numbers3.size());

    
    
    
    
    Set unite1;
    ItemType uniteOne[17] = {"2","8","3","9","5","\0"};
    j=0;
    while(uniteOne[j] != "\0"){
        unite1.insert(uniteOne[j]);
        j++;
    }
    Set unite2;
    ItemType uniteTwo[17] = {"6","3","8","5","10","\0"};
    j=0;
    while(uniteTwo[j] != "\0"){
        unite2.insert(uniteTwo[j]);
        j++;
    }
    Set united;
    unite(unite1, unite2, united);
    assert(united.size() == 7);
    assert(united.contains("9"));
    assert(united.contains("3"));
    assert(united.contains("5"));
    assert(united.contains("6"));
    assert(united.contains("10"));
    assert(united.contains("2"));
    assert(united.contains("8"));
    
    subtract(unite1, unite2, united);
    assert(united.size() == 2);
    assert(united.contains("2"));
    assert(united.contains("9"));
   
    
    unite(unite1, unite2, united);
    assert(united.size() == 7);
    assert(united.contains("9"));
    assert(united.contains("3"));
    assert(united.contains("5"));
    assert(united.contains("6"));
    assert(united.contains("10"));
    assert(united.contains("2"));
    assert(united.contains("8"));
    
    
    
    
    cout << "Passed all tests" << endl;
    
}



