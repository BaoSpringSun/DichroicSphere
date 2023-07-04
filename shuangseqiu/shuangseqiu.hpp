/**
 *
*/
#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>
#include <atomic>
#include <set>

#define     SHUANGSEQIU_SOURCE_FILE                             "./shuangseqiu/shuangseqiu_data"
#define     SHUANGSEQIU_DB_FILE                                 "./shuangseqiu/shuangseqiu.db"

class SHUANGSEQIU_C
{
public:
    explicit SHUANGSEQIU_C();
    ~SHUANGSEQIU_C();

public:
    void init();
    void operateStart();

private:
    void shuangseqiuInitSqlData();
    void initVec2Vec(const string &dateStart, const string &dateEnd);
    void initDatas(const vector<vector<int>> &vec2Vec);
template<typename T>
    void getTheMultiDatas(const vector<vector<int>> &datas, const T &units, bool yesNo = false);
    void printVecPairIntMapVec(const vector<pair<int, vector<int>>> &vecPairIntMapVec);
    bool checkVecIsInVecVec(const vector<int> &vecVec, const vector<int> &vec);
    bool checkVecIsInVecVec(const vector<vector<int>> &vecVec, const vector<int> &vec);
    void printVecPairIntMapVec(const vector<pair<string, int>> &vecPairStr2Int);
    void getTheWholeData(set<vector<int>> &setVec);

private:
    /**
     * 一期数据：(23073,15,25,28,30,35,08,10),
    */
    vector<vector<int>> mVec2Vec;//->(15,25,28,30,35,08,10),...
    vector<vector<int>> mRedDatas;//->(15,25,28,30,35,08),...
    vector<vector<int>> mBlDatas;//->(10),...
};

int sample_shuangseqiu_main(int argc, char** argv);
