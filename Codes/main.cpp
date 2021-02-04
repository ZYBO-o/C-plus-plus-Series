
//arctanx函数
#include<iostream>
#include <fstream>
#include <vector>
using namespace std;

//9.22



int main(){
    vector<int> Array ;

    cout << "Array size : " << Array.size() << " capacity : " << Array.capacity() << endl;

    for (vector<int>::size_type num= 0;  num < 24; num++) {
        Array.push_back(num);
    }

    cout << "Array size : " << Array.size() << " capacity : " << Array.capacity() << endl;

    Array.reserve(50);

    cout << "Array size : " << Array.size() << " capacity : " << Array.capacity() << endl;

    while (Array.size() != Array.capacity())
        Array.push_back(0);

    cout << "Array size : " << Array.size() << " capacity : " << Array.capacity() << endl;

    Array.push_back(51);

    cout << "Array size : " << Array.size() << " capacity : " << Array.capacity() << endl;


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




