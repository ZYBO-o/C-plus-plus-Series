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





















