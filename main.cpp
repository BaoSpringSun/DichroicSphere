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

using namespace std;

/**
 * 可优化点：	findBaseSqlData  接口的效率的优化
 *
*/


#define 	USE_TIMER 	0

void func1(sqlite_tb *psql, vector<int> &vred, const vector<int> &vblue);
int64_t funcCnFrom(const int32_t from, const int8_t n);
void findRule();
void mathematics();
void createBaseSqlDb_1107568();
void createBingoSql();
void createBaseSqlDb();
void createFinalResultSqlDb();
void checkResult();
void checkVecBingoVec(vector<vector<int>> &vecBingoVec);
void getCoverResultDatas();
void getCoverResultDatas2();
void getCoverResultDatas3(const map<vector<int>, vector<vector<int>>> &LargeDatas,
							const vector<vector<int>> &baseVec6Vec);
void getCoverResultDatas4(const map<vector<int>, vector<vector<int>>> &LargeDatas,
							const vector<vector<int>> &baseVec6Vec);
void creatVec6VecResult(const vector<vector<int>> &vec6Vec, const char* dbname);
void findBaseSqlData(const vector<vector<int>> &vecBingoVec);
void getVec6VecFromDb(const vector<int> &vec6, vector<vector<int>> &vec6Vec);
unsigned int getTheSameAmountFromVec6Vecs(const vector<vector<int>> &vec6VecA, const vector<vector<int>> &vec6VecB);
void findTheSameAmountWithComparedVec6Vecs(/*const vector<int> &vec6A,const vector<int> &vec6B*/);
void genVec6VecForFindBaseSqlData();
void getVec6Vec1107568(vector<vector<int>> &vec6VecAll);
void startTask();
void initObjectDir();
void testGetCoverResultDatas2();
void getLargeDatas(map<vector<int>, vector<vector<int>>> &LargeDatas,
					const vector<vector<int>> &baseVec6Vec);

#if USE_TIMER
static void sigalrm_handler(int sig);
void set_timer();
static unsigned long cnt = 0;
#endif


static vector<vector<int>> gVecBingoVec;//要去重复

struct upperS
{
	bool operator()(const pair<vector<int>, int> &a,
					const pair<vector<int>, int> &b)
	{
		return a.second > b.second;//  ">"---从大到小重新排序;   "<"---从小到大重新排序，
	}
};

struct lowerS
{
	bool operator()(const pair<vector<int>, int> &a,
					const pair<vector<int>, int> &b)
	{
		return a.second < b.second;//  ">"---从大到小重新排序;   "<"---从小到大重新排序，
	}
};


struct cmp2
{
	bool operator()(const vector<int> &a,
					const vector<int> &b)
	{
		unsigned int size = static_cast<unsigned int>((a.size()<b.size())?(a.size()):(b.size()));
		for(unsigned int i=0;i<size;i++)
		{
			if(a[i]!=b[i])
			{
				return a[i]<b[i];
			}
		}
		return a==b;//  ">"---从大到小重新排序;   "<"---从小到大重新排序，
	}
};


int main(int argc, char** argv)
{

#if 0

	// vector<int> redbingo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	// 						  11,12,13,14,15,16,17,18,19,20,
	// 						  21,22,23,24,25,26,27,28,29,30,
	// 						  31,32,33 };//

	// vector<int> redbingo{1,6,7,9,10,11,12,14,15,16,18,20,26,27,29,32};//至今出现概率最多的16个数组的组合
	// vector<int> redbingo{6,10,11,14,15,20,26,29};
	// vector<int> redbingo{2,8,9,10,11,16,17,29};
	// vector<int> redbingo{2,4,7,8,9,10,11,16,17,18,20,22,27,29};
	// vector<int> redbingo{3,19,21,23,24,28,31};
	// vector<int> redbingo{2,3,4,6,7,8,9,11,12,13,15,18,19,20,21,23,24,25,27,28,29,30,31,32,33};
	vector<int> redbingo{3,9,11,12,16,19,20,23,25,27,29,30,31,33};//最有概率出现的组合
	// vector<int> redbingo{3,7,9,11,12,19,20,27,28,29,30,31,33};

	const vector<int> bluebingo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};//380


	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb();
    // 打开数据库，不存在，创建数据库db
	sql->OpenDB();
	// 创建表
	const char* sqlcmd = "create table if not exists tbldatas(date int PRIMARY key not null default 2000,"
                            "red1 int not null default 1,red2 int not null default 1,red3 int not null default 1,"
                            "red4 int not null default 1,red5 int not null default 1,red6 int not null default 1,"
                            "blue1 int not null default 1);";
	sql->CreateTable(sqlcmd);
	// 插入数据
	sql->InsertData();
	// 删除
	sql->DeleteData(2025032);
	// 更新
	// sql->UpdateData();

#if 1
	// 查询服务
	int count = 0;
	sql->SelectData(redbingo, bluebingo, count);//复式计算方式
	sql->SelectUniqueData();
	sql->SelectRepeatData();
	sql->SelectGetTotalRows();
	sql->SelectDistinctDataByLineName("red1,red6");
	sql->SelectDistinctDataAmountByLineName("red1");
#else
	func1(sql, redbingo, bluebingo);
#endif

	// sql->SelectUniqueDataAmount();
	sql->CloseDB();

#else
	map<vector<int>, vector<vector<int>>> LargeDatas = map<vector<int>, vector<vector<int>>>();
	initObjectDir();
  #if 1
	createBingoSql();
	createBaseSqlDb();

	arrange::startTime();
	printf("-----------startTime-------------\r\n");
	// genVec6VecForFindBaseSqlData();//(按规则)查找BaseSqlData
	// checkResult();
	//testGetCoverResultDatas2();//
	getLargeDatas(LargeDatas, gVecBingoVec);getCoverResultDatas4(LargeDatas, gVecBingoVec);
	// findTheSameAmountWithComparedVec6Vecs();
	arrange::endTime();
	unsigned int index = 0;
	while(true)
	{
		index++;
		printf("proceeding>>>\r\n");
		usleep(1000000);
		if(index > 5)
		{
			break;
		}
	}
  #else
	// createBaseSqlDb_1107568();
	createBingoSql();
	createBaseSqlDb();
#if USE_TIMER
	//~ 设定信号SIGALRM的处理函数
    signal(SIGALRM, sigalrm_handler);
    set_timer();
#endif
	sqlite_tb::mDebug = true;
	startTask();
	getLargeDatas(LargeDatas, gVecBingoVec);getCoverResultDatas3(LargeDatas, gVecBingoVec);//验证得到的所有数据块的覆盖率
  #endif

	map<vector<int>, vector<vector<int>>>().swap(LargeDatas);
	LargeDatas.clear();

#endif
	return 0;
}

/**
 * set容器默认是有自动从小到大排序好了的
 * set 的insert可以合并去重复 ， vector不行
 * set_union		取合并部分  a+b		a,b的合集，已去掉重复数据，里面是数据都是唯一的；
 * set_difference	取不同部分	a-b		a有b没有
 * set_intersection	去共有部分	a&b		a,b 都有
*/
void testGetCoverResultDatas2()
{
	set<vector<int>> a = {{1, 2, 3}, {3, 4, 5}, {1, 2, 3}, {3, 4, 8}};
	set<vector<int>> b = {{2, 3, 4}, {2, 8, 4}, {1, 2, 3}, {7, 3, 4}, {3, 4, 5}, {13, 14, 15}};
#if 0
	/**
	 * 借用set.insert 可去重复
	*/
	set<vector<int>> uniqueElements = set<vector<int>>();
	uniqueElements.insert(a.begin(), a.end());
	uniqueElements.insert(b.begin(), b.end());
	vector<vector<int>> hbvectors(uniqueElements.begin(), uniqueElements.end());

	printf("hbvectors.size=%ld\r\n", hbvectors.size());
    for (const auto &elementvec : hbvectors)
	{
        for (const auto &element : elementvec)
		{
			printf("%d\t", element);
		}printf("\r\n");
    }
#else
	set<vector<int>> c = set<vector<int>>();
	set<vector<int>> d = set<vector<int>>();
	set<vector<int>> e = set<vector<int>>();
	set_union(a.begin(), a.end(), b.begin(), b.end(), insert_iterator<set<vector<int>>>(c, c.begin()));
	printf("c.size=%ld\r\n", c.size());
    for (const auto &elementvec : c)
	{
        for (const auto &element : elementvec)
		{
			printf("%d\t", element);
		}printf("\r\n");
    }
	c.erase(c.begin());
	printf("c.size=%ld\r\n", c.size());
    for (const auto &elementvec : c)
	{
        for (const auto &element : elementvec)
		{
			printf("%d\t", element);
		}printf("\r\n");
    }
	set<vector<int>>().swap(c);//清空并释放内存
    set_difference(a.begin(), a.end(), b.begin(), b.end(), insert_iterator<set<vector<int>>>(d,d.begin()));
	printf("d.size=%ld\r\n", d.size());
    for (const auto &elementvec : d)
	{
        for (const auto &element : elementvec)
		{
			printf("%d\t", element);
		}printf("\r\n");
    }
	set<vector<int>>().swap(d);//清空并释放内存
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), insert_iterator<set<vector<int>>>(e, e.begin()));
	printf("e.size=%ld\r\n", e.size());
    for (const auto &elementvec : e)
	{
        for (const auto &element : elementvec)
		{
			printf("%d\t", element);
		}printf("\r\n");
    }
	set<vector<int>>().swap(e);//清空并释放内存
#endif
	return;
}

bool exists_test2(const std::string& name)
{
    return ( access( name.c_str(), F_OK ) != -1 );
}

/**
 * 初始化创建目标文件目录
*/
void initObjectDir()
{
	if(!exists_test2(SAVE6RET_DB_FILEDIR))
	{
		system("mkdir " SAVE6RET_DB_FILEDIR " -p");
	}

	if(!exists_test2(SAVE8RET_DB_FILEDIR))
	{
		system("mkdir " SAVE8RET_DB_FILEDIR " -p");
	}

	return;
}

void startTask()
{
	int timeCount = 0;
	ThreadPool<MyTask>* mt = new ThreadPool<MyTask>();//创建线程池
	if(false == mt->ThreadPoolInit())
	{
		printf("error:ThreadPoolInit failed.\r\n");
		return ;
	}

	arrange::startTime();
	printf("-----------startTime-------------\r\n");
	for(const auto &elemVec:gVecBingoVec)
	{
		MyTask t(elemVec);//创建任务
    	mt->Push(t);//将任务推送给队列中
		sleep(1);
	}
	arrange::endTime();
	mt->setIfExit(true);

	while(true)
    {
		if(MyTask::mSave8RetMap.size() < gVecBingoVec.size())
		{
			printf("proceeding>>>\r\n");
			usleep(1000000);
		}
		else if(MyTask::mSave8RetMap.size() == gVecBingoVec.size())
		{
			usleep(1000000);
			printf("proceeding>>>--->%d\r\n", timeCount);
			timeCount++;
			if(timeCount > 20)
			{
				createFinalResultSqlDb();
				break;
			}
		}
		else
		{
			printf("error:proceeding>>> something wrong has happened~\r\n");
			usleep(1000000);
		}
    }
	delete mt;
	mt = nullptr;
	return;
}


/**
 * 去重复数据，保证里面的每个数据的唯一性
*/
void checkVecBingoVec(vector<vector<int>> &vecBingoVec)
{
	const vector<vector<int>> vecVec = vecBingoVec;
	for(const auto &elemVec1:vecVec)
	{
		unsigned int index =  0;
		for(vector<vector<int>>::iterator itor=vecBingoVec.begin(); itor!=vecBingoVec.end(); itor++)
		{
			if(elemVec1 == (*itor))
			{
				index++;
				if(index > 1)
				{
					// for(const auto elem:(*itor))
					// {
					// 	printf("%d|", elem);
					// }printf("\t]\n");
					itor = vecBingoVec.erase(itor);
					if(itor!=vecBingoVec.begin())
					{
						itor--;
					}
				}
			}
		}
	}
	return;
}

void checkResult()
{
	const char* sqlcmd = "create table if not exists tbldatas("
                            "elems text not null, result int not null default 0,"
                            "UNIQUE(elems, result));";
	for(const auto &elemVec:gVecBingoVec)
	{
		sqlite_tb *sql = nullptr;
		int retInt = 0;
		string vecBingoToStrs = "";
		string check8RetDb = SAVE8RET_DB_FILEDIR;
		for(const auto &elem:elemVec)
		{
			vecBingoToStrs += arrange::NumberToString<int>(elem) + "_";
		}
		check8RetDb += vecBingoToStrs;

		sql = new sqlite_tb(check8RetDb.c_str());
		sql->OpenDB();
		sql->CreateTable(sqlcmd);

		if(false != sql->SelectResultFromVec8Db(vecBingoToStrs, retInt))
		{
			MyTask::mSave8RetMap.insert(std::make_pair(elemVec, retInt));
		}

		sql->CloseDB();
		delete sql;
		sql = nullptr;
	}

	printf("MyTask::mSave8RetMap.size = %ld\r\n", MyTask::mSave8RetMap.size());
	createFinalResultSqlDb();
	return;
}

void creatVec6VecResult(const vector<vector<int>> &vec6Vec, const char* dbname)
{
	const char* sqlcmd = "create table if not exists tbldatas("
                            "red1 int not null default 0,red2 int not null default 0,red3 int not null default 0,"
                            "red4 int not null default 0,red5 int not null default 0,red6 int not null default 0,"
                            "UNIQUE(red1, red2, red3, red4, red5, red6));";

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

	sql->CreateTable(sqlcmd);

	printf("vec6Vec.size=%ld\r\n", vec6Vec.size());
	if(vec6Vec.size() <= 0)
	{
		printf("vec6Vec.size()=0~\r\n");
		return;
	}
	for(const auto &elemVec6:vec6Vec)
	{
		string elemDatas = "(";
		for(const auto &elem:elemVec6)
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

/**
 * 生成全局1107568的vec的所有6个号码的总集合vec6VecAll->目的是为了(按规则)查找BaseSqlData
*/
void genVec6VecForFindBaseSqlData()
{
	vector<vector<int>> vec6VecAll;
	getVec6Vec1107568(vec6VecAll);
	findBaseSqlData(vec6VecAll);
	return;
}

void getVec6Vec1107568(vector<vector<int>> &vec6VecAll)
{
	sqlite_tb *sqlBase = nullptr;
	sqlBase = new sqlite_tb(BASESQLDBDATA_1107568);
	if(nullptr == sqlBase)
	{
		printf("sqlBase is nullptr.\r\n");
		return;
	}
    // 打开数据库，不存在，创建数据库db
	sqlBase->OpenDB();

	sqlBase->SelectAllData(vec6VecAll);
	sqlBase->CloseDB();
	delete sqlBase;
	sqlBase = nullptr;
	/**
	 * 添加自定义排序规则
	*/
	// sort(vec6VecAll.begin(), vec6VecAll.end(), cmp2());
	sort(vec6VecAll.begin(), vec6VecAll.end());
	return;
}

/**
 * 用线程池生产者和消费者方式实现：~结果还是很慢，效率跟跑单线程差不多
 * 得到 BASESQLDBDATA 所对应的所有数据块的覆盖率：
 * 最终得到的 vec6VecAll 表示未覆盖到的量保存在对应的db文件中
 * 最终得到的 vec6VecUseFull 表示gVecBingoVec中有用的数据量保存在对应的db文件中
 * 最终得到的 vec6VecUseLess 表示gVecBingoVec中无用的数据量保存在对应的db文件中
*/
void getCoverResultDatas2()
{
	int timeCount = 0;
	unsigned int index = 0;
	ThreadPool<MyTask2>* mt = new ThreadPool<MyTask2>();//创建线程池
	getVec6Vec1107568(MyTask2::mVec6VecAll);
	arrange::startTime();
	printf("-----getCoverResultDatas2------startTime-------------\r\n");
	if(false == mt->ThreadPoolInit())
	{
		printf("error:ThreadPoolInit failed.\r\n");
		return ;
	}

	for(const auto &elemVec:gVecBingoVec)
	{
		MyTask2 t(elemVec, index++);//创建任务
		mt->Push(t);//将任务推送给队列中
		sleep(1);
	}
	mt->setIfExit(true);

	while(true)
	{
		usleep(1000000);
		printf("proceeding>>>--->%d\r\n", timeCount);
		timeCount++;
		if(timeCount > 20)
		{
			creatVec6VecResult(MyTask2::mVec6VecAll , OBJECT_EXE_DIR "Vec6VecAll_letf_2");
			creatVec6VecResult(MyTask2::mVec6VecUseFull , OBJECT_EXE_DIR "vec6VecUseFull_2");
			creatVec6VecResult(MyTask2::mVec6VecUseLess , OBJECT_EXE_DIR "vec6VecUseLess_2");
			break;
		}
	}
	arrange::endTime();
	delete mt;
	mt = nullptr;
}

/**
 * 两个vector相加
 * a + b;
*/
void vectorsAdd(vector<vector<int>> &a, const vector<vector<int>> &b)
{
	set<vector<int>> uniqueElementsSet = set<vector<int>>();
	uniqueElementsSet.insert(a.begin(), a.end());
	uniqueElementsSet.insert(b.begin(), b.end());
	vector<vector<int>> mergeVec6Vec(uniqueElementsSet.begin(), uniqueElementsSet.end());
	/**
	 * 相加其实不用清空操作，内存空间肯定比之前的大
	*/
	// vector<vector<int>>().swap(a);//清空a，并且释放了a的内存空间
	a = mergeVec6Vec;
	return;
}

/**
 * 两个vector相减：前提是b是a的子集
 * a - b;
*/
void vectorsPlus(vector<vector<int>> &a, const vector<vector<int>> &b)
{
	set<vector<int>> uniqueElementsSet = set<vector<int>>();
	uniqueElementsSet.insert(a.begin(), a.end());
	for(const auto &elemvec:b)
	{
		uniqueElementsSet.erase(elemvec);
	}
	vector<vector<int>> mergeVec6Vec(uniqueElementsSet.begin(), uniqueElementsSet.end());
	vector<vector<int>>().swap(a);//清空a，并且释放了a的内存空间
	a = mergeVec6Vec;
	return;
}


unsigned long getVecAddValue(const set<vector<int>> &a, const vector<vector<int>> &b)
{
#if 0
	unsigned long add = a.size();
	set<vector<int>> uniqueElementsSet(a);
	uniqueElementsSet.insert(b.begin(), b.end());
	add = uniqueElementsSet.size() - add;

	/**
	 * 注意要释放资源，不然会内存泄漏
	*/
	set<vector<int>>().swap(uniqueElementsSet);
	uniqueElementsSet.clear();
#else
	/**
	 * 此方法会快一些
	*/
	unsigned long add = 0;
	set<vector<int>> c = set<vector<int>>();
	set<vector<int>> ret = set<vector<int>>();
	c.insert(b.begin(), b.end());
	set_difference(c.begin(), c.end(), a.begin(), a.end(), insert_iterator<set<vector<int>>>(ret,ret.begin()));
	add = ret.size();
	/**
	 * 注意要释放资源，不然会内存泄漏
	*/
	set<vector<int>>().swap(c);
	c.clear();
	set<vector<int>>().swap(ret);
	ret.clear();
#endif
	return add;
}


void getFinalVec6Vec(vector<vector<int>> &finalVec6Vec,
					 set<vector<int>> &uniqueElementsSet,
					 set<vector<int>> &leftVec6Vec,
					 const map<vector<int>, vector<vector<int>>> &LargeDatas)
{
	vector<int> insertVec6 = vector<int>();

	unsigned long addValue = 0;
	if((uniqueElementsSet.size() <= 0) || (finalVec6Vec.size() <= 0))
	{
		printf("uniqueElementsSet.size() <= 0\r\n");
		return;
	}

	printf("finalVec6Vec.size=%ld\r\n", finalVec6Vec.size());
	for(const auto &elemVec:leftVec6Vec)
	{
		unsigned long value = 0;

		value = getVecAddValue(uniqueElementsSet, LargeDatas.at(elemVec));
		if(addValue < value)
		{
			vector<int>().swap(insertVec6);
			insertVec6.clear();
			insertVec6 = elemVec;
			addValue = value;
		}
	}

	uniqueElementsSet.insert(LargeDatas.at(insertVec6).begin(), LargeDatas.at(insertVec6).end());
	finalVec6Vec.push_back(insertVec6);
	leftVec6Vec.erase(insertVec6);

	vector<int>().swap(insertVec6);
	insertVec6.clear();

	return;
}



void getLargeDatas(map<vector<int>, vector<vector<int>>> &LargeDatas,
					const vector<vector<int>> &baseVec6Vec)
{
	arrange::startTime();
	printf("-----------startTime-------------%s\r\n", __FUNCTION__);
	for(const auto &elemVec:baseVec6Vec)
	{
		vector<vector<int>> vec6Vec = vector<vector<int>>();
		getVec6VecFromDb(elemVec, vec6Vec);
		LargeDatas.insert(make_pair(elemVec, vec6Vec));
		vector<vector<int>>().swap(vec6Vec);
		vec6Vec.clear();
	}
	printf("-----------endTime-------------%ld\r\n", LargeDatas.size());
	arrange::endTime();
	return;
}

/**
 * baseVec6Vec 传值为gVecBingoVec
*/
void getCoverResultDatas4(const map<vector<int>, vector<vector<int>>> &LargeDatas,
							const vector<vector<int>> &baseVec6Vec)
{
	vector<vector<int>> finalVec6Vec = vector<vector<int>>();
	set<vector<int>> uniqueElementsSet = set<vector<int>>();
	set<vector<int>> leftVec6Vec = set<vector<int>>();
	unsigned long baseVec6VecSize = baseVec6Vec.size();

	leftVec6Vec.insert(baseVec6Vec.begin(), baseVec6Vec.end());//初始化leftVec6Vec set容器

	finalVec6Vec.push_back(baseVec6Vec[0]);
	leftVec6Vec.erase(baseVec6Vec[0]);
	uniqueElementsSet.insert(LargeDatas.at(baseVec6Vec[0]).begin(), LargeDatas.at(baseVec6Vec[0]).end());

	while(finalVec6Vec.size() < baseVec6VecSize)
	{
		arrange::startTime();
		printf("-----------startTime-------------%ld\r\n", uniqueElementsSet.size());
		getFinalVec6Vec(finalVec6Vec, uniqueElementsSet, leftVec6Vec, LargeDatas);
		arrange::endTime();
	}

	set<vector<int>>().swap(leftVec6Vec);
	leftVec6Vec.clear();

	set<vector<int>>().swap(uniqueElementsSet);
	uniqueElementsSet.clear();

	getCoverResultDatas3(LargeDatas, finalVec6Vec);
	return;
}


void genFinalBaseVec6Vec(vector<pair<vector<int>, int>> &vec6intPairVec)
{
	unsigned int effectSum = 0;
	vector<vector<int>> finalVec = vector<vector<int>>();

	/**
	 * 从大到小   反转/逆序	为从小到大
	*/
	// std::reverse(vec6intPairVec.begin(), vec6intPairVec.end());
	sort(vec6intPairVec.begin(), vec6intPairVec.end(), lowerS());
	for(const auto &elemPair:vec6intPairVec)
	{
		effectSum += elemPair.second;
		if(effectSum >= 200)
		{
			finalVec.push_back(elemPair.first);
		}
	}

	for(const auto &elemVec : finalVec)
	{
		for(const auto &elem : elemVec)
		{
			printf("%d_", elem);
		}printf("\r\n");
	}
	printf("finalVec.size=%ld\r\n", finalVec.size());

	// creatVec6VecResult(finalVec , OBJECT_EXE_DIR "FinalBaseVec6Vec");//可执行文件getVecAddValue->
	creatVec6VecResult(finalVec , OBJECT_EXE_DIR "FinalBaseVec6Vec_0201");//可执行文件FinalBaseVec6Vec_0201->
	return;
}

/**
 * 用set加insert的特性可去重复来实现：效率最高
 * 得到 BASESQLDBDATA 所对应的所有数据块的覆盖率：
*/
void getCoverResultDatas3(const map<vector<int>, vector<vector<int>>> &LargeDatas,
							const vector<vector<int>> &baseVec6Vec)
{
	unsigned int index = 0;
	unsigned long addValue = 0;
	set<vector<int>> uniqueElementsSet = set<vector<int>>();
	vector<pair<vector<int>, int>> vec6intPairVec = vector<pair<vector<int>, int>>();

	printf("baseVec6Vec.size = %ld\r\n", baseVec6Vec.size());
	for(const auto &gElemVec : baseVec6Vec)
	{
		index++;
		// arrange::startTime();
		// printf("-----------startTime----%d---------%ld\r\n", index, uniqueElementsSet.size());
		addValue = uniqueElementsSet.size();
		uniqueElementsSet.insert(LargeDatas.at(gElemVec).begin(), LargeDatas.at(gElemVec).end());
		addValue = uniqueElementsSet.size() - addValue;
		vec6intPairVec.push_back(make_pair(gElemVec, addValue));
		printf("-----------%s----%d---------%ld\r\n",__FUNCTION__ , index, addValue);
		// arrange::endTime();
		if(uniqueElementsSet.size() == 1107568)
		{
			break;
		}
	}
	/**
	 * 已去重复后生成的总的集合mergeVec6Vec
	*/
	vector<vector<int>> mergeVec6Vec(uniqueElementsSet.begin(), uniqueElementsSet.end());
	printf("mergeVec6Vec.size=%ld\r\n", mergeVec6Vec.size());

	set<vector<int>>().swap(uniqueElementsSet);
	uniqueElementsSet.clear();

	genFinalBaseVec6Vec(vec6intPairVec);

	vector<pair<vector<int>, int>>().swap(vec6intPairVec);
	vec6intPairVec.clear();
	return;
}

/**
 * 单线程实现：
 * 得到 BASESQLDBDATA 所对应的所有数据块的覆盖率：
 * 最终得到的 vec6VecAll 表示未覆盖到的量保存在对应的db文件中
 * 最终得到的 vec6VecUseFull 表示gVecBingoVec中有用的数据量保存在对应的db文件中
 * 最终得到的 vec6VecUseLess 表示gVecBingoVec中无用的数据量保存在对应的db文件中
*/
void getCoverResultDatas()
{
	vector<vector<int>> vec6VecAll;
	vector<vector<int>> vec6VecUseFull;
	vector<vector<int>> vec6VecUseLess;
	unsigned int index = 0;

	getVec6Vec1107568(vec6VecAll);
	printf("gVecBingoVec.size = %ld\r\n", gVecBingoVec.size());
	for(const auto &gElemVec : gVecBingoVec)
	{
		vector<vector<int>> vec6Vec;
		unsigned long dataInVec6VecAllSize = 0;
		index++;
		arrange::startTime();
		printf("-----------startTime----%d---------\r\n", index);
		getVec6VecFromDb(gElemVec, vec6Vec);
		dataInVec6VecAllSize = vec6VecAll.size();

		for(const auto &elemVec:vec6Vec)
		{
			vector<vector<int>>::iterator itor = std::find(vec6VecAll.begin(), vec6VecAll.end(), elemVec);
			if(itor != vec6VecAll.end())
			{//找到就删除
				vec6VecAll.erase(itor);
			}
		}

		if((dataInVec6VecAllSize - vec6VecAll.size()) > 84)//算有变化
		{
			vec6VecUseFull.push_back(gElemVec);
			printf("vec6VecAll.size=%ld\r\n", vec6VecAll.size());
		}
		else
		{
			vec6VecUseLess.push_back(gElemVec);
		}
		arrange::endTime();

		if((vec6VecAll.size() < 10000)&&(vec6VecAll.size() >= 1000))
		{
			printf("\tindex=%d\r\n", index);
		}
		else if((vec6VecAll.size() < 1000)&&(vec6VecAll.size() > 0))
		{
			printf("\tindex=%d\r\n", index);
			break;
		}
	}

	creatVec6VecResult(vec6VecAll, OBJECT_EXE_DIR "Vec6VecAll_letf");
	creatVec6VecResult(vec6VecUseFull, OBJECT_EXE_DIR "vec6VecUseFull");
	creatVec6VecResult(vec6VecUseLess, OBJECT_EXE_DIR "vec6VecUseLess");
	return;
}

/**
 * 查找两个数据块的交集的数目
*/
void findTheSameAmountWithComparedVec6Vecs(/*const vector<int> &vec6A,const vector<int> &vec6B*/)
{
	const vector<int> vec6A{4,6,7,10,13,25};
	const vector<int> vec6B{4,9,12,17,30,32};
	unsigned int ret = 0;
	vector<vector<int>> vec6VecA;
	vector<vector<int>> vec6VecB;
	getVec6VecFromDb(vec6A, vec6VecA);
	getVec6VecFromDb(vec6B, vec6VecB);
	ret = getTheSameAmountFromVec6Vecs(vec6VecA, vec6VecB);
	printf("the same amount is %d\r\n", ret);
}

unsigned int getTheSameAmountFromVec6Vecs(const vector<vector<int>> &vec6VecA, const vector<vector<int>> &vec6VecB)
{
	unsigned int sameAmount = 0;
	for(const auto &elemVecA:vec6VecA)
	{
		for(const auto &elemVecB:vec6VecB)
		{
			if(elemVecA == elemVecB)
			{
				sameAmount++;
			}
		}
	}
	return sameAmount;
}

void getVec6VecFromDb(const vector<int> &vec6, vector<vector<int>> &vec6Vec)
{
	sqlite_tb *sql = nullptr;
	string sqlDbFile = SAVE6RET_DB_FILEDIR;
	string dataStr = "";

	for(const auto &elem : vec6)
	{
		dataStr += arrange::NumberToString<int>(elem) + "_";
	}
	// printf("[\t %s \t]\r\n", dataStr.c_str());
	sqlDbFile += dataStr;
	sql = new sqlite_tb(sqlDbFile.c_str());
	sql->OpenDB();
	sql->SelectAllData(vec6Vec);
	sql->CloseDB();
	delete sql;
	sql = nullptr;
	return;
}


/**
 * 创建最终gVecBingoVec中的每个（包含6个号码的）子集合的所代表的数据块，命中期数的统计~
 * 命中结果的排序已按照由大到小已排列好~
*/
void createFinalResultSqlDb()
{
	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(FINAL_RET_DB_FILE);
	if(nullptr == sql)
	{
		printf("sql is nullptr.\r\n");
		return;
	}
    // 打开数据库，不存在，创建数据库db
	sql->OpenDB();
	// 创建表
	const char* sqlcmd = "create table if not exists tbldatas("
							"bingovec text not null, rate int not null default 0, "
							"UNIQUE(bingovec, rate));";
	sql->CreateTable(sqlcmd);

	{
		vector<pair<vector<int>, int>> save8RetVec(MyTask::mSave8RetMap.begin(), MyTask::mSave8RetMap.end());
		std::sort(save8RetVec.begin(), save8RetVec.end(), lowerS());//按照value值排序，不是按照key值；
		for(const auto &pairelem : save8RetVec)
		{
			string insertData = "(\"";
			for(const auto &elem : pairelem.first)
			{
				printf("%d_", elem);
				insertData += arrange::NumberToString<int>(elem) + "_";
			}
			printf(",\t%d\r\n", pairelem.second);
			insertData += "\"," + arrange::NumberToString<int>(pairelem.second) + ");";
			// 插入数据
			sql->InsertData(insertData);
		}

		printf("MyTask::mSave8RetMap.size = %ld\r\n", MyTask::mSave8RetMap.size());
	}

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	return;
}



#if USE_TIMER
/**
 * 定时器的使用；
*/
void set_timer()
{
    struct itimerval itv, oldtv;
    //~ 定时间隔1秒0微秒
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    //~ 初始计时
    itv.it_value = itv.it_interval;
    //~ 打开定时器ITIMER_REAL
    setitimer(ITIMER_REAL, &itv, &oldtv);
}

//~ 定时器中断处理程序入口
static void sigalrm_handler(int sig)
{
    cnt++;
    // printf("timer trigered .. %d \r\n", cnt);
	if(0 == cnt%10)
	{
		arrange::mUsed = true;
	}
}
#endif


void createBaseSqlDb_1107568()
{
	const vector<int> oriVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
							  11,12,13,14,15,16,17,18,19,20,
							  21,22,23,24,25,26,27,28,29,30,
							  31,32,33 };//
	// const vector<int> oriVec{2,8,9,10,11,16,17,29};

	arrange* arrg = nullptr;
	const char* dbName = "./ret/baseSqliteDbData_1107568";
	arrg = new arrange(dbName, oriVec, 6);
	if(nullptr == arrg)
	{
		printf("arrg new false.\r\n");
		return;
	}

	if(false == arrg->init())
	{
		printf("arrange.init false.\r\n");
		return;
	}
	arrg->startTime();
    arrg->sqlRecursion(0, 0);
	arrg->endTime();

	delete arrg;
	arrg = nullptr;
	return;
}

/**
 * createBaseSqlDb->生成gVecBingoVec；
*/
void createBaseSqlDb()
{
	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(BASESQLDBDATA);

	if(nullptr == sql)
	{
		printf("sql is nullptr.\r\n");
		return;
	}
	gVecBingoVec.clear();
    // 打开数据库，不存在，创建数据库db
	sql->OpenDB();

	sql->SelectAllData(gVecBingoVec);

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	/**
	 * 去重复操作~
	*/
	checkVecBingoVec(gVecBingoVec);
	/**
	 * 重新按cmp2规则排序~
	*/
	// sort(gVecBingoVec.begin(), gVecBingoVec.end(), cmp2());
	return;
}

template<typename T>
bool checkELemIsInVec(const T &data, const vector<T> &vec)
{
	bool ret = false;
	for(const auto &elem : vec)
	{
		if(data == elem)
		{
			ret = true;
		}
	}
	return ret;
}


template<typename T>
bool findElemIsInVec(const T &elem, const vector<T> &vec)
{
	/**
	 * 不用事先进行sort
	*/
	return (std::find(vec.begin(), vec.end(), elem) != vec.end());
}


/**
 * 二分法查找
 * 对T是普通类型是有效，如int char等等，对于T是容器类型则不行~
*/
template<typename T>
bool binarySearchElemIsInVec(const T &elem, const vector<T> &vec)
{
	/**
	 * 前提：vec必须是已经sort排序好了的集合；
	 * 先sort(vec.begin(), vec.end());
	*/
	return (std::binary_search(vec.begin(), vec.end(), elem));
}

enum{
	RET_BIGGER,
	RET_SMALLER,
	RET_EQUAL,
	RET_ERROR,
};

/**
 * return
 * 	TRUE:a<b
 * 	FALSE:a>=b
*/
int compare2vec(const vector<int> &a, const vector<int> &b)
{
	unsigned int size = static_cast<unsigned int>((a.size()<b.size())?(a.size()):(b.size()));

	if(a == b)
	{
		return RET_EQUAL;
	}

	for(unsigned int i=0;i<size;i++)
	{
		if(a[i]!=b[i])
		{
			return ((a[i]<b[i])?(RET_SMALLER):(RET_BIGGER));
		}
	}
	return RET_ERROR;//  ">"---从大到小重新排序;   "<"---从小到大重新排序，
}


/**
 * 二分法查找
 * 效率快，且少占内存，放心使用，但是必须先进行sort进行有序（从大到小或者从小到大）排序后再用~
 * 先sort(vec.begin(), vec.end(), cmp2());再调用   ->cmp2()是自定义从大到小或者从小到大排序的规则~
*/
bool binarySearchElemVecIsInVecVec2(const vector<int> &elem, const vector<vector<int>> &vec)
{
	unsigned long half = 0;
	unsigned long start = 0;
	unsigned long end = vec.size()-1;
	while(1)
	{	half = (start+end)/2;
		if(1 == (end-start))
		{
			if((elem == vec[start])||(elem == vec[end]))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		int ret = compare2vec(elem, vec[half]);
		if(RET_EQUAL == ret)
		{//	elem == vec[half]
			return true;
		}
		else if(RET_BIGGER == ret)
		{//	vec[half] < elem
			start = half;
			// printf("--%ld\r\n", end-start);
		}
		else if(RET_SMALLER == ret)
		{// elem < vec[half]
			end = half;
			// printf("--%ld\r\n", end-start);
		}
		else
		{
			return false;
		}
	}
}


#define 	WAY		1
/**
 * 找basesqldata的函数接口；
 * 其中关键点是设置：if(i>=3 && i<6)//->
 * A:if(i>=2 && i<6)//->  则得到的basesqldata为互不重叠数目不超过1(即<=1)的集合；
 * B:if(i>=3 && i<6)//->  则得到的basesqldata为互不重叠数目不超过2(即<=2)的集合；
 * C:if(i>=4 && i<6)//->  则得到的basesqldata为互不重叠数目不超过3(即<=3)的集合；
 *
 * 当总量为1107568时，则情况A的结果为 23
 * 当总量为1107568时，则情况B的结果为 139   能覆盖：1107568-476904=630664个条目数
 * 当总量为1107568时，则情况C的结果为 1528	能覆盖：1107568，完美
 *
 * 每个数据块大小为((27*26)/(2*1))*((8*7)/(2*1)) =  351*28 = 9828;
 * 但集合中有重叠的部分，所以最终得到的没有重复的集合的组合数为：5428
 *
 * 当两个6个号码，重叠数字个数不超过1，即情况A，则两个数据块的交集条目数为 0；
 * 当两个6个号码，重叠数字个数不超过2，即情况B，则两个数据块的交集条目数为 36；
 * 当两个6个号码，重叠数字个数不超过3，即情况C，则两个数据块的交集条目数为 261；
 * 当两个6个号码，重叠数字个数不超过4，即情况D，则两个数据块的交集条目数为 828；
*/
void findBaseSqlData(const vector<vector<int>> &vecBingoVec)
{
	vector<vector<int>> vecBingoVecRet = vecBingoVec;//直接赋值效率更快

	printf("%s--%d--Start~--vecBingoVecRet.size=%ld\r\n",__func__, __LINE__, vecBingoVecRet.size());
	unsigned long index_j = 0;
	int delSame = 0;
	int delDiff = 0;
	for(const auto &elemVec1 : vecBingoVec)
	{
		index_j++;
		// printf("%s--%d--%d--%ld---%ld\r\n",__func__, __LINE__, index_j, vecBingoVec.size(), vecBingoVecRet.size());
		unsigned int index_i = 0;
		bool isIn = false;

	#if(WAY == 0)
		isIn = checkELemIsInVec<vector<int>>(elemVec1, vecBingoVecRet);
	#else
		// isIn = binarySearchElemVecIsInVecVec2(elemVec1, vecBingoVecRet);	////childBingoFinal_9 高效且占内存少~
		isIn = findElemIsInVec<vector<int>>(elemVec1, vecBingoVecRet);
	#endif

		printf("131313131313-index_j=%ld---isIn=%d\r\n", index_j, isIn);
		if(!isIn)
		{
			continue;
		}

		/**
		 * 这种方法效率更快~
		*/
		for(vector<vector<int>>::iterator itor=vecBingoVecRet.begin(); itor!=vecBingoVecRet.end(); itor++)
		{
			int i = 0;
			if((*itor).size() > 0)
			{
				for(const auto &elem2 : (*itor))
				{
				#if(WAY == 0)
					if(checkELemIsInVec<int>(elem2, elemVec1))//
				#else
					if(findElemIsInVec<int>(elem2, elemVec1))//
				#endif
					{
						i++;
					}
				}

				if(i==6)
				{
					index_i++;
					if(index_i>1)//只保留有且只有一份数据；每条数据的唯一性；
					{
						delSame++;
						itor = vecBingoVecRet.erase(itor);
						/**
						 * 本来erase操作就是删除目标迭代器的元素，目标迭代器的后面的元素都往前移动一位，
						 * 且返回指向目标迭代器的下一个元素的迭代器
						 * 此处需注意在操作erase是，返回的迭代器需要执行自减(itor--,而不是直接break)（且需判断是否是begin），来中和for循环里面的自加，
						 * 不然会出现段错误或者有漏球~！！！
						*/
						if(itor!=vecBingoVecRet.begin())
							itor--;
						/**
						 * 谨防用下列语句来执行，有漏球~
						*/
						// if(itor==vecBingoVecRet.end())
						// 		break;
					}
				}
				else if(i>=4 && i<6)
				{
					// if(i == 2)
					// {
					// 	printf("[\t");
					// 	for(const auto elem:elemVec1)
					// 	{
					// 		printf("%d_", elem);
					// 	}printf("\t|\t");
					// 	for(const auto elem:(*itor))
					// 	{
					// 		printf("%d_", elem);
					// 	}printf("\t]\n");
					// }
					delDiff++;
					itor = vecBingoVecRet.erase(itor);
					/**
					 * 本来erase操作就是删除目标迭代器的元素，目标迭代器的后面的元素都往前移动一位，
					 * 且返回指向目标迭代器的下一个元素的迭代器
					 * 此处需注意在操作erase是，返回的迭代器需要执行自减（且需判断是否是begin），来中和for循环里面的自加，
					 * 不然会出现段错误或者有漏球~！！！
					*/
					if(itor!=vecBingoVecRet.begin())
						itor--;
					/**
					 * 谨防用下列语句来执行，有漏球~
					*/
					// if(itor==vecBingoVecRet.end())
					// 		break;
				}
			}
		}
		// printf("delSame=%d, delDiff=%d\r\n", delSame, delDiff);
	}
	printf("delSame=%d, delDiff=%d\r\n", delSame, delDiff);
	printf("vecBingoVecRet.size=%ld\r\n", vecBingoVecRet.size());
	/**
	 * 这里生成的就是 BASESQLDBDATA  所用到的文件
	*/
	creatVec6VecResult(vecBingoVecRet, BASESQLDBDATA "_13");
	printf("%s--%d--End~\r\n",__func__, __LINE__);
	return;
}

void findBaseSqlData2(const vector<vector<int>> &vecBingoVec)
{
	vector<vector<int>> vecBingoVecRet = vecBingoVec;//直接赋值效率更快

	printf("%s--%d--Start~--vecBingoVecRet.size=%ld\r\n",__func__, __LINE__, vecBingoVecRet.size());
	unsigned long index_j = 0;
	int delSame = 0;
	int delDiff = 0;
	for(const auto &elemVec1 : vecBingoVec)
	{
		index_j++;
		// printf("%s--%d--%d--%ld---%ld\r\n",__func__, __LINE__, index_j, vecBingoVec.size(), vecBingoVecRet.size());
		unsigned int index_i = 0;
		bool isIn = false;

	#if(WAY == 0)
		isIn = checkELemIsInVec<vector<int>>(elemVec1, vecBingoVecRet);
	#else
		// isIn = binarySearchElemVecIsInVecVec2(elemVec1, vecBingoVecRet);	////childBingoFinal_9 高效且占内存少~
		isIn = findElemIsInVec<vector<int>>(elemVec1, vecBingoVecRet);
	#endif

		printf("131313131313-index_j=%ld---isIn=%d\r\n", index_j, isIn);
		if(!isIn)
		{
			continue;
		}

		vector<vector<int>> deleteVecVec;
		for(const auto &elem2vec : vecBingoVecRet)
		{
			if(elem2vec == elemVec1)
			{
				index_i++;
				if(index_i>1)//只保留有且只有一份数据；每条数据的唯一性；
				{
					delSame++;
					deleteVecVec.push_back(elem2vec);
				}
			}
			else
			{
				int i = 0;
				for(const auto &elem2 : elem2vec)
				{
				#if(WAY == 0)
					if(checkELemIsInVec<int>(elem2, elemVec1))//
				#else
					if(findElemIsInVec<int>(elem2, elemVec1))//
				#endif
					{
						i++;
					}
				}

				if(i>=3 && i<6)
				{
					delDiff++;
					deleteVecVec.push_back(elem2vec);
				}
			}
		}

		// printf("vecBingoVecRet-deleteVecVec=%ld\r\n", vecBingoVecRet.size()-deleteVecVec.size());
		for(const auto &elem2vec : deleteVecVec)
		{
			// printf("[\t");
			// for(const auto elem:elemVec1)
			// {
			// 	printf("%d_", elem);
			// }printf("\t|\t");
			// for(const auto elem:elem2vec)
			// {
			// 	printf("%d_", elem);
			// }printf("\t]\n");
			vecBingoVecRet.erase(std::find(vecBingoVecRet.begin(), vecBingoVecRet.end(), elem2vec));
		}
		// printf("vecBingoVecRet.size=%ld\r\n", vecBingoVecRet.size());
		// printf("delSame=%d, delDiff=%d\r\n", delSame, delDiff);
	}
	printf("delSame=%d, delDiff=%d\r\n", delSame, delDiff);
	printf("vecBingoVecRet.size=%ld\r\n", vecBingoVecRet.size());
	/**
	 * 这里生成的就是 BASESQLDBDATA  所用到的文件
	*/
	creatVec6VecResult(vecBingoVecRet, BASESQLDBDATA "_13_1");
	printf("%s--%d--End~\r\n",__func__, __LINE__);
	return;
}

void findBaseSqlData3(const vector<vector<int>> &vecBingoVec)
{
	vector<vector<int>> vecBingoVecRet = vecBingoVec;//直接赋值效率更快

	printf("%s--%d--Start~--vecBingoVecRet.size=%ld\r\n",__func__, __LINE__, vecBingoVecRet.size());
	unsigned long index_j = 0;
	int delSame = 0;
	int delDiff = 0;
	for(const auto &elemVec1 : vecBingoVec)
	{
		index_j++;
		// printf("%s--%d--%d--%ld---%ld\r\n",__func__, __LINE__, index_j, vecBingoVec.size(), vecBingoVecRet.size());
		unsigned int index_i = 0;
		bool isIn = false;

	#if(WAY == 0)
		isIn = checkELemIsInVec<vector<int>>(elemVec1, vecBingoVecRet);
	#else
		// isIn = binarySearchElemVecIsInVecVec2(elemVec1, vecBingoVecRet);	////childBingoFinal_9 高效且占内存少~
		isIn = findElemIsInVec<vector<int>>(elemVec1, vecBingoVecRet);
	#endif

		printf("131313131313-index_j=%ld---isIn=%d\r\n", index_j, isIn);
		if(!isIn)
		{
			continue;
		}

		/**
		 * 这种方法效率更快~
		*/
		for(vector<vector<int>>::iterator itor=vecBingoVecRet.begin(); itor!=vecBingoVecRet.end(); itor++)
		{
			int i = 0;
			for(const auto &elem2 : (*itor))
			{
			#if(WAY == 0)
				if(checkELemIsInVec<int>(elem2, elemVec1))//
			#else
				if(findElemIsInVec<int>(elem2, elemVec1))//
			#endif
				{
					i++;
				}
			}

			if(i==6)
			{
				index_i++;
				if(index_i>1)//只保留有且只有一份数据；每条数据的唯一性；
				{
					delSame++;
					itor = vecBingoVecRet.erase(itor);
					/**
					 * 本来erase操作就是删除目标迭代器的元素，目标迭代器的后面的元素都往前移动一位，
					 * 且返回指向目标迭代器的下一个元素的迭代器
					 * 此处需注意在操作erase是，返回的迭代器需要执行自减(itor--,而不是直接break)（且需判断是否是begin），来中和for循环里面的自加，
					 * 不然会出现段错误或者有漏球~！！！
					*/
					if(itor!=vecBingoVecRet.begin())
						itor--;
					/**
					 * 谨防用下列语句来执行，有漏球~
					*/
					// if(itor==vecBingoVecRet.end())
					// 		break;
				}
			}
			else if(i>=4 && i<6)
			{
				// if(i == 2)
				// {
				// 	printf("[\t");
				// 	for(const auto elem:elemVec1)
				// 	{
				// 		printf("%d_", elem);
				// 	}printf("\t|\t");
				// 	for(const auto elem:(*itor))
				// 	{
				// 		printf("%d_", elem);
				// 	}printf("\t]\n");
				// }
				delDiff++;
				itor = vecBingoVecRet.erase(itor);
				/**
				 * 本来erase操作就是删除目标迭代器的元素，目标迭代器的后面的元素都往前移动一位，
				 * 且返回指向目标迭代器的下一个元素的迭代器
				 * 此处需注意在操作erase是，返回的迭代器需要执行自减（且需判断是否是begin），来中和for循环里面的自加，
				 * 不然会出现段错误或者有漏球~！！！
				*/
				if(itor!=vecBingoVecRet.begin())
					itor--;
				/**
				 * 谨防用下列语句来执行，有漏球~
				*/
				// if(itor==vecBingoVecRet.end())
				// 		break;
			}
		}
		// printf("delSame=%d, delDiff=%d\r\n", delSame, delDiff);
	}
	printf("delSame=%d, delDiff=%d\r\n", delSame, delDiff);
	printf("vecBingoVecRet.size=%ld\r\n", vecBingoVecRet.size());
	/**
	 * 这里生成的就是 BASESQLDBDATA  所用到的文件
	*/
	creatVec6VecResult(vecBingoVecRet, BASESQLDBDATA "_13");
	printf("%s--%d--End~\r\n",__func__, __LINE__);
	return;
}

/**
 * 生成截止目前的bingo的号码的统计生成db文件
*/
void createBingoSql()
{
	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb();
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
                            "red1 int not null default 0,red2 int not null default 0,red3 int not null default 0,"
                            "red4 int not null default 0,red5 int not null default 0,red6 int not null default 0,"
                            "blue1 int not null default 0);";
	sql->CreateTable(sqlcmd);
	/**
	 * 插入数据：这里默认从SOURCE_FILE文件中读取数据后导入~
	*/
	sql->InsertData();
	/**
	 * 删除操作
	*/
	// sql->DeleteData(2023032);
	/**
	 * 更新
	*/
	// sql->UpdateData();

	/**
	 * 获取总条目数
	*/
	sql->SelectGetTotalRows();

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	return;
}


void mathematics()
{
	long ret6 = (33*32*31*30*29*28)/(6*5*4*3*2*1);
	long ret8 = ((33L*32*31*30*29*28*27*26)/(8*7*6*5*4*3*2*1));
	long ret = 3000*(27*26)/(2*1);
	printf("ret6=%ld\r\n", ret6);
	printf("ret8=%ld\r\n", ret8);
	printf("ret=%ld\r\n", ret);
	return;
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
		// vector<int>::iterator itor = vred.begin();
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


void func1(sqlite_tb *psql, vector<int> &vred, const vector<int> &vblue)
{
	map<int32_t, int32_t> retm;

	if(vred.size() <= 22)
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
		vector<int> initvred = vred;
		int count = 0;
		int index = initvred[i];
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

#if 1
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


