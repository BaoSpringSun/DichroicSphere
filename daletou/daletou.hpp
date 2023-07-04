/**
 *
*/
#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>
#include <atomic>
#include <set>

#define     DALETOU_SOURCE_FILE                             "./daletou/daletou_data"
#define     DALETOU_DB_FILE                                 "./daletou/daletou.db"

class DALETOU_C
{
public:
    explicit DALETOU_C();
    ~DALETOU_C();

public:
    void init();
    void operateStart();

private:
    void daletouInitSqlData();
    void initVec2Vec(const string &dateStart, const string &dateEnd);
    void initDatas(const vector<vector<int>> &vec2Vec);
template<typename T>
    void getTheMultiDatas(const vector<vector<int>> &datas, const T &units);
    void printIntMapVec(const map<int, vector<int>> &intMapVec, unsigned int step);
    void printVecPairIntMapVec(const vector<pair<int, vector<int>>> &vecPairIntMapVec);
    bool checkVecIsInVecVec(const vector<int> &vecVec, const vector<int> &vec);
    bool checkVecIsInVecVec(const vector<vector<int>> &vecVec, const vector<int> &vec);

private:
    /**
     * 一期数据：(23073,15,25,28,30,35,08,10),
    */
    vector<vector<int>> mVec2Vec;//->(15,25,28,30,35,08,10),...
    vector<vector<int>> mYeDatas;//->(15,25,28,30,35),...
    vector<vector<int>> mBlDatas;//->(08,10),...
};

int sample_daletou_main(int argc, char** argv);
