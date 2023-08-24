/**
 * 较高概率，还是可以统计推测出结果~
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
#include "pailie3.hpp"
using namespace std;

#define         SELECT_DATA_THRESHOLD              20
#if(DATA_KIND_IS == PAILIE3_DATA)
    #define     SELECT_DATE_START                               "04021"
    #define     SELECT_DATE_END                                 "23165"//"23001"
    #define     SELECT_DATE_NOW                                 "23195"
#else//->FUCAI3D_DATA
    #define     SELECT_DATE_START                               "2004021"
    #define     SELECT_DATE_END                                 "2023165"
    #define     SELECT_DATE_NOW                                 "2023195"
#endif

__attribute__((unused)) static bool selfDefineSort4(const pair<int, vector<int>> &a,
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

__attribute__((unused)) static bool selfDefineSort5(const pair<int, vector<vector<int>>> &a,
						                          const pair<int, vector<vector<int>>> &b)
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

__attribute__((unused)) static bool toDown(const int &a, const int &b)
{
    return a > b;
}

__attribute__((unused)) static bool vecVecToUp(const vector<int> &a, const vector<int> &b)
{
    return a[0] < b[0];
}

__attribute__((unused)) static bool vecVecToDown(const vector<int> &a, const vector<int> &b)
{
    return a[0] > b[0];
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

__attribute__((unused)) static bool selfDefineSort(const vector<int> &a, const vector<int> &b)
{
    if((a.size() == b.size())&&(a.size() == 2))
    {
        return (a[0] > b[0]);
    }
    else
    {
        return false;
    }
}

__attribute__((unused)) static bool selfDefineSort2(const pair<vector<int>, vector<vector<int>>> &a,
                                                    const pair<vector<int>, vector<vector<int>>> &b)
{
    return (a.second.size() > b.second.size());
}

__attribute__((unused)) static bool selfDefineSort3(const pair<int, vector<vector<int>>> &a,
						                            const pair<int, vector<vector<int>>> &b)
{
	if(a.second.size() != b.second.size())
	{
		return a.second.size() > b.second.size();
	}
	else
	{
		return a.first > b.first;
	}
}

int sample_pailie3_main(int argc, char** argv)
{
    PAILIE3_C* pailie3 = new PAILIE3_C();
    if(nullptr != pailie3)
    {
        pailie3->init();
        pailie3->operateStart();
        delete pailie3;
        pailie3 = nullptr;
    }

    return 0;
}

PAILIE3_C::PAILIE3_C()
    :mVec2Vec(vector<vector<int>>())
    ,mDatas(vector<int>())
    ,mIndexVecVec(vector<vector<int>>())
    ,mDatasUntilNow(vector<vector<int>>())
    ,mIndex(0)
    ,mSpendMoney(0)
    ,mIndexVec(vector<int>())
    ,mBaseVec(vector<int>())
    ,mIncrease(0)
    ,mBingoNum(0)
    ,mBaseVecVec(vector<vector<int>>())
{

}

PAILIE3_C::~PAILIE3_C()
{
    freeResource<vector<vector<int>>>(mVec2Vec);
    freeResource<vector<int>>(mDatas);
    freeResource<vector<vector<int>>>(mIndexVecVec);
    freeResource<vector<vector<int>>>(mDatasUntilNow);
    freeResource<vector<int>>(mIndexVec);
    freeResource<vector<int>>(mBaseVec);
    freeResource<vector<vector<int>>>(mBaseVecVec);
}

void PAILIE3_C::init()
{
    pailie3InitSqlData();
    genVecVecDatasUntilNow(SELECT_DATE_START, SELECT_DATE_NOW);
    initVec2Vec(SELECT_DATE_START, SELECT_DATE_END);

    return;
}

void PAILIE3_C::operateStart()
{
    checkDatasByUnit();
    // checkDatasByTens();
    // zuHeXuan3();
    // zhiXuan();
    // zuHeXuan6();
    // checkDatasPianDuan();
    return;
}


void PAILIE3_C::loopOperate(const string &dateEnd)
{
    string strDateNow = SELECT_DATE_NOW;
    if(dateEnd >= strDateNow)
    {
        freeResource<string>(strDateNow);
        return;
    }
    freeResource<string>(strDateNow);
    initVec2Vec(SELECT_DATE_START, dateEnd);
    // zuHeXuan3();
    // zhiXuan();
    zuHeXuan6();
    return;
}

void PAILIE3_C::printVecPairIntMapVec(const vector<pair<int, vector<int>>> &vecPairIntMapVec)
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

void PAILIE3_C::printIntMapVec(const map<int, vector<int>> &intMapVec, unsigned int step)
{
    vector<pair<int, vector<int>>>vecPairIntMapVec(intMapVec.begin(), intMapVec.end());
	sort(vecPairIntMapVec.begin(), vecPairIntMapVec.end(), selfDefineSort4);

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
    //     }printf("\t\r\n");
	// }

    freeResource<vector<pair<int, vector<int>>>>(vecPairIntMapVec);
    return;
}

void PAILIE3_C::checkDatasByTens()
{
    map<int, vector<int>> intMapVec = map<int, vector<int>>();
    for(int i=0;i<100;i++)
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

void PAILIE3_C::checkDatasByUnit()
{
    map<int, vector<int>> intMapVec = map<int, vector<int>>();
    for(int i=0;i<1000;i++)
    {
        vector<int> temp = vector<int>();
        for(const auto &elem:mDatas)
        {
            if(elem == i)
            {
                temp.push_back(elem);
            }
        }
        intMapVec.emplace(i, temp);
        freeResource<vector<int>>(temp);
    }

    printIntMapVec(intMapVec, 1);
    freeResource<map<int, vector<int>>>(intMapVec);
    return;
}

void PAILIE3_C::getTheMultiDatas(const vector<int> &datas)
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

void PAILIE3_C::initDatas(const vector<vector<int>> &vec2Vec)
{
    freeResource<vector<int>>(mDatas);
    for(const auto &elemVec2:vec2Vec)
    {
        if(elemVec2.size() < 2)
        {
            printf("error[%d]:the input paras vec2Vec is wrong~\r\n", __LINE__);
            break;
        }
        mDatas.push_back(elemVec2[1]);
    }

    if(mDatas.size() != vec2Vec.size())
    {
        printf("error[%d]:the input paras vec2Vec is wrong~\r\n", __LINE__);
    }
    printf("mDatas.size=%ld\r\n", mDatas.size());
    mIndex = mDatas.size()-1;

    // getTheMultiDatas(mDatas);
    return;
}

void PAILIE3_C::checkDatasPianDuan(const vector<int> &vecDatas, int length)
{
    int index1 = 0, index2 = 0;
    vector<int>::const_iterator itor1 = vecDatas.begin();
    for(; (itor1+length)!=vecDatas.end(); itor1++,index1++)
    {
        vector<int> tmp1(itor1, itor1+length);
        index2 = 0;
        vector<int>::const_iterator itor2 = vecDatas.begin();
        for(; (itor2+length)!=vecDatas.end(); itor2++,index2++)
        {
            vector<int> tmp2(itor2, itor2+length);
            if((tmp1 == tmp2)&&(index1 < index2))
            {
                printf("找到相同片段(length=%d)[%d]=[%d],间隔:%d\r\n", length, index1, index2, (index2-index1));
            }
            freeResource<vector<int>>(tmp2);
        }
        freeResource<vector<int>>(tmp1);
    }

    return;
}

void PAILIE3_C::checkDatasPianDuan(const vector<int> &vecDatas1, const vector<int> &vecDatas2)
{
    int length = 6;
    int index1 = 0, index2 = 0;
    vector<int>::const_iterator itor1 = vecDatas1.begin();
    for(; (itor1+length)!=vecDatas1.end(); itor1++,index1++)
    {
        vector<int> tmp1(itor1, itor1+length);
        index2 = 0;
        vector<int>::const_iterator itor2 = vecDatas2.begin();
        for(; (itor2+length)!=vecDatas2.end(); itor2++,index2++)
        {
            vector<int> tmp2(itor2, itor2+length);
            if((tmp1 == tmp2)&&(index1 < index2))
            {
                printf("找到相同片段(length=%d)[%d]=[%d],间隔:%d\r\n", length, index1, index2, (index2-index1));
            }
            freeResource<vector<int>>(tmp2);
        }
        freeResource<vector<int>>(tmp1);
    }

    return;
}


void PAILIE3_C::analysiseVecData(const vector<int> &vecDatas)
{
    vector<pair<int,int>> tmp = vector<pair<int,int>>();
    int checkRet = 0;
    printf("***********************************\r\n");
    for(int i=0;i<10;i++)
    {
        int count = 0;
        for(const auto &elem : vecDatas)
        {
            if(elem == i)
            {
                count++;
            }
        }
        tmp.push_back(make_pair(i, count));
        printf("\t统计[%d]有%d个~\r\n", i, count);
    }

    sort(tmp.begin(), tmp.end(), sortVec_ToDown);
    for(const auto &elemPair : tmp)
    {
        printf("\t[%d]:%d\r\n", elemPair.first, elemPair.second);
        checkRet += elemPair.second;
    }
    freeResource<vector<pair<int,int>>>(tmp);
    printf("***********************************%d\r\n", checkRet);
    return;
}

/**
 * bai : data/100;
 * shi : (data/10)%10
 * ge : data%10
*/
void PAILIE3_C::checkDatasPianDuan()
{
    vector<int> geVec = vector<int>();
    vector<int> shiVec = vector<int>();
    vector<int> baiVec = vector<int>();

    for(const auto &elem : mDatas)
    {
        geVec.push_back(elem%10);
        shiVec.push_back((elem/10)%10);
        baiVec.push_back(elem/100);
    }
    printf("checkDatasPianDuan(mDatas)\r\n");
    checkDatasPianDuan(mDatas, 2);
    printf("\r\n");
    printf("checkDatasPianDuan(geVec)\r\n");
    checkDatasPianDuan(geVec);
    printf("\r\n");
    printf("checkDatasPianDuan(shiVec)\r\n");
    checkDatasPianDuan(shiVec);
    printf("\r\n");
    printf("checkDatasPianDuan(baiVec)\r\n");
    checkDatasPianDuan(baiVec);
    printf("\r\n");
    printf("checkDatasPianDuan(geVec, shiVec)\r\n");
    checkDatasPianDuan(geVec, shiVec);
    printf("\r\n");
    printf("checkDatasPianDuan(geVec, baiVec)\r\n");
    checkDatasPianDuan(geVec, baiVec);
    printf("\r\n");
    printf("checkDatasPianDuan(shiVec, baiVec)\r\n");
    checkDatasPianDuan(shiVec, baiVec);
    printf("\r\n");
    printf("analysiseVecData(geVec)\r\n");
    analysiseVecData(geVec);
    printf("\r\n");
    printf("analysiseVecData(shiVec)\r\n");
    analysiseVecData(shiVec);
    printf("\r\n");
    printf("analysiseVecData(baiVec)\r\n");
    analysiseVecData(baiVec);

    freeResource<vector<int>>(geVec);
    freeResource<vector<int>>(shiVec);
    freeResource<vector<int>>(baiVec);

    return;
}

void PAILIE3_C::genVecVecDatasUntilNow(const string &dateStart, const string &dateEnd)
{
    sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(PAILIE3_DB_FILE);
    if(nullptr == sql)
	{
		printf("sql is nullptr.\r\n");
		return;
	}

    /**
	 * 打开数据库，不存在，创建数据库db
	*/
	sql->OpenDB();

    sql->SelectDataByDate(dateStart, dateEnd, mDatasUntilNow, 2);
    sort(mDatasUntilNow.begin(), mDatasUntilNow.end(), vecVecToUp);
    printf("mDatasUntilNow.size=%ld\r\n", mDatasUntilNow.size());

    sql->CloseDB();
	delete sql;
	sql = nullptr;

    // for(const auto &elemVec : mDatasUntilNow)
    // {
    //     printf("\t");
    //     for(const auto &elem : elemVec)
    //     {
    //         printf("%d ", elem);
    //     }
    //     printf("\r\n");
    // }

	return;
}

void PAILIE3_C::initVec2Vec(const string &dateStart, const string &dateEnd)
{
    sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(PAILIE3_DB_FILE);
    if(nullptr == sql)
	{
		printf("sql is nullptr.\r\n");
		return;
	}

    /**
	 * 打开数据库，不存在，创建数据库db
	*/
	sql->OpenDB();

    sql->SelectDataByDate(dateStart, dateEnd, mVec2Vec, 2);
    printf("mVec2Vec.size=%ld\r\n", mVec2Vec.size());

    sql->CloseDB();
	delete sql;
	sql = nullptr;

    initDatas(mVec2Vec);
	return;
}

void PAILIE3_C::pailie3InitSqlData()
{
	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(PAILIE3_DB_FILE);
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
                            "data int not null default 0);";
	sql->CreateTable(sqlcmd);
	/**
	 * 插入数据：这里默认从  文件中读取数据后导入~
	*/
	sql->InsertDataFromSourceFile(PAILIE3_SOURCE_FILE);

	/**
	 * 获取总条目数
	*/
	sql->SelectGetTotalRows();

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	return;
}

template<typename T>
bool PAILIE3_C::checkTheElemIsInTheVec(const T &elem, const vector<T> &vec)
{
    if(0 == vec.size())
    {
        return false;
    }

    auto itor = find(vec.begin(), vec.end(), elem);
    if(itor != vec.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PAILIE3_C::zhiXuanOrderBaseData(const vector<vector<int>> &vec2Vec, vector<int> &baseVec)
{
    freeResource<vector<int>>(baseVec);
    for(const auto &elemVec : vec2Vec)
    {
        if(elemVec.size() >= 2)
        {
            bool ret = checkTheElemIsInTheVec<int>(elemVec[1], baseVec);
            if(false == ret)
            {//不在
                baseVec.push_back(elemVec[1]);
            }
            if(baseVec.size() >= 1000)
            {
                break;
            }
        }
    }
    // printf("根据最新的期数号码(有%ld个)得到最新的基数的排序为:\r\n", baseVec.size());
    // int ic = 0;
    // for(const auto &elem : baseVec)
    // {
    //     printf("\t(%d)", elem);
    //     if(ic++ > 8)
    //     {
    //         ic = 0;
    //         printf("\r\n");
    //     }
    // }
    // printf("\r\n");

    return;
}

void PAILIE3_C::zhiXuanTongJiJieGuoShengChengMap(const vector<vector<int>> &vec2Vec,
                                          const vector<int> &baseVec,
                                          map<int, vector<vector<int>>> &mapRet)
{
    for(const auto &elem : baseVec)
    {
        vector<vector<int>> tmp = vector<vector<int>>();
        for(const auto &elemVec : vec2Vec)
        {
            if(elemVec.size() >= 2)
            {
                if(elem == elemVec[1])
                {
                    tmp.push_back(elemVec);
                }
            }
        }
        sort(tmp.begin(), tmp.end());
        mapRet.emplace(elem, tmp);
        freeResource<vector<vector<int>>>(tmp);
    }
    return;
}

void PAILIE3_C::zhiXuanPrintfTongJiJieGuo(const map<int, vector<vector<int>>> &mapRet)
{
    vector<pair<int, vector<vector<int>>>> vecPair(mapRet.begin(), mapRet.end());
    set<int> cishuSet = set<int>();
    map<int, vector<int>> mapIntToVec = map<int, vector<int>>();
    sort(vecPair.begin(), vecPair.end(), selfDefineSort3);
    for(const auto &elemPair : vecPair)
    {
        set<int> intervalSet = set<int>();
        int index = 0;
        // printf("  (%d)出现了[%ld]次:\r\n", elemPair.first, elemPair.second.size());
        cishuSet.insert(static_cast<int>(elemPair.second.size()));
        for(const auto &elemVec : elemPair.second)
        {
            if(index)
            {
                intervalSet.insert(elemVec[0]-index);
                index = elemVec[0];
            }
            else
            {
                index = elemVec[0];
            }

            // printf("\t索引");
            // for(const auto &elem : elemVec)
            // {
            //     printf("%d_", elem);
            // }
            // printf("\r\n");
        }

        // printf("\t间隔值分别为:");
        // for(const auto &elem : intervalSet)
        // {
        //     printf("%d ", elem);
        // }
        // printf("\r\n");
        freeResource<set<int>>(intervalSet);
    }

    /**
     * 根据出现的次数判断结果
    */
    for(const auto &cishu : cishuSet)
    {
        vector<int> tmp = vector<int>();
        for(const auto &elemPair : vecPair)
        {
            if(cishu == static_cast<int>(elemPair.second.size()))
            {
                tmp.push_back(elemPair.first);
            }
        }
        mapIntToVec.emplace(cishu, tmp);
        freeResource<vector<int>>(tmp);
    }

    vector<pair<int, vector<int>>> vecPairIntToVec(mapIntToVec.begin(), mapIntToVec.end());
    sort(vecPairIntToVec.begin(), vecPairIntToVec.end(), selfDefineSort4);
    // for(const auto &elemPair : vecPairIntToVec)
    // {
    //     __attribute__((unused)) int i = 0;
    //     printf("出现了[%d]次的有%ld个:  \r\n", elemPair.first, elemPair.second.size());
    //     for(const auto &elem : elemPair.second)
    //     {
    //         printf("\t(%d)", elem);
    //         if(++i > 9)
    //         {
    //             i = 0;
    //             printf("\r\n");
    //         }
    //     }
    //     printf("\r\n");
    // }

    /**
     * 得出 mIndexVec
    */
    freeResource<vector<int>>(mIndexVec);
    //->经过排序后第一个值就是目标值
    mIndexVec.insert(mIndexVec.end(), vecPairIntToVec[0].second.begin(), vecPairIntToVec[0].second.end());
    // mIndexVec.insert(mIndexVec.end(), vecPairIntToVec[1].second.begin(), vecPairIntToVec[1].second.end());
    // mIndexVec.insert(mIndexVec.end(), vecPairIntToVec[2].second.begin(), vecPairIntToVec[2].second.end());
    // mIndexVec.insert(mIndexVec.end(), vecPairIntToVec[3].second.begin(), vecPairIntToVec[3].second.end());
    // mIndexVec.insert(mIndexVec.end(), vecPairIntToVec[4].second.begin(), vecPairIntToVec[4].second.end());
    // mIndexVec.insert(mIndexVec.end(), vecPairIntToVec[5].second.begin(), vecPairIntToVec[5].second.end());
    printf("mIndexVec.size=%ld\r\n", mIndexVec.size());
    vector<int> tmp(mBaseVec.begin(), mBaseVec.begin()+mIncrease+200);
    for(const auto &elem : tmp)
    {
        auto itor = find(mIndexVec.begin(), mIndexVec.end(), elem);
        if(itor != mIndexVec.end())
        {
            mIndexVec.erase(itor);
        }
    }

    sort(mIndexVec.begin(), mIndexVec.end(), toDown);
    // printf("所以我选择的号码有:\r\n");
    // int ic = 0;
    // for(const auto &elem : mIndexVec)
    // {
    //     printf("\t(%d)", elem);
    //     if(ic++ > 8)
    //     {
    //         ic = 0;
    //         printf("\r\n");
    //     }
    // }
    // printf("\r\n");

    freeResource<vector<int>>(tmp);
    freeResource<vector<pair<int, vector<int>>>>(vecPairIntToVec);
    freeResource<vector<pair<int, vector<vector<int>>>>>(vecPair);
    freeResource<set<int>>(cishuSet);
    freeResource<map<int, vector<int>>>(mapIntToVec);
    return;
}

/**
 * 直选操作
*/
void PAILIE3_C::zhiXuanGetNewIndexForLoop()
{
    printf("mIndexVec.size=%ld, mIndex=%ld\r\n", mIndexVec.size(), mIndex);
    if(mIndex < static_cast<long>(mDatasUntilNow.size()))
    {
        long changeValue = mIndex;
        long spendMoney = 0;
        int bingo = 0;
        for( ;mIndex<static_cast<long>(mDatasUntilNow.size()); mIndex++)
        {
            if(mDatasUntilNow[mIndex].size() >= 2)
            {
                if(checkTheElemIsInTheVec<int>(mDatasUntilNow[mIndex][1], mIndexVec))
                {
                    printf("中了的号码(%d)的mIndex值为%ld\r\n", mDatasUntilNow[mIndex][1], mIndex);
                    mBingoNum++;
                    bingo = 1;
                    break;
                }
            }
        }
        printf("mIndex=%ld\r\n", mIndex);
        changeValue = mIndex - changeValue;
        mIncrease = static_cast<int>(changeValue);
        spendMoney = mIndexVec.size()*2*changeValue;
        mSpendMoney = mSpendMoney - spendMoney + 1040*bingo;
        printf("此次购买彩票[%d]次数花费%ld元,截止目前盈收%ld元，一共中了%d个号码.\r\n", mIncrease, spendMoney, mSpendMoney, mBingoNum);
        if(mIndex < static_cast<long>(mDatasUntilNow.size()))
        {
            loopOperate(to_string(mDatasUntilNow[mIndex][0]));
        }
    }
    return;
}

void PAILIE3_C::zhiXuan()
{
    int index = 1;
    vector<vector<int>> vec2Vec = vector<vector<int>>();
    map<int, vector<vector<int>>> mapRet = map<int, vector<vector<int>>>();

    /**
     * 生成 vec2Vec   vec2Vec里面的元素是(index, data);index是为了计算间隔值
    */
    sort(mVec2Vec.begin(), mVec2Vec.end(), selfDefineSort);
    for(const auto &elemVec2:mVec2Vec)
    {
        vector<int> tmp = vector<int>();
        if(elemVec2.size() < 2)
        {
            printf("error[%d]:the input paras vec2Vec is wrong~\r\n", __LINE__);
            break;
        }
        tmp.push_back(index++);
        tmp.push_back(elemVec2[1]);
        vec2Vec.push_back(tmp);
        freeResource<vector<int>>(tmp);
    }
    printf("index=%d, vec2Vec.size=%ld\r\n", index, vec2Vec.size());

    zhiXuanOrderBaseData(vec2Vec, mBaseVec);
    zhiXuanTongJiJieGuoShengChengMap(vec2Vec, mBaseVec, mapRet);
    zhiXuanPrintfTongJiJieGuo(mapRet);
    zhiXuanGetNewIndexForLoop();

    freeResource<vector<vector<int>>>(vec2Vec);
    freeResource<map<int, vector<vector<int>>>>(mapRet);
    return;
}


/**
 * 从0-9任取两个不同数字的组合的集合，
 * 1，得出每个组合所出现的相邻的次数之间的间隔的集合
 * 2，得出每个组合所出现的次数之间的间隔的集合中的最小值
*/
void PAILIE3_C::zuHeXuan3()
{
    int index = 1;
    vector<int> resVec = vector<int>(2, 0);
    vector<int> Vec10Ori = {0,1,2,3,4,5,6,7,8,9};
    vector<vector<int>> vec2Vec = vector<vector<int>>();
	vector<vector<int>> resVec2Vec = vector<vector<int>>();
    map<vector<int>, vector<vector<int>>> mapRet = map<vector<int>, vector<vector<int>>>();

    sort(mVec2Vec.begin(), mVec2Vec.end(), selfDefineSort);
    for(const auto &elemVec2:mVec2Vec)
    {
        vector<int> tmp = vector<int>();
        if(elemVec2.size() < 2)
        {
            printf("error[%d]:the input paras vec2Vec is wrong~\r\n", __LINE__);
            break;
        }
        tmp.push_back(index++);
        tmp.push_back(elemVec2[1]);
        vec2Vec.push_back(tmp);
        freeResource<vector<int>>(tmp);
    }
    printf("index=%d, vec2Vec.size=%ld\r\n", index, vec2Vec.size());

	arrange::recursion(Vec10Ori, resVec, 0, 0, static_cast<int>(resVec.size()), resVec2Vec);
    printf("resVec2Vec.size=%ld\r\n", resVec2Vec.size());
    zuHeXuan3GetMapRet(vec2Vec, resVec2Vec, mapRet);
    zuHeXuan3PrintfMapRet(mapRet);
    zuHeXuan3GetTheCurrentVec2Order(vec2Vec);
    zuHeXuan3GetNewIndexForLoop();

    freeResource<vector<int>>(resVec);
    freeResource<vector<int>>(Vec10Ori);
    freeResource<vector<vector<int>>>(vec2Vec);
    freeResource<vector<vector<int>>>(resVec2Vec);
    freeResource<map<vector<int>, vector<vector<int>>>>(mapRet);
}

void PAILIE3_C::zuHeXuan3GetNewIndexForLoop()
{
    printf("mIndexVecVec.size=%ld, mIndex=%ld\r\n", mIndexVecVec.size(), mIndex);
    if(mIndex < static_cast<long>(mDatasUntilNow.size()))
    {
        long changeValue = mIndex;
        long spendMoney = 0;
        for( ;mIndex<static_cast<long>(mDatasUntilNow.size()); mIndex++)
        {
            bool ret = false;
            for(const auto &elemVec : mIndexVecVec)
            {
                if(mDatasUntilNow[mIndex].size() >= 2)
                {
                    if(checkElemsIsBeLong(mDatasUntilNow[mIndex][1], elemVec))
                    {
                        ret = true;
                        break;
                    }
                }
            }

            if(ret)
            {
                mIndex++;
                break;
            }
        }
        printf("mIndex=%ld\r\n", mIndex);
        changeValue = mIndex - changeValue;
        spendMoney = mIndexVecVec.size()*4*changeValue;
        mSpendMoney = mSpendMoney - spendMoney + 346;
        printf("此次购买彩票花费%ld元,截止目前盈收%ld元\r\n", spendMoney, mSpendMoney);
        if(mIndex < static_cast<long>(mDatasUntilNow.size()))
        {
            loopOperate(to_string(mDatasUntilNow[mIndex][0]));
        }
    }
    return;
}

/**
 * 所传入的vec2Vec里面的每个元素都是唯一的
*/
bool PAILIE3_C::checkTheVec2IsInTheVec2Vec(const vector<int> &vec2, const vector<vector<int>> &vec2Vec)
{
    bool ret = true;
    if(vec2Vec.size() == 0)
    {
        return false;
    }

    set<vector<int>> set2Vec(vec2Vec.begin(), vec2Vec.end());
    auto itor = set2Vec.find(vec2);
    if(itor != set2Vec.end())
    {
    }
    else
    {
        ret = false;
    }
    freeResource<set<vector<int>>>(set2Vec);
    return ret;
}

void PAILIE3_C::zuHeXuan3GetTheCurrentVec2Order(const vector<vector<int>> &vec2Vec)
{
    int index = 0;
    vector<vector<int>> vec2VecRet = vector<vector<int>>();
    /**
     * 倒序
    */
    // reverse(vec2Vec.begin(), vec2Vec.end());
    for(const auto &elemVec : vec2Vec)
    {
        if(elemVec.size() >= 2)
        {
            vector<int> vec2 = vector<int>();
            if(getTheVec2ByData(elemVec[1], vec2))
            {
                if(!checkTheVec2IsInTheVec2Vec(vec2, vec2VecRet))
                {
                    vec2VecRet.push_back(vec2);
                    if(vec2VecRet.size() > 45)
                    {
                        freeResource<vector<int>>(vec2);
                        break;
                    }
                }
            }
            freeResource<vector<int>>(vec2);
        }
    }
    // printf("45个两位数组合的截止目前的最新排序:\r\n");
    for(const auto &elemVec : vec2VecRet)
    {
        if(index++ < SELECT_DATA_THRESHOLD)
        {
            auto itor = find(mIndexVecVec.begin(), mIndexVecVec.end(), elemVec);
            if(itor != mIndexVecVec.end())
            {
                mIndexVecVec.erase(itor);
            }
        }
        // printf("\t");
        // for(const auto &elem : elemVec)
        // {
        //     printf("%d_", elem);
        // }
        // printf("\r\n");
    }

    printf("mIndexVecVec.size=%ld\r\n", mIndexVecVec.size());

    if(mIndexVecVec.size() > SELECT_DATA_THRESHOLD-5)
    {
        vector<vector<int>> tmpVecVec(mIndexVecVec.begin(), mIndexVecVec.begin()+(SELECT_DATA_THRESHOLD-5));
        tmpVecVec.swap(mIndexVecVec);
        freeResource<vector<vector<int>>>(tmpVecVec);
    }

    // printf("所以我购买的两个数字组合的号码分别是:\r\n");
    // for(const auto &elemVec : mIndexVecVec)
    // {
    //     printf("\t");
    //     for(const auto &elem : elemVec)
    //     {
    //         printf("%d_", elem);
    //     }
    //     printf("\r\n");
    // }

    printf("检验结果:%s\r\n", ((vec2VecRet.size()==45)?"正确":"错误"));
    freeResource<vector<vector<int>>>(vec2VecRet);

    return;
}

void PAILIE3_C::zuHeXuan3PrintfMapRet(const map<vector<int>, vector<vector<int>>> &mapRet)
{
    int index = 0;
    vector<pair<vector<int>, vector<vector<int>>>> vecPair(mapRet.begin(), mapRet.end());
    set<int> setCiShu = set<int>();
    map<int, vector<vector<int>>> intMapVec = map<int, vector<vector<int>>>();

    for(const auto &elemPair : mapRet)
    {
        set<int> intervalSet = set<int>();
        // if(elemPair.first.size() >= 2)
        // {
        //     printf("%d:%d\r\n", elemPair.first[0], elemPair.first[1]);
        // }
        setCiShu.insert(static_cast<int>(elemPair.second.size()));

        for(const auto &elemVec : elemPair.second)
        {
            if(elemVec.size() >= 2)
            {
                // printf("\t[%d,%d]\r\n", elemVec[1], elemVec[0]);
                intervalSet.insert(elemVec[0]-index);
                index = elemVec[0];
            }
        }
        // printf("\t一共出现[%ld]个元素\r\n", elemPair.second.size());
        // for(const auto &elem : intervalSet)
        // {
        //     if(elem < SELECT_DATA_THRESHOLD)
        //     {
        //         printf("\t%d\r\n", elem);
        //     }
        // }
        freeResource<set<int>>(intervalSet);
    }

    sort(vecPair.begin(), vecPair.end(), selfDefineSort2);
    // for(const auto &elemPair : vecPair)
    // {
    //     if(elemPair.first.size() >= 2)
    //     {
    //         printf("%d:%d\t出现了:[%ld]个元素\r\n",elemPair.first[0], elemPair.first[1], elemPair.second.size());
    //     }
    // }

    for(const auto &cishu : setCiShu)
    {
        vector<vector<int>> tmp = vector<vector<int>>();
        for(const auto &elemPair : vecPair)
        {
            if(static_cast<int>(elemPair.second.size()) == cishu)
            {
                tmp.push_back(elemPair.first);
            }
        }
        sort(tmp.begin(), tmp.end());
        intMapVec.emplace(cishu, tmp);
        freeResource<vector<vector<int>>>(tmp);
    }

    vector<pair<int, vector<vector<int>>>> vecPair2(intMapVec.begin(), intMapVec.end());
    sort(vecPair2.begin(), vecPair2.end(), selfDefineSort3);
    freeResource<vector<vector<int>>>(mIndexVecVec);//->生成之前先清一下数据
    for(const auto &elemPair : vecPair2)
    {
        // printf("出现了:[%d]个元素的两个数字的组合集合有%ld个:", elemPair.first, elemPair.second.size());
        // for(const auto &elemVec : elemPair.second)
        // {
        //     printf("\t");
        //     for(const auto &elem : elemVec)
        //     {
        //         printf("%d_", elem);
        //     }
        //     printf(";");
        // }
        // printf("\r\n");
        // if(mIndexVecVec.size() < SELECT_DATA_THRESHOLD+10)
        {
            mIndexVecVec.insert(mIndexVecVec.end(), elemPair.second.begin(), elemPair.second.end());
        }
    }

    freeResource<set<int>>(setCiShu);
    freeResource<vector<pair<int, vector<vector<int>>>>>(vecPair2);
    freeResource<map<int, vector<vector<int>>>>(intMapVec);
    freeResource<vector<pair<vector<int>, vector<vector<int>>>>>(vecPair);
    return;
}

void PAILIE3_C::zuHeXuan3GetMapRet(const vector<vector<int>> &vec2Vec,
                          const vector<vector<int>> &resVec2Vec,
                          map<vector<int>, vector<vector<int>>> &mapRet)
{
    for(const auto &elemResVec2 : resVec2Vec)
    {
        vector<vector<int>> tmpVecVec = vector<vector<int>>();
        for(const auto &elemVec2 : vec2Vec)
        {
            if(elemVec2.size() >= 2)
            {
                if(checkElemsIsBeLong(elemVec2[1], elemResVec2))
                {
                    tmpVecVec.push_back(elemVec2);
                }
            }
        }
        mapRet.emplace(elemResVec2, tmpVecVec);
        freeResource<vector<vector<int>>>(tmpVecVec);
    }
    return;
}

bool PAILIE3_C::getTheVec2ByData(const int &data, vector<int> &vec2)
{
    bool ret = true;
    set<int> splitNums = set<int>();
    splitNums.insert(data/100);
    splitNums.insert((data/10)%10);
    splitNums.insert(data%10);
    if(splitNums.size() == 2)
    {
        vector<int> tmp(splitNums.begin(), splitNums.end());
        tmp.swap(vec2);
        freeResource<vector<int>>(tmp);
    }
    else
    {
        ret = false;
    }
    freeResource<set<int>>(splitNums);
    return ret;
}

bool PAILIE3_C::checkElemsIsBeLong(const int &data, const vector<int> &resVec)
{
    bool ret = true;
    set<int> splitNums = set<int>();
    splitNums.insert(data/100);
    splitNums.insert((data/10)%10);
    splitNums.insert(data%10);
    if(splitNums.size() == 2)
    {
        for(const auto &elem:resVec)
        {
            auto itor = splitNums.find(elem);
            if(itor != splitNums.end())
            {
                // ret &= true;
            }
            else
            {
                ret &= false;
            }
        }
    }
    else
    {
        ret = false;
    }
    freeResource<set<int>>(splitNums);
    return ret;
}


/**
 * 组合选 6
*/
void PAILIE3_C::zuHeXuan6()
{
    int index = 1;
    vector<vector<int>> vec2Vec = vector<vector<int>>();
    map<vector<int>, vector<vector<int>>> mapRet = map<vector<int>, vector<vector<int>>>();

    /**
     * 生成 vec2Vec   vec2Vec里面的元素是(index, data);index是为了计算间隔值
    */
    sort(mVec2Vec.begin(), mVec2Vec.end(), selfDefineSort);
    for(const auto &elemVec2:mVec2Vec)
    {
        vector<int> tmp = vector<int>();
        if(elemVec2.size() < 2)
        {
            printf("error[%d]:the input paras vec2Vec is wrong~\r\n", __LINE__);
            break;
        }
        tmp.push_back(index++);
        tmp.push_back(elemVec2[1]);
        vec2Vec.push_back(tmp);
        freeResource<vector<int>>(tmp);
    }
    printf("index=%d, vec2Vec.size=%ld\r\n", index, vec2Vec.size());

    zuHeXuan6OrderBaseData(vec2Vec, mBaseVecVec);
    zuHeXuan6TongJiJieGuoShengChengMap(vec2Vec, mBaseVecVec, mapRet);
    zuHeXuan6PrintfTongJiJieGuo(mapRet);
    zuHeXuan6GetNewIndexForLoop();

    freeResource<vector<vector<int>>>(vec2Vec);
    freeResource<map<vector<int>, vector<vector<int>>>>(mapRet);
    return;
}


bool PAILIE3_C::zuHeXuan6CheckTheDataIsRight(const int &data, vector<int> &indexVec)
{
    bool ret = true;
    set<int> splitNums = set<int>();
    splitNums.insert(data/100);
    splitNums.insert((data/10)%10);
    splitNums.insert(data%10);
    if(splitNums.size() == 3)
    {
        freeResource<vector<int>>(indexVec);
        vector<int> tmp(splitNums.begin(), splitNums.end());
        tmp.swap(indexVec);
        freeResource<vector<int>>(tmp);
    }
    else
    {
        ret = false;
    }
    freeResource<set<int>>(splitNums);
    return ret;
}

void PAILIE3_C::zuHeXuan6OrderBaseData(const vector<vector<int>> &vec2Vec, vector<vector<int>> &baseVecVec)
{
    freeResource<vector<vector<int>>>(baseVecVec);
    for(const auto &elemVec : vec2Vec)
    {
        if(elemVec.size() >= 2)
        {
            vector<int> indexVec = vector<int>();
            bool ret = zuHeXuan6CheckTheDataIsRight(elemVec[1], indexVec);
            if(false != ret)
            {//符合类型
                bool ret1 = checkTheElemIsInTheVec<vector<int>>(indexVec, baseVecVec);
                if(false == ret1)
                {
                    baseVecVec.push_back(indexVec);
                }
            }
            freeResource<vector<int>>(indexVec);
            if(baseVecVec.size() >= 120)
            {
                break;
            }
        }
    }
    printf("根据最新的期数号码(有%ld个)得到最新的基数的排序为:\r\n", baseVecVec.size());
    int ic = 0;
    for(const auto &elemVec : baseVecVec)
    {
        printf("\t[");
        for(const auto &elem : elemVec)
        {
            printf("%d,", elem);
        }
        printf("]");
        if(ic++ > 3)
        {
            printf("\r\n");
            ic = 0;
        }
    }
    printf("\r\n");

    return;
}

bool PAILIE3_C::zuHeXuan6CheckTheLeftDataIsBelongRightData(const int &data, const vector<int> &vec)
{
    bool ret = true;
    int bai=0,shi=0,ge=0;
    bai = data/100;
    shi = (data/10)%10;
    ge = data%10;
    ret &= checkTheElemIsInTheVec<int>(bai, vec);
    ret &= checkTheElemIsInTheVec<int>(shi, vec);
    ret &= checkTheElemIsInTheVec<int>(ge, vec);
    if((ret)&&(bai!=shi)&&(shi!=ge)&&(ge!=bai))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PAILIE3_C::zuHeXuan6TongJiJieGuoShengChengMap(const vector<vector<int>> &vec2Vec,
                                                   const vector<vector<int>> &baseVecVec,
                                                   map<vector<int>, vector<vector<int>>> &mapRet)
{
    for(const auto &elemVec1 : baseVecVec)
    {
        vector<vector<int>> tmp = vector<vector<int>>();
        for(const auto &elemVec : vec2Vec)
        {
            if(elemVec.size() >= 2)
            {
                if(zuHeXuan6CheckTheLeftDataIsBelongRightData(elemVec[1], elemVec1))
                {
                    tmp.push_back(elemVec);
                }
            }
        }
        sort(tmp.begin(), tmp.end());
        mapRet.emplace(elemVec1, tmp);
        freeResource<vector<vector<int>>>(tmp);
    }
    return;
}

void PAILIE3_C::zuHeXuan6PrintfTongJiJieGuo(const map<vector<int>, vector<vector<int>>> &mapRet)
{
    vector<pair<vector<int>, vector<vector<int>>>> vecPair(mapRet.begin(), mapRet.end());
    set<int> cishuSet = set<int>();
    map<int, vector<vector<int>>> mapIntToVec = map<int, vector<vector<int>>>();
    sort(vecPair.begin(), vecPair.end(), selfDefineSort2);
    for(const auto &elemPair : vecPair)
    {
        set<int> intervalSet = set<int>();
        int index = 0;
        // printf("(");
        // for(const auto &elem : elemPair.first)
        // {
        //     printf("%d,", elem);
        // }
        // printf(")出现了[%ld]次:\r\n", elemPair.second.size());
        cishuSet.insert(static_cast<int>(elemPair.second.size()));
        for(const auto &elemVec : elemPair.second)
        {
            if(index)
            {
                intervalSet.insert(elemVec[0]-index);
                index = elemVec[0];
            }
            else
            {
                index = elemVec[0];
            }

            // printf("\t索引");
            // for(const auto &elem : elemVec)
            // {
            //     printf("%d_", elem);
            // }
            // printf("\r\n");
        }

        // printf("\t间隔值分别为:");
        // for(const auto &elem : intervalSet)
        // {
        //     printf("%d ", elem);
        // }
        // printf("\r\n");
        freeResource<set<int>>(intervalSet);
    }

    /**
     * 根据出现的次数判断结果
    */
    for(const auto &cishu : cishuSet)
    {
        vector<vector<int>> tmp = vector<vector<int>>();
        for(const auto &elemPair : vecPair)
        {
            if(cishu == static_cast<int>(elemPair.second.size()))
            {
                tmp.push_back(elemPair.first);
            }
        }
        sort(tmp.begin(), tmp.end());
        mapIntToVec.emplace(cishu, tmp);
        freeResource<vector<vector<int>>>(tmp);
    }

    vector<pair<int, vector<vector<int>>>> vecPairIntToVec(mapIntToVec.begin(), mapIntToVec.end());
    sort(vecPairIntToVec.begin(), vecPairIntToVec.end(), selfDefineSort5);
    // for(const auto &elemPair : vecPairIntToVec)
    // {
    //     __attribute__((unused)) int ic = 0;
    //     printf("出现了[%d]次的有%ld个:  \r\n", elemPair.first, elemPair.second.size());
    //     for(const auto &elemVec : elemPair.second)
    //     {
    //         printf("\t[");
    //         for(const auto &elem : elemVec)
    //         {
    //             printf("%d,", elem);
    //         }
    //         printf("]");
    //         if(ic++ > 3)
    //         {
    //             ic = 0;
    //             printf("\r\n");
    //         }
    //     }
    //     printf("\r\n");
    // }

    /**
     * 得出 mIndexVecVec
    */
    freeResource<vector<vector<int>>>(mIndexVecVec);
    //->经过排序后第一个值就是目标值
    // mIndexVecVec.insert(mIndexVecVec.end(), vecPairIntToVec[0].second.begin(), vecPairIntToVec[0].second.end());
    for(const auto &elemPair : vecPairIntToVec)
    {
        mIndexVecVec.insert(mIndexVecVec.end(), elemPair.second.begin(), elemPair.second.end());
        if(mIndexVecVec.size() > 30)
        {
            break;
        }
    }

    printf("mIndexVecVec.size=%ld, mIncrease=%d\r\n", mIndexVecVec.size(), mIncrease);
    vector<vector<int>> tmp(mBaseVecVec.begin(), mBaseVecVec.begin()+ mIncrease+10);
    for(const auto &elem : tmp)
    {
        auto itor = find(mIndexVecVec.begin(), mIndexVecVec.end(), elem);
        if(itor != mIndexVecVec.end())
        {
            mIndexVecVec.erase(itor);
        }
    }

    sort(mIndexVecVec.begin(), mIndexVecVec.end(), vecVecToDown);
    // printf("所以我选择的号码有:\r\n");
    // int ic = 0;
    // for(const auto &elemVec : mIndexVecVec)
    // {
    //     printf("\t[");
    //     for(const auto &elem : elemVec)
    //     {
    //         printf("%d,", elem);
    //     }
    //     printf("]");
    //     if(ic++ > 3)
    //     {
    //         printf("\r\n");
    //         ic = 0;
    //     }
    // }
    // printf("\r\n");

    freeResource<vector<vector<int>>>(tmp);
    freeResource<vector<pair<int, vector<vector<int>>>>>(vecPairIntToVec);
    freeResource<vector<pair<vector<int>, vector<vector<int>>>>>(vecPair);
    freeResource<set<int>>(cishuSet);
    freeResource<map<int, vector<vector<int>>>>(mapIntToVec);
    return;
}

/**
 * 直选操作
*/
void PAILIE3_C::zuHeXuan6GetNewIndexForLoop()
{
    printf("mIndexVecVec.size=%ld, mIndex=%ld\r\n", mIndexVecVec.size(), mIndex);
    if(mIndex < static_cast<long>(mDatasUntilNow.size()))
    {
        long changeValue = mIndex;
        long spendMoney = 0;
        int bingo = 0;
        for( ;mIndex<static_cast<long>(mDatasUntilNow.size()); mIndex++)
        {
            if(mDatasUntilNow[mIndex].size() >= 2)
            {
                vector<int> indexVec = vector<int>();
                bool ret = zuHeXuan6CheckTheDataIsRight(mDatasUntilNow[mIndex][1], indexVec);
                if(false != ret)
                {//符合类型
                    bool ret1 = checkTheElemIsInTheVec<vector<int>>(indexVec, mIndexVecVec);
                    if(false != ret1)
                    {
                        printf("中了的号码(%d)的mIndex值为%ld\r\n", mDatasUntilNow[mIndex][1], mIndex);
                        mBingoNum++;
                        bingo = 1;
                        mIndex++;
                        freeResource<vector<int>>(indexVec);
                        break;
                    }
                }
                freeResource<vector<int>>(indexVec);
            }
        }
        printf("mIndex=%ld\r\n", mIndex);
        changeValue = mIndex - changeValue;
        mIncrease = static_cast<int>(changeValue);
        if(mIncrease)
        {
            // mIndex++;
        }
        else
        {
            // mIndex++;
            // mIncrease = 1;
        }
        spendMoney = mIndexVecVec.size()*2*changeValue;
        mSpendMoney = mSpendMoney - spendMoney + 173*bingo;
        printf("此次购买彩票[%d]次数花费%ld元,截止目前盈收%ld元，一共中了%d个号码.\r\n", mIncrease, spendMoney, mSpendMoney, mBingoNum);
        if(mIndex < static_cast<long>(mDatasUntilNow.size()))
        {
            printf("the 目标值为:%d, %d\r\n", mDatasUntilNow[mIndex][0], mDatasUntilNow[mIndex][1]);
            loopOperate(to_string(mDatasUntilNow[mIndex][0]));
        }
    }
    return;
}