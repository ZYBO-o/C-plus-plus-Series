
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
using namespace std;

//9.22


void elimDups(vector<string> &words)
{
    //按照字典序排序words,以便查找重复单词
    sort(words.begin(), words.end());
    //unique重排输入范围，使得每个单词只出现一次
    //排序在范围的前部，返回指向不重复区域之后的一个位置的迭代器
    auto end_unique = unique(words.begin(), words.end());
    //使用向量操作erase删除重复单词
    words.erase(end_unique, words.end());
}

bool isShorter(const string &s1, const string &s2)
{
    return s1.size() < s2.size();
}

bool check_size(const string &s, string::size_type sz)
{
    return s.size() >= sz;
}

//计数并按字典序打印vector<string>中长度>=给定值的string
void biggies(vector<string> &words, vector<string>::size_type sz){
    //按字典排序并消除重复
    elimDups(words);
    //对字符串长度做稳定排序，长度相同的单词维持字典序
    //用lambda做二元谓词比较两元素
    stable_sort(words.begin(),words.end(),
                [](const string &a, const string &b){return a.size()<b.size();});
    //找到第一个长度>=sz的元素
    //用lambda做一元谓词比较元素和变量
    auto wc=find_if(words.begin(),words.end(), [sz](const string &a){return a.size()>=sz;});
    //计算长度>=sz的元素数目
    auto count=words.end()-wc;
    cout<<count<<" "<< ((count>1)?("words"):("word") ) <<" of length "<<sz<<" or longer" <<endl;
    //打印长度>=sz的元素，每个元素后接一个空格
    //用lambda遍历元素
    for_each(wc,words.end(),
             [](const string &s){cout<<s<<" ";});
    cout<<endl;
}



int main(){

    list<int> l1 {1,2,3,4};
    list<int> l2 {10,20};
    list<int> l3 {10,20};


    copy(l1.begin(),l1.end(),front_inserter(l2));

    copy(l1.begin(),l1.end(),inserter(l3,l3.begin()));

    for (int i : l2)
        cout << i << " ";
    cout << endl;
    for (int i : l3)
        cout << i << " ";
    cout << endl;





    /*
    vector<string> words {"fox", "jumps", "the", "over", "turtle", "quick", "red", "red", "slow", "the"};
    biggies(words,4);

    auto wc = find_if(words.begin(), words.end(), bind(check_size, std::placeholders::_1, 4));

    auto check6 = bind(check_size,std::placeholders::_1,6);
    string s = "hello";

    bool b1 = check6(s);

    cout << b1 << endl;
*/


    //cout << words.end() - wc << endl;
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





