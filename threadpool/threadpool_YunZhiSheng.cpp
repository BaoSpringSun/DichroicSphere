#include<time.h>
#include<stdlib.h>
#include"threadpool_YunZhiSheng.hpp"

void sample_threadpool_YunZhiSheng_main()
{
    ThreadPool_YunZhiSheng* ThreadPoolT = new ThreadPool_YunZhiSheng();
    ThreadPoolT->ThreadPoolInit();
    return;
}
