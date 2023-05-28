#include <stdio.h>
#include <stdlib.h>
#include<algorithm>
#include <thread>
#include <iostream>
#include <future>
using namespace std;
promise<string> val;


void thread_a()
{
    // promise<string> *val = static_cast<promise<string> *>(pval);
    // future<string> fu = val.get_future();//future 只能调用一次.get();
    // shared_future<string> fu;//shared_future可调用多次.get();
    cout << "waiting promise->future" << endl;
    while(1)
    {
        this_thread::sleep_for( chrono::milliseconds(1000) );
        future<string> fu = val.get_future();
        cout << "get:" << fu.get() << endl;//.get()是阻塞函数；
        this_thread::sleep_for( chrono::milliseconds(1000) );
    }

    cout << "over!" << endl;
    return;
}

void thread_b()
{
    // promise<string> *val = static_cast<promise<string> *>(pval);
    // this_thread::sleep_for( chrono::milliseconds(5000) );
	val.set_value("promise is set0");
    this_thread::sleep_for( chrono::milliseconds(2000) );
    val = promise<string>();//move 赋值，是prom成为新的promise对象；然后又可以继续.set_value();
    val.set_value("promise is set1");
    this_thread::sleep_for( chrono::milliseconds(2000) );
    val = promise<string>();
    val.set_value("promise is set2");
    this_thread::sleep_for( chrono::milliseconds(2000) );
    val = promise<string>();
    val.set_value("promise is set3");

    return;
}

void sample_promise_main()
{
    thread thra(thread_a/*, static_cast<void*>(&val)*/);
    thread thrb(thread_b/*, static_cast<void*>(&val)*/);

    thra.join();
    thrb.join();

    return;
}