//
//  eval.cpp
//  Homework2
//
//  Created by Brendon Ng on 2/3/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

#include "Set.h"  // with ItemType being a type alias for char
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int getPrecedence(char ch);

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
    //CHECK SYNTAX AND CONVERT TO POSTFIX
    if(infix.length() == 0)
        return 1;
    stack<char> charStack;
    string post = "";
    bool lastCharOperand = false;
    for(int i=0; i<infix.size(); i++) {
        //If its an operand
        if(isalpha(infix[i]) && islower(infix[i]) && !lastCharOperand){
            post+=infix[i];
            lastCharOperand = true;
        }
        //If its a '(' or '!'
        else if((infix[i] == '(' || infix[i] == '!') && !lastCharOperand){
            charStack.push(infix[i]);
        }
        //If its an operator and the stack is empty
        else if((infix[i]=='&' || infix[i]=='|') && charStack.empty() && lastCharOperand){
            charStack.push(infix[i]);
            lastCharOperand = false;
        }
        //If its an operator and the stack is NOT empty
        else if((infix[i]=='&' || infix[i]=='|') && !charStack.empty() && lastCharOperand){
            while(!charStack.empty()){
                int cur = getPrecedence(infix[i]);
                if(charStack.top()=='(')                        // If we encounter a '(', then stop
                    break;
                else if (charStack.top() == '!') {              // If there is a '!' remaining, append
                    post += charStack.top();                    // and pop
                    charStack.pop();
                }
                else if(getPrecedence(charStack.top()) >= cur){ // Pop all operators w >= precedence
                    post+=charStack.top();                      // and append them on the postfix string
                    charStack.pop();
                }
                else                                            // If lower precedence, stop
                    break;
            }
            charStack.push(infix[i]);                           //Push the new operator on the stack
            lastCharOperand = false;
        }
        //If we encounter a ')'
        else if (infix[i] == ')' && lastCharOperand) {
            bool matchingParen = false;
            while(!charStack.empty()) { //pop operators until we reach a matching '('
                if(charStack.top() == '(') {
                    charStack.pop();
                    matchingParen = true;
                    // If a '!' precedes the opening '(' append it and pop it
                    while(!charStack.empty() && charStack.top()=='!'){
                        post += charStack.top();
                        charStack.pop();
                    }
                    break;
                }
                post += charStack.top();
                charStack.pop();
            }
            if (!matchingParen)
                return 1; //If it reaches here, it has not found a matching (
        }
        //If we encounter a space, ignore it
        else if (infix[i] == ' ')
            continue;
        //If all these cases fail it is invalid
        else
            return 1;
    }
    //Pop remaining operators and append them
    while(!charStack.empty()){
        if(charStack.top() == '(') {
            // If a '!' precedes the opening '(' append it and pop it
            while(!charStack.empty() && charStack.top()=='!'){
                post += charStack.top();
                charStack.pop();
            }
            charStack.pop();
            continue;
        }
        post += charStack.top();
        charStack.pop();
    }
    // If the last character in the string was an operator followed by no operand, it is invalid
    if(!lastCharOperand)
        return 1;
    
    postfix = post;
    
    
    //EVALUATE EXPRESSION
    stack<int> tokenStack;
    for(int i=0; i<post.size(); i++){
        //If the token is an operand
        if(isalpha(post[i])) {
            if(trueValues.contains(post[i]) && falseValues.contains(post[i]))//If token is in both lists
                return 3;
            else if (trueValues.contains(post[i])){
                tokenStack.push(1);
            }
            else if (falseValues.contains(post[i])){
                tokenStack.push(0);
            }
            else    // If token is in neither list
                return 2;
        }
        // If token is '&' operator
        else if (post[i] == '&'){
            int v2 = tokenStack.top();
            tokenStack.pop();
            int v1 = tokenStack.top();
            tokenStack.pop();
            if (v1 == 1 && v2 == 1)
                tokenStack.push(1);
            else
                tokenStack.push(0);
        }
        // If token is '|' operator
        else if (post[i] == '|'){
            int v2 = tokenStack.top();
            tokenStack.pop();
            int v1 = tokenStack.top();
            tokenStack.pop();
            if (v1 == 1 || v2 == 1)
                tokenStack.push(1);
            else
                tokenStack.push(0);
        }
        // If token is '!' then pop the top value and push its complement
        else if (post[i] == '!'){
            int top = tokenStack.top();
            tokenStack.pop();
            if (top == 1)
                tokenStack.push(0);
            else
                tokenStack.push(1);
        }
        else // Something will have gone wrong
            return 1;
    }
    
    if (tokenStack.top() == 1)
        result = true;
    else
        result = false;

    return 0;
}

int getPrecedence(char ch)
{
    switch (ch) {
    case '|':
        return 1;
        break;
    case '&':
        return 2;
        break;
    default: // something will have gone wrong
        return -999;
        break;
    }
}


int main()
{
    string trueChars  = "tywz";
    string falseChars = "fnx";
    Set trues;
    Set falses;
    for (int k = 0; k < trueChars.size(); k++)
        trues.insert(trueChars[k]);
    for (int k = 0; k < falseChars.size(); k++)
        falses.insert(falseChars[k]);
    
    string pf;
    bool answer;
    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  answer);
    assert(evaluate("y|", trues, falses, pf, answer) == 1);
    assert(evaluate("n t", trues, falses, pf, answer) == 1);
    assert(evaluate("nt", trues, falses, pf, answer) == 1);
    assert(evaluate("()", trues, falses, pf, answer) == 1);
    assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
    assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
    assert(evaluate("", trues, falses, pf, answer) == 1);
    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
           &&  pf == "ff!tn&&|"  &&  !answer);
    assert(evaluate(" x  ", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  !answer);
    trues.insert('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 3);
    falses.erase('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  answer);
    trues.erase('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 2);
    falses.insert('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  !answer);
    
    
    
    
    string str0 = "a|b|a";
    string str1 = "a|(b|a)";
    Set s;
    string str;
    bool temp;
    evaluate(str0, s, s, str, temp);
    assert(str == "ab|a|");
    evaluate(str1, s, s, str, temp);
    assert(str == "aba||");
    
    Set specTestTrue;
    Set specTestFalse;
    specTestTrue.insert('u');
    specTestTrue.insert('c');
    specTestTrue.insert('l');
    specTestTrue.insert('a');
    specTestTrue.insert('t');
    specTestFalse.insert('n');
    specTestFalse.insert('s');
    specTestFalse.insert('x');
    specTestFalse.insert('f');
    
    assert(evaluate("u", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("u&c&l&a & !(u&s&c)", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("(n)", specTestTrue, specTestFalse, str, temp)==0 && temp == false);
    assert(evaluate("a&(s)", specTestTrue, specTestFalse, str, temp)==0 && temp == false);
    assert(evaluate("a & !!!s", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("!!!(n|u)", specTestTrue, specTestFalse, str, temp)==0 && temp == false);
    assert(evaluate("!!!n|u", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("a|n&n", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("a&!!!!!!!!!(s|u&c|n)|!!!!!!!(s&u&n)", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("!!!!!(s&u&n)|(a&!!!!!!!(s|u&c|n))", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("a&!!!!!(s|u&c|n)|!!!(s&u&n)", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("a&!!!(s|u&c|n)", specTestTrue, specTestFalse, str, temp)==0 && temp == false);
    assert(evaluate("!!!!!(s&u&n)", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("a&!!!!!!!(s|u&c|n)|!!!!(s&u&n)", specTestTrue, specTestFalse, str, temp)==0 && temp == false);
    assert(evaluate("", specTestTrue, specTestFalse, str, temp)==1);
    assert(evaluate("t&!!!!(s|u&c|n)&!!!f&(!!(f)|!(t&f))", specTestTrue, specTestFalse, str, temp)==0 && temp == true);
    assert(evaluate("a&!!!(s|U&c|n)", specTestTrue, specTestFalse, str, temp)==1);
    assert(evaluate("a&!!!(s!|u&c|n)", specTestTrue, specTestFalse, str, temp)==1);
    assert(evaluate(")a&!!!(s|u&c|n)", specTestTrue, specTestFalse, str, temp)==1);
    assert(evaluate("a&!!!(s|u&c|n)!", specTestTrue, specTestFalse, str, temp)==1);
    assert(evaluate("a&!!!(s.|u&c|n)", specTestTrue, specTestFalse, str, temp)==1);
    

    
    cout << "Passed all tests" << endl;
    
}

