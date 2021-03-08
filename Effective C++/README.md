# Effective C++

## 一.让自己习惯C++

+ [1.视C++为一个语言联邦](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Accustoming%20Yourself%20to%20C%2B%2B.md#1%E8%A7%86c%E4%B8%BA%E4%B8%80%E4%B8%AA%E8%AF%AD%E8%A8%80%E8%81%94%E9%82%A6)
+ [2.尽量以 const, enum, inline 替换 #define](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Accustoming%20Yourself%20to%20C%2B%2B.md#2%E5%B0%BD%E9%87%8F%E4%BB%A5-const-enum-inline-%E6%9B%BF%E6%8D%A2-define)
+ [3.尽可能使用 const](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Accustoming%20Yourself%20to%20C%2B%2B.md#3%E5%B0%BD%E5%8F%AF%E8%83%BD%E4%BD%BF%E7%94%A8const)
+ [4.确定对象被使用前已先被初始化](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Accustoming%20Yourself%20to%20C%2B%2B.md#4%E7%A1%AE%E5%AE%9A%E9%9D%A2%E5%90%91%E5%AF%B9%E8%B1%A1%E8%A2%AB%E4%BD%BF%E7%94%A8%E5%89%8D%E5%B7%B2%E8%A2%AB%E5%88%9D%E5%A7%8B%E5%8C%96)

## 二.构造/析构/赋值运算

* [5.了解C++默认编写并调用哪些函数](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Constructors%2CDestruction%2C%20and%20Assignment%20Operators.md#5%E4%BA%86%E8%A7%A3c%E9%BB%98%E8%AE%A4%E7%BC%96%E5%86%99%E5%B9%B6%E8%B0%83%E7%94%A8%E5%93%AA%E4%BA%9B%E5%87%BD%E6%95%B0)

* [6.若不想使用编译器自动生成的函数，就该明确拒绝](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Constructors%2CDestruction%2C%20and%20Assignment%20Operators.md#6%E8%8B%A5%E4%B8%8D%E6%83%B3%E4%BD%BF%E7%94%A8%E7%BC%96%E8%AF%91%E5%99%A8%E8%87%AA%E5%8A%A8%E7%94%9F%E6%88%90%E7%9A%84%E5%87%BD%E6%95%B0%E5%B0%B1%E8%AF%A5%E6%98%8E%E7%A1%AE%E6%8B%92%E7%BB%9D)

+ [7.为多基类声明virtual析构函数](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Constructors%2CDestruction%2C%20and%20Assignment%20Operators.md#7%E4%B8%BA%E5%A4%9A%E5%9F%BA%E7%B1%BB%E5%A3%B0%E6%98%8Evirtual%E6%9E%90%E6%9E%84%E5%87%BD%E6%95%B0)
+ [8.别让异常逃离析构函数](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Constructors%2CDestruction%2C%20and%20Assignment%20Operators.md#8%E5%88%AB%E8%AE%A9%E5%BC%82%E5%B8%B8%E9%80%83%E7%A6%BB%E6%9E%90%E6%9E%84%E5%87%BD%E6%95%B0)
+ [9.绝不在构造和析构过程中调用virtual函数](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Constructors%2CDestruction%2C%20and%20Assignment%20Operators.md#9%E7%BB%9D%E4%B8%8D%E5%9C%A8%E6%9E%84%E9%80%A0%E5%92%8C%E6%9E%90%E6%9E%84%E8%BF%87%E7%A8%8B%E4%B8%AD%E8%B0%83%E7%94%A8virtual%E5%87%BD%E6%95%B0)
+ [10.令operator = 返回一个reference to * this](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Constructors%2CDestruction%2C%20and%20Assignment%20Operators.md#10%E4%BB%A4operator--%E8%BF%94%E5%9B%9E%E4%B8%80%E4%B8%AAreference-to--this)

+ [11.在operator = 中处理"自我赋值"](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Constructors%2CDestruction%2C%20and%20Assignment%20Operators.md#11%E5%9C%A8operator--%E4%B8%AD%E5%A4%84%E7%90%86%E8%87%AA%E6%88%91%E8%B5%8B%E5%80%BC)
+ [12.复制对象时勿忘其每一个成分](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Constructors%2CDestruction%2C%20and%20Assignment%20Operators.md#12%E5%A4%8D%E5%88%B6%E5%AF%B9%E8%B1%A1%E6%97%B6%E5%8B%BF%E5%BF%98%E5%85%B6%E6%AF%8F%E4%B8%80%E4%B8%AA%E6%88%90%E5%88%86)

## 三.资源管理

* [13.以对象管理资源](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Resourse%20Management.md#13%E4%BB%A5%E5%AF%B9%E8%B1%A1%E7%AE%A1%E7%90%86%E8%B5%84%E6%BA%90)
* [14.以资源管理类中小心copying行为](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Resourse%20Management.md#14%E4%BB%A5%E8%B5%84%E6%BA%90%E7%AE%A1%E7%90%86%E7%B1%BB%E4%B8%AD%E5%B0%8F%E5%BF%83copying%E8%A1%8C%E4%B8%BA)
* [15.在资源管理类中提供原始资源访问](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Resourse%20Management.md#15%E5%9C%A8%E8%B5%84%E6%BA%90%E7%AE%A1%E7%90%86%E7%B1%BB%E4%B8%AD%E6%8F%90%E4%BE%9B%E5%8E%9F%E5%A7%8B%E8%B5%84%E6%BA%90%E8%AE%BF%E9%97%AE)
* [16.成对使用new和delete时要采取相同形式](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Resourse%20Management.md#16%E6%88%90%E5%AF%B9%E4%BD%BF%E7%94%A8new%E5%92%8Cdelete%E6%97%B6%E8%A6%81%E9%87%87%E5%8F%96%E7%9B%B8%E5%90%8C%E5%BD%A2%E5%BC%8F)
* [17.以独立语句将newed对象置入智能指针](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Resourse%20Management.md#17%E4%BB%A5%E7%8B%AC%E7%AB%8B%E8%AF%AD%E5%8F%A5%E5%B0%86newed%E5%AF%B9%E8%B1%A1%E7%BD%AE%E5%85%A5%E6%99%BA%E8%83%BD%E6%8C%87%E9%92%88)

## 四.设计与声明



## 五.实现



## 六.继承与面向对象设计





## 七模板与泛型编程



## 八.定制 new 和 delete



## 九.杂项讨论





