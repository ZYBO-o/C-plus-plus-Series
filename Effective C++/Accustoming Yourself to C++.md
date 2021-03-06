## 让自己习惯C++

<!-- GFM-TOC -->

* [1.视C++为一个语言联邦](#1视c为一个语言联邦)
* [2.尽量以 const, enum, inline 替换 #define](#2尽量以-const-enum-inline-替换-define)
  + [用const和enum替换不带参宏](#用const和enum替换不带参宏)
  + [用inline替换带参的宏](#用inline替换带参的宏)
  + [static 的小细节](#static-的小细节)

+ [3.尽可能使用const](#3尽可能使用const)
  + [修饰变量]()
  + [修饰指针]()
  + [修饰迭代器]()
  + [在类中修饰成员函数]()
+ [4.确定面向对象被使用前已被初始化](#4确定面向对象被使用前已被初始化)
  + [内置对象，STL和自定义类](#内置对象stl和自定义类)
  + [内置对象，STL和自定义类](#不同编译单元内定义的non-local-static-对象)

<!-- GFM-TOC -->

---

### 1.视C++为一个语言联邦

将C++视为一个由相关语言组成的联邦而非单一语言。在其某个次语言中，各种守则和通例都倾向于简单，直观易懂，并且容易记住。当从一个次语言移往另一个次语言，守则可能发生改变。

C++主要次语言：

+ C

  > 以C++内的C工作时，高效编程守则映射出C语言的局限性：没有模板，没有异常，没有重载，等等

+ Object-Oriented C++

  > 面向对象编程是C++不同于C的显著部分，C++引入类和对象的概念，对事物提供了很好的抽象途径，在一个class里面不仅仅可以像C中的结构体一样定义成员变量，而且可以提供方便快捷的成员函数，而不用像在C的结构体中用函数指针来实现。

+ Template C++

  > C++的模板，亦即泛型编程堪称一绝，有了模板，就不用手工提供所有类型的重载版本了，而交由编译器自行生成，大大减少了代码的冗长。良好编程守则中"惟 template 适用"的特殊条款并不罕见。template功能强大，带来了新的编程泛型。

+ STL

  > STL是优秀的C++模板库，里面集成了大量实用的库函数，比如string，vector，list，map等（唯一可惜的是没有提供hash相关的库，同时也要留意一些接口的不一致，比如string大量使用index作为接口，而vector等其他容器则是用iterator作为接口的）。

**C++ 高效编程守则视情况而变幻，取决于使用 C++ 的哪个部分。**

---

### 2.尽量以 const, enum, inline 替换 #define

#### 用const和enum替换不带参宏

宏定义`#define`发生在预编译期，而`const`，`enum`定义的常量发生在编译期，两者的重要差别在于编译期里的变量是进符号表的，而预编译期的宏是简单的替换，不进符号表。因此，`const`, `enum`定义的常量具有以下优势：

+ **支持类型检查**

+ **支持访问权限**

第(1)条优势，其实在Visual Studio编译器也已经对宏也引入了类型检查了，但不是所有的编译器都这样；

第(2)条优势是指可以把这些常量放在类中，从而避免了全局的作用域，而宏定义只能是全局的（全局变量在多线程中容易出问题，一份优秀的代码里是几乎不出现全局变量的），所以这条优势其实是`const`和`enum`替换宏定义最好的理由。在书中还提到了，用`const`和`enum`定义的常量名还会出现在编译器的提示信息里，而宏定义在编译器中显示的就不是宏定义名了，而直接是一个数字量了，这样就不方便调试了。


**const与enum使用契机：**

+ const适合于单个常量，比如double const PI = 3.1415926

+ enum适合于一组相关的常量，比如星期：

  ```c++
  enum DayOfWeek
  {
      Sunday,
      Monday,
      Tuesday,
      Wednesday,
      Thursday,
      Friday,
      Saturday,
  };
  ```

  > 这段枚举定义了Sunday = 0, Monday = 1, …, Saturday = 6（去掉DayOfWeek也行，这时就定义了匿名的枚举类型了）。



#### 用inline替换带参的宏

不带参的宏还说得过，带参的宏本身就是一个坑，可谓是bug四伏，一个不小心，就掉坑里了。举个最简单的例子：

```c++
#define square(a) a * a
```

在main函数中调用时，可能会这样`square(3)`，计算的是3的平方，这是OK的，但如果换成是`square(1+2)`，计算的还是3的平方吗？注意这时编译器解释成`1 + 2 * 1 + 2`，这样2 * 1的优先级高一些，所以先做了，这就出问题了。

好的，这是常见的错误，有些程序员可能认为多加括号就行，比如：

```c++
#define square(a) (a) * (a)
```

这的确可以避免上面所说的优先级的问题，但万一调用时是这样写的呢？

```c++
int main()
{
    int v = 3;
    squre(++v);
}
```

本意是想求4的平方，但编译器会翻译成`(++v)*(++v)`，这样v就被加了两次，这个结果肯定不是你想要的！

一句话，带参的宏很容易出问题，特别是针对复合操作（一句话做了不止一件事情）时，bug频出。

解决这个问题的方法是：**用函数替换！因为函数调用时，若实参是表达式，总是会先计算这个表达式的值，再去进行调用的，不会出现宏展开时的bug。**

```c++
template <class T>
T square(const T& v)
{
    return v * v;
}
```

就是一种比较好的解决方案（注意这里v不用加括号了，也不用担心参数被求值多次，而且提供了可靠的作用域限制），**但函数调用有一个保存现场和恢复现场的过程（本质是栈的压入和弹出），频繁地调用会导致性能低下，解决方法是<font color = red>在函数前面加上inline关键字</font>**，像这样：

```c++
template <class T>
inline T square(const T& v)
{
    return v * v;
}
```

这样就告诉编译器了，我想牺牲空间换时间——把这段函数体部分直接替换到原代码中，就不要保存现场和恢复现场了。但这里注意，**inline并不是强制的，就算你用了inline，编译器也不一定100%地进行代码替换，比如递归函数，编译器会直接忽略掉你加的inline。**所以，inline只是向编译器建议进行代码内联而已，inline适合于函数体本身短小（无递归）且频繁调用的场景。

#### static 的小细节

通常使用static来确保class专属常量至多只有一个实体。

```c++
class GamePlayer{
private:
  static const int NumTurns = 5;//常量声明式
  int scores[NumTurns];					//使用该常量
  ...
};
```

上述的是`NumTurns`的声明式而不是定义式。通常C++要求对使用的任何东西提供一个定义式，但如果是class专属常量又是static且为整数类型(ints,chars,bools)，则需要特殊处理。只要不取它们的地址，可以声明并使用而无需提供定义式。但如果需要取某个class专属常量的地址或者编译器坚持要看定义式，则需要提供定义式。

```c++
const int GamePlayer::NumTurns; //NumTurns的定义
```

定义式是放在源文件而不是头文件中。

---

### 3.尽可能使用const

const是常量的意思，它可以定义一个不可改变的量，主要用于以下几个地方：

#### 修饰变量
```c++
const int var = 3;
```

此时var的值就不能改变了。也正是因为const的变量不能轻易修改存储的值， **所以在声明的时候就要初始化** ，这样就是不行的：

```c++
const int var;
```

编译器就会报错。

#### 修饰指针

指针是特殊的变量，有时我们希望对它所指向的对象操作，而有时我们又希望对指针本身进行操作。同样，const应用于指针也有两个含义：

+ 一个是指向常量（指向的内容不可更改）
+ 一个是常量指针（指针的指向不可更改）

```c++
/* p为指向常量的指针，即p指向的对象是常量，不可以通过*p = 3 来修改a的值，但这时p = &b换个指向还是可以的 */
const int* p = &a;
/* p为常量指针，即p的指向不可更改，不可以通过p = &b来修改p的指向，但这时*p = 3改变a的值还是可以的 */
int* const p = &a; 
/* p为指向常量的常量指针，p的指向以及指向的对象都不可以更改，无论是*p = 3，还是p = &b都是错误的 */
const int* const p = &a;
```

还有一种形式是`int const * p`，这种形式是表示常量指针，还是指向常量的指针呢？

Effective C++给出的建议是看"*"的位置，当const位于星号左侧时，const修饰的是值，即表示指向常量，而当const位于星号右侧时，const修饰的是指针，即表示常量指针。所以`int const *p`等价于`const int *p`，你想对了吗？（关键看const修饰的是 * p 还是p）。

const有时还会修饰函数的形参或者函数的返回值，都是属于1或2这两种情况。修饰函数形参的用法：

```c++
void fun(const char a)
{
	a = ‘d’; // 错误，因为的值不可以改变
	cout << a; // OK
}
```

还有一个地方要注意一下，若有：

```c++
void fun1(const char* a)
{
	cout << a << endl;
}
void fun2(char *a)
{
	cout << a << endl;
}
```

当实参为const时，比如`const char* msg = "hello"`，此时`fun1(msg)`是可以的，但`fun2(msg)`会报编译错，说是无法将`const char *` 转成`char *` ；而当实参为普通变量时，比如`char* msg = "hello"`，`fun1(msg)`和`fun2(msg)`都是OK的。这是为什么呢？

因为**当const的变量传递给非const的变量会不安全（非const的变量可以修改原来定义为常量的东西了！）**，所以C++限制了这种用法（需用强制类型转换来告诉编译器，编译器才会放行）；而反过来，**当非const的变量传递给const变量时，不存在安全问题**，所以C++编译器总是会放行的。因此，如果在函数体内确实不改变形参a的值，那么采用带`const`的`fun1`的写法会更好，适用性更强。

#### 修饰迭代器

C++的STL中使用迭代器作为接口，它定义了普通的迭代器，如`vector<T>::iterator`，也定义了指向常量的迭代器，如`vector<T>::const_iterator`，初学者可能想当然地认为`const vector<T>::iterator`等价于`vector<T>::const_iterator`，其实不是这样的。

+ `const vector<T>::iterator`表示这个迭代器的指向不可以更改，即表示的是常量迭代器，
+ `vector<T>::const_iterator`表示指向常量的迭代器。

```c++
std::vector<int> vec;
...
const std::vector<int>::iterator iter = vec.begin();
*iter = 10; 	//可以改变iter所指的物
++ iter;			//错误，iter是const
std::vector<int>::const_iterator cIter = vec.begin();
*cIter = 10;	//错误，*cIter是const
++ cIter;			//可以改变cIter
```

#### 在类中修饰成员函数

const放在类中成员函数的后面，表示这个成为函数不会修改类的成员变量，比如：

```c++
class A
{
private:
	int a;
	double b;
public:
	void fun1() const;
	void fun2();
};
```

注意这里的`fun1()`函数后面有一个`const`，表示这个函数不会修改类的成员变量（在它的函数体里面出现任何改变a或b的值的操作，均不能通过编译）；另一方面`fun2()`函数后面没有`const`，表示这个函数 **可能** 修改类的成员变量，注意这里用的词是“可能”，`fun2()`可以修改也可以不修改，但 **为了增强安全性，所以良好的编程风格一般会把不改动成员变量的成员函数修饰为`const`的** 。

+ 有一点要切记： **<font color = red>有无const是可以构成成员函数的重载的！</font>**

+ **如果非得在const成员函数中进行修改变量，<font color = red>可以利用C++ 的一个与const先关的摇摆场：mutable </font>。mutable 可以释放掉no-static 成员变量的bitwise constness约束。**

+ **<font color = red>在const和非const成员函数中避免重复。</font>** 

我觉得这是一个非常重要的内容，有没有加const是构成函数重载的，但通常这种重载的相似度很高，就用书上的例子：

```c++
class TestBlock
{
private:
	string text;
public:
	…
	const char& operator[](size_t position) const
	{
		…
		return text[position];
	}

	char& operator[](size_t position)
	{
		…
		return text[position];
	}
};
```

可以看到两个重载函数里面的操作都是一样的，别因此认为可以用ctrl+c，ctrl+v而省事了，如果你要改动其中一个函数体里的内容，另一个就要同步更新，而万一你忘记了更新，后果是非常严重的！

一个好的方法来实现同步—— **<font color = red>在非const的函数中调用const函数</font>** ！这样来修改：

```c++
char& operator[] (size_t position)	//只调用const op[]
{
	return 
    const_cast<char&>(			 //将op[]返回值的const转换
    	static_cast<const TextBlock&>(*this)//为*this加上construction
    	 [position] //调用const op[] 
    );
}
```



---

### 4.确定面向对象被使用前已被初始化

#### 内置对象，STL和自定义类

+ **内置对象**
  + 永远在使用对象之前将之初始化。
+ **STL**
  + C++提供了丰富的容器，比如vector，list，deque，map和set等，这些容器已经写好了构造函数，所以总会自动初始化成默认值，程序员可以直接使用。
+ **自定义类**
  + 初始化最好使用列表初始化，使用函数体进行赋值会降低程序的效率，因为在进入构造函数的函数体时，这些成员变量已经被初始化了，会覆盖之前的值，使得效率变低。
  + 始化列表时，初始成员变量的顺序与列表排列的顺序没有关系，只取决于声明这些成员变量的顺序。



#### 不同编译单元内定义的non-local static 对象

+ `static`是静态的意思，表示这个变量不由栈分配，而存储在特有的全局变量/静态变量区域中，具有长寿命的特点（从被构造出来，直到程序结束时，才会由系统释放资源）

+ `non-local`则是说这个对象是全局的，而不是函数内的静态变量，是说它的作用范围广。

以例子来说明

比如在文件1中定义了

```c++
int a = 1;
```

而在文件2中又会去使用：

```c++
extern int a;
int b = a *3;
```

可以看到文件1应在文件2之后执行，这样a才能获得初值，否则b得到的将是垃圾值，但事实上C++对于不同文件执行的相对次序并无明确定义，这样b究竟得到的是垃圾值还是3就不能确定。

解决这个问题是方法是不要使变量有全局的作用域，可以在文件1中定义：

```c++
int& GetA()
{
	static int a = 1;
	return a;
}
```

而在文件2中调用

```c++
int b = GetA() * 3;
```

这样就一定保证a的初始化在先了。

**即 为免除"跨编译单元之初始化次序" 问题，请以 `local static` 对象代替 `non-local static` 对象。**

