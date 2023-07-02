/**
 *
*/
#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>
#include <atomic>
#include <set>

#define     PAILIE3_SOURCE_FILE                             "./pailie3/pailie3_data"
#define     PAILIE3_DB_FILE                                 "./pailie3/pailie3.db"

class PAILIE3_C
{
public:
    explicit PAILIE3_C();
    ~PAILIE3_C();

public:
    void init();
    void operateStart();

private:
    void pailie3InitSqlData();
    void initVec2Vec(const string &dateStart, const string &dateEnd);
    void initDatas(const vector<vector<int>> &vec2Vec);
    void getTheMultiDatas(const vector<int> &datas);
    void printIntMapVec(const map<int, vector<int>> &intMapVec, unsigned int step);
    void checkDatasByUnit();
    void checkDatasByTens();

private:
    vector<vector<int>> mVec2Vec;
    vector<int>         mDatas;
};

int sample_pailie3_main(int argc, char** argv);
