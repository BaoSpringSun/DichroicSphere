#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include<sstream>
#include "arrange.hpp"


bool arrange::mTimeOnOff = false;
struct timeval arrange::mTv = {0};
atomic<bool> arrange::mUsed = false;

arrange::arrange(const char* dbFile, const vector<int> oriVec, const int size)
    :mDatas("")
    ,mSql(nullptr)
    ,mOriVec(oriVec)
    ,mResSize(size)
    ,mInsertIndex(0)
{
    mSql = new sqlite_tb(dbFile);
    mResVec.assign(size, 0);
}

bool arrange::init()
{
    bool ret = false;
	// const char* sqlcmd = "create table if not exists tbldatas(index int PRIMARY key auto_increment,"
    //                         "red1 int not null default 0,red2 int not null default 0,red3 int not null default 0,"
    //                         "red4 int not null default 0,red5 int not null default 0,red6 int not null default 0);";

    const char* sqlcmd = "create table if not exists tbldatas("
                            "red1 int not null default 0,red2 int not null default 0,red3 int not null default 0,"
                            "red4 int not null default 0,red5 int not null default 0,red6 int not null default 0,"
                            "UNIQUE(red1, red2, red3, red4, red5, red6));";

    if(nullptr == mSql)
    {
        return ret;
    }
    // 打开数据库，不存在，创建数据库db
	ret = mSql->OpenDB();
    // 创建表
	ret &= mSql->CreateTable(sqlcmd);

    return ret;
}

arrange::~arrange()
{
    if(mSql)
    {
        delete mSql;
        mSql = nullptr;
    }
}

void arrange::startTime()
{
    mTimeOnOff = true;
    gettimeofday(&mTv, NULL);
}

void arrange::endTime()
{
    if(false != mTimeOnOff)
    {
        struct timeval now;
        gettimeofday(&now, NULL);
        long long msec = (now.tv_sec*1000+now.tv_usec/1000)-(mTv.tv_sec*1000+mTv.tv_usec/1000);
        printf("the spent time is %lld sec,%lld msec.\r\n", msec/1000, msec-(msec/1000)*1000);
        mTimeOnOff = false;
    }
}

void arrange::print(__int128 x)
{
	if(x<0){
        putchar('-');
        x=-x;
    }
    if(x>9)
        print(x/10);

    putchar(x%10+'0');
}

void arrange::printf__int128(__int128 x)
{
    print(x);
    printf("\r\n");
    return;
}

void arrange::sample_combination_main()
{
	int m=33, n=6;

	// printf("n的阶乘为:%lld; m的阶乘为:%lld; m和n的排列为:%ld; m和n的组合为:%ld.\r\n",
    //         factorial(n), factorial(m), Arrangement(m, n), Combination(m, n));
    printf("m的阶乘为:");
    printf__int128(factorial(m));
    printf("n的阶乘为:");
    printf__int128(factorial(n));
    printf("m和n的排列为:");
    printf__int128(Arrangement(m, n));
    printf("m和n的组合为:");
    printf__int128(Combination(m, n));
	return;
}

/**
 * 阶乘
*/
__int128 arrange::factorial(int n)
{
    __int128 ret = 0;
    if(n == 1)
    {
        return (ret = 1);
    }
    else
    {
        ret = factorial(n - 1);
        return (n * ret);
    }
}

/**
 * 排列
*/
__int128 arrange::Arrangement(int m, int n)
{
	return (factorial(m) / factorial(m-n));
}

/**
 * 组合
*/
__int128 arrange::Combination(int m, int n)
{
	return (factorial(m) / (factorial(n) * factorial(m-n)));
}


void arrange::sample_arrange_main()
{
    sample_arr_main();
    // sample_vec_main();
    sample_combination_main();
    return;
}

void arrange::sample_arr_main()
{
    int max = 0, num = 3;
    int arr[] = {32,31,29,27,15,12,8,6,3};
    int res[num] = {0};

    max = sizeof(arr)/sizeof(int);
    //组合数据
    recursionDemo(arr, max, res, num, 0, 0, num);
    printf("the total combination number=");
    printf__int128(Combination(max, num));
    return;
}

void arrange::sample_vec_main()
{
    const vector<int> oriVec{32,31,29,27,15,12,8,6,3};
    vector<int> resVec(3, 0);
    recursionDemo(oriVec, resVec, 0, 0, resVec.size());
    printf("the total combination number=");
    printf__int128(Combination(oriVec.size(), resVec.size()));
    return;
}

template <class T>
string arrange::NumberToString(T value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

template <class T>
T arrange::stringToNumber(const string str)
{
    T number {};
    /* can auto remove leading 0 */
    std::istringstream iss(str);
    iss >> number;
    return number;
}

void arrange::sqlRecursion(int start, int index)
{
    bool ret = false;
    ret = recursion(start, index, mResVec.size());
    if(false == ret)
    {
        printf("[%d]recursion error;\r\n", __LINE__);
        return;
    }

    if(false == mDatas.empty())
    {
        mDatas = mDatas.substr(0, mDatas.length()-1) + ";";//去掉“,” 加“;”
        /*
        mDatas = "(10,17,22,26,30,33),\
                    (2,06,13,22,27,32),\
                    (9,11,17,19,30,31),\
                    (01,04,06,11,12,22),\
                    (11,19,23,24,26,33);";
        */
        if(false == mSql->InsertData(mDatas))
        {
            printf("[%d]insert mDatas error:%s\r\n", __LINE__, mDatas.c_str());
        }
        mInsertIndex = 0;
        mDatas = "";
    }

#if USE_TIMER
    if(mUsed)
    {
        mSql->SelectGetTotalRows();
        mUsed = false;
    }
#endif

    return;
}


bool arrange::recursion(int start, int index, int remain)
{
    if (remain == 0)
    {//当待填充的个数为0时，表示结束

        string elemDatas = "(";
        for(const auto &elem : mResVec)
        {
            // printf("%d\t", elem);
            elemDatas += NumberToString<int>(elem) + ",";
        }
        elemDatas = elemDatas.substr(0, elemDatas.length()-1);//去掉“,”
        elemDatas = elemDatas + ")";

        if(mInsertIndex < ONCE_INSERT_MAX_ITERM)
        {
            mInsertIndex++;
            mDatas = mDatas + elemDatas + ",";
        }
        else
        {

            mDatas = mDatas + elemDatas + ";";

            printf("mInsertIndex = %d\r\n", mInsertIndex);
            if(false == mSql->InsertData(mDatas))
            {
                return false;
            }
            mInsertIndex = 0;
            mDatas = "";
        }
    }
    else
    {
        for(int i=start; i<static_cast<int>(mOriVec.size()); i++)
        {
            bool ret = false;
            //针对当前index, 遍历剩余可用的数填充递归
            mResVec[index] = mOriVec[i];
            ret = recursion(i+1, index+1, remain-1);
            if(false == ret)
            {
                return false;
            }
        }
    }

    return true;
}


void arrange::recursion( const vector<int> &oriVec,
                vector<int> resVec,
                int start, int index, int remain,
                vector<vector<int>> &resVecVec, const vector<int> &constVec)
{
    if (remain == 0)
    {
        vector<int> result;
        //当待填充的个数为0时，表示结束
        // for(const auto &elem : resVec)
        // {
        //     printf("%d\t", elem);
        // }
        // printf("\r\n");
        //合并两个容器；
        result.insert(result.end(), resVec.begin(), resVec.end());
        result.insert(result.end(), constVec.begin(), constVec.end());
        std::sort(result.begin(), result.end());//重新排序；!!

        resVecVec.push_back(result);
        // printf("resVecVec.size=%ld\r\n", resVecVec.size());
    }
    else
    {
        for(int i=start; i<static_cast<int>(oriVec.size()); i++)
        {
            //针对当前index, 遍历剩余可用的数填充递归
            resVec[index] = oriVec[i];
            recursion(oriVec, resVec, i+1, index+1, remain-1, resVecVec, constVec);
        }
    }
}


/**
 * oriArr 含有m个元素的待组合的原始数据
 * oriArrLen 待组合的原始数据所含的元素个数m
 * resArr 保存组合结果的数组---resArr是oriArr的子集
 * resArrLen 保存组合结果的数组的元素个数，方便输出
 * start 当前递归开始位置
 * index 当前要写入结果的key
 * remain 当前尚需组合(还未参与组合/剩下)的元素个数
 *
 * 数组写法
 */
void arrange::recursionDemo( int *oriArr, int oriArrLen,
                int *resArr, int resArrLen,
                int start, int index, int remain)
{
    if (remain == 0)
    {
        //当待填充的个数为0时，表示结束
        for(int i=0; i<resArrLen; i++)
        {
            printf("%d\t", resArr[i]);
        }
        printf("\n");
    }
    else
    {
        for(int i=start; i<oriArrLen; i++)
        {
            //针对当前index, 遍历剩余可用的数填充递归
            resArr[index] = oriArr[i];
            recursionDemo(oriArr, oriArrLen, resArr, resArrLen, i+1, index+1, remain-1);
        }
    }
}

/**
 * oriVec 含有m个元素的待组合的原始数据容器
 * resVec 保存组合结果的容器---resVecr是oriVec的子集
 * start 当前递归开始位置
 * index 当前要写入结果的key
 * remain 当前尚需组合(还未参与组合/剩下)的元素个数
 *
 * 容器写法
*/
void arrange::recursionDemo( const vector<int> oriVec,
                vector<int> resVec,
                int start, int index, int remain)
{
    if (remain == 0)
    {
        //当待填充的个数为0时，表示结束
        for(const auto &elem : resVec)
        {
            printf("%d\t", elem);
        }
        printf("\n");
    }
    else
    {
        for(int i=start; i<static_cast<int>(oriVec.size()); i++)
        {
            //针对当前index, 遍历剩余可用的数填充递归
            resVec[index] = oriVec[i];
            recursionDemo(oriVec, resVec, i+1, index+1, remain-1);
        }
    }
}

