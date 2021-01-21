#include <iostream>
#include <string>
#include <vector>

using namespace std;




int main() {

    vector<int> v1(10);

    cout<< v1.size() << endl;

    for (int i = 0; i < 15; ++i) {
        v1.push_back(i);
    }

    cout<< v1.size() << endl;

    for (auto i : v1) {
        cout << i << endl;
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







    return 0;
}
