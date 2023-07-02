#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <map>
#include "sqlitetbl.hpp"
using namespace std;

class MyTask
{
public:
   explicit MyTask(const vector<int> &bingoVec);
   MyTask();
    ~MyTask();

    void run();

private:
    bool init();
    void getComplementVec(const vector<int> inputVec, vector<int> &outputVec);
    void createChild6MembersSql(const vector<int> &elemVec);
    void createChild8MembersSql();
    bool genChild8MembersDb(const vector<int> &vec8);
    void genSet8VecTotal(const vector<vector<int>> &vec8vec);
    bool findVec8RetFromMyDb(const vector<int> vec8, int &ret);
    void updateVec8RetMap(const vector<int> &vecBingo, const int &retInt);

private:
    vector<vector<int>> mVec8Vec;   //根据每期中奖的6个号，对应生成包含其的8个号的集合,size = (27*26)/(2*1) = 351;
    vector<int> mVecBingo;      //每期中奖的6个号
    string mSaveVec6RetDb;             //每期中奖的6个号所对应的8个号的集合，再生成的6个号的子集合的数据库db文件
    string mSaveVec8RetDb;         //每期中奖的6个号所对应的8个号的集合的数据库db文件
    sqlite_tb *mDb8Sql;
    int mTotalBingo;
    string mVecBingoToStrs; //如：1_2_3_14_19_33_

public:
    static sqlite_tb *mMyDb;
    static std::mutex mLock;//互斥锁
    static map<vector<int>, int> mSave8RetMap;
    static set<vector<int>> mSet8VecTotal;
};
