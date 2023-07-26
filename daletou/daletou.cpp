#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <set>
#include<algorithm>
#include <signal.h>
#include "sqlite3.h"
#include "sqlitetbl.hpp"
#include "promise.hpp"
#include "arrange.hpp"
#include "myjson.hpp"
#include "task.hpp"
#include "task2.hpp"
#include "threadpool.hpp"
#include <unistd.h>
#include <libgen.h>		//->basename dirname
#include"threadpool_YunZhiSheng.hpp"
#include "observer.hpp"
#include "subserver.hpp"
#include "zmq.hpp"
#include "demo.hpp"
#include "daletou.hpp"

using namespace std;

#define     SELECT_DATE_START                               "00001"
#define     SELECT_DATE_END                                 "23075"

#define     YE_BLOCK_DIR            "./daletou/yeblock/"
#define     YE_BLOCK_FILE_BASE      YE_BLOCK_DIR "yeBlock"

__attribute__((unused)) static bool exists_test2(const std::string& name)
{
    return ( access( name.c_str(), F_OK ) != -1 );
}

template<typename T>
__attribute__((unused)) static void creatVecVecResult(const T &vecVec, const char* dbname, const unsigned int bingoNums)
{
	const char* sqlVec6Cmd = "create table if not exists tbldatas("
                            "red1 int not null default 0,red2 int not null default 0,red3 int not null default 0,"
                            "red4 int not null default 0,red5 int not null default 0,red6 int not null default 0,"
                            "UNIQUE(red1, red2, red3, red4, red5, red6));";

	const char* sqlVec10Cmd = "create table if not exists tbldatas("
                            "red1 int not null default 0,red2 int not null default 0,red3 int not null default 0,"
                            "red4 int not null default 0,red5 int not null default 0,red6 int not null default 0,"
							"red7 int not null default 0,red8 int not null default 0,red9 int not null default 0,red10 int not null default 0,"
                            "UNIQUE(red1, red2, red3, red4, red5, red6, red7, red8, red9, red10));";

	const char* sqlVec7Cmd = "create table if not exists tbldatas("
                            "red1 int not null default 0,red2 int not null default 0,red3 int not null default 0,"
                            "red4 int not null default 0,red5 int not null default 0,red6 int not null default 0,"
							"red7 int not null default 0,"
                            "UNIQUE(red1, red2, red3, red4, red5, red6, red7));";

	const char* sqlVec5Cmd = "create table if not exists tbldatas("
                            "yellow1 int not null default 0,yellow2 int not null default 0,yellow3 int not null default 0,"
                            "yellow4 int not null default 0,yellow5 int not null default 0,"
                            "UNIQUE(yellow1, yellow2, yellow3, yellow4, yellow5));";

	sqlite_tb *sql = nullptr;
	string datas = "";
	unsigned int insertIndex = 0;
	sql = new sqlite_tb(dbname);
	if(nullptr == sql)
	{
		printf("sql is nullptr.\r\n");
		return;
	}
    // 打开数据库，不存在，创建数据库db
	sql->OpenDB();

	if(bingoNums == 6)
	{
		sql->CreateTable(sqlVec6Cmd);
	}
	else if(bingoNums == 10)
	{
		sql->CreateTable(sqlVec10Cmd);
	}
	else if(bingoNums == 7)
	{
		sql->CreateTable(sqlVec7Cmd);
	}
	else if(bingoNums == 5)
	{
		sql->CreateTable(sqlVec5Cmd);
	}
	else
	{
		printf("error: the input para bingoNums is error~%d\r\n", bingoNums);
		sql->CloseDB();
		delete sql;
		sql = nullptr;
		return;
	}


	// printf("vecVec.size=%ld\r\n", vecVec.size());
	if(vecVec.size() <= 0)
	{
		printf("vecVec.size()=0~\r\n");
		sql->CloseDB();
		delete sql;
		sql = nullptr;
		return;
	}
	for(const auto &elemVec:vecVec)
	{
		string elemDatas = "(";
		for(const auto &elem:elemVec)
		{
			elemDatas += arrange::NumberToString<int>(elem) + ",";
		}
		elemDatas = elemDatas.substr(0, elemDatas.length()-1);//去掉“,”
		elemDatas = elemDatas + ")";

		if(insertIndex < ONCE_INSERT_MAX_ITERM)
        {
            insertIndex++;
            datas = datas + elemDatas + ",";
        }
        else
        {
            datas = datas + elemDatas + ";";
            sql->InsertData(datas);
            insertIndex = 0;
            datas = "";
        }
	}

	if(false == datas.empty())
    {
        datas = datas.substr(0, datas.length()-1) + ";";//去掉“,” 加“;”
        sql->InsertData(datas);
        insertIndex = 0;
        datas = "";
    }

	sql->SelectGetTotalRows();

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	return;
}

__attribute__((unused)) static bool defineCmpRule(const pair<int, vector<int>> &a,
						  const pair<int, vector<int>> &b)
{
	if(a.second.size() != b.second.size())
	{
		return a.second.size() > b.second.size();
	}
	else
	{
		return a.first < b.first;
	}
}

__attribute__((unused)) static bool toUp(const int &a, const int &b)
{
    return a < b;
}

__attribute__((unused)) static bool sortVec_ToDown(const pair<int, int> &a, const pair<int, int> &b)
{
    if(a.second != b.second)
    {
        return a.second > b.second;
    }
    else
    {
        return a.first < b.first;
    }
}

__attribute__((unused)) static bool sortVec_ToDown2(const pair<string, int> &a, const pair<string, int> &b)
{
    if(a.second != b.second)
    {
        return a.second > b.second;
    }
    else
    {
        return a.first < b.first;
    }
}

__attribute__((unused)) static bool sortVec_ToDown3(const pair<string, vector<vector<int>>> &a,
                                                    const pair<string, vector<vector<int>>> &b)
{
    if(a.second.size() != b.second.size())
    {
        return a.second.size() > b.second.size();
    }
    else
    {
        return a.first < b.first;
    }
}

int sample_daletou_main(int argc, char** argv)
{
    DALETOU_C* daletou = new DALETOU_C();
    if(nullptr != daletou)
    {
        daletou->init();
        daletou->operateStart();
        delete daletou;
        daletou = nullptr;
    }

    return 0;
}

DALETOU_C::DALETOU_C()
    :mVec2Vec(vector<vector<int>>())
    ,mYeDatas(vector<vector<int>>())
    ,mBlDatas(vector<vector<int>>())
{

}

DALETOU_C::~DALETOU_C()
{
    freeResource<vector<vector<int>>>(mVec2Vec);
    freeResource<vector<vector<int>>>(mYeDatas);
    freeResource<vector<vector<int>>>(mBlDatas);
}

void DALETOU_C::init()
{
    if(!exists_test2(YE_BLOCK_DIR))
	{
		system("mkdir " YE_BLOCK_DIR " -p");
	}
    daletouInitSqlData();
    initVec2Vec(SELECT_DATE_START, SELECT_DATE_END);
    return;
}

void DALETOU_C::operateStart()
{
    return;
}

void DALETOU_C::printVecPairIntMapVec(const vector<pair<string, int>> &vecPairStr2Int)
{
    set<int> count = set<int>();
    vector<pair<int, int>> vecPairInt2Int = vector<pair<int, int>>();
    for(const auto &pairElem : vecPairStr2Int)
    {
        count.insert(pairElem.second);
    }

    for(const auto &elem : count)
    {
        int index = 0;
        for(const auto &pairElem : vecPairStr2Int)
        {
            if(pairElem.second == elem)
            {
                index++;
            }
        }
        vecPairInt2Int.push_back(make_pair(elem, index));
    }
    sort(vecPairInt2Int.begin(), vecPairInt2Int.end(), sortVec_ToDown);
    printf("/----------------------------/\r\n");
    for(const auto &elemPair : vecPairInt2Int)
    {
        printf("\t%d (个) [%d]\r\n", elemPair.second, elemPair.first);
    }
    printf("/----------------------------/\r\n");

    freeResource<vector<pair<int, int>>>(vecPairInt2Int);
    freeResource<set<int>>(count);

    return;
}

template<typename T>
void DALETOU_C::getTheMultiDatas(const vector<vector<int>> &vecVec, const T &maps, bool yesNo)
{
    map<string, int> mapStr2Int = map<string, int>();
    vector<pair<string, vector<vector<int>>>> vecPairStrMapVecVec = vector<pair<string, vector<vector<int>>>>();
    vector<pair<string, int>> vecPairStr2Int = vector<pair<string, int>>();
    for(const auto &pairs:maps)
    {
        int index = 0;
        vector<vector<int>> temp = vector<vector<int>>();
        for(const auto &elemVec:vecVec)
        {
            if(false != checkVecIsInVecVec(pairs.second, elemVec))
            {
                index++;
                temp.push_back(elemVec);
            }
        }
        mapStr2Int.emplace(pairs.first, index);
        if(yesNo)
        {
            vecPairStrMapVecVec.push_back(make_pair(pairs.first, temp));
        }
    }

    vecPairStr2Int.insert(vecPairStr2Int.end(), mapStr2Int.begin(), mapStr2Int.end());
    sort(vecPairStr2Int.begin(), vecPairStr2Int.end(), sortVec_ToDown2);

    printf("/*****************************/\r\n");
    for(const auto &elemPair:vecPairStr2Int)
    {
        printf("(%s):\t[%d]\r\n", elemPair.first.c_str(), elemPair.second);
    }
    printf("/*****************************/\r\n");
    printVecPairIntMapVec(vecPairStr2Int);

    if(yesNo)
    {
        sort(vecPairStrMapVecVec.begin(), vecPairStrMapVecVec.end(), sortVec_ToDown3);
        for(const auto &pairElem : vecPairStrMapVecVec)
        {
            printf("(%s):\t[%ld]\r\n", pairElem.first.c_str(), pairElem.second.size());
            for(const auto &elemVec : pairElem.second)
            {
                printf("\t");
                for(const auto &elem : elemVec)
                {
                    printf("%d_", elem);
                }
                printf("\r\n");
            }
            printf("\t\r\n");
        }
    }

    freeResource<vector<pair<string, vector<vector<int>>>>>(vecPairStrMapVecVec);
    freeResource<map<string, int>>(mapStr2Int);
    freeResource<vector<pair<string, int>>>(vecPairStr2Int);
    return;
}

/**
 * 确认vec 和vecVec是否相等~
*/
bool DALETOU_C::checkVecIsInVecVec(const vector<int> &vecVec, const vector<int> &vec)
{
    bool ret = false;
    if(vecVec == vec)
    {
        ret = true;
    }
    return ret;
}

/**
 * 确保vecVec里面的元素都是唯一的
 * 确认vec在不在vecVec里面~
*/
bool DALETOU_C::checkVecIsInVecVec(const vector<vector<int>> &vecVec, const vector<int> &vec)
{
    bool ret = false;
    for(const auto &elemVec : vecVec)
    {
        if(elemVec == vec)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

/**
 * init the mYeDatas and mBlDatas by mVec2Vec
*/
void DALETOU_C::initDatas(const vector<vector<int>> &vec2Vec)
{
    string blueStr = "";
    string yellowStr = YE_BLOCK_FILE_BASE;
    int operaIndex = 0;
    vector<vector<int>> vecVecBlock = vector<vector<int>>();
    set<vector<int>> setVecCheckYe = set<vector<int>>();
    set<vector<int>> setVecCheckBl = set<vector<int>>();
    map<string, vector<vector<int>>> mapStr2VecVecYe = map<string, vector<vector<int>>>();
    map<string, vector<int>> mapStr2VecBl = map<string, vector<int>>();
    for(const auto &elemVec : vec2Vec)
    {
        vector<int> tempYe(elemVec.begin(), elemVec.begin()+5);
        vector<int> tempBl(elemVec.begin()+5, elemVec.end());
        mYeDatas.push_back(tempYe);
        mBlDatas.push_back(tempBl);
        freeResource<vector<int>>(tempYe);
        freeResource<vector<int>>(tempBl);
    }

    arrange::startTime();
    printf("getTheWholeData->startTime~\r\n");
    getTheWholeData(setVecCheckYe);
    arrange::endTime();
    for(const auto &elemVec : setVecCheckYe)
    {
        vecVecBlock.push_back(elemVec);
        if(vecVecBlock.size() > 1000)
        {
            operaIndex++;
            mapStr2VecVecYe.emplace(yellowStr+to_string(operaIndex), vecVecBlock);
            // creatVecVecResult<vector<vector<int>>>(vecVecBlock, (yellowStr+to_string(operaIndex)).c_str(), 5);
            freeResource<vector<vector<int>>>(vecVecBlock);
        }
    }
    if(vecVecBlock.size() > 0)
    {
        operaIndex++;
        mapStr2VecVecYe.emplace(yellowStr+to_string(operaIndex), vecVecBlock);
        freeResource<vector<vector<int>>>(vecVecBlock);
    }

    setVecCheckBl.insert(mBlDatas.begin(), mBlDatas.end());
    printf("[Bl]setVecCheckBl.size=%ld\r\n", setVecCheckBl.size());
    for(const auto &elemVec : setVecCheckBl)
    {
        for(const auto &elem : elemVec)
        {
            blueStr = blueStr + " " + to_string(elem);
        }
        blueStr = blueStr + " ";
        // printf("[Bl] = %s\r\n", blueStr.c_str());
        mapStr2VecBl.emplace(blueStr, elemVec);
        freeResource<string>(blueStr);
        blueStr = "";
    }
    // for(const auto &elemVec : mYeDatas)
    // {
    //     printf("[Ye]elemVec.size=%ld\r\n", elemVec.size());
    // }

    arrange::startTime();
    printf("getTheMultiDatas[mYeDatas]->startTime~\r\n");
    getTheMultiDatas<map<string, vector<vector<int>>>>(mYeDatas, mapStr2VecVecYe, false);
    arrange::endTime();
    printf("/+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++/\r\n");
    printf("/-------------------------------------------------------------/\r\n");
    printf("/+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++/\r\n");
    arrange::startTime();
    printf("getTheMultiDatas[mBlDatas]->startTime~\r\n");
    getTheMultiDatas<map<string, vector<int>>>(mBlDatas, mapStr2VecBl);
    arrange::endTime();

    freeResource<set<vector<int>>>(setVecCheckYe);
    freeResource<set<vector<int>>>(setVecCheckBl);
    freeResource<map<string, vector<vector<int>>>>(mapStr2VecVecYe);
    freeResource<map<string, vector<int>>>(mapStr2VecBl);
    freeResource<string>(blueStr);
    return;
}

void DALETOU_C::getTheWholeData(set<vector<int>> &setVec)
{
    vector<int> oriVec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                        11,12,13,14,15,16,17,18,19,20,
                        21,22,23,24,25,26,27,28,29,30,
                        31,32,33,34,35};//
	vector<int> resVec = vector<int>(5, 0);

    freeResource<set<vector<int>>>(setVec);
	arrange::recursion(oriVec, resVec, 0, 0, static_cast<int>(resVec.size()), setVec);
	printf("setVec.size=%ld\r\n", setVec.size());

    freeResource<vector<int>>(oriVec);
    freeResource<vector<int>>(resVec);
    return;
}


void DALETOU_C::initVec2Vec(const string &dateStart, const string &dateEnd)
{
    sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(DALETOU_DB_FILE);
    if(nullptr == sql)
	{
		printf("sql is nullptr.\r\n");
		return;
	}

    /**
	 * 打开数据库，不存在，创建数据库db
	*/
	sql->OpenDB();

    sql->SelectDataByDate(dateStart, dateEnd, mVec2Vec, 7);
    printf("mVec2Vec.size=%ld\r\n", mVec2Vec.size());

    sql->CloseDB();
	delete sql;
	sql = nullptr;

    initDatas(mVec2Vec);
	return;
}

void DALETOU_C::daletouInitSqlData()
{
	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(DALETOU_DB_FILE);
	if(nullptr == sql)
	{
		printf("sql is nullptr.\r\n");
		return;
	}

    /**
	 * 打开数据库，不存在，创建数据库db
	*/
	sql->OpenDB();
	/**
	 * 创建表
	*/
	const char* sqlcmd = "create table if not exists tbldatas(date int PRIMARY key not null default 2000,"
                            "yellow1 int not null default 0, yellow2 int not null default 0,"
                            "yellow3 int not null default 0, yellow4 int not null default 0, yellow5 int not null default 0,"
                            "blue1 int not null default 0, blue2 int not null default 0);";
	sql->CreateTable(sqlcmd);
	/**
	 * 插入数据：这里默认从  文件中读取数据后导入~
	*/
	sql->InsertDataFromSourceFile(DALETOU_SOURCE_FILE);

	/**
	 * 获取总条目数
	*/
	sql->SelectGetTotalRows();

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	return;
}


