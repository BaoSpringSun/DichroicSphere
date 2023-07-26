/**
 *
*/
#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>
#include <atomic>
#include <set>
#define     FUCAI3D_DATA        0
#define     PAILIE3_DATA        1

#define     DATA_KIND_IS        PAILIE3_DATA

#if(DATA_KIND_IS == PAILIE3_DATA)
    #define     PAILIE3_SOURCE_FILE                             "./pailie3/pailie3_data"
    #define     PAILIE3_DB_FILE                                 "./pailie3/pailie3.db"
#else//->FUCAI3D_DATA
    #define     PAILIE3_SOURCE_FILE                             "./pailie3/fucai3d_data"
    #define     PAILIE3_DB_FILE                                 "./pailie3/fucai3d.db"
#endif

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
    void checkDatasByUnit();
    void checkDatasByTens();
    void printVecPairIntMapVec(const vector<pair<int, vector<int>>> &vecPairIntMapVec);
    void getTheMultiDatas(const vector<int> &datas);
    void printIntMapVec(const map<int, vector<int>> &intMapVec, unsigned int step);
    void initVec2Vec(const string &dateStart, const string &dateEnd);
    void initDatas(const vector<vector<int>> &vec2Vec);
    void zuHeXuan3();
    void getMapRet(const vector<vector<int>> &vec2Vec,
                   const vector<vector<int>> &resVec2Vec,
                   map<vector<int>, vector<vector<int>>> &mapRet);
    bool checkElemsIsBeLong(const int &data, const vector<int> &resVec);
    bool getTheVec2ByData(const int &data, vector<int> &vec2);
    void printfMapRet(const map<vector<int>, vector<vector<int>>> &mapRet);
    bool checkTheVec2IsInTheVec2Vec(const vector<int> &vec2, const vector<vector<int>> &vec2Vec);
    void getTheCurrentVec2Order(const vector<vector<int>> &vec2Vec);
    void genVecVecDatasUntilNow(const string &dateStart, const string &dateEnd);
    void getNewIndexForLoop();
    void loopOperate(const string &dateEnd);

private:
    vector<vector<int>> mVec2Vec;           /*-> 带有期数的集合的集合*/
    vector<int>         mDatas;             /*-> 截止到enddate不带有期数的集合*/
    vector<vector<int>> mIndexVecVec;
    vector<vector<int>> mDatasUntilNow;     /*-> 截止到目前now的不带有期数的集合*/
    long                mIndex;
    long                mSpendMoney;
};

int sample_pailie3_main(int argc, char** argv);
