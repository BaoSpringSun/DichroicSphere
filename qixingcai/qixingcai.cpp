/**
 * 低概率，且很难统计推测结果~
*/
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
#include "qixingcai.hpp"
using namespace std;

#define     SELECT_DATE_START                               "00001"
#define     SELECT_DATE_END                                 "23056"

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

int sample_qixingcai_main(int argc, char** argv)
{
    QIXINGCAI_C* qixingcai = new QIXINGCAI_C();
    if(nullptr != qixingcai)
    {
        qixingcai->init();
        qixingcai->operateStart();
        delete qixingcai;
        qixingcai = nullptr;
    }

    return 0;
}

QIXINGCAI_C::QIXINGCAI_C()
    :mVec2Vec(vector<vector<int>>())
    ,mDatas(vector<int>())
    ,mData2s(vector<int>())
{

}

QIXINGCAI_C::~QIXINGCAI_C()
{
    freeResource<vector<vector<int>>>(mVec2Vec);
    freeResource<vector<int>>(mDatas);
    freeResource<vector<int>>(mData2s);
}

void QIXINGCAI_C::init()
{
    qixingcaiInitSqlData();
    initVec2Vec(SELECT_DATE_START, SELECT_DATE_END);
    return;
}

void QIXINGCAI_C::operateStart()
{
    // checkDatasByTenThousands();
    checkDatasByThousands();
    // checkDatasByHundreds();
    // checkDatasByTens();
    checkData2s();
    return;
}

void QIXINGCAI_C::printVecPairIntMapVec(const vector<pair<int, vector<int>>> &vecPairIntMapVec)
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
    printf("----------------------------\r\n");
    for(const auto &elemPair : vecPairInt2Int)
    {
        printf("\t%d (个) [%d]\r\n", elemPair.second, elemPair.first);
    }
    printf("----------------------------\r\n");

    freeResource<vector<pair<int, int>>>(vecPairInt2Int);
    freeResource<set<int>>(count);

    return;
}

void QIXINGCAI_C::printIntMapVec(const map<int, vector<int>> &intMapVec, unsigned int step)
{
    vector<pair<int, vector<int>>> vecPairIntMapVec(intMapVec.begin(), intMapVec.end());
	sort(vecPairIntMapVec.begin(), vecPairIntMapVec.end(), defineCmpRule);

    printf("****************************\r\n");
    for(const auto &pairElem : vecPairIntMapVec)
    {
        printf("\t%5d:\t[%ld]\r\n", pairElem.first*step, pairElem.second.size());
    }
    printf("****************************\r\n");
    printVecPairIntMapVec(vecPairIntMapVec);

	// for(const auto &pairElem : vecPairIntMapVec)
	// {
	// 	printf("%5d:\t%ld\r\n", pairElem.first*step, pairElem.second.size());
	// 	for(const auto &elem : pairElem.second)
    //     {
    //         printf("\t%d_\r\n", elem);
    //     }printf("\r\n");
	// }

    freeResource<vector<pair<int, vector<int>>>>(vecPairIntMapVec);
    return;
}

void QIXINGCAI_C::checkData2s()
{
    map<int, int> intMapInt = map<int, int>();
    vector<pair<int, int>> vecPairInt2Int = vector<pair<int, int>>();
    for(int i=0;i<15;i++)
    {
        int temp = 0;
        for(const auto &elem:mData2s)
        {
            if(elem == i)
            {
                temp++;
            }
        }
        intMapInt.emplace(i, temp);
    }
    vecPairInt2Int.insert(vecPairInt2Int.end(), intMapInt.begin(), intMapInt.end());
    sort(vecPairInt2Int.begin(), vecPairInt2Int.end(), sortVec_ToDown);
    printf("++++++++++++++++++++++++++++\r\n");
    for(const auto &elemPair : vecPairInt2Int)
    {
        printf("[%d]:%d\r\n", elemPair.first, elemPair.second);
    }
    printf("++++++++++++++++++++++++++++\r\n");

    freeResource<vector<pair<int, int>>>(vecPairInt2Int);
    freeResource<map<int, int>>(intMapInt);
    return;
}

void QIXINGCAI_C::checkDatasByTenThousands()
{
    map<int, vector<int>> intMapVec = map<int, vector<int>>();
    for(int i=0;i<100;i++)
    {
        vector<int> temp = vector<int>();
        for(const auto &elem:mDatas)
        {
            if(elem/10000 == i)
            {
                temp.push_back(elem);
            }
        }
        intMapVec.emplace(i, temp);
        freeResource<vector<int>>(temp);
    }

    printIntMapVec(intMapVec, 10000);
    freeResource<map<int, vector<int>>>(intMapVec);
    return;
}

void QIXINGCAI_C::checkDatasByThousands()
{
    map<int, vector<int>> intMapVec = map<int, vector<int>>();
    for(int i=0;i<1000;i++)
    {
        vector<int> temp = vector<int>();
        for(const auto &elem:mDatas)
        {
            if(elem/1000 == i)
            {
                temp.push_back(elem);
            }
        }
        intMapVec.emplace(i, temp);
        freeResource<vector<int>>(temp);
    }

    printIntMapVec(intMapVec, 1000);
    freeResource<map<int, vector<int>>>(intMapVec);
    return;
}

void QIXINGCAI_C::checkDatasByHundreds()
{
    map<int, vector<int>> intMapVec = map<int, vector<int>>();
    for(int i=0;i<10000;i++)
    {
        vector<int> temp = vector<int>();
        for(const auto &elem:mDatas)
        {
            if(elem/100 == i)
            {
                temp.push_back(elem);
            }
        }
        intMapVec.emplace(i, temp);
        freeResource<vector<int>>(temp);
    }

    printIntMapVec(intMapVec, 100);
    freeResource<map<int, vector<int>>>(intMapVec);
    return;
}

void QIXINGCAI_C::checkDatasByTens()
{
    map<int, vector<int>> intMapVec = map<int, vector<int>>();
    for(int i=0;i<100000;i++)
    {
        vector<int> temp = vector<int>();
        for(const auto &elem:mDatas)
        {
            if(elem/10 == i)
            {
                temp.push_back(elem);
            }
        }
        intMapVec.emplace(i, temp);
        freeResource<vector<int>>(temp);
    }

    printIntMapVec(intMapVec, 10);
    freeResource<map<int, vector<int>>>(intMapVec);
    return;
}

void QIXINGCAI_C::getTheMultiDatas(const vector<int> &datas)
{
    set<pair<int, int>> setPairint2int = set<pair<int, int>>();
    vector<pair<int, int>> vecPairint2int = vector<pair<int, int>>();
    vector<int> multiVecDatas = vector<int>();
    multiset<int> multiSetDatas(datas.begin(), datas.end());
    vector<int> vecDatas(datas.begin(), datas.end());
    sort(vecDatas.begin(), vecDatas.end(), toUp);
    auto itor = unique(vecDatas.begin(), vecDatas.end());
    /**
     * 剩下的都是唯一的元素；
     * 不能用a.erase(a.begin(), iter);来找重复的元素，不能不能！！！
    */
    vecDatas.erase(itor, vecDatas.end());
    printf("vecDatas.size=%ld\r\n", vecDatas.size());
    for(const auto &data:vecDatas)
    {
        auto itor = multiSetDatas.find(data);
        if(itor != multiSetDatas.end())
        {
            multiSetDatas.erase(itor);//->只会删除容器中查找到的第一个data
        }
        // multiSetDatas.erase(data);//->会删除容器中所有的data
    }
    printf("multiSetDatas.size=%ld\r\n", multiSetDatas.size());
    for(const auto &data:multiSetDatas)
    {
        int count = static_cast<int>(multiSetDatas.count(data));
        // printf("%d_%d\r\n", data, count);
        setPairint2int.insert(make_pair(data, count));
    }

    vecPairint2int.insert(vecPairint2int.end(), setPairint2int.begin(), setPairint2int.end());
    sort(vecPairint2int.begin(), vecPairint2int.end(), sortVec_ToDown);
    for(const auto &elemPair:vecPairint2int)
    {
        printf("%d-%d\r\n", elemPair.first, elemPair.second);
    }
    freeResource<vector<pair<int, int>>>(vecPairint2int);
    freeResource<set<pair<int, int>>>(setPairint2int);
    freeResource<vector<int>>(vecDatas);
    freeResource<vector<int>>(multiVecDatas);
    freeResource<multiset<int>>(multiSetDatas);

    // set<int> multiDatas = set<int>();
    // set<int> setDatas(datas.begin(), datas.end());
    // multiset<int> mulsetDatas(datas.begin(), datas.end());
    // printf("setDatas.size=%ld, mulsetDatas.size=%ld\r\n", setDatas.size(), mulsetDatas.size());
    // for(const auto &data:datas)
    // {
    //     if(mulsetDatas.count(data)>=2)
    //     {
    //         if(mulsetDatas.count(data)>=3)
    //             printf("%d---%ld\r\n", data, mulsetDatas.count(data));
    //         multiDatas.insert(data);
    //     }
    // }
    // printf("multiDatas.size=%ld\r\n", multiDatas.size());
    // freeResource<set<int>>(multiDatas);
    // freeResource<set<int>>(setDatas);
    // freeResource<multiset<int>>(mulsetDatas);
    return;
}

void QIXINGCAI_C::initDatas(const vector<vector<int>> &vec2Vec)
{
    for(const auto &elemVec2:vec2Vec)
    {
        if(elemVec2.size() < 3)
        {
            printf("error[%d]:the input paras vec2Vec is wrong~\r\n", __LINE__);
            break;
        }
        mDatas.push_back(elemVec2[1]);
        mData2s.push_back(elemVec2[2]);
    }

    if(mDatas.size() != vec2Vec.size())
    {
        printf("error[%d]:the input paras vec2Vec is wrong~\r\n", __LINE__);
    }
    printf("mDatas.size=%ld\r\n", mDatas.size());

    printf("****************************\r\n");
    getTheMultiDatas(mDatas);
    printf("****************************\r\n");
    printf("++++++++++++++++++++++++++++\r\n");
    getTheMultiDatas(mData2s);
    printf("++++++++++++++++++++++++++++\r\n");
    return;
}


void QIXINGCAI_C::initVec2Vec(const string &dateStart, const string &dateEnd)
{
    sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(QIXINGCAI_DB_FILE);
    if(nullptr == sql)
	{
		printf("sql is nullptr.\r\n");
		return;
	}

    /**
	 * 打开数据库，不存在，创建数据库db
	*/
	sql->OpenDB();

    sql->SelectDataByDate(dateStart, dateEnd, mVec2Vec, 3);
    printf("mVec2Vec.size=%ld\r\n", mVec2Vec.size());

    sql->CloseDB();
	delete sql;
	sql = nullptr;

    initDatas(mVec2Vec);
	return;
}

void QIXINGCAI_C::qixingcaiInitSqlData()
{
	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(QIXINGCAI_DB_FILE);
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
                            "data int not null default 0, data1 int not null default 0);";
	sql->CreateTable(sqlcmd);
	/**
	 * 插入数据：这里默认从  文件中读取数据后导入~
	*/
	sql->InsertDataFromSourceFile(QIXINGCAI_SOURCE_FILE);

	/**
	 * 获取总条目数
	*/
	sql->SelectGetTotalRows();

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	return;
}


