- 定义一个类时，可显式或隐式的指定在此类型对象上`拷贝`、`移动`、`赋值`、`销毁`时做什么。通过5种`成员函数`实现`拷贝控制`操作：
  - `拷贝构造函数`：用同类型的另一个对象初始化本对象时做什么（拷贝初始化）
  - `拷贝赋值算符`：将同类型的另一个对象赋值给本对象时做什么（拷贝赋值）
  - `移动构造函数`：用同类型的另一个对象初始化本对象时做什么（移动初始化）
  - `移动赋值算符`：将同类型的另一个对象赋值给本对象时做什么（移动赋值）
  - `析构函数`：本对象销毁时做什么（析构）
- 如果类未定义这些拷贝控制成员，编译器会自动合成一部分缺失的操作，因此很多类不需要自定义拷贝控制。最困难的经常是认识到什么时候需要自定义拷贝控制：编译器合成版本的行为可能并非预期。

## 一.拷贝，赋值与销毁

### 1.拷贝构造函数

- `拷贝构造函数`：这种构造函数的第一个参数是`自身类类型的引用`，且任何额外参数都有默认值。

- 拷贝构造函数的第一个参数 **必须** 是引用类型（否则传参时需要拷贝，循环调用）。虽然也可定义为非const，但几乎总是用`const引用`（不会改变被拷贝对象，且const引用能接受更多类型的参数）。

  ```c++
  class Foo{
    public:
    Foo();							//默认构造函数
    Foo(const Foo&);		//拷贝构造函数
    //...
  }
  ```

- 拷贝构造函数经常会被 **隐式使用（例如函数的传参和返回值）** ，故**不应该是`explicit`**

- 若未自定义拷贝构造函数， **<font color = red>即使定义了其他构造函数，编译器也会合成一个拷贝构造函数（这一点与默认构造函数不同）</font>**

- `合成的拷贝构造函数`若非删除，则会将其参数的非static成员逐个拷贝到正在构造的对象中：

  - 内置类型：直接拷贝
  - 类类型：用拷贝构造函数来拷贝
  - 数组：逐元素拷贝（若元素是类类型，也用拷贝构造函数）

- 例子：合成的拷贝构造函数

  ```c++
  class Sales_data{
  public:
      //拷贝构造函数，参数是该类类型的引用，经常是const引用
      Sales_data(const Sales_data &);
  private:
      string bookNo;
      int units_sold=0;
      double revenue=0.0;
  };
  //与合成的拷贝构造函数等价，逐个拷贝成员
  Sales_data::Sales_data(const Sales_data &orig):
                        bookNo(orig.bookNo),
                        units_sold(orig.units_sold),
                        revenue(orig.revenue)
                        {}
  ```

  ```c++
  //没有写拷贝构造函数，但是编译器使用了合成拷贝构造函数
  class Sales_data {
  public:
      Sales_data();
      Sales_data(string s,int Units_sold,double Revenue)
      {
          bookNo = s;
          units_sold =Units_sold;
          revenue = Revenue;
      }
      void Print()
      {
          cout << bookNo << endl;
          cout << units_sold << endl;
          cout << revenue << endl;
      }
  private:
      std::string bookNo;
      int units_sold = 0;
      double revenue = 0.0;
  };
  
  int main(){
      Sales_data sa("heoo",1,1);
      Sales_data b(sa);//编译器使用了合成拷贝构造函数
      b.Print();
  }
  ```

- 直接初始化和拷贝初始化的差别：

  - `直接初始化`：要求编译器使用普通函数匹配来选择最匹配的构造函数
  - `拷贝初始化`：要求编译器将`=`右侧运算对象拷贝到正在创建的对象中，需要时可进行隐式转换

  ```c++
  string dots(10,'.');                //直接初始化
  string s(dots);                     //直接初始化
  string s2=dots;                     //拷贝初始化
  string null_book="9-999-99999-9";   //拷贝初始化，发生C风格字符串到const string的隐式转换（临时量）
  string nines=string(100,'9');       //拷贝初始化
  ```

  > 拷贝初始化通常使用拷贝构造函数来完成，但当有一个类有一个移动构造函数，则拷贝初始化有时会使用移动构造函数而非拷贝构造函数来完成。

- 拷贝初始化的工具：

  - `拷贝构造函数`：通常使用拷贝构造函数
  - `移动构造函数`：如果该类有移动构造函数且用`右值`调用时

- 拷贝初始化发生的情况:

  - 用`=`定义变量
  - 将对象作为 **实参传递给非引用类型的形参**
  - 从 **返回类型非引用的函数返回对象**
  - 用 **花括号列表初始化一个数组的元素或一个聚合类的成员**
  - 某些类类型会对其分配的对象使用拷贝初始化。例如初始化标准库容器或是调用`insert`/`push`时容器对元素进行拷贝初始化，而用`emplace`是对元素直接初始化

- 使用explicit的构造函数时，只能直接构造，不能拷贝构造（即，explicit的构造函数不可用于拷贝构造）

- 传递实参或返回值时，不能隐式使用explicit构造函数，必须显式使用

- 例子：explicit的构造函数不可用于拷贝构造

  ```c++
  vector<int> v1(10);     //对，直接初始化
  vector<int> v2=10;      //错，vector的接受大小的构造函数是explicit
  void f(vector<int>);    //f的参数非引用，需拷贝初始化
  f(10);                  //错，int不可拷贝为vector
  f(vector<int>(10));     //对，从int直接构造临时vector
  ```

- 拷贝初始化时，编译器可以（但不是必须）跳过拷贝/移动构造函数，直接创建对象。但即使编译器跳过拷贝/移动构造函数，他们仍必须是存在且可访问的（例如不能是private）

- 例子：跳过拷贝/移动构造函数，直接创建

  ```c++
  //编译器允许将上一行代码改写为下面一行，直接构造。
  string null_book="9-999-99999-9";   //拷贝初始化
  string null_book("9-999-99999-9");  //跳过拷贝构造函数，直接构造
  ```



### 2.拷贝赋值运算符

- 类可通过拷贝构造函数来控制初始化，也可通过`拷贝赋值算符`来控制对象赋值

- `重载算符`本质上是函数，其名字是`operator`关键字后接要定义的算符的符号。为了与内置类型的赋值保持一致，赋值运算符通常返回一个指向起左侧运算对象的引用。

- 赋值算符是名为`operator=`的函数，它也有返回类型和参数列表

- 某些算符，包括赋值算符，必须定义为成员函数。若算符是成员函数，则其`左侧对象`隐式绑定到this指针。对于二元算符，例如赋值算符，其`右侧对象`作为显式参数传递。

- 拷贝赋值算符接受一个与其所在类型相同的参数，返回左侧运算对象的引用（为与内置类型的赋值保持一致）。

- 标准库通常要求容器元素的类型有赋值算符，且返回值是左侧对象的引用，因为很多操作会拷贝元素。

- 若未自定义拷贝赋值算符，编译器会合成一个

- `合成的拷贝赋值算符`若非删除，则会将右侧对象的每个非static成员赋予左侧对象的对应成员（类类型成员调用拷贝赋值算符，数组成员逐个赋值）。它返回一个指向左侧对象的引用

- 例子：合成拷贝赋值算符

  ```c++
  class Sales_data{
  public:
      //拷贝赋值算符，参数是该类类型的引用，经常是const引用，返回该类类型的引用
      Sales_data &Sales_data::operator=(const Sales_data &);
  private:
      string bookNo;
      int units_sold=0;
      double revenue=0.0;
  };
  //左侧对象隐式绑定到返回的this，引用类型。右侧对象是传入参数，常量引用类型
  Sales_data &Sales_data::operator=(const Sales_data &rhs){
      bookNo=rhs.bookNo;
      units_sold=rhs.units_sold;
      revenue=rhs.revenue;
      return *this;
  }
  ```

  ```c++
  ////没有写拷贝赋值函数，但是编译器使用了合成拷贝赋值函数
  class Sales_data {
  public:
      Sales_data();
      Sales_data(string s,int Units_sold,double Revenue)
      {
          bookNo = s;
          units_sold =Units_sold;
          revenue = Revenue;
      }
      void Print()
      {
          cout << bookNo << endl;
          cout << units_sold << endl;
          cout << revenue << endl;
      }
  private:
      std::string bookNo;
      int units_sold = 0;
      double revenue = 0.0;
  };
  
  int main(){
      Sales_data sa("heoo",1,1);
      Sales_data b = sa;//合成拷贝赋值运算符
      b.Print();
  }
  ```

  

### 3.析构函数

- 析构函数执行与构造函数相反的操作：构造函数初始化对象的非static数据成员并执行函数体，析构函数执行函数体并销毁对象的非static数据成员

- `析构函数`的名字是波浪线`~`后接类名，它没有返回值也不接受参数，故不可重载，一个类只能有一个析构函数

- 构造函数和析构函数的共性和差异：

  - 构造函数有一个显式的初值列表和一个函数体；析构函数有一个函数体和一个隐式的析构部分。
  - **<font color = red>构造函数先做成员初始化再执行函数体，成员按照类中出现的顺序初始化；析构函数先执行函数体再销毁成员，成员按照初始化的逆序销毁。</font>**

- 析构函数的析构部分是隐式的，成员如何销毁完全取决于类型：类类型成员析构时调用析构函数，内置成员析构时什么都不做，特别是，析构内置指针成员不会delete它指向的对象。

- 对象被销毁时调用析构函数：

  - 变量离开作用域被销毁
  - 类对象被销毁时成员被销毁
  - 标准库容器/数组被销毁时，元素被销毁
  - 动态对象的指针被delete时对象被销毁
  - 临时对象，创建它的完整表达式结束时被销毁

- 析构函数自动运行，故程序可按需分配资源，无需担心何时释放（前提是析构函数良好定义）

- 例子：析构函数自动运行

  ```c++
  {//进入新作用域
      Sales_data *p=new Sales_data;       //内置指针
      auto p2=make_shared<Sales_data>();  //智能指针
      Sales_data item(*p);                //拷贝构造对象
      vector<Sales_data> vec;
      vec.push_back(*p2);                 //拷贝进容器
      delete p;                           //管理动态对象的内置指针需手动delete
  }
  //离开作用域，p2、item、vec的析构函数自动运行
  //销毁p2会递减其引用计数，计数为0时对象被释放
  //销毁vec时其元素也被销毁
  ```

- 若未自定义析构函数，编译器会合成一个
- `合成析构函数`若非删除，则其函数体为空，只有隐式的析构部分。
- 析构函数体并不直接销毁成员，成员是在析构函数体执行之后的隐式析构阶段被销毁。



### 4.三/五法则

> 由于拷贝控制操作是由三个特殊的成员函数来完成的，所以我们称此为“C++三法则”。在较新的 C++11 标准中，为了支持移动语义，又增加了移动构造函数和移动赋值运算符，这样共有五个特殊的成员函数，所以又称为“C++五法则”。
>
> 也就是说，“三法则”是针对较旧的 C++89 标准说的，“五法则”是针对较新的 C++11 标准说的。为了统一称呼，后来人们把它叫做“C++ 三/五法则”。

- 三个基本操作可控制类的拷贝：`拷贝构造函数`、`拷贝赋值算符`、`析构函数`。C++11中还定义了`移动构造函数`和`移动赋值算符`

- 这5个操作不必全部定义，可以只定义一两个。但这些操作经常是一个整体，需要同时定义。

- `需要自定义析构函数的类也需要自定义拷贝和赋值`

- 若一个类需要自定义析构函数（如管理动态对象），几乎肯定它也需要自定义拷贝和赋值操作

- 例子：需要析构函数时使用默认的拷贝和赋值（反例）

  ```c++
  lass HasPtr{
  public:
      //构造对象时产生动态对象，其指针作为成员
      HasPtr(const string &s=string()) : ps(new string(s)),i(0) {}
    
      //自定义了析构函数来处理动态对象，但未自定义拷贝和赋值。故该类对象的所有拷贝共享动态对象
      ~HasPtr(){delete ps;}
  private:
      string *ps;
      int i;
  };
  //定义函数处理该类对象
  HasPtr f(HasPtr hp){    //调用函数时传入指针被拷贝给形参hp，实参和hp共享底层对象
      HasPtr ret=hp;      //拷贝，默认拷贝操作只拷贝指针，使ret和hp共享底层对象
      return ret;         //将拷贝返回
  }                       //问题1、离开函数作用域时形参hp被销毁，调用析构函数将底层对象释放，实参和ret内的指针都空悬
                          //问题2、返回类型非引用，故ret传出时拷贝，ret也被销毁，同一底层对象释放两次
  HasPtr p("some values");
  f(p);                   //调用后，p中的指针空悬，且p底层的对象被释放两次
  HasPtr q(p);            //p和q的指针都空悬
  
  ```

- `需要自定义拷贝操作的类也需要自定义赋值操作，反之亦然`

- 需要自定义拷贝和赋值时，不一定需要自定义析构函数。例如，为类的每个对象生成编号

#### 习题尝试

+ 13.15

```c++
#include<iostream>
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
```



### 5.使用=default

> 有时候当我们仅创建了有参构造函数后，如果你想调用无参构造函数编译是会报错的。因为一旦你自己定义了构造函数，系统的默认构造函数是被屏蔽的，也就是说此时是没有无参构造函数的，所以我们需要自己定义一个无参构造函数。**但是现在在C++11中，如果我们仅定义了有参构造函数，可以通过default关键字让默认构造函数恢复。**

- 将拷贝控制成员定义为`=default`可显式要求编译器生成合成的版本。只能对具有合成版本的成员函数使用（构造函数和拷贝控制成员）

- 例子：使用=default

  ```c++
  class Sales_data{
  public:
      Sales_data()=default;
      Sales_data(const Sales_data &)=default;
      Sales_data &operator=(const Sales_data &);
      ~Sales_data()=default;
  };
  //=default可在类内定义也可在类外定义
  Sales_data &Sales_data::operator=(const Sales_data &)=default;
  ```

  > 在类内用`=default`修饰成员的声明时，合成的函数将隐式地声明为内联的；不希望合成的成员时内联函数，应该只对成员的类定义使用`=default`。



### 6.阻止拷贝

- 大多数类应该定义默认构造函数、拷贝构造函数、拷贝赋值算符，无论显式还是隐式。但对某些类，这些操作并无意义。例如iostream不可拷贝。
- 若要阻止一个操作（例如禁止拷贝），不定义是无效的，因为编译器会合成。正确做法是将其定义为删除函数
- `删除函数`是这样一种函数：虽然定义了，但不可以任何形式使用。C++11可将拷贝构造函数和拷贝赋值算符定义为删除来阻止拷贝。
- 定义删除函数的方法是在参数列表后将其定义为`=delete`，它通知编译器，我们不希望定义这些成员。
- `=delete`和`=default`的区别：
  - =delete必须出现在第一次声明时，而=default只需在定义时给出。因为编译器在函数声明时就必须知道它是否为删除的，以便禁止使用它
  - =delete可对任意函数使用，而=default只能对编译器能合成的函数使用
- 不能删除析构函数。若删除析构函数则无法销毁此类型对象。

- 删除析构函数的情形：

  - 对于删除了析构函数的类，编译器不允许对该类型定义变量/临时对象
  - 若类的类类型成员删除了析构函数，则该类也不能定义变量/临时对象。
  - 对于删除了析构函数的类，可以分配该类型的动态对象，只是不能释放

- 例子：删除析构函数的类可分配动态对象

  ```c++
  struct NoDtor{
  public:
      NoDtor()=default;
      ~NoDtor()=delete;   //删除析构函数
  };
  NoDtor nd;              //错，不可创建变量
  NoDtor *p=new NoDtor(); //对，可分配动态对象但不可释放
  delete p;               //错，析构函数是删除的
  ```

对某些类的某些拷贝控制成员，编译器将合成的成员定义为删除函数**。本质上，这些规则的含义是：<font color = red>若类有数据成员不可默认构造、拷贝、赋值、销毁，则该类对应的成员函数被定义为删除。</font>**

- 删除合成析构函数：
  - 类某成员的析构函数是删除的或不可访问
- 删除合成拷贝构造函数：
  - 类某成员的拷贝构造函数是删除的或不可访问
  - 类某成员的析构函数是删除的或不可访问（可能创建无法销毁的对象）
- 删除合成拷贝赋值算符：
  - 类某成员的拷贝赋值算符是删除的或不可访问
  - 类有const成员或引用成员（可给引用成员赋值，但改变的是底层共享对象，不是期望结果）
- 删除默认构造函数：
  - 类某成员的析构函数是删除的或不可访问
  - 类的引用成员没有类内初始值
  - 类的const成员没有类内初始值且类型未显式定义默认构造函数

- C++11之前，阻止拷贝控制的方法是将拷贝构造函数和拷贝赋值算符`声明为private且不定义`

  - 声明为private，保证类外拷贝时在编译期报错
  - 声明但不定义，保证友元和成员函数拷贝时在链接期报错

- 例子：通过声明为`private`且不定义来阻止拷贝

  ```c++
  class PrivateCopy{
  public:
      PrivateCopy()=default;
      ~PrivateCopy();
  private:
      //将拷贝构造函数和拷贝赋值算符声明为private且不定义，阻止类被拷贝
      PrivateCopy(const PrivateCopy &);
      PrivateCopy &operator=(const PrivateCopy &);
  };
  ```

#### 习题尝试

+ 13.18

  ```c++
  class Employee
  {
  private:
      static int number;
  public:
      Employee(){number++}
      Employee(string s = string()) : name(s), num(number++) {}
      Employee(const Employee & e)
      {
          name = e.name;
          num = number++;
      }
      Employee&operator = (const Employee & e)
      {
          string newps = string(e.name);
          name = newps;
          num = number++;
          return *this;
      }
      int GetNum(){return num;}
  private:
      string name;
      int num;
  };
  
  int Employee::number = 0;
  
  int main()
  {
      Employee e1("Ok");
      cout << e1.GetNum() << endl;
      Employee e2("hello");
      cout << e2.GetNum() << endl;
      Employee e3(e1);
      cout << e3.GetNum() << endl;
      Employee e4 = e1;
      cout << e4.GetNum() << endl;
  
  }
  ```

  

## 二.拷贝控制和资源管理

- 管理类外资源的类通常需要定义拷贝控制成员，因为它需要析构函数来释放资源，几乎肯定也需要拷贝构造函数和拷贝赋值算符`拷贝语义`：有2种选择，可定义拷贝操作使类的行为看起来像值或指针
  - `行为像值的类`：每个对象有自己的资源。拷贝一个像值的对象时，副本和原对象完全独立。改变副本不会改变原对象，反之亦然。
  - `行为像指针的类`：所有对象共享资源。拷贝一个像指针的对象时，副本和原对象使用相同的底层数据。改变副本会改变原对象，反之亦然。
- 标准库容器和string的行为像值，shared_ptr的行为像指针，IO类型和unique_ptr不允许拷贝/赋值故不像值也不像指针



### 1.行为像值的类

- 为了提供类值的行为，对于类管理的资源，每个对象都应有一份拷贝。

- `赋值算符通常组合了析构函数和构造函数的操作`（销毁左侧对象类似析构，从右侧对象拷贝/移动类似构造），且应保证执行顺序正确，可处理自赋值。如果可能，还要保证异常安全。

  - `自赋值安全`：先拷贝右侧对象资源，再销毁左侧对象资源
  - `异常安全`：先分配空间存放临时资源，再改变左侧对象状态

- 最佳实践：定义赋值算符的步骤：

  1. 将右侧对象的资源拷贝出来
  2. 释放左侧对象的资源
  3. 让左侧对象接管从右侧对象拷出的资源

- 例子：类值版本的HasPtr

  ```c++
  class HasPtr{
  public:
      //构造时分配动态内存
      explicit HasPtr(const string &s=string()):ps(new string(s)),i(0) {}
      //拷贝构造时也分配动态内存并拷贝数据，保证各对象资源独立
      HasPtr(const HasPtr &p): ps(new string(*p.ps)),i(p.i) {}
      HasPtr &operator=(const HasPtr &);
      ~HasPtr(){delete ps;}
      string getString() {return *ps;}
  private:
      string *ps;
      int i;
  };
  //定义拷贝赋值算符，保证自赋值安全、异常安全
  HasPtr &HasPtr::operator=(const HasPtr &rhs){
      auto newp = new string(*rhs.ps);  //1、将右侧对象的资源拷出
      delete ps;                      //2、释放左侧对象的资源
      ps=newp;                        //3、让左侧对象接管拷出的资源
      i=rhs.i;
      return *this;
  }
  
  int main(){
      HasPtr st;
      cout << st.getString() << endl;
      HasPtr str("OK");
      cout << str.getString() << endl;
      return 0;
  }
  ```

  

### 2.定义行为像指针的类

- 对于行为像指针的类，需为其定义拷贝构造函数和拷贝赋值算符，来拷贝指针成员本身而不是它管理的底层资源
- 令一个类行为像指针的最好办法是用`shared_ptr`来管理类内的资源
- `引用计数`的工作方式：
  - 每个`构造函数`（除拷贝构造函数外）都会创建独立的计数器（经常放在动态内存中），记录有多少对象与正在创建的对象共享数据。每个计数器初始化时都为1。
  - `拷贝构造函数`不创建新的计数器，而是拷贝数据成员（包括指向计数器的指针），并将共享的计数器递增。
  - `析构函数`递减计数器，若计数为0则销毁底层数据。
  - `拷贝赋值算符`递增右侧对象的计数，递减左侧对象的计数。若左侧对象计数为0则销毁其底层数据
- 计数器被保存在动态内存中，每次调用构造函数（除拷贝构造函数外）创建一个对象时都分配新的计数器。拷贝或赋值对象时，拷贝指向计数器的指针，使得副本和原对象共享相同的计数器。
- 赋值运算必须处理`自赋值`。实现方式是先递增右侧对象的计数再递减左侧对象的计数（检查是否为0之前递增）
- 例子：类指针版本的HasPtr，手动实现引用计数

```c++
class HasPtr{
public:
    //构造函数
    HasPtr(const string &s=string()):
          ps(new string(s)),i(0),use(new size_t(1)) //动态内存中分配新的底层数据和计数器
          {}
    //拷贝构造函数
    HasPtr(const HasPtr &p):   ps(p.ps),i(p.i),use(p.use)                //拷贝指针，共享底层数据和计数器
          {++*use;}                                 //递增引用计数
    //拷贝赋值算符
    HasPtr &operator=(const HasPtr &);
    //析构函数
    ~HasPtr();
private:
    string *ps; //底层数据
    int i;
    size_t use; //引用计数器
};
//析构函数
HasPtr::~HasPtr(){
    if(--*use==0){  //递减计数器，若递减后为0则释放资源
        delete ps;
        delete use;
    }
}
//拷贝赋值算符，保证处理自赋值
HasPtr &HasPtr::operator=(const HasPtr &rhs){
    ++*rhs.use;     //1、递增右侧计数器
    if(--*use==0){  //2、递减左侧计数器，若递减后为0则释放资源
        delete ps;
        delete use;
    }
    ps=rhs.ps;      //3、将右侧指针拷贝到左侧，共享底层数据和计数器
    i=rhs.i;
    use=rhs.use;
    return *this;
}
```



## 三.交换操作

- 除定义拷贝控制成员外，管理资源的类通常还需定义`swap函数`（非成员）。对于与重排元素顺序的算法一起使用的类而言，swap函数很重要。这类算法需要在交换两元素时调用其类型的swap函数。若未自定义swap，则算法调用标准库定义的swap

- 交换两个对象，需要`一次拷贝和两次赋值`，即swap建立在拷贝控制之上

- `swap时尽量交换指针`，而不是分配新的副本

- 例子：交换对象时需要一次拷贝和两次赋值

  ```c++
  //以下是拷贝值实现swap，假设HasPtr是行为像值的类
  HasPtr temp=v1;
  v1=v2;
  v2=temp;
  //以下是拷贝指针实现swap，效率高很多
  string *temp=v1.ps;
  v1.ps=v2.ps;
  v2.ps=temp;
  ```

- 在类中将swap声明为友元函数，即可重载出该类型的swap

- 自定义swap不是必须的，但对于管理资源的类，自定义swap可能是很重要的优化手段

- swap经常被声明为`inline`以提高性能

- 例子：自定义swap友元函数

  ```c++
  class HasPtr{
      //声明为友元以访问private
      friend void swap(HasPtr &,HasPtr &);
      /* 其他成员定义，与“行为像值的类”相同 */
  };
  inline void swap(HasPtr &lhs,HasPtr &rhs){
      using std::swap;        //允许但不是必须使用std::swap
      swap(lhs.ps,rhs.ps);    //交换底层数据的指针，优先使用匹配的swap
      swap(lhs.i,rhs.i);      //交换内置类型的成员，优先使用匹配的swap
  }
  ```

- 不同版本的swap：

  - 对于内置类型，没有特定版本的swap，使用std::swap即可
  - 对于类类型，应使用重载的swap，即使用swap而不是std::swap

- 最佳实践：`using std::swap;`允许使用标准库的swap，使用时用`swap`尽量选择最匹配的自定义swap

- 定义了swap的类可用`swap实现赋值算符`，使用`拷贝并交换`技术，传参时拷贝右侧对象，将左侧对象与右侧对象的形参副本交换。

- 例子：用swap实现拷贝赋值算符

  ```c++
  //右侧对象按值传递，传进来时已经拷贝了一次，拷贝到形参rhs
  HasPtr &HasPtr::operator=(HasPtr rhs){
      swap(*this,rhs);    //交换左侧对象与形参rhs
      return *this;       //此时this指针指向右侧对象拷贝出的对象，形参rhs是交换前的左侧对象
  }                       //离开作用域，rhs（交换前的左侧对象）被销毁
  ```

- “拷贝并交换”技术是`自赋值安全`且是`异常安全`的：

  - 改变左侧对象之前就拷贝右侧对象，保证自赋值安全
  - 唯一可能抛出异常的是拷贝右侧对象时的new，但这发生在改变左侧对象之前



## 四.拷贝控制实例

+ 例子：使用拷贝控制进行簿记操作。每个Message可出现在多个Folder中，每个Folder也可容纳多个Message，但每个Message的内容只有一个副本。如图13.1

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/58.png"  width="600"/> 
  </div>

- 具体实现：

  - 每个Message保存一条内容和一个它所在的Folder指针的`set`，每个Folder保存一个它容纳的Message指针的`set`
  - Message类提供`save`和`remove`操作，用于给定Folder添加/删除这个Message
  - Folder类提供`addMsg`和`remMsg`操作，用于在该Folder中添加/删除一个Message
  - `拷贝`Message时，除拷贝消息内容和Folder指针集合外，还要在容纳它的所有Folder中添加指向新Message的指针
  - `析构`Message时，除析构Message对象外，还要在容纳它的所有Folder中删除指向它的指针
  - 将一个Message`赋值`给另一个Message时，除更新左侧对象的内容外，还要从容纳左侧对象的Folder中删除左侧对象，并在容纳右侧对象的Folder中添加左侧对象

- 拷贝赋值算符通常执行拷贝构造函数和析构函数的工作，此时可定义公共的操作放在private中

- 代码：

  ```c++
  class Message{
      friend class Folder;                    //允许Folder访问
  public:
      explicit Message(const string &str=""): //默认初始化为空字符串
                      contents(str)           //显式初始化内容，而Folder指针的集合隐式地默认初始化
                      {}
      Message(const Message &);               //拷贝构造函数
      Message &operator=(const Message &);    //拷贝赋值算符
      ~Message();                             //析构函数
      void save(Folder &);                    //给定Folder，双向添加
      void remove(Folder &);                  //给定Folder，双向删除
  private:
      string contents;                        //Message存放的内容
      set<Folder *> folders;                  //用集合来存储Folder的指针，构造时默认初始化
      void add_to_Folders(const Message &);   //在给定Message所属的各Folder中添加本Message
      void remove_from_Folders();             //在本Message所属的各Folder中删除本Message
  };
  //给定Folder，双向添加
  void Message::save(Folder &f){
      folders.insert(&f);
      f.addMsg(this);
  }
  //给定Folder，双向删除
  void Message::remove(Folder &f){
      folders.erase(&f);
      f.remMsg(this);
  }
  //在给定Message所属的各Folder中添加本Message
  void Message::add_to_Folders(const Message &m){
      for(auto f:m.folders)
          f->addMsg(this);
  }
  //在本Message所属的各Folder中删除本Message
  void Message::remove_from_Folders(){
      for(auto f:folders)
          f->remMsg(this);
  }
  //拷贝构造函数
  //1、拷贝数据成员
  //2、在右侧对象所属的各Folder中添加左侧对象
  Message::Message(const Message &m):
                  contents(m.contents),folders(m.folders)
  {
      add_to_Folders(m);
  }
  //析构函数
  //1、在本对象所属的各Folder中删除该对象
  //2、释放该对象的资源
  Message::~Message(){
      remove_from_Folders();
  }
  //拷贝赋值算符
  //1、在左侧对象所属的各Folder中删除左侧对象
  //2、将右侧对象的数据成员拷贝到左侧对象
  //3、在右侧对象所属的各Folder中添加左侧对象
  //先删除后添加是为了处理自赋值。若先添加后删除，自赋值时最后删除会全部删掉
  Message &Message::operator=(const Message &rhs){
      remove_from_Folders();
      contents=rhs.contents;
      folders=rhs.folders;
      add_to_Folders(rhs);
      return *this;
  }
  //swap操作
  //1、两对象所属的各Folder中分别删除两对象
  //2、交换数据成员
  //3、两对象所属的各Folder中分别添加两对象（添加的是交换前的对方）
  void swap(Message &lhs,Message &rhs){
      using std::swap;
      for(auto f:lhs.folders)
          f->remMsg(&lhs);
      for(auto f:rhs.folders)
          f->remMsg(&rhs);
      swap(lhs.folders,rhs.folders);      //调用set的swap
      swap(lhs.contents,ths.contents);    //调用string的swap
      for(auto f:lhs.folders)
          f->addMsg(&lhs);
      for(auto f:rhs.folders)
          f->addMsg(&rhs);
  }
  ```

  

## 五.动态内存管理类

- 某些类在运行时需分配可变大小的内存空间，一般可使用标准库容器来保存可变数据，也可自定义拷贝控制成员来管理动态内存

- vector的每个添加元素的成员函数都会检查空间是否足够。若足够则在下一个位置构造元素，若不够则重新分配空间，将已有元素移动到新空间中并释放旧空间，再添加元素

- 例子：使用拷贝控制实现vector的内存分配

  - alloc静态成员是allocator对象，用于获得原始内存并用construct/destroy创建和删除元素

  - 3个指针成员界定内存的3个边界，如图13.2

  - 函数alloc_n_copy分配内存并拷贝给定范围的元素

  - 函数free销毁元素并释放内存

  - 函数reallocate在内存用完时重新分配

  - 函数chk_n_alloc保证至少有空间容纳一个新元素，如果没有就调用reallocate分配更多空间

    <div align="center">  
      <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/59.png"  width="600"/> 
    </div>

+ 代码：

  ```c++
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
  #include <utility>
  using namespace std;
  class StrVec
  {
  public:
      StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}
      StrVec(const StrVec&);                      //拷贝构造函数
      StrVec &operator = (const StrVec&);         //拷贝赋值运算符
      ~StrVec();                                  //析构函数
  
      void push_back(const string&);
      size_t size() const { return first_free - elements; }
      size_t capacity() const { return cap - elements; }
      string *begin() const {return elements; }
      string *end() const {return first_free; };
  
  
  private:
      static allocator<string> alloc;//分配元素
      //被添加元素的函数所使用
      void chk_n_alloc() { if(size() == capacity()) reallocate(); }
      //工具函数，被拷贝构造函数，赋值运算符和析构函数所使用
      pair<string*, string*> alloc_n_copy(const string*, const string*);
      //销毁元素并释放内存
      void free();
      //获取更多内存并拷贝现有元素
      void reallocate();
  
      string *elements;       //指向数组首元素的指针
      string *first_free;     //指向数组第一个空闲元素的指针
      string *cap;            //指向数组为后位置的指针
  };
  
  //拷贝构造函数
  //1、将右侧对象的元素部分拷贝，得到首指针和尾后指针
  //2、让成员指针接管右侧对象拷出的动态元素
  StrVec::StrVec(const StrVec &s)
  {
      //调用alloc_n_copy分配空间以容纳与s中一样多的元素
      auto newdata = alloc_n_copy(s.begin(), s.end());
      elements = newdata.first;
      first_free = cap = newdata.second;
  }
  
  //拷贝赋值算符
  //1、将右侧对象的元素部分拷贝，得到首指针和尾后指针
  //2、销毁左侧对象管理的动态元素
  //3、让左侧对象的指针接管右侧对象拷出的动态元素
  class StrVec & StrVec::operator=(const class StrVec & rhs)
  {
      //调用alloc_n_copy分配空间,大小与rhs中元素占用空间一样多
      auto data = alloc_n_copy(rhs.begin(), rhs.end());
      free();
      elements = data.first;
      first_free = cap = data.second;
      return *this;
  }
  
  //析构函数
  //1、函数体调用free销毁该对象管理的动态元素，释放空间
  //2、隐式析构部分销毁该对象的成员
  StrVec::~StrVec(){free();}
  
  //检查空间是否够用，并在元素部分的尾部构造新元素
  void StrVec::push_back(const string & s)
  {
      //确保有足够的空间容纳新元素
      chk_n_alloc();
      //在first_free指向的元素中构造s的副本
      alloc.construct(first_free++,s);
  }
  
  //拷贝一个迭代器范围，返回元素部分的起始指针和尾后指针
  pair<string *, string *> StrVec::alloc_n_copy(const string *b, const string *e)
  {
      //分配空间保存给定范围中的元素
      auto data = alloc.allocate(e - b);
      //初始化并返回一个pair,该pair由data和uninitialized_copy的返回值构成
      return {data, uninitialized_copy(b,e,data)};
  }
  
  //若首指针非空则逆序销毁其管理的所有元素并释放动态内存
  void StrVec::free()
  {
      //不能传递给deallocate一个空指针，如果elements为0，则函数什么也不做
      if (elements)
      {
          //逆序销毁旧元素
          for (auto p = first_free;  p != elements ; )
              alloc.destroy(--p);
          alloc.deallocate(elements, cap - elements);
      }
  }
  
  //1、分配原来2倍的空间,如果StrVec为空，则分配一个容纳一个元素的空间
  //2、把原来的元素依次移动到新空间
  //3、释放原来的空间
  //4、让类的指针接管新的空间
  void StrVec::reallocate()
  {
      //分配以前大小两倍的内存空间
      auto newcapacity = size() ? 2 * size() : 1;
      //重新分配空间
      auto newdata = alloc.allocate(newcapacity);
      //将数据从旧内存移到新内存
      auto dest = newdata;    //指向新数组中的下一个空闲位置
      auto elem = elements;   //指向旧数组中的下一个元素
      for (size_t i = 0; i != size() ; ++i) {
          alloc.construct(dest++,std::move(*elem++));
      }
      //一旦完成转移，就释放旧内存
      free();
      //更新数据结构，执行新元素
      elements = newdata;
      first_free = dest;
      cap = elements + newcapacity;
  }
  ```

  

