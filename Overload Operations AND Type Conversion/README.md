<!-- GFM-TOC -->

* [时间与空间复杂度分析](#时间与空间复杂度分析)
  * [一、时间复杂度分析](#一时间复杂度分析)
  * [二、空间复杂度分析](#二空间复杂度分析)

<!-- GFM-TOC -->

---

- 当运算符被用于类类型对象时，可利用运算符重载为其指定新含义（以提升可读性）
- 可自定义类类型之间的转换规则

## 一.基本概念

- `重载的算符`是特殊的函数：名字由关键字`operator`后接要定义的算符共同组成，也有返回类型、参数列表、函数体。

- 重载算符函数的参数量与该算符作用的运算对象数量一样多

- 除重载调用算符`operator()`外，其他重载算符不能有默认实参

- 若一个重载算符是成员函数，则其第一个运算对象（左侧对象）隐式绑定到该类的`this`指针上。故成员函数的显式参数数量比运算对象数量少1

- 不可重载内置类型的算符。即，重载算符或者是类的成员，或者至少有一个类类型参数

- 可重载的算符见表14.1

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/60.png"  width="600"/> 
  </div>

- 只能重载已有的算符，不可发明新的算符

- `+`、`-`、`*`、`&`这4个既可是一元算符也可是二元算符，都可被重载，根据参数数量判断是哪种。

- 重载算符的`优先级`和`结合律`与对应的内置算符一致，但`求值顺序`和`短路求值`不会被保留

- 可用算符形式调用重载算符，也可直接用函数名

- 例子：可用函数名调用重载算符

  ```c++
  //对于非成员函数算符，下两行等价
  data1+data2;
  operator+(data1,data2);
  //对于成员函数算符，下两行等价
  data1+=data2;
  data1.operator+=(data2);
  ```

- 不建议重载的算符：

  - `&&`、`||`、`,`的求值顺序不会被保留（因为算符重载本质是函数调用）
  - `&&`、`||`的短路求值不会被保留
  - `,`、`&`这两种算符对类类型对象有特殊含义，不建议重载

- 如果某些操作在逻辑上与算符相关，适合重载：

  - 执行IO操作的类，重载`<<`、`>>`以和内置类型的IO操作一致
  - 检查相等性的类，重载`==`，同时也需重载`!=`
  - 单序比较操作的类，重载`<`，同时也需重载`<=`、`>`、`>=`
  - 重载算符的返回类型通常应与内置版本兼容：逻辑算符和相等算符返回bool，算术算符返回类类型的值，赋值算符和复合赋值算符返回左侧对象的引用
  - 如果类重载了算术算符或位算符，则最好也提供对应的复合赋值算符

- 选择将算符定义为成员/非成员：

  - `=`、`[]`、`()`、`->`必须是成员
  - 复合赋值算符一般是成员，但非必须
  - 改变对象状态的算符或与给定类型密切相关的算符（如`++`、`--`、`*`等）通常应该是成员
  - 具有对称性的算符（如算术、相等性、关系、位）可能转换任意一侧的对象，通常不应该是成员

- 把算符定义为成员函数时，其左侧对象必须是该类的一个对象

- 例子：成员函数算符的左侧必须是该类对象

  ```c++
  string s="world";
  string t=s+"!";     //对，左侧是string对象
  string u="hi"+s;    //若operator+是string的成员函数且未定义非成员版本，则错
  ```

  > 若operator+是string的成员，则`string u="hi"+s; `就是错误的，因为"hi"是const char*，是一种内置类型，没有成员函数。
  >
  > 而string是将 + 定义称为了普通的非成员函数，所以`string u="hi"+s;`等价于`operator+("hi",s);`



## 二.输入输出运算符

- IO库用`>>`和`<<`来执行输入和输出，IO库定义了读写内置类型的版本，类需自定义它们以支持IO

### 1.重载输出运算符<<

- 通常`输出算符`的参数和返回值：

  - 第一个参数是非常量ostream对象的引用。非常量是因为向流写内容会改变流，引用是因为ostream不可拷贝
  - 第二个参数一般是常量引用。常量是因为打印不改变对象内容，引用是因为避免拷贝
  - 返回值是它的ostream形参的引用（以便重复打印）

- IO算符左侧是iostream，故它们必须是`非成员函数`（iostream类型不可被用户改变）

- IO算符经常要读写类的非公有成员，故一般声明为`友元`

- 例子：重载输出算符

  ```c++
  //第一个参数是非常量ostream对象的引用，第二个参数一般是常量引用，返回值是它的ostream形参的引用
  ostream &operator<<(ostream &os,const Sales_data &item){
      os<<item.isbn()<<" "<<item.units_sold<<" "
        <<item.revenue<<" "<<item.avg_price();
      return os; //返回传入的ostream（以便重复打印）
  }
  ```

### 2.重载输入运算符>>

- 通常`输入算符`的参数和返回值：

  - 第一个参数是要读取的非常量istream对象的引用。非常量是因为读取也会改变流的状态
  - 第二个参数是将读入的非常量对象的引用。非常量是因为要写入该变量，引用也是因为要写入该变量
  - 返回值是它的istream形参的引用（以便重复读取）

- 输入算符必须处理`输入流可能无效`的情况（因为输入会改变程序中的变量，无效流导致读入未定义的数据），而输出算符不需要

  - 流含有错误类型的数据时读取操作可能失败
  - 读取操作到达文件末尾或遇到输入流的其他错误时也会失效

- 检查输入流是否有效应该在读完后使用前检查，不应在读之前检查（否则无法检查读取过程中发生的错误）

- 若在发现错误时对象已经被输入流改变，则改变部分是未定义的值。需要将对象重置为合法的（确定的）状态。

- 重载输入算符中若读到了不符合用户需求的格式，也可手动设置输入流的`failbit`。

- 例子：重载输入算符

  ```c++
  //第一个参数是要读取的istream的引用，第二个参数是将读入的非常量对象的引用，返回值是它的istream形参的引用
  istream &operator>>(istream &is,Sales_data &item){
      double price;
      is>>item.bookNo>>item.units_sold>>price;
      if(is)                                  //读完再判断流是否有效
          item.revenue=item.units_sold*price; //必须在使用读取的变量前判断
      else
          item=Sales_data();                  //若输入流无效，将对象重置为确定状态
      return is;                              //返回传入的istream（以便重复读取）
  }
  ```



## 三.算术和关系运算符

- 通常将算术和关系算符定义为`非成员`，以允许对左侧或右侧的变量做类型转换。

- 参数和返回值：

  - 形参都是常量引用，因为不需要改变对象状态
  - 返回值是非引用，经常是在函数内定义局部变量作为结果返回

- 若类定义了算术算符，一般也应该定义复合赋值算符，且应该用复合赋值算符实现算术算符（`算术算符比复合赋值算符多一次拷贝`）

- 例子：用复合赋值算符实现算术算符

  ```c++
  Sales_data operator+(Sales_data &lhs,Sales_data &rhs){
      Sales_data sum=lhs; //拷贝初始化
      sum+=rhs;
      return sum;         //局部变量传出时拷贝
  }
  ```

### 1.相等运算符

- 类通过相等算符来检验两个对象是否相等

- 相等算符的设计准则：

  - 定义相等算符可更容易地使用标准库容器/算法
  - 若类定义了`==`，则可判断一组对象中是否有重复的
  - 相等算符应该有`传递性`，即由`a==b`和`b==c`可推出`a==c`
  - 定义了`==`的类也应该定义`!=`
  - `==`和`!=`中的一个应该把工作委托给另外一个（可保证两个对象不是相等就是不等）

  - `==`和`!=`应该返回`bool`类型

- 例子：定义==和!=

  ```c++
  //参数为常量引用，返回值为bool
  bool operator==(const Sales_data &lhs,const Sales_data &rhs){
      return lhs.isbn()==rhs.isbn() &&
             lhs.units_sold==rhs.units_sold &&
             lhs.revenue==rhs.revenue;
  }
  //参数为常量引用，返回值为bool，委托给operator==实现
  bool operator!=(const Sales_data &lhs,const Sales_data &rhs){
      return !(lhs==rhs); //委托给operator==
  }
  ```

### 2.关系运算符

- 定义了相等算符的类经常（但非必须）定义关系算符
- 关联容器和一些算法会用到`operator<`，它比较有用
- 关系算符的设计：
  - 定义`顺序关系`，这种关系与关联容器中对关键字的要求一致
  - 若类也有`==`，则定义的关系应与`==`保持一致，即关系算符定义的等价类应与相等算符定义的`等价类`一致，即若两个对象都不小于对方，它们应该相等
  - 若不存在唯一的无歧义的`<`，则不定义`<`更好



## 四.赋值运算符

- 除`拷贝赋值算符`和`移动赋值算符`外，类还可定义其他赋值算符以使用其他类型作为右侧对象。但不需要考虑`自赋值`问题，因为类型不同。

- 赋值算符必须定义为`成员函数`，无论形参类型是什么。复合赋值算符也通常（不必须）定义为成员

- 赋值算符和复合赋值算符都应返回左侧对象的引用（对成员函数而言是`*this`）

- 例子：vector定义赋值算符用于列表初始化

  ```c++
  class StrVec{
  public:
      StrVec &operator=(initializer_list<string>);
      /* 其他成员与13.5一致 */
  };
  //重载赋值算符，通过initializer_list实现列表初始化
  StrVec &StrVec::operator=(initializer_list<string> il){
      auto data=alloc_n_copy(il.begin(),il.end());    //分配空间并将右侧对象的元素拷贝进去
      free();                                         //释放左侧对象原来管理的资源
      elements=data.first;                            //用左侧对象的指针管理拷贝的右侧对象
      first_free=cap=data.second;
      return *this                                    //返回左侧对象的引用
  }
  StrVec v;
  v={"a","an","the"}; //列表初始化（传入列表，调用参数为initializer_list的赋值算符）
  ```



## 五.下标运算符

- `容器类`经常可通过元素的位置来访问元素，这些类一般需要下标算符`[]`

- 下标算符必须是`成员函数`

- 参数和返回值：

  - 通常返回所访问元素的引用，这样可允许下标操作出现在`=`的左侧或右侧
  - 通常定义const和非const两个版本，作用于const对象的版本其返回值也应该是const

- 例子：定义下标算符

  ```c++
  class StrVec{
  public:
      //非const版本的[]，可赋值
      string &operator[](size_t n){return elements[n];}
      //const版本的[]，不可赋值，不可改变对象。重载的原因不是返回值const而是后面的const限定符
      const string &operator[](size_t n) const {return elements[n];}
      /* 其他成员与13.5一致 */
  private:
      string *elements;   //指向动态数组首元素的指针
  };
  StrVec svec;            //非const对象
  const StrVec cvec=svec; //const对象
  if(svec.size() && svec[0].empty()){
      svec[0]="zero";     //对，对非const对象调用非const版本的[]，左侧是元素的引用
      cvec[0]="zip";      //错，对const对象调用const版本的[]，左侧是元素的常量引用
  }
  ```

  

## 六.递增递减运算符

- 迭代器类通常需要实现`++`和`--`，使迭代器类可在元素序列中前后移动。

- 递增递减算符建议（非必须）设为`成员函数`，因为它们改变操作对象的状态

- 递增递减算符同时存在前置版本和后置版本，它们应该被同时定义。

  - `前置版本`实现递增/递减操作，并返回对象本身（`返回引用`）
  - `后置版本`实现递增/递减操作，并返回操作前的对象的拷贝（`返回值`）（需在操作前就拷贝）

- 前置版本和后置版本名字相同无法区分（返回类型不同不算重载），因此使后置版本接受一个额外的`int`形参以区分，该形参的唯一作用是区分前置/后置因此不需命名，使用后置时编译器为它自动赋值为0

- 后置版本只是比前置版本多一次拷贝且返回值不同，故应该用前置版本来实现后置版本

- 例子：定义前置/后置的递增/递减

  ```c++
  class StrBlobPtr{
  public:
      StrBlobPtr &operator++();
      StrBlobPtr &operator--();
      StrBlobPtr operator++(int);
      StrBlobPtr operator--(int);
      /* 其他成员与12.1.6中一致 */
  };
  //前置版本++，包含越界检查
  StrBlobPtr &StrBlobPtr::operator++(){
      check(curr,"increment past end of StrBlobPtr");
      ++curr;                 //先判断是否>=size再递增，因为右边界是开区间
      return *this;
  }
  //前置版本--，包含越界检查
  StrBlobPtr &StrBlobPtr::operator--(){
      --curr;                 //先递减再判断是否>=size（size_t下溢），因为左边界是闭区间
      check(curr,"decrement past begin of StrBlobPtr");
      return *this;
  }
  //后置版本++，委托前置++来实现
  StrBlobPtr StrBlobPtr::operator++(int){
      StrBlobPtr ret=*this;   //先拷贝一份用于返回
      ++*this;                //将本对象++的实现委托给前置版本
      return ret;             //返回改变前的拷贝
  }
  //后置版本--，委托前置--来实现
  StrBlobPtr StrBlobPtr::operator--(int){
      StrBlobPtr ret=*this;   //先拷贝一份用于返回
      --*this;                //将本对象--的实现委托给前置版本
      return ret;             //返回改变前的拷贝
  }
  ```

- 可显式调用重载的递增/递减。调用后置版本时需给int传参

- 例子：显式调用时用实参选择前置/后置

  ```c++
  StrBlobPtr p(a1);
  p.operator++(0);    //调用后置版本
  p.operator++();     //调用前置版本
  ```



## 七.成员访问运算符

- 迭代器类或智能指针类中常用到解引用算符`*`和箭头算符`->`

- `->`必须是成员函数，`*`经常也是成员函数（非必须）

- `*`和`->`是作用在指针上，访问底层对象，不改变指针本身，因此常定义为const函数

- 理论上可让`*`做任何事，但`->`只能用于获取成员。且`->`的工作经常委托给`*`来实现

- 对于表达式`point->mem`，point必须是指向类对象的指针，或是重载了`->`的对象：

  - 若point是指向类对象的指针，则等价于`(*point).mem`。
  - 若point是重载了`->`的对象，则等价于`point.operator->()->mem`。

- 因此`operator->()`应该返回一个指向类对象的指针，或是另一个重载了`->`的对象

- 例子：重载*和->

  ```c++
  class StrBlobPtr{
  public:
      //返回string的引用
      string &operator*() const {
          auto p=check(curr,"dereference past end");  //若未越界则返回指向底层对象的shared_ptr
          return (*p)[curr];                          //在底层对象中随机索引，返回索引到的元素
      }
      //返回指向string的指针
      string *operator->() const{
          return &this->operator*();                  //对operator*的返回对象取地址得到指针
      }
      /* 其他成员与12.1.6中一致 */
  };
  //使用
  StrBlob a1={"hi","bye","now"};
  StrBlobPtr p(a1);
  *p="okay";                  //解引用得到的是左值引用，可赋值
  cout<<p->size()<<endl;      //结果是4，等价于p.operator->()->size()
  cout<<(*p).size()<<endl;    //结果是4，等价于p.operator*().size()
  ```

  

