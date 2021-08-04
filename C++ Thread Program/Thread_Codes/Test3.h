//
// Created by 张永斌 on 2021/4/20.
//

#ifndef THREAD_CODES_TEST3_H
#define THREAD_CODES_TEST3_H

#include <iostream>
#include <thread>
using namespace std;

void myprint(const int& i, char* pmybuf) {
    cout << i << endl;
    cout << pmybuf << endl;
}

void Test3() {

    int mvar = 1;
    int &mvary  = mvar;
    char mybuf[] = "this is a test";
    thread mytobj(myprint,mvar,mybuf);
    mytobj.detach();
    cout << "this is test3 function" << endl;
}

#endif //THREAD_CODES_TEST3_H
