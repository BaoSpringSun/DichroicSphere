#pragma once
#include <string>
#include "sqlite3.h"
#include <vector>
using namespace std;

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char uchar;

class sqlite_tb
{
public:
    explicit sqlite_tb(const char* dbFile = "./mydb");
    ~sqlite_tb();

public:
	bool  OpenDB();//打开数据库，不存在，创建数据库db
    bool  CloseDB();
	bool  CreateTable(const char* sqlcmd);//创建数据库表
	bool  InsertData(const string &data);//插入数据
    bool  InsertData();//插入数据
	bool  DeleteData(unsigned int date);//删除
	bool  UpdateData();//更新
	bool  SelectData(const vector<uint8> &vred, const vector<uint8> &vblue, uint32 &retcount);//查询
    void  GenRedCondition(const vector<uint8> &vec, string &cond);
    void  GenBlueCondition(const vector<uint8> &vec, string &cond);
    bool  SelectUniqueData();
    bool  SelectAllData();
    bool  SelectRepeatData();
    bool  SelectGetTotalRows();
    bool  SelectUniqueDataAmount();
    bool  SelectDistinctDataByLineName(const char *linename);
    bool  SelectDistinctDataAmountByLineName(const char *linename);

private:
    sqlite3*    mDb;
    const char* mDbFile;
};
