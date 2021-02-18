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
    while(b1.size()>0){
        cout<<b1.back()<<endl;
        b1.pop_back();
    }
}
```

- 对类对象使用默认版本的拷贝/赋值/销毁操作时，这些操作拷贝/赋值/销毁类的数据成员（包括智能指针）。



### 2.直接管理内存



## 二.动态数组

### 1.new和数组



### 2.allocator类





## 三.使用标准库，文本查询程序









