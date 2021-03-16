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

* [18.让接口容易被正确使用，不易被误用](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Designs%20and%20Declarations.md#18%E8%AE%A9%E6%8E%A5%E5%8F%A3%E5%AE%B9%E6%98%93%E8%A2%AB%E6%AD%A3%E7%A1%AE%E4%BD%BF%E7%94%A8%E4%B8%8D%E6%98%93%E8%A2%AB%E8%AF%AF%E7%94%A8)
* [19.设计class犹如设计type](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Designs%20and%20Declarations.md#19%E8%AE%BE%E8%AE%A1class%E7%8A%B9%E5%A6%82%E8%AE%BE%E8%AE%A1type)
* [20.宁以pass-by-reference-to-const替代pass-by-value](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Designs%20and%20Declarations.md#20%E5%AE%81%E4%BB%A5pass-by-reference-to-const%E6%9B%BF%E6%8D%A2pass-by-value)
* [21.必须返回对象时，别妄想返回其reference](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Designs%20and%20Declarations.md#21%E5%BF%85%E9%A1%BB%E8%BF%94%E5%9B%9E%E5%AF%B9%E8%B1%A1%E6%97%B6%E5%88%AB%E5%A6%84%E6%83%B3%E8%BF%94%E5%9B%9E%E5%85%B6reference)
* [22.将成员变量声明为private](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Designs%20and%20Declarations.md#22%E5%B0%86%E6%88%90%E5%91%98%E5%8F%98%E9%87%8F%E5%A3%B0%E6%98%8E%E4%B8%BAprivate)
* [23.宁以non-member,non-friend替换member函数](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Designs%20and%20Declarations.md#23%E5%AE%81%E4%BB%A5non-membernon-friend%E6%9B%BF%E6%8D%A2member%E5%87%BD%E6%95%B0)
* [24.若所有参数皆需类型转换，请为此采用non-member函数](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Designs%20and%20Declarations.md#24%E8%8B%A5%E6%89%80%E6%9C%89%E5%8F%82%E6%95%B0%E7%9A%86%E9%9C%80%E7%B1%BB%E5%9E%8B%E8%BD%AC%E6%8D%A2%E8%AF%B7%E4%B8%BA%E6%AD%A4%E9%87%87%E7%94%A8non-member%E5%87%BD%E6%95%B0)
* [25.考虑写一个不抛异常的swap函数](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Designs%20and%20Declarations.md#25%E8%80%83%E8%99%91%E5%86%99%E4%B8%80%E4%B8%AA%E4%B8%8D%E6%8A%9B%E5%BC%82%E5%B8%B8%E7%9A%84swap%E5%87%BD%E6%95%B0)

## 五.实现

* [26.尽可能延后变量定义式的出现时间](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Implementations.md#26%E5%B0%BD%E5%8F%AF%E8%83%BD%E5%BB%B6%E5%90%8E%E5%8F%98%E9%87%8F%E5%AE%9A%E4%B9%89%E5%BC%8F%E7%9A%84%E5%87%BA%E7%8E%B0%E6%97%B6%E9%97%B4)
* [27.尽量少做转型动作](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Implementations.md#27%E5%B0%BD%E9%87%8F%E5%B0%91%E5%81%9A%E8%BD%AC%E5%9E%8B%E5%8A%A8%E4%BD%9C)
* [28.避免返回handles指向对象内部成分](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Implementations.md#28%E9%81%BF%E5%85%8D%E8%BF%94%E5%9B%9Ehandles%E6%8C%87%E5%90%91%E5%AF%B9%E8%B1%A1%E5%86%85%E9%83%A8%E6%88%90%E5%88%86)
* [29.为"异常安全"而努力是值得的](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Implementations.md#29%E4%B8%BA%E5%BC%82%E5%B8%B8%E5%AE%89%E5%85%A8%E8%80%8C%E5%8A%AA%E5%8A%9B%E6%98%AF%E5%80%BC%E5%BE%97%E7%9A%84)
* [30.透彻了解inlining的里里外外](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Implementations.md#30%E9%80%8F%E5%BD%BB%E4%BA%86%E8%A7%A3inlining%E7%9A%84%E9%87%8C%E9%87%8C%E5%A4%96%E5%A4%96)
* [31.将文件间的编译依存关系降至最低](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Implementations.md#31%E5%B0%86%E6%96%87%E4%BB%B6%E9%97%B4%E7%9A%84%E7%BC%96%E8%AF%91%E4%BE%9D%E5%AD%98%E5%85%B3%E7%B3%BB%E9%99%8D%E8%87%B3%E6%9C%80%E4%BD%8E)

## 六.继承与面向对象设计

* [32.确定你的public继承塑模出is-a关系](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Inheritance%20and%20Object-Oriented%20Design.md#32%E7%A1%AE%E5%AE%9A%E4%BD%A0%E7%9A%84public%E7%BB%A7%E6%89%BF%E5%A1%91%E6%A8%A1%E5%87%BAis-a%E5%85%B3%E7%B3%BB)
* [33.避免遮掩继承而来的名称](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Inheritance%20and%20Object-Oriented%20Design.md#33%E9%81%BF%E5%85%8D%E9%81%AE%E6%8E%A9%E7%BB%A7%E6%89%BF%E8%80%8C%E6%9D%A5%E7%9A%84%E5%90%8D%E7%A7%B0)
* [34.区分接口继承和实现继承](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Inheritance%20and%20Object-Oriented%20Design.md#34%E5%8C%BA%E5%88%86%E6%8E%A5%E5%8F%A3%E7%BB%A7%E6%89%BF%E5%92%8C%E5%AE%9E%E7%8E%B0%E7%BB%A7%E6%89%BF)
* [35.考虑virtual函数以外的其他选择](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Inheritance%20and%20Object-Oriented%20Design.md#35%E8%80%83%E8%99%91virtual%E5%87%BD%E6%95%B0%E4%BB%A5%E5%A4%96%E7%9A%84%E5%85%B6%E4%BB%96%E9%80%89%E6%8B%A9)
* [36.绝不重新定义继承而来的non-virtal函数](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Inheritance%20and%20Object-Oriented%20Design.md#36%E7%BB%9D%E4%B8%8D%E9%87%8D%E6%96%B0%E5%AE%9A%E4%B9%89%E7%BB%A7%E6%89%BF%E8%80%8C%E6%9D%A5%E7%9A%84non-virtal%E5%87%BD%E6%95%B0)
* [37.绝不重新定义继承而来的缺省参数值](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Inheritance%20and%20Object-Oriented%20Design.md#37%E7%BB%9D%E4%B8%8D%E9%87%8D%E6%96%B0%E5%AE%9A%E4%B9%89%E7%BB%A7%E6%89%BF%E8%80%8C%E6%9D%A5%E7%9A%84%E7%BC%BA%E7%9C%81%E5%8F%82%E6%95%B0%E5%80%BC)
* [38.通过符合塑模出has-a或"根据某物实现出"](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Inheritance%20and%20Object-Oriented%20Design.md#37%E7%BB%9D%E4%B8%8D%E9%87%8D%E6%96%B0%E5%AE%9A%E4%B9%89%E7%BB%A7%E6%89%BF%E8%80%8C%E6%9D%A5%E7%9A%84%E7%BC%BA%E7%9C%81%E5%8F%82%E6%95%B0%E5%80%BC)
* [39.明智而审慎地使用private继承](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Inheritance%20and%20Object-Oriented%20Design.md#39%E6%98%8E%E6%99%BA%E8%80%8C%E5%AE%A1%E6%85%8E%E5%9C%B0%E4%BD%BF%E7%94%A8private%E7%BB%A7%E6%89%BF)
* [40.明智而审慎地使用多重继承](https://github.com/ZYBO-o/C-plus-plus-Series/blob/main/Effective%20C%2B%2B/Inheritance%20and%20Object-Oriented%20Design.md#40%E6%98%8E%E6%99%BA%E8%80%8C%E5%AE%A1%E6%85%8E%E5%9C%B0%E4%BD%BF%E7%94%A8%E5%A4%9A%E9%87%8D%E7%BB%A7%E6%89%BF)

## 七模板与泛型编程

* [41.了解隐式接口与编译期动态]()
* [42.了解typename的双重意义]()
* [43. 学习处理模板化基类内的名称]()
* [44.将与参数无关的代码抽离 templates]()
* [45. 运用成员函数模板接受所有兼容类型]()
* [46.需要类型转换时请为模板定义非成员函数]()
* [47. 请使用traits classes 表现类信息]()
* [48.认识 template 元编程]()

## 八.定制 new 和 delete



## 九.杂项讨论





