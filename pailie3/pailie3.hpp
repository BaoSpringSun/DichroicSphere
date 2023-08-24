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

#define     DATA_KIND_IS        FUCAI3D_DATA

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
    void zuHeXuan3GetMapRet(const vector<vector<int>> &vec2Vec,
                   const vector<vector<int>> &resVec2Vec,
                   map<vector<int>, vector<vector<int>>> &mapRet);
    bool checkElemsIsBeLong(const int &data, const vector<int> &resVec);
    bool getTheVec2ByData(const int &data, vector<int> &vec2);
    void zuHeXuan3PrintfMapRet(const map<vector<int>, vector<vector<int>>> &mapRet);
    bool checkTheVec2IsInTheVec2Vec(const vector<int> &vec2, const vector<vector<int>> &vec2Vec);
    void zuHeXuan3GetTheCurrentVec2Order(const vector<vector<int>> &vec2Vec);
    void genVecVecDatasUntilNow(const string &dateStart, const string &dateEnd);
    void zuHeXuan3GetNewIndexForLoop();
    void loopOperate(const string &dateEnd);


    void zhiXuanPrintfTongJiJieGuo(const map<int, vector<vector<int>>> &mapRet);
    void zhiXuanTongJiJieGuoShengChengMap(const vector<vector<int>> &vec2Vec,
                                          const vector<int> &baseData,
                                          map<int, vector<vector<int>>> &mapRet);
    void zhiXuanOrderBaseData(const vector<vector<int>> &vec2Vec, vector<int> &baseData);
    template<typename T>
    bool checkTheElemIsInTheVec(const T &elem, const vector<T> &vec);
    void zhiXuan();
    void zhiXuanGetNewIndexForLoop();

    void zuHeXuan6();
    void zuHeXuan6OrderBaseData(const vector<vector<int>> &vec2Vec, vector<vector<int>> &baseVec);
    void zuHeXuan6TongJiJieGuoShengChengMap(const vector<vector<int>> &vec2Vec,
                                          const vector<vector<int>> &baseVec,
                                          map<vector<int>, vector<vector<int>>> &mapRet);
    void zuHeXuan6PrintfTongJiJieGuo(const map<vector<int>, vector<vector<int>>> &mapRet);
    void zuHeXuan6GetNewIndexForLoop();
    bool zuHeXuan6CheckTheDataIsRight(const int &data, vector<int> &indexVec);
    bool zuHeXuan6CheckTheLeftDataIsBelongRightData(const int &data, const vector<int> &vec);

    void checkDatasPianDuan(const vector<int> &vecDatas, int length=6);
    void checkDatasPianDuan(const vector<int> &vecDatas1, const vector<int> &vecDatas2);
    void checkDatasPianDuan();
    void analysiseVecData(const vector<int> &vecDatas);

private:
    vector<vector<int>> mVec2Vec;           /*-> 带有期数的集合的集合*/
    vector<int>         mDatas;             /*-> 截止到enddate不带有期数的集合*/
    vector<vector<int>> mIndexVecVec;
    vector<vector<int>> mDatasUntilNow;     /*-> 截止到目前now的不带有期数的集合*/
    long                mIndex;
    long                mSpendMoney;
    vector<int>         mIndexVec;
    vector<int>         mBaseVec;
    int                 mIncrease;
    int                 mBingoNum;
    vector<vector<int>> mBaseVecVec;
};

int sample_pailie3_main(int argc, char** argv);
