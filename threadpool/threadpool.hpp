#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include"task.hpp"
using namespace std;

#define QUEUE_MAX_SIZE  10
#define THREAD_NUM  20

template<class T>

class ThreadPool
{
public:
    explicit ThreadPool(const int num = THREAD_NUM)
        :thread_num(num)
        ,mIfExit(false)
    {
        pthread_mutex_init(&lock,NULL);
        pthread_cond_init(&cond,NULL);
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&cond);
    }


    static void* Routine(void* arg)//线程执行流
    {
        /*pthread_self()返回类型pthread_t，和 创建线程时得到的pthread_t tid是一个值；*/
        pthread_detach(pthread_self());//子线程与主线程分离

        ThreadPool* self=(ThreadPool*)arg;

        while(1)
        {
            self->LockQueue();
            while(self->Empty())//任务队列是否为空
            {
                if(false != self->getIfExit())//线程关闭决断
                {
                    printf("return [%ld]\r\n", pthread_self());
                    self->SignalThread();   /**SignalThread和UnlockQueue的先后无关紧要，都可以；*/
                    self->UnlockQueue();
                    return nullptr;
                }
                printf("queue is empty.\r\n");
                self->Wait();
            }
            T data;//T data();
            self->Pop(data);//取出任务
            self->SignalThread();   /**SignalThread和UnlockQueue的先后无关紧要，都可以；*/
            self->UnlockQueue();

            //处理任务
            data.run();//处理任务
            // sleep(1);  //因为data.run()是耗时任务，所以不需要sleep();  如果是很快完成的，则可视情况添加sleep；
        }
    }

    bool Empty()
    {
        return q.size()==0?true:false;
    }

    void Wait()//线程等待
    {
        pthread_cond_wait(&cond,&lock);
    }

    void LockQueue()//锁住队列
    {
        pthread_mutex_lock(&lock);
    }
    void UnlockQueue()//解锁队列
    {
        pthread_mutex_unlock(&lock);
    }
    void SignalThread()//唤醒cond变量下的线程
    {
        pthread_cond_signal(&cond);
    }

    bool ThreadPoolInit()
    {
        for(int i=0;i<thread_num;i++)//创建线程池中的线程
        {
            pthread_t tid;
            pthread_create(&tid,NULL,Routine,(void*)this);
        }
        return true;
    }

    void Push(const T& in)//将任务推进任务队列中
    {
        LockQueue();
        while(q.size() > QUEUE_MAX_SIZE)
        {
            printf("queue is full.\r\n");
            Wait();
        }
        q.push(in);
        UnlockQueue();
        SignalThread();
    }
    void Pop( T& out)//取出任务队列中的任务
    {
        out=q.front();
        q.pop();
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
    queue<T> q;//任务队列
    int thread_num;//线程池的线程数量
    pthread_mutex_t lock;//互斥锁
    pthread_cond_t cond;//条件变量
    atomic<bool> mIfExit;
};


