#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "sqlitetbl.hpp"
#include <vector>
#include <map>
#include<algorithm>
using namespace std;

void func1(sqlite_tb *psql, vector<uint8> &vred, const vector<uint8> &vblue);
int64_t funcCnFrom(const int32_t from, const int8_t n);
void findRule();

int main(int argc, char** argv)
{
	// vector<uint8> redbingo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	// 						  11,12,13,14,15,16,17,18,19,20,
	// 						  21,22,23,24,25,26,27,28,29,30,
	// 						  31,32,33 };//2979

	// vector<uint8> redbingo{1,6,7,9,10,11,12,14,15,16,18,20,26,27,29,32};//至今出现概率最多的16个数组的组合
	vector<uint8> redbingo{3,9,11,12,16,19,20,23,25,27,29,30,31,33};//最有概率出现的组合

	const vector<uint8> bluebingo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};//380


	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb();
    // 打开数据库，不存在，创建数据库db
	sql->OpenDB();
	// 创建表
	sql->CreateTable();
	// 插入数据
	sql->InsertData();
	// 删除
	// sql->DeleteData(2023032);
	// 更新
	// sql->UpdateData();

#if 1
	// 查询服务
	uint32 count = 0;
	sql->SelectData(redbingo, bluebingo, count);//复式计算方式
	sql->SelectUniqueData();
	sql->SelectAllData();
	sql->SelectRepeatData();
	sql->SelectGetTotalRows();
	sql->SelectDistinctDataByLineName("red1,red6");
	sql->SelectDistinctDataAmountByLineName("red1");
#else
	func1(sql, redbingo, bluebingo);
#endif

	// sql->SelectUniqueDataAmount();
	sql->CloseDB();

	findRule();

	return 0;
}

void findRule()
{
	for(int32_t i=9;i<=33;i++)
	{
		printf("%d\t[%ld\t\t/\t%ld\t\t%ld]\r\n", i, funcCnFrom(i,6), funcCnFrom(i,8), funcCnFrom(8,6));
	}
	printf("%ld\r\n", ulong(1107568*16));
}


bool VecIsHasElem(const auto &vred, auto element)
{
	for(const auto &elem:vred )
	{
		if(elem == element)
		return true;
	}
	return false;
}

void VecDelIndexElem(auto &vred, auto element)
{
	if(false != VecIsHasElem(vred, element))
	{
		// vector<uint8>::iterator itor = vred.begin();
		// for ( ; itor != vred.end(); itor++)
		// {
		// 	if(*itor == element)
		// 	{
		// 		vred.erase(itor);
		// 		return;
		// 	}
		// }
		auto itor = vred.begin();
		for ( ; itor != vred.end(); itor++)
		{
			if(*itor == element)
			{
				vred.erase(itor);
				return;
			}
		}
	}

	return;
}

static bool cmppair(const pair<int32_t, int32_t> a, const pair<int32_t, int32_t> b)
{
	// return a.first > b.first;//按照key从大到小
	return a.second > b.second;//按照value从大到小
}

static bool cmpint(const int32_t a, const int32_t b)
{
	return a < b;//按照从小到大
	// return a > b;//按照从大到小
}


void func1(sqlite_tb *psql, vector<uint8> &vred, const vector<uint8> &vblue)
{
	map<int32_t, int32_t> retm;

	if(vred.size() <= 16)
	{
		sort(vred.begin(), vred.end(), cmpint);
		for(const auto &elem:vred)
		{
			printf("\t%d", elem);
		}
		printf("\r\n");
		return;
	}

	for (size_t i = 0; i < vred.size(); i++)
	{
		vector<uint8> initvred = vred;
		uint32 count = 0;
		uint8 index = initvred[i];
		VecDelIndexElem(initvred, index);
		if(psql->SelectData(initvred, vblue, count))
		{
			pair<int32_t, int32_t> mem = make_pair(-index, count);
			retm.insert(mem);
			// pair<map<int32_t, int32_t>::iterator,bool> ret = (retm.insert(mem));
			// printf("insert[%d]:%d %s\r\n",index, count, ret.second?"success":"fail");
			// retm.insert(make_pair(-index, count));
		}
	}
	// printf("retm.size()=%ld\r\n", retm.size());

	if(0 < retm.size())
	{
		int32_t indexkey = 0, indexvalue = 0;
		vector<pair<int32_t, int32_t>> vecpair(retm.begin(), retm.end());
		sort(vecpair.begin(), vecpair.end(), cmppair);
		printf("#############################################\r\n");
		printf("start:%d---end:%d\r\n", vecpair.front().first, vecpair.back().first);
		// vector<pair<int32_t, int32_t>>::const_iterator constitor = vecpair.begin();
		// for( ; constitor != vecpair.end(); constitor++)
		// {
		// 	printf("%d:\t%d\r\n", constitor->first, constitor->second);
		// }
		for(const auto &emel:vecpair)
		{
			printf("%d:\t%d\r\n",emel.first, emel.second);
		}
		printf("#############################################\r\n");

#if 0
		indexvalue = vecpair.back().second;
		if(0 != indexvalue)
		{
			for(const auto &elempair : vecpair)
			{
				indexkey = -elempair.first;//选择至今未出现的组合数字
				if(elempair.second == indexvalue)
				{
					VecDelIndexElem(vred, indexkey);
				}
			}
		}
		else
		{
			indexkey = -vecpair.back().first;//选择至今未出现的组合数字
			VecDelIndexElem(vred, indexkey);
		}
#else
		indexvalue = vecpair.front().second;
		if(0 != indexvalue)
		{
			for(const auto &elempair : vecpair)
			{
				indexkey = -elempair.first;//选择至今已出现次数最多的组合数字
				if((0 != indexvalue)&&(elempair.second == indexvalue))
				{
					VecDelIndexElem(vred, indexkey);
				}
			}
		}
		else
		{
			indexkey = -vecpair.front().first;//选择至今已出现次数最多的组合数字
			VecDelIndexElem(vred, indexkey);
		}
#endif

		func1(psql, vred, vblue);
	}
	return;
}


int64_t funcCnFrom(const int32_t from, const int8_t n)
{
	int64_t ret1 = 1, ret2 = 1;
	for(int32_t i=from;i>from-n;i--)
	{
		ret1 = ret1 * i;
	}

	for(int32_t i=n;i>0;i--)
	{
		ret2 = ret2 * i;
	}

	return (ret1/ret2);
}


