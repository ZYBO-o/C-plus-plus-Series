
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

class numbered
{
private:
    static int seq;
public:
    numbered() {mysn = ++seq;}

    //13.15
    numbered(numbered &n) {mysn = ++seq;}
    int mysn;
};

int numbered::seq = 0;

void f(const numbered &s)
{
    cout << s.mysn << endl;
}

int main(){
    numbered a, b = a, c = b;
    f(a);
    f(b);
    f(c);

    return 0;

}





