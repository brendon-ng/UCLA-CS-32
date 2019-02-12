//
//  linear.cpp
//  Homework3
//
//  Created by Brendon Ng on 2/10/19.
//  Copyright © 2019 Brendon Ng. All rights reserved.
//


//#include <iostream>
//using namespace std;
//
//bool somePredicate(double x)
//{
//    return x > 0;
//}


// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
    if(n <= 0)  //Base Case
        return false;
    
    if(somePredicate(a[0])) // If this element is true, return true or else check rest of the array
        return true;
    else
        return anyTrue(a+1, n-1);
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
    if(n <= 0)  //Base Case
        return false;
    
    if(somePredicate(a[0]))     // If current element is true, add one to total from the rest of array
        return 1 + countTrue(a+1, n-1);
    else
        return countTrue(a+1, n-1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
    if(n<=0)    //Base Case
        return -1;
    
    int past = firstTrue(a, n-1);   //Check if there's been a true prior to index n
    if(somePredicate(a[n-1]) && past == -1) {
        return n-1;
    }
    else
        return past;
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.
int positionOfMin(const double a[], int n)
{
    //Base Cases
    if(n <= 0)
        return -1;
    if(n==1)
        return 0;
    
    int prevMin = positionOfMin(a, n-1);    //Index of the previously checked minimum
    if(a[n-1] < a[prevMin])
        return n-1;
    else
        return prevMin;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if(n1 < n2) //Base case: If we have run out of a1 elements to match to
        return false;
    if(n2 == 0) //Base case: If there are no n2 elements, it is true
        return true;

    if(a1[0] == a2[0])
        return includes(a1+1, n1-1, a2+1, n2-1);//If the first element matches, we have cleared the
                                                //next a2 element, move on to the next element in both
    else
        return includes(a1+1, n1-1, a2, n2);    //If it doesn't match move forward in only a1
}


//int main(){
//    double n1[7] = {10., 50., 40., 20., 50., 40., 30.};
//    double n2[3] = {10., 20., 20.};
//    cout << includes(n1, 7, n2, 3) << endl;
//}

