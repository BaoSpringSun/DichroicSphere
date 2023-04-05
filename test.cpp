#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "sqlitetbl.hpp"
#include <vector>
#include <map>
#include<algorithm>
using namespace std;

void func1(sqlite_tb *psql, vector<uint8> &vred, const vector<uint8> &vblue);

int main(int argc, char** argv)
{
    // const vector<uint8> redbingo{1, 11, 12, 18, 21, 27, 31};
    // const vector<uint8> redbingo{2, 4, 5, 6, 9, 10, 12, 13, 19, 21, 27, 33};
    // const vector<uint8> bluebingo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

	// const vector<uint8> redbingo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 18, 21, 27, 31};
	// const vector<uint8> redbingo{11, 12, 18, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};

	// const vector<uint8> redbingo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,14,15,16,17, 18,19,20};//86
	// const vector<uint8> redbingo{11, 12, 13,14,15,16,17, 18,19,20,21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };//75
	// const vector<uint8> redbingo{11, 12, 13,14,15,16,17, 18,19,20,21, 22, 23, 24, 25, 26, 27, 28, 29, 30,31,32,33 };//197
	// const vector<uint8> redbingo{1, 2, 3,4,5,6,7, 8,9,10,21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };//77
	// const vector<uint8> redbingo{1, 2, 3,4,5,6,7, 8,9,10,21, 22, 23, 24, 25, 26, 27, 28, 29, 30,31,32,33 };//199
	// const vector<uint8> redbingo{1, 2, 3,4,5,6,7, 8,9,10,14,21, 22, 23, 24, 25, 26, 27, 28, 29, 30,31,32,33 };//282
	// const vector<uint8> redbingo{1, 2, 3,4,5,6,7, 8,9,10,14,23, 24, 25, 26, 27, 28, 29, 30,31,32,33 };//147

	// const vector<uint8> redbingo{1, 2, 3,4,5,6,7,8,9,10,11, 30,31,32,33 };
	// const vector<uint8> redbingo{30,3,12,4,5,9,13,2,10,8,19,27,7,17,18,20,6,32,22,1,26,14 };//233
	// const vector<uint8> redbingo{33	,28	,24	,23	,21	,31	,25,29,11,16,15,30,3,12,4,5,9,13,2,10,8,19};//167
	// const vector<uint8> redbingo{33,28,24,23,21,31,25,29,11,16,15,9,27,7,17,18,20,6,32,22,1,26,14};//275
	// const vector<uint8> redbingo{1, 2, 3,4,5,6,7, 8,9,10,14,21, 22, 23, 24, 25, 26, 27, 28, 29, 30,31,32,33 };//380

	// const vector<uint8> redbingo{ 1,2, 3,4,5,6,7, 8,9,10,14,20,21,22,23, 24, 25, 26, 27, 28, 29, 30,31,32,33 };//500
	/**
	 * -1:383 
	 * -2:384
	 * -3:373
	 * -4:379
	 * -5:378
	 * -6:387
	 * -7:365
	 * -8;373
	 * -9;390
	 * -10;386
	 * -14;364
	 * -20;380
	 * -21:378
	 * -22:357
	 * -23:383
	 * -24:379
	 * -25:385
	 * -26:366
	 * -27;393
	 * -28:396
	 * -29:391
	 * -30:388
	 * -31:384
	 * -32;378
	 * -33:380
	*/
	// const vector<uint8> redbingo{  1,2,3,4,5, 6,7,8,10,14,20,21, 22, 23, 24, 25,26,30,31,32,33 };//185
	/**
	 * -1:133 
	 * -2:140
	 * -3: 125
	 * -4: 134
	 * -5: 135
	 * -6: 129
	 * -7: 124
	 * -8; 127
	 * -10; 140
	 * -14; 133
	 * -20; 132
	 * -21: 131
	 * -22: 121
	 * -23: 133
	 * -24: 135
	 * -25: 141
	 * -26: 127
	 * -30: 137
	 * -31: 132
	 * -32; 132
	 * -33: 134
	 * */

	// const vector<uint8> redbingo{  1,3,4, 5,6,7,8,14,20, 21, 22, 23, 24,26,30,31,32,33 };//185
	/**
	 * -1:  55
	 * -3:  52
	 * -4:  55
	 * -5:  52
	 * -6:  51
	 * -7:  50
	 * -8;  47
	 * -14;  53
	 * -20;  51
	 * -21:  56
	 * -22:  45
	 * -23:  55
	 * -24:  52
	 * -26:  52
	 * -30:  54
	 * -31:  51
	 * -32;  49
	 * -33:  56
	 * */
	
	// const vector<uint8> redbingo{ 3,5,6,7,8,14, 20, 22,24,26,30,31,32 };//15
	/**
	 * -3:  7
	 * -5:  8
	 * -6:  8
	 * -7:  9
	 * -8;  5
	 * -14;  9
	 * -20;  8
	 * -22:  6
	 * -24:  7
	 * -26:  8
	 * -30:  11
	 * -31:  11
	 * -32;  8
	 * */


	// const vector<uint8> redbingo{ 3,5,6,7,8, 14, 20,22,24,26,32 };//7
	/**
	 * -3:  1
	 * -5:  5
	 * -6:  4
	 * -7:  6 
	 * -8;  2 
	 * -14; 4 
	 * -20; 3 
	 * -22: 4 
	 * -24: 1  
	 * -26: 3  
	 * -32; 2 
	 * */
	
	// const vector<uint8> redbingo{ 3,6, 8, 14,20,22,24,26,32 };//4
	/**
	 * -3: 0
	 * -6: 3  
	 * -8; 0   
	 * -14; 2  
	 * -20; 1  
	 * -22: 3  
	 * -24: 0   
	 * -26: 2   
	 * -32; 1  
	 * */

	// const vector<uint8> redbingo{ 3,8, 14,20,24,26,32 };//2
	// const vector<uint8> redbingo{ 33    ,3,8, 14,20,24,26,32 };
	/**
	 * +1:  2
	 * +2;   2  
	 * +4;   2 
	 * +5;   2 
	 * +6:   				3  ---
	 * +7:   				3   ---
	 * * +9:  				3  ---
	 * +10;   2  
	 * +11;   2 
	 * +12;   2 
	 * +13:   2  
	 * +15:   2  
	 * +16;  2
	 * * +17:  2
	 * +18;  2   
	 * +19;   2 
	 * +21;    2
	 * +22:  				3   ---
	 * +23:   				3   --- 
	 * +25;   2  
	 * +27;   2 
	 * +28;   				3  ---
	 * +29:  2   
	 * +30:   2  
	 * +31; 2
	 * +33:  				3  --- 
	 * */


	// const vector<uint8> redbingo{ 2, 3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23, 24, 25, 26, 27, 28, 29, 30,31,32,33 };//2794
	/*
		-1:2398
		-2:
		-3:
		-4:
		-5:
		-6:
		-7:
		-8:
		-9:
		-10:
		-11：
		-12：
		-13：
		-14：
		-15：
		-16：
		-17：
		-18：
		-19：
		-20：
		-21：
		-22：
		-23：
		-24：
		-25：
		-26：
		-27：
		-28：
		-29：
		-30：
		-31：
		-32：
		-33:
	*/


	// vector<uint8> redbingo{1,2, 3,4,5,6,7, 8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23, 24, 25, 26, 27, 28, 29, 30,31,32,33 };//2794

	vector<uint8> redbingo{1,7,11,12,14,15,26,27};

	const vector<uint8> bluebingo{1, 2,3, 4, 5, 6, 7,8, 9, 10, 11, 12, 13,14, 15,16};//380
	

	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb();
    //打开数据库，不存在，创建数据库db
	sql->OpenDB();
	//创建表
	sql->CreateTable();
	//插入数据
	sql->InsertData();
	//删除
	// sql->DeleteData(2023032);
	//更新
	// sql->UpdateData();
	
	// func1(sql, redbingo, bluebingo);
	uint32 count = 0;
	sql->SelectData(redbingo, bluebingo, count);

	// //查询
	// sql->SelectData(redbingo, bluebingo, count);
	sql->SelectUniqueData();
	sql->SelectAllData();
	sql->SelectRepeatData();
	sql->SelectGetTotalRows();
	sql->SelectDistinctDataByLineName("red1,red6");
	sql->SelectDistinctDataAmountByLineName("red1");
	// sql->SelectUniqueDataAmount();
	sql->CloseDB();
	return 0;
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

static bool cmp(const pair<int32_t, int32_t> a, const pair<int32_t, int32_t> b)
{
	// return a.first > b.first;//按照key从大到小
	return a.second > b.second;//按照value从大到小
}


void func1(sqlite_tb *psql, vector<uint8> &vred, const vector<uint8> &vblue)
{
	map<int32_t, int32_t> retm;

	if(vred.size() <= 8)
	{
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
		uint8 index = 0;
		vector<pair<int32_t, int32_t>> vecpair(retm.begin(), retm.end());
		sort(vecpair.begin(), vecpair.end(), cmp);

		// vector<pair<int32_t, int32_t>>::const_iterator constitor = vecpair.begin();
		// for( ; constitor != vecpair.end(); constitor++)
		// {
		// 	printf("%d:\t%d\r\n", constitor->first, constitor->second);
		// }
		for(const auto &emel:vecpair)
		{
			printf("%d:\t%d\r\n",emel.first, emel.second);
		}
		printf("start:%d---end:%d\r\n", vecpair.front().first, vecpair.back().first);
		// index = -vecpair.back().first;//选择至今未出现的组合数字
		index = -vecpair.front().first;//选择至今已出现次数最多的组合数字
		VecDelIndexElem(vred, index);
		func1(psql, vred, vblue);
	}
	return;
}



