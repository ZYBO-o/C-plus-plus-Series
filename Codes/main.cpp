
//arctanx函数
#include<iostream>
using namespace std;

class Sales_date
{
public:
    Sales_date(string s, unsigned int cnt, double price):bookNo(s), count(cnt), revenue(price)
    {
        cout<< "This is pure function" << endl;
    }

    Sales_date(): Sales_date("",0,0)
    {
        cout<< "This is function1" << endl;
    }

    Sales_date(string s): Sales_date(s,0,0)
    {
        cout<< "This is function2" << endl;
    }
    Sales_date(istream &is): Sales_date()
    {
        cout<< "This is function3" << endl;
    }
    
private:
    string bookNo;
    unsigned int count;
    double revenue;
};

int main(){

    Sales_date s;
    Sales_date s1("hello");
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

