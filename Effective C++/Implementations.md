## 实现

<!-- GFM-TOC -->

* [26.尽可能延后变量定义式的出现时间](#26尽可能延后变量定义式的出现时间)
* [27.尽量少做转型动作](#27尽量少做转型动作)
* [28.避免返回handles指向对象内部成分](#28避免返回handles指向对象内部成分)
* [29.为"异常安全"而努力是值得的](#29为异常安全而努力是值得的)
* [30.透彻了解inlining的里里外外](#30透彻了解inlining的里里外外)
* [31.将文件间的编译依存关系降至最低](#31将文件间的编译依存关系降至最低)

<!-- GFM-TOC -->

---

> 大多数情况下，适当提出class与function声明，是最花费心里的两件事，一旦完成，相应的实现大多直接了当。尽管如此，还是有些东西需要小心。
>
> + 太快定义变量可能造成效率上的拖延。
> + 过度使用转型可能导致代码变慢又难维护，又招来微妙难解的错误
> + 返回对象"内部数据之号码牌(handles)"可能造成破坏封装并留给客户虚吊号码牌(dangling handls)
> + 未考虑异常带来的冲击则可能导致资源泄露和数据败坏
> + 过度热心地inlining可能引起代码膨胀
> + 过度耦合(coupling)则可能导致让人不满意的冗长建置时间(build times)
>
> 下面的六则条款是对应的解决方案

---

### 26.尽可能延后变量定义式的出现时间

+ **尽可能延后变量定义式的出现，这样做可增加程序的清晰度并改善程序效率。**

这个条款从字面意思还是很好理解的，就是在使用这个变量前才去定义，而不是很早就定义了它，而在很后面的时候才去使用。这个条款只适用于对变量声明位置没有要求的语言，比如C++。对于像C或者一些脚本语言，语法要求变量声明放在函数开始处，这个条款就不能使用了。

但其实从使用的角度而言，如果不是语法的硬性要求，还是在变量使用前再去定义变量的做法比较好。这有几点原因:

+ 最直观的就是可读性比较好，程序员在阅读代码时，看到一个陌生的变量名，不用向上翻好几页才看到它的定义类型，而且对于开发者而言，也不会出现前面定义了一个变量，之后又忘记使用它的情况。
+ 可以节省资源

考虑以下的代码：

```c++
void example(const A& parm)
{
    A a;
    fun(); // 这个fun()函数不会使用a
    a = parm;
    …
}
```

假设存在一个类A，如果在程序的开始处就定义它的对象a（调用了构造函数），中间夹了一个函数fun()， **这个函数可能抛出异常，导致下面的程序不去执行，这样对象a的构造函数就白调用了，既浪费了空间，也浪费了时间。**

所以改进的做法就是：

```c++
void example(const A& parm)
{
    fun();
    A a;
    a = parm;
    …
}
```

这下如果fun()抛出异常，那么也不会浪费资源了，但这里还是有一个比较“废”的地方， **就是调用了构造函数，也调用了赋值运算，但这两个操作的目的都是给a一个值，所以可以将这两个操作进行精减，直接调用A的拷贝构造函数** ，像这样：

```c++
void example(const A& parm)
{
    fun();
    A a(parm);
    …
}
```

赋值运算就不用调用了。
书上还讲到了一个写代码常见的问题，就是循环中的变量定义，是放在循环内比较好呢，还是放在循环外比较好呢？看下面的例子：

```c++
// 放在循环外
A a;
for(int i = 0; i < N; ++i)
{
    a = a * b[i];
    …
}
//后面不再使用a
```

```c++
// 放在循环内
for(int i = 0; i < N; ++i)
{
    A a(b[i]);
    …
}
//后面不再使用a
```

分析一下:

+ **如果采用放在循环外的方法，代价是1次构造+1次析构+N次赋值；**
+ **如果采用放在循环内的方法，代价是N次构造+N次析构。**

所以究竟哪个更好，还是要看构造函数、析构函数VS赋值操作，哪一个比较废。 **<font color = red>但如果两方的代价差不多，还是推荐放在循环内的做法，因为这种做法使得a的作用域局限于for循环内，符合作用域能小就小的编程原则。</font>**



---

### 27.尽量少做转型动作

+ **如果可以，尽量避免转型，特别是在注重效率的代码中避免dynamic_casts。**

+ **如果转型是必要的，试着将它隐藏于某个函数背后，客户可以随后调用这个函数，而不需要将转型放在他们自己的代码里。**

+ **宁可使用C++风格的新式转型，少用C风格转型，因为前者很容易辨识出来，而且也比较有着分门别类的职掌。**

转型可以按风格可以分成C风格转型和C++风格转型两大类，C风格转型很容易看到，因为我们会经常使用，像

```c++
(T) expression
```

以及：

```c++
T (expression)
```

最经典的例子就是处理整数除法，在C/C++程序中，整数除法的结果还是整数，有时会得不到我们想到的结果，比如3/5，结果是0，而不是0.6，但如果这样`double(3) / 5`，结果就会是0.6了，因为转型操作`double(3)`，将整数转成了浮点数，这样就是小数除法了，可以得到带小数点的结果。

C++风格的转型操作分成四类：

```c++
const_cast<T>(expression)
static_cast<T>(expression)
dynamic_cast<T>(expression)
reinterpret_cast<T>(expression)
```

**effective推荐使用的是前三种，最后一种是把一个指针转成整数或者把整数看成指针，对平台依赖性很强，不建议使用。**

#### const_cast

`const_cast`很简单，就是去掉常量属性，比如：

```c++
int main()
 {
     int a= 3;
    const int* ca = &a;
     int *pb = const_cast<int*> (ca);
     *pb = 5;
     cout << a << endl; // a的输出结果是5
 }
```

但这种 **<font color = red>常量性的转换只是针对于指针或引用（包括this指针），是不能针对于普通变量的</font>** ，比如下面的做法就是错误的：

```c++
 int main()
 {
     const int a = 3;
     int b = const_cast<int>(a);  //编译出错，只能使用static_cast或者C风格转换
 }
```

对于`const_cast`，既可以将`const->非const`，也可以将`非const->const`，只要记住两点:

**其一，这个操作只对指针或引用有效**

**其二，<font color = red>这个操作并不改变被转换对象的常属性</font>**

```c++
 int a= 3;
 const int* ca = &a;
 int *pb = const_cast<int*> (ca);
 //ca仍是指向常量的指针，对它的操作，比如*ca = 5，是会报编译错的。
```

#### static_cast

`static_cast`是最为常用C++转型了，我们常见的`int->double`亦或是`float->int`等等，都是用`static_cast`来进行转换的（包括`const int -> int`，以及`int -> const int`， **只要不是常指针/引用->non常指针/引用** ）。

#### dynamic_cast

`dynamic_cast`是当想把基类指针转成派生类指针时用，这种转换可以保证安全性，当把两个不相干的类之间用`dynamic_cast`时，转换的结果将是空指针，同时若基类指针指向的对象只是基类本身时，对基类指针进行`dynamic_cast`向下转型到派生类，会得到空指针，防止进一步的操作。

介绍到这里，需要我们回答一个问题，既然旧式转型已经够用了（毕竟C语言就是这样用的，没有人抱怨转型的功能不够丰富），那为什么C++还要再定义功能重复的转型呢？

原因有两个：

+ **第一，细化转型使它们很容易在代码中被辨识出来，可以不去看上下文就知道这样的转型是什么类型的（常量转型还是向下转型）**
+ **第二，各转型动作的目标愈窄化，编译器愈可能诊断出错误的应用**

下面来讲一下，为什么effective中要我们尽量少做转型动作。

不清楚原理的转型会带来严重的bug，比如书上提到的，假设有一个基类Window，它有一个派生类SpecialWindow，它们都有一个`OnResize()`的成员函数，在派生类的`OnResize()`中想要先调用基类的`OnResize()`，于是有程序员便这样做了：

```c++
void SpecialWindow::OnResize()
{
	static_cast(*this).OnResize();
	…
}
```

这样真的没有问题吗？将派生类对象转成Window对象，然后对其调用OnResize()，这确确实实调用的是基类的OnResize()，但OnReszie()对成员变量操作的结果是你想要的吗？

答案是否定的， **因为static_cast生成的是一个临时的基类的对象，这个对象并不是真正组成派生类的那个基类，而是它的一个副本（<font color = red>这个副本的成员变量值与派生类对象中基类的成分是一样的，但地址不同</font>），调用OnResize()变更的结果是这个副本的成员变量变了，但派生类中包含的基类的成员变量却是没有任何变化的。** 好了，这就是bug了，在之后的测试中这个问题会让程序员纠结好一阵子。

一句话，转型生成的是一个copy，一份副本，有的时候这并不是你想要的，修正方法其实很简单，就是：

```c++
void SpecialWindow::OnResize()
{
	Window::OnResize(); // OK了，就这么简单
	…
}
```

有的转型操作也是比较废的，比如`dynamic_cast`，这个转型会对类名称进行strcmp，以判断向下转型是否合理， **如果继承深度比较大，那么每一次的`dynamic_cast`将会进行多次strcmp，这将严重影响程序的执行效率。** 解决方法就是如果可以话，直接使用指向子类的指针，真的想用父类的指针（比如工厂设计模式等），那就考虑多态吧，在父类相应的函数前面加virtual，然后子类进行覆盖即可。

最后总结一下，有的程序员认为，转型其实什么也没有做，只是告诉编译器把某种类型视为另一种类型而已。如果说的是指针，这样理解是正确的，但如果说的是全部的话（包括变量），恐怕就不妥了，比如`static_cast(a) / b`，这一类经典的整数除法->小数除法转换，转换前后在底层产生的代码是绝对不同的。


---

### 28.避免返回handles指向对象内部成分

+ **避免返回`handles`（包括reference、指针、迭代器）指向对象内部，遵守这个条款可增加封装性，并将发生`dangling handles`的可能性降至最低。如果有必要必须要返回`handles`，在编写代码时就一定要注意对象和传出`handle`的生命周期。**

```c++
class Student
{
private:
    int ID;
    string name;
public:
    string& GetName()
    {
        return name;
    }
};
```

这是一个学生的类，类里面有两个成员变量，一个是学生ID，用整数表示，另一个是姓名，用string表示。有一个公有的方法`GetName()`，获得学生的名字，根据条款20所说的，使用引用可以防止资源不必要地拷贝，那么在返回值这边就用string&。但现在问题来了， **这个函数只是想返回学生的姓名，并不想用户对之进行修改，但返回引用却提供了这样的一个接口** ，如：

```c++
int main()
{
    Student s;
    s.GetName() = "Jerry";
    cout << s.GetName() << endl;
}
```

就可以把名字进行修改。你也许想到了，如果在前面加上const，像这样：

```c++
const string& GetName()
 {
     return name;
 }
```

就可以阻止`s.GetName() = “Jerry”`这样的代码了。

但这样写还是存在问题，就是 **<font color = red>如果返回的引用生命周期比对象本身要长时，引用就会悬空，它会指向一个不存在的string。</font>** 下面看一下“返回的引用生命周期比对象本身要长”的情况，这种情况还是很容易举出例子的，比如：

```c++
const string& fun()
{
    return Student().GetName();
}

int main()
{
    string name = fun(); //name指向一个不存的对象的成员变量
}
```

这时候即使name读取不报错，也是一个巨大的隐患，因为它已经是 **虚吊(dangling)** 的了。

**这就是为什么函数如果“返回一个handle代表对象内部成分”总是危险的原因，不在于返回值是不是const，而是<font color = red>在于如果handle（指针或引用）传出去了，就会暴露在“handle比其所指对象更长寿”的风险下。</font>**

但有些情况还是需要返回handle的，比如string或者vector里面的operator[]，就是返回的引用，因为需要对这里面的元素进行操作。



---

### 29.为"异常安全"而努力是值得的

+ **异常安全函数是指即使发生异常也不会泄漏资源或者允许任何数据结构败坏。这样的函数区分为三种可能的保证：基本型、强烈型、不抛异常型。**

+ **强烈保证往往可以通过`copy-and-swap`实现出来，但“强烈保证”并非对所有函数都可以实现或具备现实意义。**

+ **异常安全保证通常最高只等于其所调用之各个函数的异常安全保证中的最弱者。**

```c++
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    lock(&mutex);
    delete bgImage;
    ++ imageChanges;
    bgImage = new Image(imgSrc);
    unlock(&mutex);
}
```

这段代码大致的意思就是改变背景图片，删掉旧的背景图片，记录修改次数，然后创建新的背景图片。考虑到多线程操作，所以这里用了lock和unlock。

但这里会出现问题，因为并不是每次new都会成功的，有可能抛出异常，一旦抛出异常，unlock就没有执行了，这样资源就会一直处于lock状态，而无法继续操作了。另一方面，虽然本次改变背景的操作的失败了，但imageChanges仍然自增了一次，这就不符合程序员设计的初衷了。

有读者就会想，那还不简单，加上`try…catch`块就行了，像这样：

```c++
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    try
    {
        lock(&mutex);
        delete bgImage;
        bgImage = new Image(imgSrc);
        ++ imageChanges;
        unlock(&mutex);
    }
    catch (Exception* e)
    {
        unlock(&mutex);
    }
}
```

在catch里面写上unlock函数，另外，调换imageChanges的位置，在new之后再对齐自增。这样做固然可以，但回想一下， **我们在条款十三和条款十四做了资源管理类，让类的析构函数自动替我们完成这个操作** ，不是会更好吗？像这样：

```c++
class PrettyMenu
{
    …
    shared_ptr<Image> bgImage;
    …
}
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    Lock m1(&mutex);
    bgImage.reset(new Image(imgSrc));
    ++ imageChanges;
}
```

这样，即使抛出了异常，锁资源还有imageChanges都保证是异常发生之前的状态。

现在上升到理论的高度，异常安全性要做到：

+ **不泄漏任何资源**
+ **不允许数据败坏**

带异常安全性的函数会提供三个保证之一：

+ **<font color = red>基本承诺：</font>如果异常被抛出，程序内的任何事物仍然保持在有效状态下。没有任何对象或者数据结构会因此被破坏。比如上例中本次更换背景图失败，不会导致相关的数据发生破坏。**
+ **<font color = red>强烈保证：</font>在基本承诺的基础上，保证成功就是完全成功，失败也能回到之前的状态，不存在介于成功或失败之间的状态。**
+ **<font color = red>不抛出异常：</font>承诺这个代码在任何情况下都不会抛出异常，但这只适用于简单的语句。**

强烈保证有一种实现方法，那就是`copy and swap`。原则就是： **在修改这个对象之前，先创建它的一个副本，然后对这个副本进行操作，如果操作发生异常，那么异常只发生在这个副本之上，并不会影响对象本身，如果操作没有发生异常，再在最后进行一次swap。**

```c++
struct PMImpl {
  	std::tr1::shared_ptr<Image>bgImage;
  	int imageChanges;
};

class PreetyMenu {
  	...
private:
  	Mutex mutex;
  	std::tr1::shared_ptr<PMImpl>pIpml;
};

void PrettyMenu::changeBackground(std::istream& imgSrc)
{
  	using std::swap;
    Lock m1(&mutex);
   	//弄一个临时的tempBgImage
    //对tempBgImage进行操作
  	std::tr1::shared_ptr<PMImpl> pNew(new PMImpl(*pImpl));
  	pNew->bgImage.reset(new Image(imgSrc));//修改副本
  	++pNew->imageChanges;
    swap(pImpl, pNew);
}
```

`copy-and-swap`策略关键在于“修改对象数据的副本，然后在一个不抛异常的函数中将修改后的数据和原件置换”。它确实提供了强异常安全保障，但代价是时间和空间，因为必须为每一个即将被改动的对象造出副本。另外，这种强异常安全保障，也会在下面的情况下遇到麻烦：

```c++
void someFunc()
 {
 		 ... //对local状态做一份副本
     f1();
     f2();
     ... //将修改后的状态置换过来
 }
```

f1()和f2()都是强异常安全的，但万一f1()没有抛异常，但f2()抛了异常呢？是的， **数据会回到f2()执行之前的状态，但程序员可能想要的是数据回复到f1()执行之前。** 要解决这个问题就需要将f1与f2内容进行融合，确定都没有问题了，才进行一次大的swap，这样的代价都是需要改变函数的结构，破坏了函数的模块性。 **如果不想这么做，只能放弃这个copy-and-swap方法，将强异常安全保障回退成基本保障。**

类似于木桶效应，代码是强异常安全的，还是基本异常安全的，还是没有异常安全，取决于最低层次的那个模块。换言之，哪怕只有一个地方没有考虑到异常安全，整个代码都不是异常安全的。



---

### 30.透彻了解inlining的里里外外

+ **将大多数inlining限制在小型、被频繁调用的函数身上**

+ **不要只因为function templates出现在头文件，就将它们声明为inline**

`inline`是内联的关键字，它可以建议编译器将函数的每一个调用都用函数本体替换。这是一种以空间换时间的做法。把每一次调用都用本体替换，无疑会使代码膨胀，但可以节省函数调用的成本，因为函数调用需要将之前的参数以堆栈的形式保存起来，调用结束后又要从堆栈中恢复那些参数。

但注意`inline`只是对编译器的一个建议，编译器并不表示一定会采纳，比如当一个函数内部包含对自身的递归调用时，`inline`就会被编译器所忽略。对于虚函数的`inline`，编译器也会将之忽略掉，因为内联（代码展开）发生在编译期，而虚函数的行为是在运行期决定的，所以编译器忽略掉对虚函数的`inline`。对于函数指针，当一个函数指针指向一个`inline`函数的时候，通过函数指针的调用也有可能不会被编译器处理成内联。

```c++
inline void f() {...}	//假设编译器有意愿inline"对f的调用"
void (*pf) () = f;		//pf指向f
...
f();									//这个调用将被inline，因为它是一个正常调用
pf();									//这个调用不许不被inline，因为它是通过函数指针达成。
```

另一方面，即使有些函数没有inline关键字，编译器也会将之内联，用本体替换调用代码，比如直接写在class内部的成员函数，如下：

```c++
class Person
{
private:
    int age;
public:
    int getAge() const
    {
        return age;
    }
    void setAge(const int o_age);
};
void Person::setAge(const int o_age)
{
    age = o_age;
}
```

> 这里getAge()尽管没有inline关键字，但因为是直接写在class里面的，所以编译器将之处理成内联的；setAge()是在类内声明、类外定义的，编译器就不会将之处理成内联的了。

构造函数和析构函数虽然“看”似简单， **但编译器会在背后做很多事情，比如一个空的构造函数里面会由编译器写上对所有成员函数的初始化，如果将之inline，将会导致大批量的代码复制，所以不对构造函数和析构函数inline为好。**

要慎用inline，是因为一旦编译器真的将之inline了，那么这个inline函数一旦被修改，整个程序都需要重新编译，而如果这个函数不是inline的，那么只要重新连接就好。另外，一些调试器对inline函数的支持也是有限的。

作者认为，“一开始先不要将任何函数声明为inline”，经测试，确实发现某个函数的inline要比不对之inline的性能提升很多，才对之inline。在大多数情况下，inline并不是程序的瓶颈，真正的精力应该放在改善一些算法的修缮，以及反复调用的代码研究上，它们往往才是耗时的瓶颈所在。

---

### 31.将文件间的编译依存关系降至最低

+ 支持"编译依存性最小化"的一般构想是：相依于声明式，不要相依于定义式，基于此构想的两个手段是Handler classes和Interface classes。

+ 程序库头文件应该以“完全且仅有声明式”的形式存在，这种做法不论是否涉及templates都适用。

在说这一条款之前，先要了解一下C/C++的编译知识，假设有三个类ComplexClass, SimpleClass1和SimpleClass2，采用头文件将类的声明与类的实现分开，这样共对应于6个文件，分别是`ComplexClass.h`，`ComplexClass.cpp`，`SimpleClass1.h`，`SimpleClass1.cpp`，`SimpleClass2.h`，`SimpleClass2.cpp`。

ComplexClass复合两个BaseClass，SimpleClass1与SimpleClass2之间是独立的，ComplexClass的.h是这样写的：

```c++
#ifndef COMPLESS_CLASS_H
#define COMPLESS_CLASS_H

#include “SimpleClass1.h”
#include “SimpleClass2.h”

class ComplexClass
{
    SimpleClass1 xx;
    SimpleClass2 xxx;
};
…

#endif /* COMPLESS _CLASS_H */
```

我们来考虑以下几种情况：

**Case 1：**

现在`SimpleClass1.h`发生了变化，比如添加了一个新的成员变量，那么没有疑问，`SimpleClass1.cpp`要重编，`SimpleClass2`因为与`SimpleClass1`是独立的，所以`SimpleClass2`是不需要重编的。

那么现在的问题是，`ComplexClass`需要重编吗？

**答案是"是"，因为ComplexClass的头文件里面包含了`SimpleClass1.h`（使用了SimpleClass1作为成员对象的类），而且所有使用ComplexClass类的对象的文件，都需要重新编译！**

如果把ComplexClass里面的`#include “SimpleClass1.h”`给去掉，当然就不会重编ComplexClass了，但问题是也不能通过编译了，因为ComplexClass里面声明了SimpleClass1的对象xx。那如果把`#include “SimpleClass1.h”`换成类的声明class SimpleClass1，会怎么样呢？能通过编译吗？

**答案是"否"，因为编译器需要知道ComplexClass成员变量SimpleClass1对象的大小，而这些信息仅由class SimpleClass1是不够的，但如果SimpleClass1作为一个函数的形参，或者是函数返回值，用class SimpleClass1声明就够了。** 如：

```c++
// ComplexClass.h
class SimpleClass1;
…
SimpleClass1 GetSimpleClass1() const;
…
```

但如果换成指针呢？像这样：

```c++
// ComplexClass.h
#include “SimpleClass2.h”

class SimpleClass1;

class ComplexClass: 
{
    SimpleClass1* xx;
    SimpleClass2 xxx;
};
```

这样能通过编译吗？

答案是"是"，因为编译器视所有指针为一个字长（在32位机器上是4字节），因此`class SimpleClass1`的声明是够用了。但如果要想使用SimpleClass1的方法，还是要包含`SimpleClass1.h`，但那是ComplexClass.cpp做的，因为`ComplexClass.h`只负责类变量和方法的声明。

那么还有一个问题，如果使用`SimpleClass1*`代替SimpleClass1后，`SimpleClass1.h`变了，ComplexClass需要重编吗？

先看Case2。

**Case 2：**

回到最初的假定上（成员变量不是指针），现在`SimpleClass1.cpp`发生了变化，比如改变了一个成员函数的实现逻辑（换了一种排序算法等），但`SimpleClass1.h`没有变，那么SimpleClass1一定会重编，SimpleClass2因为独立性不需要重编，那么现在的问题是，ComplexClass需要重编吗？

**答案是"否"，因为编译器重编的条件是发现一个变量的类型或者大小跟之前的不一样了，但现在SimpleClass1的接口并没有任务变化，只是改变了实现的细节，所以编译器不会重编。**

**Case 3：**

结合Case1和Case2，现在我们来看看下面的做法：

```c++
// ComplexClass.h
#include “SimpleClass2.h”

class SimpleClass1;

class ComplexClass
{
    SimpleClass1* xx;
    SimpleClass2 xxx;
};
```

```c++
// ComplexClass.cpp

void ComplexClass::Fun()
{
    SimpleClass1->FunMethod();
}
```

请问上面的ComplexClass.cpp能通过编译吗？

**答案是"否"，因为这里用到了SimpleClass1的具体的方法，所以需要包含SimpleClass1的头文件，但这个包含的行为已经从ComplexClass里面拿掉了（换成了class SimpleClass1），所以不能通过编译。**

如果解决这个问题呢？其实很简单，只要在`ComplexClass.cpp`里面加上`#include “SimpleClass1.h”`就可以了。换言之，我们其实做的就是将`ComplexClass.h`的`#include “SimpleClass1.h”`移至了`ComplexClass1.cpp`里面，而在原位置放置`class SimpleClass1`。

这样做是为了什么？假设这时候SimpleClass1.h发生了变化，会有怎样的结果呢？

**SimpleClass1自身一定会重编，SimpleClass2当然还是不用重编的，`ComplexClass.cpp`因为包含了`SimpleClass1.h`，所以需要重编，但换来的好处就是所有用到ComplexClass的其他地方，它们所在的文件不用重编了！因为ComplexClass的头文件没有变化，接口没有改变！**

**总结一下，对于C++类而言，如果它的头文件变了，那么所有这个类的对象所在的文件都要重编，但如果它的实现文件（cpp文件）变了，而头文件没有变（对外的接口不变），那么所有这个类的对象所在的文件都不会因之而重编。**

因此，避免大量依赖性编译的解决方案就是：**<font color = red>在头文件中用class声明外来类，用指针或引用代替变量的声明；在cpp文件中包含外来类的头文件。</font>**

#### Handler classes

下面再来看书，去理解书上说的`Handler classes`就简单多了

假设我们要写一个Person类，如下：

```c++
class Person
{
private:
    string name;
    MyDate birthday;
    MyAddress address;

public:
    // fallows functions
    // ...
};
```

这个Person类里面包含有人的名字，人的生日以及地址，还有一些没有列出来的方法。注意到这里用到了string（它不是一个class，只是一个typedef，大多数情况下，我们认为它不会有任何改变），以及自定义的类MyDate与MyAddress，一种常见的做法是采用include头文件，像这样：

```c++
#include <string>
#include "MyDate.h"
#include "MyAddress.h"
```

在`MyDate.h`里面写好日期类相关的成员变量与方法，而在`MyAddress.h`里面写好地址类相关的成员变量与方法。 **但如果此后要往MyDate类或者MyAddresss类添加成员变量，那么不仅仅所有用到MyDate或者MyAddress对象的文件需要重新编译，而且所有用到Person对象的文件也需要重编译，一个小改动竟然会牵涉到这么多的地方！**

可以把问题归结为 **“C++并没有把将接口从实现中分离这件事做好”** ，因为包含头文件的做法很直观很方便，用的人也很普遍，而C++并没有对这种做法加以限制。

如果要把编译的依赖性降低，就要换一种思路来处理， **不能出现定义式，只能出现声明式，代价是增加代码的复杂度以及性能上的一些损失。**

书上提到了两种方法，第一种是采用 `Handler Classes`（用指针指向真正实现的方法），第二种是 `Interface Classes`（抽象基类）。

对于第一种`Handler Class`，一句话，**就是`.h`里面不包含类的自定义头文件，用“class 类名”的声明方式进行代替（也要把相应的成员变量替换成指针或引用的形式），在`.cpp`文件里面包含类的自定义头文件去实现具体的方法。** 改造之后的程序看起来是这样子的：

```c++
// Person.h
#include <string>
using namespace std;

class PersonImp;

class Person
{
private:
    //string Name;
    //MyDate Birthday;
    //MyAddress Address;
    PersonImp* MemberImp;

public:
    string GetName() const;
    string GetBirthday() const;
    string GetAddress() const;
    // follows functions
    // ...
};
```

```c++
// Person.cpp
#include "PersonImp.h"
#include "Person.h"

string Person::GetName() const
{
    return MemberImp->GetName();
}
string Person::GetBirthday() const
{
    return MemberImp->GetName();
}
string Person::GetAddress() const
{
    return MemberImp->GetAddress();
}
```

```c++
// PersonImp.h
#ifndef PersonImp_H
#define PersonImp_H

#include <string>
#include "MyAddress.h"
#include "MyDate.h"
using namespace std;

class PersonImp
{
private:
    string Name;
    MyAddress Address;
    MyDate Birthday;

public:
    string GetName() const
    {
        return Name;
    }

    string GetAddress() const
    {
        return Address.ToString();
    }

    string GetBirthday() const
    {
        return Birthday.ToString();
    }
};

#endif /* PersonImp_H */
```

```c++
// MyDate.h
#ifndef MY_DATE_H
#define MY_DATE_H

#include <string>
using namespace std;

class MyDate
{
private:
    int Year;
    int Month;
    int DayOfMonth;

public:
    string ToString() const;
}
#endif /* MY_DATE_H */
```

```c++
// MyAddress.h
#ifndef MY_ADDRESS_H
#define MY_ADDRESS_H

#include <string>
using namespace std;

class MyAddress
{
private:
    string Country;
    string Province;
    string City;
    string Street;

public:
    string ToString() const;
};

#endif /* MY_ADDRESS_H */
```

这里有一点要说一下，在`Person.h`里面并没有使用`MyDate *`和`MyAddress *`，而是用了`PersonImp*`，由PersonImp里面包含MyDate与MyAddress，这样做的好处 **就是方便统一化管理，它要求PersonImp里面的方法与Person的方法是一致的。以后Person添加成员变量，可以直接在PersonImp中进行添加了，从而起到了隔离和隐藏的作用，因为客户端代码大量使用的将是Person，而不必关心PersonImp，用于幕后实现的PersonImp只面向于软件开发者而不是使用者**。

书上是用shared_ptr来管理PersonImp的，使资源管理上更加科学与合理。

另外，书上也提倡把class x; class xx; class xxx;的声明放至一个名为`”xxxfwd.h”`的头文件里，比如`”datefwd.h”`，这个头文件里面只有声明式，而没有具体的类细节。也就是说，对于某个类，比如MyDate，应该分出三个文件，一个是`datefwd.h`，里面是一些用到的外来的class声明式；一个是`MyDate.h`里面是MyDate类的结构声明；一个是`MyDate.cpp`，它与MyDate.h配对，给出具体的实现细节。

#### Interface Classes

下面来谈谈书中的第二部分，用`Interface Classes`来降低编译的依赖。从上面也可以看出，避免重编的诀窍就是保持头文件（接口）不变化，而保持接口不变化的诀窍就是不在里面声明编译器需要知道大小的变量， **`Handler Classes`的处理就是把变量换成变量的地址（指针），头文件只有class xxx的声明，而在cpp里面才包含xxx的头文件。`Interface Classes`则是利用继承关系和多态的特性，在父类里面只包含成员方法（成员函数），而没有成员变量，像这样：**

```c++
// Person.h
#include <string>
using namespace std;

class MyAddress;
class MyDate;
class RealPerson;

class Person
{
public:
    virtual string GetName() const = 0;
    virtual string GetBirthday() const = 0;
    virtual string GetAddress() const = 0;
    virtual ~Person(){}
};
```

而这些方法的实现放在其子类中，像这样：

```c++
// RealPerson.h
#include "Person.h"
#include "MyAddress.h"
#include "MyDate.h"

class RealPerson: public Person
{
private:
    string Name;
    MyAddress Address;
    MyDate Birthday;
public:
    RealPerson(string name, const MyAddress& addr, const MyDate& date):Name(name), Address(addr), Birthday(date){}
    virtual string GetName() const;
    virtual string GetAddress() const;
    virtual string GetBirthday() const;
};
```

在`RealPerson.cpp`里面去实现`GetName()`等方法。从这里我们可以看到，只有子类里面才有成员变量，也就是说，如果Address的头文件变化了，那么子类一定会重编，所有用到子类头文件的文件也要重编， **所以为了防止重编，应该尽量少用子类的对象。利用多态特性，我们可以使用父类的指针，像这样`Person* p = new RealPerson(xxx)`，然后`p->GetName()`实际上是调用了子类的`GetName()`方法。**

但这样还有一个问题，就是`new RealPerson()`这句话一写，就需要RealPerson的构造函数，那么RealPerson的头文件就要暴露了，这样可不行。还是只能用Person的方法，所以我们在`Person.h`里面加上这个方法：

```c++
// Person.h
 static Person* CreatePerson(string name, const MyAddress& addr, const MyDate& date);
```

注意这个方法是静态的（没有虚特性），它被父类和所有子类共有，可以在子类中去实现它：

```c++
// RealPerson.cpp
#include “Person.h”
Person* Person::CreatePerson(string name, const MyAddress& addr, const MyDate& date)
{
    return new RealPerson(name, addr, date);
}
```

```c++
// Main.h
class MyAddress;
class MyDate;
void ProcessPerson(const string& name, const MyAddress& addr, const MyDate& date);
```

```c++
// Main.cpp
#include "Person.h"
#include “MyAddress.h”;
#include “MyDate.h”;

void ProcessPerson(const string& name, const MyAddress& addr, const MyDate& date)
{
    Person* p = Person::CreatePerson(name, addr, date);
…
}
```

总结一下，`Handler classes`与`Interface classe`s解除了接口和实现之间的耦合关系，从而降低文件间的编译依存性。减少编译依存性的关键在于保持`.h`文件不变化，具体地说，是保持被大量使用的类的`.h`文件不变化，这里谈到了两个方法：`Handler classes`与`Interface classes`。

`Handler classes`化类的成员变量为指针，在`.h`文件里面只包含`class xxx`的外来类声明，而不包含其头文件，在`.cpp`涉及到具体外来类的使用时，才包含`xxx.h`的头文件，这样最多只影响本身类的`cpp`重编，但因为`.h`文件没有变化，所以此类的对象存在的文件不必重编。

当然，书上说的`Handler classes`更想让我们在类A的基础上另造一个中间类AImp（成员函数完全与类A一致），这个中间类的成员中里面放置了所有类A需要的外来类的对象，然后类的逻辑细节完全在`Almp.cpp`中实现，而在`A.cpp`里面只是去调用`Almp.cpp`的同名方法。`A.h`的成员变量只有Almp的指针，这看上去好像一个Handler，因此而得名。

`Interface classes`则是将细节放在子类中，父类只是包含虚方法和一个静态的Create函数声明，子类将虚方法实现，并实现Create接口。利用多态特性，在客户端只需要使用到Person的引用或者指针，就可以访问到子类的方法。由于父类的头文件里面不包含任何成员变量，所以不会导致重编（其实由于父类是虚基类，不能构造其对象，所以也不用担心由于父类头文件变化导致的重编问题）。






---



