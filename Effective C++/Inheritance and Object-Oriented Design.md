# 继承与面向对象设计

<!-- GFM-TOC -->

* [32.确定你的public继承塑模出is-a关系](#32确定你的public继承塑模出is-a关系)
* [33.避免遮掩继承而来的名称](#33避免遮掩继承而来的名称)
* [34.区分接口继承和实现继承](#34区分接口继承和实现继承)
* [35.考虑virtual函数以外的其他选择](#35考虑virtual函数以外的其他选择)
* [36.绝不重新定义继承而来的non-virtal函数](#36绝不重新定义继承而来的non-virtal函数)
* [37.绝不重新定义继承而来的缺省参数值](#37绝不重新定义继承而来的缺省参数值)
* [38.通过符合塑模出has-a或"根据某物实现出"](#38通过符合塑模出has-a或根据某物实现出)
* [39.明智而审慎地使用private继承](#39明智而审慎地使用private继承)
* [40.明智而审慎地使用多重继承](#40明智而审慎地使用多重继承)

<!-- GFM-TOC -->

### 32.确定你的public继承塑模出is-a关系

+ **"public继承“意味着`is-a`。适用于base classes身上的每一件事情一定也适用于derived classes身上，因为每一个derived class对象也都是一个base class对象。**

这一条款是说的是公有继承的逻辑， **如果使用继承，而且继承是公有继承的话，一定要确保子类是一种父类（is-a关系）** 。这种逻辑可能与生活中的常理不相符，比如企鹅是生蛋的，所有企鹅是鸟类的一种，直观来看，我们可以用公有继承描述：

```c++
class Bird
{
public:
    virtual void fly(){cout << "it can fly." << endl;}
};

class Penguin: public Bird
{
    // fly()被继承过来了，可以覆写一个企鹅的fly()方法，也可以直接用父类的
};

int main()
{
    Penguin p;
    p.fly(); // 问题是企鹅并不会飞！
}
```

>  但问题就来了，虽然企鹅是鸟，但鸟会飞的技能并不适用于企鹅，该怎么解决这个问题呢？

+ 方法一， **在Penguin的fly()方法里面抛出异常，一旦调用了p.fly()，那么就会在运行时捕捉到这个异常。** 这个方法不怎么好，因为它要在运行时才发现问题。

+ 方法二， **去掉Bird的fly()方法，在中间加上一层FlyingBird类（有fly()方法）与NotFlyingBird类（没有fly()方法），然后让企鹅继承与NotFlyingBird类。** 这个方法也不好，因为会使注意力分散，继承的层次加深也会使代码难懂和难以维护。

+ 方法三， **保留所有Bird一定会有的共性（比如生蛋和孵化），去掉Bird的fly()方法，只在其他可以飞的鸟的子类里面单独写这个方法。** 这是一种比较好的选择，因为根本没有定义fly()方法，所以Penguin对象调用fly()会在编译期报错。

在艰难选择方法三之后， **我们回过头来思考，就是在所有public继承的背后，一定要保证父类的所有特性子类都可以满足（父类能飞，子类一定可以飞），<font color = red> 抽象起来说，就是在可以使用父类的地方，都一定可以使用子类去替换。</font>**

这正是Liskov替代原则告诉我们的：任何父类可以出现的地方，子类一定可以替代这个父类，只有当替换使软件功能不受影响时，父类才可以真正被复用。通俗地说，是 **<font color = red> “子类可以扩展父类的功能，但不能改变父类原有的功能”</font>** 。

下面再来看一个数学上的例子，对于基础几何里的长方形和正方形，老师会说“正方形是特殊的长方形”，“特殊”体现在长宽是相等的，从字面上来看，我们可以表达成is-a关系，像这样

```c++
class Rectangle
{
protected:
    int length;
    int width;
public:
    void virtual IncreaseLength(int DeltaLength)
    {
        length += DeltaLength;
    }
};

class Square: public Rectangle
{
public:
    void virtual IncreaseLength(int DeltaLength)
    {
        length += DeltaLength;
        width += DeltaLength;
    }
};
```

为了与保持正方形长宽等值的特性，不得不将IncreaseLength里面也对width进行了操作， **到这一步，恐怕成员函数名IncreaseLength已经变味了，明明是扩展长度，但“偷偷地”也把宽度给变了。这就与“子类可以扩展父类的功能，但不能改变父类原有的功能”相背离了**  ，所以这个is-a关系在数学世界里也可这样说说，但在程序的世界里并不成立！

举上面两个例子，足可以看出public继承并不像口头上说的那么容易，要去理解两个类是否可以真正成为is-a关系，还需要好好斟酌。


---

### 33.避免遮掩继承而来的名称

+ **derived classses内的名称会遮掩base classes内的名称。在public继承下从来没有人希望如此。**
+ **为了让被遮掩的名称再见天日，可使用using声明式或转交函数（forwarding functions）。**

名称的遮掩可以分成 **变量的遮掩** 与 **函数的遮掩** 两类， **本质都是名字的查找方式导致的** ，当编译器要去查找一个名字时，它一旦找到一个相符的名字，就不会再往下去找了，因此遮掩本质上是优先查找哪个名字的问题。

而查找是分作用域的，虽然本条款的命名是打着“继承”的旗子来说的，但我觉得其实与继承并不是很有关系，关键是作用域。
举例子说明这个问题会比较好理解。

```c++
//例1：普通变量遮掩
int i = 3;

int main()
{
    int i = 4;
    cout << i << endl; // 输出4
}
```

这是一个局部变量遮掩全局变量的例子，编译器在查找名字时，优先查找的是局部变量名，找到了就不会再找，所以不会有warning，不会有error，只会是这个结果。

```c++
//例2：成员变量遮掩
class Base
{
public:
    int x;
    Base(int _x):x(_x){}
};

class Derived: public Base
{
public:
    int x;
    Derived(int _x):Base(_x),x(_x + 1){}
};

int main()
{
    Derived d(3);
    cout << d.x << endl; //输出4
}
```

**因为定义的是子类的对象，所以会优先查找子类独有的作用域，这里已经找到了x，所以不会再查找父类的作用域，因此输出的是4，如果子类里没有另行声明x成员变量，那么才会去查找父类的作用域。** 那么这种情况下如果想要访问父类的x，怎么办呢？

可以在子类里面添加一个方法：

```c++
int GetBaseX() {return Base::x;}
```

利用`Base::x`，可以使查找指定为父类的作用域，这样就能返回父类的x的值了。

```c++
//例3：函数的遮掩
class Base
{
public:
    void CommonFunction(){cout << "Base::CommonFunction()" << endl;}
    void virtual VirtualFunction(){cout << "Base::VirturalFunction()" << endl;}
    void virtual PureVirtualFunction() = 0;
};

class Derived: public Base
{
public:
    void CommonFunction(){cout << "Derived::CommonFunction()" << endl;}
    void virtual VirtualFunction(){cout << "Derived::VirturalFunction()" << endl;}
    void virtual PureVirtualFunction(){cout << "Derived::PureVirtualFunction()" << endl;}
};

int main()
{
    Derived d;
    d.CommonFunction(); // Derived::CommonFunction()
    d.VirtualFunction(); // Derived::VirtualFunction()
    d.PureVirtualFunction(); // Derived::PureVirtualFunction()
    return 0;
}
```

与变量遮掩类似，函数名的查找也是先从子类独有的作用域开始查找的，一旦找到，就不再继续找下去了。这里无论是普通函数，虚函数，还是纯虚函数，结果都是输出子类的函数调用。

下面我们来一个难点的例子。

```c++
//例4：重载函数的遮掩
class Base
{
public:
    void CommonFunction(){cout << "Base::CommonFunction()" << endl;}
    void virtual VirtualFunction(){cout << "Base::VirturalFunction()" << endl;}
    void virtual VirtualFunction(int x){cout << "Base::VirtualFunction() With Parms" << endl;}
    void virtual PureVirtualFunction() = 0;
};

class Derived: public Base
{
public:
    void CommonFunction(){cout << "Derived::CommonFunction()" << endl;}
    void virtual VirtualFunction(){cout << "Derived::VirturalFunction()" << endl;}
    void virtual PureVirtualFunction(){cout << "Derived::PureVirtualFunction()" << endl;}
};

int main()
{
    Derived d;
    d.VirtualFunction(3); // ?
    return 0;
}
```

很多人都会认为输出的是`Base::VirtualFunction() With Parms`， **实际上这段代码却是编译不过的。因为编译器在查找名字时，并没有“为重载而走的很远”，C++的确是支持重载的，编译器在发现函数重载时，会去寻找相同函数名中最为匹配的一个函数（从形参个数，形参类型两个方面考虑，与返回值没有关系），如果大家的匹配程度都差不多，那么编译器会报歧义的错。**

但以上法则成立的条件是这些函数位于相同的作用域中，而这里是不同的域！编译器先查找子类独有的域，一旦发现了完全相同的函数名，它就已经不再往父类中找了！在核查函数参数时，发现了没有带整型形参，所以直接报编译错了。

如果去掉子类`VirualFunction()`，那么才会找到父类的`VirtualFunction(int)`。

提醒一下，千万不要被前面的`Virtual`关键字所误导，你可以试一个普通函数，结果是一样的，只要子类中有同名函数，不管形参是什么，编译器都不会再往父类作用域里面找了。

好，如果现在你非要访问父类里面的方法，那也可以，书上给出了两种作法:

+ **一种是采用using声明**
+ **另一种是定义转交函数。**

```c++
//例5：采用using声明，使查找范围扩大至父类指定的函数：
class Base
{
public:
    void CommonFunction(){cout << "Base::CommonFunction()" << endl;}
    void virtual VirtualFunction(){cout << "Base::VirturalFunction()" << endl;}
    void virtual VirtualFunction(int x){cout << "Base::VirtualFunction() With Parms" << endl;}
    void virtual PureVirtualFunction() = 0;
};

class Derived: public Base
{
public:
    using Base::VirtualFunction; // 第一级查找也要包括Base::VirtualFunction
    void CommonFunction(){cout << "Derived::CommonFunction()" << endl;}
    void virtual VirtualFunction(){cout << "Derived::VirturalFunction()" << endl;}
    void virtual PureVirtualFunction(){cout << "Derived::PureVirtualFunction()" << endl;}
};

int main()
{
    Derived d;
    d.VirtualFunction(3); // 这样没问题了，编译器会把父类作用域里面的函数名VirtualFunciton也纳入第一批查找范围，这样就能发现其实是父类的函数与main中的调用匹配得更好（因为有一个形参），这样会输出Base::VirtualFunction() With Parms
    return 0;
}
```

**<font color = red>用了using，实际上是告诉编译器，把父类的那个函数也纳入第一批查找范围里面，这样就能发现匹配得更好的重载函数了。</font>**



```c++
//例6：使用转交函数强制指定父类的作用域
class Base
{
public:
    void CommonFunction(){cout << "Base::CommonFunction()" << endl;}
    void virtual VirtualFunction(){cout << "Base::VirturalFunction()" << endl;}
    void virtual VirtualFunction(int x){cout << "Base::VirtualFunction() With Parms" << endl;}
    void virtual PureVirtualFunction() = 0;
};

class Derived: public Base
{
public:
    using Base::VirtualFunction;
    void CommonFunction(){cout << "Derived::CommonFunction()" << endl;}
    void virtual VirtualFunction(){cout << "Derived::VirturalFunction()" << endl;}
    void virtual PureVirtualFunction(int x){cout << "Derived::PureVirtualFunction()" << endl;}
  	//定义转交函数。
    void virtual VirtualFunction(int x){Base::VirtualFunction(x)};
};

int main()
{
    Derived d;
    d.VirtualFunction(3); // 输出Base::VirtualFunction() With Parms
    return 0;
}
```

**采用这种做法，需要在子类中再定义一个带int参的同名函数，在这个函数里面用Base进行作用域的指定，从而调用到父类的同名函数。**

快结尾了，这里还是声明一下，在不同作用域内定义相同的名字，无论是发生在变量还是函数身上，都是非常无聊也是不好的做法。除了考试试卷上，还是不要把名字遮掩问题带到任何地方，命个不同的名字真的有那么难吗？



---

### 34.区分接口继承和实现继承

+ **接口继承和实现继承不同。在public继承之下，`derived class`总是继承`base class`的接口；**

+ **`pure virtual`函数只具体<font color = red>指定接口继承</font>；**

+ **`impure virtual`函数具体<font color = red>指定接口继承和缺省实现继承</font>；**

+ **`non-virutal`函数具体<font color = red>指定接口继承以及强制性实现继承</font>。**

这个条款书上内容说的篇幅比较多，但其实思想并不复杂。只要能理解三句话即可，

+ 第一句话是： **纯虚函数只继承接口；**
+ 第二句话是： **虚函数既继承接口，也提供了一份默认实现；**
+ 第三句话是： **普通函数既继承接口，也强制继承实现。**

这里假定讨论的成员函数都是public的。

这里回顾一下这三类函数，如下：

```c++
class BaseClass
{
public:
    void virtual PureVirtualFunction() = 0; // 纯虚函数
    void virtual ImpureVirtualFunction(); // 虚函数
    void CommonFunciton(); // 普通函数
};
```

纯虚函数有一个“等于0”的声明，具体实现一般放在派生中（但基类也可以有具体实现），所在的类（称之为虚基类）是不能定义对象的，派生类中仍然也可以不实现这个纯虚函数，交由派生类的派生类实现，总之直到有一个派生类将之实现，才可以由这个派生类定义出它的对象。

**虚函数则必须有实现，否则会报链接错误。** 虚函数可以在基类和多个派生类中提供不同的版本，利用多态性质，在程序运行时动态决定执行哪一个版本的虚函数（机制是编译器生成的虚表）。virtual关键字在基类中必须显式指明，在派生类中不必指明，即使不写，也会被编译器认可为virtual函数，virtual函数存在的类可以定义实例对象。

普通函数则是将接口与实现都继承下来了，如果在派生类中重定义普通函数，将会出现名称的遮盖（见条款33），事实上，也是极不推荐在派生类中覆盖基类的普通函数的，如果真的要这样做，请一定要考虑是否该把基类的这个函数声明为虚函数或者纯虚函数。

下面是三类成员函数的应用：

```c++
class BaseClass
{
public:
    void virtual PureVirtualFunction() = 0; // 纯虚函数
    void virtual ImpureVirtualFunction(); // 虚函数
    void CommonFunciton(); // 普通函数
}; 
void BaseClass::PureVirtualFunction()
{
    cout << "Base PureVirtualFunction" << endl;
}
void BaseClass::ImpureVirtualFunction()
{
    cout << "Base ImpureVirtualFunciton" << endl;
}

class DerivedClass1: public BaseClass
{
    void PureVirtualFunction()
    {
        cout << "DerivedClass1 PureVirturalFunction Called" << endl;
    }
};

class DerivedClass2: public BaseClass
{
    void PureVirtualFunction()
    {
        cout << "DerivedClass2 PureVirturalFunction Called" << endl;
    }
};

int main()
{
    BaseClass *b1 = new DerivedClass1();
    BaseClass *b2 = new DerivedClass2();
    b1->PureVirtualFunction(); // 调用的是DerivedClass1版本的PureVirtualFunction
    b2->PureVirtualFunction(); // 调用的是DerivedClass2版本析PureVirtualFunction
    b1->BaseClass::PureVirtualFunction(); // 当然也可以调用BaseClass版本的PureVirtualFucntion
    return 0;
}
```

书上提倡用纯虚函数去替代虚函数， **因为虚函数提供了一个默认的实现，如果派生类的想要的行为与这个虚函数不一致，而又恰好忘记去覆盖虚函数，就会出现问题。<font color = red>但纯虚函数不会，因为它从语法上限定派生类必须要去实现它，否则将无法定义派生类的对象。</font>**

同时，因为纯虚函数也是可以有默认实现的（但是它从语法上强调派生类必须重定义之，否则不能定义对象），所以完全可以替换虚函数。

**普通函数所代表的意义是不变性凌驾与特异性，所以它绝不该在派生类中被重新定义。**

在设计类成员函数时，一般既不要将所有函数都声明为non-virtual（普通函数），这会使得没有余裕空间进行特化工作；也一般不要将所有函数都声明为virtual（虚函数或纯虚函数），因为一般会有一些成员函数是基类就可以决定下来的，而被所有派生类所共用的。这个设计法则并不绝对，要视实际情况来定。


---

### 35.考虑virtual函数以外的其他选择



---

### 36.绝不重新定义继承而来的non-virtal函数

+ **任何情况下都不该重新定义一个继承而来的non-virtual函数。**

```c++
class BaseClass
{
public:
    void NonVirtualFunction()
    {
        cout << "BaseClass::NonVirtualFunction" << endl;
    }
};

class DerivedClass: public BaseClass
{
public:
    void NonVirtualFunction()
    {
        cout << "DerivedClass::NonVirtualFunction" << endl;
    }
};

int main()
{
    DerivedClass d;
    BaseClass* bp = &d;
    DerivedClass* dp = &d;
    bp->NonVirtualFunction(); // 输出BaseClass::NonVirtualFunction
    dp->NonVirtualFunction(); // 输出DerivedClass::NonVirtualFunction
}
```

从输出结果可以看到一个有趣的现象， **那就是两者都是通过相同的对象d调用成员函数NonVirutalFunction，但显示结果却不相同，这会给读者带来困惑。**

**<font color = red>现在这个现象的原因是在于`BaseClass:NonVirutalFunction`与`DerivedClass:NonVirtualFunction`都是静态绑定，所以调用的non-virtual函数都是各自定义的版本。</font>**

回顾下之前的条款，如果是public继承的话，那么：

+ 适用于BaseClass的行为一定适用于DerivedClass，因为每一个DerivedClass对象都是一个BaseClass对象；

+ 如果BaseClass里面有非虚函数，那么DerivedClass一定是既继承了接口，也继承了实现；

+ 子类里面的同名函数会掩盖父类的同名函数，这是由于搜索法则导致的。

如果DerivedClass重定义一个non-virtual函数，那么会违反上面列出的法则。以第一条为例，如果子类真的要重定义这个函数，那么说明父类的这个函数不能满足子类的要求，这就与每一个子类都是父类的原则矛盾了。


---

### 37.绝不重新定义继承而来的缺省参数值

+ **绝对不要重新定义一个继承而来的缺省参数值，因为缺省参数值都是静态绑定，而virtual函数——你唯一应该覆写的东西——却是动态绑定。**

```c++
enum MyColor
{
    RED,
    GREEN,
    BLUE,
};

class Shape
{
public:
    void virtual Draw(MyColor color = RED) const = 0;
};

class Rectangle: public Shape
{
public:
    void Draw(MyColor color = GREEN) const
    {
        cout << "default color = " << color << endl;
    }
};

class Triangle : public Shape
{
public:
    void Draw(MyColor color = BLUE) const
    {
        cout << "default color = " << color << endl;
    }
};


int main()
{
    Shape *sr = new Rectangle();
    Shape *st = new Triangle();
    cout << "sr->Draw() = "; // ？
    sr->Draw();
    cout << "st->Draw() = "; // ？
    st->Draw();

    delete sr;
    delete st;
}
```

问号所在处的输出是什么？

要回答这个问题，需要回顾一下虚函数的知识，如果父类中存在有虚函数，那么编译器便会为之生成虚表与虚指针，在程序运行时，根据虚指针的指向，来决定调用哪个虚函数，这称之与动态绑定，与之相对的是静态绑定，静态绑定在编译期就决定了。

所以color=0。

**实现动态绑定的代价是比较大的，所以编译器在函数参数这部分，并没有采用动态绑定的方式，也就是说，默认的形参是静态绑定的，它是编译期就决定下来了。**

我们看下这两行代码，分析一下：

```c++
Shape *sr = new Rectangle();
Shape *st = new Triangle();
```

sr的静态类型是`Shape *` ，动态类型才是`Rectangle*` ，类似地，st的静态类型是`Shape*`，动态类型是`Triangle*`。这里没有带参数，所以使用的是默认的形参，即为静态的`Shape::Draw()`里面的缺省值RED，所以两个问题所在处的输出值都是0。

正因为编译器并没有对形参采用动态绑定，所以如果对继承而来的虚函数使用不同的缺省值，将会给读者带来极大的困惑，试想一下下面两行代码：

```c++
Shape *sr = new Rectangle(); // 默认值是RED
Rectangle *rr = new Rectangle(); // 默认值是GREEN
```

如果一定要为虚函数采用默认值，那么只要在父类中设定就可以了，可以借用条款35所说的NVI方法，像下面这样：

```c++
class Shape
{
public:
    void DrawShape(MyColor color = RED)
    {
        Draw(color);
    }
private:
    virtual void Draw(MyColor color) const = 0
    {
        cout << "Shape::Draw" << endl;
    }
};

class Rectangle: public Shape
{
private:
    void Draw(MyColor color) const
    {
        cout << "Rectangle::Draw" << endl;
    }
};

class Triangle : public Shape
{
private:
    void Draw(MyColor color) const
    {
        cout << "Triangle::Draw" << endl;
    }
};

int main()
{
    Shape *sr = new Rectangle();
    Shape *st = new Triangle();
    cout << "sr->DrawRectangle() = "; // Rectangle::Draw
    sr->DrawShape();
    cout << "st->DrawTriangle() = "; // Triangle::Draw
    st->DrawShape();
    delete sr;
    delete st;
}
```

因为前面条款已经约定non-virtual函数不会被覆写，所以这样就不用担心在子类中出现定义不同缺省形参值的问题了。



---

### 38.通过符合塑模出has-a或"根据某物实现出"

+ **复合的意义和public继承完全不同；**

+ **用域，复合意味着has-a（有一个），在实现域，复合意味着is-implemented-in-terms-of（根据某物实现出）。**

如果说public是一种is-a的关系的话，那么复合就是has-a的关系。直观来说，复合就是在一个类中采用其他类的对象作为自身的成员变量，可以举个例子，像下面这样：

```c++
class Person
{
private:
    string Name; // 复合string类型的变量
    PhoneNumber HomeNumber; // 复合PhoneNumber对象
    PhoneNumber TelephoneNumber;
};
```

我们一般会说人有名字，有家庭电话，有手机电话等，但我们一般不会说人是一个名字，或者人是一个家庭电话等。所以在这里，我们并不会去使用public继承表现出来的is-a关系，而是使用“拥有”这样的has-a关系。

复合的目的就是在Person类里面可以很自然地操作Person的属性，比如输出HomeNumber，查询HomeNumber等，这些可以调用PhoneNumber里面现成的成员函数。标准库里的string类型更是集成了丰富的成员方法，可以方便我们对Name进行各种各样的操作。

除了has-a关系外，复合还有一种含义，那就是is-implemented-in-terms-of，这说起来有些长，中文的意思是据某物实现出。举书上的例子，假定我们需要利用list来实现set，如果采用的是继承，像这样：

```c++
class MySet: public list
{
	…
}
```

那就麻烦了，还记得我们在最初讲public继承时，就说了，public继承链下有Liskov法则， **即父类存在的地方一定可以被子类所替代。这个例子里父类是list，子类是MySet，如果用MySet去替换list，那么就会有问题，因为list是支持重复元素的，如果连续执行两次list.push_back(1)，那么list里面会有两个1元素，但换成MySet，结果却只会有一个1元素（因为Set是不重复元素的集合）。** 那怎么办呢，我们既想利用list现有的特性，也不想违反可替代法则。

方法就是复合，像下面这样：

```c++
template <class T>
class MySet
{
    list<T> MyList;
    …
}
```

这样就可以放心使用list现有的方法去实现MySet的功能了，具体的示例代码如下

```c++
#include <iostream>
#include <list>
using namespace std;
template <class T>
class MySet
{
private:
    list<T> MyList;

public:
    int Size() const
    {
        return MyList.size();
    }

    bool IsContained(T Element) const
    {
        return (find(MyList.begin(), MyList.end(), T) != MyList.end());
    }

    bool Insert(T Element)
    {
        if (!IsContained(T))
        {
            MyList.push_back(Element);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Remove(T Element)
    {
        list<T>::iterator Iter = find(MyList.begin(), MyList.end(), T);
        if (Iter != MyList.end())
        {
            MyList.erase(Iter);
            return true;
        }
        else
        {
            return false;
        }
    }
};
```

好，到目前为止，大家应该能理解什么叫is-implemented-in-terms-of了吧，就是依据某物来实现，就像这里的set，就是依据于list来实现自身的结构的，这种情况下也用has-a复合模型。

---

### 39.明智而审慎地使用private继承

+ **Private继承意味着`is implemented in terms of`，它通常比复合的级别低（即优先使用复合），但是当derived class需要访问protected base class的成员，或需要重新定义继承而来的virtual函数时，这么设计是合理的。**
+ **与复合不同，private继承可以造成empty base最优化。这对致力于“对象尺寸最小化”的程序库开发者而言，可能很重要。**

private继承的意义在于“be implemented in turns of”，这个与上一条款中说的复合模型的第二层含义是相同的，这也意味着通常我们可以在这两种设计方法之间转换，但书上还是更提倡使用复合来进行类的设计。

private继承与public的继承是完全不同的，主要体现在两个地方：

+ public继承在子类中保持父类的访问权限，即父类中是public的成员函数或成员变量，在子类中仍是public，对private或者protected的成员函数或成员变量亦是如此； **但private继承则不是这样了，它破坏了父类中的访问权限标定，将之都转成private，这对子类本身并无影响（照常访问），但却影响了子类的子类，子类的子类将无法访问这些声明/定义在爷爷辈类的成员变量或成员函数。**

+ Liskov法则不再适用，也就是说“一切父类出现的地方都可以被子类所替代”的法则在private这里不成立，请看下面的例子（来源自书上）：

  ```c++
  #include <iostream>
  using namespace std;
  
  class Person{};
  
  class Student : private Person{};
  
  void eat(const Person& p){}
  
  int main()
  {
      Person p;
      Student s;
      eat(p); // OK
      eat(s); // 编译报错：error C2243: “类型转换”: 从“Student *”到“const Person &”的转换存在，但无法访问
  }
  ```

  >  但如果令Student公有继承Person，则编译器不会报错。这正是Liskov的可替代原则在private继承中不适用的体现。

private继承使用的地方实在不多，除非有一些奇葩的设计需求，书上说了一个例子：

```c++
class TypeDefine
{};

class SimpleClass
{
    int a;
    TypeDefine obj;
};

class SimpleDerivedClass : private TypeDefine
{
    int a;
};

int main()
{
    cout << sizeof(TypeDefine) << endl; // ？=> 1
    cout << sizeof(SimpleClass) << endl; // ？=> 8
    cout << sizeof(SimpleDerivedClass) << endl; // ？=> 4
}
```

大家可以想一下“ ？”处的输出是什么。

第一个是空类，空类就像是空气一样，仅仅是名字里面包含了“空”字，看起来是“空”的，但其实不是这样子的，空气里面混合了氧、氮、二氮化碳等气体，还有各种微生物，而对于空类，编译器会为之生成四个默认的函数：默认构造函数，默认拷贝构造，默认析构函数，默认赋值运算符。读者就会问了，编译器生成了默认的函数不假，但函数是不占空间的，为什么空类的sizeof算出的值是1？ **原来类的每一个对象都需要一个独一无二的内存地址，所以编译器会在空类对象中插入一个1字节变量，正是这个1字节的变量，才能够区分空类的不同对象。非空类因为已经有了成员变量，所以编译器可以利用这些成员变量来进行内存地址的区分，从而标识类的不同对象，这个时候是不需要插入一个1字节的变量的。所以第一个问号处输出的是1。**

第二个问号输出的是5吗？int四字节再加到空类对象的四字节？理论上是这样， **但编译器还会做一种内存对齐的操作，使得类对象的大小会是处理字长的整数倍，一般是4字节的整数倍，所以最后的结果其实是8。**

第三个问号呢？前面讲的那么多，好像都与private无关，这个问题终于与它有关了。运行下看看，结果是4。为什么用复合模型时输出的结果是8，但private继承时却是4呢？ **<font color = red>这其实是编译器做了空白基类优化（EBO），原本是要为空白类对象插入1字节的，但因为子类中已经有了对象了，这样理论上就可以凭借这个对象来进行同一个类不同对象间的识别了，所以这时候编译器就不再插入字节了。</font>**

这个结果就是用private继承的好处，是不是很奇葩呢~所以我说，在大部分情况下，都不会考虑private继承，因为它的含义be implemented in terms of 可以用复合来替换。

书上还提到了关于虚函数不想被子类的子类所覆写的问题，这时候不能用private限制虚函数，因为生成的虚指针是一直会被继承下去的，解决方法就是用复合，而且复合的类是一个临时类且复合对象标记为private，这样就只能限制在这个类本身去覆写了。具体的例子可以去看原书。


---

### 40.明智而审慎地使用多重继承

+ **多重继承比单一继承更复杂。它可能导致新的歧义性，以及对virtual继承的需要。**
+ **virtual继承会增加大小、速度、初始化（及赋值）复杂度等等成本。如果virtual base classes不带任何数据，将是最具实用价值的情况。**
+ **多重继承的确有正当用途。其中一个情节涉及”public继承某个Interface class”和”private继承某个协助实现的class”的两两组合。**

多重继承是一种比较复杂的继承关系，它意味着如果用户想要使用这个类，那么就要对它的父类也了如指掌，所以在项目中会带来可读性的问题，一般我们都会尽量选择用单继承去替代它。

使用多重继承过程容易碰到的问题就是名字冲突，像下面这样：

```c++
class Base1
{
public:
    void fun(){}
};

class Base2
{
private:
    void fun(){}
};

class Derived : public Base1, public Base2
{};

int main()
{
    Derived d;
    d.fun(); // error C2385: 对“fun”的访问不明确
    return 0;
}
```

因为在两个父类中都有名为fun的函数，所以这时候编译器不知道用户想调用的是哪个函数。但这里细心的读者会发现，这里我们是把Base2的fun的访问权限设为了private的。这个例子同时也说明了， **编译器会优先去查找最合适的重载函数，再去考虑它的可访问性。** <font color = red>如果真的要去访问重名的函数，可以指定作用域，像这样`d.Base1::fun()`（但注意d.Base2::fun()不行，因为它的访问性是private的）。</font>

多重继承另一个容易碰到的问题就是虚继承，我记得这还是面试官的一道面试题。试想一下，有一个父类名为A，类B和类C都继承于A，类D又同时继承了B和C（多重继承），那么如果不做任何处理，C++的类继承图里会包含两份A。

但如果在继承的时候加了virtual，像下面这样：

```c++
class B: virtual public A{…}
class C: virtual public A{…}
```

那么D中就只有一份A了。C++标准库里面的流就是采用这样的形式，有一个父流basic_ios，basic_istream和basic_ostream分别虚继承于basic_ios，而basic_iostream又多重继承于basic_istream和basic_ostream。

为了保证不会出现两份父类，只要是public继承理论上都应该有virutal关键字， **但virutal也是有代价的，访问virtual base class的成员变量要比访问non-virutal base class的成员变量速度要慢。** 所以作者的忠告是：

+ **非必要不使用virtual classes继承，普通情况请使用non-virtual classes继承**
+ **如果必须使用virtual base classes，尽可能避免在其中放置数据。**

后面的篇幅书上就举了一个多重继承的例子，在这里我就不说了，有兴趣的读者可以自己看看，但个人觉得还是能不用多重继承的时候，就尽量不用它，用复合+单继承往往能达到目的。


---

