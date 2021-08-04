# C++11 并发编程

## (一).



## (二).



## (三). 线程传参详解

### 1.传递临时对象作为线程参数

 ```c++
#include <iostream>
#include <thread>
using namespace std;

void myprint(const int& i, char* pmybuf) {
    cout << i << endl;
    cout << pmybuf << endl;
}

void Test3() {
    int mvar = 1;
    int &mvary  = mvar;
    char mybuf[] = "this is a test";
    thread mytobj(myprint,mvar,mybuf);
    mytobj.detach();
    cout << "this is test3 function" << endl;
}
 ```

+ **注意事项1：**

  + i 不是真的引用，是把mvar复制了临时对象给i，是值传递。它们的地址是不同的。所以detach()中使用引用是安全的，但不推荐使用。

  + 指针在detach子线程中，绝对会有问题，因为地址相同，当test3()先执行结束的话，myprint中的pmybuf就为野指针了。

  + 把形参改成`const string&`类型，两者的地址是不同的，似乎是变安全了。但是如果test3()先运行结束，系统还用mybuf转换成string，也是会出错。所以也是不稳定的。

    > 把`thread mytobj(myprint,mvar,mybuf);`中的mybuf修改为string(mybuf)就安全了。
    >
    > 会在传值之前构造出string(mybuf)，然后传递给函数，进行值传递，所以会引发构造函数与拷贝构造函数，生成两个对象。

+ **事实1：只要用临时构造对象作为参数传递给线程，那就一定能够在主线程执行完毕之前把线程函数的参数构造出来，确保detach()的子线程能够安全运行。**
+ **<font color = red>传递int类型时，避免使用引用传递，使用值传递</font>**
+ **<font color = red>传递对象时，避免隐式类型转换。在创建线程时进行构建临时对象。在函数参数中，使用引用来接否则会构造出三个对象(多构造一次)。</font>**
+ **<font color = red>建议不使用detach()，否则会容易出现内存问题。</font>**

### 2.临时对象  作为线程参数拓展

#### (1).线程id概念

+ 每个线程都有一个独自的线程id。
+ 使用`std::this_thread::get_id()`来获取。

#### (2).临时对象构造实际抓捕

+ 使用线程id进行抓捕发现：
  + 如果是使用隐式转换，则函数中的形参对象是在子线程中构造的，所以当使用detach()时，如果主线程先执行完毕，则子线程构造时会出现内存问题
  + 如果使用临时对象，则函数中形参对象是在主线程中构造，则不会出现内存问题。

### 3.传递类对象，智能指针作为线程参数

+ 之前的引用传递是不彻底的引用，如果在线程中使用真正的引用传递，则需要`std::ref`来进行修饰。

  ```c++
  thread mytobj(myprint,std::ref(mvar),mybuf);
  ```

  > 这种方法不会再出现拷贝构造函数。

+ 使用智能指针unique_ptr时，传递需要`std::move`；而且为了内存安全，只能使用join，不能使用detach。

### 4.用成员函数指针做线程函数

+ 是用函数指针作为线程函数的格式

  ```c++
  std::thread mytobj(&A::thread_work, myobj, 15);
  ```

  

