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
#include "shuangseqiu.hpp"
using namespace std;

#define     SELECT_DATE_START                               "00001"
#define     SELECT_DATE_END                                 "23076"

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

int sample_shuangseqiu_main(int argc, char** argv)
{
    SHUANGSEQIU_C* shuangseqiu = new SHUANGSEQIU_C();
    if(nullptr != shuangseqiu)
    {
        shuangseqiu->init();
        shuangseqiu->operateStart();
        delete shuangseqiu;
        shuangseqiu = nullptr;
    }

    return 0;
}

SHUANGSEQIU_C::SHUANGSEQIU_C()
    :mVec2Vec(vector<vector<int>>())
    ,mRedDatas(vector<vector<int>>())
    ,mBlDatas(vector<vector<int>>())
{

}

SHUANGSEQIU_C::~SHUANGSEQIU_C()
{
    freeResource<vector<vector<int>>>(mVec2Vec);
    freeResource<vector<vector<int>>>(mRedDatas);
    freeResource<vector<vector<int>>>(mBlDatas);
}

void SHUANGSEQIU_C::init()
{
    shuangseqiuInitSqlData();
    initVec2Vec(SELECT_DATE_START, SELECT_DATE_END);
    return;
}

void SHUANGSEQIU_C::operateStart()
{
    return;
}

void SHUANGSEQIU_C::printVecPairIntMapVec(const vector<pair<int, vector<int>>> &vecPairIntMapVec)
{
    set<int> count = set<int>();
    vector<pair<int, int>> vecPairInt2Int = vector<pair<int, int>>();
    for(const auto &pairElem : vecPairIntMapVec)
    {
        count.insert(static_cast<int>(pairElem.second.size()));
    }

    for(const auto &elem : count)
    {
        int index = 0;
        for(const auto &pairElem : vecPairIntMapVec)
        {
            if(static_cast<int>(pairElem.second.size()) == elem)
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
        printf("[%d]:%d\r\n", elemPair.first, elemPair.second);
    }
    printf("/----------------------------/\r\n");

    freeResource<vector<pair<int, int>>>(vecPairInt2Int);
    freeResource<set<int>>(count);

    return;
}

void SHUANGSEQIU_C::printVecPairIntMapVec(const vector<pair<string, int>> &vecPairStr2Int)
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
void SHUANGSEQIU_C::getTheMultiDatas(const vector<vector<int>> &vecVec, const T &maps, bool yesNo)
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
bool SHUANGSEQIU_C::checkVecIsInVecVec(const vector<int> &vecVec, const vector<int> &vec)
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
bool SHUANGSEQIU_C::checkVecIsInVecVec(const vector<vector<int>> &vecVec, const vector<int> &vec)
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
 * init the mRedDatas and mBlDatas by mVec2Vec
*/
void SHUANGSEQIU_C::initDatas(const vector<vector<int>> &vec2Vec)
{
    string blueStr = "";
    string redStr = "redBlock";
    int operaIndex = 0;
    map<string, vector<vector<int>>> mapStr2VecVecRed = map<string, vector<vector<int>>>();
    map<string, vector<int>> mapStr2VecBl = map<string, vector<int>>();
    vector<vector<int>> vecVecBlock = vector<vector<int>>();
    set<vector<int>> setVecCheckRed = set<vector<int>>();
    set<vector<int>> setVecCheckBl = set<vector<int>>();

    for(const auto &elemVec : vec2Vec)
    {
        vector<int> tempRed(elemVec.begin(), elemVec.begin()+6);
        vector<int> tempBl(elemVec.begin()+6, elemVec.end());
        mRedDatas.push_back(tempRed);
        mBlDatas.push_back(tempBl);
        freeResource<vector<int>>(tempRed);
        freeResource<vector<int>>(tempBl);
    }

    arrange::startTime();
    printf("getTheWholeData->startTime~\r\n");
    getTheWholeData(setVecCheckRed);
    arrange::endTime();
    for(const auto &elemVec : setVecCheckRed)
    {
        vecVecBlock.push_back(elemVec);
        if(vecVecBlock.size() > 2000)
        {
            operaIndex++;
            mapStr2VecVecRed.emplace(redStr+to_string(operaIndex), vecVecBlock);
            freeResource<vector<vector<int>>>(vecVecBlock);
        }
    }
    if(vecVecBlock.size() > 0)
    {
        operaIndex++;
        mapStr2VecVecRed.emplace(redStr+to_string(operaIndex), vecVecBlock);
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
    // for(const auto &elemVec : mRedDatas)
    // {
    //     printf("[Red]elemVec.size=%ld\r\n", elemVec.size());
    // }

    arrange::startTime();
    printf("getTheMultiDatas[mRedDatas]->startTime~\r\n");
    getTheMultiDatas<map<string, vector<vector<int>>>>(mRedDatas, mapStr2VecVecRed, false);
    arrange::endTime();
    printf("/+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++/\r\n");
    printf("/-------------------------------------------------------------/\r\n");
    printf("/+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++/\r\n");
    arrange::startTime();
    printf("getTheMultiDatas[mBlDatas]->startTime~\r\n");
    getTheMultiDatas<map<string, vector<int>>>(mBlDatas, mapStr2VecBl);
    arrange::endTime();

    freeResource<set<vector<int>>>(setVecCheckRed);
    freeResource<set<vector<int>>>(setVecCheckBl);
    freeResource<map<string, vector<vector<int>>>>(mapStr2VecVecRed);
    freeResource<map<string, vector<int>>>(mapStr2VecBl);
    freeResource<string>(blueStr);
    return;
}

void SHUANGSEQIU_C::getTheWholeData(set<vector<int>> &setVec)
{
    vector<int> oriVec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                        11,12,13,14,15,16,17,18,19,20,
                        21,22,23,24,25,26,27,28,29,30,
                        31,32,33};//
	vector<int> resVec = vector<int>(6, 0);

    freeResource<set<vector<int>>>(setVec);
	arrange::recursion(oriVec, resVec, 0, 0, static_cast<int>(resVec.size()), setVec);
	printf("setVec.size=%ld\r\n", setVec.size());

    freeResource<vector<int>>(oriVec);
    freeResource<vector<int>>(resVec);
    return;
}


void SHUANGSEQIU_C::initVec2Vec(const string &dateStart, const string &dateEnd)
{
    sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(SHUANGSEQIU_DB_FILE);
    if(nullptr == sql)
	{
		printf("sql is nullptr.\r\n");
		return;
	}

    /**
	 * 打开数据库，不存在，创建数据库db
	*/
	sql->OpenDB();

    sql->SelectDataByDate(dateStart, dateEnd, mVec2Vec, 8);
    printf("mVec2Vec.size=%ld\r\n", mVec2Vec.size());

    sql->CloseDB();
	delete sql;
	sql = nullptr;

    initDatas(mVec2Vec);
	return;
}

void SHUANGSEQIU_C::shuangseqiuInitSqlData()
{
	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(SHUANGSEQIU_DB_FILE);
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
                            "red1 int not null default 0, red2 int not null default 0,"
                            "red3 int not null default 0, red4 int not null default 0, red5 int not null default 0,"
                            "red6 int not null default 0, blue1 int not null default 0);";
	sql->CreateTable(sqlcmd);
	/**
	 * 插入数据：这里默认从  文件中读取数据后导入~
	*/
	sql->InsertDataFromSourceFile(SHUANGSEQIU_SOURCE_FILE);

	/**
	 * 获取总条目数
	*/
	sql->SelectGetTotalRows();

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	return;
}


