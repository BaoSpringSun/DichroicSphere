#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <map>
#include "sqlitetbl.hpp"
using namespace std;

class MyTask2
{
public:
   explicit MyTask2(const vector<int> &Vec, const unsigned int &index);
   MyTask2();
    ~MyTask2();

    void run();

private:
    bool init();
    bool getVec6VecFromDb();
    bool initVec6VecObjDb();
    void insertDatas();
    void updateMapVec6TotalRet();

private:
    sqlite_tb* mVec6VecObjDb;
    vector<int> mVec6;
    vector<vector<int>> mVec6Vec;   //-> 每个数据块的集合
    map<vector<int>, int>  mMapVec6Ret;
    unsigned int mOperateIndex;
    unsigned int mTotalMapSecond;

public:
    static std::mutex mLock;//互斥锁
    static multiset<vector<int>> mVec6VecAll;
    static map<vector<int>, int> mMapVec6TotalRet;
};
