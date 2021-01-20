# 一.定义抽象数据类型

### 1.基本概念

```c++
struct Sales_data{
    string isbn () const { return bookNo; }
    Sales_data& combine (const Sales_data&);
    double avg_price () const;

    string bookNo;
    unsigned unit_sold = 0;
    double revenue = 0.0;
};

double Sales_data::avg_price() const {
    if (unit_sold)
        return revenue/unit_sold;
    else
        return 0;
}

Sales_data& Sales_data::combine(const struct Sales_data &rhs) {
    unit_sold += rhs.unit_sold;
    revenue += rhs.revenue;
    return *this;
}

int main(){
    Sales_data total;
    total.revenue = 100;
    total.unit_sold = 10;
    Sales_data trans;
    trans.revenue = 20;
    trans.unit_sold = 2;
    total.combine(trans);
    cout << total.revenue << " " << total.unit_sold << endl;
}
```



+ **引入this：**
  + 成员函数通过this这个额外的隐式参数来访问调用它的对象。
  + this是一个常量指针，不允许改变this中保存的值。

+ **引入const成员函数：**
  
+ `double Sales_data::avg_price() const `通过把const放在成员函数的参数列表之后，来修改隐式this的类型为指向常量的常量指针。这种使用const的成员函数称为常量成员函数。
  
+ **类作用域与成员函数：**

  + 因为编译器分两步处理类：首先编译成员的声明，然后才会到成员函数体。所以成员函数体可以随意使用类中的其他成员函数，无须在意出现的次序。

+ **定义一个返回this对象的函数：**

  ```C++
  Sales_data& Sales_data::combine(const struct Sales_data &rhs) {
      unit_sold += rhs.unit_sold;
      revenue += rhs.revenue;
      return *this;
  }
  ```

  + 不太清楚为什么非得使用引用的返回类型，代码编译显示不用引用类型也能成功。
  + return语句解引用this指针会获得执行该函数的对象。



### 2.构造函数

+ 不同于其他的成员函数，构造函数不能被声明成const。创建类的const对象时，**直到构造函数完成初始化过程，对象才能真正获取其"常量"属性**。因此构造函数在const对象的构造过程中可以向其写值。
+ **合成的默认构造函数：**
  + 合成的默认构造函数初始化类成员函数的规则如下：
    + 若存在类的初始值，用它来初始化成员。
    + 否则默认初始化该成员
+ **合成的默认构造函数只适合非常简单的类，普通类需要定义它自己的默认构造函数。原因如下：**
  + 只有发现类不包含任何构造函数时才会生成默认的构造函数。**一旦定义了其他构造函数，除非自己再定义一个默认的构造函数，否则类将没有默认的构造函数。**
  + 合成的默认构造函数可能会执行错误操作。如果类包含有内置类型或者复合类型的成员，则只有当这些成员全被赋予了雷内的初始值时，这个类才适合于使用合成的默认构造函数。
  + 有时候编译器不能够为某些类合成的默认构造函数。比如类包含其他类的成员，但这个成员的类型没有构造函数，那编译器则无法初始化该成员。

+ **定义构造函数：**

  ```c++
  struct Sales_data{
  
      Sales_data() = default;
      Sales_data(const string &s) : bookNo(s) {}
      Sales_data(const string &s, unsigned n, double p) : bookNo(s), unit_sold(n), revenue(p*n) {}
      Sales_data(istream &);
  
      string isbn () const { return bookNo; }
      Sales_data combine (const Sales_data&);
      double avg_price () const;
  
      string bookNo;
      unsigned unit_sold = 0;
      double revenue = 0.0;
  
  };
  
  Sales_data::Sales_data(istream &is)
  {
      read(is, *this);
  }
  ```

  + C++11标准规定如果需要默认的行为， 可以在参数在参数列表后面写上`= default`来要求编译器生成构造函数。
  + 如果`= default`在类的内部，则默认构造函数是内联的，外在不是内联。
  + 当某个数据成员被构造函数初始值列表忽略时，它将以与合成构造默认函数相同的方式隐式初始化。

+ 构造函数不应该轻易覆盖掉类内的初始值，除非新赋的值与原值不同。如果不能用类的初始值，则所有构造函数都应该显示地初始化每个内置类型的成员。

<br/>

## 二.访问控制和封装

+ **使用public和private访问说明符来进行加强类的封装性。**
+ **使用class与struct关键字：**
  + class与struct定义类的唯一区别就是默认的访问权限。

### 1.友元

+ 类通过增加friend关键字在函数开头进行声明来将其作为友元，这样就允许函数访问自己的非公有成员。
+ 友元声明只能在类的内部，但是声明友元之外必须再专门对函数进行一次声明。

```c++
class Sales_data{

    friend Sales_data add (const Sales_data&, const Sales_data&);
    friend istream &read (istream& ,Sales_data&);
    friend ostream print (ostream&, const Sales_data&);

public:
    Sales_data() = default;
    Sales_data(const string &s) : bookNo(s) {}
    Sales_data(const string &s, unsigned n, double p) : bookNo(s), unit_sold(n), revenue(p*n) {}
    Sales_data(istream &);

    string isbn () const { return bookNo; }
    Sales_data combine (const Sales_data&);

private:
    double avg_price () const
        { return unit_sold ? revenue/unit_sold : 0; }
  
    string bookNo;
    unsigned unit_sold = 0;
    double revenue = 0.0;
};

Sales_data add (const Sales_data&, const Sales_data&);
istream &read (istream& ,Sales_data&);
ostream print (ostream&, const Sales_data&);
```

<br/>

## 三.类的其他特性

### 1.类成员再探

+ **可以定义类型成员：**
  + 用来定义类型的成员必须先定义后使用，这和普通成员有所区别。
+ **令成员作为内联函数：**
  + 定义在类内部的成员函数是自动inline的。**注意，不是声明**。
+ **可变数据成员：**
  + 可以通过在变量的声明中加入mutable关键字做到修改类的数据成员，哪怕是在const函数中也可以。

### 2.返回*this的成员函数

+ 当返回值是调用set对象的引用时，意味着函数返回的是对象本身而不是对象的副本，这样的操作性更强，而且减小了开销。

+ const实践

### 3.友元再探

+ 当一个类指定了友元类，则友元类的成员函数可以访问此类包括非公有成员在内的所有成员。
+ 友元不具有传递性。
+ 也可以令成员函数作为友元。
+ 当类想把一组重载函数声明成友元时，必须每一个都进行声明。

<br/>

## 四.类的作用域

+ 在类外定义成员函数时，当函数的返回值类型是类的数据成员时，需要明确指定是哪个类定义了它。

  ```c++
  class Window_mgr {
  public:
      using ScreenIndex = vector<Screen>::size_type ;
      void clear(ScreenIndex);
      ScreenIndex addScreen(const Screen&);
  
  private:
      vector<Screen> screens{Screen(24, 80, ' ')};
  };
  
  Window_mgr::ScreenIndex Window_mgr::addScreen(const  Screen& s)
  {
      screens.push_back(s);
      return screens.size() - 1;
  }
  ```

### 1.名字查找与类的作用域

+ 在类中，如果成员使用了外层作用域中的某个名字，而这个名字代表一种类型，则类不能在之后重新定义该名字。

  ```c++
  typedef double Money;
  class Account{
  public:
    Money balace() { return bal;}//使用外层作用域的Money
  private:
    typedef double Money;//错误：不能重新定义Money
    Money bal;
  }
  ```

<br/>

## 五.构造函数再探

### 1.默认构造函数初始值列表

+ **构造函数的初始值有时必不可少：**
  + 类成员时const或者是引用时，必须进行初始化；或者当成员属于某种类类型且该类没有定义默认构造函数时，也必须将这个成员初始化。
  + 初始化const或者引用类型的数据成员唯一机会就是通过构造函数初始值。
  + 初始化与赋值的区别事关底层效率问题，前者直接初始化数据成员，后者则先初始化再赋值。
+ 成员初始化的顺序
  + 成员的初始化顺序与它们在类定义中的出现顺序一致。

### 2.委托构造函数

+ 委托构造函数将自己的一些或者全部职责委托给了其他构造函数。
+ 委托构造函数的参数列表必须与类中另外一个构造函数匹配。
+ 如果受委托的构造函数体不是空的，则先执行函数体代码，然后将控制权交给委托者的函数体。

```c++
class Sales_date
{
public:
    Sales_date(string s, unsigned int cnt, double price):bookNo(s), count(cnt), revenue(price)
    {
        cout<< "This is pure function" << endl;
    }

    Sales_date(): Sales_date("",0,0)
    {
        cout<< "This is function1" << endl;
    }

    Sales_date(string s): Sales_date(s,0,0)
    {
        cout<< "This is function2" << endl;
    }
    Sales_date(istream &is): Sales_date()
    {
        cout<< "This is function3" << endl;
    }

private:
    string bookNo;
    unsigned int count;
    double revenue;
};

int main(){

    Sales_date s;
    Sales_date s1("hello");
}
//输出：
// This is pure function
// This is function1
// This is pure function
// This is function2
```

### 3.默认构造函数的作用

+ 可以通过explicit来抑制构造函数定义的隐形转换。
  + 关键字explicit只对一个实参的构造函数有效，需要多个实参的构造函数不能用于执行隐形转换。
  + explicit只允许出现在构造函数声明处。
  + explicit声明构造函数时，只能进行直接初始化的形式。

### 4.聚合类

+ 聚合类满足的条件：
  + 所有成员都是public
  + 没有定义任何构造函数
  + 没有类内初始值
  + 没有基类，没有virtual函数
+ 聚合类的初始值必须与声明顺序一致

### 5.字面值常量类

+ 字面值满足的要求：
  + 数据成员都是字面值类型的聚合类是字面值常量类。
  + 满足以下条件的类也是字面值常量类：
    + 数据成员必须是字面值类型
    + 类必须至少有一个constexpr构造函数
    + 如果一个数据成员含有类内初始值，则内置类型成员的初始值必须是一条常量表达式
    + 如果成员属于某种类类型，则初始值必须使用成员自己的constexpr构造函数
    + 类必须使用析构函数的默认定义，该成员负责销毁类的对象

<br/>

