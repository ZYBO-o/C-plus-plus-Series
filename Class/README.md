<!-- GFM-TOC -->

* 类

  * [一、定义抽象数据类型](#一定义抽象数据类型)

  * [二、访问控制和封装](#二访问控制和封装)

  * [三、类的其他特性](#三类的其他特性)

  * [四、类的作用域](#四类的作用域)

  * [五、构造函数再探](#五构造函数再探)

    <!-- GFM-TOC -->

## 一.定义抽象数据类型

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
  
  + 在成员函数声明时，形参列表后加const关键字。这个const使得隐式传入该函数的this指针成为`指向常量的指针`，因此const成员函数不可修改对象内容，可被const对象调用。
  
  + `double Sales_data::avg_price() const `通过把const放在成员函数的参数列表之后，来修改隐式this的类型为指向常量的常量指针。这种使用const的成员函数称为常量成员函数。
  
  + 将成员函数设置为const有助于提高灵活性，使const对象和非const对象都可调用
  
  + 例子：const成员函数传入指向const对象的this指针
  
    ```c++
    string Sales_data::isbn() const {                       //实际的定义
        return this->bookNo;
    }
    string Sales_data::isbn(const Sales_data *const this) { //上一个对应的伪代码，说明const成员函数
        return this->bookNo;
    }
    ```
  
    
  
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

  + 不太清楚为什么非得使用引用的返回类型，代码编译显示不用引用类型也能成功：返回引用类型这样就能继续操作本身类的操作。
  + return语句解引用this指针会获得执行该函数的对象。

### 2.构造函数

+ `构造函数`：控制类对象的初始化过程；构造函数的名字和类名相同，没有返回类型；只要类的对象被创建，就会执行构造函数。每个类有一个或多个构造函数，类似于重载，以形参列表区分。

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

+ 成员`初始化`流程（`优先级：构造函数初值列表->类内初始值->默认初始化`）：

  1. 如果成员在构造函数初值列表中，则初始化为`构造函数初值列表`提供的值
  2. 如果成员不在构造函数初值列表中但有类内初始值，则初始化为`类内初始值`
  3. 如果成员既不在构造函数初值列表中又没有类内初始值，则`默认初始化`

+ 构造函数不应该轻易覆盖掉类内的初始值，除非新赋的值与原值不同。如果不能用类的初始值，则所有构造函数都应该显示地初始化每个内置类型的成员。

+ **拷贝，赋值和析构：**
  + 对象在一些情况下会被`拷贝`，如初始化变量，以及以传值方式传递一个对象等
  + 使用赋值运算符`=`时，对象会被`赋值`
  + 对象不存在时需要`析构`
  + 如果不手动定义拷贝、赋值、析构，编译器也会自动合成它们。这样的版本将对对象的每个成员分别拷贝、赋值、析构
  + 对于某些类，合成的版本无法正常工作。特别是当类需要`分配对象之外的资源`时，例如管理动态内存的类
  + 一些需要管理动态内存的类可通过`vector`或`string`管理，它们可被合成的版本正确拷贝、赋值、析构
  + 在不手动定义拷贝、赋值、析构的情况下，类中分配的资源都应以数据成员的形式存储。



## 二.访问控制和封装

+ **使用public和private访问说明符来进行加强类的封装性。**
+ **使用class与struct关键字：**
  + class与struct定义类的唯一区别就是默认的访问权限。

### 1.友元

+ 类通过增加friend关键字在函数开头进行声明来将其作为友元，这样就允许函数访问自己的非公有成员。
+ 友元声明只能在类的内部，但是 **声明友元之外必须再专门对函数进行一次声明。**

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



## 三.类的其他特性

### 1.类成员再探

+ **可以定义类型成员：**
  
  + 用来定义类型的成员必须先定义后使用，这和普通成员有所区别。
+ **令成员作为内联函数：**
  
  + 定义在类内部的成员函数是自动inline的。**注意，不是声明**。
+ **可变数据成员：**
  
  + 可以通过在变量的声明中加入mutable关键字做到修改类的数据成员，哪怕是在const函数中也可以。
  
    ```c++
    class Test
    {
    public:
        mutable int value;
        Test():value(0) {};
        int getValue () const {
            return value++ ;
        }
    };
    ```

### 2.返回*this的成员函数

+ 当返回值是调用set对象的引用时，意味着函数返回的是对象本身而不是对象的副本，这样的操作性更强，而且减小了开销。

+ 如果返回`*this`的成员函数其返回类型是引用，则返回的是对象本身而不是副本，是`左值`

+ 如果返回`*this`的成员函数其返回类型不是引用，则返回类型是`*this`的副本，是对象的拷贝

+ 若const成员函数以引用形式返回`*this`，则其返回的是常量引用。因为传入const成员函数的`*this`就是指向常量的指针

+ 通过区分成员函数是否是const的，可对其重载。原因是传入的隐式形参`*this`有`底层const`的差异

+ 将返回`*this`的const成员函数重载为非const成员函数是有必要的，因为非const对象调用它时希望返回一个非const引用

+ const对象上只能调用const成员函数，非const对象优先调用非const成员函数

+ 例子：重载出返回*this的const和非const的成员函数

  ```c++
  class Screen{
  public:
      //非常量对象的版本
      Screen &display(ostream &os){               //传入this指针和返回引用都非const
          do_display(os); //非const方法调用const方法，传入do_display的this指针是指向const对象
          return *this;   //返回的this指针是非const方法的形参，故指向非const对象
      }
      //常量对象的版本
      const Screen &display(ostream &os) const{   //传入指向const的this指针，返回const引用
          do_display(os);
          return *this;
      }
  private:
      //将底层实现的函数封装为private
      void do_display(ostream &os) const{         //不改变类内容也不返回类的引用，声明为const成员函数
          os<<contents;
      }
  };
  ```

  - 如上例这种在底层实现函数外套一层接口的方式不会增加运行时开销，因为定义于类内的成员函数都是内联的

   

### 3.友元再探

+ 当一个类指定了友元类，则友元类的成员函数可以访问此类包括非公有成员在内的所有成员。
+ 友元不具有传递性。
+ 也可以令成员函数作为友元。
+ 当类想把一组重载函数声明成友元时，必须每一个都进行声明。



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

- 如果未在`构造函数初值列表`中显式初始化成员，且未提供`类内初始值`，则该成员在执行构造函数体之前被`默认初始化`。
- 在构造函数初值列表中显式初始化成员相当于`初始化`，在构造函数体内赋值相当于`赋值`

+ **构造函数的初始值有时必不可少：**
  + 类成员时const或者是引用时，必须进行初始化；或者当成员属于某种类类型且该类没有定义默认构造函数时，也必须将这个成员初始化。
  + 初始化const或者引用类型的数据成员唯一机会就是通过构造函数初始值。
  + 初始化与赋值的区别事关底层效率问题，前者直接初始化数据成员，后者则先初始化再赋值。
  
  ```c++
  class ConstRef
  {
  public:
      ConstRef(int ii);
  private:
      int i;
      const int ci;
      int &ri;
  };
  //初始化const或者引用类型的数据成员的唯一机会就是通过构造函数初始值。
  ConstRef::ConstRef(int ii) {
      i = ii;
      ci = ii;
      ri = i;
  }
  //正确形式
  ConstRef::ConstRef(int ii) : i(ii), ci(ii), ri(ii) {}
  ```
  
  
  
+ 成员初始化的顺序
  
  + 成员的初始化顺序与它们在类定义中的出现顺序一致。
  
  - 例子：构造函数初值列表未规定求值顺序
  
  ```c++
  class X{
      int i;
      int j;
  public:
      X(int val): j(val), i(j) {}     //未定义行为
      X(int val): j(val), i(val) {}   //正确行为
  }
  ```
  
  

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

+ 关于隐式的类类型转换

  + `转换构造函数`：如果构造函数只接受一个实参，则它实际上定义了由那种类型转换为该类类型的隐式转换
  + 编译器只能执行一步隐式类型转换
  + 例子：转换构造函数一步转换

  ```c++
  string null_book="9-999-99999-9";
  /* 上下文：item.combine()接受一个Sales_data类型，
   * 而Sales_data类有一个接受string的构造函数，但没有接受C风格字符串的构造函数
   */
  item.combine(null_book);        //对，需要一步转换：string->Sales_data，用string创建临时量Sales_data传入
  item.combine("9-999-99999-9");  //错，需要两步转换：字符串字面值->string->Sales_data
  ```

+ 可将构造函数声明为`explicit`以禁止发生隐式类型转换，但仍可用`static_cast`做显式类型转换，此时会创建一个临时量

+ 可以通过explicit来抑制构造函数定义的隐形转换。
  + 关键字explicit只对一个实参的构造函数有效，需要多个实参的构造函数不能用于执行隐形转换。
  + explicit只允许出现在构造函数声明处。
  + explicit声明构造函数时，只能进行直接初始化的形式。
  
  ```c++
  class Sales_data{
  public:
      Sales_data()=default;
      Sales_data(const string &s, unsigned n, double p):
                  bookNo(s), units_sold(n), revenue(p*n) {}
      explicit Sales_data(const string &S): bookNo(s) {}
      explicit Sales_data(istream &);
  };
  explicit Sales_data(istream &is) {read(is,*this);} //错，定义时不应用explicit
  string null_book="9-999-99999-9";
  /* 上下文：item.combine()接受一个Sales_data类型 */
  item.combine(null_book)                     //错，string的构造函数是explixit，不可隐式转换
  item.combine(cin)                           //错，istream的构造函数是explixit，不可隐式转换
  Sales_data item1(null_book);                //对，直接初始化，显式调用构造函数
  Sales_data item2=null_book;                 //错，不可用隐式转换的拷贝初始化
  item.combine(Sales_data(null_book));        //对，显式构造
  item.combine(static_cast<Sales_data>(cin)); //对，显式转换，创建了临时的Sales_data对象
  ```
  
  

### 4.聚合类

+ 聚合类满足的条件：
  + 所有成员都是public
  + 没有定义任何构造函数
  + 没有类内初始值
  + 没有基类，没有virtual函数
+ 可以提供花括号的初值列表用于`列表初始化`聚合类的数据成员，初始值的顺序必须和声明顺序一致
+ 如果花括号初始化聚合类时参数较少，则靠后的成员被`值初始化`，类似数组
+ 像这样显式列表初始化类的对象的成员的缺点：
  - 要求成员都public
  - 将正确初始化的任务交给用户
  - 修改成员时需要修改用户代码

### 5.字面值常量类

+ 一般的字面值常量是算术类型、引用/指针，某些类也可以是字面值类型
+ 字面值类型的类可能含有constexpr成员，这样的成员必须符合constexpr的所有要求，它是隐式的const成员

+ 字面值满足的要求：
  + 数据成员都是字面值类型的聚合类是字面值常量类。
  + 满足以下条件的类也是字面值常量类：
    + 数据成员必须是字面值类型
    + 类必须至少有一个constexpr构造函数
    + 如果一个数据成员含有类内初始值，则内置类型成员的初始值必须是一条常量表达式
    + 如果成员属于某种类类型，则初始值必须使用成员自己的constexpr构造函数
    + 类必须使用析构函数的默认定义，该成员负责销毁类的对象
  
+ constexpr构造函数用于生成constexpr对象以及constexpr函数的参数/返回类型

+ 例子：字面值常量类和constexpr构造函数

  ```c++
  //定义一个字面值常量类
  class Debug{
  public:
      constexpr Debug(bool b=true): hw(b), io(b), other(b) {}             //constexpr构造函数，仅使用初值列表
      constexpr Debug(bool h, bool i, bool o): hw(h), io(i), other(o) {}  //constexpr构造函数，仅使用初值列表
      constexpr bool any() {return hw||io||other;}
      void set_hw(bool b){hw=b;}
      void set_io(bool b){io=b;}
      void set_other(bool b){other=b;}
  private:
      bool hw;    //硬件错误
      bool io;    //io错误
      bool other; //其他错误
  };
  constexpr Debug io_sub(false,true,false);   //字面值常量类的对象，调试io
  if(io_sub.any())                            //等价于 if(true)
      cerr<<"print appropriate error messages"<<endl;
  constexpr Debug prod(false);                //字面值常量类的对象，无调试
  if(prod.any())                              //等价于 if(false)
      cerr<<"print an error message"<<endl;    
  ```

### 6.类的静态成员

- 类的`静态成员`只与类本身相关，与其任何对象都无关。形式是在成员声明前加`static`关键字

- 静态成员可以是public或private，类型可是常量、引用、指针、类类型等

- 类的静态成员存在于任何对象之外，任何对象中都不包含与之相关的数据

- 静态成员不与任何对象绑定，故不存在`this指针`。因此既不能在函数体内使用this指针，也不能被声明为const成员函数。

- 静态成员函数调用非静态成员时，并不知道是哪个对象的成员

- 可用类的`作用域运算符`直接访问静态成员，也可用类的对象、引用、指针来访问静态成员

- 成员函数不用通过作用域运算符就可访问静态成员

- `静态成员函数`可在类内或类外定义，在类外定义时不可重复static关键字，`static只出现在声明中`。

- `静态数据成员`并非在创建类时被定义，因此`静态数据成员不由构造函数初始化`。

- 不能在类内部初始化`静态数据成员`，`静态数据成员必须在类外定义和初始化`，一个静态数据成员只能被定义一次

- 静态数据成员定义在任何函数之外，一旦被定义就存在于程序整个生命周期。

- 为确保静态数据成员只被定义一次，最好将其定义与其他非内联函数的定义放在同一头文件

- `静态成员函数可在类内和类外定义，静态数据成员只能在类外定义和初始化`

- 例子：声明、定义、访问静态成员

  ```c++
  //声明静态成员
  class Account{
  public:
      void calculate() {amount+=amount*interestRate;}
      static double rate() {return interestRate;} //静态成员函数，它可在类内也可在类外定义
      static void rate(double);                   //静态成员函数
  private:
      string owner;
      double amount;
      static double interestRate;                 //静态成员变量
      static double initRate();                   //静态成员函数
  };
  //定义静态成员
  void Account::rate(double newRate){         //定义静态成员函数，它可在类内也可在类外定义
      interestRate=newRate;
  }
  double Account::interestRate=initRate();    //定义静态成员变量，它只能在类外定义和初始化
  //访问静态成员
  double r;
  r=Account::rate();  //通过作用域访问
  Account ac1;
  Account *ac2=&ac1;
  r=ac1.rate();       //通过类引用访问
  r=ac2->rate();      //通过类指针访问
  ```

- 通常，类的静态数据成员不应在类内初始化。特例是，可为静态数据成员提供`const整型`的`类内初始值`，且该静态数据成员必须是`constexpr类型`，初值必须是常量表达式。它们可用到任何需要常量表达式的地方

- 例子：类内初始化的静态数据成员必须是字面值常量类型的constexpr

  ```c++
  class Account{
  public:
      static double rate(){return interestRate;}
      static void rate(double);
  private:
      static constexpr int period=30; //常量表达式
      double daily_tbl[period];       //可用于需要常量表达式的地方
  };
  ```

- 如果某个静态成员的应用场景仅限于编译器可替换其值的情况，则一个初始化的const或constexpr static不需要分别定义。相反，如果将其用于值不能替换的场景，则该成员必须有定义语句。

- 例子：必须在类外定义的情形

  ```c++
  //如果要把Account::period传给接受const int &的函数，则必须在类外定义
  //类内已提供初始值，则不可重复提供
  constexpr int Account::period;
  ```

- 如果类内部提供了一个初值，则成员定义不可再提供初值。

- 即使一个常量静态数据成员在类内被初始化了，通常也应在类外部定义一下（不提供初值）

- 在某些非静态成员非法的场合，静态成员可正常使用，如静态成员可以是不完全类型，特别的，静态数据成员的类型可以是他所属的类类型。而非静态数据成员只能被声明为它所属的类的指针或引用

- 例子：静态数据成员的类型可以是他所属的类类型

  ```c++
  class Bar{
      static Bar mem1;    //对，静态成员可以是不完全类型
      Bar *mem2;          //对，指针可以是不完全类型
      Bar mem3;           //错，数据成员必须是完全类型
  };
  ```

- 可用静态成员做默认实参，非静态不可，因为它的值属于对象的一部分，读到声明时类未完全定义

- 例子：静态成员做默认实参

  ```c++
  class Screen{
  public:
      Screen &clear(char=bkground); //用静态成员做默认实参
  private:
      static const char bkground;
  };
  ```