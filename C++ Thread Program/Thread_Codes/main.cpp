#include <iostream>
#include <thread>
#include <vector>
#include "Test3.h"
#include <string>

void Hello () {
    std::cout << "This is thread1" << std::endl;
}
class Obj {
public:
    int value;
    Obj(int val) : value(val) {
        std::cout << "构造函数" << std::endl;
    }
    Obj(const Obj & obj) : value(obj.value){
        std::cout << "拷贝构造函数" << std::endl;
    }
    ~Obj(){
        std::cout << "析构函数" << std::endl;
    }
    void operator () () const{
        std::cout << "The value of value1 is : " << value << std::endl;
        std::cout << "The value of value2 is : " << value << std::endl;
        std::cout << "The value of value3 is : " << value << std::endl;
        std::cout << "The value of value4 is : " << value << std::endl;
        std::cout << "The value of value5 is : " << value << std::endl;
        std::cout << "The value of value6 is : " << value << std::endl;
        std::cout << "The value of value7 is : " << value << std::endl;
    }
};
/*
 * thread为标准库类
 * join为加入阻塞，阻塞主线程，让主线程等待子线程执行完毕，然后主线程和子线程汇合
 * detach:传统多线程需要主进程等待子进程执行完毕后才可自己最后退出
 *      detach:分离，即使用detach会让主线程与子线程汇合，主线程不必再等待子线程
 *  joinable:判断是否可以成功使用join或detach
 */
int main() {

    Test3();

    /*
     * 创建线程，线程执行起点为Hello()
     * Hello线程开始执行
     */
    //std::thread t(Hello);
    /*
     * 主线程阻塞在这里，等待Hello()执行完毕，当线程执行完毕，join()执行完毕，主线程接着执行
     */
    //t.join();
    /*
     * 一旦detach，与主线程关联的Thread对象就会失去与这个主线程的关联。此时子线程会驻留在后台运行。
     * 子线程相当于被C++运行时刻接管，当子线程完毕之后由运行时库清理该线程相关资源(守护线程)
     * 一旦使用detach，就不能再进行join操作
     */
    //t.detach();

    /*
     * 其他创建线程的方法1:
     * 使用类对象
     * 如果主线程结束，那val不存在，传入的引入也不存在
     * 一旦主线程结束，此时obj已经不再，但是obj拷贝进入thread(会调用拷贝构造函数),所以备份依旧存在
     *
     * int val = 6;
     * Obj obj(val);
     * std::thread t(obj);
     * t.join();
     * std::cout<< "this is main thread " << std::endl;
     */

    /*
     * 其他创建线程的方法2：
     * 使用lambda表达式
     * auto mylambda = [] {
     *    std::cout << "This lambda thread1 start" << std::endl;
     *    std::cout << "This lambda thread1 over" << std::endl;
     * };

     * std::thread t(mylambda);
     * t.join();
     * std::cout<< "this is main thread " << std::endl;
     */



}
