

<!-- GFM-TOC -->

- [模板与泛型编程]()
  + [定义模板](#一定义模板)
    + [函数模板](#1函数模板)
    + [类模板](#2类模板)
    + [模板参数](#3模板参数)
    + [成员模板](#4成员模板)
    + [控制实例化](#5控制实例化)
    + [效率与灵活性](#6效率与灵活性)
  + [模板实参推断](#二模板实参推断)
    + [类型转换与模板类型参数](#1类型转换与模板类型参数)
    + [函数模板显示实参](#2函数模板显式实参)
    + [尾置返回类型与类型转换](#3尾置返回类型与类型转换)
    + [函数指针和实参推断](#4函数指针和实参推断)
    + [模板实参推断和引用](#5模板实参推断和引用)
    + [理解std::move](#6理解stdmove)
    + [转发](#7转发)
  + [重载与模板](#三重载与模板)
  + [可变参数模板](#四可变参数模板)
    + [编写可半参数函数模板](#1-编写可变参数函数模板)
    + [包扩展](#2-包扩展)
    + [转发参数包](#3转发参数包)
  + [模板特例化](#五-模板特例化)

<!-- GFM-TOC -->



- `面向对象编程(OOP)`和`泛型编程(GP)`

  都能处理在编写程序时类型未知的情况

  - OOP能处理`运行时`获取类型的情况
  - GP能处理`编译期`可获取类型的情况

- 标准库的容器、迭代器、算法都是泛型编程

- 编写泛型程序时独立于任何类型，使用泛型程序时提供类型，程序实例在该类型上运行

- `模板`是泛型编程的基础。一个`模板`是一个创建类/函数的蓝图，使用泛型类型/泛型函数时，提供信息将蓝图转换为特定的类/函数，该转换发生在编译期。



## 一.定义模板

### 1.函数模板

- 可定义一个通用的`函数模板`来处理参数为多种类型的情形，而不是为每个类型定义一个重载

- 一个`函数模板`是一个公式，用于生成针对特定类型的函数版本

- 模板定义以关键字`template`开始，后跟一个模板参数列表。

- `模板参数列表`是一个逗号分隔的一个或多个模板参数的列表，用`<>`包围起来

- 定义模板时，模板参数列表不可为空（特化时可为空）

- `模板参数`表示类/函数定义中用到的类型或值。使用模板时隐式或显式地提供`模板实参`，将其绑定到模板参数

- 调用函数模板时，编译器通常用函数实参来`推断`模板实参的类型。

- 调用函数模板时，编译器用实参中推断出的模板参数来`实例化`一个特定版本的函数。

- 实例化一个模板时，使用实际的模板实参代替对应的模板参数，来创建出模板的一个`实例`

  ```c++
  template <typename T>   //模板参数列表，T是实例化时从实参中推断的类型
  int compare(const T &v1,const T &v2){
      if(v1<v2)   return -1;
      if(v2<v1)   return 1;
      return 0;
  }
  cout<<compare(1,0)<<endl;               //实例化出T为int的版本
  vector<int> vec1{1,2,3},vec2{4,5,6};
  cout<<compare(vec1,vec2)<<endl;         //实例化出T为vector<int>的版本
  ```

- 模板参数可以是`类型参数`，可将类型参数看作类型说明符，就像内置类型/类类型一样。

- `类型参数`可用于指定返回值类型和形参类型，以及在函数体内用于变量声明/类型转换

- 类型参数前必须使用关键字`class`或`typename`，它们在模板参数列表中的含义相同，但在其他地方含义不同。一个模板参数列表中也可混用class和typename

- `typename`用于显式说明其后的内容是类型而不是变量

- 使用typename指定模板类型参数比class更直观，class是为了兼容旧代码（模板被广泛使用之后才引入typename关键字）

  ```c++
  template <typename T>           //用typename声明类型参数
  T foo(T *p){                    //类型参数可作为形参和返回类型
      T tmp=*p;                   //函数体内可使用类型参数
      return tmp;
  }
  template <typename T,U>         //错，每个类型参数前都必须有typename/class
  T calc(const T &,const U &);
  template <typename T,class U>   //对，可混用typename/class
  T calc(const T &,const U &);
  ```

- 模板参数可以是`非类型参数`，它表示一个值而非一个类型。通过一个`类型名`（而不是typename/class）来指定非类型参数

- 模板被实例化时，非类型参数被用户提供的/编译器推出的值所代替，该值必须是`constexpr`，即编译期可求值（因为模板在编译期实例化）

- 非类型参数可以是整型，或是指向对象/函数的指针/引用，不可是浮点/string/const char *

- 在非类型参数中表达浮点可用两个整型表示为分数，表达字符串可用整型字符的可变参数模板

- 绑定到非类型整型参数的实参必须是`constexpr`，绑定到非类型指针/引用参数的实参必须有`static`的生存期

- 不能用非static的局部对象/动态对象作为非类型指针/引用参数的实参，非类型指针参数可用nullptr/0来实例化

- 在模板定义内，非类型参数是一个常量值。在需要constexpr的地方都可用非类型参数

- 例子：模板非类型参数

  ```c++
  template <unsigned N,unsigned M>    //用类型名来声明非类型参数
  //形参是定长的常量字符数组的引用，使用引用是因为数组不可拷贝
  int compare(const char (&p1)[N],const char (&p2)[M]){
      return strcmp(p1,p2);
  }
  compare("hi","mom");                //实例化时，N=3,M=4
  ```

- 函数模板可声明为`inline`的或`constexpr`的，这两个说明符放在模板参数列表之后，返回类型之前

  ```c++
  template <typename T>
  inline T min(const T &,const T &);
  ```

- 编写泛型代码的两个原则：

  - 模板中的函数参数尽量是`const引用`：可用于不可拷贝的类型，且引用处理大对象更快
  - 对实参类型的要求越低越好：函数体中的比较判断仅使用`operator<`来比较

- 对于真正需要类型无关的比较，需要用`less`来进行比较，该函数能比较指针，而内置的operator<不可以

- 例子：用less进行类型无关的比较

  ```c++
  template <typename T>
  int compare(const T &v1,const T &v2){
      if(less<T>()(v1,v2))    return -1;
      if(less<T>()(v1,v2))    return 1;
      return 0;
  }
  ```

- 编译器遇到模板定义时并不生成代码，只有实例化出模板的一个特定版本时（即使用模板时）才生成代码。

- 模板与普通函数/类在文件上的区别：

  - 调用普通函数时编译器只需要知道它的声明，使用普通类类型对象时也需知道类的定义而不一定需知道成员函数的定义
  - 因此普通函数的声明和类的定义放在`头文件`中，普通函数和类成员函数的定义放在`源文件`中
  - 对于模板，编译器需要掌握函数模板/类模板成员函数的定义才能进行实例化
  - 因此函数模板和类模板成员函数的定义都放在`头文件`中

- 模板定义中有两种`名字`：

  - 不依赖模板参数的名字，使用模板时它们必须可见
  - 依赖模板参数的名字

- 模板作者和模板用户的工作：

  - `作者`的工作：
    - 保证：模板使用时，不依赖模板参数的名字必须可见。模板实例化时，模板的定义（包括类模板成员函数的定义）必须可见
    - 提供一个头文件，它包含模板的定义，以及在函数模板/类模板/类模板成员函数中用到的所有名字的声明
  - `用户`的工作：
    - 保证：用于实例化模板的所有函数、类型、与类型相关的算符的声明都必须可见
    - 包含模板的头文件，以及用于实例化此模板时使用的任何类型的头文件

- 模板直到实例化时才生成代码，故模板内代码的编译错误直到实例化才会被发现

- 编译器报错的三个阶段：

  1. `编译模板本身`：语法错误、拼写错误等
  2. `编译器遇到模板使用`：对于函数模板，检查实参数量、类型是否匹配。对于类模板，检查模板实参数量
  3. `模板实例化`：由模板生成代码，检查其中类型相关的错误。这类错误可能在链接期才会被发现

- 模板代码通常对其使用的类型（即模板类型参数）有一些假设，例如模板内对该类型的对象比较大小时可能需要operator<

- 模板的用户必须保证传入的模板实参支持模板所要求的操作



### 2.类模板

- `类模板`用来生成类的蓝图，实例化时编译器无法为类模板推导模板参数类型（与函数模板不同），而是必须在模板名后用尖括号`<>`提供实参。

- 类模板以关键字`template`开始，后跟模板参数列表。在类模板及其成员函数定义中，使用模板参数代替使用模板时用户提供的类型或值

- 使用类模板时，必须额外提供`显式模板实参列表`，它们被绑定到模板参数，编译器使用这些模板参数来实例化出特定的类

- 编译器从类模板实例化出一个类时，会重写模板，将模板参数的每个实例替换为给定的模板实参

- 类模板的每个实例都是独立的类。使用不同模板实参实例化出的类之间没有关联，也没有特殊的访问权限

- 类模板的名字不是一个类名，实例化的类名中包含模板实参

- 若一个类模板中使用了另一个类模板（如vector），可以用自己的模板参数来当作被使用的模板的实参

- 例子：类模板的定义和实例化

  ```c++
  template <typename T>
  class Blob{
  public:
      //定义类型
      typedef T value_type;
      //因为T未知，故实例化之前无法确定std::vector<T>::size_type是什么
      //故用typename来显式说明std::vector<T>::size_type是类型而不是变量
      typedef typename std::vector<T>::size_type size_type;
      //默认构造和列表初始化构造
      Blob();
      Blob(std::initializer_list<T> il);
      //委托vector来实现相关功能
      size_type size() const {return data->size();}
      bool empty() const {return data->empty();}
      void push_back(const T &t){data->push_back(t);}         //拷贝版本
      void push_back(T &&t){data->push_back(std::move(t));}   //移动版本
      //以下3个功能需用check来检查越界行为，放在类外实现
      void pop_back();
      T &back();
      T &operator[](size_type i);
  private:
      //用对应类型的vector存储底层数据，shared_ptr访问数据
      std::shared_ptr<std::vector<T>> data;
      //检查越界
      void check(size_type i,const std::string &msg) const;
  };
  //用不同类型实例化
  Blob<std::string> names;
  Blob<double> prices;
  Blob<int> ia;
  Blob<int> ia2={0,1,2,3,4};  //前面已经用int实例化过，重复的实例化不需要重新生成代码
  /* 使用int实例化时，生成的代码类似下面：
   * template <>              //模板参数列表为空
   * class Blob<int>{...};    //类名中带有模板实参列表
   * 类内的所有T替换为int
   */
  ```

- 类模板的成员函数可定义在类模板内部或外部，定义在内部时隐式地是`内联函数`

- 类模板的每个实例都有其自己版本的成员函数，故类模板的成员函数具有和类模板相同的模板参数。因此定义在类模板外的成员函数需以template开始，后接与类模板相同的模板参数列表

- 类外定义成员时必须用作用域算符`::`说明属于哪个类，而模板实例化的类名中有模板实参。故在类模板外定义成员时，类名需加上与模板形参相同的模板实参

- 类模板外定义构造函数时，类名需加模板实参，但构造函数名不需要模板实参

- 默认下，类模板的成员函数只有程序用到它时才进行实例化。即类模板实例化时成员函数不实例化

- 成员函数只有使用时才实例化这一特性，使得即使某种类型不能完全符合模板的所有需求，也可以用它来使用模板的部分功能。即使模板兼容更多的类型

- 例子：定义类模板的成员函数

  ```c++
  * 上下文：16.1.2中的Blob定义 */
  //检查越界
  //模板类外定义成员时需用作用域算符并用模板形参指明模板实参
  template <typename T>
  void Blob<T>::check(size_type i,const std::string &msg) const {
      if(i>=data->size())
          throw std::out_of_range(msg);
  }
  template <typename T>
  void Blob<T>::pop_back(){
      check(0,"pop_back on empty Blob");
      data->pop_back();
  }
  template <typename T>
  T &Blob<T>::back(){
      check(0,"back on empty Blob");
      return data->back();
  }
  template <typename T>
  T &Blob<T>::operator[](size_t i){
      check(i,"subscript out of range");
      return (*data)[i];
  }
  //默认初始化的指向vector<T>的智能指针
  //类名需加模板实参，但构造函数名不需要模板实参
  template <typename T>
  Blob<T>::Blob():data(std::make_shared<std::vector<T>>()){}
  //列表初始化vector<T>然后用智能指针指向它
  template <typename T>
  Blob<T>::Blob(std::initializer_list<T> il):data(std::make_shared<std::vector<T>>(il)){}
  //使用类模板的成员函数
  Blob<int> squares={0,1,2,3,4,5};        //实例化类模板时不会实例化成员函数
  for(size_t i=0;i!=squares.size();++i)
      squares[i]=i*i;                     //使用成员函数时才会实例化成员函数
  ```

- 使用一个类模板时必须提供模板实参，有一个例外：在类模板自己的作用域中，可直接使用模板名而不提供实参

- 处于类模板的作用域中时，编译器处理模板自身引用时，就像已经提供了与模板参数相同的实参一样。

- 但在类模板外定义成员时，直到遇到类名才进入类作用域。即，返回类型中出现模板自身时需要提供模板实参。若不提供模板实参，则编译器假定使用的实参与成员实例化所用的实参一致

- 例子：

  ```c++
  /* 上下文：16.1.2中的Blob定义 */
  template <typename T>
  class BlobPtr{
  public:
      BlobPtr():curr(0){}
      BlobPtr(Blob<T> &a,size_t sz=0):wptr(a.data),curr(sz){}
      //解引用得到底层vector中对应位置元素的引用
      T &operator*() const{
          auto p=check(curr,"dereference past end");
          return (*p)[curr];
      }
      BlobPtr &operator++();
      BlobPtr &operator--();
      /* 类模板内使用自身时，就好像已经提供了与模板参数相同的模板实参一样。
       * 即，上两行类似于：
       * BlobPtr<T> &operator++();
       * BlobPtr<T> &operator++();
       */
  private:
      std::shared_ptr<std::vector<T>> check(std::size_t,const std::string &) const;
      std::weak_ptr<std::vector<T>> wptr;
      std::size_t curr;
  };
  //定义成员函数
  //返回类型在类模板作用域外，需加模板实参。函数体在类模板作用域内，不需加模板实参
  template <typename T>
  BlobPtr<T> BlobPtr<T>::operator++(int){
      BlobPtr ret=*this;
      ++*this;
      return ret;
  }
  ```

- 当类包含`友元`声明时，类和友元各自是否是模板，是相互无关的。

- 若一个类模板包含一个友元时：

  - 若该友元不是模板，则友元是该类模板所有实例的友元
  - 若该友元是模板，则该类模板可以授权给所有友元模板实例，也可只授权给特定实例

- 类模板与另一个模板间友好关系的最常见形式是：只授权给模板实参相同的友元模板

- 当友元模板的友元声明用该模板类的模板形参作为友元自己的模板实参时，友好关系被限定在相同类型的实例之间

- 例子：仅当模板参数相同时是友元

  ```c++
  /* 上下文：16.1.2中的Blob和BlobPtr的定义 */
  //友元的特定实例是友元时，友元需前向声明
  //前置声明模板时也需要加上template关键字和模板参数列表
  template <typename> class BlobPtr;
  template <typename> class Blob;
  template <typename T> bool operator==(const Blob<T> &,const Blob<T> &);
  //对Blob定义的补充：加入友元
  template <typename T>
  class Blob{
      //声明友元时在友元名字中带上模板实参，用于说明只有这种类型的实例才是友元
      friend class BlobPtr<T>;
      friend bool operator==<T>(const Blob<T> &,const Blob<T> &);
      /* 其他成员定义和16.1.2的相同 */
  };
  ```

- 一个类也可将另一个模板的每个实例都声明为自己的友元，或限定特定的实例为友元

- 为了让友元模板的所有实例成为友元，友元声明中必须使用与类模板本身不同的模板参数

- 关于`前向声明`：

  - 将模板的特定实例声明为友元时需要将友元模板前向声明
  - 将模板的所有实例声明为友元时不需要将友元模板前向声明
  - 将非模板声明为友元时不需要将友元前向声明

- C++11中可将模板类型参数声明为友元，此时可能造成模板的友元是内置类型，这是允许的

- 例子：友元模板的所有实例可以都是友元

  ```c++
  template <typename T> class Pal;                //前向声明
  //非模板类的模板类友元
  class C{
      friend class Pal<C>;                        //用类型C实例化的Pal是友元
      template <typename T> friend class Pal2;    //Pal2的所有实例都是友元，此时不需前向声明
  };
  //模板类的模板类友元
  template <typename T>
  class C2{
      friend class Pal<T>;                        //使用相同模板实参的实例是友元
      template <typename X> friend class Pal2;    //Pal2的所有实例都是友元，此时不需前向声明
      friend class Pal3;                          //非模板类是友元，不需前向声明
  };
  //将模板类型参数声明为友元
  template <typename Type>
  class Bar{
      friend Type;                                //类型参数是友元，如用内置类型实例化会让内置类型成为友元
  };
  ```

- 类模板的一个实例定义了一个类类型，可使用`typedef`来为其定义别名。

- 模板不是类型，故不可定义typedef来引用模板，但C++11允许为类模板定义类型别名。一个模板类型别名是一组类的别名

- 定义模板类型别名时可固定一些模板参数

- 例子：为类模板定义类型别名

  ```c++
  //为类模板定义类型别名
  typedef Blob<string> StrBlob;
  //为类模板定义类型别名
  template<typename T> using twin=pair<T,T>;
  twin<string> authors;   //类型是pair<string,string>
  //定义类模板的类型别名时可固定一些模板参数
  template<typename T> using partNo=pair<T,unsigned>;
  partNo<string> books;   //类型是pair<string,unsigned>
  partNo<Student> kids;   //类型是pair<Student,unsigned>
  ```

- 类模板可声明`static`成员

- 类模板的每个实例都有自己的static成员实例。即所有的`Foo<T>`共享static成员，所有的`Foo<X>`共享static成员，但`Foo<T>`和`Foo<X>`不共享static成员

- 类的每个static数据成员必须有且仅有一个定义，但类模板的每个实例都独有static成员。因此类模板的static数据成员也应定义为模板（类似成员函数）。

- 可通过类类型的对象访问其static成员，也可用类名和作用域算符。（通过类名访问时需使用特定实例）

- 类模板的static成员函数只有使用时才实例化（与其他成员函数相同）

- 例子：类模板声明并定义static成员

  ```c++
  template <typename T>
  class Foo{
  public:
      //类内定义模板的static成员函数（直到使用该函数时才实例化）
      static std::size_t count(){return ctr;}
  private:
      //类内声明模板的static成员
      static std::size_t ctr;
  };
  //类外定义模板的static成员，将其也定义为模板，仅属于模板参数相同的类。实现不同实例间不共享
  template <typename T> size_t Foo<T>::ctr=0;
  //以下两个不同的实例之间不共享static成员
  Foo<string> fs;
  Foo<int> fi1,fi2,fi3;       //这3个对象使用同一个实例，共享static成员
  auto ct=Foo<int>::count();  //对，可用类名和作用域访问static
  ct=fi1.count();             //对，可使用对象访问static
  ct=Foo::count();            //错，类名中需要指定模板参数
  ```



### 3.模板参数

- 模板参数名的可用范围是声明之后，模板声明/定义结束之前，即遵循通常的作用域规则

- 模板参数会隐藏外层作用域的相同名字，但模板内不能重用模板参数名（即不能给模板参数的名字赋予其他含义）

- 模板参数名不可重用，故一个模板参数名在一个模板参数列表中只能出现一次

- 例子：模板参数名不可重用

  ```c++
  typedef double A;
  template <typename A,typename B>    //在模板中重用名字，隐藏模板外的相同名字
  void f(A a,B b){                    //此处A是typename中的A
      A tmp=a;
      double B;                       //错，B是模板参数名，不可重用
  }
  template <typename V,typename V>    //错，模板参数名不可重用
  ```

- 模板前置声明时必须包含模板参数

- 模板声明中的模板参数名不必与模板定义中一致，但声明和定义必须有相同数量和种类（即类型/非类型）的参数，用位置区分

- 最佳实践：一个文件中需要的所有模板的声明通常一起放在在文件起始处，出现在任何使用这些模板的代码之前

- 例子：模板的声明

  ```c++
  template <typename T> int compare(const T &,const T &); //函数模板前置声明
  template <typename T> class Blob;                       //类模板前置声明
  template <typename Type>
  int compare(const Type &,const Type &){/* 实现 */}      //声明和定义的模板参数名不必一致
  ```

- 类的static成员和类型成员都可用作用域算符`::`访问

- 对类使用作用域算符`::`

  访问成员时

  - 若该类不是模板类，则编译器掌握类的定义，它知道访问的成员是static成员还是类型成员
  - 若该类是模板类，则编译器在实例化之前不知道类的定义，它不知道访问的成员是static成员还是类型成员

- 在模板中，编译器需要知道一个名字是否表示一个类型，例如`T::size_type * p;`可以有两种解读：

  - 若`T::size_type`是类型成员，则`*`是指针
  - 若`T::size_type`是static成员，则`*`是乘法

- 处理上述二义性（即作用域算符`::`左侧的类型未知导致无法区分static成员和类型成员）的方案：

  - 编译器默认通过作用域算符访问的名字不是类型
  - 用`typename`显式通知编译器，作用域算符访问的名字是类型。（而不能用class）

- 例子：用typename显式指明一个名字是类型

  ```c++
  template <typename T>
  typename T::value_type top(const T &c){     //告知编译器，T::value_type是一个类型
      if(!c.empty())
          return c.back();
      else
          return typename T::value_type();    //告知编译器，T::value_type是一个类型
  }
  ```

- 可以为模板参数提供`默认模板实参`，C++11允许为函数模板和类模板提供默认实参，旧标准只允许为类模板提供默认实参

- 对于一个模板参数，只有当它右侧的所有参数都有默认实参时，它才可以有默认实参。即，默认实参只在参数列表的尾部（与函数默认实参一样）

- 无论何时使用类模板，都必须在模板名后加尖括号`<>`，它指出该类由模板实例化而来。特别是，即使模板参数全都使用默认实参，空的`<>`也不可省略

- 例子：函数模板默认实参

  ```c++
  //函数模板定义默认模板实参和默认函数实参
  template <typename T,typename F=less<T>>        //模板默认实参，默认F是std::less<T>
  int compare(const T &v1,const T &v2,F f=F()){   //函数默认实参，默认f是F()，即std::less<T>的调用算符
      if(f(v1,v2))    return -1;                  //使用时f是一个两参数的谓词
      if(f(v2,v1))    return 1;
      return 0;
  }
  //使用函数模板的默认实参
  bool i=compare(0,42);                           //使用默认函数实参和默认模板实参
  Sales_data item1(cin),item2(cin);
  bool j=compare(item1,item2,compareIsbn);        //传入自定义的谓词作为函数实参并推导出模板实参
  ```

- 例子：类模板默认实参

  ```c++
  //类模板定义默认模板实参
  template <class T=int>                          //模板默认实参
  class Numbers{
  public:
      Numbers(T v=0):val(v){}                     //构造函数默认实参
  private:
      T val;
  };
  //使用类模板的默认实参
  Numbers<long double> lots_of_precision;         //不使用默认实参
  Numbers<> average_precision;                    //即使参数都是默认实参，也不可省略<>
  ```

  

### 4.成员模板

- 普通类和类模板都可包含模板成员函数，这种成员是`成员模板`

- 成员模板不能是虚函数

- 定义成员模板时也以template开头并带上模板参数列表

- 例子：非模板类的成员模板

  ```c++
  //泛型可调用对象，调用时对传入的指针调用delete
  class DebugDelete{
  public:
      DebugDelete(std::ostream &s=std::cerr):os(s){}
      template <typename T>           //模板成员也是以template和模板参数列表开始
      void operator()(T *p) const{    //调用算符，可处理任何类型的指针
          os<<"deleting unique_ptr"<<std::endl;
          delete p;
      }
  private:
      std::ostream &os;
  };
  //用法1：由该类实例化出对象，对象调用成员函数模板，处理double指针
  double *p=new double;
  DebugDelete d;
  d(p);                                                           //DebugDelete::operator()<double>(double *)
  //用法2：该类的临时对象调用成员函数模板，处理int指针
  int *ip=new int;
  DebugDelete()(ip);                                              //DebugDelete::operator()<int>(int *)
  //用法3：将该类作为unique_ptr的删除器，unique_ptr的模板参数是类型，构造函数实参是该类型的临时对象
  //unique_ptr析构时调用临时对象DebugDelete()的调用算符，unique_ptr的析构函数实例化时该成员函数模板被实例化
  unique_ptr<int,DebugDelete> p(new int,DebugDelete());           //DebugDelete::operator()<int>(int *)
  unique_ptr<string,DebugDelete> sp(new string,DebugDelete());    //DebugDelete::operator()<string>(string *)
  ```

- 类模板也可有成员函数模板，此时类和成员可以各自有独立的模板参数

- 在类模板外定义其成员函数模板时，要为类模板和成员模板都提供模板参数列表。类模板的参数列表在前，成员模板的参数列表在后

- 实例化类模板的成员模板时，必须同时提供类模板和函数模板的实参（类模板实参手动给，函数模板实参自动推导）

- 例子：模板类的成员模板

  ```c++
  template <typename T>   //这是一个类模板
  class Blob{
  public:
      template <typename It>  //这个成员是模板函数0
      Blob(It b,It e);
      /* 其他成员定义 */
  };
  //类模板外定义成员模板（构造函数），需写两个模板参数列表
  template <typename T>   //类模板参数列表
  template <typename It>  //成员模板参数列表
  Blob<T>::Blob(It b,It e):data(std::make_shared<std::vector<T>>(b,e)){}
  //使用该模板构造函数来构造模板类，同时提供类模板参数和函数模板参数（从函数参数中推导）
  //int型数组初始化int型Blob
  int ia[]={0,1,2,3,4};
  Blob<int> a1(begin(ia),end(ia));
  //long型vector初始化int型Blob
  vector<long> vi={0,1,2,3};
  Blob<int> a2(vi.begin(),vi.end());
  //const char *型list初始化string型Blob
  list<const char *> w={"now","is","the","time"};
  Blob<string> a3(w.begin(),w.end());
  ```

  

### 5.控制实例化

- 模板只有被使用时才会被实例化，这意味着相同的实例会出现在多个文件中。当多个独立编译的源文件使用了相同的模板并进行相同的实例化时，每个文件中都会有该模板的该实例。这会造成额外的开销，在多个源文件使用同一个模板时额外开销可能会很大。

- C++11允许通过`显式实例化`来避免模板重复实例化的开销。显式实例化的形式如下：

  ```c++
  //下面declaration是一个类或函数的声明，该声明是模板实例，即用模板实参代替了所有模板参数
  extern template declaration;    //实例化声明
  template declaration;           //实例化定义
  ```

+ 编译器遇到`extern`模板声明时不会在本文件中生成实例化代码，声明为extern表示承诺在其他位置有该实例化的一个非extern声明（定义）

+ 对于一个给定的实例化版本，可以有多个extern声明，但必须有且仅有一个定义。

+ extern声明必须出现在使用此实例化的代码之前，否则编译器在使用模板时自动实例化，起不到外部实例化的作用

+ 编译器遇到实例化的定义（非extern）时，将为其生成实例化的代码。在`链接`时必须将extern声明的`.o`文件和非extern声明（定义）的`.o`文件链接起来

+ 显式实例化会实例化所有成员

  - 类模板的显式实例化定义会实例化其所有成员（而使用模板时的普通实例化不会实例化成员函数，因为局部少量使用不一定要用到全部功能）
  - 因为编译器显式遇到实例化定义时，假定它会被多个地方多次使用，因此会实例化尽量多的成员以保证通用性（即实例化所有成员）
  - 结果是用于显式实例化一个类模板的类型，必须能用于模板的所有成员

+ 例子：显式实例化与extern

  ```c++
  /* 上下文：16.1.2中定义的Blob */
  //文件名：Application.cc
  extern template class Blob<string>;                     //必须在外部有显式实例化，使用外部实例
  extern template int compare(const int &,const int &);   //必须在外部有显式实例化，使用外部实例
  Blob<string> sa1,sa2;
  Blob<int> a1={0,1,2,3};
  Blob<int> a2=a1;
  int i=compare(a1[0],a2[0]);
  //文件名：templateBuild.cc
  template class Blob<string>;                            //显式实例化，创建实例
  template int compare(const int &,const int &);          //显式实例化，创建实例
  //链接时要把Application.o和templateBuild.o链接起来
  ```



### 6.效率与灵活性

- shared_ptr和unique_ptr是模板设计时效率与灵活性选择的好例子，它们重载删除器的策略很不同：

  - `shared_ptr`重载删除器只需在创建或reset指针时向构造函数传递一个可调用对象即可
  - `unique_ptr`的删除器是类型的一部分，定义指针时必须显式将删除器类型指定为模板实参，并向构造函数传递删除器类型的一个可调用对象

- 在shared_ptr生存期中可随时改变其删除器类型：用一种类型的删除器构造shared_ptr，随后使用reset给它另一种类型的删除器

- 对shared_ptr而言，删除器的类型在`运行时`才能确定，因此删除器一定不是shared_ptr的成员，shared_ptr调用删除器需要跳转到外部

- shared_ptr需要调用删除器，但删除器不是它的成员，因此shared_ptr中必须有一个成员（指针或句柄类）用于访问删除器

- 例子：shared_ptr调用删除器时跳转到外部

  ```c++
  //shared_ptr的析构函数中必须有如下语句：
  //假定del是指向自定义删除器的指针，p是shared_ptr底层的指针
  //该析构函数在运行时判断是否有自定义删除器，若有则用句柄调用，否则使用默认delete
  del?del(p):delete p;
  ```

- 在unique_ptr中，删除器是类型的一部分，在声明时（`编译期`）即确定，不可改变。因此删除器是unique_ptr的成员。

- 例子：unique_ptr调用删除器时调用自己的成员

  ```c++
  //unique_ptr的析构函数中必须有如下语句：
  //假定del是unique_ptr的删除器成员，p是unique_ptr底层的指针
  //该析构函数直接调用删除器成员，不用管它是自定义删除器还是默认删除器。因为编译期即可确定
  del(p);
  ```

- 效率与灵活性的权衡：

  - shared_ptr在运行时绑定删除器，使用户重载更方便
  - unique_ptr在编译期绑定删除器，避免间接调用删除器的运行时开销



## 二.模板实参推断

- `模板实参推断`：调用函数模板时，从函数实参来推断模板实参
- 编译器使用函数调用中的实参类型来推断模板实参，用这些实参生成的函数版本与给定调用最为匹配（不一定有唯一匹配，也不总是精确匹配）

### 1.类型转换与模板类型参数

- 调用函数模板时传递给函数的实参被用于初始化函数形参。若该形参的类型使用了模板类型参数，则采用特殊的初始化规则：

  - 编译器通常不对实参进行类型转换，而是生成新的模板实例。例如算术转换、派生类向基类的转换、用户定义的转换，都不能应用于函数模板（它们会生成不需类型转换的新实例，而不是匹配到需要类型转换的实例）
  - 只有有限的几种`类型转换`会被应用于函数模板的实参（即通过类型转换将不同实参对应到同一实例）
    - 顶层const无论在形参还是实参中都会被忽略
    - const转换：可将非const对象的引用/指针传递给const的引用/指针形参
    - 数组/函数指针的转换：若函数形参不是引用类型，则可将数组或函数类型的实参转为指针（若形参是引用则不会转换）

- 将实参传递给带模板类型的函数形参时，自动应用的类型转换只有`const转换`和`数组/函数到指针`的转换

- 例子：调用模板函数时的类型转换

  ```c++
  template <typename T>
  T fobj(T,T);                    //实参被拷贝
  template <typename T>
  T fref(const T &,const T &);    //传常量引用
  string s1("a value");
  const string s2("another value");
  fobj(s1,s2);                    //调用fobj(string,string)，s2的顶层const被忽略
  fref(s1,s2);                    //调用fref(const string &,const string &)，s1可转换为底层const
  int a[10],b[42];
  fobj(a,b);                      //调用fobj(int *,int *)，数组被转为指针
  fref(a,b);                      //错，形参是引用，数组不会转指针，故a与b类型不一致（大小不同），无法实例化
  ```

- 一个模板类型参数用作多个形参时，传递给它们的实参必须有相同的类型（或可进行const转换或数组/函数到指针的转换）。若根据不同实参推出的类型参数不同，则调用错误

- 若希望多个形参各自接受类型不同的实参，可为它们赋予不同的模板类型参数

- 例子：定义不同的模板类型参数，允许实参类型不同

  ```c++
  template <typename T>
  int compare(const T &,const T &);           //接受的两个实参类型必须相同，或进行const转换或数组/函数到指针的转换
  template <typename A,typename B>
  int flexibleCompare(const A &,const B &);   //接受的两个实参类型不必相同
  long lng;
  compare(lng,1024);                          //错，由long和int推出的类型参数T不同
  flexibleCompare(lng,1024);                  //对，不是同一个类型参数，允许不同
  ```

- 对于不含模板类型参数的函数形参，不进行特殊处理，遵循普通函数的实参到形参转换规则

- 例子：不含模板类型参数的形参遵循普通函数的转换规则

  ```c++
  template <typename T>
  ostream &print(ostream &os,const T &obj){   //一个形参不含模板类型，一个形参含有模板类型
      return os<<obj;
  }
  print(cout,42);                             //实例化print(ostream &,int)
  ofstream f("output");
  print(f,10);                                //使用print(ostream &,int)，第一个参数遵循类型转换
  ```



### 2.函数模板显式实参

- 显式提供函数模板实参常用于两种情形：

  - 有时候编译器无法推断模板实参的类型（例如该类型只出现在返回类型/函数体中，不在形参列表中）
  - 有时候希望允许用户手动控制模板的实例化

- 当函数返回的类型与形参列表的任何类型都不相同时，经常需要提供显式模板实参

- `显式模板实参`：在函数名后用尖括号`<>`提供模板实参，类似类模板的实例化

- 显式模板实参按从左到右的顺序与模板参数列表匹配，只有处于模板参数列表尾部且可由实参推导出的才可忽略

- 设计需提供显式实参的模板时，应将需提供显式实参的模板参数放在模板参数列表最前面

- 例子：显式提供函数模板的实参

  ```c++
  template <typename T1,typename T2,typename T3>
  T1 sum(T2,T3);                                          //T1不可推导，需手动指定。T2和T3可推导且在列表最后，实例化时可忽略
  int i;
  long lng;
  auto val3=sum<long long>(i,lng);                        //实例化出long long sum(int,long);
  //糟糕的设计：必须指定所有模板实参才行
  template <typename T1,typename T2,typename T3>
  T3 alternative_sum(T2,T1);
  auto val2=alternative_sum<long,int,long long>(i,lng);   //实例化出long long alternative_sum(int,long);
  auto val3=alternative_sum<long long>(i,lng);            //错，显式模板实参按绑定，T3仍未知
  ```

- 对于使用显式模板实参指定了的模板参数，与其相关的形参类型也被指定。此时实参到形参遵循普通函数的类型转换

- 例子：显式提供模板实参后使用普通函数的实参类型转换

  ```c++
  template <typename T> int compare(const T &,const T &v2);
  long lng;
  compare(lng,1024);          //错，long和int不可推导出同一种类型
  compare<long>(lng,1024);    //实例化compare(long,long)，使用普通函数的实参转换
  compare<int>(lng,1024);     //实例化compare(int,int)，使用普通函数的实参转换
  ```



### 3.尾置返回类型与类型转换

- 当函数模板的返回类型不可由实参直接推导得到时，可以用显式模板实参，但这是不必要的（可以在模板内部解决的问题尽量少留给用户）

- 另一种解决方案是使用`decltype`从函数体内的局部变量中推出返回类型，而不使用模板参数

- 只能在`尾置返回类型`中decltype局部变量，因为前置返回类型不在函数的作用域中

- 例子：尾置返回中使用decltype确定函数模板的返回类类型

  ```c++
  emplate <typename It>
  auto fcn(It beg,It end) -> decltype(*beg){  //对解引用的结果decltype得到引用，只能传引用不能传值
      return *beg;
  }
  vector<int> vi={0,1,2,3};
  Blob<string> ca={"hi","bye"};
  auto &i=fcn(vi.begin(),vi.end());           //fcn返回int &
  auto &s=fcn(ca.begin(),ca.end());           //fcn返回string &
  ```

- `类型转换模板`：有时无法直接从decltype中获取需要的类型（例如对解引用的结果decltype得到引用，而有时候不希望得到引用），可使用标准库的`类型转换模板`，它们定义于头文件`type_traits`，该头文件中的类通常用于`模板元编程`

- 可使用`remove_reference`模板来脱去类型中的引用。它有一个模板类型参数和一个名为`type`的public成员。若用一个引用类型实例化remove_reference，则其type成员是脱去引用的类型

- 在模板中使用`remove_reference<???>::type`时，由于remove_reference的模板参数在编译期未知，故必须在其前面加上`typename`来说明这是一个类型

- 例子：使用remove_reference来脱去引用

  ```c++
  template <typename It>
  //remove_reference的模板参数是需要被脱引用的类型，其type成员是脱去引用后的类型
  //需使用typename的原因是无法在编译期确定remove_reference<decltype(*beg)>::type是类型还是static成员
  auto fcn2(It beg,It end) -> typename remove_reference<decltype(*beg)>::type{
      return *beg;
  }
  ```

- 标准库的每个类型转换模板：

  - 都有一个模板参数，表示转换前的类型
  - 都有一个名为type的public成员，表示转换得到的类型
  - 若不能或不必转换，则type成员就是模板参数本身

- 标准库提供的类型转换模板在表16.1中：

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/66.png"  width="600"/> 
  </div>



### 4.函数指针和实参推断

- 用一个函数模板来初始化或赋值给一个函数指针时，编译器用指针的类型来推断模板实参（即从左边的类型推断右边的类型）。

- 若无法从函数指针的类型来推导函数模板的类型，则产生错误（可通过显式实例化来解决）

- 当函数指针被函数模板初始化/赋值时，必须保证每个模板参数都能唯一地确定其类型或值

- 例子：函数模板用于初始化/赋值给函数指针

  ```c++
  template <typename T> int compare(const T &,const T &); //函数模板
  int (*pf1)(const int &,const int &)=compare;            //用函数模板初始化函数指针，从函数指针的类型中推导模板参数
  //重载func使其可接受两种类型的函数指针
  void func(int(*)(const string &,const string &));
  void func(int(*)(const int &,const int &));
  func(compare);                                          //错，无法判断是哪个重载的func，模板参数不能确定
  func(compare<int>);                                     //对，显式指出模板实例化为compare(const int &,const int &)
  ```

  

### 5.模板实参推断和引用

- 当形参是模板类型参数的引用时：

  - 若形参是模板类型参数的`左值引用T &`，则可给它传递左值。实参可以是const或非const，若实参是const则T被推断为const
  - 若形参是模板类型参数的`常量左值引用const T &`，则可给它传递任何类型的实参。此时T不会被推断为const，因为const在形参中已经有了
  - 若形参是模板类型参数的`右值引用T &&`，则可给它传递右值。此时推断出的T类型是该右值实参的类型

- 例子：形参是模板类型参数的引用

  ```c++
  int i;
  const int ci;
  //形参是模板类型参数的左值引用T &
  template <typename T> void f1(T &);
  f1(i);  //T是int
  f1(ci); //T是const int，因为const的实参不可传递给非const的引用形参
  f1(5);  //错，传给左值引用的实参必须是左值
  //形参是模板类型参数的常量左值引用const T &
  template <typename T> void f2(const T &);
  f2(i);  //T是int，因为非const的实参可传递给const的引用形参，const在形参中已有
  f2(ci); //T是int，因为const在函数形参中已有
  f2(5);  //T是int，因为const引用可绑定到右值
  //形参是模板类型参数的右值引用T &&
  template <typename T> void f3(T &&);
  f3(42); //T是int
  ```

- 通常不能将右值引用绑定到左值上，但C++有两个例外：

  - `右值引用的模板参数推断`：将`type`类型的左值传递给函数的右值引用形参`T &&`，且该右值引用形参指向模板类型参数`T`时，编译器推断模板类型参数`T`为实参的左值引用类型`type &`
  - `引用折叠`：通常不可直接定义引用的引用，但通过`类型别名`或`模板参数推断`（如上一条）可间接创建引用的引用，此时这些引用折叠为一个引用，折叠规则：
    - `T & &`、`T & &&`、`T && &`都折叠为`T &`
    - `T && &&`折叠为`T &&`

- 以上两个规则组合：若函数形参是指向模板类型参数`T`的右值引用`T &&`，则它可被绑定到左值。若传入的实参是`type`类型的左值，则推断出的模板参数类型`T`是左值引用`type &`，函数形参也是左值引用`type &`

- 例子：模板参数类型右值引用的形参被折叠为左值引用

  ```c++
  /* 上下文：上例中的i、ci、f3 */
  f3(i);  //T被推断为int &，形参int & &&被折叠为int &
  f3(ci); //T被推断为const int &，形参const int & &&被折叠为const int &
  ```

- `万能引用`：若一个函数形参是指向模板参数类型`T`的右值引用`T &&`，则可给它传递任意类型（左值或右值）的实参：

  - 传入左值时，`T`是实参类型的左值引用，形参被折叠为实参类型的左值引用
  - 传入右值时，`T`是实参类型的非引用，形参是实参类型的右值引用

- 由于万能引用导致只有在运行时才能确定形参是左值还是右值，以及模板参数是左值引用还是非引用，这使模板的编写变得困难。

- 实际中，模板类型参数右值引用的形参通常用于两种情况：`模板转发实参`（16.2.7）或`模板重载`（16.3）

- 使用模板类型参数右值引用的形参通常重载为两个版本：const左值（`拷贝`）和非const右值（`移动`）

- 例子：对模板类型参数右值引用的形参进行重载

  ```c++
  template <typename T> void f(const T &);    //拷贝版本，绑定到左值和const右值
  template <typename T> void f(T &&);         //移动版本，绑定到非const右值
  ```



### 6.理解std::move

- `std::move()`是使用模板类型参数的右值引用作为形参的一个好例子

- 使用std::move()可获得一个绑定到左值上的右值引用，它可接受任何类型实参，故是一个函数模板

- 例子：std::move()的实现

  ```c++
  template <typename T>
  //形参是万能引用：传入左值时T是左值引用，t是左值引用；传入右值时T是非引用，t是右值引用
  //返回类型是先将T脱去引用再加上右值引用：传入左值和右值都返回右值引用
  typename remove_reference<T>::type &&move(T &&t){
      //remove_reference<T>::type &&是先将T脱去引用再加上右值引用，保证返回类型一定是右值引用
      //无论传入左值还是右值，即无论t是左值引用还是右值引用，都强制转换为右值引用
      return static_cast<typename remove_reference<T>::type &&>(t);
  }
  ```

- 由于std::move()的形参是万能引用，故可传递左值也可传递右值，返回类型都是右值引用。

  - 若传入`type`类型的`右值`，则：
    - `T`是`type`
    - `t`的类型是`type &&`
    - 返回类型`remove_reference<T>::type &&`是`type &&`
    - 函数头是`type &&move(type &&)`
  - 若传入`type`类型的`左值`，则：
    - `T`是`type &`
    - `t`的类型是`type &`
    - 返回类型`remove_reference<T>::type &&`是`type &&`
    - 函数头是`type &&move(type &)`

- 不能隐式地将左值转换为右值引用，但可用`static_cast`显式转换。

- 将右值引用绑定到左值可以`截断左值`（即在这之后失去左值特性）。有时候截断左值是安全的，例如移动对象时。但必须强制使用static_cast，避免发生意外的截断

- 截断左值很容易，只需static_cast。但最好统一使用std::move()，它更好用且更易于查找程序中哪里截断了左值



### 7.转发

- 某些函数需要将其多个实参连同类型不变地转发给其他函数，此时需保持被转发的实参的所有性质，包括是否是const、左值/右值。

- 直接使用模板类型参数作为函数形参时无法保留传入实参的左值属性。因为将左值传入时发生拷贝，外部的值不会被内部代码改变，即转发该实参时失去左值属性

- 例子：转发时若存在拷贝，则无法保留顶层const和引用

  ```c++
  //该函数用于将参数转发给另一个函数，另一个函数也是它的参数
  template <typename F,typename T1,typename T2>
  void flip1(F f,T1 t1,T2 t2){
      f(t2,t1);           //该函数用于转发时改变两参数的顺序
  }
  //内部调用的函数，有一个参数是引用
  void f(int v1,int &v2){ //传入v2是引用，在函数内改变v2会影响外部
      cout<<v1<<" "<<++v2<<endl;
  }
  //通过f直接调用和flip1转发调用的区别：无法保留引用
  int i,j;
  f(42,i);                //运行f后i被改变，因为第二个参数是引用
  flip1(f,j,42);          //运行flip后j不会被改变，因为实例化flip1时T1和T2都是int，在转发时被拷贝了
  ```

- 在函数模板的形参中：

  - 使用引用可保证转发时实参的const属性被保留，因为引用类型中的const是底层const，实参为const时形参中的`T &`和`T &&`都会将T实例化为带const的类型
  - 使用右值引用可保证转发时实参的所有属性（包括const属性和左右值属性）被保留，因为此时的右值引用是万能引用，通过引用折叠可保留左值/右值属性

- 一个变量是右值引用，但它本身是左值，不可被转发给接受右值引用的函数

- 例子：使用右值引用保证转发时实参的所有属性被保留，但将变量转发给接受右值引用的函数会出错

  ```c++
  //使用模板类型参数的右值引用作为参数，转发时保留实参的const属性和左右值属性
  template <typename F,typename T1,typename T2>
  void flip2(F f,T1 &&t1,T2 &&t2){    //t1和t2可能是指向右值的引用，但它们本身一定是变量，即是左值
      f(t2,t1);
  }
  //内部调用的函数，接受右值引用
  void g(int &&i,int &j){             //传入i是右值引用，不可接受左值
      cout<<i<<" "<<j<<endl;
  }
  //可保留实参所有属性，但无法将右值转发给接受右值引用的函数
  int i;
  flip2(g,i,42);                      //错，42导致flip2中的t2是右值引用，但t2是变量（左值），不可传递给g的右值引用形参
  ```

- 函数`std::forward<type>(arg)`接受一个显式模板实参和一个变量，返回该显式模板实参类型的右值引用，即`std::forward<T>`的返回类型是`T &&`

- std::forward和std::move都定义于头文件`utility`

- 通常使用std::forward传递那些定义为模板类型参数的右值引用的函数参数。通过返回类型的引用折叠，可保证给定实参的左右值属性

- 例子：使用std::forward的情形

  ```c++
  template <typename Type>
  intermediary(Type &&arg){               //此处的右值引用是万能引用
      finalFcn(std::forward<Type>(arg));  //std::forward的返回值不是变量，且可保留转发参数的所有性质
  }
  ```

- std::forward可保留传入参数的所有属性：

  - 若传入实参是右值，则Type是非引用，arg是右值引用，std::forward的返回值Type &&是右值引用
  - 若传入实参是左值，则Type是左值引用，arg是左值引用，std::forward的返回值Type &&折叠为左值引用

- 上述代码可实现`完美转发`：

  - 通过右值引用（`万能引用`）在传入外层函数时保留实参的全部属性
  - 通过`std::forward`在传入内层函数时保留实参的全部属性

- std::forward比std::move更灵活：

  - std::forward可指定模板参数，且可对返回值使用引用折叠来保留左右值属性
  - std::move返回一定是右值引用

- std::forward和std::move最好都不要使用using声明，而应显式指明是`std::`中的

- 例子：使用万能引用和std::forward实现完美转发

  ```c++
  //使用万能引用和std::forward实现完美转发
  template <typename F,typename T1,typename T2>
  void flip(F f,T1 &&t1,T2 &&t2){                     //传入外层函数时使用万能引用保留实参的所有性质
      f(std::forward<T2>(t2),std::forward<T1>(t1));   //传入内层函数时使用std::forward保留实参的所有性质（尤其是，不再是变量）
  }
  //内部调用的函数，接受右值引用
  void g(int &&i,int &j){                             //传入i是右值引用，不可接受左值
      cout<<i<<" "<<j<<endl;
  }
  //整个转发过程中都保留实参的所有属性
  int i;
  flip(g,i,42);   /* 在该调用中：
                   * 第一步通过右值引用（万能引用）在传入外层函数时保留属性。T1是int &，T2是int &&，
                   * 第二步通过std::forward在传入内层函数时保留属性。std::forward<T1>是int &，std::forward<T2>是int &&
                   * 最终传入f的两个实参分别为int &&和int &，与最外层的实参完全一致
                   */
  ```



## 三.重载与模板

- 函数模板可被另一个函数模板或普通非模板函数重载。名字相同的函数必须有不同数量/类型的参数

- 设计到函数模板的`函数匹配规则`（不设计模板的函数匹配规则见6.6）：

  - `候选函数`包括所有模板实参推断成功的函数模板实例
  - 函数模板得到的候选函数都是`可行函数`，因为模板实参推断会排除不可行的模板
  - 可行函数按类型转换来排序，虽然用于函数模板的类型转换非常有限（const转换和数组/函数到指针的转换）
  - 若恰有一个函数提供比其他函数都好的匹配，则选择它
  - 若有多个函数提供一样好的匹配，则：
    - 若只有一个是非模板函数，则选择非模板函数
    - 若只有多个函数模板，且有一个模板比其他模板更特化，则选择更特化的模板
    - 否则有歧义，调用失败

- 多个重载的模板对一个调用提供同样好的匹配时，选择最特例化的版本。因为形如`const T &`的形参可用于任何类型，若不进行这种设计会导致特化的类型无法被调用

- 例子：函数模板的重载与匹配

  ```c++
  //最通用版本：接受常量引用，打印对象的string表示
  template <typename T>
  string debug_rep(const T &t){
      ostringstream ret;
      ret<<t;
      return ret.str();
  }
  //输入指针的版本：打印地址。
  //该版本不可接受char *，因为IO库为char *定义了<<，默认打印\0结尾的字符数组，而不是地址
  template <typename T>
  string debug_rep(T *p){
      ostringstream ret;
      ret<<"pointer: "<<p;
      if(p)
          ret<<" "<<debug_rep(*p);
      else
          ret<<" null pointer";
      return ret.str();
  }
  string s("hi");
  cout<<debug_rep(s)<<endl;   //只能匹配到debug_rep<string>(const T &t)，T是string
  cout<<debug_rep(&s)<<endl;  /* 可以匹配到debug_rep<string *>(const string *&)，需要普通指针到const指针的转换
                               * 也可匹配到debug_rep<string>(string *)，是精确匹配
                               * 选择精确匹配的版本：debug_rep<string>(string *)
                               */
  const string *sp=&s;
  cout<<debug_rep(sp)<<endl;  /* 可以匹配到debug_rep<string *>(const string *&)，是精确匹配
                               * 也可匹配到debug_rep<string>(string *)，是精确匹配
                               * 选择更特例化的版本：debug_rep<string>(string *)
                               */
  ```

- 当函数模板和普通函数都被匹配时，选择普通函数。原因和优先选择特化的模板一样，普通函数可看作完全特化的模板。

- 例子：函数模板和普通函数都被匹配时，选择普通函数

  ```c++
  /* 上下文：本节的所有例子 */
  //普通函数，非模板
  string debug_rep(const string &s){
      return '"'+s+'"';
  }
  string s("hi");
  cout<<debug_rep(s)<<endl;   /* 可以匹配到debug_rep<string>(const string &)，函数模板
                               * 也可匹配到debug_rep(const string &)，非模板函数
                               * 选择非模板的版本：debug_rep(const string &)
                               */
  ```

- 对于C风格字符串指针和字符串字面常量，希望它们调用接受string的版本，但存在类型转换，需要套一个接口

- 例子：给接受string的函数套上接口使其可接受C风格字符串指针和字符串字面常量（不存在类型转换）

  ```c++
  /* 上下文：本节的所有例子 */
  cout<<debug_rep("hi world!")<<endl; /* 可以匹配到debug_rep<char [10]>(const char [10]&)，精确匹配
                                       * 也可匹配到debug_rep<const char>(const char *)，精确匹配
                                       * 也可匹配到debug_rep(const string &)，要求const char *的类型转换
                                       * 选择精确匹配中更特例化的模板：debug_rep<const char>(const char *)
                                       */
  //定义接受C风格字符串指针和字符串字面常量的接口，使得可顺利调用接受string的普通函数
  string debug_rep(char *p){
      return debug_rep(string(p));
  }
  string debug_rep(const char *p){
      return debug_rep(string(p));
  }
  cout<<debug_rep("hi world!")<<endl; //此时选择精确匹配的非模板函数：debug_rep(const char *p)
  ```

- 通常忘记声明普通函数会导致错误，但对于重载函数模板的函数而言不会。若编译器可从模板实例化出匹配的版本，编译期就不会报错，但运行时会调用不期望的版本

- 最佳实践：在定义任何函数之前，记得声明所有重载的版本，保证希望调用的版本一定可见



## 四.可变参数模板

- 一个`可变参数模板`是一个接受可变数目参数的函数模板/类模板，这些可变数量的参数被称为`参数包`

- `模板参数包`表示零个或多个模板参数，`函数参数包`表示零个或多个函数参数

- 用省略号`...`来指出一个模板参数/函数参数是一个包。

  - 在模板参数列表中：
    - class或typename后跟…指出接下来的参数表示零个或多个类型的列表
    - 类型名后跟…表示零个或多个该类型的非类型参数的列表
  - 在函数参数列表中，若一个参数的类型是模板参数包，则该参数也是一个函数参数包

- 编译器从函数实参推断模板参数类型。对于可变参数模板，编译器会推断包中参数的数目

- 可用`sizeof...`算符得到包中元素的数目，它返回一个constexpr，且不会对实参求值。

- 例子：可变参数模板

  ```c++
  //定义可变参数模板
  template <typename T,typename... Args>      //模板类型参数Args是一个模板参数包，代表数量可变的类型参数
  void foo(const T &t,const Args &... rest);  //形参rest是一个函数参数包，代表数量可变的形参。const Args &...是包扩展
  //使用可变参数模板
  int i=0;double d=3.14;string s="how now brown cow";
  foo(i,s,42,d);  //实例化foo(const int &,const string &,const int &,const double &);
  foo(s,42,"hi"); //实例化foo(const string &,const int &,const char[3] &);
  foo(d,s);       //实例化foo(const double &,const string &);
  foo("hi");      //实例化foo(const char[3] &);
  //使用sizeof...得到模板参数包和函数参数包的参数数目
  template <typename... Args>
  void g(Args... args){
      cout<<sizeof...(Args)<<endl;            //模板参数包的数目
      cout<<sizeof...(args)<<endl;            //函数参数包的数目
  }
  ```

### 1. 编写可变参数函数模板

- 可使用`initializer_list`来定义一个接受可变数目实参的函数，但initializer_list只能容纳相同类型，这样定义的函数只能接受相同类型（或能转为相同类型）的可变数目实参

- 函数参数既不知道数目又不知道类型时，最好使用可变参数模板

- 可变参数函数通常是`递归`的，每一步调用处理参数包中的一个实参，然后用包中剩余实参调用自身。为了终止递归，还需定义一个非可变参数的函数，用于处理参数包中的最后一个实参

- 在可变参数模板的递归设计中，到达终止条件时可同时匹配到可变参数版本和非可变参数版本（终止递归），它们的匹配一样好，但非可变参数版本是更特化的版本，故最后会调用非可变参数的版本，顺利终止递归

- 例子：可变参数模板经常是递归的

  ```c++
  //只处理包中的一个参数t，终止递归
  template <typename T>
  ostream &print(ostream &os,const T &t){
      return os<<t;
  }
  //每次只处理一个参数t，rest容纳剩余的参数。递归调用自身依次处理包中每个参数，当rest...为空时调用非可变参数版本
  template <typename T,typename... Args>
  ostream &print(ostream &os,const T &t,const Args &... rest){
      os<<t<<", ";
      return print(os,rest...);
  }
  print(cout,i,s,42); /* 第一次调用：print(cout,i,s,42)，处理i，rest...是s,42
                       * 第二次调用：print(cout,s,42)，处理s，rest...是42
                       * 第三次调用：print(cout,42)，处理42，rest...为空，调用的是非可变参数版本
                       */
  ```

  

### 2. 包扩展

- 对于一个参数包，除了获取大小外，能对他做的唯一一件事是`扩展`

- 扩展一个包时，需要提供应用于每个扩展元素的`模式`。

- 扩展一个包就是将其分解为单个元素，对每个元素分别应用模式，得到扩展后的列表。形式是在模式右边放省略号`...`来触发扩展

  - 可用`const Args &...`将模板参数包`Args`中的所有类型`T`都扩展为`const T &`再打包
  - 可用`args...`从函数参数包`args`（同时也是模式）中扩展出一个由包中元素组成的、逗号分隔的列表（仅扩展为其构成元素，是平凡扩展）
  - 可用`f(args)...`对函数参数包`args`中的所有元素调用`f`

- 扩展中的模式会独立地应用于包中的每个元素

- 例子：包扩展

  ```c++
  /* 上下文：该函数是16.4.1例子中的递归函数 */
  template <typename T,typename... Args>
  //扩展模板参数包，将Args中的类型T都变成const T &
  ostream &print(ostream &os,const T &t,const Args &... rest){
      os<<t<<", ";
      //扩展函数参数包，模式是rest，扩展的结果是生成一个由包中元素组成的、逗号分隔的列表
      return print(os,rest...);
  }
  ```

- 例子：用函数扩展一个函数参数包

  ```c++
  /* 上下文：print在16.4.1中定义，debug_rep在16.3中定义 */
  //正确写法：用函数扩展一个函数参数包
  template <typename... Args>
  ostream &errorMsg(ostream &os,const Args &... rest){
      return print(os,debug_rep(rest)...);    //使用模式debug_rep(rest)，对rest中的每个参数分别调用debug_rep，得到这些调用结果的列表
  }
  //下面一行代码等价于print(cerr,debug_rep(fcnName),debug_rep(code.num()),debug_rep("other"));
  errorMsg(cerr,fcnName,code.num(),"other");
  //下面是错误写法：
  template <typename... Args>
  ostream &errorMsg(ostream &os,const Args &... rest){
      return print(os,debug_rep(rest...));    //是错误写法，先扩展rest得到列表，再用这个列表调用debug_rep报错
  }
  //错，下面一行代码等价于print(cerr,debug_rep(fcnName,code.num(),"other"));
  errorMsg(cerr,fcnName,code.num(),"other");
  ```

### 3.转发参数包

- C++11中，可组合使用`可变参数模板`和`完美转发`来编写函数，实现将可变数量和类型的参数完美转发

- 标准库容器的`emplace_back`成员用其实参在容器管理的内存空间中直接构造一个元素：

  - emplace_back是可变参数成员模板，因为元素类型的构造函数参数未知
  - emplace_back需要完美转发，因为内部会调用元素类型的构造函数

- 显式实例化并调用函数模板时，若同时存在模板参数包和函数参数包，可同时扩展两者。即，`f<Args>(args)...`等价于`f<Arg1>(arg1),f<Arg2>(arg2),f<Arg3>(arg3),...`

- 例子：emplace_back的实现，同时使用可变参数模板和完美转发

  ```c++
  /* 上下文：StrVec定义于13.5 */
  class StrVec{
  public:
      template <typename... Args>
      void emplace_back(Args &&...);                              //将模板参数包中的类型都扩展为右值引用
      /* 其他成员的定义和13.5相同 */
  };
  template <typename... Args>
  inline void StrVec::emplace_back(Args &&... args){              //完美转发第一：形参定义为万能引用
      chk_n_alloc();
      //同时扩展模板参数包和函数参数包
      //std::forward<Args>(args)...等价于std::forward<Arg1>(arg1),std::forward<Arg2>(arg2),...
      alloc.construct(first_free++,std::forward<Args>(args)...);  //完美转发第二：转发时使用std::forward
  }
  ```

- 可变参数函数通常将它们的参数转发给其他函数，这种函数通常与emplace_back有一样的形式，即将可变参数转发给另一个函数实现功能

- 例子：将可变参数完美转发给另一个函数的通用形式

  ```c++
  template <typename... Args>             //可变参数
  void fun(Args &&... args){              //将每个参数扩展为万能引用
      work(std::forward<Args>(args)...);  //对每个参数使用对应类型的std::forward（同时扩展模板参数包和函数参数包）
  }
  ```



## 五. 模板特例化

- 某些情况下，通用模板的定义可能不适合特定的类型。此时可定义该模板的`特化`版本

  - 对特定类型可能工作并非预期
  - 对特定类型可能可以优化

- 模板的一个`特化`版本是模板的一个独立定义，在其中有一个或多个模板参数被指定为特定的类型/值

- 特化模板实际上是在定义模板参数为某些特定类型/值时的特殊行为

- 特化一个函数模板时，必须为原模板中的每个模板参数都提供实参（即函数模板只能`全特化`）。形式是使用`template <>`表示该模板中的所有模板参数都已被指定

- 定义一个特化版本时，函数参数类型必须与一个先前声明的模板中对应的类型匹配，即接口确实是原模板的一个特例。

- 例子：函数模板特化

  ```c++
  //通用模板
  template <typename T>
  int compare(const T &,const T &);
  //特化模板
  //模板参数列表为空，所有参数都被特化
  template <>
  /* 希望将T特化为const char *，即所指对象是const，而T是指针，此时const T的含义是指针为const，
   * 因此const T &变为const char * const &
   */
  int compare(const char * const &p1,const char * const &p2){
      return strcmp(p1,p2);
  }
  //根据实参选择调用通用版本还是特化版本
  compare("hi","mom");    //两个版本都是精确匹配，选择特化的版本
  ```

- 定义函数模板的特化版本时，本质上是接管了编译器的工作。即，为原模板的一个特殊实例提供了重新定义。

- 函数模板的特化版本，本质上是一个实例，而不是重载。因此特化不影响函数匹配

- 将特殊函数定义为特化版本，和定义为非模板函数，对函数匹配而言是不同的。特化的比非特化的更优，非模板比模板更优

- 为了特化一个模板，原模板的声明必须在作用域中。且在使用模板实例之前，通用版本和特化版本都必须在作用域中，确保所有版本都可见。若特化版本不可见，编译不会报错，但会根据通用版本实例化，这并非预期。

- `最佳实践`：模板及其特化版本应声明在同一个头文件中，所有同名模板的声明应放在前面，然后是这些模板的特化版本

- 类模板也可特化

- 无序容器默认使用`hash<key_type>`来组织元素，为了让自定义的类型也可使用默认的组织方式（而不是构造模板时手动提供哈希），可将hash特化为自定义的类型。此时需要：

  - 两个类型成员：返回类型result_type和参数类型argument_type
  - 重载调用算符
  - 默认构造函数和拷贝赋值算符

- 可使用`namespace`打开命名空间，向其中添加成员，形式是：`namespace std{/* 向其中添加代码 */}`，花括号之间的任何定义都将成为命名空间`std`的一部分

- 哈希函数和`operator==`应该兼容（对相等的对象生成相同哈希值，对不相等的对象尽量生成不同哈希值）。因为无序容器内部会同时使用hash和key_type上的==算符。

- 例子：特化hash

  ```c++
  //用Sales_data类型特化hash<key_type>，并放进命名空间std
  namespace std{
  //类模板全特化，定义模板参数为Sales_data时hash的行为
  template <>
  struct hash<Sales_data>{
      typedef size_t result_type;                     //返回类型
      typedef Sales_data argument_type;               //参数类型
      size_t operator()(const Sales_data &s) const;   //调用算符
  };
  //实现调用算符，即hash值的计算
  //实现方式是对每个数据成员分别使用内置类型哈希，然后结果相互异或
  size_t hash<Sales_data>::operator()(const Sales_data &s) const{
      return hash<string>()(s.bookNo)^
             hash<unsigned>()(s.units_sold)^
             hash<double>()(s.revenue);
  }
  }
  //由于特化hash时使用了Sales_data的private成员，故需声明为友元
  //为了让Sales_data的用户可使用hash的特化版本，应在Sales_data的头文件中定义（不仅仅是声明）该特化版本
  template <typename T> class std::hash;  //声明友元之前需要先在外部声明
  class Sales_data{
      friend class std::hash<Sales_data>; //将Sales_data特化的hash声明为友元
      /* 其他定义和之前一样 */
  };
  //使用Sales_data作为关联容器的key时，默认即可使用特化的hash
  unordered_multiset<Sales_data> SDset;
  ```

- 全特化与偏特化：

  - 函数模板必须`全特化`，即指定所有模板参数
  - 类模板既可全特化也可`偏特化`，即指定部分模板参数，或是参数的部分属性而非全部属性
  - 全特化得到的是实例，使用时不需指定参数
  - 偏特化得到的是模板，使用时仍需指定参数

- 偏特化时，未完全确定类型的模板参数仍放在`<>`中，即偏特化的模板参数列表非空。使用时也需提供模板实参，这些实参与原始模板中的参数按位置对应

- 偏特化版本的模板参数列表是原始模板参数列表的一个子集或是特化版本

- 标准库的remove_reference类型是通过偏特化（指定参数的部分属性）实现的

- 例子：偏特化实现remove_reference

  ```c++
  template <typename T>
  struct remove_reference{        //未特化的版本
      typedef T type;
  };
  template <typename T>
  struct remove_reference<T &>{   //针对左值引用特化
      typedef T type;
  };
  template <typename T>
  struct remove_reference<T &&>{  //针对右值引用特化
      typedef T type;
  };
  //使用remove_reference，结果都是脱去引用
  int i;
  remove_reference<decltype(42)>::type a;             //decltype推出int，非引用，
  remove_reference<decltype(i)>::type b;              //decltype推出int &，使用特化为左值引用的版本
  remove_reference<decltype(std::move(i))>::type c;   //decltype推出int &&，使用特化为右值引用的版本
  ```

- 可以只特化类模板的特定成员而不特化整个模板，特化成员是对类模板指定模板参数

- 使用模板的实例调用成员时，若该实例的模板实参与特化该成员时的参数一致，则调用特化版本的成员

- 例子：特化成员

  ```c++
  template <typename T>
  struct Foo{
      Foo(const T &t=T()):mem(t){}
      //定义通用的成员
      void Bar(){/* 定义通用的版本 */}
      T mem;
  };
  //定义特化的成员
  template <>
  void Foo<int>::Bar(){/* 定义模板实参为int类型的版本 */}
  //使用特化的成员
  Foo<string> fs;
  fs.Bar();       //调用通用版本
  Foo<int> fi;
  fi.Bar();       //调用特化版本（模板实参与特化成员指定的参数相同）
  ```

  















