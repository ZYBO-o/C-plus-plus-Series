# 一.定义抽象数据类型

### 1.基本概念

```c++
struct Sales_data{
    string isbn () const { return bookNo; }
    Sales_data combine (const Sales_data&);
    double avg_price () const;

    string bookNo;
    unsigned unit_sold = 0;
    double revenue = 0.0;
};

double Sales_data::avg_price() const {
    if (unit_sold)
        return revenue/unit_sold;
    else
        return 0;
}

Sales_data Sales_data::combine(const struct Sales_data &rhs) {
    unit_sold += rhs.unit_sold;
    revenue += rhs.revenue;
    return *this;
}

int main(){
    Sales_data total;
    total.revenue = 100;
    total.unit_sold = 10;
    Sales_data trans;
    trans.revenue = 20;
    trans.unit_sold = 2;
    total.combine(trans);
    cout << total.revenue << " " << total.unit_sold << endl;
}
```



+ **引入this：**
  + 成员函数通过this这个额外的隐式参数来访问调用它的对象。
  + this是一个常量指针，不允许改变this中保存的值。

+ **引入const成员函数：**
  + `double Sales_data::avg_price() const `通过把const放在成员函数的参数列表之后，来修改隐式this的类型为指向常量的常量指针。这种使用const的成员函数称为常量成员函数。

+ **类作用域与成员函数：**