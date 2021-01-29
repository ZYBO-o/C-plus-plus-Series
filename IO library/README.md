<!-- GFM-TOC -->

* IO

  * [一、IO类](#一IO类)

  * [二、文件输入输出](#二文件输入输出)

  * [三、string流](#三string流)

    <!-- GFM-TOC -->

---

- C++语言不直接处理输入输出，而是通过标准库中的一组类来处理IO

- 这些IO类可从设备读写数据，设备可以是文件、控制台窗口等。还有一些类型允许内存IO，即读写字符串

- 1.2节介绍的IO库：

  - `istream`（输入流）类型，提供输入
  - `ostream`（输出流）类型，提供输出
  - `cin`，是istream对象，从标准输入读取数据
  - `cout`，是ostream对象，向标准输出写数据
  - `cerr`，是ostream对象，用于输出错误信息，写到标准错误
  - `>>`运算符，从istream对象读输入
  - `<<`运算符，向ostream对象写输出
  - `getline`函数，从给定的istream读取一行数据，存入string对象

- IO类之间的继承关系：

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/tree/main/images/13.png"  width="600"/> 
  </div>

## 一.IO类

- 实际程序不仅要从`控制台窗口`进行IO操作，还需要读写`文件`，而且用IO操作处理`字符串`也很方便。另外，程序可能需要读写`宽字符`文本

- 标准库的IO类型在3个头文件中：

  - `iostream`头文件定义了读写流的基本类型
  - `fstream`头文件定义了读写命名文件的类型
  - `sstream`头文件定义了读写string对象的类型

- 表8.1是标准库中的这些IO类型和头文件

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/tree/main/images/8.png"  width="600"/> 
  </div>

- 声明一个类`继承`自另一个类，则通常可将`派生类`当作`基类`来使用
- 类型ifstream和istringstream都继承自istream，即可以像使用istream对象一样使用ifstream和istringstream对象
- 本节所述的流特性都可无差别地应用于普通流、文件流、字符串流，以及char和wchar_t版本

### 1.IO对象无拷贝或赋值

+ 不能拷贝IO对象
+ 不能对IO对象赋值——>不能将形参或返回类型设置为流类型
+ 进行IO操作的函数通常以引用方式传递和返回流，因为会改变其状态，所以不能是const类型

### 2.条件状态

- IO很可能发生错误。一些错误可恢复，另一些错误在系统深处，超过了程序可处理的范围

- 表8.2定义的函数和标志可帮助访问和操纵流的`条件状态`

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/tree/main/images/9.png"  width="600"/> 
  </div>

- 表8.2中，定义了一个机器无关（？）的`iostate`类型，可表达流的状态，这个类型是`标志位`的集合
- 表8.2中，定义了4个iostate类型的constexpr值来表示特定的位模式。这些值用于表示特定的条件状态，可与位运算符一起使用来一次检测或设置多个标志位
  - `badbit`表示系统级错误，不可恢复。一旦它被置位，流就无法使用
  - `failbit`在发生可恢复错误时被置位，如读取类型错误
  - 读到EOF处，`eofbit`和failbit都被置位
  - `goodbit`的值为0表示无错误
- badbit、failbit、eofbit中的任一个被置位，则检测流状态的条件都会失败
- 表8.2中，定义了一组函数来查询这些iostate标志位的状态，
  - `good()`在所有错误位均未置位时返回true
  - `bad()`、`fail()`、`eof()`均在对应错误位被置位时返回true
  - badbit被置位时，`fail()`也会返回true
- 将流当作条件时，等价于判断`!fail()`，而`eof()`和`bad()`操作只能表示特定错误
- 表8.2中定义的`rdstate()`操作返回iostate类型值，表示当前状态
- 表8.2中定义的`setstate()`操作接受iostate类型值，将给定的条件位置位，表示发生了对应错误
- 表8.2中定义的`clear`成员函数有两个版本：
  - `clear()`清除所有错误标志位
  - `clear(flags)`接受iostate类型值，表示流的新状态
- 例子：rdstate、setstate、clear

```c++
//记住cin的当前状态
auto old_state = cin.rdstate(); //记住cin的当前状态
cin.clear();                    //使cin有效
process_input(cin);             //使用cin
cin.setstate(old_state);        //将cin置为原有状态
```

```c++
int main(){
    int value = 0, sum = 0;

    while (cin >> value, !cin.eof())
    {
        if (cin.bad())
        {
            throw runtime_error("IO stream corrupted");
        }
        if (cin.fail())
        {
            cerr << "bad data, try again" << endl;
            cin.clear();//回复流状态
            cin.ignore(200,'\n');
            continue;
        }
        sum += value;
        cout<< "The sum of value is : " << sum << endl;
    }
  	return 0;
}

//=============================输出==============================
2
The sum of value is : 2
3
The sum of value is : 5
q
bad data, try again
ee
bad data, try again
```

### 3.管理输出缓冲

+ **刷新输出缓冲**
  + `endl`：输出换行，然后刷新缓冲区
  + `ends`：输出空格，然后刷新缓冲区
  + `flush`：无输出，刷新缓冲区

+ **unitbuf操纵符**
  + `unitbuf`：告诉流在接下来的每次写操作之后都进行一次flush操作
  + `nounitbuf`：重置流，使其恢复使用正常的系统管理的缓冲区刷新机制

## 二.文件输入输出

- 头文件`fstream`定义了3个IO类来读写文件：

  - 类`ifstream`从给定文件读数据
  - 类`ofstream`向给定文件写数据
  - 类`fstream`可读写文件

- fstream中的这些类型继承自iostream的对应类型，它们提供的操作类似cin和cout，即`<<`、`>>`、`getline`等，以及8.1节中的所有操作

- 表8.3是fstream中比iostream新增的成员 

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/tree/main/images/10.png"  width="600"/> 
  </div>

### 1.使用文件流对象

- 要读写文件时应定义`文件流对象`，并将对象与文件关联
- 表8.3中定义的`open(s)`将对象与文件关联，定位给定文件并视情况打开为读/写模式
- 创建文件流对象时可在构造函数中提供文件名`filename`，此时`open(filename)`会被自动调用。在C++11之前，文件名只能是C风格字符串，C++11后文件名可是string对象或C风格字符串
- 在要求`基类`对象的地方，可用`派生类`对象代替。例如，接受iostream引用/指针的函数，可用对应的fstream/sstream引用/指针来调用
- 可以先定义空文件流对象（默认初始化），再调用`open`与文件关联
- 如果调用`open`失败，则`failbit`被置位
- 对已经关联到文件的流再次调用`open`会失败，并将`failbit`置位
- 如要将已经关联到文件的流关联到另一个文件，必须先用`close`关闭已关联的文件

```c++
int main(){
    ofstream outfile("test.txt");
    if (outfile)
        outfile << "This is a test text file" << endl;
    outfile.close();
    
    string filename = "one.txt";
    ifstream infile(filename);
    string line;
    if(infile)
    {
        while (infile >> line)
            cout << line <<endl;
    }
    infile.close();
   	return 0;
}
```



### 2.文件模式

+ 表8.4定义了每个文件流都有一个关联的`文件模式`，用来指出如何使用文件

  <div align="center">  
    <img src="https://github.com/ZYBO-o/C-plus-plus-Series/tree/main/images/11.png"  width="600"/> 
  </div>

- 用open打开文件或用文件名构造文件流对象时，都可指定文件模式

- 指定文件模式的限制：

  - 只可对ofstream或fstream对象设定`out`模式
  - 只可对ifstream或fstream对象设定`in`模式
  - 只有当out也被设定时才可设定`trunc`模式
  - 只要trunc未被设定，就可设定app模式。在`app`模式下，文件总以out模式被打开
  - 默认情况下，即使未指定trunc，以out模式打开的文件也会被截断，即默认用out即用了trunc。为`避免截断`，可指定app模式，使数据追加到文件末尾；或指定in模式，同时读写。
  - `ate`和`binary`模式可用于任何文件流对象，且可与其他任何模式组合

- 每个文件流类型都定义了默认的文件模式：

  - `ifstream`关联的文件默认以in模式打开
  - `ofstream`关联的文件默认以out模式打开
  - `fstream`关联的文件默认以in和out模式打开

- 默认方式（out模式）打开ostream时，文件会被丢弃。因为out模式意味着同时使用trunc模式。保留已有数据的方法是显式指定`app`或`in`模式

- 同一个流，每次用open关联到不同文件时，都可改变模式

- 例子：指定模式

  ```c++
  //以下3条等价，都会截断
  file1 ofstream out("file1"); 
  ofstream out("file1", ofstream::out); 
  ofstream out("file1", ofstream::out|ofstream::trunc); 
  //以下2条等价，为保留文件内容，显式指定app模式 
  ofstream out("file2", ofstream::app); 
  ofstream out("file2", ofstream::out|ofstream::app);`
  ```

  

## 三.string流

- 头文件`sstream`定义了3个类型来支持内存IO，它们可读写string：
  - `istringstream`从string读数据
  - `ostringstream`向string写数据
  - `stringstream`既可读string又可写string
- 头文件sstream中定义的类型都继承自iostream中对应的类型
- 表8.5是sstream中定义的类型的特有操作

<div align="center">  
  <img src="https://github.com/ZYBO-o/C-plus-plus-Series/tree/main/images/12.png"  width="600"/> 
</div>


### 1.使用istringstream

- 用`getline`逐行读取，每次读到的整行文本用`istringstream`读取单词

### 2.使用ostringstream

- 用`ostringstream`逐步构造输出，最后一起打印



```c++
//文章转换为单词例子：

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
int main(){
    string fileName , s, word;
    vector<string> svec;
    istringstream isstream;

    fileName = "file.txt";
    ifstream inFile(fileName);

    if(!inFile)
        return -1;

    while(getline(inFile,s))
        svec.push_back(s);

    inFile.close();

    for (vector<string>::const_iterator iterator= svec.begin();  iterator != svec.end() ; ++iterator) {
        isstream.str(*iterator);
        while (isstream >> word)
        {
            cout<< word << endl;
        }
        isstream.clear();
    }
		return 0;
}
```