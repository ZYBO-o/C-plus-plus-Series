## 构造/析构/赋值运算

<!-- GFM-TOC -->

* [5.了解C++默认编写并调用哪些函数](#5了解c默认编写并调用哪些函数)

* [6.若不想使用编译器自动生成的函数，就该明确拒绝](#6若不想使用编译器自动生成的函数就该明确拒绝)

+ [7.为多基类声明virtual析构函数](#7为多基类声明virtual析构函数)
+ [8.别让异常逃离析构函数](#8别让异常逃离析构函数)
+ [9.绝不在构造和析构过程中调用virtual函数](#9绝不在构造和析构过程中调用virtual函数)
+ [10.令operator = 返回一个reference to * this](#10令operator--返回一个reference-to--this)

+ [11.在operator = 中处理"自我赋值"](#11在operator--中处理自我赋值)
+ [12.复制对象时勿忘其每一个成分](#12复制对象时勿忘其每一个成分)

<!-- GFM-TOC -->

---

### 5.了解C++默认编写并调用哪些函数

**编译器可以暗自为class创建default构造函数，copy构造函数，copy assignment操作符以及析构函数。**

一个常见的面试题就是问**<font color = red>"一个空类占几个字节"</font>**，想当然的是0个字节，但事实上类要区分不同的对象，比如：

```c++
EmptyClass obj1;
EmptyClass obj2;
```

即便是空类，也要能识别obj1和obj2的不同，所以空类仍然要占字节数，**一般占一个字节**。

> "To ensure that the addresses of two different objects will be different." 就用那1byte来在内存中占用不同地址了。

还有一个针对空类的问题是**<font color = red>"一个空类里面有什么"</font>**，就是想问编译器为这个空类自动生成了哪些成员函数。
很容易想到的是生成了默认的构造函数和析构函数，事实上还有拷贝构造函数和赋值运算符，所以，总共生成了四个成员函数。具体地说，就是你表面上写了 Class EmptyClass{}；但实际编译器为你加了四个成员函数，所以看起来像这样：

```c++
Class EmtpyClass
{
public:
//构造函数
EmtpyClass(){}
// 析构函数
~EmptyClass(){}
// 拷贝构造函数
EmptyClass(const EmptyClass& obj){ }
// 赋值运算符重载
EmptyClass& operator= (const EmptyClass& obj){ }
};
```

赋值运算符要返回自身`*this`，是因为考虑到可以出现连等的情况，比如`obj1 = obj2 = obj3`，另外，这里都使用了自身类的引用，即`EmptyClass&`，这里的引用是必须要加的，这是因为：

+ 引用修饰形参时，可以避免实参对形参的拷贝，一方面可以节省空间和时间资源，**更为重要的是<font color = red>若实参对形参拷贝了，又会调用一次拷贝构造函数，这样拷贝构造函数就会一遍又一遍的被调用，造成无穷递归。</font>**

+ 引用修饰返回值时，可以使返回的对象原地修改。比如`(a=b) ++`，这样**返回的a对象还可以进行自增操作**，如果不加引用，则因为生成的是原对象的拷贝，所以这样的自增操作并不使a本体自增。

本书中还讲到了一个特殊的情况，就是成员变量是const的，或者是引用，比如：

```c++
class SampleClass
{
private:
         const int var1;
         double& var2;
};
```

这时候编译器会报错，告诉你无法提供合适的构造函数，因为对于const变量以及reference，需要在声明的时候初始化，而编译器提供的默认构造函数显然无法做到这点(reference不能修改指向)。可以改成下面这样：

```c++
class SampleClass
{
private:
         const int var1;
         double& var2;
public:
         SampleClass(const int a = 0, double b = 0):var1(a), var2(b){}
};
```

编译器不会报错了，但是如果像这样：

```c++
SampleClass obj1;
SampleClass obj2;
obj2 = obj1;
```

编译器会提示`operator =`函数在`SampleClass`中不可用，这说明编译器同样没有为`SampleClass`生成赋值运算符，因为var1和var2在初始化后，值就不能再改变了。但：

```c++
SampleClass obj1;
SampleClass obj2(obj1);
```

却是可以编译通过的，这是因为**编译器可以生成默认的拷贝构造函数。 这种生成方式并不会破坏const和reference的特性。**

> 最后还有一种情况：如果某个base class将copy assignment操作符声明为private，编译器将拒绝为 derived classes生成一个copy assignment操作符。

综上，编译器总是尽量地去生成这四个成员函数，但**<font color = red>如果成员变量出现了const和reference，则编译器会拒绝生成默认的构造函数和赋值运算符重载函数。</font>**



---

### 6.若不想使用编译器自动生成的函数，就该明确拒绝

**为了驳回编译器自动提供的机能（比如可拷贝），可将相应的成员函数声明为private且不予实现。还有一种方法，继承Uncopyable也行。**

如果不想使用拷贝构造函数和赋值运算符，一种好的方法就将这些函数写在private或者protected里面，大概像这样：

```c++
class HomeForSale
{
private:
         HomeForSale(const HomeForSale& house){...}
         HomeForSale& operator= (const HomeForSale& house){...}
};
```

为了不让member函数与friend函数调用，可以只声明而不定义，这样调用会获得一个连接错误。但是往往不写在HomeForSale类本身，而是在一个专门为了阻止拷贝动作而设计的基类中。

```c++
class Uncopyable
{
    Uncopyable(){}
    ~Uncopyable(){}

private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator = (const Uncopyable&);
};
```

那么为了阻止HomeForSale对象被拷贝，我们唯一要做的就是继承Uncopyable：

```c++
class HomeForSale : private Uncopyable
{
public:
    ...

private:
    ...
        HomeForSale(const HomeForSale&); //只有声明
    HomeForSale& operator=(const HomeForSale&);
};
```

这样HomeForSale就不能被拷贝了。这里的继承可以不是public，比如private。

---

### 7.为多基类声明virtual析构函数

+ **带有多态性质的基类应该声明一个virtual析构函数。如果class带有任何virtual函数，它就应该拥有一个virtual析构函数；**

+ **若一个类不作为基类，或者不具备多态性（比如Uncopyable），就不该声明virtual析构函数。**

严格来说，多态分为编译时多态和运行时多态：

+ 编译时多态是函数的重载
+ 运行时多态则是迟绑定技术，即根据基类指针或引用的实际指向来决定采取何种行动

一般来说，多态特指运行时多态。

下面我们来举有关C++多态的一个简单例子:

```c++
class Shape
{
private:
         int color;
public:
         virtual void print() = 0;
};

class Circle: public Shape
{
private:
         double radius;
public:
         void print(){cout << "This is a Circle" << endl;}
};
class Square: public Shape
{
private:
         double length;
public:
         void print(){cout << "This is a Square" << endl;}
};
```

多态前提是继承，如上面这个例子，Shape是基类，它有一个所有形状都具有的属性：颜色。Circle和Square派生自Shape，它们都有颜色的属性，但同时Circle还有半径的属性，Square有边长的属性。这里print()函数是一个有多态行为的函数，比如：

```c++
Shape *p1 = new Square;
p1->print(); // 这时的运行结果就是“This is a Square”
Shape *p2 = new Circle;
p2->print(); // 这时的运行结果就是“This is a Circle”
```

但这里会有一个问题，如果接下来操作：

```c++
delete p1;
delete p2;
```

就**会出现内存的泄露**，为什么会这样？

**<font color = red>因为Shape没有显式定义析构函数，编译器为我们生成的析构函数不是虚的，这时候delete p1，编译器就简单地析构了基类的部分，派生类的部分就被架空了，造成了内存的泄露。</font>**

解决方法是**基类声明virtual析构函数：**

```c++
class Shape
{
private:
         int color;
public:
         virtual void print() = 0;
         virtual ~Shape(){}
};
```

有了virtual关键字，析构时就会检查指针实际指向的对象，也就会先定位到相应的派生类，完成派生类的析构之后，再析构基类。

那么是怎么""检查"指针实际指向的对象呢？

**C++为每个含有virtual关键字的类中提供了一个虚表指针vptr，它指向一个由函数指针构成的数组，称为虚表。**程序在运行时会维护类对象中的vptr，比如：

```c++
Shape *p1 = new Square;
```

vptr就会指向Square的print()函数。

但不是所有析构函数都需要加上virtual，因为一旦出现了virtual，编译器就会生成vptr，**这个vptr本身是要占空间的，对于一个并不大的类而言，vptr所占空间（32位机上是4字节，64位机上是8字节）的百分比还是很可观的。**所以virtual要相时而加，出现多态继承关系时一定要加上，而非基类或者不用于多态的基类（比如条款6说的Uncopyable）就不要加。

另外，**所有的STL，包括string，vector等等，在设计时就没把它们当作基类，所以它们的析构函数都不是虚函数，因此试图定义一个类去继承STL是非常危险的！**

本例中Shape类已经是抽象类了，使不想生成对象的类成为抽象类，这是一个很好的技术实现方案，但万一有时候难以找到一个纯虚函数，怎么办呢？自己强行加一个像print()一样的函数固然是可以的，但不自然，**一个好的方法是<font color = red>把析构函数变成纯虚函数</font>，像这样：**

```c++
class Shape
{
private:
         int color;
public:
         virtual ~Shape() = 0;
};
```

问题算是解决一半了，还有一个问题，编译器会在Shape派生类的析构函数中创建一个对~Shape()的调用动作，如果不实现这个~Shape()，链接器会报错，一个解决方案就是简单地加上括号，像这样：

```c++
class Shape
{
private:
         int color;
public:
         virtual ~Shape() = 0{};
};
```

这个细节很需要注意。

---

### 8.别让异常逃离析构函数

+ **析构函数绝对不要吐出异常，应该在内部"消化"掉这些异常；**

+ **如果客户需要对某个操作函数运行期间抛出的异常做出反应，那么class应该提供一个普通函数，而非在析构函数中执行该操作。**

```c++
class Widget
{
public:
         ~Widget(){…} // 析构函数
};

void DoSomething()
{
         vector<Widget> v;
}
```

STL的vector在析构时会逐一调用容器内每个元素的析构函数，这样问题就来了，万一在Widget的析构函数里出现了异常，又没有及时地在析构函数中处理这个异常，导致异常被抛出，因为vector中存在多个Widget元素，这样在析构时就会多次抛出未处理的异常，多个异常同时存在是非常危险的，它会导致不明确的行为。

最好的解决方案就是在析构函数里面把异常给解决掉，有两种方法：

+ 如果抛出异常就结束程序，通常调用abort完成：

```c++
~Widget()
{
         try
         {…}
         catch
         {
            //记录出错的位置
            std::abort();
         }
}
```

+ 吞下因调用close而发生的异常：

```c++
~Widget()
{
         try
         {…}
         catch
         {
            //制作转运记录，记下对close的调用失败。
         }
}
```

记录错误总是要做的，但关于要不要出现异常就终止程序，在书上也作了一番探讨，主要在于这个程序是否可以终止，还是必须要坚持运作下去，这取决于实际的需要。

书上还举了一个例子，就是重新设计接口，让析构函数做的事情模块化成一个函数，将这个函数设置成public，交由用户自行调用，若用户忘了做这件事情，再交给析构函数做。我觉得这样太麻烦，把程序弄的冗长了，所以这里就不介绍了。

---

### 9.绝不在构造和析构过程中调用virtual函数

```c++
class Base
{
public:
         Base()
         {
                   VirtualFunction();
         }

         virtual void VirtualFunction()
         {
                   cout << "In the Base Class" << endl;
         };
};

class Derived: public Base
{
public:
         void VirtualFunction()
         {
                   cout << "In the Derived Class" << endl;
         }
};

int main()
{
         Derived d;
}
```

定义派生类对象时，会先构造基类，调用基类的构造函数，在基类的构造函数中调用了虚函数。

如果按照多态的思路，行为的执行者应该是派生类的`VirtualFunction()`，也就是输出的是`In the Derived Class`，然而实际跑一下这个程序，运行结果却是`In the Base Class `！

一种很好的理解方法就是，**派生类部分必须在基类部分构造完全之后才会去构造，因此在虚表中尚未注册派生类的`VirtualFunction()`，这时只能调用基类的`VirtualFunction()`**。对于析构函数，同样是如此，派生类部分先析构，这时基类中的虚函数将无法定位到派生类，只能调用基类自身的函数。书上指出**<font color = red>"在base class构造期间，virtual函数不是virtual函数"</font>**。

这样的结果会使我们感到困惑，与多态法则的效果不一致，**所以本书作者强调"绝不在构造和析构函数中调用virtual函数"**。

**当实例化一个派生类对象时，首先进行基类部分的构造，然后再进行派生类部分的构造。**即创建Derive对象时，会先调用Base的构造函数，再调用Derive的构造函数。当在构造基类部分时，派生类还没被完全创建，从某种意义上讲此时它只是个基类对象。即当Base::Base()执行时Derive对象还没被完全创建，此时它被当成一个Base对象，而不是Derive对象，因此Foo绑定的是Base的Foo。

在构造函数中，所有基类构造函数总是在继承类构造函数执行之前被调用，而继承类对象将在构造函数执行完之后被完整创建。如果在构造函数中调用一个虚函数，则会不会发生动态绑定呢？答案是**调用的是这个虚函数的本地版本。**在构造函数中，基类构造函数已经被执行完成，如果在这个时候通过基类引用或指针调用一个虚函数，会把对象视为base class对象，因为这个时候，继承类对象还尚未被初始化完成，所以面对它们，最安全的做法就是视它们不存在。

**所有的构造函数被调用后才会有最后派生的VTABLE，如果在构造函数中进行虚函数调用，应该使用早绑定，因为它们知道，晚绑定只会对本地函数产生调用。在构造函数中调用虚函数往往得到的结果并不是我们想要的，一般好的设计并不在构造函数中调用虚函数。**

---

### 10.令operator = 返回一个reference to * this

之所以返回的是一个引用，这并不是编译器强制的，但有三个原因觉得这样做是明智的：

+  **返回引用可以节省资源，不必要为返回值调用构造函数了；**

+ 形如(x = y) = 3这样的连等，编译器也能接受了，因为这样的写法要求赋值运算符返回的是可以修改的左值；

+ STL和boost库中的标准代码都是这样写的，为了与它们相兼容，还是放弃你的标新立异吧



---

### 11.在operator = 中处理"自我赋值"



---

### 12.复制对象时勿忘其每一个成分
