# 继承与面向对象设计

<!-- GFM-TOC -->

* [41.了解隐式接口与编译期动态](#41了解隐式接口与编译期动态)
* [42.了解typename的双重意义](#42了解typename的双重意义)
* [43. 学习处理模板化基类内的名称](#43-学习处理模板化基类内的名称)
* [44.将与参数无关的代码抽离 templates](#44将与参数无关的代码抽离-templates)
* [45. 运用成员函数模板接受所有兼容类型](#45-运用成员函数模板接受所有兼容类型)
* [46.需要类型转换时请为模板定义非成员函数](#46需要类型转换时请为模板定义非成员函数)
* [47. 请使用traits classes 表现类信息](#47-请使用traits-classes-表现类信息)
* [48.认识 template 元编程](#48认识-template-元编程)

<!-- GFM-TOC -->

---

### 41.了解隐式接口与编译期动态

1. **class和template都支持接口与多态；**
2. **对class而言，接口是显式的，以函数签名为中心。多态则是通过virtual函数发生于运行期；**
3. **对template参数而言，接口是隐式的，奠基于有效表达式。多态则是通过template具现化和函数重载解析发生于编译期。**

从本条款开始，就进入了全书的第七部分：模板与泛型编程。模板与泛型在C++中是非常重要的部分，还记得本书第一章时，把C++视为一个联邦，它由四个州政府组成，其中一个政府就是模板与泛型了。

本条款是一个介绍性质的条款，内容不难，只需要讲清楚两个概念就行了，即什么是隐式接口，什么是编译期多态。

隐式接口是相对于函数签名所代码的显式接口而言的。当我们看到一个函数签名（即函数声明），比如说：`string GetNameByStudentID(int StudentID);`我们就知道这个函数有一个整型的形参，返回值是string。

但隐式接口是由有效表达式组成的，考虑一个模板函数，像下面这样：

```c++
template <class T>
void TemplateFunction(T& w)
{
    if(w.size() > 10){…}
}
```

T可以是int，可以double，也可以是自定义的类型。光看这个函数声明，我们不能确定T具体是什么，但我们知道，要想通过编译，T必须要支持size()这个函数。也就是说，T中一定要有这样的函数接口声明。

```c++
ReturnValue size();
```

当然返回值ReturnValue不一定是int了，只要它能支持operator > (ReturnValue, 10)这样的运算即可。 **这种由表达式推判出来的函数接口，称之为隐式接口。**

简言之，显式接口由函数签名式构成，隐式接口由有效的表达式组成。

下面讨论编译期多态的问题，我们在讨论继承时就已经多次提到“运行时多态”了，它伴随着virtual关键字，本质是一个虚表和虚指针，在类对象构造时，将虚指针指向了特定的虚表，然后运行时就会根据虚表的内容进行函数调用。

**那么“编译期多态”又是什么呢，从字面上来看，它发生在编译阶段，实际上就是template 这个T的替换，它可以被特化为int，或者double，或者用户自定义类型，<font color = red>这一切在编译期就可以决定下来T到底是什么，编译器会自动生成相应的代码（把T换成具体的类型），这就是编译期多态做的事情了，它的本质可以理解成自动生成特化类型版本，T可以被替换成不同的类型，比如同时存在int版本的swap与double版本的swap，形成函数重载。</font>**  简言之，运行时多态是决定“哪一个virtual函数应该被绑定”，而编译期多态决定“哪一个重载函数应该被调用”。



---

### 42.了解typename的双重意义

1. **声明template参数时，前缀关键字class与typename可以互换**
2. **请使用关键字typename标识嵌套从属类型名称；但不得在base class lists或者member initialization list内使用typename**

顾名思义，typename有双重含意。只要你用过template，那么第一重含意一定知道，那就是声明模板的时候，我们既可以这样写：

```c++
template <class T> 
```

也可以这样写

```c++
template <typename T> 
```

这两种写法并没有任何区别，都是标记T可以是符合隐式接口的任何类型，包括系统预定义类型，也包括用户自定义类型。

typename的第二重含意其实不大能遇到，因为这个依赖于编译器，看下面的例子：

```c++
class SampleClass
{
public:
    typedef int MyInt;
// static const int MyInt = 3;
};

int main()
{
    SampleClass::MyInt *b = new int(3); // 有的编译器会报错
}
```

MyInt来源于SampleClass内的一个重定义，MyInt等价于int，所以main函数里面实质上是`int *b = new int (3)`，但有的编译器会报error，这是因为编译器在遇到这句代码上会产生歧义， **因为它可以将MyInt作为SampleClass的一个静态对象来看（可以看程序被注释掉的代码的地方），这样就变成了一个静态对象乘以b了** 。这种“看法”有些不可思议，但在有些编译器上，却是优先将之视为静态变量的，而不是类型。

为了解决这个二义性，在前面加上`typename`，这样就会强制让编译器将之视为一个类型，而不是静态变量了。 **像这种`SampleClass::MyInt`或是书上举的`T::const_iterator`，都是类中定义的名字，这种名字称之为`dependent names`（嵌套从属名称），所有dependent names都潜在具有二义性，为了消除二义性，就在前面加上typename，变成`typename T::const_iterator`，`typename SampleClass:MyInt`，这样就会解决一些看似正确却怎么也编不过的代码问题了。**

使用typename有两个特例

+ 一个是继承的时候，像下面这样：

  ```c++
  class A: public B::NestedClass{}; // 正确
  class A: public typename B::NextedClass(){}; // 错误
  ```

  在继承XXX类时，即使这个类名是dependent names，也不要使用typename，因为编译器这时候显示不会将之翻译成静态成员（被继承类必定是个类型）。

+ 另一个特例是构造函数时，对于构造成员列表，像下面这样：

  ```c++
  A(): B::NestedClass(){} // 正确
  A(): typename B::NestedClass(){} // 错误
  ```

  这时候编译器也不会将之视为静态对象的，因为静态对象是不支持成员初始化列表这样的初始化形式的，所以这时的typename，反而会被编译器认为一个是BUG。





---

### 43. 学习处理模板化基类内的名称

+ **可在derived class template内通过“this->”指涉base class templates内的成员名称，或藉由一个明白写出的“base class资格修饰符”完成。**

背景是这样的，有两个不同的公司，然后想设计一个MessageSender，为这两个公司发送不同的消息，既支持明文发送SendClearText，也支持密文发送SendEncryptedText。一种思路是采用动态绑定的方法，定义一个BasicMessageSender，里面有两个方法，分别是发送明文和密文的虚函数，然后定义它的子类MessageSenderForCompanyA，以及MessageSenderForCompanyB，在这两个子类里面覆盖发送明文和密文的虚函数，从而达到根据不同公司发送不同消息的目的。

但这里我们想换一种思路，使用静态多态的方法来实现，静态多态就是模板的技术了，代码如下：

```c++
class CompanyA
{
public:
    void SendClearText(){}
    void SendEncryptedText(){}
};

class CompanyB
{
public:
    void SendClearText(){}
    void SendEncrypedText(){}
};

template <class T>
class MsgSender
{
public:
    void SendClearText(){}
};

template <class T>
class MsgSenderWithLog: public MsgSender<T>
{
public:
    void SendClearTextWithLog()
    {
        // Logs
        SendClearText(); // 有的编译器会编不过这段代码
    }
};

int main()
{
    MsgSenderWithLog<CompanyA> MsgSender;
    MsgSender.SendClearTextWithLog();
}
```

CompanyA与CompanyB有各自的发送函数，然后有一个模板类MsgSender，这个模板待确定的参数是T，可以是CompanyA或者CompanyB，这样就可以在定义MsgSender时，比如MsgSender或者MsgSender，指定到底调用的哪个公司的发送函数了，这是在编译期就可以确定下来的事情。

但现在有一个新问题，那就是我们希望在执行发送函数之前，还是加上日志比较好，这样我们就继承了MsgSender，定义了一个新类MsgSenderWithLog，在这里定义了一个新的函数SendClearTextWithLog，在这个函数里面调用了父类的SendClearText。

对于这段代码，其实思路还是挺清晰的，但问题是有的编译器会编不过这行代码（VS2008以后的版本的都是可以的，之前的版本没试），为什么？

**这是因为在模板技术中存在全特化的概念，比如C公司，这个公司根本不想发送明文，也就是说它只有SendEncryptedText()接口，没有SendClearText()。** 为了使我们的静态多态仍然可用，我们这样定义只适用于C公司的MsgSender：

```c++
class CompanyC
{
public:
    void SendEncryptedText(){}
};

template <>
class MsgSender<CompanyC>
{
public:
    void SendEncryptedText(){}
};
```

这时候如果去调用：

```c++
MsgSenderWithLog MsgSenderC;
MsgSenderC.SendClearTextWithLog(); //编译器无法通过编译
```

这样编译器会报找到SendClearText()的错。正是因为有的编译器考虑到了全特化模板版本可以与普通版本不同，所以在有继承关系存在时，对直接调用父类的函数给出了不支持的error。但这个error是与编译器相关的，不是必然出现的。

为了让更多的编译器放弃这种全特化的忧虑，书上提供了三种解决方法：

**方法一：**

```c++
void SendClearTextWithLog()
{
    // Logs
    SendClearText(); // 有的编译器会编不过这段代码
}
//改成
void SendClearTextWithLog()
{
    // Logs
    this->SendClearText(); // 这下能编译通过了
}
```

**方法二：**

在子类中声明`using MsgSender<T>::SendClearText;`

编译器报error本质是不进行模板父类域的查找，所以这里using了父类的一个函数名，强制编译器对之进行查找。

**方法三：**

将SendClearText()指明为`MsgSender::SendClearText()`。



---

### 44.将与参数无关的代码抽离 templates

+ Template生成多个classes与多个函数，所以任何template代码都不该与某个造成膨胀的template参数产生相依关系。

+ 因非类型模板参数而造成的代码膨胀，往往可以消除，做法是以函数参数或者class成员变量替换template参数。

+ 因类型而造成的代码膨胀，也可以降低，做法是让带有完全相同二进制表述的具现类型共享实现码。

标题上说“将与参数无关的代码抽离template”，这里的参数既可以指类型，也可以是非类型，我们先来看看非类型的情况。

假定我们要为矩阵写一个类，这个矩阵的行列元素个数相等，是一个方阵，因而我们可以对之求逆运算。因为方阵的元素可以有多种类型，同时方阵的维数（方阵大小）也可以不同，像下面这样，我们使用了模板：

```c++
template <class T, size_t n>
class SquareMatrix
{
public:
    void Invert();
};

int main()
{
    SquareMatrix<int, 10> a;
    SquareMatrix<int, 5> b;
}
```

模板既可以指定类型，也可以指定其他参量，比如这里的size_t，用来表示方阵的维数。程序看起来是没有问题的，编译也是通过的，但从代码优化上看，还是有空间可以做的。 **a和b虽然都是元素为int型的方阵，但因为方阵维数不同，因而生成了两个Invert函数，这两个Invert函数的代码只是在循环个数上不同（取觉于方阵维数n），但算法思想完全是一样的。**

为了防止编译器生成冗余的代码，我们可以将Invert抽离这里，变成一个以n为参数的函数，像下面这样：

```c++
class SquareMatrixBase
{
public:
    SquareMatrixBase(T* p) : DataPointer(p){}
    void Invert(size_t n){}
private:
    T* DataPointer;
};

template <class T, size_t n>
class SquareMatrix: private SquareMatrixBase<T>
{
public:
    SquareMatrix() : SquareMatrixBase(Data)
    {}
    void Invert()
    {
        SquareMatrixBase::Invert(n);
    }
private:
    T Data[n * n];
};
```

这里我们定义了一个BaseSquareMatrix类，这个类可以视为一个工具类，因为SquareMatrix是private继承于它的，我们把算法一致、只与维数n相关的算法函数都放在BaseSquareMatrix中，且将n作为它的Invert()函数的形参，为了使BaseSquareMatrix可以访问数据，从而求逆运算，我们声明了它的成员指针， **这个指针将会在构造时指向子类的数据存储空间。**

子类SquareMatrix没有大变化， **只是在Invert的时候，用了一句代码就搞定了——调用父类的Invert函数，并把维数n传过去。**

**这样在main函数中，针对类型都是int，但矩阵维数不同的情况，Invert中生成的冗余代码非常少（只有一句话），父类也因为模板参数与维数无法，所以也只生成一份。从这个角度来看，将与参数无关的代码抽离template起到了减少代码冗余度的优化作用。**

```c++
template<typename T>      //与尺寸无关的基类  
class SquareMatrixBase{  
    protected:  
        SquareMatrixBase(std::size_t n, T* pMem) : size(n), pData(pMem){}  
        void setDataPtr(T* ptr) { pData = ptr;}     //存储矩阵大小和一个指针指向矩阵内容  
        void invert(std::size_t matrixSize);        //以给定尺寸求逆  
        ...  
    private:  
        std::size_t size;       //矩阵大小  
        T* pData;       //指针，指向矩阵的内容，否则invert()不知道要操作什么类型的数据。有些实现版本也可以在类内部直接分配内存来初始化矩阵内容，也可以通过动态内存分配存储内容  
};  

template<typename T, std::size_t n>  
class SquareMatrix : private SquareMatrixBase<T>{ //只是调用基类中的实现，而非为了表现出“is-a“关系，故使用私有继承  
    private:  
        using SquareMatrixBase<T>::invert;        //避免同名函数的名称遮掩  
        ...  
    public:  
        void invert(){ this->invert(n);}     //内联函数，调用基类版本的invert()，同样this指针也是为了防止同名函数的遮掩  
        ...  
}; 
```

目前只是说将与类型无关的代码进行抽离（造一个父类，把算法相同的部分提取出来，放到父类），对于不同类型，其实也可以通过`void*`来实。比如STL，要想令我们使用`list<int *>`，`list<const int *>`生成的代码冗余度保持很低，这就需要在相应的函数里面，将之链到父类的一个公共实现版本里面，这个版本的形参是`void *`。


---

### 45. 运用成员函数模板接受所有兼容类型

+ **请使用member function templates（成员函数模板）生成“可接受所有兼容类型”的函数；**

+ **如果你声明member templates用于“泛化copy构造”或“泛化assignment操作”，你还是需要声明正常的copy构造函数和copy assignment操作符。**

比如有一个Base类和一个Derived类，像下面这样：

```C++
class BaseClass
{…};

class DerivedClass : public BaseClass
{…};
```

因为是父类与子类的关系，所以可以这样写：

```c++
DerivedClass *d;
BaseClass *b = static_cast< BaseClass *>d; // 用C风格直接是 b = (BaseClass*) d
```

我们可以弄一个简易的Shared型智能指针类，如果直接像下面这样写：

```c++
template <class T>
class SharedPtr
{
private:
    T* Ptr;
    static size_t Count;

public:
    SharedPtr(T* _ptr)
    {
        Count = 1;
        Ptr = _ptr;
        cout << "Constructor Called Same" << endl;
    }
    ~SharedPtr()
    {
        cout << "Destructor Called" << endl;
        if (--Count == 0)
        {
            cout << "Delete Pointer" << endl;
            delete Ptr;
            Ptr = NULL;
        }
    }

    SharedPtr(const SharedPtr<T>& _Smart)
    {
        cout << "Copy Constructor Called Same" << endl;
        Ptr = _Smart.Ptr;
        ++Count;
    }
};
```

那么显示编译器不会允许`SharedPtr pb(pd)`，因为在编译期替换T时，拷贝构造函数明确了接受类型必须是SharedPtr，而由SharedPtr对象至SharedPtr的转换并不存在，所以编译器报错。

为了能使这样的转换合法，我们需要在原来的基础上这样写：

```c++
class SharedPtr
{
private:
    T* Ptr;
    static size_t Count;

public:
    SharedPtr(T* _ptr)
    {
        Count = 1;
        Ptr = _ptr;
        cout << "Constructor Called Same" << endl;
    }

    template <class Other>
    SharedPtr(Other* _ptr)
    {
        Count = 1;
        Ptr = static_cast<T*> (_ptr);
        cout << "Constructor Called Other" << endl;
    }

    ~SharedPtr()
    {
        cout << "Destructor Called" << endl;
        if (--Count == 0)
        {
            cout << "Delete Pointer" << endl;
            delete Ptr;
            Ptr = NULL;
        }
    }

    T* GetPointer()
    {
        return Ptr;
    }

    T* GetPointer() const
    {
        return Ptr;
    }

    SharedPtr(const SharedPtr<T>& _Smart)
    {
        cout << "Copy Constructor Called Same" << endl;
        Ptr = _Smart.Ptr;
        ++Count;
    }
  
    template <class Other>
    SharedPtr(const SharedPtr<Other>& _Smart)
    {
        cout << "Copy Constructor Called Other" << endl;
        Ptr = static_cast<T*>(_Smart.GetPointer());
        ++Count;
    }
};
```

注意代码标注为蓝色的部分（即为泛化部分），这里另外声明了一个模板参数Other，它可以与T相同，也可以不同，也就意味着它可以接受任何可以转化成T的类型了，比如父子类。这里还定义了GetPointer的方法，因为拷贝构建中传入的对象不一定是属于同一个类的，所以不能保证可以访问到类的私有成员。Ptr = static_cast

```c++
template<class Ty>
   class shared_ptr {
public:
    typedef Ty element_type;

    shared_ptr();
    shared_ptr(nullptr_t); 
    shared_ptr(const shared_ptr& sp);
    shared_ptr(shared_ptr&& sp);
    template<class Other>
        explicit shared_ptr(Other * ptr);
    template<class Other, class D>
        shared_ptr(Other * ptr, D dtor);
    template<class D>
        shared_ptr(nullptr_t, D dtor);
    template<class Other, class D, class A>
        shared_ptr(Other *ptr, D dtor, A alloc);
    template<class D, class A>
        shared_ptr(nullptr_t, D dtor, A alloc);
    template<class Other>
        shared_ptr(const shared_ptr<Other>& sp);
    template<class Other>
        shared_ptr(const shared_ptr<Other>&& sp);
    template<class Other>
        explicit shared_ptr(const weak_ptr<Other>& wp);
    template<class Other>
        shared_ptr(auto_ptr<Other>& ap);
    template<class Other, class D>
        shared_ptr(unique_ptr<Other, D>&& up);
    template<class Other>
        shared_ptr(const shared_ptr<Other>& sp, Ty *ptr);
    ~shared_ptr();
    shared_ptr& operator=(const shared_ptr& sp);
    template<class Other> 
        shared_ptr& operator=(const shared_ptr<Other>& sp);
    shared_ptr& operator=(shared_ptr&& sp);
    template<class Other> 
        shared_ptr& operator=(shared_ptr<Other>&& sp);
    template<class Other> 
        shared_ptr& operator=(auto_ptr< Other >&& ap);
    template <class Other, class D> 
        shared_ptr& operator=(const unique_ptr< Other, D>& up) = delete;
    template <class Other, class D>
        shared_ptr& operator=(unique_ptr<Other, D>&& up);
    void swap(shared_ptr& sp);
    void reset();
    template<class Other>
        void reset(Other *ptr);
    template<class Other, class D>
        void reset(Other *ptr, D dtor);
    template<class Other, class D, class A>
        void reset(Other *ptr, D dtor, A alloc);
    Ty *get() const;
    Ty& operator*() const;
    Ty *operator->() const;
    long use_count() const;
    bool unique() const;
    operator bool() const;

    template<class Other>
        bool owner_before(shared_ptr<Other> const& ptr) const;
    template<class Other>
        bool owner_before(weak_ptr<Other> const& ptr) const;
    template<class D, class Ty> 
        D* get_deleter(shared_ptr<Ty> const& ptr);
};
```

这里另外声明了一个模板参数Other，它可以与T相同，也可以不同，也就意味着它可以接受任何可以转化成T的类型了，比如父子类。这里还定义了GetPointer的方法，因为拷贝构建中传入的对象不一定是属于同一个类的，所以不能保证可以访问到类的私有成员。`Ptr = static_cast<T* >(_Smart.GetPointer())` 这句话其实就是转换的实质了，只要任何可以转成`T * `的`Other *` ，都是可以通过编译的，但如果是风马牛不相及的两个类，就不会通过编译。这里有一点要强调一下，我们可以把double转成int（窄化），也可以把int转成double（宽化），但注意double*与int*之间是不能相互转的，如果这样写int a = (int) (new double(2))，是不能通过编译的，可以用static_cast转换的类要有继承关系。代码还有赋值运算符也需要提供一个非泛化的版本和泛化的版本，这里简略没有写出。

这里还有一个需要注意的地方，在class类声明泛化copy构造函数（member template），并不会阻止编译器生成它们自己的copy构造函数（non-template），换言之，如果程序中只写了泛化的copy构造函数，那么编译器还是会自动生成一个非泛化的版本出来，如果不想要这个缺省版本，那一定不能偷懒，要两个版本的copy构造函数都要写。




---

### 46.需要类型转换时请为模板定义非成员函数



---

### 47. 请使用traits classes 表现类信息



---

### 48.认识 template 元编程



---

