
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

class StrBlob
{
public:
    typedef vector<string>::size_type  size_type;
    StrBlob();
    StrBlob(initializer_list<string> il);
    size_type size() const { return data->empty(); }

    void push_back(const string &t) { data->push_back(t); };
    void pop_back();

    string& front();
    string& back();

private:
    shared_ptr<vector<string >> data;
    void check(size_type i, const string &msg) const;
};

StrBlob::StrBlob() : data(make_shared<vector<string>>()) { }
StrBlob::StrBlob(initializer_list<string> il) : data(make_shared<vector<string>>(il)) {}

//检查下标是否越界
void StrBlob::check(size_type i, const string &msg) const {
    if(i>=data->size())
        throw out_of_range(msg);
}
//以下3个函数分别实现front、back、pop_back操作，用0来check索引判断是否为空
string &StrBlob::front(){
    check(0,"front on empty StrBlob");
    return data->front();
}
string &StrBlob::back(){
    check(0,"back on empty StrBlob");
    return data->back();
}
void StrBlob::pop_back(){
    check(0,"pop_back on empty StrBlob");
    data->pop_back();
}

int main(){

    /* 使用StrBlob */
    StrBlob b1;                         //创建新StrBlob
    {                                   //进入新作用域
        StrBlob b2={"a","an","the"};    //初始化b2
        b1=b2;                          //用b2初始化b1，它们共享底层数据
    }                                   //离开作用域，b2被释放，b1仍存在，共享的底层数据未丢失
    while(b1.size()>0){
        cout<<b1.back()<<endl;
        b1.pop_back();
    }
}





