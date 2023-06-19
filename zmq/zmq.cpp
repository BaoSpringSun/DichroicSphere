/**
 *
*/
#include <sys/prctl.h>
#include "zmq.hpp"


list<shared_ptr<ObserverBase>> Zmq::mObservers;
weak_ptr<Zmq> Zmq::mSelf;

Zmq::Zmq()
    :mQueues(queue<int>()) //清空队列；
    ,mQueuesPtr(new queue<int>())
    ,mTid(0)
    ,mPthreadName("zmq")
    ,mCheck(0)
{
    pthread_mutex_init(&mLock, nullptr);
    pthread_cond_init(&mCond, nullptr);
}

Zmq::~Zmq()
{
    printf("开始释放zmq~\r\n");
    pthread_mutex_destroy(&mLock);
    pthread_cond_destroy(&mCond);
}

void Zmq::init(const shared_ptr<Zmq> &p)
{
    mSelf = p;//或者用  mSelf = shared_from_this();
    pthread_create(&mTid, NULL, routine, (void*)this);
}

bool Zmq::empty()
{
    return mQueues.size()<=0?true:false;
}

bool Zmq::full()
{
    return mQueues.size()>=QUEUE_FULL_SIZE?true:false;
}

void Zmq::lock()
{
    pthread_mutex_lock(&mLock);
}

void Zmq::unlock()
{
    pthread_mutex_unlock(&mLock);
}

void Zmq::wait()
{
    pthread_cond_wait(&mCond, &mLock);
}

void Zmq::signal()
{
    pthread_cond_signal(&mCond);
}

string& Zmq::getPthreadName()
{
    return mPthreadName;
}

void Zmq::printQueue()
{
    for(int i=0;i<static_cast<int>(mQueues.size());i++)
	{
		printf("mQueues---%d\t", mQueues.front());
		mQueues.pop();
	}
    printf("\r\n");
}

void Zmq::getMsg(int &msgid)
{
    msgid = mQueues.front();
    mQueues.pop();
    return;
}

void Zmq::pushMsg(const int msgid)
{
    mQueues.push(msgid);
    return;
}

void Zmq::sendMsg(const int msgid, const void* context)
{
    if(mSelf.use_count() > 0)
    {
        shared_ptr<Zmq> self = mSelf.lock();
        self->lock();
        while(self->full())
        {
            printf("is full~\r\n");
            self->wait();
        }
        printf("sendMsg msgid=%d\r\n", msgid);
        self->pushMsg(msgid);
        self->signal();
        self->unlock();
    }
    return;
}
/**
 * 添加未注册用户
*/
void Zmq::attach(const shared_ptr<ObserverBase> observer)
{
    mObservers.push_back(observer);
}
/**
 * 删除已注册用户
*/
void Zmq::detach(shared_ptr<ObserverBase> observer)
{
    mObservers.remove(observer);
    observer.reset();//无用的智能指针置空，对象的引用计数减1； 就像普通的指针置为NUL一样，防止成为野指针；
}

void Zmq::notifyAll(const int msgid)
{
    list<shared_ptr<ObserverBase>>::iterator itor = mObservers.begin();
    for(; itor!=mObservers.end(); itor++)
    {
        (*itor)->msgHandle(msgid);
    }
    return;
}

void* Zmq::routine(void* arg)
{
    pthread_detach(pthread_self());//子线程与主线程分离独立出来；资源回收和返回结束动作完全交给自己控制；
#ifndef USE_SHARED_PTR
    Zmq* self = (Zmq*)arg;//多线程之间也可以这么用；
    prctl(PR_SET_NAME, self->getPthreadName().c_str());

    while(1)
    {
        self->lock();
        while(self->empty())
        {
            printf("is empty~\r\n");
            self->wait();
        }
        int msgId = 0;
        self->getMsg(msgId);
        self->unlock();
        self->signal();

        self->notifyAll(msgId);
    }
#else
    // printf("mSelf.use_count()=%ld--%d\r\n", mSelf.use_count(), __LINE__);
    if(mSelf.use_count() > 0)//如果这里的值小于等于0，则说明主程序已经走完，对象已经released；
    {
        shared_ptr<Zmq> self = mSelf.lock();//多线程之间需要用智能指针
        if(self.use_count() <= 0)
        {
            printf("the object is released~\r\n");
            return nullptr;
        }
        prctl(PR_SET_NAME, self->getPthreadName().c_str());

        while(1)
        {
            self->lock();
            while(self->empty())
            {
                printf("is empty~\r\n");
                self->wait();
            }
            int msgId = 0;
            self->getMsg(msgId);
            self->unlock();
            self->signal();

            self->notifyAll(msgId);
        }
    }
#endif
    return nullptr;
}

/********************************/
ObserversBob::ObserversBob(const char* name)
    :mMyName(name)
{

}
ObserversBob::~ObserversBob()
{

}
void ObserversBob::msgHandle(const int msgid)
{
    printf("%s\tget the msgid=%d\r\n", mMyName, msgid);
    switch(msgid)
    {
        default:
        break;
    }
}

Observers::Observers(const char* name)
    :mMyName(name)
{

}
Observers::~Observers()
{

}
void Observers::msgHandle(const int msgid)
{
    printf("%s\tget the msgid=%d\r\n", mMyName, msgid);
    switch(msgid)
    {
        default:
        break;
    }
}

void sample_zmq_test()
{
    shared_ptr<ObserverBase> bob = make_shared<ObserversBob>("bob");
    shared_ptr<ObserverBase> kevin = make_shared<Observers>("kevin");
    shared_ptr<ObserverBase> jone = make_shared<Observers>("jone");
    shared_ptr<ObserverBase> lili = make_shared<Observers>("lili");

    shared_ptr<Zmq> zmq_ptr = make_shared<Zmq>();
    zmq_ptr->init(zmq_ptr);
    Zmq::attach(bob);
    Zmq::attach(kevin);
    Zmq::attach(jone);
    Zmq::attach(lili);

    unsigned int msgid = 0;
    while(1)
    {
        Zmq::sendMsg(msgid);
        usleep(1000000);
        msgid++;
    }
}



