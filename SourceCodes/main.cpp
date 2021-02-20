
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
#include <fstream>
#include <memory>
using namespace std;



int main(){
    unique_ptr<string> p1(new string("Stegosaurus"));


    auto p = p1.release(); //将p1置空并将底层的内置指针交给p2管理


    delete p;


}





