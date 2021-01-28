
//arctanx函数
#include<iostream>
using namespace std;

void changeValue(int *p_value)
{
    int i = 3;

    cout<< "In function, p_value = " << *p_value <<" and Pp_value = " << p_value <<endl;

    *p_value = 4;
    cout<< "In function, After Change1 ; p_value = " << *p_value <<" and Pp_value = " << p_value <<endl;

    p_value = &i;
    cout<< "In function, After Change2 ; p_value = " << *p_value <<" and Pp_value = " << p_value <<endl;
}

int main(){
    int a = 1;
    cout<< "a = " << a <<" and p_a = " << &a <<endl;
    changeValue(&a);
    cout<< "After function, a = " << a <<" and p_a = " << &a <<endl;
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


/***
//将8位二进制数字转换为十进制数字
#include<iostream>
using namespace std;
double power(double x,int n);//计算x的n次方
int main(){

    return 0;
}

***/

