//
//  tree.cpp
//  Homework3
//
//  Created by Brendon Ng on 2/11/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//

//#include <iostream>
//using namespace std;

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then for this value of a2     the function must return
//    10 20 40            1
//    10 40 30            2
//    20 10 40            0
//    50 40 30            3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if(n1 < n2) //Base case: If we have run out of a1 elements to match to
        return 0;
    if(n2 <= 0) //Base case: If there are no n2 elements, it is true
        return 1;
    
    
    //If first elements match, keep searching and also search without moving on to the next a2 element
    if(a1[0] == a2[0])
        return countIncludes(a1+1, n1-1, a2+1, n2-1) + countIncludes(a1+1, n1-1, a2, n2);
    else
        return countIncludes(a1+1, n1-1, a2, n2);    //If it doesn't match move forward in only a1
}

// Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > divider come before all the other elements,
// and all the elements whose value is < divider come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= divider, or n if there is no such element, and firstLess is
// set to the index of the first element that is < divider, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > divider
//   * for firstNotGreater <= i < firstLess, a[i] == divider
//   * for firstLess <= i < n, a[i] < divider
// All the elements > divider end up in no particular order.
// All the elements < divider end up in no particular order.
void divide(double a[], int n, double divider,
            int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;
    
    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > divider
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == divider
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < divider
    
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < divider)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > divider)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
    if(n<=1)
        return;
    int firstNotGreater;
    int firstLess;
    divide(a, n, a[n/2], firstNotGreater, firstLess); //sort and split the array at the divider
    
    // Order the two sides of the divided array
    order(a, firstNotGreater);
    order(a+firstLess,n-firstLess);
}


//int main(){
//    double arr[7] = {10., 50., 40., 20., 50., 40., 30.};
//    double a1[3] = {10., 20., 40.};
//    double a2[3] = {10., 40., 30.};
//    double a3[3] = {20., 10., 40.};
//    double a4[3] = {50., 40., 30.};
//
//    cout << countIncludes(arr, 7, a1, 3) << endl;
//    cout << countIncludes(arr, 7, a2, 3) << endl;
//    cout << countIncludes(arr, 7, a3, 3) << endl;
//    cout << countIncludes(arr, 7, a4, 3) << endl;
//    order(arr, 7);
//    order(a1, 3);
//    order(a2, 3);
//    order(a3, 3);
//    order(a4, 3);
//    for(int i=0;i<7;i++){
//        cout << arr[i] << " ";
//    }
//    cout << endl;
//    for(int i=0;i<3;i++){
//        cout << a1[i] << " ";
//    }
//    cout << endl;
//    for(int i=0;i<3;i++){
//        cout << a2[i] << " ";
//    }
//    cout << endl;
//    for(int i=0;i<3;i++){
//        cout << a3[i] << " ";
//    }
//    cout << endl;
//    for(int i=0;i<3;i++){
//        cout << a4[i] << " ";
//    }
//    cout << endl;
//
//}
//
