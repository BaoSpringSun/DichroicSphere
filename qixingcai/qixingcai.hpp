/**
 *
*/
#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>
#include <atomic>
#include <set>

#define     QIXINGCAI_SOURCE_FILE                             "./qixingca/qixingca_data"
#define     QIXINGCAI_DB_FILE                                 "./qixingca/qixingca.db"

class QIXINGCAI_C
{
public:
    explicit QIXINGCAI_C();
    ~QIXINGCAI_C();

public:
    void init();
    void operateStart();

private:
    void qixingcaInitSqlData();
    void initVec2Vec(const string &dateStart, const string &dateEnd);
    void initDatas(const vector<vector<int>> &vec2Vec);
    void getTheMultiDatas(const vector<int> &datas);
    void printIntMapVec(const map<int, vector<int>> &intMapVec, unsigned int step);
    void checkDatasByUnit();
    void checkDatasByTens();

private:
    vector<vector<int>> mVec2Vec;
    vector<vector<int>> mDatas;
};

int sample_qixingca_main(int argc, char** argv);
