## 资源管理

<!-- GFM-TOC -->

* [以对象管理资源](#13以对象管理资源)
* [以资源管理类中小心copying行为](#14以资源管理类中小心copying行为)
* [在资源管理类中提供原始资源访问](#15在资源管理类中提供原始资源访问)
* [成对使用new和delete时要采取相同形式](#16成对使用new和delete时要采取相同形式)
* [以独立语句将newed对象置入智能指针](#17以独立语句将newed对象置入智能指针)

<!-- GFM-TOC -->

### 13.以对象管理资源

+ **为了防止资源泄漏，使用RAII思想，它们在构造函数中获得资源，并在析构函数中释放资源。**

+ **两个常用的RAII类是shared_ptr和auto_ptr，但前者一般是更佳的选择，因为其copy行为比较直观，若选择auto_ptr，复制动作会使它指向null。**

从这个条款开始，就进入到资源管理部分了。资源管理往往是大型项目的一个难点，也是重中之重，看到一些编程规范，都是将资源管理的规范列为高优先级的重点。

管理资源的最好方法其实是预防，而好的预防方法就是尽量不去使用C/C++的原生指针，这些指针像幽灵一样，一个“忘记”，就是一个隐患。当项目小的时候，这些隐患看不出来，但当研发一个拥有上万级用户的产品时，服务器对很多人同时运行含有隐患的代码，这个隐患就会爆发，导致内存不足而崩溃。

```c++
int *p = new int();
 …
delete p;
```

这个当然是OK的，内存可以回收，但万一中间的过程超过数十行，你还能记住去delete p吗？

有些同学可能会这样做，每一个new之后，先写好delete，再往中间插代码，但如果遇到这样的情况：

```c++
int* GetResource()
 {
  return new int(10);
 }
```

这个函数负责生成资源，并将之返回给上一层，因为这个资源是要在上一层用的，所以在函数内还不能直接delete，这可就麻烦了。程序员们可要随时记挂着这个内存，在不再需要的时候赶紧释放了。但一切可能出错的地方真的会出错，代码一多，记性就不那么好了，很容易出现忘记delete的事情。

那么有没有什么好的方法，可以让我们痛快地管理资源呢？这就牵涉到 **智能指针** 的问题了。常用的智能指针有memory头文件里面的`auto_ptr`，还有`boost`库里的`shared_ptr`。两者的实现机制不同，但功能是类似的，就是能 **自动管理资源** 。他们体现的思想就是`RAII`（Resources Acquisition Is Initialzeation **资源取得的时机便是初始化时机** ）。

举个形象的例子就是：

```c++ 
auto_ptr ap(new int(10));
```

或者是：

```c++
shared_ptr sp(new int(10));
```

可以看到， **这里根本就没有出现原生指针，而是直接将new出来的内存交给了一个“管理者”。这个管理者可以是auto_ptr，也可以是shared_ptr，他们的存在，<font color = red>使得程序员可以真的忘记delete了，当ap或者sp的生命周期结束时，他们会将资源释放出来，交还给系统。</font>** 比如：

```c++
void fun()
{
auto_ptr<int> ap(new int(10));
*ap = 20;
}
// ap 在fun()结束后会自动回收掉
```

`auto_ptr`和`shared_ptr`的区别在于管理方法不同。

`auto_ptr`这个管理者是一个很霸气的boss，他只想独管资源，而不允许其他管理者插手，否则他就退出管理，把这个资源交给另一方，自己再也不碰了。

```c++
auto_ptr<int> boss1(new int(10)); // 这个时候boss1对这个资源进行管理

auto_ptr<int> boss2(boss1); 
// 这个时候boss2要插手进来，boss1很生气，所以把管理权扔给了boss2，自己就不管事了。所以boss2持有这个资源，而boss1不再持有（持有的是null）。
```

所以同一时刻，只有一个auto_ptr能管理相同的资源。

这里还是有必要解释一下这句话的，比如：

```c++
auto_ptr<int> boss1(new int(10)); 
auto_ptr<int> boss2(new int(10));
```

boss1还持有资源吗？答案是有的，因为new执行了两次，虽然内存空间里的初始值是一样的，但地址并不同，所以不算相同的资源。
再来，为了把问题讲清楚，这里就让原生指针再出场一下:

```c++
int *p = new int(10);
 auto_ptr<int> boss1(p);
 auto_ptr<int> boss2(p);
```

boss1还持有资源吗？

答案是当程序运行到第三句的时候，就弹出`assertion failed`的红叉，程序崩溃了，为什么会这样呢？ **<font color = red>因为p所指向的是同一块资源，所以boss2发出管理手段后，boss1肯定要有动作的。</font>第三句话调用boss2的构造函数，但构造函数中boss2识别出来p所指向的资源已经被占用了，所以会`assertion failed`。**

**事实上，这种原生指针和智能指针混用的程序是非常不好的，如果不使用原生指针，上面的崩溃问题就不会发生。**

> C++ Primer中就说过，切记不要混用原生指针与智能指针。

根据`auto_ptr`的思想，仿写了一下这个类，有兴趣的读者可以看看，其实并不复杂：

```c++
#ifndef MY_AUTO_PTR_H
#define MY_AUTO_PTR_H

// 自定义的智能指针
#include <iostream>
using namespace std;

template <class T>
class MyAutoPtr
{
private:
    T *ptr;
    static void swap(MyAutoPtr &obj1, MyAutoPtr &obj2)
    {
        // 细到交换成员变量
        std::swap(obj1.ptr, obj2.ptr);
    };

public:
    explicit MyAutoPtr(T *p = NULL): ptr(p){}

    MyAutoPtr(MyAutoPtr& p)
    {
        ptr = p.ptr;
        p.ptr = 0;
    }

    MyAutoPtr& operator= (MyAutoPtr& p)
    {
        if(&p != this)
        {
            MyAutoPtr temp(p);
            swap(*this, temp);
        }
        return *this;
    }

    ~MyAutoPtr()
    {
        delete ptr;
        ptr = 0;
    }

    T& operator* () const
    {
        return *ptr;
    }

    T* operator-> () const 
    {
        return ptr;
    }

    T* get() const
    {
        return ptr;
    }


    friend ostream& operator<< (ostream& out, const MyAutoPtr<T>& obj)
    {
        out << *obj.ptr;
        return out;
    }

};


#endif /* MY_AUTO_PTR_H */
```

因为auto_ptr常常发生管理权的交付，所以用作形参的时候一定要小心：

若有一个函数是：

```c++
void fun(auto_ptr<int> ap){}
```

在主函数中调用`fun(main_ap);`，` main_ap`就会失去管理权了。

正是因为这个特性，所以**<font color = red>`auto_ptr`不支持STL容器，因为容器要求“可复制”，但`auto_ptr`只能有一个掌握管理权。另外，`auto_ptr`也不能用于数组，因为内部实现的时候，用的是`delete`，而不是`delete[]`。</font>**

`shared_ptr`则顾名思义，他是一个分享与合作的管理者，他的内部实现是引用计数型的，每多一次引用，计数值就会+1，而每一次引用的生命周期结束时，计数值就会-1，当计数值为0的时候，说明内存没有管理者管理了，最后一个管理这个内存的管理者就会将之释放。

再回到我们之前的例子：

```c++
shared_ptr<int> boss1(new int(10)); // 这个时候boss1对这个资源进行管理
shared_ptr<int> boss2(boss1); // boss2携手boss1对这个资源进行管理
```

注意这个时候boss1并没有交出管理权，boss2的加入只对对象内部的计数指针造成了影响，在外部就像什么也没发生一样，可以使用boss1和boss2来管理资源。

下面写上我自己写的`shared_ptr`，也很简单的（注意swap函数要自己去写，不能调用系统自带的，否则会陷入赋值运算符的无限递归中）：

```c++
#ifndef MY_SHARED_PTR_H
#define MY_SHARED_PTR_H

#include <iostream>
using namespace std;

template <class T>
class MySharedPtr
{
private:
    T *ptr;
    size_t *count;
    static void swap(MySharedPtr& obj1, MySharedPtr& obj2)
    {
        std::swap(obj1.ptr, obj2.ptr);
        std::swap(obj1.count, obj2.count);
    }

public:
    MySharedPtr(T* p = NULL): ptr(p), count(new size_t(1)){}

    MySharedPtr(MySharedPtr& p): ptr(p.ptr), count(p.count)
    {
        ++ *p.count;
    }

    MySharedPtr& operator= (MySharedPtr& p)
    {
        if(this != &p && (*this).ptr != p.ptr)
        {
            MySharedPtr temp(p);
            swap(*this, temp);
        }
        return *this;
    }

    ~MySharedPtr()
    {
        reset();        
    }

    T& operator* () const
    {
        return *ptr;
    }

    T* operator-> () const 
    {
        return ptr;
    }

    T* get() const 
    {
        return ptr;
    }

    void reset()
    {
        -- *count;
        if(*count == 0)
        {
            delete ptr;
            ptr = 0;
            delete count;
            count = 0;
            //cout << "真正删除" << endl;
        }
    }

    bool unique() const
    {
        return *count == 1;
    }

    size_t use_count() const 
    {
        return *count;
    }


    friend ostream& operator<< (ostream& out, const MySharedPtr<T>& obj)
    {
        out << *obj.ptr;
        return out;
    }

};

#endif /* MY_SHARED_PTR_H */
```



---

### 14.以资源管理类中小心copying行为

+ 复制`RAII`对象必须一并复制它所管理的资源，所以资源copying行为决定`RAII`对象的copying行为
+ 普遍而常见的`RAII` class copying行为是：抑制copying，施行引用计数法（`shared_ptr`思想），或者是转移底部资源（`auto_ptr`思想）

首先来看一个例子：

```c++
#include <iostream>
using namespace std;

class Lock
{
public:
    explicit Lock(int* pm): m_p(pm)
    {
        lock(m_p);
    }

    ~Lock()
    {
        unlock(m_p);
    }

private:
    int *m_p;
    void lock(int* pm)
    {
        cout << "Address = " << pm << " is locked" << endl;
    }

    void unlock(int *pm)
    {
        cout << "Address = " << pm << " is unlocked" << endl;
    }
};


int main()
{
    int m = 5;
    Lock m1(&m);
}
```

这个是模仿原书中的例子，做的一个加锁和解锁的操作。

运行结果如下：

```shell
Address = 0x7ffee5e8baa8 is locked
Address = 0x7ffee5e8baa8 is unlocked
```

这符合预期，当m1获得资源的时候，将之锁住，而m1生命周期结束后，也将资源的锁释放。

注意到Lock类中 **有一个指针成员，那么如果使用默认的析构函数、拷贝构造函数和赋值运算符，很可能会有严重的bug。**

我们不妨在main函数中添加一句话，变成下面这样：

```c++
int main()
{
    int m = 5;
    Lock m1(&m);
    Lock m2(m1);//拷贝构造函数中没有进行lock()操作
}
```

结果如下：

```c++
Address = 0x7ffeeb192aa8 is locked
Address = 0x7ffeeb192aa8 is unlocked
Address = 0x7ffeeb192aa8 is unlocked
```

可见，锁被释放了两次，这就出问题了。

原因是析构函数被调用了两次，在main()函数中生成了两个Lock对象，分别是m1和m2，`Lock m2(m1)`这句话使得`m2.m_p = m1.m_p`，这样这 **两个指针就指向了同一块资源。** **<font color = red>根据后生成的对象先析构的原则，所以m2先被析构，调用他的析构函数，释放资源锁，但释放的消息并没有通知到m1，所以m1在随后析构函数中，也会释放资源锁。</font>**

如果这里的释放不是简单的一句输出，而是真的对内存进行操作的话，程序就会崩溃。

归根到底，是程序使用了默认了拷贝构造函数造成的（当然，如果使用赋值运算的话，也会出现相同的bug），那么解决方案就是 **围绕如何正确摆平这个拷贝构造函数（和赋值运算符）。**

+ **方法一：** 很简单直观，就是干脆 **不让程序员使用类似于Lock m2(m1)这样的语句，一用就报编译错。** 这可以通过自己写一个私有的拷贝构造函数和赋值运算符的声明来解决。注意这里只要写声明就行了（见条款6）。

  ```c++
  class Lock
  {
  public:
      explicit Lock(int* pm): m_p(pm)
      {
          lock(m_p);
      }
  
      ~Lock()
      {
          unlock(m_p);
      }
  
  private:
      int *m_p;
      void lock(int* pm)
      {
          cout << "Address = " << pm << " is locked" << endl;
      }
  
      void unlock(int *pm)
      {
          cout << "Address = " << pm << " is unlocked" << endl;
      }
  
  private:
      Lock(const Lock&);
      Lock& operator= (const Lock&);
  };
  ```

  这样编译就不会通过了。当然也可以像书上写的一样，写一个`Uncopyable`的类，把它作为基类。在基类中把它的拷贝构造函数和赋值运算写成私有的（为了防止生成基类的对象，但又想允许派生类生成对象，可以把构造函数和析构函数的修饰符变成protected）。

+ **方法二：** 就是使用`shared_ptr`来进行资源管理（见前一个条款），但还有一个问题，我想在生命周期结束后调用Unlock的方法，其实`shared_ptr`里面的删除器可以帮到我们。

  ```c++
  class Lock
  {
  public:
      explicit Lock(int *pm): m_p(pm, unlock){…}
  private:
      shared_ptr<int> m_p;
  }
  ```

  这样在Lock的对象的生命周期结束后，就可以自动调用unlock了。

  在条款十三的基础上，我改了一下自定义的`shared_ptr`，使之也支持删除器的操作了，代码如下：

  ```c++
  #ifndef MY_SHARED_PTR_H
  #define MY_SHARED_PTR_H
  
  #include <iostream>
  using namespace std;
  
  typedef void (*FP)();    
  
  template <class T>
  class MySharedPtr
  {
  private:
      T *ptr;
      size_t *count;
      FP Del; // 声明一个删除器
      static void swap(MySharedPtr& obj1, MySharedPtr& obj2)
      {
          std::swap(obj1.ptr, obj2.ptr);
          std::swap(obj1.count, obj2.count);
          std::swap(obj1.Del, obj2.Del);
      }
  
  public:
      MySharedPtr(T* p = NULL): ptr(p), count(new size_t(1)),Del(NULL){}
  
      // 添加带删除器的构造函数
      MySharedPtr(T* p, FP fun): ptr(p), count(new size_t(1)), Del(fun){}
  
      MySharedPtr(MySharedPtr& p): ptr(p.ptr), count(p.count), Del(p.Del)
      {
          ++ *p.count;
      }
  
      MySharedPtr& operator= (MySharedPtr& p)
      {
          if(this != &p && (*this).ptr != p.ptr)
          {
              MySharedPtr temp(p);
              swap(*this, temp);
          }
          return *this;
      }
  
      ~MySharedPtr()
      {
          if(Del != NULL)
          {
              Del();
          }    
          reset();
      }
  
      T& operator* () const
      {
          return *ptr;
      }
  
      T* operator-> () const 
      {
          return ptr;
      }
  
      T* get() const 
      {
          return ptr;
      }
  
      void reset()
      {
          -- *count;
          if(*count == 0)
          {
              delete ptr;
              ptr = 0;
              delete count;
              count = 0;
              //cout << "真正删除" << endl;
          }
      }
  
      bool unique() const
      {
          return *count == 1;
      }
  
      size_t use_count() const 
      {
          return *count;
      }
  
      friend ostream& operator<< (ostream& out, const MySharedPtr<T>& obj)
      {
          out << *obj.ptr;
          return out;
      }
  
  };
  #endif /* MY_SHARED_PTR_H */
  ```

+ **方法三：** 复制底部资源，就是 **将原来的浅拷贝转换成深拷贝，需要自己显示定义拷贝构造函数和赋值运算符。** 这个也在之前的条款说过了，放到这里，其实就是在拷贝的时候对锁的计数次数进行+1，析构函数里就是对锁的计数次数进行-1，如果减到0就去unlock（其实思想还是类似于shared_ptr进行资源管理）

+ **方法四：** 是转移底部资源的控制权，这就是`auto_ptr`干的活了，在第二个方法中把`shared_ptr`换成`auto_ptr`就行了.



---

### 15.在资源管理类中提供原始资源访问

+ **一些应用程序接口往往要求访问到原始资源，所以每一个RAII类都应该提供一个“取得所管理之资源”的方法**

+ **对原始资源的访问可能经由显式或隐式转换。一般而言显式转换比较安全**

这个MySharedPtr已经可以进行资源管理了，当资源的生命周期结束后，可以自动将之删除。但对于使用者来说，还是会遇到一些问题，比如：

```c++
void PrintIntPointerValue(int* p)
{
    cout << *p << endl;
}

int main()
{
    MySharedPtr<int> ptr(new int(2));
    PrintIntPointerValue(ptr);
}
```

因为`PrintIntPointerValue`函数需要原始指针，但MySharedPtr类中并没有提供这一个接口，所以如果像上面程序一样强行使用，就会报编译错。

解决的方法其实挺简单的，就是在 **类中添加一个名为`get()`的函数** ，这个函数返回原始指针。也许会有人质疑这种做法，因为这样原始指针的阴影又会浮现出来，它提供了一个让用户可能出错的接口。

但书上有句话说的好：**<font color = red>"一个好的class应是“隐藏了客户不需要看的部分，但备妥客户需要的所有东西"。</font>**

除了`get()`外，也可以使用隐式转换函数：

```c++
operator T*() const
{
     return ptr;
}
```

但这种方法往往会使程序陷入漏洞之中，比如：

```c++
MySharedPtr<int> sharedPtr(new int(3));

int *ordinaryPtr = sharedPtr; 
// 这样编译会通过，但一旦sharedPtr释放资源后，ordinaryPtr便会成为悬空指针c
//（ordinaryPtr并会使引用计数加一，也不会参与到资源释放的操作中来）
```

```c++
shared_prt<Investment> pInv=(createInvestment());
dayHeld(pInv.get());
```

所以一般而言，提供一个`get()`函数是比较好的选择。另外，为了方便访问，还应该重载`*`和`->`操作符，使得MySharedPtr对象的操作与原始指针的操作一样方便，如下所示：

```c++
T& operator* () const
{
    return *ptr;
}

T* operator-> () const 
{
    return ptr;
}

T* get() const 
{
    return ptr;
}
```



---

### 16.成对使用new和delete时要采取相同形式

+ **如果你在new表达式中使用[]，必须在相应的delete表达式中也使用[]。如果你在new表达式中不使用[]，一定不要在相应的delete表达式中使用[]。**

这个条款比较好理解，就是new有两种形式：

```c++
int *p = new int(); // 申请一个int资源
int *p = new int[3]; // 申请连续三块int资源

//而delete也有两种资源：
delete p; // delete指针所指向的单一资源
delete [] p; // delete指针所指向的多个连续资源块
```

那么该各用哪一种形式呢？一个简单又好记的规则就是：

在`new`如果使用了`[]`，那么`delete`也去用`[]`；反之则都不用。一句话，都用或者都不用。

但事实上，如果用错了，编译器不会给任何提示，也没有运行错，这些行为是“未定义的”。这可能会产生严重的后果，因为这会造成内存回收不彻底的bug。

上面是一种直观的形式，如果换成稍稍复杂一些，比如：

```c++
typedef string AddressLines[4];
string *pal = new AddressLines;
```

那么该用`delete pal`还是`delete []pal`呢？

注意这里用了`typedef`，`AddressLines`是相当于`string[4]`，`pal`指向4个连续string内存段的首地址，所以这里应该用`delete[] pal`。

这样看来，与我们之前说的简记规则又矛盾了，因为new的时候并没有出现[]，而delete却有了[]。

书上对此作了解释， **"最好尽量不要对数组形式做typedef动作"** ，应该用C++标准程序库中的类似vector来做，这样就可以避免出错了。


---

### 17.以独立语句将newed对象置入智能指针

举书上的例子：

```c++
int GetPriority();
 void processWidget(shared_ptr<Widget> pw, int priority);
```

如果这样调用processWidget：

```c++
processWidget(new Widget(), GetPriority());
```

注意一下两边的类型，左边是管理者`shared_ptr`类，而右边是被管理者`Widget`类，两边类型不等，编译器会试图执行类型隐式转换，但为了安全起见，`shared_ptr`的构造函数前面有修饰符`explicit`，它不允许发生隐式转换，所以编译器会报错。

但如果这样：

```c++
processWidget(shared_ptr pw(new Widget), GetPriority());
```

就可以成功调用构造函数了。

**但这里可能会发生内存泄露，** 不知道读者有没有发现。千万不要认为`GetPriority()`是个配角，它的存在恰好是我们下面要讨论的。

函数参数压栈的顺序与编译器有关，这里的实参到形参的传递过程其实是不确定的，**即`GetPriority()`的调用很可能发生在`new Widget`之后，而在`pw(new Widget)`之前，这样<font color = red>万一`GetPriority`抛出了异常，而`pw`还没有来得及接管`new Widget`，这样就没有可能释放`Widget`资源了</font>** 。也就是说，如果是按这个顺序执行的话：

(1) new Widget

(2) GetPriority() 并将返回值传给priority

(3) pw接管newWidget，而后传给函数形参

就可能出现内存泄露。

那么怎样解决这个问题呢？

答案就是读书笔记的标题了： **以独立语句将new产生的对象置入智能指针（因为“在资源被创建和资源被管理这两个时间点之间有可能发生异常干扰）** 。简言之，就是一条语句不要执行太多的操作，特别是这种内存管理的操作，分成几条独立的语句会安全一些。应该这样：

```c++
shared_ptr<Widget> pw(new Widget);
processWidget(pw, GetPriority());
```

这样不管函数是以怎样的顺序传参，在第一句的时候就保证了资源的成功接管，即使`GetPriority()`发生异常，`shared_ptr`也能及时释放掉资源，从而避免内存泄露。




---

