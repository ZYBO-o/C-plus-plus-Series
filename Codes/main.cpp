
//arctanx函数
#include<iostream>
#include <fstream>
#include <vector>
using namespace std;


vector<int>::iterator search_vec(vector<int>::iterator  beg, vector<int>::iterator end, int value)
{
    while (beg != end)
        if(*beg == value)
            return beg;
        else
            beg++;
    return end;
}

int main(){
    vector<int> Array {1,2,3,4,5,6,7,8,9};

    cout <<  search_vec(Array.begin(), Array.end(), 5) - Array.begin() << endl;

    return 0;
}

    /*
    const double p1 = 3.14;
    const double p2 = 4.33;
    double p3 = 5.44;

    //错误，ptr1只是普通指针
    //double *ptr1 = &p1;

    //指向常量的指针
    const double *ptr2 = &p1;
    //不能给*ptr2进行赋值
    //*ptr2 = 1.1;

    //正确，可以修改指针
    ptr2 = &p2;
    cout << *ptr2 << endl;
    //*ptr2 = 6.66;

    //正确
    const double * const ptr3 = &p1;
    //错误
    //ptr3 = &p3;

    //正确
    const double  const *ptr4 = &p1;
    //正确
    ptr4 = &p3;
    cout << *ptr4 << endl;

    double * const ptr5 = &p3;

     */




