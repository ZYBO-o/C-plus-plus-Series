> **引入智能指针的原因：**
>
> + 动态对象的正确释放被证明是编程中极其容易出错的地方。为了更安全地使用动态对象，标准库定义了两个智能指针类型来管理动态库分配的对象。当一个对象应该被释放时，指向它的智能指针可以确保自动释放它。

- **对象的生存期：**
  - `全局对象`：程序启动时创建，程序结束时销毁
  - `局部static对象`：第一次使用前创建，程序结束时销毁
  - `局部自动对象`：定义时创建，离开定义所在程序块时销毁
  - `动态对象`：生存期由程序控制，在显式创建时创建，显式销毁时销毁
- 动态对象的正确释放极易出错。为安全使用动态对象，标准库定义了`智能指针`来管理动态对象
- 内存空间：
  - `静态内存`：局部static对象、类static数据成员、定义在任何函数之外的变量
  - `栈内存`：定义在函数内的非static对象
  - `堆内存`：动态对象，即运行时分配的对象
- 静态内存和栈内存中的对象由**编译器创建和销毁**，堆内存中的动态对象的生存期由**程序控制**

# 一.动态内存和智能指针

- C++通过一对`运算符`管理动态内存：
  - `new`算符在动态内存中为对象分配空间并返回指向该对象的指针，可选择对对象初始化
  - `delete`算符接受一个动态对象的指针，销毁该对象并释放内存
- 确保在正确时间释放内存很难：
  - 若不释放内存，导致`内存泄露`
  - 若还有指针指向该内存就将其释放，导致`指针空悬`
- C++11标准库中提供了两种`智能指针`来管理动态对象，定义于`memory头`文件：
  - `shared_ptr`允许多个指针指向同一对象
  - `unique_ptr`指针独占所指向对象
  - `weak_ptr`是伴随类，是一种弱引用，指向 `shared_ptr` 管理的对象

### 1.shared_ptr类

- 智能指针也是模板类，创建时必须在模板参数中给定其指向的类型
- 默认初始化的智能指针中保存空指针，条件判断中使用智能指针是判断其是否为空
- 解引用智能指针返回其指向的对象
- shared_ptr和unique_ptr都支持的操作见表12.1，shared_ptr独有的操作见表12.2

<div align="center">  
  <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/50.png"  width="600"/> 
</div>

<div align="center">  
  <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/51.png"  width="600"/> 
</div>

- 最安全的分配和使用动态内存的方法是调用`make_shared`函数，该函数定义于memory头文件中，它在动态内存中分配一个对象并初始化，返回指向它的shared_ptr

- `make_shared` 函数用法：

  - 是模板函数，使用时必须在模板参数中给出构造对象的类型
  - 其参数必须与构造对象的构造函数参数匹配，使用这些参数构造对象
  - 若不给实参，则对象值初始化

  ```c++
  //p1是指向一个值为"9999999999"的string
  shared_ptr<string> p1 = make_shared<string>(10,'9');
  //指向一个值为42的int的shared_ptr
  shared_ptr<int> p2 = make_shared<int>(42);
  //p3指向一个值初始化的int，值为0
  shared_ptr<int> p3 = make_shared<int>();
  //p4指向一个动态分配的空vector<string >
  auto p4 = make_shared<vector<string >>();
  ```

- 对shared_ptr进行拷贝/赋值时，每个shared_ptr会记录有多少个其他shared_ptr指向相同对象

- 每个`shared_ptr`都有一个关联的`计数器`，称为`引用计数`：

  - 一个shared_ptr的一组拷贝之间共享“引用计数管理区域”，并用原子操作保证该区域中的引用计数被互斥地访问
  - 互相独立的shared_ptr维护的引用计数也互相独立，即使指向同一对象。因此需避免互相独立的shared_ptr指向同一对象

- 改变`引用计数`：

  - `递增`：拷贝shared_ptr时，包括：用一个shared_ptr初始化另一个shared_ptr、作为参数传入函数、作为返回值从函数传出
  - `递减`：给shared_ptr赋新值、shared_ptr被销毁（例如离开作用域）
  - 一旦shared_ptr的计数器变为0，会自动释放管理的对象

- C++标准并未要求使用计数器实现引用计数，其实现取决于标准库的实现

- 指向对象的最后一个shared_ptr被销毁时，shared_ptr会通过它的`析构函数`完成对象的销毁

- `析构函数`控制此类型对象销毁时的操作，一般用于释放对象的资源。shared_ptr类型的析构函数被调用时递减引用计数，一旦计数为0即销毁对象。

- 例子：使用make_shared创建factory

  ```c++
  //创建对象并返回智能指针
  shared_ptr<Foo> factory(T arg){
      return make_shared<Foo>(arg);   //创建时计数为1，传出拷贝+1，离开作用域-1
  }
  //使用factory创建对象，使用完后销毁
  void use_factory(T arg){
      shared_ptr<Foo> p=factory(arg); //创建对象，初始引用计数为1
      /* 使用p */                     //未传出，离开作用域时计数-1变为0，对象被销毁
  }
  //使用factory创建对象，使用完后不销毁
  shared_ptr<Foo> use_factory(T arg){
      shared_ptr<Foo> p=factory(arg); //创建对象，初始引用计数为1
      /* 使用p */
      return p;                       //传出时拷贝+1，离开作用域-1，传出后计数为1
  }
  ```

- 必须确保shared_ptr在不使用时及时删除。例如容器中的shared_ptr在不使用时要erase

- 使用动态内存的`3种情况`：

  - **不知道需要使用多少对象（容器）**
  - **不知道所需对象的准确类型（多态）**
  - **需在多个对象间共享数据**

- 若两个对象共享底层数据，则某个对象被销毁时不可单方面销毁底层数据。此时应将共享的数据做成对象，在需共享它的两个类内分别用shared_ptr访问

#### 用shared_ptr实现共享资源

```c++
class StrBlob
{
public:
    typedef vector<string>::size_type  size_type;
    StrBlob();
    StrBlob(initializer_list<string> il);
    size_type size() const { return data->empty(); }

    void push_back(const string &t) { data->push_back(t); };
    void pop_back();

    string& front();
    string& back();

private:
    shared_ptr<vector<string >> data;
    void check(size_type i, const string &msg) const;
};

StrBlob::StrBlob() : data(make_shared<vector<string>>()) { }
StrBlob::StrBlob(initializer_list<string> il) : data(make_shared<vector<string>>(il)) {}

//检查下标是否越界
void StrBlob::check(size_type i, const string &msg) const {
    if(i>=data->size())
        throw out_of_range(msg);
}
//以下3个函数分别实现front、back、pop_back操作，用0来check索引判断是否为空
string &StrBlob::front(){
    check(0,"front on empty StrBlob");
    return data->front();
}
string &StrBlob::back(){
    check(0,"back on empty StrBlob");
    return data->back();
}
void StrBlob::pop_back(){
    check(0,"pop_back on empty StrBlob");
    data->pop_back();
}

int main(){

    /* 使用StrBlob */
    StrBlob b1;                         //创建新StrBlob
    {                                   //进入新作用域
        StrBlob b2={"a","an","the"};    //初始化b2
        b1=b2;                          //用b2初始化b1，它们共享底层数据
    }                                   //离开作用域，b2被释放，b1仍存在，共享的底层数据未丢失

int main(){

    /* 使用StrBlob */
    StrBlob b1;                         //创建新StrBlob
    {                                   //进入新作用域
        StrBlob b2={"a","an","the"};    //初始化b2
        b1=b2;                          //用b2初始化b1，它们共享底层数据
    }                                   //离开作用域，b2被释放，b1仍存在，共享的底层数据未丢失
    while(b1.size() != 1){
        cout<<b1.back()<<endl;
        b1.pop_back();
    }
}

```

- 对类对象使用默认版本的拷贝/赋值/销毁操作时，这些操作拷贝/赋值/销毁类的数据成员（包括智能指针）。



### 2.直接管理内存

- 两个运算符分配/释放动态内存：
  - `new`分配内存，并构造对象
  - `delete`销毁对象，并释放内存

#### 使用new动态分配和初始化对象

- 使用new/delete管理动态内存的类不能依赖动态对象成员的拷贝/赋值/销毁的任何默认操作

- 堆内存中分配的空间是`匿名`的，故new无法为其分配的对象命名，只能返回一个指向该对象的指针

- 动态对象`初始化`：

  - 默认情况下用默认初始化：内置类型的值未定义，类类型依赖默认构造函数
  - 直接初始化：用圆括号调用构造函数，或花括号列表初始化
  - 值初始化：类型名后跟一对空的圆括号。对于有默认构造函数的类类型而言，值初始化没有意义（都是调用默认构造函数），但对于内置类型值初始化可有良好定义的值
  - 拷贝初始化：使用圆括号里放单一对象，被分配的对象用它初始化。此时可用auto推导需分配的类型

- 例子：动态对象初始化

  ```c++
  /默认初始化
  int *pi=new int;                            //未定义
  string *ps=new string;                      //默认初始化为空字符串
  //直接初始化
  int *pi=new int(1024);                      //初始化为1024
  string *ps=new stirng(10,'9');              //初始化为"9999999999"
  vector<int> *pv=new vector<int>{0,1,2,3};   //初始化为{0,1,2,3,4,5}
  //值初始化
  int *pi=new int();                          //初始化为0
  string *ps=new string();                    //初始化为空字符串
  //拷贝初始化
  auto p1=new auto(obj);                      //用obj拷贝初始化p1
  auto p2=new auto{a,b,c};                    //错，括号内只能有单个初始化器
  ```

#### 动态分配的const对象

- 用new分配const对象是合法的，const对象必须初始化。
- 已有默认构造函数的类类型隐式初始化，否则必须显示初始化。
- 分配的对象是const的，new返回的指针为一个指向const的指针。

#### 内存耗尽与释放动态内存

- 若new不能分配要求的空间，则抛出名为`bad_alloc`的异常。

- 可向new算符传参来阻止抛出异常，传递了参数的new叫`定位new`。向new传入`std::nothrow`，则它不会抛出异常。若不能分配内存，则返回空指针。

- bad_alloc和nothrow都定义在头文件`new`中

  ```c++
  nt *p1=new int;            //若分配失败则抛出bad_alloc异常
  int *p2=new(nothrow) int;   //若分配失败则返回空指针
  ```

- `delete表达式`将内存归还给系统，它接受一个指针，指向需要释放的对象
- delete表达式执行两个工作：
  + 销毁指针指向的对象
  + 释放对应的内存
- 传递给delete表达式的指针 **必须指向动态内存，或是空指针**
- 用delete **释放非new分配的内存，或者将同一指针释放多次** ，都是未定义
- 编译器无法知道一个指针是否指向动态内存，也无法知道一个指针指向的内存是否已被释放，故这些错误不会被编译器发现
- const对象的值不可改变，但可被销毁

#### 动态对象的生存期直到被释放时为止

- 内置指针管理的动态对象，在显式释放之前一直存在

- 内置类型的对象被销毁时什么都不会发生（与类类型不一样）。特别是，内置指针被销毁时不影响其指向的对象。若这个内置指针指向动态对象，则空间不会被释放

  ```c++
  Foo *factory(T arg){
      return new Foo(arg);    //返回动态对象的指针，调用者负责释放它
  }
  void use_factory(T arg){
      Foo *p=factory(arg);    //分配对象，得到指向它的指针
      /* 使用p但不delete */
  }                           //离开时指针被销毁，对象仍存在。再没有指针指向该对象，该动态对象无法回收，内存泄露
  ```

- 用`new`和`delete`管理动态内存的常见问题：

  - **忘记delete内存，没有指针指向该动态内存时，内存泄露**
  - **使用已释放的对象**
  - **同一块内存释放两次**
  - **<font color = red>用智能指针管理动态内存即可避免这些问题</font>**

- delete之后，指针变为`空悬指针`，类似于未初始化的指针。为避免空悬指针，尽量在指针即将离开作用域时释放其管理的动态内存，也可在delete后立即将指针置为`nullptr`

- delete内存后将指针置nullptr的做法只对单个指针有效，若还有其他指针指向该对象则它们变为空悬指针。由于很难知道有哪些指针指向这个对象，故很难用new和delete管理动态内存

  ```c++
  int *p(new int(42));    //分配动态内存
  auto q=p;               //两个指针同时指向动态对象
  delete p;               //释放对象
  p=nullptr;              //将构造动态对象时的指针置为nullptr，但其他指针(q)变为空悬
  ```



#### 练习尝试

+ 12.6

  ```c++
  
  ```

+ 12.7

  ```c++
  
  ```



### 3.shared_pyt与new结合使用

+ shared_ptr的操作如表12.3

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/52.png"  width="600"/> 
  </div>

- 可用new返回的内置指针初始化智能指针，如果不对智能指针初始化，就被初始化为空指针

- 接受内置指针的智能指针构造函数是`explicit`的，即不能将内置指针隐式转换为智能指针，必须直接初始化

  ```C++
  shared_ptr<int> p1=new int(1024);       //错，不可隐式转换
  shared_ptr<int> p2(new int(1024));      //对，可以直接构造
  shared_ptr<int> clone(int p){
      return new int(p);                  //错，不可隐式转换
  }
  shared_ptr<int> clone(int p){
      return shared_ptr<int>(new int(p)); //对，可以直接构造
  }
  ```

- 用于初始化智能指针的内置指针必须指向动态内存，因为智能指针默认使用delete释放其指向的对象。静态内存和栈内存不需要也不能使用智能指针

- shared_ptr用于自动管理对象释放的功能，只限于其自身的一组拷贝之间，`互相独立的shared_ptr其引用计数也互相独立`，`内置指针不参与引用计数`。

- 推荐使用make_shared而不用new的内置指针初始化shared_ptr，因为make_shared可保证分配对象的同时和shared_ptr绑定，避免将一块内存绑定到多个互相独立的shared_ptr

- 例子：混合使用内置指针与智能指针

  ```c++
  void process(shared_ptr<int> ptr){  //传入时copy，计数+1
      /* 使用ptr */
  }                                   //离开作用域，计数-1
  //以下为正确用法：
  shared_ptr<int> p(new int(42));     //新建一个智能指针，引用计数为1
  process(p);                         //递增，引用计数为2
  int i=*p;														//正确，处理计数值为1
  //以下为错误用法：
  int *x(new int(1024));
  process(x);                         //错，不可将内置指针隐式转换为智能指针
  process(shared_ptr<int>(x));        //该智能指针的生存期只在这个函数中，离开时智能指针被释放，对象也被释放
  int j=*x;                           //未定义，x指向的对象已被释放,为空悬指针
  ```

- **<font color = red>使用一个内置指针来访问一个智能指针所负责的对象是很危险的，因为我们无法知道对象何时被销毁。</font>**

- 使用内置指针构造智能指针时必须立即构造，禁止混合使用两种指针，禁止传参时构造

- 将一个shared_ptr绑定到一个内置指针时，内存管理的责任被交给shared_ptr，不应该再用该内置指针访问内存

- shared_ptr定义了`get`成员函数，它返回内置指针，指向shared_ptr管理的对象。用于不兼容shared_ptr的情形。

- `get使用风险`：

  - 不可将get返回的内置指针dedete，因为原来的shared_ptr变为空悬
  - 不可用get返回的内置指针来初始化另一个shared_ptr，因为产生两套引用计数

- 例子：不可用get返回的内置指针来初始化另一个shared_ptr

  ```c++
  shared_ptr<int> p(new int(42));
  int *q=p.get();         //取底层的内置指针
  {
      shared_ptr<int>(q); //产生两个互相独立的shared_ptr，进行互相独立的引用计数
  }                       //离开程序块时，块内定义的shared_ptr计数清零，对象被释放
  int foo=*p;             //未定义，p指向的对象已被释放
  ```

- `reset`成员函数为shared_ptr赋予一个新的内置指针，同时更新原来的引用计数，必要时将原对象销毁。

- `reset`函数经常与`unique`函数一起使用，控制多个shared_ptr共享的对象。改变对象时检查自己是否是唯一的用户。若不是，可拷贝一份自己修改

- 例子：非独有时拷贝再修改

  ```c++
  if(!p.unique())                 //若p不是唯一指向该对象的shared_ptr
      p.reset(new string(*p));    //将对象拷贝一份，并用p管理这个拷贝
  *p+=newVal;
  ```



### 4.智能指针与异常

- 使用了异常处理的程序可在异常发生时让程序继续，跳进异常中断时需确保异常发生后资源被正确释放。例如new分配的对象，处理异常时要考虑delete

- 若使用智能指针，即使程序员因异常而过早结束，智能指针也可确保在指针离开作用域时释放资源

- 函数的退出有两种可能：正常处理结束或发生异常。两种情况下局部非static对象都会被销毁（包括智能指针，因此可使动态对象也被自动销毁）

- 若使用new/delete管理内存，且在new和delete之间发生异常，则内存不会被释放

- 例子：new/delete分配的内存遇到异常时可能不会释放

  ```c++
  void f(){
      int *ip=new int(42);    //分配资源
      /* 这里抛出异常 */
      delete ip;              //函数未正常结束，delete不会被执行
  }
  ```

- C++的很多类都定义了析构函数用于销毁对象释放资源。但不是所有的类都有，尤其是与C交互的类，通常要求用户显式释放使用的任何资源。若在资源的分配和释放之间发生了异常，也会有资源泄露。可用智能指针管理这些未定义析构函数的类，只需`自定义delete`操作。

- 默认情况下，shared_ptr假定它们指向的是动态内存，即销毁时对其管理的指针进行delete。这个delete也可自定义。

- 创建shared_ptr时可在参数列表中给出自定义的delete函数，该delete函数的必须接受一个指向所管理对象的内置指针

- 例子：自定义shared_ptr的delete

  ```c++
  //以下是C/C++兼容的网络库
  struct destination;
  struct connection;
  connection connect(destination *);
  void disconnect(connection);
  //以下是使用这个库，手动管理连接
  void f(destination &d){
      connection c=connect(&d);   //创建连接，类似new
      /* 使用连接 */
      disconnect(c);              //关闭连接。类似delete
  }
  //以下是使用这个库，shared_ptr管理连接
  void end_connection(connection *p){disconnect(*p);} //自定义delete操作用于关闭连接
  void f(destination &d){
      connection c=connect(&d);
      shared_ptr<connection> p(&c,end_connection);    //shared_ptr管理连接
      /* 使用连接 */
  }                                                   //shared_ptr离开作用域，自动关闭连接
  ```

- 智能指针使用`基本规范`：

  - 不使用相同的内置指针来初始化（或reset）多个智能指针
  - 不delete get()返回的指针
  - 不使用get()初始化或reset另一个智能指针
  - 谨慎使用get()返回的指针，最后一个智能指针销毁后对象就被销毁了
  - **<font color = red>若使用智能指针管理的资源不是new分配的内存，要自定义delete</font>**

#### 习题

+ 12.14

```c++

```

+ 12.15

```c++

```





### 5.unique_ptr

- 同一个时刻只能有一个unique_ptr指向给定对象。若unique_ptr被销毁，其指向的对象也被销毁

- unique_ptr的操作列于表12.4

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/53.png"  width="600"/> 
  </div>

- 定义unique_ptr时需将其绑定到一个new返回的指针上。类似shared_ptr，用内置指针初始化时必须显式构造，不可隐式转换

- unique_ptr不支持拷贝/赋值，因为独占其管理的对象

- 例子：unique_ptr不支持拷贝/赋值

  ```c++
  unique_ptr<string> p1(new string("Stegosaurus"));
  unique_ptr<string> p2(p1);  //错，不可拷贝
  unique_ptr<string> p3;
  p3=p1;                      //错，不可赋值
  ```

- 可通过release/reset成员函数将指针所有权从一个（非const）unique_ptr转移给另一个unique_ptr

  - `release`函数返回unique_ptr当前保存的指针，并将unique_ptr置为空
  - `reset`函数将unique_ptr原来指向的对象被释放，并接受一个可选的内置指针参数，令unique_ptr重新指向给定的指针。

- 例子：unique_ptr用release/reset转移权限

  ```c++
  unique_ptr<string> p1(new string("Stegosaurus"));
  unique_ptr<string> p2(p1.release());    //将p1置空并将底层的内置指针交给p2管理
  unique_ptr<string> p3(new string("Trex"));
  p2.reset(p3.release());                 //释放p2管理的对象，将p3置空并将底层的内置指针交给p2管理
  ```

- 不能拷贝unique_ptr的规则有一个例外： **可以拷贝或赋值一个将要被编译器销毁的unique_ptr，这时编译器执行一种特殊的拷贝（`移动`）。例如可从函数中返回unique_ptr，也可返回局部unique_ptr对象的拷贝**

- 例子：可拷贝将要被编译器销毁的unique_ptr

  ```c++
  //从函数中返回unique_ptr
  unique_ptr<int> clone(int p){
      return unique_ptr<int>(new int(p));
  }
  //返回局部unique_ptr对象的拷贝
  unique_ptr<int> clone(int p){
      unique_ptr<int> ret(new int(p));
      return ret;
  }
  ```

- unique_ptr默认用`delete`释放它指向的对象，也可`自定义delete`， **但unique_ptr管理删除器的方式和shared_ptr不同**

- 重载unique_ptr的删除器会影响到unique_ptr类型以及如何构造（或reset）unique_ptr对象。因此必须在unique_ptr的模板参数中提供删除器函数的指针类型，构造或reset时需提供删除器。

- 例子：自定义unique_ptr的delete

  ```c++
  //以下是C/C++兼容的网络库
  struct destination;
  struct connection;
  connection connect(destination *);
  void disconnect(connection);
  //以下是使用这个库，unique_ptr管理连接
  void end_connection(connection *p){disconnect(*p);} //自定义delete操作用于关闭连接
  void f(destination &d){
      connection c=connect(&d);
      unique_ptr<connection,decltype(end_connection) *> 
                p(&c,end_connection);                 //unique_ptr管理连接
      /* 使用连接 */
  }          
  ```

  > 必须添加一个`*`来指出我们正在使用该类型的一个指针



### 6.weak_ptr

- `weak_ptr`是一种不控制指向对象生存期的智能指针，它指向一个由shared_ptr管理的对象。

- 将weak_ptr绑定到shared_ptr指向的对象时，不会改变shared_ptr的引用计数，一旦该对象的shared_ptr引用计数清零，对象就会被释放，即使有weak_ptr指向它。

  ```c++
  auto p = make_shared<int>(42);
  cout << *p << endl;								//42
  cout << p.use_count() << endl;		//1
  shared_ptr<int> p2(p);
  cout<< p2.use_count() << endl;		//2
  weak_ptr<int> wp(p);
  cout << wp.use_count() << endl;		//2
  
  p.reset();
  p2.reset();
  cout << wp.use_count() << endl;		//0
  ```

- weak_ptr的用法如表12.5

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/54.png"  width="600"/> 
  </div>

- 创建weak_ptr时要在模板参数中给出指向对象类型，并用shared_ptr来初始化。模板参数中的类型只需能转换为shared_ptr指向的类型即可，不需严格匹配

- **由于weak_ptr的对象可能不存在，故不能用weak_ptr直接访问对象，而必须用`lock`成员函数。lock函数先检查指向对象是否存在，若存在则返回指向该对象的shared_ptr** （与初始化weak_ptr的shared_ptr共享引用计数），不存在则返回空的shared_ptr

  ```c++
  auto p = make_shared<int>(42);
  cout << *p << endl;								//42
  cout << p.use_count() << endl;		//1
  shared_ptr<int> p2(p);
  cout<< p2.use_count() << endl;		//2
  weak_ptr<int> wp(p);
  cout << wp.use_count() << endl;		//2
  if(shared_ptr<int> np = wp.lock())
  {
  	cout<< np.use_count() << endl;	//3
  }
  ```

- 例子：利用weak_ptr定义伴随指针类（类似迭代器），使用时不干涉底层对象的生存期，但在底层对象不存在时可阻止访问

  ```c++
  /* 上下文：前面例子中的StrBlob */
  class StrBlobPtr{
  public:
      StrBlobPtr():curr(0){}
      //用底层对象的shared_ptr初始化weak_ptr，在StrBlob中声明StrBlobPtr为友元才可访问
      StrBlobPtr(StrBlob &a, size_t sz=0):wptr(a.data),curr(sz){}
      //解引用，访问当前位置的元素
      string &deref() const;
      //前置递增，位置向前推进
      StrBlobPtr &incr();
  private:
      //若检查成功，返回指向底层对象的shared_ptr
      shared_ptr<vector<string>> check(size_t,const string &) const;
      //weak_ptr指向底层对象
      weak_ptr<vector<string>> wptr;
      //当前位置
      size_t curr;
  };
  //检查两项：1、底层对象是否还存在；2、索引是否越界
  shared_ptr<vector<string>> StrBlobPtr::check(size_t i,const string &msg) const{
      //使用weak_ptr检查对象是否存在
      auto ret=wptr.lock();
      if(!ret)
          throw runtime_error("unbound StrBlobPtr");
      if(i>=ret->size())              //size_t是无符号，下溢时自动变成最大值。故只需检查i>=size
          throw out_of_range(msg);
      return ret;
  }
  //解引用，访问当前位置的元素
  string &StrBlobPtr::deref() const{
      //检查当前位置是否合法，并返回shared_ptr
      auto p=check(curr,"dereference past end");
      //访问元素
      return (*p)[curr];
  }
  //对位置进行前置递增
  StrBlobPtr &StrBlobPtr::incr(){
      check(curr,"increment past end of StrBlobPtr");
      ++curr;
      return *this;
  }
  //在StrBlob定义前加上这一行，前向声明StrBlobPtr
  class StrBlobPtr;
  //在StrBlob定义中加上如下几行，声明StrBlobPtr为友元，并提供返回StrBlobPtr的接口
  friend class StrBlobPtr;
  StrBlobPtr begin();
  StrBlobPtr end();
  //在StrBlobPtr完整定义后，实现从StrBlob中得到StrBlobPtr的函数，*this是用于初始化StrBlobPtr的StrBlob
  StrBlobPtr StrBlob::begin(){return StrBlobPtr(*this);}
  StrBlobPtr StrBlob::end(){
      auto ret=StrBlobPtr(*this,data->size());
      return ret;
  }
  /* 使用StrBlob和StrBlobPtr */
  //定义一个StrBlob对象
  StrBlob b1;                         //创建新StrBlob
  {                                   //进入新作用域
      StrBlob b2={"a","an","the"};    //初始化b2
      b1=b2;                          //用b2初始化b1，它们共享底层数据
  }                                   //离开作用域，b2被释放，b1仍存在，共享的底层数未丢失
  //从StrBlob中得到StrBlobPtr
  auto p1=b1.begin(), p2=b1.end();
  //通过StrBlobPtr依次访问StrBlob中的元素
  for(int i=0; i<b1.size(); ++i){
      cout<<p1.deref()<<endl;
      p1=p1.incr();
  }
  ```



## 二.动态数组

- new/delete一次只分配/释放一个对象，但有时需要一次为很多元素分配内存，如容器扩张时
- 两种一次性分配一个`动态数组`的功能：
  - C++语言提供：另一种`new表达式`，可分配并初始化一个动态数组
  - 标准库提供：`allocator类`，可分配多个元素的内存，并将分配和初始化分离，性能更好更灵活
- 最佳实践：应优先使用容器而不是动态数组来管理可变数量的对象
- 使用容器的类可用默认版本的拷贝/赋值/析构来处理容器，而分配动态数组的类必须自定义拷贝/赋值/析构操作来处理动态数组

### 1.new和数组

- 使用new分配动态数组，要在类型名后跟一对方括号`[]`，并在方括号中指明要分配对象的数目

- 可用表示数组的类型别名来分配动态数组

  ```c++
  int *pia=new int[42];   //分配动态数组
  typedef int arrT[42];
  int *p=new arrT;        //用数组的类型别名来分配
  ```

+ new分配动态数组时，并未得到数组类型的对象，而是返回指向该数组的指针。

+ 由于new返回的不是数组类型，故不能对动态数组使用begin和end，也不能用范围for

+ new动态数组的`初始化`：

  - 默认情况下new分配的对象（单个或数组）都是`默认初始化`
  - 可用花括号对动态数组做`列表初始化`。若列表过短则剩下的值初始化，列表过长则分配失败并抛出异常`bad_array_new_length`（定义于头文件`new`）
  - 可用空的圆括号对动态数组做`值初始化`，括号内不能有值。不能用auto分配动态数组

+ 例子：new动态数组的初始化

  ```c++
  //默认初始化
  int *pia=new int[10];
  string *psa=new string[10];
  //列表初始化
  int *pia2=new int[10]{0,1,2,3,4,5,6,7,8,9};
  string *psa2=new string[10]{"a","an","the",string(3,'x')};
  //值初始化（不能在括号中给值）
  int *pia3=new int[10]();
  string *psa3=new string[10]();
  ```

+ 用new分配大小为0的数组时，new返回一个合法的非空的指针，并保证该指针与new返回的任何其他指针都不同。对于长为0的数组，该指针类似尾后迭代器。

+ 释放动态数组时可用特殊形式的delete，在指针前加空的方括号`[]`

+ 动态数组中的元素按`逆序销毁`，即从最后一个元素开始

+ 若在delete动态数组时忽略了`[]`，或在delete单个对象时使用了`[]`，其行为都是未定义

+ 即使在new时使用类型别名导致new中没有`[]`，也要在delete中写`[]`

+ 例子：销毁动态数组

  ```c++
  typedef int arrT[42];
  int *p=new arrT;
  delete []p;
  ```

+ `unique_ptr`可管理new分配的动态数组，只需在模板参数中指定类型为数组即可。unique_ptr销毁动态数组的方式是使用`delete []`

+ 例子：unique_ptr管理动态数组

  ```c++
  unique_ptr<int []> up(new int[10]); //在模板参数中指定为动态数组。由于是指针而不是数组，故不写大小
  for(size_t i=0; i!=10; ++i)
      up[i]=i;                        //下标访问unique_ptr管理的动态数组
  up.release();                       //自动调用delete []
  ```

+ 指向数组的unique_ptr操作如表12.6：

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/55.png"  width="600"/> 
  </div>

- unique_ptr指向动态数组时，不可使用点`.`和箭头`->`算符，因为指向的是数组而不是单个对象

- unique_ptr指向动态数组时，可用下标`[]`访问元素

- 若要使用shared_ptr管理动态数组，需提供`自定义delete`

- shared_ptr未定义下标算符，且智能指针都不支持指针算数运算。故shared_ptr访问数组中元素时必须用get函数取出内置指针

- 例子：shared_ptr管理动态数组

  ```c++
  shared_ptr<int> sp(new int[10],[](int *p){delete []p;});    //自定义lambda，使用delete []来释放
  for(size_t i=0; i!=10; ++i)
      *(sp.get()+i)=i;                            //使用get得到内置指针来访问元素
  sp.reset();                                     //使用自定义的lambda释放动态数组
  ```



### 2.allocator类

- new/delete在灵活性上的局限：将内存分配和对象构造组合在一起，将对象析构和内存释放组合在一起

- 分配一大块内存时，通常要按需构造对象。此时希望将内存分配和对象构造分离，只在真正需要时才构造对象

- new的局限性：

  - 分配空间时即构造对象，初始化之后再赋予新值，则每个元素被赋值两次
  - 分配空间被对象填满，可能创建了一些永远不会使用的对象
  - 没有默认构造函数的类不能用new分配动态数组

- `allocator`类定义在`memory`头文件中，提供一种类型感知的内存分配，分配的内存是原始的、未构造的。它可将内存分配和对象构造分离，将对象销毁和内存释放分离。

- allocator支持的操作见表12.7

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/56.png"  width="600"/> 
  </div>

- allocator也是模板类，需在模板参数中给出分配的对象类型。分配内存时根据给定的类型来确定恰当的内存大小和对齐位置

- `allocate`成员函数接受一个参数，指定分配能容纳多少个该对象的内存

- `construct`成员函数接受一个指针和额外参数，在指针所指位置构造一个元素，额外参数匹配到元素的构造函数

- 为使用allocate分配的内存，必须用construct构造对象。使用未构造的内存是未定义

- 早期的标准库construct只接受两个参数，一个指针和一个元素类型的值，只能把给定值拷贝进内存

- 使用完对象后必须对每个对象调用`destroy`来销毁，该成员函数接受一个指针，执行所指元素的析构函数

- 只能对真正构造了的元素进行destroy操作，对未构造的空间进行destroy是未定义

- 使用destroy销毁元素后可以再构造元素，也可将内存还给系统

- 使用`deallocate`成员函数释放内存，它接受两个参数，一个指向这块内存的指针和一个销毁元素的数量，该数量必须与allocate分配的数量相同（即只能全部释放）。

- 例子：使用allocator

  ```c++
  allocator<string> alloc;
  auto const p=alloc.allocate(n); //分配n个string的内存，返回首指针
  auto q=p;                       //指向分配区域的起始
  alloc.construct(q++);           //构造空字符串，指针推进
  alloc.construct(q++,10,'c');    //构造字符串，指针推进
  alloc.construct(q++,"hi");      //构造字符串，指针推进
  cout<<*p<<endl;                 //有效，p指向区域起始，此处有对象
  cout<<*q<<endl;                 //未定义，q指向已构造空间的尾后，此处无对象
  while(q!=p)
      alloc.destroy(--q);         //从尾部开始销毁元素
  alloc.deallocate(p,n);          //释放所有内存
  ```

+ 标准库为allocator类定义了两个`伴随算法`，用于在未初始化的内存中创建对象，它们定义于`memory`头文件中

+ allocator的伴随算法见表12.8

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/56.png"  width="600"/> 
  </div>

- uninitialized_copy类似copy，接受3个迭代器参数，前两个表示输入序列，第三个表示目的位置。目的位置必须是未构造的内存。该函数在目的位置构造元素，并返回已构造序列的尾后迭代器

- uninitialized_fill_n类似fill_n，接受一个指向目的位置的指针、一个计数、一个值。该函数在目的位置创建给定个数目的对象，用给定值初始化

- 例子：使用allocator的伴随算法

  ```c++
  allocator<int> alloc;
  auto p=alloc.allocate(vi.size()*2);                 //分配vi长度两倍的内存
  auto q=uninitialized_copy(vi.begin(),vi.end(),p);   //将vi拷贝进分配的内存，返回已拷贝区域的尾后迭代器
  uninitialized_fill_n(q,vi.size(),42);               //剩下的内存初始化未42；
  ```

  

## 三.使用标准库，文本查询程序









