//
//  main.cpp
//  HW5
//
//  Created by Brendon Ng on 3/14/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include <iostream>

struct Node{
    int value;
    Node* left;
    Node * right;
};

class Tree{
public:
    Tree(int rootval) {
        root = new Node();
        root->value = rootval;
        root->left = nullptr;
        root->right = nullptr;
    }
    ~Tree() {
        
    }
    void insert(Node* cur, int value) {
        if (value == cur-> value)
            return;
        else if(value < cur->value){
            if(cur->left == nullptr){
                cur-> left = new Node();
                (cur->left)->value = value;
                (cur->left)->right = nullptr;
                (cur->left)->left = nullptr;
                return;
            }
            else{
                insert(cur->left, value);
            }
        }
        else if(value > cur->value){
            if(cur->right == nullptr){
                cur-> right = new Node();
                (cur->right)->value = value;
                (cur->right)->right = nullptr;
                (cur->right)->left = nullptr;
                return;
            }
            else{
                insert(cur->right, value);
            }
        }
    }

    Node* root;
    
    void print(Node* cur) {
        if(cur == nullptr )
            return;
        
        print(cur->left);
        
        print(cur->right);
        std::cout << cur->value << " ";
    }
    
};


int main()
{
    Tree tre(50);
    tre.insert(tre.root, 20);
    tre.insert(tre.root, 60);
    tre.insert(tre.root, 10);
    tre.insert(tre.root, 40);
    tre.insert(tre.root, 70);
    tre.insert(tre.root, 30);
    
    tre.insert(tre.root, 80);
    tre.insert(tre.root, 65);
    tre.insert(tre.root, 72);
    tre.insert(tre.root, 15);
    tre.insert(tre.root, 38);
    tre.insert(tre.root, 25);
    
    tre.print(tre.root);
}
