#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>
#include <atomic>
#include <set>
using namespace std;

#define     ONCE_INSERT_MAX_ITERM  1024          //理论一次性插入条目数最大值为500
#define     GEN_VERSION     "2023073"           //"2023073"      "2023070"    "2023035"   "2022150"    "2022115"   "2022080"
#define 	USE_TIMER 	0

#define     OBJECT_EXE_DIR              "./childBingoFinal_14/"
#define     CHECK_ALL


/**
 * SAVE6RET_DB_FILEDIR:
 * 同 (OBJECT_EXE_DIR "vec6/2023067/")
*/
#define     SAVE6RET_DB_FILEDIR         "./baseVec6/everyBaseSqlDbData/"


#ifdef CHECK_ALL//-> check result base 1528
    #define     SAVE6RET_OCCURETIME_DB_FILEDIR         "./baseVec6/occureTimeDbData_1528/"
    #define     TASK2_FINAL_RET_DB_FILE                "./baseVec6/task2FinalRetDbData_1528"
#else//-> check result from simple 700
    #define     SAVE6RET_OCCURETIME_DB_FILEDIR         "./baseVec6/occureTimeDbData/"
    #define     TASK2_FINAL_RET_DB_FILE                 "./baseVec6/task2FinalRetDbData"
#endif


/**
 * 所有6个号码的全排列组合集合    1107568
*/
#define     BASESQLDBDATA_1107568       "./baseVec6/baseSqliteDbData_1107568"

/**
 * createVec6vecFromSqlDb->生成  gbaseVec6Vec
 * BASESQLDBDATA所对应的文件就是 FINALUNIQUESET6VEC_DB所生成的文件 重命名得到
 * baseSqliteDbData_1528 为全部能覆盖1107568集合的base vec6vec 集合
 * baseSqliteDbData_700 精简后的
*/
#ifdef CHECK_ALL
    #define     BASESQLDBDATA                       "./baseVec6/baseSqliteDbData_1528"
#else
    #define     BASESQLDBDATA                       "./baseVec6/baseSqliteDbData_700"
#endif

/**
 * 寻找精简的 BASESQLDBDATA
 * 最终得到的db文件存储的vec6vec的集合就是精简后的 BASESQLDBDATA
*/
// #define     FINALVEC6VEC_DB                     OBJECT_EXE_DIR "catchSigIntSaveFinalVec6VecToDbFile"
#define     FINALVEC6VEC_DB                     "./baseVec6/catchSigIntSaveFinalVec6VecToDbFile"
#define     FINALUNIQUESET6VEC_DB               "./baseVec6/finaluniqueelemvec6_set6vec_db"


#define 	VEC_ELEMS_NUM	10
// #define     FINALSET10VEC_DB                     "./baseVec6/catchSigIntSaveFinalSet10VecToDbFile"
// #define     FINALSET10VEC_DB                     "./baseVec6/catchSigIntSaveFinalSet10VecToDbFile_8"
#define     FINALSET10VEC_DB                     "./baseVec6/findBaseSqlData5"



#define     BASESQLDBDATAFILEDIR            "./baseVec6DbData_2000"


/**
 * BINGO_DB_FILE:
 * 截止期数所对应生成的db文件~
 *
 * FINAL_RET_DB_FILE:
 * 创建最终gVecBingoVec中的每个（包含6个号码的）子集合的所代表的数据块，命中期数的统计~
 * 命中结果的排序已按照由大到小已排列好~
 *
 * SOURCE_FILE：
 * 事先先要准备好的文件，其他的都是软件运行自动生成的文件
*/
#define     SAVE8RET_DB_FILEDIR                     OBJECT_EXE_DIR "vec8/" GEN_VERSION "/"
#define     FINAL_RET_DB_FILE                       OBJECT_EXE_DIR "myretdb_" GEN_VERSION
#define     BINGO_DB_FILE                           OBJECT_EXE_DIR "mydb_" GEN_VERSION
#define     SOURCE_FILE                             "./baseVec6/sourcefile_" GEN_VERSION

template<typename T>
void freeResource(T &mVar)
{
    mVar.clear();
    T().swap(mVar);
	return;
}

class sqlite_tb
{
public:
    explicit sqlite_tb(const char* dbFile = BINGO_DB_FILE);
    ~sqlite_tb();

public:
	bool  OpenDB();//打开数据库，不存在，创建数据库db
    bool  CloseDB();
	bool  CreateTable(const char* sqlcmd);//创建数据库表
	bool  InsertData(const string &data);//插入数据
    bool  InsertDataFromSourceFile(const char *sourceFile = SOURCE_FILE);//插入数据
	bool  DeleteData(unsigned int date);//删除  按日期期数
    bool  DeleteData(const vector<int> &data);//删除  按一组值
	bool  UpdateData();//更新
	bool  SelectData(const vector<int> &vred, const vector<int> &vblue, int &retcount);//查询
    void  GenRedCondition(const vector<int> &vec, string &cond);
    void  GenBlueCondition(const vector<int> &vec, string &cond);
    bool  SelectUniqueData();
    bool  SelectAllData(vector<vector<int>> &resVec, unsigned int vecElemsNum = 6);
    bool  SelectAllData(set<set<int>> &resSetSet, unsigned int vecElemsNum = 6);
    bool  SelectAllData(set<vector<int>> &resSetVec, unsigned int vecElemsNum = 6);
    bool  SelectAllDataStoreSetVec(set<vector<int>> &resVec);
    bool  SelectRepeatData();
    bool  SelectGetTotalRows();
    bool  SelectUniqueDataAmount();
    bool  SelectDistinctDataByLineName(const char *linename);
    bool  SelectDistinctDataAmountByLineName(const char *linename);

    bool SelectDataByDate(const string &dateStart, const string &dateEnd,
                          vector<vector<int>> &resVec, unsigned int vecElemsNum);

    bool SelectResultFromVec8Db(const string &vec6elem, int &retInt);

    template <class T>
    string NumberToString(T value);
    template <class T>
    T stringToNumber(const string &str);

private:
    sqlite3*    mDb;
    const char* mDbFile;

public:
    static atomic<bool> mDebug;
};
