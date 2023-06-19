#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include"task.hpp"
#include"threadpool.hpp"
using namespace std;

enum{
    THREAD_A,
    THREAD_B,
    THREAD_C,
};

class ThreadPool_YunZhiSheng
{
public:
    explicit ThreadPool_YunZhiSheng(const int num = THREAD_NUM)
        :thread_num(num)
        ,mIfExit(false)
        ,mCurThread(THREAD_A)
    {
        pthread_mutex_init(&lock,NULL);
        pthread_cond_init(&cond_A,NULL);
        pthread_cond_init(&cond_B,NULL);
        pthread_cond_init(&cond_C,NULL);
    }

    ~ThreadPool_YunZhiSheng()
    {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&cond_A);
        pthread_cond_destroy(&cond_B);
        pthread_cond_destroy(&cond_C);
    }


    static void* RoutineA(void* arg)//线程执行流
    {
        /*pthread_self()返回类型pthread_t，和 创建线程时得到的pthread_t tid是一个值；*/
        pthread_detach(pthread_self());//子线程与主线程分离

        ThreadPool_YunZhiSheng* self=(ThreadPool_YunZhiSheng*)arg;

        while(1)
        {
            self->LockQueue();
            if(self->mCurThread != THREAD_A)
            {
                self->AWait();
            }
            self->mCurThread = THREAD_B;
            self->SignalThreadB();   /**SignalThread和UnlockQueue的先后无关紧要，都可以；*/
            self->UnlockQueue();
            printf("Thread---A\r\n");
            sleep(1);
        }
    }
    static void* RoutineB(void* arg)//线程执行流
    {
        /*pthread_self()返回类型pthread_t，和 创建线程时得到的pthread_t tid是一个值；*/
        pthread_detach(pthread_self());//子线程与主线程分离

        ThreadPool_YunZhiSheng* self=(ThreadPool_YunZhiSheng*)arg;

        while(1)
        {
            self->LockQueue();
            if(self->mCurThread != THREAD_B)
            {
                self->BWait();
            }
            self->mCurThread = THREAD_C;
            self->SignalThreadC();   /**SignalThread和UnlockQueue的先后无关紧要，都可以；*/
            self->UnlockQueue();
            printf("Thread---B\r\n");
            sleep(1);
        }
    }
    static void* RoutineC(void* arg)//线程执行流
    {
        /*pthread_self()返回类型pthread_t，和 创建线程时得到的pthread_t tid是一个值；*/
        pthread_detach(pthread_self());//子线程与主线程分离

        ThreadPool_YunZhiSheng* self=(ThreadPool_YunZhiSheng*)arg;

        while(1)
        {
            self->LockQueue();
            if(self->mCurThread != THREAD_C)
            {
                self->CWait();
            }
            self->mCurThread = THREAD_A;
            self->SignalThreadA();   /**SignalThread和UnlockQueue的先后无关紧要，都可以；*/
            self->UnlockQueue();
            printf("Thread---C\r\n");
            sleep(1);
        }
    }

    void AWait()//线程等待
    {
        pthread_cond_wait(&cond_A,&lock);
    }
    void BWait()//线程等待
    {
        pthread_cond_wait(&cond_B,&lock);
    }
    void CWait()//线程等待
    {
        pthread_cond_wait(&cond_C,&lock);
    }

    void LockQueue()//锁住队列
    {
        pthread_mutex_lock(&lock);
    }
    void UnlockQueue()//解锁队列
    {
        pthread_mutex_unlock(&lock);
    }
    void SignalThreadA()//唤醒cond变量下的线程
    {
        pthread_cond_signal(&cond_A);
    }

    void SignalThreadB()//唤醒cond变量下的线程
    {
        pthread_cond_signal(&cond_B);
    }

    void SignalThreadC()//唤醒cond变量下的线程
    {
        pthread_cond_signal(&cond_C);
    }

    bool ThreadPoolInit()
    {
        pthread_t tidA, tidB, tidC;
        pthread_create(&tidA,NULL,RoutineA,(void*)this);
        pthread_create(&tidB,NULL,RoutineB,(void*)this);
        pthread_create(&tidC,NULL,RoutineC,(void*)this);
        return true;
    }

    bool getIfExit()
    {
        return mIfExit.load(std::memory_order_seq_cst);
    }

    void setIfExit(const bool val)
    {
        mIfExit.store(val, std::memory_order_seq_cst);
        return;
    }

private:
    int thread_num;//线程池的线程数量
    pthread_mutex_t lock;//互斥锁
    pthread_cond_t cond_A;//条件变量通知线程A
    pthread_cond_t cond_B;//条件变量通知线程B
    pthread_cond_t cond_C;//条件变量通知线程C
    atomic<bool> mIfExit;
    int mCurThread;
};

void sample_threadpool_YunZhiSheng_main();
