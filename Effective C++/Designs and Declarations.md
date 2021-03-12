## 设计与声明

<!-- GFM-TOC -->

* [18.让接口容易被正确使用，不易被误用](#18让接口容易被正确使用不易被误用)
* [19.设计class犹如设计type](#19设计class犹如设计type)
* [20.宁以pass-by-reference-to-const替代pass-by-value](#20宁以pass-by-reference-to-const替换pass-by-value)
* [21.必须返回对象时，别妄想返回其reference](#21必须返回对象时别妄想返回其reference)
* [22.将成员变量声明为private](#22将成员变量声明为private)
* [23.宁以non-member,non-friend替换member函数](#23宁以non-membernon-friend替换member函数)
* [24.若所有参数皆需类型转换，请为此采用non-member函数](#24若所有参数皆需类型转换请为此采用non-member函数)
* [25.考虑写一个不抛异常的swap函数](#25考虑写一个不抛异常的swap函数)

<!-- GFM-TOC -->

---

### 18.让接口容易被正确使用，不易被误用

+ **好的接口容易被正确使用，不容易被误用；**

+ **促进正确使用的办法包括接口的一致性，以及与内置类型的行为兼容；**

+ **阻止误用的办法包括建立新类型、限制类型上的操作，束缚对象值，以及消除客户的资源管理责任；**

+ **多多使用shared_ptr来代替原始指针**

从本条款开始，就进入到全书的第四部分：设计与声明。

程序员设计接口时应本的对用户负责的态度， **"如果客户企图使用某个接口而却没有获得他所预期的行为，这个代码不应该通过编译；如果代码通过了编译，那它的作为就该是客户所想要的"。**

举一个书上的例子：

```c++
class Date
 {
 public:
     Date(int month, int day, int year);
     …
 };
```

这个类看上去好像没有问题，提供的构造函数接口简单直观，然而它却做了一个假设——用户都能够按月、日、年的顺序来传参。

但事实上，一定会有不少用户记错这个顺序，比如我们常用的顺序是年、月、日，所以有同学会下意识地这样调用：

```c++
Date d(2013, 5, 28);
```

也许还有同学随便输入，导致传参不符合实际，比如：

```c++
Date d(5, 32, 2013);
```

其实 **当你设计的程序需要假定用户都能按你想像来进行操作的话，这个程序就存在隐患。**

一种好的解决方法， **<font color = red>是假定用户输入的数据都是不可靠的，需要对输入进行严格的检测，这是防御式编程的思想，这对那个心怀不轨的用户来说是很好的处理方法。</font>** 但对于一般的用户，不合法的输入只是缺乏正确的引导。

如何去引导用户，如何让用户动最少的脑筋却能最佳地使用接口，是我们接下来要讨论的。

**一种好的引导方式，是让用户在传参的时候，知道自己传的是什么，之前用是一个int，这显然不能提醒用户。那就创建相应的类** ，像这样：

```c++
class Month
{
private:
    int m_month;
public:
    explicit Month(int month): m_month(month){}
};

class Day
{
private:
    int m_day;
public:
    explicit Day(int day): m_day(day){}
};

class Year
{
private:
    int m_year;
public:
    explicit Year(int year): m_year(year){}
};

class Date
{
private:
    Year m_year;
    Month m_month;
    Day m_day;
public:
    Date(Year year, Month month, Day day): m_year(year), m_month(month), m_day(day){}

};

int main()
{
    Date date(Year(2018), Month(3), Day(28));
}
```

注意Year、Month和Day类中构造函数前有`explicit`关键字，也就是不允许隐式构造，诸如`Date date(2018, 3, 28)`等会报错误：

允许的写法就是像main()函数中所示, **为年、月、日提供了类封装的好处还不仅仅有这些，当用户输入一个非法数值后，可以在类中进行判断。**

比如我们可以进一步对Month类的构造函数进行扩充，像这样：

```c++
explicit Month(int month): m_month(month){assert(m_month >= 1 && m_month <= 12);}
```

> 这样就限制了输入Month数据的合法性。当用户试图传入一个Month(15)的时候，断言失败就会报错。

还有一种好的设计，就是在Month类中给出月份的枚举类型，这样用户可以更直观地使用:

```c++
class Month
{
private:
    int m_month;
public:
    explicit Month(int month): m_month(month){assert(m_month >= 1 && m_month <= 12);}
    enum 
    {
        Jan = 1,
        Feb,
        Mar,
        Apr,
        May,
        Jun,
        July,
        Aus,
        Sep,
        Oct,
        Nov,
        Dec
    };
    int GetMonth() const
    {
        return m_month;
    };
};
```

然后在main函数中可以这样写：

```c++
int main()
 {
     Date date(Year(2013), Month(Month::May), Day(28));
 }
```

总之就是时刻提醒用户知道自己传的参数是什么。

预防用户不正确使用的另一个办法是， **让编译器对不正确的行为予以阻止，常见的方法是加上const，** 比如初学者常常这样写：

```c++
if(a = b * c){…}
```

很明显，初学者想表达的意思是比较两者是否相等，但代码却变成了赋值。这时如果在运算符重载时用const作为返回值，像这样：

```c++
const Object operator* (const Object& a, const Object& b);
```

注意这里的返回值是const，这时编译器就会识别出赋值运算符的不恰当了。

书上还提到很重要的一点，就是 **"<font color = red>尽量令你的自定义类型的行为与内置类型行为一致" </font>** ，举个夸张的例子：

+ 不要重载乘号运算符，但里面做的却是加法。
+ 自定义类型同时也要形成统一的风格，比如长度，不要有的类型用size表示，有的类型用length表示，这会使得哪怕是一个程序老手也会犯糊涂。

> 虽然有些IDE插件能够自动去寻找相应的方法名，但“不一致性对开发人员造成的心理和精神上的摩擦与争执，没有任何一个IDE可以完全抹除”。

最后再谈谈前几节说到的智能指针，<font color = red> **"任何接口如果要求客户必须记得做某些事情，就有着不正使使用的倾向，因为客户可能会忘记做那件事"。**</font> 所以像：

```c++
auto_ptr<Investment> createInvestment();
//或
shared_ptr<Investment> createInvestment();
```

使用shared_ptr会更好一些，因为它允许存在多个副本，不会在传递过程中改变原有资源管理者所持的资源，且支持自定义的删除器。书上说，自定义删除器可以有效解决”cross-DLL problem”， **这个问题发生于在不同的DLL中出生(new)和删除(delete)的情况（对象生命周期横跨两个DLL，但在第二个DLL中结束生命的时候却希望调用的是第一个DLL的析构函数），自定义删除器则会在删除时仍然调用诞生时所在的那个DLL的析构函数。**

当然，使用智能指针也是需要代价的， **它比原始指针大（Boost库中实现的shared_ptr体积是原始指针的2倍）且慢，而且使用辅助动态内存。任何事物都具有两面性，权衡一下就会发现，智能指针能避免的资源泄露问题（好的接口），相较于它的空间和时间代价而言，都是值得的。**



---

### 19.设计class犹如设计type

+ **class的设计就是type的设计，在设计之前先要把上述12个问题过一遍，这样可以有把握设计一个成功的类。**

要设计高效的class，需要回答以下的问题：

1. **对象如何去创建和销毁？**

   > 这是构造函数和析构函数的问题，当然如果想要有堆分配方式，还需要重载`new、new[]、delete和delete[]`运算符；

2. **对象初始化与对象的赋值应该有什么样的差别？**

   >  初始化是构造函数和拷贝构造函数的事（这时候对象还没有产生），但对象的赋值却不同，因为此时等号左边的对象已经存在了。

3. **新type如果被pass-by-value意味着什么？**

   > 会调用相应的拷贝构造函数，要注意是否需要一个深拷贝。 

4. **什么是新type的合法值？**

   > 要对对象的合法值进行限定，通常在构造函数中就对成员变量的 范围给出限定，警惕不安全的输入值！

5. **你的新type需要配合某个继承图系吗？**

   > 就是判断自己设计的class是否需要继承或被继承，是基类还是派生类。如果是基类的话，要注意是否允许这个基类生成对象（是否需要利用纯虚函数设计成抽象类），以及要将析构函数前面加上`virtual`。

6. **新type需要什么样的转换？**

   > 主要是针对隐式转换，`operator OtherType() const`，但通常情况下隐式转换也意味着隐患，所以设计时要谨慎。另外，构造函数中也要当心，如果不想让隐式构造发生，就要在前面加上`explicit`关键字。举个例子：

   ```c++
   class A
   {
   private:
            int a;
   public:
            A(int b):a(b){}
   };
   void fun(A obj);
   ```

   若调用`fun(3)`，则编译器也能接受，因为编译器自动作了`fun(A(3))`的处理，这就是隐式构造。而如果用户自己写`fun(A(3))`，这是显式构造。当A的构造函数前有`explicit`时，`fun(3)`的调用将通不过编译器。通常情况下，隐式转换是安全的。

7. **什么样的操作符和函数对此新type而言是合理的？**

   > 就是设计什么样的成员函数，以及重载哪些运算符。

8. **什么样的标准函数应该驳回？**

   > 是说哪些函数对外公开，哪些函数对内使用，这就是private，public和protected的功能啦，protected只有在有继承关系的类中使用才能发挥它真正的力量，普通的类用private和public就足够了。

9. **谁该取用新type成员？**

   > 与上面一条类似，就是类的封装性问题，一些而言，成员变量都应该是private的，而在public函数里面提供对这些成员变量的访问get和set函数。如果需要的话，可以使用友元，但友元也要慎用，因为有些编译器对之支持的不是很好，另一方面，友元成为破坏封装性的特例，会带来安全的隐患。

10. **什么是新type的未声明接口？**

    >  这个不大明白意思，先放在这里……

11. **你的新type有多么一般化？**

    > 这牵涉到泛型编程了，就是模板的概念，这是C++比C要方便的多的多的地方——它将需要的类型交给编译器自动生成。

12. **你真的需要一个新type吗？**

    > 其实我觉得应该把这个问题放在第一个，因为设计最初首先要问的就是这个问题，如果都没有必要设计了，还需要回答前十一个问题吗？

---

### 20.宁以pass-by-reference-to-const替换pass-by-value

+ **尽量以pass-by-reference-to-const来代替pass-by-value，前者通常比较高效，并可以避免切割**

+ **以上规则并不适用于内置类型、STL迭代器和函数对象，对它们而言，pass-by-value是更好的选择**

默认情况下，C++的函数调用是传值调用，也就是说形参拷贝实参的内容。举书上的例子：

```c++
class Person
{
private:
    string name;
    string address;
public:
    Person(){}
    virtual ~Person(){}
};

class Student: public Person
{
private:
    string schoolName;
    string schoolAddress;
public:
    Student(){}
    ~Student(){}
};
```

假设有个函数接口：

```c++
void PrintStudent(Student ss);
```

那么如果这样调用：

```c++
Student s;
PrintStudent(s);
```

形参ss就会把实参s完完整整地复制一遍，这其中先后调用了`Person`的构造函数和`Student`的构造函数，而每个类中又包含两个`string`对象，所以总共 **会调用6次构造函数（4个`string`构造、`Person`构造和`Student`构造，注意这里的构造包括构造函数和拷贝构造函数），相应也会调用6次析构函数（注意当有派生关系存在时，基类的析构函数应该声明成虚的，这个在条款七中提到过）。**

但事实上，在`PrintStudent`里面其实只是打印`Student`的信息，并没有对这个对象做出修改，所以完整的拷贝显然既浪费时间，又浪费空间。如果你习惯使用C，那么会想到把形参作成指针，这样拷贝只是拷贝一个指针的内容而已，像下面这样：

```c++
void PrintStudent(Student* ss);
```

调用的时候只要：

```c++
PrintStudent(&s);
```

对于C++而言，还可以使用引用，像这样：

```c++
void PrintStudent(Student& ss);
```

调用的时候只要：

```c++
PrintStudent(s);
```

就可以了，引用在原来代码的基础上只是多加了一个`&`，看上去比指针更自然，这算是C++的一个优点吧，在函数体中也可以避免对指针的操作，从而减少出错的可能性。

> 如果看汇编代码，就可以发现，引用的底层还是用指针实现的，所以两者看上去不同，但底层的原理却是相同的，就是对一个对象的地址进行操作，因此传引用的本质是传对象的地址。

传对象的地址是对原有对象直接操作，而不是它复本，所以这其中没有任何构造函数发生，是一种非常好的传参方法。但这种方法也有一定的“危险”性，与指针的功能相同，程序员可以在这个函数内部修改Student的内容，这样会出现一种现象，在主函数中Student是学生A，但通过这个函数之后，却变成了学生B。除非你刻意这样做，但像PrintStudent这种打印函数，是不应该修改原始内容的。

**<font color = red>所以pass-by-reference还是不够的，用pass-by-reference-to-const才更"安全"</font>** ，像这`void PrintStudent(const Student& ss);`使程序员在函数实现中不小心修改了`Student`对象的内容，编译器也会把这个错误指出来。

除了上面所言， **传引用调用可以减少构造函数的调用外，还可以防止对象切割，** 对象切割发生在存在派生关系的类中，比如：

```c++
void PrintStudent(Person ss);
```

调用时：

```c++
Student s;
PrintStudent(s);
```

**这个时候就发生了对象切割，<font color = red>因为`PrintStudnet`的函数形参是基类，它没有派生类多出来的结构，所以当传值调用时，ss只能保存基类的内容，对于多出的派生类的内容作丢弃处理。</font>**

但如果换成传引用调用，像这样：

```c++
void PrintStudnet(Person &ss);
```

或者：

```c++
void PrintStudnet(Person *ss);
```

**因为本质传的是地址，所以不存在对象的切割（地址类型Person&，可以理解为引用类型或者指针类型，存放的都是对象的地址，对于32位机而言，是4字节的整数，Person型的地址只是告诉编译器应该把这段地址的内容解释成什么）。** Person&只是告诉编译器它保存的地址对应的内容是一个Person类型的，它会优先把这个内容往Person上去套，但如果里面有虚函数，即使用了virtual关键字，那么编译的时候就会往类中安插一个虚指针，这个虚指针的指向将在运行时决定，这就是多态的机制了，它会调用实际传入的那个对象的虚函数，而不是基类对象的虚函数。

**<font color = red>如果不加引用或指针，那么形参就会复制实参，但形参是基类的，它没有实参多出来的那部分，所以它就不能复制了，只能丢弃了；但如果加了引用或指针，那么无论是什么类型的，都是保存实参的地址，地址是个好东西啊，它不会发生切割，不会丢弃。</font>**

总结一下pass-by-reference-to-const的优点：

+ 一是可以节省资源复制的时间和空间
+ 二是可以避免切割，触发多态，在运行时决定调用谁的虚函数。

那么是不是pass-by-reference-to-const一定好呢？答案 **显示是否定的** 。

+ 一是任何事物都不是绝对的
+ 二是C++默认是pass-by-value，自然有它的道理。

可以这样解释，因为pass-by-reference传的是地址，在32位机上，就是4字节， **但如果参数是一个字节的char或者bool，那么这时候用char&或bool&来传引用就不划来了。** 一般地，当参数是基本类型时，还是用pass-by-value的效率更高，这个忠告也适用于STL的迭代器和函数对象。

对于复杂的类型，还是建议pass-by-reference-to-const，即使这个类很小。但事实上，一个类看似很小，比如只有一个指针，但这个指针也许指向的是一个庞然大物，在深拷贝的时候，所花费的代价还是很可观的。另一方面，谁也不能保证类的内容总是一成不变的，很有可能需要扩充，这样原本看似很小的类，过一段时间就成了“胖子”了。

---

### 21.必须返回对象时，别妄想返回其reference

+ **绝对不要返回`pointer`或`reference`指向一个`local stack`对象，指向一个`heap-allocated`对象也不是好方法，更不能指向一个`local static`对象（数组），该让编译器复制对象的时候，就让它去复制！返回在函数中创建的变量，不要用引用，就用复制就好。**

当你理解条款20后，很可能出现一种过度使用的情况：只要看到是一个非内置类型，就去使用引用传值。举个书上的例子：

```c++
class Rational
{
private:
    int numerator;
    int denominator;
public:
    Rational():numerator(0), denominator(1){}
    friend const Rational& operator* (const Rational& r1, const Rational& r2){…}
};
```

`numerator`表示分子，`denominator`表示分母，这里我们需要把注意力放在`operator*`上，不是在它的形参上（用的`const`引用是完全正确的），而是在它的返回值上， **这个带引用的返回值是一个巨大的bug。**

我们可以设想一下实现：

```c++
friend const Rational& operator* (const Rational& r1, const Rational& r2)
{
        Rational temp;
        temp.numerator = r1.numerator * r2.numerator;
        temp.denominator = r1.denominator * r2.denominator;
        return temp;
}
```

>  这是一种最容易想到的实现方式，在栈上创建对象temp，分子、分母分别相乘后，将之返回。

但仔细再想想，调用函数真的能收到这个temp吗？**它是在`operator *` 函数调用时的栈上产生的，当这个函数调用结束后，栈上的temp也会被pop掉，换言之，temp的生命周期仅存在于`operator *`  函数内，离开这个函数，返回的引用将指向一个已经不在的对象！**

对此，VS编译器已经给出了warning，如下：

```SH
“warning C4172: returning address of local variable or temporary”
```

千万不能忽略它。

那既然栈上创建对象不行，还可以在堆上创建嘛（new出来的都是在堆上创建的），于是我们有：

```c++
friend const Rational& operator* (const Rational& r1, const Rational& r2)
{
    Rational *temp = new Rational();
    temp->numerator = r1.numerator * r2.numerator;
    temp->denominator = r1.denominator * r2.denominator;
    return *temp;
}
```

这下VS编译器没有warning了，之前在资源管理那部分说过， **<font color = red>new和delete要配对</font>** ，这里只有new，那delete了？delete肯定不能在这个函数里面做， **只能放在外面，这样new和delete实际位于两个不同的模块中了，程序员很容易忘记回收，而且给维护也带来困难，所以这绝对不是一种好的解决方案。** 书上举了一个例子，比如：

```c++
Rational w, x, y, z;
w = x * y * z;
```

> 连乘操作会有两次`new`的过程，我们 **很难取得`operator*`返回的`reference`背后隐藏的那个指针。**
>
> 当然，如果把`new`换成`auto_ptr`或者是`shared_ptr`，这种资源泄露的问题就可以避免。

栈上创建的临时对象不能传入主调模块，堆上创建的对象就要考虑资源管理的难题，还有其他解决方法吗？

我们还有static对象可以用，static对象位于全局静态区，它的生命周期与这个程序的生命周期是相同的，所以不用担心它会像栈对象那样很快消失掉，也不用担心它会像堆对象那样有资源泄露的危险。可以像这样写：

```c++
friend const Rational& operator* (const Rational& r1, const Rational& r2)
{
    static Rational temp;
    temp.numerator = r1.numerator * r2.numerator;
    temp.denominator = r1.denominator * r2.denominator;
    return temp;
}
```

这样写编译器同样不会报错，但考虑一下这样的式子：

```c++
Rational r1, r2, r3;
if(r1 * r2 == r1 * r3)
{
  …
}
```

if条件恒为真，这就是静态对象做的！ 因为所有对象共享这个静态对象，在执行`r1 * r2`时，temp的值为t1，但执行`r1 * r3`之后，temp的值统一都变成t2了。 **它在类中只有一份，明白这个原因后就不难理解了。**

既然一个static对象不行，那弄一个static数组？把r1 * r2的值放在static数组的一个元素里，而把r1 * r3放在static数组的另一个元素里？仔细想想就知道这个想法是多么的天马行空。

**一个必须返回新对象的正确写法是去掉引用，就这么简单！**

```c++
friend const Rational operator* (const Rational& r1, const Rational& r2)
{
    Rational temp;
    temp.numerator = r1.numerator * r2.numerator;
    temp.denominator = r1.denominator * r2.denominator;
    return temp;
}
```

该让编译器复制的时候就要放手去复制。

---

### 22.将成员变量声明为private

+ **切记将成员变量声明为private，这可赋予客户访问数据的一致性、可细微划分访问控制、允诺约束条件获得保证，并提供class作者以充分的实现弹性**

+ **protected并不比public更具封装性**

```c++
class Clothes
{
public:
    int price;
    string name;
};
```

假设有一个衣服的类，里面的成员变量用来描述它的价格和衣服名。将之设为public的话，类外可以直接接触到price成员变量。这样很危险，因为客户端可以直接修改price了，卖衣服的商家就会不开心了。而将price设置成private，像这样：

```c++
class Clothes
{
private:
    int price;
    string name;
public:
    int GetPrice()
    {
        return price;
    }
};
```

price的访问需要经由GetPrice()的成员函数才能进行，类外客户端不能直接对price进行操作。从这里就可以看到将price设为private的三个好处：

+ **可以对访问进行控制** ，对于一个变量，可以设置它为可读可写（同时提供get和set函数），亦可设置为只读（只提供get函数），甚至可以设置成为只写（只提供set函数）。
+ **语法一致性。** 很多时候程序员在编程的时候都在想到底是Object.price呢，还是Object.price()呢？如果只能通过成员函数访问，就知道后面应该加上小括号了。
+ **面向对象的三大基石之一——封装** 。商家修改了这个类，比如在商店门口公布了打折信息，那么只要在类中这样做：

```c++
int GetPrice()
 {
     return price * 0.9;
 }
```

客户端完全不需要作任何修改。封装可以更直观地打一个比方，有两个同学A和B，他们都约自己的朋友去爬山，A约了10个人，而B只约了1个人，不凑巧天下雨了，爬山的计划只能暂时取消，需要通知自己的朋友，这时你想想，是A容易通知呢，还是B容易通知呢？

**是的，很明显应该是B好解决问题一些。为什么呢？因为与它发生关系（爬山）的人最少，所以处理起来很方便（一句话，接触越少越容易维护）。这唯一的1个人就好比是成员函数，如果功能变更，只要修改这个成员函数就可以了，而不必修改类外任何代码。**

好了，我们总结一下为什么要将成员变量声明为private而不是public的原因：

+ **<font color = red>能够提供语法一致性，写代码的时候能够马上区分要不要加函数括号；</font>**

+ **<font color = red>提供更好的访问控制，只读、只写还是可读可写，都容易控制；</font>**

+ **<font color = red>封装，减少与外界接触的机会，方便修改和维护。</font>**

现在回答下一个问题：为什么protected不行？

如果不存在继承关系，protected的作用与private等同，除了本类之外，其他类或者类外都不能访问， **但如果存在继承关系时，protected标识的成员变量，在它的子类中仍可以直接访问，所以封装性就会受到冲击。这时如果更改父类的功能，相应子类涉及到的代码也要修改，这就麻烦了。** 而如果使用private标识，则根据规则，所有private标识的变量或者函数根本不被继承的，这样就可以保护父类的封装性了，仍可以在子类中通过父类的成员函数进行访问。

书上说了这样一句话： **"从封装的角度观之，其实只有两种访问权限：private（提供封装）和其他（不提供封装）。"**


---

### 23.宁以non-member,non-friend替换member函数

+ **宁可拿non-member non-friend函数替换member函数，这样可以增加封装性、包裹弹性和机能扩充性**

本条款还是讨论封装的问题，举书上的例子：

```c++
class WebBrower
{
public:
    void ClearCach();
    void ClearHistory();
    void RemoveCookies();
};
```

定义了一个WebBrower的类，里面执行对浏览器的清理工作，包括清空缓存，清除历史记录和清除Cookies，现在需要将这三个函数打包成一个函数，这个函数执行所有的清理工作，那是将这个清理函数放在类内呢，还是把他放在类外呢？

如果放在类内，那就像这样：

```c++
class WebBrower
{
    …
    void ClearEverything()
    {
        ClearCach();
        ClearHistory();
        RemoveCookies();
    }
    …
};
```

如果放在类外，那就像这样：

```c++
void ClearWebBrowser(WebBrower& w)
{
    w.ClearCach();
    w.ClearHistory();
    w.RemoveCookies();
}
```

**根据面向对象守则的要求，数据以及操作数据的函数应该捆绑在一起，都放在类中，这意味着把它放在类内会比较好。<font color = red>但从封装性的角度而言，它却放在类外好</font>**，为什么？

为了区分开，我们把在类内的总清除函数称之为ClearEverything，而把类外的总清除函数称之为ClearWebBrower。

ClearEverything对封装性的冲击更大，**因为它位于类内，这意味着它除了访问这三个公有函数外，还可以访问到类内的私有成员，是的，你也许会说现在这个函数里面只有三句话，但随着功能的扩充，随着程序员的更替，这个函数的内容很可能会逐渐“丰富”起来。而越“丰富”说明封装性就会越差，一旦功能发生变更，改动的地方就会很大。**

再回过头来看看类外的实现，**在ClearWebBrowser()里面，是通过传入WebBrower的形参对象来实现对类内公有函数的访问的，在这个函数里面是绝对不会访问到私有成员变量（编译器会为你严格把关）。因此，ClearWebBrowser的封装性优于类内的ClearEverything。**

但这里有个地方需要注意，ClearWebBrower要是类的非友元函数，上面的叙述才有意义，因为 **<font color = red>类的友元函数与类内成员函数对封装性的冲击程度是相当的。</font>**

看到这里，你也许会争辩， **把这个总清除的功能函数放在类外，就会割离与类内的关联，逻辑上看，这个函数就是类内函数的组合，放在类外会降低类的内聚性。**

为此，书上提供了命名空间的解决方案，事实上，这个方案也是C++标准程序库的组织方式，好好学习这种用法很有必要！像这样：

```c++
namespace WebBrowserStuff
{
    …
    class WebBrowser();
    void ClearWebBrowser(WebBrowser& w);
    …
}
```

**namespace与class不同，前者可以跨越多个源码文件，而后者不能。通过命名空间的捆绑，是在封装和内聚之间非常好的平衡。**

更有意思的是，与WebBrower相关的其他功能，比如书签、cookie管理等等，他们与WebBrower紧密相关，但放在单看书签和cookie，两者又是逻辑不同的，喜欢书签管理的用户不一定喜欢cookie管理，这时如果把他们统统放在类内，会显得有些不妥。书上提供了很好的命名空间组织方式，像这样：

```c++
// 头文件webbrowser.h
namespace WebBrowserStuff
{
    class WebBrowser(); // 核心功能
    void ClearWebBrowser(WebBrowser& w); // non-member non-friend函数
}

// 头文件webbrowserbookmarks.h
namespace WebBrowserStufff
{
    …// 与书签相关的函数
}

// 头文件 webbrowsercookies.h
namespace WebBrowserStuff
{
    …// 与cookie管理相关的函数
}
```

将他们放在不同的头文件中，但位于同一个namespace中，是内聚与封装平衡的极佳处理方式。

C++标准库中，容器是std命名空间的重要组成部分，但容器也有好多种，比如vector，set和map等等，如果把它们写在一个头文件中，会很臃肿。 C++把它们放在不同的头文件中，但又位于同一个命名空间里，方便用户使用，比如用户只想用vector，那么只要包含`#include< vector>`就行了，而不必`#include<C++ standardLibrary>`，并且一句`using namespace std`即可不加前缀地使用其内定义的各种名称。

**最后总结一下，本条款强调封装性优先于类的内聚逻辑，这是因为“愈多东西被封装，愈少人可以看到它，而愈少人看到它，我们就有愈大的弹性去改变它，因为我们的改变仅仅影响看到改变的那些人或事物”。采用namespace可以对内聚性进行良好的折中。**



---

### 24.若所有参数皆需类型转换，请为此采用non-member函数

+ **如果你需要为某个函数的所有参数（包括被this指针所指的那个隐喻参数）进行类型转换，那么这个函数必须是个non-member，且为了封装，最好是个non-friend。**

```c++
class Rational
{
private:
    int numerator;
    int denominator;
public:
    Rational(int n = 0, int d = 1): numerator(n), denominator(d){assert(denominator != 0);}
    int GetNumerator() const{return numerator;}
    int GetDenominator() const {return denominator;}
    const Rational operator* (const Rational& r)
    {
        return Rational(numerator * r.numerator, denominator * r.denominator);
    }
};
```

有理数的私有成员是分子numerator和分母denominator，定义了构造函数，注意这个构造函数前面没有加explicit关键字，这意味着允许隐式转换（即标题所说的类型转换）。GetNumerator()和GetDenominator()是用来获取私有成员的值的，下面是我们要重点讲述的乘法运算符的重载。

我们的问题是，用什么方式实现乘法运算符的重载呢？我记得在我初学C++时，书上介绍了两种方法：

+ 采用类内成员函数（也就是上面这段程序的写法）

+ 采用友元函数，像这样：

  ```c++
  class Rational
  {
  private:
      int numerator;
      int denominator;
  public:
      Rational(int n = 0, int d = 1): numerator(n), denominator(d){assert(denominator != 0);}
      int GetNumerator() const{return numerator;}
      int GetDenominator() const {return denominator;}
      friend const Rational operator* (const Rational& r1, const Rational& r2);
  };
  const Rational operator* (const Rational& r1, const Rational& r2)
  {
      return Rational(r1.numerator * r2.numerator, r1.denominator * r2.denominator);
  }
  ```

就这两种方法而言，其实 **友元函数的实现方式更优** ，为什么这样说？因为它们对封装性的冲击是相当的，但友元函数支持下面的代码：

```c++
int main()
{
    Rational r1(3, 5);
    Rational r2(2, 1);
    r2 = r1 * 2;
    r2 = 2 * r1;
    return 0;
}
```

成员函数实现却在`r2 = 2 * r1`上通不过编译。将这句话还原成运算符函数的形式，就能看到原因了：

```c++
r2 = 2.opeator* (r1);
```

哈哈，这样就知道为什么编译器不允许了，2必须首先转成Rational对象，才能使用它自身的`operator*`，但编译器不允许类型转换成this（隐含的自身指针）的，所以报错。

但对于友元函数实现，我们也将之还原成运算符函数的形式：

```c++
r2 = operator *(2, r1);
```

第一个参数会发生隐式转换（因为构造函数前面没有explicit），不会同时进行向this的转换，所以编译器是放行的。

这里插一句，隐式转换的发生过程就像这样：

```c++
const Rational temp(2);
r2 = operator*(temp, r1);
```

综上， **成员函数实现不能支持含隐式转换的交换率，而友元函数实现却可以，故我们认为友元函数实现要优于成员函数实现（程序不支持乘法交换率是难以接受的）。**

解决了这个问题之后，再来思考一下，能否对封装性进行改进，因为条款二十三说了， **<font color = red>宁以non-member，non-friend函数去替换member函数，其实对友元函数形式稍加修改，去掉friend，改用public成员变量来访问函数内的私有成员，就可以实现好的封装</font> **，像这样：

```c++
const Rational operator* (const Rational& r1, const Rational& r2)
 {
     return Rational(r1.GetNumerator() * r2.GetNumerator(), r1.GetDenominator() * r2.GetDenominator());
 }
```

在这个函数里面，是不能直接访问到类的私有成员的，因而保证了好的封装性。



---

### 25.考虑写一个不抛异常的swap函数

+ **当`std::swap`对你的类型效率不高时，提供一个swap成员函数，这个成员函数不抛出异常，只对内置类型进行操作**
+ **如果提供一个member swap，也该提供一个non-member swap来调用前者，对于普通类，也请特化`std::swap`**
+ **调用swap时，区分是调用自身命名空间的swap还是std的swap，不能乱加std::符号**
+ **为“用户自定义类型”进行`std template`全特化是好的，但千万不要尝试在std内加入某些对std而言全新的东西。**

> 不知道为什么作者给这个条款起这样的名字，因为这样看上去重点是在“不抛出异常”，但事实上作者只是在全文最后一段说了一下不抛异常的原因，大部分段落是在介绍怎样写一个节省资源的swap函数。

你可以试一下，只要包含了头文件iostream，就可以使用swap函数，比如：

```c++
#include <iostream>

int main()
{
    int a = 3;
    int b = 4;
    std::swap(a, b);
}
```

结果就是a为4，b为3了，也就是说，在std的命名空间内，已经有现成的swap的函数了，这个swap函数很简单，它看起来像这样：

```c++
template<class T>
void swap(T& a, T& b)
{
    T c = a;
    a = b;
    b = c;
}
```

这是最常见形式的两数交换了（特别地，当T是整数的时候，还可以使用异或的操作，这不是本条款讨论的重点，所以略过了，但面试题里面很喜欢问这个）。

假设存在一个类Element，类中的元素比较占空间

```c++
class Element
{
private:
    int a;
    int b;
    vector<double> c;
};
```

Sample类中的私有成员是Element的指针，有原生指针，大多数情况下都需要自定义析构函数、拷贝构造函数和赋值运算符，像下面一样。

```c++
class Sample
{
private:
    Element* p;
public:
    ~Sample();
    Sample(const Sample&);
    Sample& operator= (const Sample&);
};
```

在实现`operator=`的时候，有一个很好的实现方法，参见条款十一。大概像这样：

```c++
Sample& operator= (const Sample& s)
{
    if(this != &s)
    {
        Sample temp(s);
        swap(*this, temp);
    }
    return *this;
}
```

当判断不是自我赋值后，是通过调用拷贝构造函数来创建一个临时的对象（这里可能会有异常，比如不能分配空间等等），如果这个对象因异常没有创建成功，那么下面的swap就不执行，这样不会破坏this的原始值，如果这个对象创建成功了，那么swap一下之后，把临时对象的值换成*this的值，达到了赋值的效果。

上面的解释是条款九的内容，如果不记得了，可以回头翻翻看，本条款的重点在这个swap函数上。  **这里调用的是默认的std里面的swap函数，<font color = red>它会创建一个临时的Sample对象（拷贝构造函数），然后调用两次赋值运算，这就会调回来了</font>，即在swap函数里面调用`operator=`，而之前又是在`operator=`中调用swap函数，这可不行，会造成无穷递归，堆栈会溢出。**

**因此，我们要写一份自己的swap函数，这个函数是将Sample里面的成员进行交换。**

问题又来了，Sample里面存放的是指向Element的指针，那是交换指针好呢，还是逐一交换指针所指向的对象里面的内容好呢？**Element里面的东西挺多的，所以显然还是直接交换指针比较好（本质是交换了Element对象存放的地址）。**

因此，可以定义一个swap的成员函数。像这样：

```c++
void swap(Sample& s)
{
    std::swap(p, s.p);
}
Sample& operator= (const Sample& s)
{
    if(this != &s)
    {
        Sample temp(s);
        this->swap(s);
    }
    return *this;
}
```

但这样看上去有点别扭，我们习惯的是像`swap(a, b)`这种形式的swap，如果交给其他程序员使用，他们也希望在类外能够像`swap(SampleObj1, SampleObj2)`那样使用，而不是`SampleObj1.swap(SampleObj2)`。 **为此我们可以在std空间里面定义一个全特化的版本（`std namespace`是不能随便添加东西的，只允许添加类似于swap这样的全特化版本）** ，像这样：

```c++
namespace std
{
	template<>
	void swap<Sample>(Sample &s1, Sample &s2)
	{
    	s1.swap(s2); // 在这里调用类的成员函数
	}
}
```

```c++
Sample& operator= (const Sample& s)
{
    if(this != &s)
    {
        Sample temp(s);
        swap(*this, s); // 顺眼多了，会先去调用特化版本的swap
    }
    return *this;
}
```

这样，就可以在使用`namespace std`的地方用`swap()`函数交换两个Sample对象了。

下面书上的内容就变难了，因为假设Sample现在是一个模板类，Element也是模板类，即：

```c++
template <class T>
class Element
{…};

template <class T>
class Sample
{…};
```

那应该怎么做呢？

**<font color = red>在模板下特化std的swap是不合法的（这叫做偏特化，编译器不允许在std里面偏特化），只能将之定义在自定义的空间中，</font>** 比如：

```c++
namespace mysample
{
	template <class T>
	class Element
	{…};

	template <class T>
	class Sample
	{…};

	template <class T>
	void swap(Sample<T> &s1, Sample<T> &s2)
	{
    	s1.swap(s2);
	}
}
```

总结一下，

+ 当是 **普通类** 时， **可以将swap的特化版本放在std的namespace中，swap指定函数时会优先调用这个特化版本；**
+ 当是 **模板类** 时， **只能将swap的偏特化版本放在自定义的namespace中。**

好了，问题来了，这时候用`swap(SampleObj1, SampleObj2)`时，调用的是std版本的swap，还是自定义namespace的swap？

事实上， **编译器还是会优先考虑用户定义的特化版本，只有当这个版本不符合调用类型时，才会去调用std的swap。** 但注意此时：

```c++
template<typename T>
void doSomething(T& obj1, T& obj2) 
{
  using std::swap; //令std::swap在此函数中可用
  ...
  swap(obj1,obj2); //为T类型对象调用最佳swap版本
  ...
}
```

当编译器看到对swap的调用时，它们会查找适当的swap并调用之。

+ **如果T是Sample并位于命名空间mysample内，编译器会找出mysample内的swap。**
+ **如果没有T专属的swap存在，编译器会使用std内的swap。**

好了，最后一段终于说到了不抛异常的问题， **书上提到的是不要在成员函数的那个swap里抛出异常，因为成员函数的swap往往都是简单私有成员（包括指针）的置换，比如交换两个int值之类，都是交换基本类型的，不需要抛出异常，把抛出异常的任务交给non-member的swap吧。**



---

