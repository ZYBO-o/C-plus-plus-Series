## 一.函数基础

### 1.局部对象

+ 把存在于执行期间的对象称为自动对象。当块的执行结束之后，块中创建的自动对象的值就变成未定义的了。
+ 局部静态对象在程序的执行路径第一次经过对象定义语句时初始化，并且直到程序终止才被销毁，在此期间即使对象所在的函数执行结束也不会对它有影响。

```c++
void cout_calls()
{
    int count = 0;
    static int static_int = 0;
    cout<< "count = " << ++count << endl;
    cout<< "static_int = " << ++static_int << endl;
    cout << endl;
}

int main() {
    for (int i = 0; i < 10; ++i) {
        cout_calls();
     return 0;
}
```



## 二.参数传递

形参的类型决定了形参和实参交互的方式。如果形参是引用类型，则会绑定在对应的实参上；否则将实参的值拷贝后赋给形参。

+ 形参是引用类型时，对应的实参被**引用传递**，即函数被传**引用调用**。
+ 实参的值被拷贝给形参时，实参被**值传递**，即函数被**传值调用**。

### 1.传值参数

+ **指针形参**

  + 当执行指针拷贝操作时，拷贝的是指针的值。拷贝后，两个指针是不同的指针。

    ```c++
    void changeValue(int *p_value)
    {
        int i = 3;
        cout<< "In function, p_value = " << *p_value <<" and Pp_value = " << p_value <<endl;
        *p_value = 4;
        cout<< "In function, After Change1 ; p_value = " << *p_value <<" and Pp_value = " << p_value <<endl;
        p_value = &i;
        cout<< "In function, After Change2 ; p_value = " << *p_value <<" and Pp_value = " << p_value <<endl;
    }
    
    int main(){
      int a = 1;
      cout<< "a = " << a <<" and p_a = " << &a <<endl;
      changeValue(&a);
      cout<< "After function, a = " << a <<" and p_a = " << &a <<endl;
      return 0;
    }
    
    /*
    *输出：
    *	a = 1 and p_a = 0x7ffee5de0a78
    *	In function, p_value = 1 and Pp_value = 0x7ffee5de0a78
    *	In function, After Change1 ; p_value = 4 and Pp_value = 0x7ffee5de0a78
    *	In function, After Change2 ; p_value = 3 and Pp_value = 0x7ffee5de0a14
    *	After function, a = 4 and p_a = 0x7ffee5de0a78
    /
    ```

  + 在C++中，建议使用引用类型的形参代替指针。

<br/>

### 2.传引用参数

+ **通过使用引用形参，允许函数改变一个或多个实参的值。**

  ```c++
  void changeValue(int &p_value)
  {
      p_value = 3;
  }
  
  
  int main() {
  
      int a = 1;
      changeValue(a);
    	//输出为3
      cout<< "After function, a = " << a  <<endl;
  		return 0;
  }
  ```

+ **当某种类型不支持拷贝操作时，函数只能通过引用形参访问该对象。**

+ **<font color = red>当所传参数需要占很大的内存时，应该避免拷贝，把形参定义为对常量的引用。</font>**

+ **函数只能返回一个值，<font color = red>当需要函数同时返回多个值时，引用形参提供了有效途径。</font>**

  ```c++
  string::size_type find_char(const string &s, char c, string::size_type &occurs)
  {
      auto ret = s.size();
      occurs = 0;
      for (decltype(ret) i = 0; i != s.size(); i++){
          if (s[i] == c){
              if (ret == s.size())
                  ret = i;
              ++occurs;
          }
      }
      return ret;
  }
  
  int main() {
      string s = "abccc";
      string::size_type ctr;
      auto index = find_char(s, 'c', ctr);
      cout<<index<<endl;
      cout<<ctr<<endl;
      return 0;
  }
  //输出为：
  // 2
  // 3
  ```

<br/>

### 3.const形参与实参

+ 用实参初始化形参时，会忽略顶层const。

  ```c++
  void fcn(const int i )
  void fcn(int i )
  //错误，重复定义了func(int)
  ```

+ **尽量使用常量引用。**

  + 这样既能减少开销
  + 又能给别人提示，这是一个常量。
  + **<font color = red>普通引用无法传入常量与字面值，限制了函数所能接受的实参类型。</font>**

</br>

### 4.数组实参

+ **C++允许将变量定义成数组的引用，同样，形参也可以是数组的引用。**

  ```c++
  void print(int (&array)[10])
  {
      for (auto elem : array)
          cout<< elem << endl;
  }
  
  int main() {
      int array[10] = {1,2,3,4,45,56,6};
      print(array);
      return 0;
  }
  ```

  + 其中`&array`两端的括号必不可少

    ```c++
    f(int &array[10])//错误：将array声明成引用的数组
    f(int (&array)[10])//正确，array是具有10个整数的整型数组的引用
    int (*array)[10];//这是指向含有10个整数的数组的指针
    //以上都要区分好
    ```

<br/>

### 5.含有可变形参的函数

+ **对于处理不同数量实参的函数：**

  + 实参类型相同，可以传递一个名为`initializer_list`的标准库类型；
  + 实参类型不同，编写可变参数模板。

+ **initializer_list 形参(C++ 11特性)：**

  ```c++
  double sum(initializer_list<double> il){
      double sum = 0;
      for (auto tmp : il) sum += tmp;
      return sum;
  }
  
  double average(const initializer_list<double> &ril){
      if (ril.size() > 0){
          double sum = 0;
          for (auto tmp : ril) sum += tmp;
          return sum / ril.size();
      }
      return 0.0;
  }
  
  
  int main(){
      cout << "sum:" << sum({ 2, 3, 4 }) << ", ave:" << average({ 2.0, 3, 4 }) << endl;
      initializer_list<double> dl = { 1.5, 2.5, 3.5, 4.5, 5.5 };
      cout << "sum:" << sum(dl) << ", ave:" << average(dl) << endl;
      dl = { 1, 2, 3, 4, 5, 6 };
      cout << "sum:" << sum(dl) << ", ave:" << average(dl) << endl;
      system("pause");
  }
  ```

+ **initializer_list 对象的元素永远都是常量值**

<br/>

## 三.返回类型和return类型

### 1.有返回值函数

+ **不要返回局部对象的指针或者引用**

  + 当函数终止意味着局部变量的引用将指向不再有效的内存区域。

    ```c++
    int *change(int *p){
        int temp = 2;
        *p = temp;
        return p;
    }
    
    int *change2(){
        int temp = 2;
        return &temp;
    }
    
    int *change3(){
      	//加上static就不一样啦
        static int temp = 2;
        return &temp;
    }
    
    
    int main(){
    
        int a = 1;
        int *p = &a;
        cout<< "*p = " << *p << ", p = " << p << endl;
      
        p = change(p);
        cout<< "After change ; *p = " << *p << ", p = " << p << endl;
    
        p = change2();
        cout<< "After change2 ; *p = " << *p << ", p = " << p << endl;
    
        p = change3();
        cout<< "After change3 ; *p = " << *p << ", p = " << p << endl;
    
        return 0;
    }
    /*输出：
    * *p = 1, p = 0x7ffee9411a98
    * After change ; *p = 2, p = 0x7ffee9411a98
    * After change2 ; *p = 32767, p = 0x7ffee9411a5c
    * After change3 ; *p = 2, p = 0x1067f10c8
    */
    ```

+ **返回左值**

  ```c++
  char &get_val(string &str, string::size_type ix)
  {
      return str[ix];
  }
  
  int main(){
  
      string str{"a value"};
      get_val(str,0) = 'A';
      cout<< str <<endl;
      return 0;
  }
  ```

  + 返回值是引用，因此调用是个左值，和其他左值一样它也能出现在赋值运算符的左侧。

+ **列表初始化返回值(C++ 11特性)**

  函数可以返回花括号包围的值的列表，此处的列表也能对表示函数返回的临时变量进行初始化。

  ```c++
  vector<string > Process(int value)
  {
      if(value < 2)
          return {"this", "string", "is less than", "2"};
      else
          return {"this string is less", "than 3"};
  }
  
  int main(){
      vector<string> str1 = Process(1);
      for (decltype(str1.size()) i = 0; i != str1.size(); ++i)
          cout<< str1[i] <<endl;
  
      cout<<endl;
  
      vector<string> str2 = Process(3);
      for (decltype(str2.size()) i = 0; i != str2.size(); ++i)
          cout<< str2[i] <<endl;
    	return 0;
  }
  //输出：
  // this
  // string
  // is less than
  // 2
  // 
  // this string is less
  // than 3
  ```

  + 如果函数返回类型是内置类型，则花括号包围的列表最多包含一个值，而且该值所占空间不应该大于目标类型的空间。

<br/>

## 2.返回数组指针

+ **声明一个返回数组指针的函数：**

  + 返回数组指针的函数形式：`Type (*function (parameter_list)) [dimension]`，如：`int (*func(int i))[10]`

  + 从以下顺序逐层理解

    ```c++
    (*func(int i))//意味着我们可以对函数调用的结果执行解引用操作
    (*func(int i))[10]//表示解引用func的掉用那个将得到一个大小是10的数组
    int (*func(int i))[10]//表示数组中的元素是int了类型
      
    int arr[10];//arr是一个含有10个整数的数组
    int *p1[10];//p1是一个含有10个指针的数组
    int (*p2)[10] = &arr;//p2是一个指针，指向含有10个整数的数组。
    ```

    ```c++
    int *func(int i)
    {
        int temp = i + 2;
        int *p = &temp;
        return p;
    }
    int main(){
    
        int value = 4;
        int *p = func(value);
        cout<< *p <<endl;
    }
    //输出6
    ```

    ```c++
    int array[10] = {1,2,3,4};
    
    int (*func(int i ))[10]
    {
      	//加上引用和全局变量符号
        for (auto &elem : ::array) {
            elem = elem + i;
        }
        return &::array;
    }
    int main(){
        int (*array)[10] = func(4);
      	//加上解引用符号
        for( auto elem : *array)
            cout<< elem << " ";
    }
    //输出：5 6 7 8 4 4 4 4 4 4
    ```

+ C++11新标准规定可以使用**尾置返回类型**，任何函数都可以这么使用，只是对复杂的函数最为有效，比如返回的类型是数组的指针或者数组的引用。为了表示函数真正的返回类型跟在形参列表之后，把应该出现返回类型的地方放置一个auto。

  ```c++
  auto func(int i) -> int (*)
  {
      int temp = i + 2;
      int *p = &temp;
      return p;
  }
  int main(){
  
      int value = 4;
      int *p = func(value);
      cout<< *p <<endl;
  }
  ```

  ```c++
  int array[10] = {1,2,3,4};
  
  //修改函数类型，和形参类型设置
  auto func(int i ) -> int(*)[10]
  {
      for (auto &elem : ::array) {
          elem = elem + i;
      }
      return &::array;
  }
  
  int main(){
      int (*array)[10] = func(4);
      for( auto elem : *array)
          cout<< elem << " ";
  }
  ```

<br/>

## 四.函数重载

+ **重载和const形参**
  + 顶层const不影响传入函数的对象。一个拥有顶层const的形参无法和一个米有顶层const的形参区分开来。
  + 如果形参是引用或指针时，通过区分其指向的是常量对象还是非常量对象可以实现函数重载。

+ **const_cast与重载**

  + const_cast在重载场景中很有效果。

+ **调用重载的函数**

  + 当两个重载函数参数数量相同且参数类型可以互相转换时，选择函数可能比较困难。

    ```c++
    void print(int i)
    {
        cout<< i << endl;
    }
    void print(double i)
    {
        cout << i << endl;
    }
    
    int main(){
        print(1);
        print(1.3);
        return 0;
    }
    ```

    找最佳匹配。

<br/>

## 五.特殊用途语言特性

### 1.默认参数

+ **一旦形参被赋予了默认值，则后面的所有形参都必须有默认值。**默认实参负责填补函数调用缺少的尾部实参。

+ **默认实参声明：**

  + **<font color = red>C++规定默认参数必须从函数参数的右边向左边使用：</font>**

    正确声明：

    ```c++
    void fun1(int a, int b=10);
    void fun2(int a, int b=10, int c=20);
    ```

    错误声明：

    ```c++
    void fun3(int a=5, int b, int c);
    void fun4(int a, int b=5, int c);
    ```

  + **<font color = red>默认参数不能在声明和定义中同时出现：</font>**

    错误使用示范：

    ```c++
    //声明：
    void fun1(int a=10);
    
    //定义：
    void fun1(int a=10){......}
    ```

    正确使用示范：

    ```c++
    //声明：
    void fun1(int a=10);
    
    //定义：
    void fun1(int a){......}
    ```

+ **默认实参初始值：**

  + 如果函数默认实参是全局变量，同时在局部作用域中重新赋值了和全局变量名一样的变量后，函数默认实参会被隐藏掉。

    ```c++
    using sz = string::size_type;
    
    sz a = 1; sz b = 1; char c = 'a';
    
    void screen(sz = a, sz = b, char = c);
    
    void screen(sz a, sz b, char c)
    {
        cout << "sz1 = " << a << "; sz2 = " << b << "; c = " << c << endl;
    }
    
    int main(){
        a = 2;
        b = 2;
        char c = 'b';
        screen();
    }
    ```

<br/>

### 2.内联函数与constexpr函数

+ **内联函数：**
  + 内联函数可以避免函数调用的开销(在调用点上"内联地"展开)。
  + 内联机制用于规模小，流程直接，频繁调用的函数。
+ **constexpr函数：**
  + constexpr函数是指用于常量表达式的函数。函数的**返回值以及所有形参类型都得是字面值类型**，而且函数体中必须**有且只有一条return语句**。
  + constexpr函数会被隐式地定义为内联函数。
+ 内联函数和constexpr函数可以在程序中多次定义，它们通常定义在头文件中。

<br/>

## 六.函数匹配

+ 第一步是选定本次调用对应的重载函数集，集合中的函数称为**候选函数**。

  + 候选函数的特征：
    + 与被调用的函数同名
    + 声明在调用点可见

+ 第二步是考察本次调用提供的实参，然后从候选函数中选出能被这组实参调用的函数，这些新选的函数称为**可行函数**。

  + 可行函数的特征：

    + 实参数量相同

    + 实参类型与形参类型相同，或者能转换为形参的类型

      > 函数含有默认实参时，调用函数传入的实参数量可能少于它实际使用的实参数量。

+ 第三步是从可行函数中选择与本次调用最匹配的函数。选择的基本思想是：

  + 实参类型与形参类型越接近，匹配得越好。

    ```c++
    void f(int);
    void f(short);
    f('a');
    //char直接提升为int类型。
    ```

```c++
void fun1(int a, int b=10)
{
    cout << a << " " << b << endl;
}
void fun1(int a, int b=10, int c=20)
{
    cout << a << " " << b << c << endl;
}

int main(){
    //有二义性，报错
    fun1(10);
  	//这个也有二义性，会报错
   	fun1(10,12);
    fun1(10,12,13);
}

```

<br/>

## 七.函数指针

+ 想要声明一个口可以指向函数的指针，只需要把指针替换函数名即可。

  ```c++
  bool lengthCompare(const string&, const string&)；
  bool (*pf)(const string&, const string&)；
  ```

  > *pf两端的括号不能少，不然pf就是一个返回值是bool指针的函数。decltype作为返回函数类型时可以不加括号。

+ 使用函数指针的时候，可以直接使用指向函数的指针调用该函数，无需提前解引用指针。

  ```c++
  bool b1 = pf("hello","goodbye");
  bool b2 = (*pf)("hello","goodbye");
  bool b3 = lengthCompare("hello","goodbye");
  //三者等价
  ```

+ 函数指针作为形参时，可以直接把函数作为实参使用，会自动转换为指针(只有作为实参时会自动转换)。

+ 返回指向函数的指针：

  + 和函数类型的形参不一样，返回类型不会自动的转换为指针。

    ```c++
    using F = int(int *, int);
    using PF = int(*)(int *, int);
    
    PF f1(int);//PF是指向函数的指针，f1返回指向函数的指针
    F f2(int);//错误：F是函数类型，f2不能返回函数类型
    F *f2(int);//正确：显示地指定返回类型是指向函数的指针
    //也可以以下面的形式进行声明
    int (*f1(int)) (int *, int);
    auto f1(int) -> int (*)(int *, int);
    //int (*)(int *, int)是返回的函数类型与形参
    ```

+ 将auto和decltype用于函数指针类型

  ```c++
  string ::size_type  sumLength(const string&, const string&);
  string ::size_type  largerLength(const string&, const string&);
  //根据形参的取值，getFuc函数返回指向sumLength或者largerLength的指针
  decltype(sumLength) *getFuc(const string&);
  ```

  + **decltype作用于某函数时，返回的是函数类型而不是指针类型。**

