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
void createVec6vecFromSqlDb(vector<vector<int>> &vec6Vec, const char* sqldbfile);
void createFinalResultSqlDb();
void checkResult();
void checkVec6VecUnique(vector<vector<int>> &vecBingoVec);
void getCoverResultDatas();
void getCoverResultDatas2();
void getCoverResultDatas3(const map<vector<int>, vector<vector<int>>> &LargeDatas,
							const vector<vector<int>> &baseVec6Vec);
void getCoverResultDatas4(const map<set<int>, set<set<int>>> &LargeDatas,
							const set<set<int>> &baseSetSet);
void getCoverResultDatas5(const map<vector<int>, vector<vector<int>>> &LargeDatas,
							const vector<vector<int>> &baseVec6Vec);
void getCoverResultDatas7(const map<vector<int>, set<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec);
void getCoverResultDatas8(const map<vector<int>, set<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec);
void getCoverResultDatas9(const map<vector<int>, set<vector<int>>> &LargeDatas, \
							const vector<vector<int>> &baseVec6Vec, \
							vector<vector<int>> &finalVec6Vec);
void creatVec6VecResult(const vector<vector<int>> &vec6Vec, const char* dbname);
void findBaseSqlData(const vector<vector<int>> &vecBingoVec);
void findBaseSqlData3(const vector<vector<int>> &vecBingoVec);
void getVec6VecFromDb(const vector<int> &vec6, vector<vector<int>> &vec6Vec);
void getSet6SetFromDb(const set<int> &set6, set<set<int>> &set6Set);
void getSet6VecFromDb(const vector<int> &vec6, set<vector<int>> &set6Vec);
unsigned int getTheSameAmountFromVec6Vecs(const vector<vector<int>> &vec6VecA, const vector<vector<int>> &vec6VecB);
void findTheSameAmountWithComparedSet6Set();
void genVec6VecForFindBaseSqlData();
void getVec6Vec1107568(vector<vector<int>> &vec6VecAll);
void startTask();
void initObjectDir();
void testGetCoverResultDatas2();
void getLargeSetSetDatas(map<set<int>, set<set<int>>> &LargeDatas,const set<set<int>> &baseVec6Vec);
void getLargeVecVecDatas(map<vector<int>, vector<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec);
void getLargeSetVecDatas(map<vector<int>, set<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec);
void convertVecVec2SetSet(const vector<vector<int>> &vecVec, set<set<int>> &setSet);
void convertSetSet2VecVec(const set<set<int>> &setSet, vector<vector<int>> &vecVec);

#if USE_TIMER
static void sigalrm_handler(int sig);
void set_timer();
static unsigned long cnt = 0;
#endif


static vector<vector<int>> gbaseVec6Vec = vector<vector<int>>();//要去重复
static vector<vector<int>> gFinalVec6Vec = vector<vector<int>>();

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

void testMergeVec(const vector<vector<int>> &vectorA);
void testMergeSet(const set<set<int>> &setA);
void testMergeSetVec(const vector<vector<int>> &vectorA);

typedef void (*sighandler_t) (int);  //定义sighandler_t类型

void catchSigIntSaveFinalVec6VecToDbFile(int signo)
{
	arrange::endTime();
	printf("handle start-----------------catch--%d--%ld\n", signo, gFinalVec6Vec.size());
	creatVec6VecResult(gFinalVec6Vec , FINALVEC6VEC_DB);
	printf("handle end-----------------catch--%d\n", signo);
	arrange::startTime();
	printf("-----------startTime-------------\r\n");
}


/**
 * 推荐使用~
 * 信号队列中，屏蔽连续执行的相同信号只执行一次，在信号执行函数执行期间，该信号已从队列中吐出~
*/
void regSignalHandle2()
{
	int ret = 0;
	struct sigaction act;

    act.sa_handler = catchSigIntSaveFinalVec6VecToDbFile;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT); //信号捕捉函数执行期间sa_mask屏蔽字中，3号信号置1  3号信号屏蔽     置1就是屏蔽，置0就是不屏蔽
	sigaddset(&act.sa_mask, SIGINT);  //信号捕捉函数执行期间 sa_mask屏蔽字中，2号信号置1  2号信号屏蔽     置1就是屏蔽，置0就是不屏蔽
    act.sa_flags = 0;  //默认属性   信号捕捉函数执行期间，自动屏蔽本信号

    ret = sigaction(SIGINT, &act, NULL);   //注册2号信号
    if (ret == -1) {
        perror("sigaction error");
        exit(1);
    }
	return;
}

void regSignalHandle1()
{
	sighandler_t handler;
    handler = signal(SIGINT, catchSigIntSaveFinalVec6VecToDbFile);  //注册2号信号
    if(handler == SIG_ERR)
	{
        perror("signal error");
        exit(1);
    }
	return;
}


int main(int argc, char** argv)
{

	set<set<int>> gBaseSetSet = set<set<int>>();
	map<set<int>, set<set<int>>> LargeDatas = map<set<int>, set<set<int>>>();
	map<vector<int>, vector<vector<int>>> LargeMapVecVecDatas = map<vector<int>, vector<vector<int>>>();
	map<vector<int>, set<vector<int>>> LargeMapSetVecDatas = map<vector<int>, set<vector<int>>>();
	initObjectDir();
	regSignalHandle2();
#if 1
	createBingoSql();
	createVec6vecFromSqlDb(gbaseVec6Vec, BASESQLDBDATA);//生成  gbaseVec6Vec
	createVec6vecFromSqlDb(gFinalVec6Vec, FINALVEC6VEC_DB);//生成  gFinalVec6Vec
	// convertVecVec2SetSet(gbaseVec6Vec, gBaseSetSet);

	// vector<vector<int>> vec6VecAll = vector<vector<int>>();
	// getVec6Vec1107568(vec6VecAll);
	// convertVecVec2SetSet(vec6VecAll, gBaseSetSet);
	// testMergeVec(vec6VecAll);
	// testMergeSet(gBaseSetSet);
	// testMergeSetVec(vec6VecAll);

	arrange::startTime();
	printf("-----------startTime-------------\r\n");
	// genVec6VecForFindBaseSqlData();//(按规则)查找BaseSqlData
	// checkResult();
	// testGetCoverResultDatas2();//
	// getLargeSetSetDatas(LargeDatas, gBaseSetSet);getCoverResultDatas4(LargeDatas, gBaseSetSet);
	// getLargeVecVecDatas(LargeMapVecVecDatas, gbaseVec6Vec);getCoverResultDatas5(LargeMapVecVecDatas, gbaseVec6Vec);

	// getLargeSetVecDatas(LargeMapSetVecDatas, gbaseVec6Vec);getCoverResultDatas7(LargeMapSetVecDatas, gbaseVec6Vec);
	getLargeSetVecDatas(LargeMapSetVecDatas, gbaseVec6Vec);getCoverResultDatas9(LargeMapSetVecDatas, gbaseVec6Vec, gFinalVec6Vec);//效率最高

	// findTheSameAmountWithComparedSet6Set();
	arrange::endTime();
	unsigned int index = 0;
	while(true)
	{
		index++;
		printf("proceeding>>>\r\n");
		usleep(1000000);
		if(index > 50)
		{
			break;
		}
	}
  #else
	// createBaseSqlDb_1107568();
	createBingoSql();
	createVec6vecFromSqlDb(gbaseVec6Vec, BASESQLDBDATA);//生成  gbaseVec6Vec
	createVec6vecFromSqlDb(gFinalVec6Vec, FINALVEC6VEC_DB);//生成  gFinalVec6Vec
	#if USE_TIMER
	//~ 设定信号SIGALRM的处理函数
    signal(SIGALRM, sigalrm_handler);
    set_timer();
	#endif
	sqlite_tb::mDebug = true;
	startTask();
	// getLargeVecVecDatas(LargeMapVecVecDatas, gbaseVec6Vec);getCoverResultDatas3(LargeDatas, gbaseVec6Vec);//验证得到的所有数据块的覆盖率
	// getLargeSetVecDatas(LargeMapSetVecDatas, gbaseVec6Vec);getCoverResultDatas8(LargeMapSetVecDatas, gbaseVec6Vec);
#endif

	gBaseSetSet.clear();
	set<set<int>>().swap(gBaseSetSet);
	LargeDatas.clear();
	map<set<int>, set<set<int>>>().swap(LargeDatas);
	LargeMapVecVecDatas.clear();
	map<vector<int>, vector<vector<int>>>().swap(LargeMapVecVecDatas);
	LargeMapSetVecDatas.clear();
	map<vector<int>, set<vector<int>>>().swap(LargeMapSetVecDatas);

	gbaseVec6Vec.clear();
	vector<vector<int>>().swap(gbaseVec6Vec);
	gFinalVec6Vec.clear();
	vector<vector<int>>().swap(gFinalVec6Vec);

	return 0;
}

void testMergeSetVec(const vector<vector<int>> &vectorA)
{
	set<vector<int>> aaa(vectorA.begin(), vectorA.end());
	set<vector<int>> bbb(aaa);
	arrange::startTime();
	printf("-----111111------startTime----%s---------\r\n", __FUNCTION__);
	bbb.insert(aaa.begin(), aaa.end());
	arrange::endTime();

	vector<vector<int>> ccc(aaa.begin(), aaa.end());

	printf("-----22222------startTime----%ld---------\r\n", ccc.size());
	arrange::startTime();
	ccc.insert(ccc.end(), vectorA.begin(), vectorA.end());
	// sort(ccc.begin(), ccc.end());
	// auto iter = std::unique(ccc.begin(), ccc.end());
    // ccc.erase(iter, ccc.end());
	// vector<vector<int>>(ccc).swap(ccc);
	set<vector<int>> ddd(ccc.begin(), ccc.end());
	printf("-----22222------startTime----%ld---------\r\n", ddd.size());
	arrange::endTime();

	aaa.clear();set<vector<int>>().swap(aaa);
	bbb.clear();set<vector<int>>().swap(bbb);
	ccc.clear();vector<vector<int>>().swap(ccc);
	// ddd.clear();set<vector<int>>().swap(ddd);
}


void testMergeVec(const vector<vector<int>> &vectorA)
{
	vector<vector<int>> tmpVecA = vector<vector<int>>();
	vector<vector<int>> tmpVecB = vector<vector<int>>();
	arrange::startTime();
	printf("-----typedef------startTime----%s---------\r\n", __FUNCTION__);
	vector<vector<int>> tmpVec(vectorA);
	arrange::endTime();

	tmpVecA.resize(vectorA.size() * 2);
	arrange::startTime();
	printf("-----merge1------startTime----%s---------\r\n", __FUNCTION__);
	merge(tmpVec.begin(), tmpVec.end(), vectorA.begin(), vectorA.end(), tmpVecA.begin());
	arrange::endTime();

	arrange::startTime();
	printf("-----merge2------startTime----%s---------\r\n", __FUNCTION__);
	merge(tmpVec.begin(), tmpVec.end(), vectorA.begin(), vectorA.end(), back_inserter(tmpVecB));
	arrange::endTime();

	arrange::startTime();
	printf("-----insert------startTime----%s---------\r\n", __FUNCTION__);
	tmpVec.insert(tmpVec.end(), vectorA.begin(), vectorA.end());//最优
	arrange::endTime();

	vector<vector<int>> tmpVecC(vectorA);
	arrange::startTime();
	printf("-----push_back------startTime----%s---------\r\n", __FUNCTION__);
	for(const auto &xxx:vectorA)//最优
	{
		tmpVecC.push_back(xxx);
	}
	arrange::endTime();

	vector<vector<int>> tmpVecD(vectorA);
	tmpVecD.resize(vectorA.size() * 2);
	arrange::startTime();
	printf("-----resize push_back------startTime----%s---------\r\n", __FUNCTION__);
	for(const auto &xxx:vectorA)
	{
		tmpVecC.push_back(xxx);
	}
	arrange::endTime();
}

void testMergeSet(const set<set<int>> &setA)
{
	arrange::startTime();
	printf("-----typedef------startTime----%s---------\r\n", __FUNCTION__);
	set<set<int>> tmpSetA(setA);//最优
	arrange::endTime();

	arrange::startTime();
	printf("-----insert------startTime----%s---------\r\n", __FUNCTION__);
	tmpSetA.insert(setA.begin(), setA.end());
	arrange::endTime();

	arrange::startTime();
	printf("-----typedef2------startTime----%s---------\r\n", __FUNCTION__);
	set<set<int>> tmpSetB = set<set<int>>();
	tmpSetB.insert(setA.begin(), setA.end());
	arrange::endTime();

	set<set<int>> tmpSetC(setA);//最优
	set<set<int>> tmpSetR = set<set<int>>();
	vector<vector<int>> tmpVecD = vector<vector<int>>();
	vector<vector<int>> tmpVecE = vector<vector<int>>();
	arrange::startTime();
	printf("-----convertSetSet2VecVec-convertSetSet2VecVec-----startTime----%s---------\r\n", __FUNCTION__);
	convertSetSet2VecVec(tmpSetC, tmpVecD);
	convertSetSet2VecVec(setA, tmpVecE);
	tmpVecD.insert(tmpVecD.end(), tmpVecE.begin(), tmpVecE.end());
	convertVecVec2SetSet(tmpVecD, tmpSetR);
	arrange::endTime();
}


/**
 * set容器默认是有自动从小到大排序好了的
 * set 的insert可以合并去重复 ， vector不行
 * set_union		取合并部分  a+b		a,b的合集，已去掉重复数据，里面是数据都是唯一的；
 * set_difference	取不同部分	a-b		a有b没有
 * set_intersection	取共有部分	a&b		a,b 都有
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
	for(const auto &elemVec:gbaseVec6Vec)
	{
		MyTask t(elemVec);//创建任务
    	mt->Push(t);//将任务推送给队列中
		sleep(1);
	}
	arrange::endTime();
	mt->setIfExit(true);

	while(true)
    {
		if(MyTask::mSave8RetMap.size() < gbaseVec6Vec.size())
		{
			printf("proceeding>>>\r\n");
			usleep(1000000);
		}
		else if(MyTask::mSave8RetMap.size() == gbaseVec6Vec.size())
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
void checkVec6VecUnique(vector<vector<int>> &vec6Vec)
{
#if 1
	set<vector<int>> set6Vec(vec6Vec.begin(), vec6Vec.end());
	if(vec6Vec.size() != set6Vec.size())
	{
		vector<vector<int>> tmp(set6Vec.begin(), set6Vec.end());
		tmp.swap(vec6Vec);
		tmp.clear();
		vector<vector<int>>().swap(tmp);
	}
	set6Vec.clear();
	set<vector<int>>().swap(set6Vec);
#else
	vector<vector<int>> vecVec(vec6Vec);
	for(const auto &elemVec1:vecVec)
	{
		unsigned int index =  0;
		for(vector<vector<int>>::iterator itor=vec6Vec.begin(); itor!=vec6Vec.end(); itor++)
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
					itor = vec6Vec.erase(itor);
					if(itor!=vec6Vec.begin())
					{
						itor--;
					}
				}
			}
		}
	}
	vecVec.clear();
	vector<vector<int>>().swap(vecVec);
#endif
	return;
}

void checkResult()
{
	const char* sqlcmd = "create table if not exists tbldatas("
                            "elems text not null, result int not null default 0,"
                            "UNIQUE(elems, result));";
	for(const auto &elemVec:gbaseVec6Vec)
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
		sql->CloseDB();
		delete sql;
		sql = nullptr;
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
	findBaseSqlData3(vec6VecAll);
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
	sort(vec6VecAll.begin(), vec6VecAll.end(), cmp2());
	// sort(vec6VecAll.begin(), vec6VecAll.end());
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

	for(const auto &elemVec:gbaseVec6Vec)
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
 * 前提是a和b里面都没有重复元素
 * 两个vector相与
 * a & b;
*/
void vectorsFindSameElems(vector<vector<int>> &a, const vector<vector<int>> &b)
{
#if 1
	/**
	 * 对于任何集合a,b 都适用
	*/
	set<vector<int>> setA(a.begin(), a.end());
	set<vector<int>> setB(b.begin(), b.end());
	set<vector<int>> setRet = set<vector<int>>();
	set_intersection(setA.begin(), setA.end(), setB.begin(), setB.end(), \
					insert_iterator<set<vector<int>>>(setRet,setRet.begin()));
	vector<vector<int>> retVec(setRet.begin(), setRet.end());
	retVec.swap(a);

	setA.clear();set<vector<int>>().swap(setA);
	setB.clear();set<vector<int>>().swap(setB);
	setRet.clear();set<vector<int>>().swap(setRet);
	retVec.clear();vector<vector<int>>().swap(retVec);
#else
	set<vector<int>> uniqueElementsSetA(a.begin(), a.end());
	uniqueElementsSetA.insert(b.begin(), b.end());
	vector<vector<int>> mergeVec6Vec(uniqueElementsSetA.begin(), uniqueElementsSetA.end());
	mergeVec6Vec.swap(a);//交换两个容器的内容

	uniqueElementsSetA.clear();set<vector<int>>().swap(uniqueElementsSetA);
	mergeVec6Vec.clear();vector<vector<int>>().swap(mergeVec6Vec);
#endif
	return;
}

/**
 * 前提是a和b里面都没有重复元素
 * 两个vector相加
 * a + b;
*/
void vectorsPlus(vector<vector<int>> &a, const vector<vector<int>> &b)
{
#if 1
	/**
	 * 对于任何集合a,b 都适用
	*/
	set<vector<int>> setA(a.begin(), a.end());
	set<vector<int>> setB(b.begin(), b.end());
	set<vector<int>> setRet = set<vector<int>>();
	set_union(setA.begin(), setA.end(), setB.begin(), setB.end(), \
					insert_iterator<set<vector<int>>>(setRet,setRet.begin()));
	vector<vector<int>> retVec(setRet.begin(), setRet.end());
	retVec.swap(a);

	setA.clear();set<vector<int>>().swap(setA);
	setB.clear();set<vector<int>>().swap(setB);
	setRet.clear();set<vector<int>>().swap(setRet);
	retVec.clear();vector<vector<int>>().swap(retVec);
#else
	set<vector<int>> uniqueElementsSetA(a.begin(), a.end());
	uniqueElementsSetA.insert(b.begin(), b.end());
	vector<vector<int>> mergeVec6Vec(uniqueElementsSetA.begin(), uniqueElementsSetA.end());
	mergeVec6Vec.swap(a);//交换两个容器的内容

	uniqueElementsSetA.clear();set<vector<int>>().swap(uniqueElementsSetA);
	mergeVec6Vec.clear();vector<vector<int>>().swap(mergeVec6Vec);
#endif
	return;
}

/**
 * 前提是a和b里面都没有重复元素
 * 两个vector相减：
 * a - b;
*/
void vectorsMinus(vector<vector<int>> &a, const vector<vector<int>> &b)
{
#if 1
	/**
	 * 对于任何集合a,b 都适用
	*/
	set<vector<int>> setA(a.begin(), a.end());
	set<vector<int>> setB(b.begin(), b.end());
	set<vector<int>> setRet = set<vector<int>>();
	set_difference(setA.begin(), setA.end(), setB.begin(), setB.end(), \
					insert_iterator<set<vector<int>>>(setRet,setRet.begin()));
	vector<vector<int>> retVec(setRet.begin(), setRet.end());
	retVec.swap(a);

	setA.clear();set<vector<int>>().swap(setA);
	setB.clear();set<vector<int>>().swap(setB);
	setRet.clear();set<vector<int>>().swap(setRet);
	retVec.clear();vector<vector<int>>().swap(retVec);
#else
	/**
	 * 前提是b是a的子集
	 * 若不是子集，则for循环一个一个删除
	*/
	set<vector<int>> uniqueElementsSetA(a.begin(), a.end());
	set<vector<int>> uniqueElementsSetB(b.begin(), b.end());
	uniqueElementsSetA.erase(uniqueElementsSetB.begin(), uniqueElementsSetB.end());
	vector<vector<int>> mergeVec6Vec(uniqueElementsSetA.begin(), uniqueElementsSetA.end());
	mergeVec6Vec.swap(a);//交换两个容器的内容

	uniqueElementsSetA.clear();set<vector<int>>().swap(uniqueElementsSetA);
	uniqueElementsSetB.clear();set<vector<int>>().swap(uniqueElementsSetB);
	mergeVec6Vec.clear();vector<vector<int>>().swap(mergeVec6Vec);
#endif
	return;
}


unsigned long getSetVecAddValue(const set<vector<int>> &a, const set<vector<int>> &b)
{
	/**
	 * 此方法会快一些
	*/
	unsigned long add = 0;
	set<vector<int>> ret = set<vector<int>>();
	set_difference(b.begin(), b.end(), a.begin(), a.end(), insert_iterator<set<vector<int>>>(ret,ret.begin()));
	add = ret.size();
	/**
	 * 注意要释放资源，不然会内存泄漏
	*/
	ret.clear();
	set<vector<int>>().swap(ret);

	return add;
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
	set<vector<int>> ret = set<vector<int>>();
	set<vector<int>> c(b.begin(), b.end());
	set_difference(c.begin(), c.end(), a.begin(), a.end(), insert_iterator<set<vector<int>>>(ret,ret.begin()));
	add = ret.size();
	/**
	 * 注意要释放资源，不然会内存泄漏
	*/
	c.clear();
	set<vector<int>>().swap(c);
	ret.clear();
	set<vector<int>>().swap(ret);
#endif
	return add;
}


unsigned long getSetAddValue(const set<set<int>> &a, const set<set<int>> &b)
{
#if 0
	unsigned long add = a.size();
	set<set<int>> uniqueElementsSet(a);
	uniqueElementsSet.insert(b.begin(), b.end());
	add = uniqueElementsSet.size() - add;

	/**
	 * 注意要释放资源，不然会内存泄漏
	*/
	uniqueElementsSet.clear();
	set<set<int>>().swap(uniqueElementsSet);
#else
	/**
	 * 此方法会快一些
	*/
	unsigned long add = 0;
	set<set<int>> ret = set<set<int>>();
	// set<set<int>> c(b);
	set_difference(b.begin(), b.end(), a.begin(), a.end(), insert_iterator<set<set<int>>>(ret,ret.begin()));
	add = ret.size();
	/**
	 * 注意要释放资源，不然会内存泄漏
	*/
	// c.clear();
	// set<set<int>>().swap(c);
	ret.clear();
	set<set<int>>().swap(ret);
#endif
	return add;
}

void getFinalVec6VecFromLargeMapSetVecDatas(vector<vector<int>> &finalVec6Vec,
					 set<vector<int>> &uniqueElementsSet,
					 set<vector<int>> &leftVec6Vec,
					 const map<vector<int>, set<vector<int>>> &LargeDatas)
{
	vector<int> insertVec6 = vector<int>();

	unsigned long addValue = 0;
	if((uniqueElementsSet.size() <= 0) || (finalVec6Vec.size() <= 0))
	{
		printf("uniqueElementsSet.size() <= 0\r\n");
		return;
	}

	for(const auto &elemVec:leftVec6Vec)
	{
		unsigned long value = 0;

		value = getSetVecAddValue(uniqueElementsSet, LargeDatas.at(elemVec));
		if(addValue < value)
		{
			insertVec6.clear();
			vector<int>().swap(insertVec6);
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
			insertVec6.clear();
			vector<int>().swap(insertVec6);
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


void getFinalSetSet(set<set<int>> &finalSetSet,
					 set<set<int>> &uniqueElementsSet,
					 set<set<int>> &leftSetSet,
					 const map<set<int>, set<set<int>>> &LargeDatas)
{
	set<int> insertSet6 = set<int>();

	unsigned long addValue = 0;
	if((uniqueElementsSet.size() <= 0) || (finalSetSet.size() <= 0))
	{
		printf("uniqueElementsSet.size() <= 0\r\n");
		return;
	}

	for(const auto &elemSet:leftSetSet)
	{
		unsigned long value = 0;

		value = getSetAddValue(uniqueElementsSet, LargeDatas.at(elemSet));
		if(addValue < value)
		{
			insertSet6.clear();
			set<int>().swap(insertSet6);
			insertSet6 = elemSet;
			addValue = value;
		}
	}

	uniqueElementsSet.insert(LargeDatas.at(insertSet6).begin(), LargeDatas.at(insertSet6).end());
	finalSetSet.insert(insertSet6);
	leftSetSet.erase(insertSet6);

	insertSet6.clear();
	set<int>().swap(insertSet6);
	return;
}


void getLargeSetVecDatas(map<vector<int>, set<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec)
{
	/**
	 * 去脏数据
	*/
	if(LargeDatas.size() > 0)
	{
		/**
		 * 清空脏数据
		*/
		LargeDatas.clear();
		map<vector<int>, set<vector<int>>>().swap(LargeDatas);
	}

	arrange::startTime();
	printf("-----------startTime-------------%s\r\n", __FUNCTION__);
	for(const auto &elemVec:baseVec6Vec)
	{
		set<vector<int>> set6Vec = set<vector<int>>();
		getSet6VecFromDb(elemVec, set6Vec);
		// LargeDatas.insert(make_pair(elemVec, set6Vec));
		LargeDatas.emplace(elemVec, set6Vec);
		set6Vec.clear();set<vector<int>>().swap(set6Vec);
	}
	printf("-----------endTime-------------%ld\r\n", LargeDatas.size());
	arrange::endTime();
	return;
}

void getLargeVecVecDatas(map<vector<int>, vector<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec)
{
	/**
	 * 去脏数据
	*/
	if(LargeDatas.size() > 0)
	{
		/**
		 * 清空脏数据
		*/
		LargeDatas.clear();
		map<vector<int>, vector<vector<int>>>().swap(LargeDatas);
	}

	arrange::startTime();
	printf("-----------startTime-------------%s\r\n", __FUNCTION__);
	for(const auto &elemVec:baseVec6Vec)
	{
		vector<vector<int>> vec6Vec = vector<vector<int>>();
		getVec6VecFromDb(elemVec, vec6Vec);
		// LargeDatas.insert(make_pair(elemVec, vec6Vec));
		LargeDatas.emplace(elemVec, vec6Vec);
		vec6Vec.clear();vector<vector<int>>().swap(vec6Vec);
	}
	printf("-----------endTime-------------%ld\r\n", LargeDatas.size());
	arrange::endTime();
	return;
}

void getLargeSetSetDatas(map<set<int>, set<set<int>>> &LargeDatas, const set<set<int>> &baseSet6Set)
{
	/**
	 * 去脏数据
	*/
	if(LargeDatas.size() > 0)
	{
		/**
		 * 清空脏数据
		*/
		LargeDatas.clear();
		map<set<int>, set<set<int>>>().swap(LargeDatas);
	}

	arrange::startTime();
	printf("-----------startTime-------------%s\r\n", __FUNCTION__);
	for(const auto &elemSet:baseSet6Set)
	{
		set<set<int>> set6Set = set<set<int>>();
		getSet6SetFromDb(elemSet, set6Set);
		// LargeDatas.insert(make_pair(elemSet, set6Set));
		LargeDatas.emplace(elemSet, set6Set);
		set6Set.clear();set<set<int>>().swap(set6Set);
	}
	printf("-----------endTime-------------%ld\r\n", LargeDatas.size());
	arrange::endTime();
	return;
}

/**
 * 将vector<vector<int>> 全部转化为 set<set<>>的方式
 * baseVec6Vec 传值为gVecBingoVec
*/
void getCoverResultDatas4(const map<set<int>, set<set<int>>> &LargeDatas,
							const set<set<int>> &baseSetSet)
{
	set<set<int>> finalSetSet = set<set<int>>();
	set<set<int>> uniqueElementsSet = set<set<int>>();
	set<set<int>> leftSetSet(baseSetSet);
	unsigned long baseSetSetSize = baseSetSet.size();

	finalSetSet.insert(*(baseSetSet.begin()));
	leftSetSet.erase(*(baseSetSet.begin()));
	uniqueElementsSet.insert(LargeDatas.at(*(baseSetSet.begin())).begin(),
							 LargeDatas.at(*(baseSetSet.begin())).end());

	arrange::startTime();
	printf("-----------startTime-------------\r\n");
	while(finalSetSet.size() < baseSetSetSize)
	{
		printf("------------------------%ld\r\n", uniqueElementsSet.size());
		getFinalSetSet(finalSetSet, uniqueElementsSet, leftSetSet, LargeDatas);
	}
	arrange::endTime();

	set<set<int>>().swap(leftSetSet);
	leftSetSet.clear();
	set<set<int>>().swap(uniqueElementsSet);
	uniqueElementsSet.clear();

	// getCoverResultDatas3(LargeDatas, finalSetSet);
	return;
}

/**
 * set<vector<>>的混合使用的方式,添加中断，避免一旦停止运行 finalVec6Vec 就得重新开始生成~
 * baseVec6Vec 传值为gVecBingoVec
*/
void getCoverResultDatas9(const map<vector<int>, set<vector<int>>> &LargeDatas, \
							const vector<vector<int>> &baseVec6Vec, \
							vector<vector<int>> &finalVec6Vec)
{
	set<vector<int>> uniqueElementsSet = set<vector<int>>();
	set<vector<int>> leftVec6Vec = set<vector<int>>();
	unsigned long baseVec6VecSize = baseVec6Vec.size();

	leftVec6Vec.insert(baseVec6Vec.begin(), baseVec6Vec.end());//初始化leftVec6Vec set容器

	if(finalVec6Vec.size() <= 0)
	{
		finalVec6Vec.push_back(baseVec6Vec[0]);
		leftVec6Vec.erase(baseVec6Vec[0]);
		uniqueElementsSet.insert(LargeDatas.at(baseVec6Vec[0]).begin(), LargeDatas.at(baseVec6Vec[0]).end());
	}
	else
	{
		for(const auto &elemVec6:finalVec6Vec)
		{
			leftVec6Vec.erase(elemVec6);
			uniqueElementsSet.insert(LargeDatas.at(elemVec6).begin(), LargeDatas.at(elemVec6).end());
		}
	}


	arrange::startTime();
	printf("-----------startTime-------------\r\n");
	while(finalVec6Vec.size() < baseVec6VecSize)
	{
		printf("------------%ld------------%ld\r\n", finalVec6Vec.size(), uniqueElementsSet.size());
		getFinalVec6VecFromLargeMapSetVecDatas(finalVec6Vec, uniqueElementsSet, leftVec6Vec, LargeDatas);
	}
	arrange::endTime();

	set<vector<int>>().swap(leftVec6Vec);
	leftVec6Vec.clear();

	set<vector<int>>().swap(uniqueElementsSet);
	uniqueElementsSet.clear();

	getCoverResultDatas8(LargeDatas, finalVec6Vec);
	return;
}

/**
 * set<vector<>>的混合使用的方式
 * baseVec6Vec 传值为gVecBingoVec
*/
void getCoverResultDatas7(const map<vector<int>, set<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec)
{
	vector<vector<int>> finalVec6Vec = vector<vector<int>>();
	set<vector<int>> uniqueElementsSet = set<vector<int>>();
	set<vector<int>> leftVec6Vec = set<vector<int>>();
	unsigned long baseVec6VecSize = baseVec6Vec.size();

	leftVec6Vec.insert(baseVec6Vec.begin(), baseVec6Vec.end());//初始化leftVec6Vec set容器

	finalVec6Vec.push_back(baseVec6Vec[0]);
	leftVec6Vec.erase(baseVec6Vec[0]);
	uniqueElementsSet.insert(LargeDatas.at(baseVec6Vec[0]).begin(), LargeDatas.at(baseVec6Vec[0]).end());

	arrange::startTime();
	printf("-----------startTime-------------\r\n");
	while(finalVec6Vec.size() < baseVec6VecSize)
	{
		printf("------------%ld------------%ld\r\n", finalVec6Vec.size(), uniqueElementsSet.size());
		getFinalVec6VecFromLargeMapSetVecDatas(finalVec6Vec, uniqueElementsSet, leftVec6Vec, LargeDatas);
	}
	arrange::endTime();

	set<vector<int>>().swap(leftVec6Vec);
	leftVec6Vec.clear();

	set<vector<int>>().swap(uniqueElementsSet);
	uniqueElementsSet.clear();

	getCoverResultDatas8(LargeDatas, finalVec6Vec);
	return;
}

/**
 * set<vector<>>的混合使用的方式，效率值还是很慢的
 * baseVec6Vec 传值为gVecBingoVec
*/
void getCoverResultDatas5(const map<vector<int>, vector<vector<int>>> &LargeDatas,
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

	arrange::startTime();
	printf("-----------startTime-------------\r\n");
	while(finalVec6Vec.size() < baseVec6VecSize)
	{
		printf("------------------------%ld\r\n", uniqueElementsSet.size());
		getFinalVec6Vec(finalVec6Vec, uniqueElementsSet, leftVec6Vec, LargeDatas);
	}
	arrange::endTime();

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

	// creatVec6VecResult(finalVec , OBJECT_EXE_DIR "FinalBaseVec6Vec");//可执行文件 getVecAddValue->
	// creatVec6VecResult(finalVec , OBJECT_EXE_DIR "FinalBaseVec6Vec_0201");//可执行文件 FinalBaseVec6Vec_0201->
	// creatVec6VecResult(finalVec , OBJECT_EXE_DIR "getCoverResultDatas7");//可执行文件 getCoverResultDatas7->
	creatVec6VecResult(finalVec , OBJECT_EXE_DIR "getCoverResultDatas9");//可执行文件 catchSigIntSaveFinalVec6VecToDbFile->
	return;
}

/**
 * 用set加insert的特性可去重复来实现：效率最高
 * 得到 BASESQLDBDATA 所对应的所有数据块的覆盖率：
*/
void getCoverResultDatas8(const map<vector<int>, set<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec)
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
	printf("total uniqueElementsSet.size=%ld\r\n", uniqueElementsSet.size());

	set<vector<int>>().swap(uniqueElementsSet);
	uniqueElementsSet.clear();

	genFinalBaseVec6Vec(vec6intPairVec);

	vector<pair<vector<int>, int>>().swap(vec6intPairVec);
	vec6intPairVec.clear();
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
	printf("total uniqueElementsSet.size=%ld\r\n", uniqueElementsSet.size());

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
	printf("gbaseVec6Vec.size = %ld\r\n", gbaseVec6Vec.size());
	for(const auto &gElemVec : gbaseVec6Vec)
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
void findTheSameAmountWithComparedSet6Set()
{
	set<int> set6A = {4,6,7,10,13,25};
	set<int> set6B = {4,9,12,17,30,32};
	unsigned int ret = 0;
	set<set<int>> retSetSet = set<set<int>>();
	set<set<int>> set6SetA = set<set<int>>();
	set<set<int>> set6SetB = set<set<int>>();
	getSet6SetFromDb(set6A, set6SetA);
	getSet6SetFromDb(set6B, set6SetB);
	set_intersection(set6SetA.begin(), set6SetA.end(),
				 	 set6SetB.begin(), set6SetB.end(),
					 insert_iterator<set<set<int>>>(retSetSet, retSetSet.begin()));
	ret = retSetSet.size();

	/**
	 * 回收内存空间，释放资源
	*/
	set6A.clear();set<int>().swap(set6A);
	set6B.clear();set<int>().swap(set6B);
	set6SetA.clear();set<set<int>>().swap(set6SetA);
	set6SetB.clear();set<set<int>>().swap(set6SetB);
	retSetSet.clear();set<set<int>>().swap(retSetSet);

	printf("the same amount is %d\r\n", ret);
}

/**
 * 最笨的方法，vector查找交集，重复是效率最低的，应该用set容器
*/
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

void getSet6SetFromDb(const set<int> &set6, set<set<int>> &set6Set)
{
	sqlite_tb *sql = nullptr;
	string sqlDbFile = SAVE6RET_DB_FILEDIR;
	string dataStr = "";

	for(const auto &elem : set6)
	{
		dataStr += arrange::NumberToString<int>(elem) + "_";
	}
	// printf("[\t %s \t]\r\n", dataStr.c_str());
	sqlDbFile += dataStr;
	sql = new sqlite_tb(sqlDbFile.c_str());
	sql->OpenDB();
	sql->SelectAllData(set6Set);
	sql->CloseDB();
	delete sql;
	sql = nullptr;
	return;
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

void getSet6VecFromDb(const vector<int> &vec6, set<vector<int>> &set6Vec)
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
	sql->SelectAllDataStoreSetVec(set6Vec);
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
 * vecVec  和  setSet 是一一对应的，只是存储方式不一样而已，
 * vec是数组类型的容器，
 * set是关联性容器，关联链表，红黑树方式存储，查找，删除，定位都是用的二叉树查找，所以效率很高
 * setSet必须是一个空容器；
*/
void convertVecVec2SetSet(const vector<vector<int>> &vecVec, set<set<int>> &setSet)
{
	if(setSet.size() > 0)
	{//若不为空，则清空之前的数据，避免内存泄漏，或者得到额setSet里面会有脏数据~
		setSet.clear();
		set<set<int>>().swap(setSet);
	}

	for(const auto &elemVec:vecVec)
	{
		set<int> tmp(elemVec.begin(), elemVec.end());
		setSet.insert(tmp);
		tmp.clear();
		set<int>().swap(tmp);
	}
	return;
}


void convertSetSet2VecVec(const set<set<int>> &setSet, vector<vector<int>> &vecVec)
{
	if(vecVec.size() > 0)
	{//若不为空，则清空之前的数据，避免内存泄漏，或者得到额vecVec里面会有脏数据~
		vecVec.clear();
		vector<vector<int>>().swap(vecVec);
	}

	for(const auto &elemSet:setSet)
	{
		vector<int> tmp(elemSet.begin(), elemSet.end());
		vecVec.push_back(tmp);
		tmp.clear();
		vector<int>().swap(tmp);
	}
	return;
}


/**
 * ->生成 gbaseVec6Vec
 * ->生成 gFinalVec6Vec
*/
void createVec6vecFromSqlDb(vector<vector<int>> &vec6Vec, const char* sqldbfile)
{
	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(sqldbfile);
	if(nullptr == sql)
	{
		printf("sql db is nullptr.\r\n");
		return;
	}

    // 打开数据库，不存在，创建数据库db
	sql->OpenDB();

	if(vec6Vec.size() > 0)
    {//清空容器，避免有脏数据
        vec6Vec.clear();
        vector<vector<int>>().swap(vec6Vec);
    }
	sql->SelectAllData(vec6Vec);

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	/**
	 * 去重复操作~
	*/
	checkVec6VecUnique(vec6Vec);
	/**
	 * 重新按cmp2规则排序~
	*/
	// sort(vec6Vec.begin(), vec6Vec.end(), cmp2());
	printf("vec6Vec.size=%ld\r\n", vec6Vec.size());
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

/**
 * 转化成set容器取查找和删除和插入动作都是效率最高的
*/
void findBaseSqlData3(const vector<vector<int>> &vecBingoVec)
{
	vector<vector<int>> vecBaseRetVec = vector<vector<int>>();
	set<set<int>> setBaseSet = set<set<int>>();
	set<set<int>> setBaseSetConst = set<set<int>>();
	for(const auto &elemVec : vecBingoVec)
	{
		set<int> tmp(elemVec.begin(), elemVec.end());
		setBaseSet.insert(tmp);
		tmp.clear();
		set<int>().swap(tmp);
	}
	printf("%s--%d--Start~--setBaseSet.size=%ld\r\n",__func__, __LINE__, setBaseSet.size());

	setBaseSetConst.insert(setBaseSet.begin(), setBaseSet.end());

	for(const auto &elemSetConst : setBaseSetConst)
	{
		if(setBaseSet.find(elemSetConst) == setBaseSet.end())
		{//未找到，不存在
			continue;
		}

		set<int> b(elemSetConst);
		set<set<int>> setDeleteSet = set<set<int>>();

		for(const auto &elemSet : setBaseSet)
		{
			if(elemSet != elemSetConst)
			{
				set<int> a(elemSet);
				set<int> tmp = set<int>();

				/**
				 * 取两个容器的相同的元素的，装入到tmp容器里面；
				*/
				set_intersection(a.begin(), a.end(), b.begin(), b.end(),
									insert_iterator<set<int>>(tmp, tmp.begin()));
				// printf("tmp.size=%ld\r\n", tmp.size());
				if(tmp.size() >= 3)//被删除的判断条件~相同的数字数量的判断条件
				{
					setDeleteSet.insert(elemSet);
				}

				tmp.clear();
				set<int>().swap(tmp);
				a.clear();
				set<int>().swap(a);
			}
		}

		for(auto &delElemSet : setDeleteSet)
		{
			setBaseSet.erase(delElemSet);
		}
		b.clear();
		set<int>().swap(b);
		setDeleteSet.clear();
		set<set<int>>().swap(setDeleteSet);
		printf("setBaseSet.size=%ld\r\n", setBaseSet.size());
	}

	for(const auto &elemSet : setBaseSet)
	{
		vector<int> tmp(elemSet.begin(), elemSet.end());
		vecBaseRetVec.push_back(tmp);
		tmp.clear();
		vector<int>().swap(tmp);
	}

	setBaseSet.clear();
	set<set<int>>().swap(setBaseSet);
	setBaseSetConst.clear();
	set<set<int>>().swap(setBaseSetConst);

	printf("vecBaseRetVec.size=%ld\r\n", vecBaseRetVec.size());
	/**
	 * 这里生成的就是 BASESQLDBDATA  所用到的文件
	*/
	creatVec6VecResult(vecBaseRetVec, BASESQLDBDATA "_0620");
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


