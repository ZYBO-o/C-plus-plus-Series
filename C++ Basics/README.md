<!-- GFM-TOC -->

* C++基础

  * [一、变量与基本类型](#一变量和基本类型)

  * [二、字符串,向量和数组](#二字符串向量和数组)

  * [三、表达式](#三表达式)

  * [四、语句](#四语句)

    <!-- GFM-TOC -->


# 一.变量和基本类型

## (一).基本内置类型

> C++基本数据类型分为：
>
> + 算数类型
>   + 字符，整型数，布尔值和浮点数。
> + 空类型
>   + 不对应具体的值

### 1.算数类型

算数类型分为两类：

+ 整型数(包括字符和布尔型)
+ 浮点型

| 类型        | 含义           | 最小尺寸     |
| ----------- | -------------- | ------------ |
| bool        | 布尔类型       | 未定义       |
| char        | 字符           | 8位          |
| wchar_t     | 宽字符         | 16位         |
| char16_t    | Unicode 字符   | 16位         |
| char32_t    | Unicode 字符   | 32位         |
| short       | 短整型         | 16位         |
| int         | 整型           | 16位         |
| long        | 长整型         | 32位         |
| long long   | 长整型         | 64位         |
| float       | 单精度浮点型   | 6位有效数字  |
| double      | 双精度浮点型   | 10位有效数字 |
| long double | 扩展精度浮点型 | 10位有效数字 |

+ 可寻址的最小内存块称为`字节`
+ 存储的基本单元称为`字`，大多数字由4或8个字节构成
+ int，long，long long (**C++ 11特性**)，short都属于整型，区分是C++标准规定的尺寸的最小值(类型在内存中所占的比特数)不同。其中short是短整型，占16位；int是整型，占16位；long和 long long 是长整型，分别占32位和64位。
+ float和double分别是单精度浮点数和双精度浮点数。float以32比特表示，具有7个有效位；double以32位表示，具有16个有效位；long double具有3或4个字表示，但是它的计算代价相对更大。

### 2.类型转换

+ 赋给带符号类型一个超过它范围的值时，结果是未定义的。
+ 切勿混**用带符号数**与**无符号数类型**，当带符号数取值为负数时会出现异常结果，因为带符号数会先自动转换成无符号数。

### 3.字面值常量

+ 可以像使用普通字符那样使用C++语言定义的转义序列。
  + `std::cout << "Hi \x4dO\155!\n;" ` //输出为Hi MOm!
  + `std::cout << '155' << '\n';`//输出m
  + `std::cout << "\1234";`//表示两个字符，八进制数123与字符4 ，输出为S4
  + `std::cout << "\x1234";`//会报错，因为这表示一个16位的字符，该字符由这4个16进制所对应的比特数唯一确定。而大多数机器的char类型只占8位，所以会报错。

---

## (二).变量

变量提供一个具名的，可供程序操作的存储空间。每个变量都有其数据类型，而数据类型决定着变量 **所占空间大小** ，**布局方式**， **该空间能存储的值的范围**， 以及**变量能参与的运算**。

### 1.变量定义

>  对于C++程序员来说，变量和对象一般可以互换使用。

+ **对于初始化：**

  + 对于C++而言，初始化和赋值是两个完全不同的操作。

    > 初始化：创建变量时赋予其一个初始值。
    >
    > 赋值：把对象的当前值擦除，而以一个新值代替。

+ **C 11特性：列表初始化。**

  + 使用列表初始化且初始值存在丢失信息的风险，则编译器报错。

    > `long double Id = 3.1215926; int a{Id}`//报错

### 2.变量声明和定义的关系

+ C++为了支持分离式编译，C++将声明与定义区分开来。

  + 声明使得名字为程序所知，一个文件若想要别处定义的名字，则必须包含对那个名字的声明。
  + 定义负责创建与名字关联的实体。

+ 若想声明一个变量而非定义它，就在变量名前添加关键字`extern`，而且不要显示地初始化变量。任何包含了初始化的声明即成了定义。

  > `extern double pi = 3.1415`//定义

+ 如果在多个文件中使用同一个变量，就必须将声明与定义分离。

### 3.标识符

+ 用户自定义的标识符中不能连续出现两个下划线，也不能以下划线连接大写字母开头。
+ 定义在函数体外的标识符不能以下划线开头。

> 但是自己实验发现并不报错。。

### 4.名字的作用域

+ 在局部作用域中使用全局作用域，利用`::`符号。

---

## (三).复合类型

### 1.引用

+ 引用就是给对象起另一个名字。**引用必须被初始化。**
+ 定义引用时，程序会把引用和它的初值绑定，而不是简单的拷贝。
+ 引用必须是指向一个实际存在的对象，而不是字面值常量。

### 2.指针

+ 指针与引用不同点在于：

  + 指针本身是对象，允许对指针赋值和拷贝，而且在指针的生命周期内它可以先后指向几个不同的对象
  + 指针无需在定义时赋初值

+ 定义的指针类型必须和指向的对象的类型一致

  `double dval; int *p = &dval; `//错误

+ 利用指针访问对象是需要注意：

  ```c
  int value1 = 1;
  int value2= 2;
  int *pv = &value1;
      
  std::cout << *pv  << std::endl;//输出1
  
  *pv = 3;
  std::cout << "value1 = " << value1  << std::endl;//输出 value11 = 3
  
  pv = &value2;
  std::cout << "value1 = " << value1  << std::endl;//输出 value11 = 3
  //*pv = 3;相当于改变了value1的值
  //pv = &value2;是改变了指针pv的指向
  ```

+ 定义空指针的方式有以下方法：

  ```c
  int *p1 = nullptr;//等价于int *p1 = 0
  int *p2 = 0;//直接将p2初始化为字面常量
  int *p3 = NULL;
  
  /*
  *int *p;
  * *p = 0;//只是把值为赋为0
  *  p = 0;//这是赋为空指针
  */
  ```

+ 建议初始化所有指针，尽量等定义了对象之后在定义指向它的指针，若真的不清楚指针指向何处，可以先初始化为nullptr或0；

+ 指针的改变

  ```c
  int i = - 42, a = 2;
  int * p3 = &i;
  
  //p3的值发生了改变
  p3 = &a;
  
  //p3的值没有改变，*p3即a的值发生了改变
  *p3 = 4;
  ```

+ `void*` 是一种特殊的指针类型，可用于存放任何对象的地址。

### 3.复合类型的声明

+ 指向指针的引用

  ```c
  int i = 4;
  int *p;
  int *&r = p;//从右往左阅读r的定义，离变量名最近的符号对变量的类型有直接的影响，因此r是一个引用。
  r = &i;//r引用了指针，所以r赋值&i就是令p指向i
  *r= 0;//引用r得到i，也就是p指向的对象，将i的值修改为0
  ```

---

## (四).const限定符

> 有点绕，需要多些代码实践

+ const对象一旦创建后就不能改变，所以需要初始化，编译时初始化和运行时初始化都可以。
+ 若利用一个对象去初始化另一个对象，则它们是不是const类型都无所谓。
+ 若想在多个文件之间共享const对象，必须在变量的定义之前添加extern关键字。

### 1.const的引用

+ 对常量进行引用时，定义的引用类型必须一致。

  ```c
  double i = 3.1243;
  const int k = i;
  int &j = k;//试图用一个非常量引用指向一个常量对象
  ```

+ **临时量的引入：**

  ```c
  double i = 3.214;
  const int &j = i;//j的值为3
  ```

  上述程序编译器修改为：

  ```c
  double i = 3.214;
  const int temp = i;
  const int &j = temp;
  ```

  其中temp就是编译器需要空间来暂存表达式的求值结果时临时创建的一个未命名对象。

### 2.指针和const

+ 上面提过指针的类型必须和其所指向的对象同一个类型，但是有两个例外，第一个就是**允许令一个<font color = red>指向常量的指针</font>指向一个<font color = red>非常量对象</font>。**

  ```c
  double i = 3.214;
  double k  = 4.3;
  
  //可以让p指向i，但是不能通用p改变i的值。即*p = k是错的。
  const double *p = &i;
  
  //但是可以修改p的指向。
  p = &k;
  
  //但是可以自己修改自己的值。
  i = 4.5;
  ```

  > `const double *p = &i;`对于现在p而言，是指向常量的指针。**所谓指向常量的指针仅仅要求不能通过该指针修改对象的值，但没有规定那个对象的值不能用过其他途径改变。**
  >
  > + 所谓指向常量的指针或者引用，只是指针和引用的自以为是，自作多情，它们觉得自己指向了常量，所以自觉的不去改变所指向对象的值。

+ **常量指针**：*放在const关键字之前说明指针是一个常量，**必须初始化**，而且**不变的是指针本身的值，而不是指向的值**。

  ```c
  int errNum = 0;
  //pErr一直指向errNum.
  int *const pErr = &errNum;
  
  const double pi = 3.1415;
  //pIp是一个指向常量对象的常量指针
  const double *const pIp = &pi;
  ```

+ **顶层const**

  + 顶层const表示：指针本身是一个常量，它可以任意的对象是常量。
  + 底层const表示：指针所指向的对象是一个常量，但是指针本身可以修改。

  ```c
  int i = 0;
  //顶层const
  int *const p1 = &i;
  //不能改变ci的值，顶层const
  const int ci = 42;
  //不能改变*p2的值，底层const
  const int *p2 = &ci;
  //靠右的const是顶层const,靠左的const是底层const 
  const int *const p3 = p2;
  //用于声明引用的const都是底层const
  const int &r = ci;
  ```

### 3.constexpr表达式

+ **常量表达式：**值不会改变且在编译过程中就能得到计算结果的表达式，运行时得到结果不算。

+ **constexpr变量：**

  C++11 规定允许将变量声明为constexpr类型以便于编译器来验证变量值是否是一个常量表达式。

  ```c
  constexpr int mf = 20;
  
  //当size函数是一个constexpr函数时才是一条正确的声明语句。
  constexpr int sz = size();
  ```

+ constexpr指针的初始值必须是nullptr或者0，或者是存储于某个固定地址中的对象。

  + 函数内定义的变量一般来说并非存放在固定地址中。
  + 函数体之外的对象其地址固定不变，可以用来初始化constexpr指针。

+ **指针与constexpr**

  在constexpr声明中定义了一个指针，限定符constexpr只对指针有效，对指针所指向的对象无效。

  ```c
  //p是一个指向整型常量的指针
  const int *p = nullptr;
  //q是一个指向整数的常量指针
  constexpr int *q = nullptr;
  ```

  p是一个指向常量的指针，而q是一个常量指针。

---

## (五).处理类型

### 1.类型别名

+ C++11 中新规定了一种类型声明来定义类型的别名：

  ```c
  //使用myint来别名int
  using myint = int;
  ```

+ **特别注意指针类别名**

  ```c
  typedef char *pstring;
  //cstr是指向char的常量指针
  const ptring cstr = 0;
  ```

  千万不要把上句错误理解成：

  ```c
  const char *cstr = 0;
  ```

  这种理解是错误的。声明语句中用到pstring时，其基本类型是指针。前者是一个指向char类型的常量指针，后者是声明了一个指向const char的指针。

### 2.auto类型说明符

+ C++11引入auto类型说明符，让编译器来分析表达式所属的类型。而且auto定义的变量必须有初始值。而且一条语句只能有一种基本数据类型。

+ auto会忽略顶层const，保留底层const，但是在引用时，顶层const会保留

  ```c
  int i = 0;
  const int ci = i, &ci = i;
  auto b = ci;//b是一个整数，忽略顶层
  auto c = &i;//c是一个整型指针
  auto d = &ci;//d是一个指向整型常量的指针(对常量对象取地址是一种底层const)
  
  auto &g = ci;//g是一个整型常量引用
  auto &h = 42;//错误
  const auto &j = 42;//***正确，可以为常量引用绑定字面值。***
  ```

### 3.decltype类型指示符

+ C++11新标准引入decltype来选择并返回操作数的数据类型。
+ 引用基本都是作为其指向对象的同义词出现，只有在decltype是意外。

```c
int i = 1 , *p = &i;
int j = 2;

//此时赋给的是int
decltype(*p + 2) c = j;

//此时赋给的是int &，是一个引用，必须初始化
decltype(*p) e = j;
j = 3;

//此时赋给的是指针类型，不需要初始化
decltype(p) d = &j;
```

+ decltype加上括号需要看表达式的结果类型，双层括号及其以上的结果永远是引用。

+ decltype和auto都可以用来推断类型，但是二者有几处明显的差异：
  + auto忽略顶层const，decltype保留顶层const；
  + 对引用操作，auto推断出原有类型，decltype推断出引用；
  + 对解引用操作，auto推断出原有类型，decltype推断出引用；
  + auto推断时会实际执行，decltype不会执行，只做分析。总之在使用中过程中和const、引用和指针结合时需要特别小心。

---

# 二.字符串,向量和数组

## (一).标准库类型string

### 1.定义和初始化string类型

+ 初始化方式主要有：

<div align="center">  
  <img src="https://github.com/ZYBO-o/C-Series/blob/main/images/1.png"  width="600"/> 
</div>


> 其中主要分为：**拷贝初始化**(用等号初始化)和**直接初始化**。

### 2.string对象上的操作

string的操作主要有：

<div align="center">  
  <img src="https://github.com/ZYBO-o/C-Series/blob/main/images/2.png"  width="600"/> 
</div>


+ **IO操作符进行读写string对象时需要注意：**

  + 在执行读操作时，string类型会自动忽略开头的空白(即空格符，换行符，制表符等)并**从第一个真正的字符开始读起，直到遇见下一个空白为止。**

+ **可以使用getline读取一整行**

  + 用getline读取时，会保留输入时的空白符，直到遇到换行符(会被读入)。但是出发getline函数返回的换行符最后会被丢弃，得到string对象中并不包含那个换行符。

+ **size函数返回类型**

  + size函数返回的是 `string::size` 类型的值，它是一个无符号数类型。
  + 表达式中已经有了size函数就不要用int了，这样可以避免混用有符号数与无符号数带来的问题。

+ **string对象相加**

  + 当把string对象和字符字面值以及字符串字面值混在一一条语句中使用时，必须确保每个加法运算符的两侧的运算对象至少有一个是string 。

    ```c++
    string s1 = "!";
    string s2 = "hello " + "," + "world" + s1;//错误，最左边相加时没有string类型。
    ```

### 3.处理string对象中的字符

+ **处理字符时，C++11标准提出了范围for语句：**

  ```C++
  for (declaration : expression)
  	statement
  ```

  + 输出每一个字符：

    ```c++
    string str("some string");
    for(auto c : str)
      cout<< c <<endl;
    ```

+ **改变字符串中的字符可以考虑使用引用：**

  ```c++
  string str("Hello world!");
  for (auto &c : s)
    c = toupper(c);
  cout<< s <<endl;
  ```

+ **只处理一部分字符串的方法：**

  + 使用迭代器

  + 使用下标

    + 下标运算符`([])`接收的参数是`string::size_type`类型。

    + 使用下标进行迭代的例子。

      ```c++ 
       string s1 = "honey lay";
          
          for (decltype(s1.size()) index = 0; index != s1.size() && !isspace((s1[index])); index++   )
              s1[index] = toupper(s1[index] );
      		//输出结果为HONEY lay
          cout<<s1<<endl;
      
          for (decltype(s1.size()) index = 0; index != s1.size() ; index++   )
          {
              if (isspace((s1[index])))
                  continue;
              else
                  s1[index] = toupper(s1[index] );
          }
      	//输出结果为HONEY LAY
          cout<<s1<<endl;
      ```

---

## (二).标准库类型vector

### 1.定义和初始化vector对象

+ **定义vector对象的常用方法：**

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-Series/blob/main/images/3.png"  width="600"/> 
  </div>


  + C++11标准规定可以使用花括号初始化vector对象。但是不能在圆括号中进行列表初始化。

    ```c++
    vector<int> array1 = {1,2,3,4};//正确
    vector<int> array2 = (1,2,3,4);//错误
    ```

+ **圆括号与花括号初始化的区别：**

  ```C++
  vector<int> array1(10);//初始化了10个元素，每个值都为0
  vector<int> array2{10};//初始化一个元素，为10
  vector<int> array3(10, 1);//初始化了10个元素，每个值都为1
  vector<int> array4{10, 1};//初始化两个元素，为10，1
  //对于string类型
  vector<string> str1{"hello"};//初始化只有一个元素
  vector<string> str2("hello");//初始化错误
  vector<string> str3{10}; //初始化10个默认初始化元素
  vector<string> str4{10, "hello"};//初始化10个元素，为"hello"
  ```

### 2.vector的一些操作

+ **vector对象中的一些操作：**

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-Series/blob/main/images/4.png"  width="600"/> 
  </div>


  + 如果循环体内包含有向vector对象添加元素的语句，咋不能使用范围for循环。
  + 如果定义时`vector<int>`，则它的size函数类型是`vector<int>::size_type`。

+ **在没定义vector元素大小时，不能用下标形式添加元素，定义了大小的话可以。**

  ```c++
  int count = 0;
  vector<int> array(10);
  for (decltype(array.size()) i = 0; i < array.size(); ++i) {
  		array[i] = ++count;
  }
  ```

---

## (三).迭代器介绍

### 1.迭代器使用

+ 和指针不同的是，迭代器使用自己拥有返回迭代器的成员，如begin,end。begin指向第一个元素，end指向尾元素的下一个。

+ **迭代器运算符**

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-Series/blob/main/images/5.png"  width="600"/> 
  </div>


  使用迭代器来实现修改字符串为大写：

  ```c++
  		string str{"some string"};
  
  		//输出Some string
      if (str.begin() != str.end()){
          auto it = str.begin();
          *it = toupper(*it);
      }
      cout<< str <<endl;
  
  		//输出SOME string
      for (auto it = str.begin(); it != str.end() && !isspace(*it); ++it)
      {
          *it = toupper(*it);
      }
      cout<< str <<endl;
  
  		//输出SOME STRING
      for (string::iterator it = str.begin(); it != str.end(); ++it)
      {
          if (isspace(*it))
              continue;
          else
              *it = toupper(*it);
      }
      cout<< str <<endl;
  ```

+ **迭代器类型：**

  + 迭代器使用`iterator`和`const_iterator`来表示迭代器的类型。

    ```c++
    vector<int>::iterator it;//it能读写vector<int>元素
    string::iterator it2;//it2能读写string中的元素
    vector<int>::const_iterator it3;//it3只能读，不能写
    string::const_iterator it4;//it4只能读，不能写
    ```

  + vector对象是常量时，只能用const_iterator；不是常量两者都能使用

+ **begin与end运算符：**

  + begin和end的返回的具体类型由对象是否为常量决定。
  + C++11标准规定了`cbegin`与`cend`，返回的是`const_iterator`类型。

+ **某些对vector对象的操作会使迭代器失效**

  + 不能在范围for循环中向vector对象添加元素
  + 任何一种可能改变vector对象容量的操作(如push_back)，都会是该vector对象的迭代器失效。

### 2.迭代器计算

+ **迭代器运算：**

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-Series/blob/main/images/6.png"  width="600"/> 
  </div>

  
  > 参与比较的两个迭代器必须合法且指向的是同一个容器的元素。

---

## (四).数组

### 1.定义和初始化内置数组

+ **数组的维度必须是一个常量表达式**

  ```c++
  unsigned cnt = 42;
  string bad[cnt];//错误，cnt不是常量表达式
  ```

### 2.指针与数组

+ **标准库函数begin和end(C++11 标准)**

  这两个函数不是成员函数，正确使用的形式是将数组作为它们的参数。

  ```c++
  int ia[] = {0,1,2,3,4,5,6,7,8,9};
  int *beg = begin(ia);
  int *last = end(ia);
  ```

+ **有时候数组的下标也可以是负数：**

  ```c++
  int array[5] = {1,2,3,5,6};
  int a = 2;
  int *p1 = array + a;
  //输出为 2
  cout<< p1[-1] <<endl;
  ```

### 3.C风格字符串

+  **传入C风格字符串函数的字符数组必须以空字符作为结束。**

  ```c++
  char ca[] = {'C', '+', '+'};
  cout<< strlen(ca) << endl;//严重错误：ca没有以空字符作为结束
  ```

+ **string类型转换为C风格字符串：`c_str()`成员函数。**

  + 转换后的C风格字符串只能为const类型，且只能为指针类型。

  ```c++
  string  string1 = "Hello";
  const char *c = string1.c_str();
  //此时c字符数变为Hello!!
  string1 += "!!";
  ```

### 4.与旧代码的接口

+ **允许使用数组来初始化vector对象，但是相反不行。**

  ```C++
  int int_arr[] = {0,1,2,3,4,5};
  vector<int> i_vec(begin(int_arr),end(int_arr));
  vector<int> i_vec2(int_arr + 1, int_arr + 5);
  ```



---

## (五).多维数组

### 1.多维数组引用

+ 多维数组使用范围for语句时，需要把最底层以上的变量设置为引用类型。

  ```c++
  int ia[3][4] = {
              {0,1,2,3},
              {4,5,6,7},
              {8,9,10,11}
      };
  //正确
  for (auto &line : ia)
  	for (auto col : line)
      cout<< col <<endl;
  //错误
  for (auto line : ia)
  	for (auto col : line)
      cout<< col <<endl;
  ```

+ **指针与二维数组:**

  + 因为多维数组是数组的数组，所以**<font color = red>由多维数组名转换得来的指针实际上是指向第一个内层数组的指针</font>**。

    ```C++
    int ia[3][4] = {
                {0,1,2,3},
                {4,5,6,7},
                {8,9,10,11}
        };
    //row是指向含有4个整数的数组
    int (*row)[4] = ia;
    //输出的是row地址
    cout<< row[0] <<endl;
    //输出的是ia[0][2]的值
    cout<< row[0][2] <<endl;
    //输出的是ia[2][0]
    cout<< *row[2] <<endl;
    ```

  + **二维数组与指针剖析**

    ```c++
    int ia[3][4] = {
                {0,1,2,3},
                {4,5,6,7},
                {8,9,10,11}
        };
    //遍历
    for (int (*row)[4] = ia; row != ia + 3; row++)
    	for (int *q = *row ; q != *row + 4 ; q++)
    		cout << *q << " ";
    cout<<endl;
    
    //输出ia地址
    cout<< ia <<endl;
    ////输出ia地址
    cout<< *ia <<endl;
    //输出ia[2][0]
    cout<< **(ia +2 ) <<endl;
    //输出ia[0][2]
    cout<< *(*ia +2) <<endl;
    ```

  + **与引用区分**

    ```c++
    int ia[3][4] = {
                {0,1,2,3},
                {4,5,6,7},
                {8,9,10,11}
        };
    int array [] = {0,1,2,3};
    //row是指向含有4个整数的数组
    int (*row)[4] = ia;
    //引用一个含有4个整数的数组
    int (&arrPtr)[4] = array;
    ```

---

# 三.表达式

## (一).基础

### 1.基本概念

+ **对于左值和右值：**
  + 左值 (lvalue, locator value) 表示了一个占据内存中某个可识别的位置（也就是一个地址）的对象。
  + 右值 (rvalue)则使用排除法来定义。一个表达式不是 左值 就是 右值 。 那么，右值是一个 不 表示内存中某个可识别位置的对象的表达式。

### 2.算数运算符

+ **对于布尔类型取负：**

  ```c++
  bool b = true;
  bool b2 = -b;//b2仍然是true.
  ```

+ **对于取模计算：**

  + 取模计算必须都是整型。
  + m%(-n) 等于m%n；(-m)%n 等于-(m%n)。

## (二).递增和递减运算符

+ **除非必须，否则不用递增递减运算符的后置版本**

  后置版本需要将原始值存储下来以便于返回这个未修改的内容。如果不需要这个值，这就是一种浪费。

+ **后置递增运算符的优先级高于解引用运算符**，因此`*pbeg++`等价于`*(pbeg++)`。

  但是数组名不能直接使用这个例子，迭代器和指针可以。

  ```c++
  int pq[] = {1,2,3,4};
  auto pbegin = begin(pq);
  int *pp = pq;
  //可以
  cout << *pp++ <<endl;
  //可以
  cout << *pbegin++ <<endl;
  //不可以
  cout << *pq++ <<endl;
  ```

## (三).条件运算符

+ **嵌套条件运算符**

  ```c++
  string finalgrade = (grade > 90) ? "high pass"
    															 : (grade < 60) ? "false" : "pass";
  ```

+ **在输出条件表达式中使用条件运算符**

  ```c++
  cout << ((grade < 60) ? "false" : "pass");//输出pass 或者 flase
  cout << (grade < 60) ? "false" : "pass";//输出1 或者 0
  cout << grade < 60 ? "false" : "pass";//错误：试图比较count和60
  ```

## (四).位运算符

+ 对于位运算符，强烈建议仅将位运算符用于处理无符号类型。

## (五).sizeof运算符

+ **C++11标准规定获取类成员大小，只需要使用sizeof运算符时无需提供具体的对象。**

+ **数组和vector使用sizeof：**

  ```C++
  vector<int> vc { 1,2,3,5,6,6,7,8,9,11};
  //输出24，固定大小
  cout<< sizeof(vc)<<endl;
  //输出4，int大小
  cout<< sizeof(vc[1])<<endl;
  
  char st[] = {'h', 'e', 'l','l','o'};
  char st1[] = {'h', 'e', 'l','l','o','\0'};
  string str2 = "hello";
  //输出5
  cout<< sizeof(st)<<endl;
  //输出6
  cout<< sizeof(st1)<<endl;
  //输出24,固定大小
  cout<< sizeof(st2)<<endl;
  ```

---

## (六).类型转换

+ **表达式中既有整数类型的运算对象也有浮点类型的运算对象时，整型会转换为浮点型。**

### 1.算数转换

+ **算数转换的规则定义了一套类型转换的层次，其中运算符的运算对象将转换成最宽的类型。**

### 2.显示转换

+ **static_cast**
  + 任何具有明确定义的类型转换，只要不包括底层const，都可以使用`static_cast`。
  + 当需要把一个较大的算数类型赋值给较小的类型时，`static_cast`很有用。
  + 当把指针存放在`void*`中，并且使用static_cast将其强制转换回原来的类型时，必须确保转换后所得的类型就是指针所指的类型。

---

# 四.语句

























