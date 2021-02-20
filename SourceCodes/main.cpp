
//arctanx函数
#include<iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <functional>
#include <iterator>
#include <set>
#include <map>
#include <memory>
using namespace std;

class HasPtr{
public:
    //构造时分配动态内存
    explicit HasPtr(const string &s=string()):ps(new string(s)),i(0) {}

    //拷贝构造时也分配动态内存并拷贝数据，保证各对象资源独立
    HasPtr(const HasPtr &p): ps(new string(*p.ps)),i(p.i) {}

    HasPtr &operator=(const HasPtr &);

    ~HasPtr(){delete ps;}

    string getString() {return *ps;}

private:
    string *ps;
    int i;
};
//定义拷贝赋值算符，保证自赋值安全、异常安全
HasPtr &HasPtr::operator=(const HasPtr &rhs){
    auto newp = new string(*rhs.ps);  //1、将右侧对象的资源拷出
    delete ps;                      //2、释放左侧对象的资源
    ps=newp;                        //3、让左侧对象接管拷出的资源
    i=rhs.i;
    return *this;
}
int main(){


    auto *s = new string("OK");
    string s1 = "hello";

    cout << *s << endl;

    delete s;
    s = &s1;
    cout << *s << endl;

    return 0;

}





