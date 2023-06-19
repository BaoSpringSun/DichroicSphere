/**
 *
*/
#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <string>
#include <memory>
#include <list>
#pragma once
using namespace std;
#define QUEUE_FULL_SIZE  64
// #define     USE_SHARED_PTR

class ObserverBase
{
public:
    virtual void msgHandle(const int msgid) = 0;
};



class Zmq:public std::enable_shared_from_this<Zmq>
{
public:
    Zmq();
    ~Zmq();

public:
    void init(const shared_ptr<Zmq> &p);
    bool empty();
    bool full();
    void lock();
    void unlock();
    void wait();
    void signal();
    void pushMsg(const int msgid);
    void notifyAll(const int msgid);
    void getMsg(int &msgid);
    void printQueue();
    string& getPthreadName();

public:
    static void sendMsg(const int msgid, const void* context = nullptr);
    static void attach(const shared_ptr<ObserverBase> observer);
    static void detach(shared_ptr<ObserverBase> observer);
    static void* routine(void* arg);

private:
    queue<int> mQueues;//事件消息队列
    shared_ptr<queue<int>> mQueuesPtr;
    pthread_mutex_t mLock;
    pthread_cond_t mCond;
    pthread_t mTid;
    string mPthreadName;
    atomic<int> mCheck;

public:
    static list<shared_ptr<ObserverBase>> mObservers;
    static weak_ptr<Zmq> mSelf;
};

class ObserversBob:public ObserverBase
{
public:
    explicit ObserversBob(const char* name = "");
    ~ObserversBob();

public:
	void msgHandle(const int msgid) override;

private:
    const char* mMyName;
};

class Observers:public ObserverBase
{
public:
    explicit Observers(const char* name = "");
    ~Observers();

public:
	void msgHandle(const int msgid) override;

private:
    const char* mMyName;
};

void sample_zmq_test();
