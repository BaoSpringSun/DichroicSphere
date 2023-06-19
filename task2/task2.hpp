#pragma once
#include <vector>
#include <string>
#include <mutex>
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

private:
    vector<int> mVec6;
    vector<vector<int>> mVec6Vec;
    unsigned int mOperaIndex;

public:
    static std::mutex mLock;//互斥锁
    static vector<vector<int>> mVec6VecAll;
    static vector<vector<int>> mVec6VecUseFull;
	static vector<vector<int>> mVec6VecUseLess;
};



