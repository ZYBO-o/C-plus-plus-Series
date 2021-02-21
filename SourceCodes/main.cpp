
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
using namespace std;



class GradeBook
{
public:


    GradeBook(string name)
            :maximumGrade(0)  //定义构造函数 初始化
    {
        setCourseName(name);
    }
    void setCourseName(string name)  //定义函数设置课程名
    {
        if(name.size()<=25)  //如果课程名字节数<=25，那么课程名不变
            courseName=name;
        else  //如果课程名>25 那么取前25字节为课程名  并且输出相关字节超过信息
        {
            courseName=name.substr(0,25);
            cerr<<"Name\""<<name<<"\"exceeds maximum length(25).\n"
                <<"Limiting courseaName to first 25characters.\n"<<endl;
        }
    }
    string getCourseName()const  //定义得到程名得成员函数  返回课程名
    {
        return courseName;
    }
    void displayMessage()const  //定义显示信息成员函数  得到上某课程得显示信息
    {
        cout<<"Welcome to the grade book for\n"<<getCourseName()<<"!\n"<<endl;
    }
    void inputGrades()  //定义输入成绩成员函数  用于输入学生成绩
    {
        int grade1;
        int grade2;
        int grade3;
        cout<<"Enter three integer grades: ";
        cin>>grade1>>grade2>>grade3;  //输入三门成绩
        maximumGrade=maximum(grade1,grade2,grade3);  //求出三门成绩中最大值
    }
    int maximum(int x,int y,int z)const  //定义最大值函数，用于求出三门成绩中的最大值
    {
        int maximumValue=x;
        if(y>maximumValue)
            maximumValue=y;
        if(z>maximumValue)
            maximumValue=z;
        return maximumValue;
    }
    void displayGradeReport()const  //显示三门成绩中最大值
    {
        cout<<"Maximum of grades entered: "<<maximumGrade<<endl;
    }
private:
    std::string courseName;  //定义数据成员课程名
    int maximumGrade;  //定义数据成员 成绩最高值
};
int main()
{
    GradeBook myGradeBook("English");  //创建一个类的对象 初始化数据成员English

    myGradeBook.displayMessage();  //调用显示欢迎信息函数
    myGradeBook.inputGrades();  //调用输入成绩函数
    myGradeBook.displayGradeReport();  //调用显示成绩报告函数
}
















