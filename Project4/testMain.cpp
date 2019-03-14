//
//  testMain.cpp
//  Project4
//
//  Created by Brendon Ng on 3/11/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "provided.h"
using namespace std;

int main() {
    /*
     Trie<int> trie;
     trie.insert("GATTACA",42);
     trie.insert("GATTACA",17);
     trie.insert("GATTACA", 42);
     trie.insert("GCTTACA",30);
     trie.insert("hit",1);
     trie.insert("hit",2);
     trie.insert("hi",9);
     trie.insert("hi",17);
     trie.insert("hip",20);
     trie.insert("hip",10);
     trie.insert("hat",7);
     trie.insert("hat",8);
     trie.insert("hat",9);
     trie.insert("a",14);
     trie.insert("to",22);
     trie.insert("to",23);
     trie.insert("tap",19);
     trie.insert("tap",6);
     trie.insert("tap",32);
     
     
     vector<int> output = trie.find("hit", false);
     for(int i=0; i < output.size(); i++){
     cout << output[i] << ", ";
     }
    */
    
    
    GenomeMatcher matcher(4);
    matcher.addGenome(Genome("Genome 1", "CGGTGTACNACGACTGGGGATAGAATATCTTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC"));
    matcher.addGenome(Genome("Genome 2", "TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG"));
    matcher.addGenome(Genome("Genome 3", "TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA"));
    
    std::vector<DNAMatch> matches;
    bool result;
    
    result = matcher.findGenomesWithThisDNA("GAAG", 5, true, matches);

    
    cout << "Result: " << result << endl;
    for(int i =0; i< matches.size(); i++){
        cout << matches[i].genomeName << " of length " << matches[i].length << " at position " << matches[i].position << endl;
    }
    
    
}




