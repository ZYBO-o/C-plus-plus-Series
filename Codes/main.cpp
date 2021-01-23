
//arctanx函数
#include<iostream>
using namespace std;
double arctan(double x){


    double sqr = x*x;

    double e = x;

    double r = 0;

    int i = 1;
    while(e/i > 1e-15){

        double f = e/i;
        r = (i%4==1) ? r+f : r-f;
        e = e*sqr;
        i+=2; 
    }
    return r;
}


int main()
{

    double a=16.0*arctan(2);
    double b=4.0*arctan(1/239.0);
    //整数相除结果取整，如果参数写1/5，1/239，结果就都是0
    cout<<"PI = "<<a-b<<endl;





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

