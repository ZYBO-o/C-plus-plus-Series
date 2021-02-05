- 标准库未给容器添加大量功能，而是提供一组独立于容器的`泛型算法`：
  - `算法`：它们实现了一些经典算法的公共接口
  - `泛型`：它们可用于不同类型的容器和不同类型的元素
- 利用这些算法可实现容器基本操作很难做到的事，例如查找/替换/删除特定值、重排顺序等

## 一.概述

- 大多数算法定义在`algorithm`头文件中，另外一组数值算法定义在`numeric`头文件中

- 标准库算法不直接操作容器，而是遍历迭代器范围

- 指针就像内置数组上的迭代器，故泛型算法也可操作内置数组和指针

- `find`算法：

  - 作用：将范围中每一个元素与给定值比较，返回第一个等于给定值的元素的迭代器，如果没有匹配则返回该范围的尾后迭代器。
  - 用法：有3个参数，前2个是输入范围，第3个是给定值。
  - 实现：调用给定值类型的`==`算符来比较。

  ```c++
  vector<int> Array {1,2,3,4,5,6,7,8,9};
  int value = 6;
  auto result = find(Array.cbegin(), Array.cend(), value);
  cout << "The value " << value << (result == Array.cend() ? " is not present !" : " is present !") << endl;
  ```

- 算法只依赖迭代器来访问元素并在范围中推进，不依赖于容器操作。但迭代器依赖元素类型上定义的算符，如`==`、`<`等

- 算法不会改变容器大小。它可能改变元素值或移动元素，但不会添加或删除。

- 标准库定义了`插入迭代器`，给它们赋值时会在容器上插入。算法操作这样的迭代器时可完成插入元素的效果。

- 一些算法提供了接口，允许使用自定义的`谓词`来代替默认算符

- `count` 算法：

  - 作用：将范围中每一个元素与给定值比较，返回给定值在范围中出现的次数。
  - 用法：有3个参数，前2个是输入范围，第3个是给定值。
  - 实现：调用给定值类型的`==`算符来比较。

## 二.初试泛型算法

- 附录A中列出所有算法
- `输入范围`：大多标准库算法都对一个范围内的元素操作，这个范围称为输入范围。接受输入范围的算法总是用前两个参数来表示输入范围。
- 多数算法遍历输入范围的方式相似，但使用元素的方法不同（是否读，是否写，是否重排等）。

### 1.只读算法

- `只读算法`只读取输入范围的元素，不改变它们。如上一节的find和count

- 使用只读算法，最好用cbegin/cend

- `accumulate`算法（定义于numeric）：

  - 作用：对范围中元素求和，再加上给定值，返回求值结果。
  - 用法：有3个参数，前2个是输入范围，第3个是给定值。
  - 实现：调用给定值类型的`+`算符来求和。

- 例子：算法严格使用给定值的操作符

  ```c++
  vector<string> v={"hello","world"};
  string sum=accumulate(v.cbegin(),v.cend(),"OK");          //错，const char *类型未定义+算符
  string sum=accumulate(v.cbegin(),v.cend(),string(""));  //对，string上定义了+算符
  //sum:OKhelloworld
  ```

- `equal`算法：

  - 作用：确定两序列的值是否相同。所有元素都相等时返回true，否则false
  - 用法：有3个参数，前2个是第一个序列的输入范围，第3个是第二个范围的首迭代器。
  - 实现：调用`==`算符来比较，元素类型不必严格一致。

  > string类重载了==，可以比较两个字符串是否长度相等且元素对位相等。
  >
  > c风格字符串本质是char* 类型，用==比较两个char* 对象，只是检查两个指针值是否相等，即地址是否相等。

- 某些算法接受单一迭代器来指定第二个序列，这些算法都假定第二个序列至少和第一个序列一样长，如果第二个更短，会出产生严重的错误。

```c++
vector<string> string1={"hello","world"};
list<const char *> string2 = {"hello","world","OK"};
cout << equal(string1.cbegin(), string1.cend(), string2.cbegin()) << endl;
//输出为true
```

> + 算法要求的是两个序列中的元素，所以元素类型不要求相同，但是必须能够使用`==`来进行比较两个序列中的元素。



### 二.写容器元素的算法 

- 可对序列中元素重新赋值，要求原序列大小不小于要写入的元素数目。算法不执行容器操作，故不可改变序列大小

- `fill`算法：

  - 作用：用给定值填满输入范围
  - 用法：有3个参数，前2个是输入范围，第3个是给定值。

- 操作两序列的算法不要求两序列的容器相同，但要求元素可操作

- 操作两序列的算法分为两种：

  - 接受3个迭代器，前两个表示第一个序列的范围，第三个表示第二个序列的起始。总是假定第二个序列至少和第一个序列一样长
  - 接受4个迭代器，前两个表示第一个序列的范围，后两个表示第二个序列的范围

- `fill_n`算法：

  - 作用：用给定值填满长为n的区间
  - 用法：有3个参数，第1个代表序列起始的迭代器，第2个是序列长度的计数值，第3个是填入的给定值。

- fill_n假定长为n的空间总是有效的，类似指针运算。算法不会改变容器的大小。

  > **<font color = red>不能在空容器上调用fill_n，或者类似的写元素的算法，因为是空的，这条语句的结果是未定义的。</font>**

- `插入迭代器`：给插入迭代器赋值会向容器中插入元素，即真正改变容器的大小。

- 通过给插入迭代器赋值，算法可保证容器中总有足够的空间

- `back_inserter`函数定义于`iterator`头文件中，**<font color = red>它接受一个指向容器的引用，返回该容器的一个插入迭代器。通过此迭代器赋值时，赋值符会调用容器类型的`push_back`来添加元素</font>**

- 例子：算法中使用back_inserter

  ```c++
  vector<int> vec;                    //空vector
  //fill_n(vec.begin(),10,0);           //错，算法不可向空vector写值
      
  cout<< "size is : " << vec.size() << endl;
  fill_n(back_inserter(vec),10,0);    //对，在vec尾部插入10个0
  
  for (auto i : vec) 
      cout << i << " " ;
  cout << endl;
  cout<< "size is : " << vec.size() << endl;
  
  //=======================输出=========================
  //	size is : 0
  //	0 0 0 0 0 0 0 0 0 0 
  //	size is : 10
  ```

- `copy`算法：

  - 作用：将输入范围的值拷贝到目标序列，返回目标序列的尾后迭代器
  - 用法：有3个参数，前2个是输入范围，第3个是目标序列的起始位置

- 很多算法提供`copy版本`，即计算新元素的值后，不放入原来的序列，而是放入一个新序列中。

```c++
int array1[] {1,2,3,4,5,6,7,8,9};
int array2[sizeof(array1)/sizeof(*array1)];
auto ret = copy(begin(a1),end(a1),a2);//把a1的内容拷贝给a2
//ret的值是指向a2尾元素之后的位置
```

- `replace`算法：

  - 作用：将序列中所有等于给定值的元素换为另一个值
  - 用法：有4个参数，前2个是输入范围，后2个分别是要搜索的值和新值

- `replace_copy`算法：

  - 作用：将序列中所有等于给定值的元素换为另一个值，放入新序列，原序列不变。
  - 用法：有5个参数，前2个是输入范围，第3个是输出序列的首迭代器，最后2个分别是要搜索的值和新值

- 例子：replace和replace_copy

  ```c++
  list<int> ilst={0,1,2,3,4};
  vector<int> ivec;
  //原址版本，将ilst中的0都替换为42
  replace(ilst.begin(),ilst.end(),0,42);                              
  for (auto i : ilst)
      cout << i << " " ;
  cout<< endl;
  //copy版本，将ilst中的0替换为42后插入ivec，ilst不变
  replace_copy(ilst.cbegin(),ilst.cend(),back_inserter(ivec),42,1);   
  for (auto i : ilst)
      cout << i << " " ;
  cout<< endl;
  for (auto i : ivec)
      cout << i << " " ;
  
  //输出：
  // 42 1 2 3 4 
  // 42 1 2 3 4 
  // 1 1 2 3 4 
  ```

### 3.重排容器元素的算法

- 可对容器中元素重新排列顺序

- `sort`算法：

  - 作用：重排输入序列的元素使其有序
  - 用法：有2个参数，是输入范围
  - 实现：调用序列元素类型的`<`算符

- `unique`算法：

  - 作用：重排输入序列，消除相邻重复项。返回消除后的无相邻重复值的范围的尾后迭代器
  - 用法：有2个参数，是输入范围

- unique不真正删除元素，只是将后面的不重复值前移来覆盖前面的重复值，使不重复值在序列前部。

- unique将不重复元素向首部集中，尾部（返回迭代器之后）的元素值是未定义

- 真正删除元素需要使用容器操作

- 例子：消除重复单词

  ```c++
  //将输入vector中的string元素重排并消除重复
  void elimDups(vector<string> &words){
      sort(words.begin(),words.end());                    //将元素排序，使重复项相邻
      auto end_unique=unique(words.begin(),words.end());  //将不重复元素集中到序列前端，返回不重复元素序列的尾后迭代器
      words.erase(end_unique,words.end());                //擦除不重复序列之后的元素
  }
  ```

## 三.定制操作

- 对于使用元素的`==`、`<`等算符的算法，标准库还为这些算法定义了额外的版本，允许我们提供自己定义的操作来代替默认运算符。

### 1.向算法传递参数

- `谓词`：是一个可调用的表达式，其返回值可用作条件（即true/false）。按照参数的数量分为`一元谓词`和`二元谓词`

```c++
bool IsShorter(const string& s1, const string& s2)
{
	return s1.size() < s2.size();
}
```

- 以上部分我们定义了一个谓词，然后就可将该谓词传递给算法`sort`，`sort`就会用该算法代替之前的<运算符比较两个元素，也就是说，之前的排序我们是用默认的`<`运算对两个元素的大小进行比较后决定两个元素的次序的，现在我们用的谓词是通过比较两个元素的长度大小来决定两个元素的次序的，于是最终结果将是按元素长度进行排序，也就是说，我们通过一个自定义的谓词传递给算法，从而改变算法是如何对元素进行排序的行为的。
- `stable_sort`是`稳定排序`，即维持相等元素的原有顺序

```c++
void elimDups(vector<string> &words)
{
    //按照字典序排序words,以便查找重复单词
    sort(words.begin(), words.end());
    //unique重排输入范围，使得每个单词只出现一次
    //排序在范围的前部，返回指向不重复区域之后的一个位置的迭代器
    auto end_unique = unique(words.begin(), words.end());
    //使用向量操作erase删除重复单词
    words.erase(end_unique, words.end());
}

bool isShorter(const string &s1, const string &s2)
{
    return s1.size() < s2.size();
}

int main(){
    vector<string> words {"fox", "jumps", "the", "over", "turtle", "quick", "red", "red", "slow", "the"};
    //将Words按字典序排序，并消除重复单词
    elimDups(words);
    for (const auto &s : words) {
        cout << s << " ";
    }
    cout << endl;
  
    //按长度重新排序，长度相同的单词维持字典序
    stable_sort(words.begin(), words.end(), isShorter);
    for (const auto &s : words) {
        cout << s << " ";
    }
    cout << endl;
    return 0;
}
```

### 2.lambda表达式

- `lambda`应用的场景：函数接口已固定，但要传入额外的参数，可用lambda的捕获列表。例如谓词中要获取局部变量时。
- `find_if`算法：
  - 作用：对输入范围的每个元素调用给定谓词，返回第一个使谓词非0的元素的迭代器
  - 用法：有3个参数，前2个是输入范围，第3个是一元谓词
- `可调用对象`：一个对象或表达式，若能使用调用运算符`()`，就是可调用的
- 4种可调用对象：`函数`、`函数指针`、`重载了调用算符的类`、`lambda表达式`

#### 介绍lambda

- `lambda表达式`：是一个可调用的代码单元，即一个未命名的`内联函数`。它有捕获列表、返回类型、形参列表、函数体，但可定义在函数内部（函数不可）

- lambda表达式形式:

   ```c++
  [capture list](parameter list) -> return type {function body}
   ```

  - capture list是`捕获列表`，是lambda所在函数中定义的局部变量的列表
  - parameter list、return type、function body与函数一样
  - lambda必须用`尾置返回`
  - 可忽略形参列表和返回类型，但必须有捕获列表和函数体
  - 若函数体不是单一return语句，则必须指定返回类型（否则为void）

- 可用变量定义的形式定义lambda，用函数调用的方式使用lambda

```c++
auto f=[]{return 42;};
cout<<f()<<endl; //用调用算符使用lambda
```

#### 向lambda传递参数

- 调用lambda时用实参初始化形参的方式和函数相同，但`lambda不可有默认实参`
- lambda将局部变量包含在捕获列表中来访问它们，只有被捕获到的局部变量才可在函数体中被使用。但只有`局部非static变量`才需要捕获，`lambda可直接使用定义在当前函数之外的名字和局部static变量`

```c++
[sz](const string &a)
{
  return a.size() > sz;
}
```

- `for_each`算法：
  - 作用：对输入范围的每个元素调用给定的可调用对象
  - 用法：有3个参数，前2个是输入范围，第3个是可调用对象
- 例子：计数并按字典序打印长度>=给定值的字符串

```c++
void elimDups(vector<string> &words)
{
    //按照字典序排序words,以便查找重复单词
    sort(words.begin(), words.end());
    //unique重排输入范围，使得每个单词只出现一次
    //排序在范围的前部，返回指向不重复区域之后的一个位置的迭代器
    auto end_unique = unique(words.begin(), words.end());
    //使用向量操作erase删除重复单词
    words.erase(end_unique, words.end());
}

bool isShorter(const string &s1, const string &s2)
{
    return s1.size() < s2.size();
}

//计数并按字典序打印vector<string>中长度>=给定值的string
void biggies(vector<string> &words, vector<string>::size_type sz){
    //按字典排序并消除重复
    elimDups(words);
    //对字符串长度做稳定排序，长度相同的单词维持字典序
    //用lambda做二元谓词比较两元素
    stable_sort(words.begin(),words.end(),
                [](const string &a, const string &b){return a.size()<b.size();});
    //找到第一个长度>=sz的元素
    //用lambda做一元谓词比较元素和变量
    auto wc=find_if(words.begin(),words.end(), [sz](const string &a){return a.size()>=sz;});
    //计算长度>=sz的元素数目
    auto count=words.end()-wc;
    cout<<count<<" "<< ((count>1)?("words"):("word") ) <<" of length "<<sz<<" or longer" <<endl;
    //打印长度>=sz的元素，每个元素后接一个空格
    //用lambda遍历元素
    for_each(wc,words.end(),
             [](const string &s){cout<<s<<" ";});
    cout<<endl;
}

int main(){
    vector<string> words {"fox", "jumps", "the", "over", "turtle", "quick", "red", "red", "slow", "the"};
    biggies(words,4);
}

```

### 3.lambda捕获和返回

- `lambda实际是匿名类`：定义lambda时，编译器生成一个与其对应的未命名的类类型
- 向函数传递lambda时，同时定义了一个新类型和该类型的一个对象，传递的参数就是该对象。用auto定义一个lambda初始化的变量时，该变量也是这种对象。
- 从lambda生成的类都有一个数据成员对应捕获到的变量。lambda的数据成员在创建时被初始化，即`被捕获的变量用于初始化lambda匿名对象的成员`
- lambda捕获变量的方式可用`值捕获`和`引用捕获`

#### 值捕获

+ `值捕获`存在拷贝，且值捕获的变量是在lambda创建（lambda对象构造）时被拷贝，而不是调用时拷贝，故创建lambda后修改捕获变量不影响lambda中的值。

#### 引用捕获

- 使用`引用捕获`时必须确保lambda执行时被捕获变量存在。例如，从函数中返回lambda时不可用引用捕获。
- 最佳实践：尽量减少捕获的变量，且避免捕获指针/引用
- 例子：值捕获的变量会被拷贝，引用捕获的变量不会

```c++
void fcn1(){
    size_t v=42;
    auto f1=[v]{return v;};     //值捕获，创建lambda（构造lambda对象）
    auto f2=[&v]{return v;};    //引用捕获，创建lambda（构造lambda对象）
    v=0;                        //改变捕获变量的值
    auto j1=f1();               //j1是42，因为lambda创建时保存了捕获变量的拷贝
    auto j2=f2();               //j2是0，因为lambda创建时未拷贝捕获变量，只是建立了引用
}
```

#### 隐式捕获

- `隐式捕获`：可让编译器根据lambda函数体中的代码来推断要捕获哪些变量。
- 使用隐式捕获，需在捕获列表中写`&`或`=`，分别对应引用捕获和值捕获
- 可混合使用隐式捕获和显式捕获，只需在捕获列表中写`&`或`=`，再写显式捕获的变量，要求：
  - 捕获列表第一个元素必须是`&`或`=`，指定默认为引用/值捕获
  - 显式捕获的变量必须使用与隐式捕获不同的方式。即隐式引用捕获，则显式必须为值捕获，反之亦然
- 例子：混合使用隐式捕获和显式捕获

```c++
void print_strings(vector<strin> &words, ostream &os=cout, char c=' '){
    //c为显式值捕获，其他变量（os）为隐式引用捕获
    for_each(words.begin(),words.end(),
             [&,c](const string &s){os<<s<<c;});
    //os为显式引用捕获，其他变量（c）为隐式值捕获，等价于上一行
    for_each(words.begin(),words.end(),
             [=,&os](const string &s){os<<s<<c;});
}
```

+ 表10.1是lambda捕获列表可能的状态

<div align="center">  
  <img src="https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/images/33.png"  width="600"/> 
</div>

#### 可变lambda

- `可变lambda`：`普通lambda不会改变值捕获的变量的copy的值`，但可变lambda可改变值捕获的变量的copy的值，只需在参数列表后使用关键字`mutable`
- 例子：可变lambda

```c++
void fcn3(){
    size_t v=42;
    auto f=[v]() mutable {return ++v;}  //mutalbe，允许改变捕获到的copy的值
    v=0;
    auto j=f();                         //j是43
}
void fcn4(){
    size_t v=42;//v是非const变量
    auto f=[&v]() {return ++v;}  //允许改变非const引用的值
    v=0;
    auto j=f();                         //j是1
}
```

#### 指定lambda返回类型

- 若lambda函数体包含return之外的任何语句，则编译器推断它返回void，返回void的函数不能返回值，除非手动指定返回类型
- `transform`算法：
  - 作用：对输入范围的每个元素调用可调用对象，将返回值依次写入目标序列
  - 用法：有4个参数，前2个是输入范围，第3个是目的序列的首迭代器，第4个是可调用对象
- transform写入的目标序列和输入序列可以相同，即可以向原址写入
- transform和for_each的区别：
  - transform可进行非原址写，for_each不可（除非在可调用对象内写非原址目标）
  - transform通过可调用对象的返回值写入，for_each在可调用对象内部操作
- 为lambda指定返回类型时，必须使用`尾置返回`
- 例子：lambda的返回值

```c++
vector<int> vi={0,1,2,3,4};
transform(vi.begin(),vi.end(),vi.begin(),
          [](int i) {if(i<0) ? -i : i;});
//错误,不能推断lambda的返回类型
transform(vi.begin(),vi.end(),vi.begin(),
          [](int i) {if(i<0) return -i; else return i;}); 
//使用尾置返回
transform(vi.begin(),vi.end(),vi.begin(),
          [](int i)->int{if(i<0) return -i; else return i;}); //取绝对值
```



### 3.参数绑定

- 对于少数地方使用的简单操作用lambda，而多次调用时应该定义函数。

- 若lambda的捕获列表为空，可用函数替换它。但对于有捕获列表的lambda，很难用函数替换。因为不能在函数中定义函数，导致不能在不修改形参的前提下使用局部变量（例如标准库算法中的可调用对象，其形参必须固定）。

  > 如find_if函数，它只接受一个一元谓词，因此传递给find_if的可调用对象必须接受单一参数。biggies传递给find_if的lambda使用捕获列表来获取sz，为了用check_size来代替此lambda，必须解决如何向sz形参传递一个参数的问题。

  ```c++
  bool check_size(const string &s, string::size_type sz)
  { 
  		return s.size() > sz;
  }
  ```

#### 标准库bind函数

- 在`functional`头文件中定义了`bind`函数，可看作通用的函数适配器。它接受一个可调用对象，生成新的可调用对象来适应原对象的参数列表（即改变可调用对象的调用接口）
- 调用bind的形式为：`auto newCallable=bind(callable,arg_list);`
  - callable是可调用对象，arg_list是逗号分隔的参数列表，对应callable的参数
  - 调用newCallable时，是在用arg_list的参数调用callable
  - arg_list中的参数可包含占位符，即`_n`，其中`n`是传入newCallable的第n个参数

```c++
bool check_size(const string &s, string::size_type sz)
{
    return s.size() >= sz;
}

int main(){
    vector<string> words {"fox", "jumps", "the", "over", "turtle", "quick", "red", "red", "slow", "the"};
    auto wc = find_if(words.begin(), words.end(), bind(check_size, std::placeholders::_1, 4));
    cout << words.end() - wc << endl;
}
```

#### 绑定check_size的sz参数

+ 使用`bind`生成一个调用`check_size`的对象。

```c++
bool check_size(const string &s, string::size_type sz)
{
    return s.size() >= sz;
}

int main()
{
		auto check6 = bind(check_size,std::placeholders::_1,6);
		string s = "hello";
  	bool b1 = check6(s);
  	cout << b1 << endl;
}
```

> 此bind调用生成一个可调用对象，将 check_size 的第二个参数绑定到 sz 的值。当 find_if 对words中的string调用这个对象时，这些对象会调用 check_size ，将给定的string和sz传递给它。因此 find_if 可以有效的输入序列中每个string调用check_size，实现string大小和size的对比。

- 名字`_n`都定义于`placeholders`命名空间中，该命名空间又定义于`std`命名空间。同时，placeholders命名空间定义于`functional`头文件
- 使用`using namespace namespace_name`来说明希望所有来自namespace_name的名字都可在程序中直接使用

- 例子：使用bind

```c++
auto g=bind(f,a,b,_2,c,_1); //对g的定义，abc类似于lambda的捕获列表
g(X,Y);                   //调用g
f(a,b,Y,c,X);             //等价于调用f
```

#### 绑定引用参数

- 默认下，bind的非占位符参数被`拷贝`到可调用对象中，类似lambda中的`值捕获`
- 用`ref`函数可实现lambda中的`引用捕获`
- `ref`函数返回一个对象，该对象中包含输入的引用，且可拷贝。若需要包含const引用，则应用`cref`函数。ref和cref也定义于`functional`头文件中
- 例子：用bind和ref捕获引用

```c++
/* 上下文：os是局部变量，引用输出流；c是局部变量，类型为char */
//lambda实现，引用捕获输出流，值捕获字符
for_each(words.begin(),words.end(),
         [&os,c](cons string &s){os<<s<<c;});
//函数实现，要被标准库算法使用，需要用bind捕获os和c
ostream &print(ostream &os, const string &s, char c){
    return os<<s<<c;
}
//错，os不可拷贝，不能用默认方式bind
for_each(words.begin(),words.end(),
         bind(print,os,_1,' '));
//对，用ref返回的对象包含os引用且可拷贝
for_each(words.begin(),words.end(),
         bind(print,ref(os),_1,' '));
```



### 四.再探迭代器

- `iterator`头文件中定义了额外的迭代器：
  - `插入迭代器`：被绑定到一个容器，赋值时向容器中插入元素
  - `流迭代器`：绑定到输入输出流，用于遍历这个流
  - `反向迭代器`：向后而不是向前移动，除forward_list外的所有标准库容器都有反向迭代器
  - `移动迭代器`：不是拷贝元素，而是移动元素

### 1. 插入迭代器

















