
//arctanx函数
#include<iostream>
using namespace std;

double sum(initializer_list<double> il){
    double sum = 0;
    for (auto tmp : il)
        sum += tmp;
    return sum;
}

double average(const initializer_list<double> &ril){
    if (ril.size() > 0){
        double sum = 0;
        for (auto tmp : ril)
            sum += tmp;
        return sum / ril.size();
    }
    return 0.0;
}

int main(){
    cout << "sum:" << sum({ 2, 3, 4 }) << ", ave:" << average({ 2.0, 3, 4 }) << endl;
    initializer_list<double> dl = { 1.5, 2.5, 3.5, 4.5, 5.5 };
    cout << "sum:" << sum(dl) << ", ave:" << average(dl) << endl;
    dl = { 1, 2, 3, 4, 5, 6 };
    cout << "sum:" << sum(dl) << ", ave:" << average(dl) << endl;
    system("pause");
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

