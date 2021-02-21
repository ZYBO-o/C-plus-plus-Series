
//arctanx函数
#include<iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <functional>
#include <iterator>
#include <set>
#include <map>
#include <fstream>
#include <memory>
#include <utility>
using namespace std;

class Folder;

class Message{
    friend class Folder;   //友元类
    friend void print(const Message&);
    friend void swap(Message&, Message&);
public:
    // folders被隐式初始化为空集合。
    explicit Message(const string &str="") : contents(str) {}

    //拷贝控制函数成员，用来管理指向本Message中的指针
    Message(const Message&);
    Message& operator = (const Message&);
    ~Message();

    //从给定的Folder科技和中添加/删除本Message
    void save(Folder&);
    void remove(Folder&);

private:
    //实际消息文本
    string contents;
    //包含本Message的Folder
    set<Folder*> folders;

    //拷贝构造函数，拷贝赋值运算符和析构函数所使用到的工具函数
    //将本Message添加到指向参数的Folder中国
    void add_to_Folders(const Message&);
    //从folders中的每个Folder中删除本Message
    void remove_from_Folders();
};



class Folder
{
    friend class Message;    //友元类
    friend void print(const Folder&);
    friend void swap(Folder&, Folder&);
public:
    Folder() = default;
    Folder(const Folder&);
    Folder& operator=(const Folder&);
    ~Folder();
    void addMsg(Message*);
    void remMsg(Message*);
private:
    set<Message*> messages;
};

//=================Message函数实现=================


//给定Folder，双向添加
void Message::save(Folder &f){
    folders.insert(&f);     //将给定Folder添加到我们的Folder列表中
    f.addMsg(this);         //将本Message添加到f的Message集合中
}

//给定Folder，双向删除
void Message::remove(Folder &f){
    folders.erase(&f);      //将给定Folder从我们的Folder列表中删除
    f.remMsg(this);         //将本Message从f的Message集合中删除
}

//将本Message添加到指向m的Folder中
void Message::add_to_Folders(const Message &m)
{
    for (auto f :  m.folders) {     //对每个包含m的Folder
        f->addMsg(this);            //向该Folder添加一个指向本Message的指针
    }
}

//在本Message所属的各Folder中删除本Message
void Message::remove_from_Folders()
{
    for (auto f : folders) {
        f->remMsg(this);
    }
}


//拷贝构造函数
//1、拷贝数据成员
//2、在右侧对象所属的各Folder中添加左侧对象
Message::Message(const Message &m) : contents(m.contents), folders(m.folders)
{
    add_to_Folders(m);//将本消息添加到指向m的Folder中
}

//析构函数
//1、在本对象所属的各Folder中删除该对象
//2、释放该对象的资源
Message::~Message()
{
    remove_from_Folders();
}

//拷贝赋值算符
//1、在左侧对象所属的各Folder中删除左侧对象
//2、将右侧对象的数据成员拷贝到左侧对象
//3、在右侧对象所属的各Folder中添加左侧对象
//先删除后添加是为了处理自赋值。若先添加后删除，自赋值时最后删除会全部删掉
Message& Message::operator=(const class Message & rhs)
{
    //通过先删除指针再插入它们来处理自赋值情况
    remove_from_Folders();   //更新已有Folder
    contents = rhs.contents;
    folders = rhs.folders;
    add_to_Folders(rhs);    //将本Message添加到那些Folder中
    return *this;
}


//swap操作
//1、两对象所属的各Folder中分别删除两对象
//2、交换数据成员
//3、两对象所属的各Folder中分别添加两对象（添加的是交换前的对方）
void swap(Message &lhs, Message &rhs)
{
    using std::swap;//在本例中严格来说不需要，但是这是一个好习惯
    //将每一个消息的指针从它原来所在的Folder中删除
    for ( auto f : lhs.folders )
        f->remMsg(&lhs);
    for ( auto f : rhs.folders )
        f->remMsg(&rhs);

    //交换contents和Folder指针set
    swap(lhs.folders, rhs.folders);
    swap(lhs.contents, rhs.contents);

    //将每一个Message的指针添加到它的新的Folder中
    for ( auto f : lhs.folders )
        f->addMsg(&lhs);
    for ( auto f : rhs.folders )
        f->addMsg(&rhs);
}

void print(const Message &m)
{
    std::cout << m.contents << std::endl;
}

//=======================Folder函数实现==================
void Folder::addMsg(Message *m)
{
    messages.insert(m);
}
void Folder::remMsg(Message *m)
{
    messages.erase(m);
}
Folder::Folder(const Folder &f) :messages(f.messages)
{
    for (auto i : f.messages)
    {
        i->folders.insert(this);
    }
}
Folder::~Folder()
{
    for (auto i : messages)
    {
        i->folders.erase(this);
    }
}
Folder& Folder::operator=(const Folder &f)
{
    for (auto i : messages)
    {
        i->folders.erase(this);
    }
    for (auto i : f.messages)
    {
        i->folders.insert(this);
    }
    return *this;
}
void print(const Folder &f)
{
    for (auto i : f.messages)
        print(*i);
}
void swap(Folder &f1, Folder &f2)
{
    using std::swap;
    for (auto i : f1.messages)
        i->remove(f1);
    for (auto i : f2.messages)
        i->remove(f2);
    swap(f1.messages, f2.messages);
    for (auto i : f1.messages)
        i->save(f1);
    for (auto i : f2.messages)
        i->save(f2);
}


int main()
{
    Message m;
    
    return 0;
}



