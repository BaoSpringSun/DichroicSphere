/**
 *
*/
#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>
#include <atomic>
#include <set>

#define     QIXINGCAI_SOURCE_FILE                             "./qixingcai/qixingcai_data"
#define     QIXINGCAI_DB_FILE                                 "./qixingcai/qixingcai.db"

class QIXINGCAI_C
{
public:
    explicit QIXINGCAI_C();
    ~QIXINGCAI_C();

public:
    void init();
    void operateStart();

private:
    void qixingcaiInitSqlData();
    void initVec2Vec(const string &dateStart, const string &dateEnd);
    void initDatas(const vector<vector<int>> &vec2Vec);
    void getTheMultiDatas(const vector<int> &datas);
    void printIntMapVec(const map<int, vector<int>> &intMapVec, unsigned int step);
    void printVecPairIntMapVec(const vector<pair<int, vector<int>>> &vecPairIntMapVec);
    void checkDatasByTenThousands();
    void checkDatasByThousands();
    void checkDatasByHundreds();
    void checkDatasByTens();
    void checkData2s();

private:
    vector<vector<int>> mVec2Vec;//->(23074,081632,12),
    vector<int> mDatas;//->(081632),
    vector<int> mData2s;//->(12),
};

int sample_qixingcai_main(int argc, char** argv);
