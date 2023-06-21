#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>
#include <atomic>
#include <set>
using namespace std;

#define     ONCE_INSERT_MAX_ITERM  1024          //理论一次性插入条目数最大值为500
#define     GEN_VERSION     2023062                 //2022112  或者  2023062 或者2023067
#define 	USE_TIMER 	0

#define     OBJECT_EXE_DIR          "./childBingoFinal_13/"

#if(GEN_VERSION == 2023067)
#define     SAVE6RET_DB_FILEDIR     OBJECT_EXE_DIR "vec6/2023067/"
#define     SAVE8RET_DB_FILEDIR     OBJECT_EXE_DIR "vec8/2023067/"
#elif(GEN_VERSION == 2023062)
#define     SAVE6RET_DB_FILEDIR     OBJECT_EXE_DIR "vec6/2023062/"
#define     SAVE8RET_DB_FILEDIR     OBJECT_EXE_DIR "vec8/2023062/"
#else
#define     SAVE6RET_DB_FILEDIR     OBJECT_EXE_DIR "vec6/2022112/"
#define     SAVE8RET_DB_FILEDIR     OBJECT_EXE_DIR "vec8/2022112/"
#endif


/**
 * 所有6个号码的全排列组合集合    1107568
*/
#define     BASESQLDBDATA_1107568       OBJECT_EXE_DIR "baseSqliteDbData_1107568"


/**
 * createBaseSqlDb->生成gVecBingoVec；
*/
#define     BASESQLDBDATA               OBJECT_EXE_DIR "baseSqliteDbData"


/**
 *
*/
#define     FINALVEC6VEC_DB             OBJECT_EXE_DIR "catchSigIntSaveFinalVec6VecToDbFile"

/**
 * 截止期数所对应生成的db文件~
*/
#if(GEN_VERSION == 2023067)
#define     BINGO_DB_FILE     OBJECT_EXE_DIR "mydb_2023067"
#elif(GEN_VERSION == 2023062)
#define     BINGO_DB_FILE     OBJECT_EXE_DIR "mydb_2023062"
#else
#define     BINGO_DB_FILE     OBJECT_EXE_DIR "mydb_2022112"
#endif


/**
 * 创建最终gVecBingoVec中的每个（包含6个号码的）子集合的所代表的数据块，命中期数的统计~
 * 命中结果的排序已按照由大到小已排列好~
*/
#if(GEN_VERSION == 2023067)
#define     FINAL_RET_DB_FILE     OBJECT_EXE_DIR "myretdb_2023067"
#elif(GEN_VERSION == 2023062)
#define     FINAL_RET_DB_FILE     OBJECT_EXE_DIR "myretdb_2023062"
#else
#define     FINAL_RET_DB_FILE     OBJECT_EXE_DIR "myretdb_2022112"
#endif


#if(GEN_VERSION == 2023067)
#define     SOURCE_FILE     "./sourcefile_2023067"
#elif(GEN_VERSION == 2023062)
#define     SOURCE_FILE     "./sourcefile_2023062"
#else
#define     SOURCE_FILE     "./sourcefile_2022112"
#endif


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
    bool  InsertData();//插入数据
	bool  DeleteData(unsigned int date);//删除  按日期期数
    bool  DeleteData(const vector<int> &data);//删除  按一组值
	bool  UpdateData();//更新
	bool  SelectData(const vector<int> &vred, const vector<int> &vblue, int &retcount);//查询
    void  GenRedCondition(const vector<int> &vec, string &cond);
    void  GenBlueCondition(const vector<int> &vec, string &cond);
    bool  SelectUniqueData();
    bool  SelectAllData(vector<vector<int>> &resVec);
    bool  SelectAllData(set<set<int>> &resSetSet);
    bool  SelectAllDataStoreSetVec(set<vector<int>> &resVec);
    bool  SelectRepeatData();
    bool  SelectGetTotalRows();
    bool  SelectUniqueDataAmount();
    bool  SelectDistinctDataByLineName(const char *linename);
    bool  SelectDistinctDataAmountByLineName(const char *linename);

    bool SelectResultFromVec8Db(const string &vec6elem, int &retInt);

    template <class T>
    string NumberToString(T value);
    template <class T>
    T stringToNumber(const string str);

private:
    sqlite3*    mDb;
    const char* mDbFile;

public:
    static atomic<bool> mDebug;
};
