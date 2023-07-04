/**
 *
*/
#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>
#include <atomic>
#include <set>

#define     PAILIE5_SOURCE_FILE                             "./pailie5/pailie5_data"
#define     PAILIE5_DB_FILE                                 "./pailie5/pailie5.db"

class PAILIE5_C
{
public:
    explicit PAILIE5_C();
    ~PAILIE5_C();

public:
    void init();
    void operateStart();

private:
    void pailie5InitSqlData();
    void initVec2Vec(const string &dateStart, const string &dateEnd);
    void initDatas(const vector<vector<int>> &vec2Vec);
    void getTheMultiDatas(const vector<int> &datas);
    void printIntMapVec(const map<int, vector<int>> &intMapVec, unsigned int step);
    void printVecPairIntMapVec(const vector<pair<int, vector<int>>> &vecPairIntMapVec);
    void checkDatasByTenThousands();
    void checkDatasByThousands();
    void checkDatasByHundreds();
    void checkDatasByTens();

private:
    vector<vector<int>> mVec2Vec;
    vector<int>         mDatas;
};

int sample_pailie5_main(int argc, char** argv);
