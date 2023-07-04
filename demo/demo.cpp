/**
 * 训练/测试接口的模块
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
#include "demo.hpp"

using namespace std;

/**
 * 可优化点：	findBaseSqlData  接口的效率的优化
 *
*/


#define 	USE_TIMER 	0

__attribute__((unused)) static void checkBingoResult();
__attribute__((unused)) static void checkBingoVec6Datas();
template<typename T>
__attribute__((unused)) static void createSetXVecFromSqlDb(const char* sqldbfile, T &setXVec, unsigned int vecElemsNum = 6);
__attribute__((unused)) static void findTheInterSectionFromThe2Vec10Vec();
__attribute__((unused)) static void func1(sqlite_tb *psql, vector<int> &vred, const vector<int> &vblue);
__attribute__((unused)) static int64_t funcCnFrom(const int32_t from, const int8_t n);
__attribute__((unused)) static void findRule();
__attribute__((unused)) static void mathematics();
__attribute__((unused)) static void createBaseSqlDb_Select6From33();
__attribute__((unused)) static void createBaseSqlDb_Select10From33(set<vector<int>> &resVec10Vec);
__attribute__((unused)) static void findBaseSqlDataFromSet10Vec(set<vector<int>> &resSet10Vec);
__attribute__((unused)) static void createBingoSql();
__attribute__((unused)) static void createFinalResultSqlDb();
__attribute__((unused)) static void checkResult();
__attribute__((unused)) static void checkTask2Result();
__attribute__((unused)) static void getCoverResultDatas();
__attribute__((unused)) static void getTheBingoVec6FromPerVec6Vec(const vector<int> &baseVec6);
__attribute__((unused)) static void getCoverResultDatas3(const map<vector<int>, vector<vector<int>>> &LargeDatas, \
								 const vector<vector<int>> &baseVec6Vec);
__attribute__((unused)) static void getCoverResultDatas4(const map<set<int>, set<set<int>>> &LargeDatas,
								 const set<set<int>> &baseSetSet);
__attribute__((unused)) static void getCoverResultDatas5(const map<vector<int>, vector<vector<int>>> &LargeDatas,
								 const vector<vector<int>> &baseVec6Vec);
__attribute__((unused)) static void getCoverResultDatas7(const map<vector<int>, set<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec);
__attribute__((unused)) static void getCoverResultDatas8(const map<vector<int>, set<vector<int>>> &LargeDatas, const vector<vector<int>> &baseVec6Vec);
__attribute__((unused)) static void getCoverResultDatas9(const map<vector<int>, set<vector<int>>> &LargeDatas, \
								 const vector<vector<int>> &baseVec6Vec, \
								 vector<vector<int>> &finalVec6Vec);
template<typename T>
__attribute__((unused)) static void creatVecVecResult(const T &vecVec, const char* dbname, const unsigned int bingoNums = 6);
__attribute__((unused)) static void findBaseSqlData(const vector<vector<int>> &vecBingoVec);
__attribute__((unused)) static void findBaseSqlData3(const vector<vector<int>> &vecBingoVec);
__attribute__((unused)) static void findBaseSqlData4(set<vector<int>> &setBaseVec);
__attribute__((unused)) static void findBaseSqlData5(set<vector<int>> &setBaseVec);
__attribute__((unused)) static void getVec6VecFromDb(const vector<int> &vec6, vector<vector<int>> &vec6Vec);
__attribute__((unused)) static void getSet6SetFromDb(const set<int> &set6, set<set<int>> &set6Set);
__attribute__((unused)) static void getSet6VecFromDb(const vector<int> &vec6, set<vector<int>> &set6Vec);
__attribute__((unused)) static unsigned int getTheSameAmountFromVec6Vecs(const vector<vector<int>> &vec6VecA, const vector<vector<int>> &vec6VecB);
__attribute__((unused)) static void findTheSameAmountWithComparedSet6Set();
__attribute__((unused)) static void genVec6VecForFindBaseSqlData();
__attribute__((unused)) static void startTask();
__attribute__((unused)) static void startTask2();
__attribute__((unused)) static void initObjectDir();
__attribute__((unused)) static void testGetCoverResultDatas2();
__attribute__((unused)) static void createTask2FinalResultSqlDb();
__attribute__((unused)) static void convertVecVec2SetSet(const vector<vector<int>> &vecVec, set<set<int>> &setSet);
__attribute__((unused)) static void convertVecVec2SetVec(const vector<vector<int>> &vecVec, set<vector<int>> &setVec);
__attribute__((unused)) static void convertSetSet2VecVec(const set<set<int>> &setSet, vector<vector<int>> &vecVec);
__attribute__((unused)) static void createVec6vecFromSqlDb(const char* sqldbfile, vector<vector<int>> &vec6Vec);
__attribute__((unused)) static void getLargeSetSetDatas(const set<set<int>> &baseVec6Vec, map<set<int>, set<set<int>>> &LargeDatas);
__attribute__((unused)) static void getLargeVecVecDatas(const vector<vector<int>> &baseVec6Vec, map<vector<int>, vector<vector<int>>> &LargeDatas);
__attribute__((unused)) static void getLargeSetVecDatas(const vector<vector<int>> &baseVec6Vec, map<vector<int>, set<vector<int>>> &LargeDatas);
__attribute__((unused)) static void getVec6Vec1107568(vector<vector<int>> &vec6VecAll);
__attribute__((unused)) static void checkVec6VecUnique(vector<vector<int>> &vec6Vec);
__attribute__((unused)) static void vectorsFindSameElems(vector<vector<int>> &a, const vector<vector<int>> &b);

static vector<vector<int>> gbaseVec6Vec = vector<vector<int>>();//要去重复
static vector<vector<int>> gFinalVec6Vec = vector<vector<int>>();
static set<vector<int>> gResSet10Vec = set<vector<int>>();

struct upperS
{
	bool operator()(const pair<vector<int>, int> &a,
					const pair<vector<int>, int> &b)
	{
		if(a.second != b.second)
		{
			return a.second > b.second;//  ">"---从大到小重新排序;   "<"---从小到大重新排序，
		}
		else
		{
			return a.first < b.first;
		}
	}
};

struct lowerS
{
	bool operator()(const pair<vector<int>, int> &a,
					const pair<vector<int>, int> &b)
	{
		if(a.second != b.second)
		{
			return a.second < b.second;//  ">"---从大到小重新排序;   "<"---从小到大重新排序，
		}
		else
		{
			return a.first < b.first;
		}
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
	creatVecVecResult<vector<vector<int>>>(gFinalVec6Vec , FINALVEC6VEC_DB);
	printf("handle end-----------------catch--%d\n", signo);
	arrange::startTime();
	printf("-----------startTime-------------\r\n");
}

void catchSigIntSaveFinalSet10VecToDbFile(int signo)
{
	arrange::endTime();
	printf("handle start-----------------catch--%d--%ld\n", signo, gResSet10Vec.size());
	system("rm " FINALSET10VEC_DB " -rf;");
	system("sync;");
	creatVecVecResult<set<vector<int>>>(gResSet10Vec , FINALSET10VEC_DB, VEC_ELEMS_NUM);
	printf("handle end-----------------catch--%d\n", signo);
	arrange::startTime();
	printf("-----------startTime-------------\r\n");
}


/**
 * 推荐使用~
 * 信号队列中，屏蔽连续执行的相同信号只执行一次，在信号执行函数执行期间，该信号已从队列中吐出~
*/
void regSignalIntHandle2()
{
	int ret = 0;
	struct sigaction act;

    act.sa_handler = catchSigIntSaveFinalSet10VecToDbFile;
    sigemptyset(&act.sa_mask);
    // sigaddset(&act.sa_mask, SIGQUIT); //信号捕捉函数执行期间sa_mask屏蔽字中，3号信号置1  3号信号屏蔽     置1就是屏蔽，置0就是不屏蔽
	sigaddset(&act.sa_mask, SIGINT);  //信号捕捉函数执行期间 sa_mask屏蔽字中，2号信号置1  2号信号屏蔽     置1就是屏蔽，置0就是不屏蔽
    act.sa_flags = 0;  //默认属性   信号捕捉函数执行期间，自动屏蔽本信号

    ret = sigaction(SIGINT, &act, NULL);   //注册2号信号
    if (ret == -1) {
        perror("sigaction error");
        exit(1);
    }
	return;
}

void regSignalIntHandle1()
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
#if USE_TIMER
static unsigned long cnt = 0;
#endif
//~ 定时器中断处理程序入口
static void sigalrm_handler(int sig)
{
#if USE_TIMER
    cnt++;
    // printf("timer trigered .. %d \r\n", cnt);
	if(0 == cnt%10)
	{
		arrange::mUsed = true;
	}
#endif
}

/**
 * 定时器的使用；
*/
void regSignalAlrmHandle()
{
    struct itimerval itv, oldtv;
    //~ 定时间隔1秒0微秒
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    //~ 初始计时
    itv.it_value = itv.it_interval;

	//~ 设定信号SIGALRM的处理函数
    signal(SIGALRM, sigalrm_handler);

    //~ 打开定时器ITIMER_REAL
    setitimer(ITIMER_REAL, &itv, &oldtv);
	return;
}


template<typename T>
void creatVecVecResult(const T &vecVec, const char* dbname, const unsigned int bingoNums)
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



int sample_demo_main(int argc, char** argv)
{
	set<set<int>> gBaseSetSet = set<set<int>>();
	map<set<int>, set<set<int>>> LargeDatas = map<set<int>, set<set<int>>>();
	map<vector<int>, set<vector<int>>> LargeMapSetVecDatas = map<vector<int>, set<vector<int>>>();
	map<vector<int>, vector<vector<int>>> LargeMapVecVecDatas = map<vector<int>, vector<vector<int>>>();
	initObjectDir();
	createBingoSql();
	// arrange::startTime();
	// printf("-----------startTime--[createBaseSqlDb_Select6From33]-----------\r\n");
	// createBaseSqlDb_Select6From33();
	// arrange::endTime();
	arrange::startTime();
	printf("-----------startTime--[checkBingoResult]-----------\r\n");
	checkBingoResult();
	arrange::endTime();
#if 0
	regSignalIntHandle2();
	createBingoSql();
	createVec6vecFromSqlDb(BASESQLDBDATA, gbaseVec6Vec);//生成  gbaseVec6Vec
	createVec6vecFromSqlDb(FINALVEC6VEC_DB, gFinalVec6Vec);//生成  gFinalVec6Vec
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
	checkResult();
	// testGetCoverResultDatas2();//
	// getLargeSetSetDatas(gBaseSetSet, LargeDatas);getCoverResultDatas4(LargeDatas, gBaseSetSet);
	// getLargeVecVecDatas(gbaseVec6Vec, LargeMapVecVecDatas);getCoverResultDatas5(LargeMapVecVecDatas, gbaseVec6Vec);

	// getLargeSetVecDatas(gbaseVec6Vec, LargeMapSetVecDatas);getCoverResultDatas7(LargeMapSetVecDatas, gbaseVec6Vec);

	// getLargeSetVecDatas(gbaseVec6Vec, LargeMapSetVecDatas);getCoverResultDatas9(LargeMapSetVecDatas, gbaseVec6Vec, gFinalVec6Vec);//效率最高

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
	// regSignalIntHandle2();findBaseSqlDataFromSet10Vec(gResSet10Vec);
	// findTheInterSectionFromThe2Vec10Vec();

	// createBingoSql();
	// createVec6vecFromSqlDb(BASESQLDBDATA, gbaseVec6Vec);//生成  gbaseVec6Vec

	// createVec6vecFromSqlDb(FINALVEC6VEC_DB, gFinalVec6Vec);//生成  gFinalVec6Vec
	#if USE_TIMER
	regSignalAlrmHandle();
	#endif
	// getTheBingoVec6FromPerVec6Vec({3,6,11,21,31,32});
	// checkTask2Result();//startTask2();//
	// startTask();//checkResult();//
	// getLargeVecVecDatas(gbaseVec6Vec, LargeMapVecVecDatas);getCoverResultDatas3(LargeMapVecVecDatas, gbaseVec6Vec);//验证得到的所有数据块的覆盖率
	// getLargeSetVecDatas(gbaseVec6Vec, LargeMapSetVecDatas);getCoverResultDatas8(LargeMapSetVecDatas, gbaseVec6Vec);
#endif

	freeResource<set<set<int>>>(gBaseSetSet);
	freeResource<vector<vector<int>>>(gbaseVec6Vec);
	freeResource<vector<vector<int>>>(gFinalVec6Vec);
	freeResource<map<set<int>, set<set<int>>>>(LargeDatas);
	freeResource<map<vector<int>, set<vector<int>>>>(LargeMapSetVecDatas);
	freeResource<map<vector<int>, vector<vector<int>>>>(LargeMapVecVecDatas);

	return 0;
}

void checkSet8VecTotal()
{
	unsigned int operateIndex = 0;
	set<set<int>> set8Set = set<set<int>>();
	for(const auto &elem8Vec : MyTask::mSet8VecTotal)
	{
		set<int> tmp(elem8Vec.begin(), elem8Vec.end());
		set8Set.insert(tmp);
		freeResource<set<int>>(tmp);
	}
	set<set<int>> set8SetConst(set8Set.begin(), set8Set.end());

	printf("set8Set.size=%ld, set8SetConst.size=%ld\r\n", set8Set.size(), set8SetConst.size());
	arrange::startTime();
	for(const auto &elem8Set1 : set8SetConst)
	{
		operateIndex++;
		printf("operateIndex=%d\r\n", operateIndex);
		if(set8Set.find(elem8Set1) == set8Set.end())
		{//未找到，不存在
			continue;
		}
		set<set<int>> setDeleteSet = set<set<int>>();
		for(const auto &elem8Set2 : set8Set)
		{
			if(elem8Set1 != elem8Set2)
			{
				set<int> tmp = set<int>();

				/**
				 * 取两个容器的相同的元素的，装入到tmp容器里面；
				*/
				set_intersection(elem8Set2.begin(), elem8Set2.end(), elem8Set1.begin(), elem8Set1.end(),
									insert_iterator<set<int>>(tmp, tmp.begin()));
				// printf("tmp.size=%ld\r\n", tmp.size());
				if(tmp.size() > 6)//被删除的判断条件~相同的数字数量的判断条件
				{
					setDeleteSet.insert(elem8Set2);
				}
				freeResource<set<int>>(tmp);
			}
		}
		for(const auto &delElemSet : setDeleteSet)
		{
			set8Set.erase(delElemSet);
		}
		freeResource<set<set<int>>>(setDeleteSet);
	}
	arrange::endTime();
	printf("set8Set.size=%ld\r\n", set8Set.size());

	freeResource<set<set<int>>>(set8Set);
	freeResource<set<set<int>>>(set8SetConst);
	freeResource<set<vector<int>>>(MyTask::mSet8VecTotal);
	return;
}


/**
 * ->生成 gbaseVec6Vec
 * ->生成 gFinalVec6Vec
*/
void createVec6vecFromSqlDb(const char* sqldbfile, vector<vector<int>> &vec6Vec)
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
		freeResource<vector<vector<int>>>(vec6Vec);
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
	// printf("vec6Vec.size=%ld\r\n", vec6Vec.size());
	return;
}

template<typename T>
void createSetXVecFromSqlDb(const char* sqldbfile, T &setXVec, unsigned int vecElemsNum)
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

	if(setXVec.size() > 0)
    {//清空容器，避免有脏数据
		freeResource<T>(setXVec);
    }
	sql->SelectAllData(setXVec, vecElemsNum);

	sql->CloseDB();
	delete sql;
	sql = nullptr;
	printf("setXVec.size=%ld\r\n", setXVec.size());
	return;
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

	freeResource<set<vector<int>>>(aaa);
	freeResource<set<vector<int>>>(bbb);
	freeResource<set<vector<int>>>(ddd);
	freeResource<vector<vector<int>>>(ccc);
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

	freeResource<vector<vector<int>>>(tmpVecA);
	freeResource<vector<vector<int>>>(tmpVecB);
	freeResource<vector<vector<int>>>(tmpVecC);
	freeResource<vector<vector<int>>>(tmpVecD);
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

	freeResource<set<set<int>>>(tmpSetA);
	freeResource<set<set<int>>>(tmpSetB);
	freeResource<set<set<int>>>(tmpSetC);
	freeResource<set<set<int>>>(tmpSetR);
	freeResource<vector<vector<int>>>(tmpVecD);
	freeResource<vector<vector<int>>>(tmpVecE);
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
	freeResource<set<vector<int>>>(uniqueElements);
	freeResource<vector<vector<int>>>(hbvectors);
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
    set_difference(a.begin(), a.end(), b.begin(), b.end(), insert_iterator<set<vector<int>>>(d,d.begin()));
	printf("d.size=%ld\r\n", d.size());
    for (const auto &elementvec : d)
	{
        for (const auto &element : elementvec)
		{
			printf("%d\t", element);
		}printf("\r\n");
    }
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), insert_iterator<set<vector<int>>>(e, e.begin()));
	printf("e.size=%ld\r\n", e.size());
    for (const auto &elementvec : e)
	{
        for (const auto &element : elementvec)
		{
			printf("%d\t", element);
		}printf("\r\n");
    }
	freeResource<set<vector<int>>>(c);//清空并释放内存
	freeResource<set<vector<int>>>(d);
	freeResource<set<vector<int>>>(e);
#endif

	freeResource<set<vector<int>>>(a);
	freeResource<set<vector<int>>>(b);
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

	if(!exists_test2(SAVE6RET_OCCURETIME_DB_FILEDIR))
	{
		system("mkdir " SAVE6RET_OCCURETIME_DB_FILEDIR " -p");
	}

	if(!exists_test2(BASESQLDBDATAFILEDIR))
	{
		system("mkdir " BASESQLDBDATAFILEDIR " -p");
	}

	return;
}

void startTask()
{
	int timeCount = 0;
	ThreadPool<MyTask>* mt = new ThreadPool<MyTask>();//创建线程池
	if(nullptr == mt)
	{
		printf("mt is nullptr~\r\n");
		return;
	}
	if(nullptr == MyTask::mMyDb)
	{
		MyTask::mMyDb = new sqlite_tb();
		if(nullptr == MyTask::mMyDb)
		{
			printf("error:new MyTask::mMyDb is failed.\r\n");
			return ;
		}
		MyTask::mMyDb->OpenDB();
	}

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
		usleep(5000);//usleep(1000000);
	}
	arrange::endTime();
	mt->setIfExit(true);

	while(true)
    {
		if(MyTask::mSave8RetMap.size() < gbaseVec6Vec.size())
		{
			usleep(1000000);
			printf("proceeding>>>\r\n");
		#if 1
			timeCount++;
			if(timeCount > 10)
			{
				break;
			}
		#endif
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
	checkSet8VecTotal();

	delete mt;
	mt = nullptr;

	return;
}

void getTheBingoVec6FromPerVec6Vec(const vector<int> &baseVec6)
{
	vector<vector<int>> vec6Vec = vector<vector<int>>();
	vector<vector<int>> bingoVec6Vec = vector<vector<int>>();
	getVec6VecFromDb(baseVec6, vec6Vec);
	createVec6vecFromSqlDb(BINGO_DB_FILE, bingoVec6Vec);
	printf("vec6Vec.size=%ld--bingoVec6Vec.size=%ld\r\n", vec6Vec.size(), bingoVec6Vec.size());
	vectorsFindSameElems(vec6Vec, bingoVec6Vec);
	printf("vec6Vec.size=%ld--bingoVec6Vec.size=%ld\r\n", vec6Vec.size(), bingoVec6Vec.size());
	for(const auto &elemVec:vec6Vec)
	{
		printf("[\t");
		for(const auto &elem:elemVec)
		{
			printf("%d_", elem);
		}printf("\t]\r\n");
	}

	freeResource<vector<vector<int>>>(vec6Vec);
	freeResource<vector<vector<int>>>(bingoVec6Vec);
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
		freeResource<vector<vector<int>>>(tmp);
	}
	freeResource<set<vector<int>>>(set6Vec);
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
	freeResource<vector<vector<int>>>(vecVec):
#endif
	return;
}


void checkTask2Result()
{
	const char* sqlcmd = "create table if not exists tbldatas("
                            "elems text not null, result int not null default 0,"
                            "UNIQUE(elems, result));";
	for(const auto &elemVec:gbaseVec6Vec)
	{
		sqlite_tb *sql = nullptr;
		int retInt = 0;
		string vecBingoToStrs = "";
		string check8RetDb = SAVE6RET_OCCURETIME_DB_FILEDIR;
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
			MyTask2::mMapVec6TotalRet.emplace(elemVec, retInt);
		}

		sql->CloseDB();
		delete sql;
		sql = nullptr;
	}

	printf("MyTask::mSave8RetMap.size = %ld\r\n", MyTask2::mMapVec6TotalRet.size());
	createTask2FinalResultSqlDb();
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
			MyTask::mSave8RetMap.emplace(elemVec, retInt);
		}

		sql->CloseDB();
		delete sql;
		sql = nullptr;
	}

	printf("MyTask::mSave8RetMap.size = %ld\r\n", MyTask::mSave8RetMap.size());
	createFinalResultSqlDb();
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
 * 生成 mulSetVecAll
*/
bool genMulSet6VecAllFromDbs(multiset<vector<int>> &mulSetVecAll)
{
    bool ret = false;
    sqlite_tb *sql = nullptr;
	string sqlDbFile = SAVE6RET_DB_FILEDIR;
	string dataStr = "";
	vector<vector<int>> vec6VecAll = vector<vector<int>>();

	if(mulSetVecAll.size() > 0)
	{
		freeResource<multiset<vector<int>>>(mulSetVecAll);
	}

	arrange::startTime();
	printf("-----%s------startTime-------------\r\n", __FUNCTION__);
	for(const auto &elemVec:gbaseVec6Vec)
	{
		sqlDbFile = SAVE6RET_DB_FILEDIR;
		dataStr = "";
		vector<vector<int>> vec6Vec = vector<vector<int>>();
		for(const auto &elem : elemVec)
		{
			dataStr += arrange::NumberToString<int>(elem) + "_";
		}
		// printf("[\t %s \t]\r\n", dataStr.c_str());
		sqlDbFile += dataStr;
		sql = new sqlite_tb(sqlDbFile.c_str());
		ret = sql->OpenDB();
		if(!ret)
		{
			sql->CloseDB();
			delete sql;
			sql = nullptr;
			return ret;
		}
		ret = sql->SelectAllData(vec6Vec);
		if(!ret)
		{
			sql->CloseDB();
			delete sql;
			sql = nullptr;
			return ret;
		}

		sql->CloseDB();
		delete sql;
		sql = nullptr;

		vec6VecAll.insert(vec6VecAll.end(), vec6Vec.begin(), vec6Vec.end());

		freeResource<vector<vector<int>>>(vec6Vec);
		freeResource<string>(sqlDbFile);
		freeResource<string>(dataStr);
	}
	/**
	 * 不去重版本~
	*/
	multiset<vector<int>> mulSetVec(vec6VecAll.begin(), vec6VecAll.end());
	mulSetVec.swap(mulSetVecAll);
	arrange::endTime();

	freeResource<vector<vector<int>>>(vec6VecAll);
	freeResource<multiset<vector<int>>>(mulSetVec);
	printf("mulSetVecAll.size=%ld\r\n", mulSetVecAll.size());

	return ret;
}

/**
 * 用线程池生产者和消费者方式实现：gbaseVec6Vec 中每一项代表的数据块 vec6vec 中的每个vec6所出现的次数~
*/
void startTask2()
{
	int timeCount = 0;
	unsigned int index = 0;
	ThreadPool<MyTask2>* mt = new ThreadPool<MyTask2>();//创建线程池
	if(nullptr == mt)
	{
		printf("mt is nullptr~\r\n");
		return;
	}
	genMulSet6VecAllFromDbs(MyTask2::mVec6VecAll);
	arrange::startTime();
	printf("-----%s------startTime-------------\r\n", __FUNCTION__);
	if(false == mt->ThreadPoolInit())
	{
		printf("error:ThreadPoolInit failed.\r\n");
		return ;
	}

	for(const auto &elemVec:gbaseVec6Vec)
	{
		MyTask2 t(elemVec, ++index);//创建任务
		mt->Push(t);//将任务推送给队列中
		usleep(100000);
	}
	mt->setIfExit(true);
	arrange::endTime();
	while(true)
	{
		usleep(1000000);
		printf("proceeding>>>--->%d\r\n", timeCount);
		timeCount++;
		if(timeCount > 20)
		{
			createTask2FinalResultSqlDb();
			break;
		}
	}

	delete mt;
	mt = nullptr;
	return;
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
	printf("setA.size=%ld, a.size=%ld\r\n", setA.size(), a.size());
	printf("setB.size=%ld, b.size=%ld\r\n", setB.size(), b.size());
	set_intersection(setA.begin(), setA.end(), setB.begin(), setB.end(), \
					insert_iterator<set<vector<int>>>(setRet,setRet.begin()));
	vector<vector<int>> retVec(setRet.begin(), setRet.end());
	retVec.swap(a);

	freeResource<set<vector<int>>>(setA);
	freeResource<set<vector<int>>>(setB);
	freeResource<set<vector<int>>>(setRet);
	freeResource<vector<vector<int>>>(retVec);
#else
	//方法1：
	a.insert(a.end(), b.begin(), b.end());
	set<vector<int>> uniqueSetA(a.begin(), a.end());
	multiset<vector<int>> multiSetA(a.begin(), a.end());
	multiSetA.erase(uniqueSetA.begin(), uniqueSetA.end());
	vector<vector<int>> mergeVec6Vec(multiSetA.begin(), multiSetA.end());
	mergeVec6Vec.swap(a);//交换两个容器的内容

	freeResource<set<vector<int>>>(uniqueSetA);
	freeResource<multiset<vector<int>>>(multiSetA);

	//方法2：
	// a.insert(a.end(), b.begin(), b.end());
	// sort(a.begin(), a.end());
	// auto iter = std::unique(a.begin(), a.end());
    // a.erase(iter, a.end());//->剩下的都是唯一的元素，不能用a.erase(a.begin(), iter);来找重复的元素，不能不能！！！

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

	freeResource<set<vector<int>>>(setA);
	freeResource<set<vector<int>>>(setB);
	freeResource<set<vector<int>>>(setRet);
	freeResource<vector<vector<int>>>(retVec);
#else
	set<vector<int>> uniqueElementsSetA(a.begin(), a.end());
	uniqueElementsSetA.insert(b.begin(), b.end());
	vector<vector<int>> mergeVec6Vec(uniqueElementsSetA.begin(), uniqueElementsSetA.end());
	mergeVec6Vec.swap(a);//交换两个容器的内容

	freeResource<set<vector<int>>>(uniqueElementsSetA);
	freeResource<vector<vector<int>>>(mergeVec6Vec);
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

	freeResource<set<vector<int>>>(setA);
	freeResource<set<vector<int>>>(setB);
	freeResource<set<vector<int>>>(setRet);
	freeResource<vector<vector<int>>>(retVec);
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

	freeResource<set<vector<int>>>(uniqueElementsSetA);
	freeResource<set<vector<int>>>(uniqueElementsSetB);
	freeResource<vector<vector<int>>>(mergeVec6Vec);
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
	freeResource<set<vector<int>>>(ret);

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
	freeResource<set<vector<int>>>(uniqueElementsSet);
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
	freeResource<set<vector<int>>>(c);
	freeResource<set<vector<int>>>(ret);
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
	freeResource<set<set<int>>>(uniqueElementsSet);
#else
	/**
	 * 此方法会快一些
	*/
	unsigned long add = 0;
	set<set<int>> ret = set<set<int>>();
	set_difference(b.begin(), b.end(), a.begin(), a.end(), insert_iterator<set<set<int>>>(ret,ret.begin()));
	add = ret.size();
	/**
	 * 注意要释放资源，不然会内存泄漏
	*/
	freeResource<set<set<int>>>(ret);
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
			freeResource<vector<int>>(insertVec6);
			insertVec6 = elemVec;
			addValue = value;
		}
	}

	uniqueElementsSet.insert(LargeDatas.at(insertVec6).begin(), LargeDatas.at(insertVec6).end());
	finalVec6Vec.push_back(insertVec6);
	leftVec6Vec.erase(insertVec6);

	freeResource<vector<int>>(insertVec6);
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
			freeResource<vector<int>>(insertVec6);
			insertVec6 = elemVec;
			addValue = value;
		}
	}

	uniqueElementsSet.insert(LargeDatas.at(insertVec6).begin(), LargeDatas.at(insertVec6).end());
	finalVec6Vec.push_back(insertVec6);
	leftVec6Vec.erase(insertVec6);

	freeResource<vector<int>>(insertVec6);
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
			freeResource<set<int>>(insertSet6);
			insertSet6 = elemSet;
			addValue = value;
		}
	}

	uniqueElementsSet.insert(LargeDatas.at(insertSet6).begin(), LargeDatas.at(insertSet6).end());
	finalSetSet.insert(insertSet6);
	leftSetSet.erase(insertSet6);

	freeResource<set<int>>(insertSet6);
	return;
}


void getLargeSetVecDatas(const vector<vector<int>> &baseVec6Vec, map<vector<int>, set<vector<int>>> &LargeDatas)
{
	/**
	 * 去脏数据
	*/
	if(LargeDatas.size() > 0)
	{
		/**
		 * 清空脏数据
		*/
		freeResource<map<vector<int>, set<vector<int>>>>(LargeDatas);
	}

	arrange::startTime();
	printf("-----------startTime-------------%s\r\n", __FUNCTION__);
	for(const auto &elemVec:baseVec6Vec)
	{
		set<vector<int>> set6Vec = set<vector<int>>();
		getSet6VecFromDb(elemVec, set6Vec);
		// LargeDatas.insert(make_pair(elemVec, set6Vec));
		LargeDatas.emplace(elemVec, set6Vec);
		freeResource<set<vector<int>>>(set6Vec);
	}
	printf("-----------endTime-------------%ld\r\n", LargeDatas.size());
	arrange::endTime();
	return;
}

void getLargeVecVecDatas(const vector<vector<int>> &baseVec6Vec, map<vector<int>, vector<vector<int>>> &LargeDatas)
{
	/**
	 * 去脏数据
	*/
	if(LargeDatas.size() > 0)
	{
		/**
		 * 清空脏数据
		*/
		freeResource<map<vector<int>, vector<vector<int>>>>(LargeDatas);
	}

	arrange::startTime();
	printf("-----------startTime-------------%s\r\n", __FUNCTION__);
	for(const auto &elemVec:baseVec6Vec)
	{
		vector<vector<int>> vec6Vec = vector<vector<int>>();
		getVec6VecFromDb(elemVec, vec6Vec);
		// LargeDatas.insert(make_pair(elemVec, vec6Vec));
		LargeDatas.emplace(elemVec, vec6Vec);
		freeResource<vector<vector<int>>>(vec6Vec);
	}
	printf("-----------endTime-------------%ld\r\n", LargeDatas.size());
	arrange::endTime();
	return;
}

void getLargeSetSetDatas(const set<set<int>> &baseSet6Set, map<set<int>, set<set<int>>> &LargeDatas)
{
	/**
	 * 去脏数据
	*/
	if(LargeDatas.size() > 0)
	{
		/**
		 * 清空脏数据
		*/
		freeResource<map<set<int>, set<set<int>>>>(LargeDatas);
	}

	arrange::startTime();
	printf("-----------startTime-------------%s\r\n", __FUNCTION__);
	for(const auto &elemSet:baseSet6Set)
	{
		set<set<int>> set6Set = set<set<int>>();
		getSet6SetFromDb(elemSet, set6Set);
		// LargeDatas.insert(make_pair(elemSet, set6Set));
		LargeDatas.emplace(elemSet, set6Set);
		freeResource<set<set<int>>>(set6Set);
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

	freeResource<set<set<int>>>(leftSetSet);
	freeResource<set<set<int>>>(uniqueElementsSet);

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
	// unsigned long baseVec6VecSize = baseVec6Vec.size();

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
	// while(finalVec6Vec.size() < baseVec6VecSize)
	while(finalVec6Vec.size() < 700)
	{
		printf("------------%ld------------%ld\r\n", finalVec6Vec.size(), uniqueElementsSet.size());
		getFinalVec6VecFromLargeMapSetVecDatas(finalVec6Vec, uniqueElementsSet, leftVec6Vec, LargeDatas);
	}
	arrange::endTime();

	creatVecVecResult<vector<vector<int>>>(finalVec6Vec , FINALVEC6VEC_DB);
	creatVecVecResult<set<vector<int>>>(uniqueElementsSet , FINALUNIQUESET6VEC_DB);

	freeResource<set<vector<int>>>(leftVec6Vec);
	freeResource<set<vector<int>>>(uniqueElementsSet);

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

	freeResource<set<vector<int>>>(leftVec6Vec);
	freeResource<set<vector<int>>>(uniqueElementsSet);

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

	freeResource<set<vector<int>>>(leftVec6Vec);
	freeResource<set<vector<int>>>(uniqueElementsSet);

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

	// creatVecVecResult<vector<vector<int>>>(finalVec , OBJECT_EXE_DIR "FinalBaseVec6Vec");//可执行文件 getVecAddValue->
	// creatVecVecResult<vector<vector<int>>>(finalVec , OBJECT_EXE_DIR "FinalBaseVec6Vec_0201");//可执行文件 FinalBaseVec6Vec_0201->
	// creatVecVecResult<vector<vector<int>>>(finalVec , OBJECT_EXE_DIR "getCoverResultDatas7");//可执行文件 getCoverResultDatas7->
	// creatVecVecResult<vector<vector<int>>>(finalVec , OBJECT_EXE_DIR "getCoverResultDatas9");//可执行文件 catchSigIntSaveFinalVec6VecToDbFile->
	creatVecVecResult<vector<vector<int>>>(finalVec , OBJECT_EXE_DIR "getCoverResultDatas3");//可执行文件 getCoverResultDatas3->
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

	freeResource<set<vector<int>>>(uniqueElementsSet);

	genFinalBaseVec6Vec(vec6intPairVec);

	freeResource<vector<pair<vector<int>, int>>>(vec6intPairVec);
	return;
}


/**
 * 用set加insert的特性可去重复来实现：效率最高
 * 得到 BASESQLDBDATA 所对应的所有数据块的覆盖率：
*/
void getCoverResultDatas3(const map<vector<int>, vector<vector<int>>> &LargeDatas, \
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

	freeResource<set<vector<int>>>(uniqueElementsSet);

	genFinalBaseVec6Vec(vec6intPairVec);

	freeResource<vector<pair<vector<int>, int>>>(vec6intPairVec);
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

	creatVecVecResult<vector<vector<int>>>(vec6VecAll, OBJECT_EXE_DIR "Vec6VecAll_letf");
	creatVecVecResult<vector<vector<int>>>(vec6VecUseFull, OBJECT_EXE_DIR "vec6VecUseFull");
	creatVecVecResult<vector<vector<int>>>(vec6VecUseLess, OBJECT_EXE_DIR "vec6VecUseLess");
	return;
}

void findTheInterSectionFromThe2Vec10Vec()
{
	vector<int> Vec10Ori1 = {4,5,8,10,13, 16,17,19,22,28};
	vector<int> Vec10Ori2 = {4,5,8,10,13, 15,18,20,21,30};

	vector<int> resVec1 = vector<int>(6, 0);
	vector<int> resVec2 = vector<int>(6, 0);
	vector<vector<int>> resVec6Vec1 = vector<vector<int>>();
	vector<vector<int>> resVec6Vec2 = vector<vector<int>>();
	arrange::recursion(Vec10Ori1, resVec1, 0, 0, static_cast<int>(resVec1.size()), resVec6Vec1);
	arrange::recursion(Vec10Ori2, resVec2, 0, 0, static_cast<int>(resVec2.size()), resVec6Vec2);
	vectorsFindSameElems(resVec6Vec1, resVec6Vec2);
	printf("resVec6Vec1.size=%ld\r\n", resVec6Vec1.size());

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
	freeResource<set<int>>(set6A);
	freeResource<set<int>>(set6B);
	freeResource<set<set<int>>>(set6SetA);
	freeResource<set<set<int>>>(set6SetB);
	freeResource<set<set<int>>>(retSetSet);

	printf("the same amount is %d\r\n", ret);
	return;
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


void createTask2FinalResultSqlDb()
{
	sqlite_tb *sql = nullptr;
	sql = new sqlite_tb(TASK2_FINAL_RET_DB_FILE);
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
		vector<pair<vector<int>, int>> save8RetVec(MyTask2::mMapVec6TotalRet.begin(), MyTask2::mMapVec6TotalRet.end());
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

		printf("MyTask2::mMapVec6TotalRet.size = %ld\r\n", MyTask2::mMapVec6TotalRet.size());
	}

	sql->CloseDB();
	delete sql;
	sql = nullptr;

	freeResource<multiset<vector<int>>>(MyTask2::mVec6VecAll);
	freeResource<map<vector<int>, int>>(MyTask2::mMapVec6TotalRet);
	return;
}

/**
 * 创建最终 gVecBingoVec 中的每个（包含6个号码的）子集合的所代表的数据块，命中期数的统计~
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

	freeResource<map<vector<int>, int>>(MyTask::mSave8RetMap);
	return;
}

bool ifIsTheSpecialVecDatas(const vector<int> &vec6)
{
	bool ret = true;
	if(vec6.size() < 6)
	{
		printf("error:the input data is wrong~");
		return ret;
	}

	// if((0 == vec6[0]%2)&&(0 == vec6[1]%2)
	//  &&(0 == vec6[2]%2)&&(0 == vec6[3]%2)
	//  &&(0 == vec6[4]%2)&&(0 == vec6[5]%2))
	// {
	// 	return ret;
	// }

	// if((1 == vec6[0]%2)&&(1 == vec6[1]%2)
	//  &&(1 == vec6[2]%2)&&(1 == vec6[3]%2)
	//  &&(1 == vec6[4]%2)&&(1 == vec6[5]%2))
	// {
	// 	return ret;
	// }

	set<int> distanceSet = set<int>();
	distanceSet.insert(vec6[5]-vec6[4]);
	distanceSet.insert(vec6[4]-vec6[3]);
	distanceSet.insert(vec6[3]-vec6[2]);
	distanceSet.insert(vec6[2]-vec6[1]);
	distanceSet.insert(vec6[1]-vec6[0]);
	if(distanceSet.size()<=1)
	{
		return ret;
	}

	freeResource<set<int>>(distanceSet);
	return false;
}


/**
 * 删除特殊值集合
 * 1，连续值的集合，<有5个及5个以上的连续值的集合>
 * 2，全奇数
 * 3，全偶数
 * 4，等间距值
*/
template<typename T>
void deleteTheSpecialDatas(T &resSet6Vec)
{
	string dbName = "";
	vector<vector<int>> needDeleteDatas = vector<vector<int>>();
	for(const auto &elem6Vec:resSet6Vec)
	{
		if(ifIsTheSpecialVecDatas(elem6Vec))
		{
			needDeleteDatas.push_back(elem6Vec);
		}
	}

	dbName = dbName + BASESQLDBDATAFILEDIR + "/needDeleteDatas";
	creatVecVecResult<vector<vector<int>>>(needDeleteDatas, dbName.c_str(), 6);
	for(const auto &delElemVec : needDeleteDatas)
	{
		resSet6Vec.erase(delElemVec);
	}
	printf("[%s]resSet6Vec.size=%ld\r\n", __FUNCTION__, resSet6Vec.size());

	return;
}

__attribute__((unused)) static bool defineCmpRule(const pair<int, vector<vector<int>>> &a,
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

void checkBingoVec6Datas()
{
	vector<vector<int>> bingoVec6Vec = vector<vector<int>>();
	vector<vector<int>> resVec6Vec = vector<vector<int>>();
	int operateIndex = 0;
	string dbName = "";
	vector<int> vecOrderIndex = {397,63,64,90,470,29,106,522,14,28,50,91,118,
								124,128,197,200,319,331,333,348,472,22,47,53,
								61,66,71,92,122,155,160,162,177,208,211,213,
								222,245,254,275,292,299,307,327,377,387,411,
								471,499,20,27,34,35,57,76,87,123,138,144,163,
								174,180,186,205,207,218,224,238,239,258,263,
								274,279,280,295,300,321,323,328,339,375,392,
								399,400,430,448,456,483,515,521,524,546,4,5,
								10,17,21,36,38,43,54,58,74,88,100,101,107,109,
								121,133,142,143,158,169,173,179,182,189,210,220,
								227,235,246,257,268,269,284,287,296,297,306,
								311,312,315,316,340,341,355,359,363,368,373,
								379,384,386,390,394,409,433,437,439,444,445,
								447,460,465,476,479,482,485,487,493,531,1,3,
								6,15,31,33,37,40,48,49,55,62,68,72,82,89,94,
								102,104,110,114,119,131,135,139,140,141,147,
								151,156,157,175,184,202,206,215,221,236,240,
								242,249,250,278,282,291,313,320,332,334,343,
								356,357,360,367,372,383,401,407,413,414,424,
								425,427,432,443,451,453,463,464,466,467,477,
								480,481,488,494,495,497,504,510,512,516,517,
								518,525,528,529,536,538,539,542,545,8,16,23,
								25,32,39,41,51,59,69,95,99,103,113,115,130,137,
								146,149,154,166,168,170,172,176,178,181,187,
								198,199,204,209,219,225,226,229,230,232,234,
								241,259,270,271,276,285,288,289,293,294,309,
								310,330,338,342,347,352,353,354,358,361,362,
								364,365,369,376,381,385,389,402,405,412,417,
								418,419,422,429,431,434,436,440,455,458,461,
								474,475,489,492,498,500,502,505,513,523,532,
								534,540,543,544,548,553,554,2,11,46,56,60,70,
								75,77,78,79,84,86,93,97,98,112,120,125,127,129,
								132,152,153,161,191,194,196,201,203,212,214,
								223,237,248,255,260,264,265,272,273,283,305,
								314,317,322,324,326,329,345,349,350,370,371,
								378,380,391,408,420,428,435,442,450,452,454,
								457,462,473,478,484,486,490,491,508,511,514,
								520,526,527,537,541,547,549,552,7,9,12,19,24,
								30,44,45,52,67,73,96,108,111,116,134,145,148,
								159,164,165,167,171,188,190,192,216,217,228,231,
								247,256,267,277,281,298,301,303,304,308,335,336,
								337,344,346,366,374,382,388,393,396,398,403,406,
								410,416,421,426,438,441,446,449,468,469,501,503,
								509,530,533,535,551,13,18,26,42,65,80,81,83,85,
								126,150,183,185,193,233,243,244,251,252,253,261,
								262,266,286,302,325,351,395,404,459,496,507,519,
								550,105,117,290,318,415,423,506,136,195};
	createVec6vecFromSqlDb(BINGO_DB_FILE, bingoVec6Vec);//生成  bingoVec6Vec
	// map<string, vector<vector<int>>> strMapVec6Vec = map<string, vector<vector<int>>>();
	// vector<pair<string, vector<vector<int>>>> vecPairStrMapVec6Vec = vector<pair<string, vector<vector<int>>>>();
	map<int, vector<vector<int>>> strMapVec6Vec = map<int, vector<vector<int>>>();
	vector<pair<int, vector<vector<int>>>> vecPairStrMapVec6Vec = vector<pair<int, vector<vector<int>>>>();

	while(1)
	{
		operateIndex++;
		dbName = dbName + BASESQLDBDATAFILEDIR + "/baseSqliteDbData" + to_string(operateIndex);
		createVec6vecFromSqlDb(dbName.c_str(), resVec6Vec);//生成  resVec6Vec
		if(resVec6Vec.size() <= 0)
		{
			freeResource<string>(dbName);
			break;
		}
		vectorsFindSameElems(resVec6Vec, bingoVec6Vec);
		strMapVec6Vec.emplace(operateIndex, resVec6Vec);
		freeResource<string>(dbName);
		freeResource<vector<vector<int>>>(resVec6Vec);
	}

	// vector<pair<int, vector<vector<int>>>>(strMapVec6Vec.begin(), strMapVec6Vec.end()).swap(vecPairStrMapVec6Vec);
	// sort(vecPairStrMapVec6Vec.begin(), vecPairStrMapVec6Vec.end(), defineCmpRule);
	// for(const auto &pairElem : vecPairStrMapVec6Vec)
	// {
	// 	// printf("%s:\t", basename(const_cast<char*>(pairElem.first.c_str())));
	// 	printf("%d:\t", pairElem.first);
	// 	// vecOrderIndex.push_back(pairElem.first);
	// 	for(const auto &elemVec : pairElem.second)
	// 	{
	// 		for(const auto &elem : elemVec)
	// 		{
	// 			printf("%d_", elem);
	// 		}printf("\r\n\t");
	// 	}
	// 	printf("\t\r\n");
	// }

	for(int i=0;i<static_cast<int>(vecOrderIndex.size());i++)
	{
		printf("%d:\t", vecOrderIndex[i]);
		for(const auto &elemVec : strMapVec6Vec[vecOrderIndex[i]])
		{
			for(const auto &elem : elemVec)
			{
				printf("%d_", elem);
			}printf("\r\n\t");
		}
		printf("\t\r\n");
	}

	// printf("(");
	// for(const auto &elem : vecOrderIndex)
	// {
	// 	printf("%d,", elem);
	// }printf(")\r\n");

	freeResource<vector<vector<int>>>(bingoVec6Vec);
	freeResource<map<int, vector<vector<int>>>>(strMapVec6Vec);
	freeResource<vector<pair<int, vector<vector<int>>>>>(vecPairStrMapVec6Vec);
	freeResource<vector<int>>(vecOrderIndex);
	return;
}

void genVec6VecByVecN(const vector<int> &oriVecN, vector<vector<int>> &RetVec6Vec)
{
	vector<int> resVec6 = vector<int>(6, 0);
	arrange::recursion(oriVecN, resVec6, 0, 0, static_cast<int>(resVec6.size()), RetVec6Vec);
	freeResource<vector<int>>(resVec6);
	return;
}
void checkBingoResult()
{
	vector<int> oriVec8_1{2, 5, 17, 19, 20, 21, 22, 23};
	vector<int> oriVec8_2{2, 5, 18, 24, 25, 26, 27, 28};
	vector<int> oriVec8_3{2, 5, 19, 29, 30, 31, 32, 33};
	vector<vector<int>> RetVec6Vec_1 = vector<vector<int>>();
	vector<vector<int>> RetVec6Vec_2 = vector<vector<int>>();
	vector<vector<int>> RetVec6Vec_3 = vector<vector<int>>();
	vector<vector<int>> bingoVec6Vec = vector<vector<int>>();
	vector<vector<int>> retMergeVec6Vec = vector<vector<int>>();

	genVec6VecByVecN(oriVec8_1, RetVec6Vec_1);printf("RetVec6Vec_1.size=%ld\r\n", RetVec6Vec_1.size());
	genVec6VecByVecN(oriVec8_2, RetVec6Vec_2);printf("RetVec6Vec_2.size=%ld\r\n", RetVec6Vec_2.size());
	genVec6VecByVecN(oriVec8_3, RetVec6Vec_3);printf("RetVec6Vec_3.size=%ld\r\n", RetVec6Vec_3.size());
	retMergeVec6Vec.insert(retMergeVec6Vec.end(), RetVec6Vec_1.begin(), RetVec6Vec_1.end());
	retMergeVec6Vec.insert(retMergeVec6Vec.end(), RetVec6Vec_2.begin(), RetVec6Vec_2.end());
	retMergeVec6Vec.insert(retMergeVec6Vec.end(), RetVec6Vec_3.begin(), RetVec6Vec_3.end());
	printf("retMergeVec6Vec.size=%ld\r\n", retMergeVec6Vec.size());
	createVec6vecFromSqlDb(BINGO_DB_FILE, bingoVec6Vec);//生成  bingoVec6Vec
	printf("bingoVec6Vec.size=%ld\r\n", bingoVec6Vec.size());
	vectorsFindSameElems(retMergeVec6Vec, bingoVec6Vec);

	printf("[vectorsFindSameElems]retMergeVec6Vec.size=%ld\r\n", retMergeVec6Vec.size());
	for(const auto &elemVec:retMergeVec6Vec)
	{
		for(const auto &elem:elemVec)
		{
			printf("%d_", elem);
		}printf("\r\n");
	}

	freeResource<vector<int>>(oriVec8_1);
	freeResource<vector<int>>(oriVec8_2);
	freeResource<vector<int>>(oriVec8_3);
	freeResource<vector<vector<int>>>(RetVec6Vec_1);
	freeResource<vector<vector<int>>>(RetVec6Vec_2);
	freeResource<vector<vector<int>>>(RetVec6Vec_3);
	freeResource<vector<vector<int>>>(bingoVec6Vec);
	freeResource<vector<vector<int>>>(retMergeVec6Vec);
	return;
}

void createBaseSqlDb_Select6From33()
{
	vector<int> oriVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
							  11,12,13,14,15,16,17,18,19,20,
							  21,22,23,24,25,26,27,28,29,30,
							  31,32,33 };//
	int operateIndex = 0;
	vector<int> resVec = vector<int>(6, 0);
	vector<vector<int>> resVec6Vec = vector<vector<int>>();
	vector<vector<int>> temp = vector<vector<int>>();

	arrange::recursion(oriVec, resVec, 0, 0, static_cast<int>(resVec.size()), resVec6Vec);
	printf("resVec6Vec.size=%ld\r\n", resVec6Vec.size());
	set<vector<int>> resSet6Vec(resVec6Vec.begin(), resVec6Vec.end());
	// creatVecVecResult<vector<vector<int>>>(resVec6Vec, "./baseVec6/baseSqliteDbData_Select6From33");
	freeResource<vector<vector<int>>>(resVec6Vec);
	freeResource<vector<int>>(resVec);
	freeResource<vector<int>>(oriVec);
	deleteTheSpecialDatas<set<vector<int>>>(resSet6Vec);
	for(const auto &elem6Vec:resSet6Vec)
	{
		temp.push_back(elem6Vec);
		if(temp.size() >= 2000)
		{
			string dbName = "";
			operateIndex++;
			dbName = dbName + BASESQLDBDATAFILEDIR + "/baseSqliteDbData" + to_string(operateIndex);
			creatVecVecResult<vector<vector<int>>>(temp, dbName.c_str(), 6);
			freeResource<vector<vector<int>>>(temp);
			freeResource<string>(dbName);
		}
	}
	if(temp.size() > 0)
	{
		string dbName = "";
		operateIndex++;
		dbName = dbName + BASESQLDBDATAFILEDIR + "/baseSqliteDbData" + to_string(operateIndex);
		creatVecVecResult<vector<vector<int>>>(temp, dbName.c_str(), 6);
		freeResource<vector<vector<int>>>(temp);
		freeResource<string>(dbName);
	}
	freeResource<set<vector<int>>>(resSet6Vec);
	freeResource<vector<vector<int>>>(resVec6Vec);
	return;
}

void createBaseSqlDb_Select10From33(set<vector<int>> &resSet10Vec)
{
	const vector<int> oriVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
							  11,12,13,14,15,16,17,18,19,20,
							  21,22,23,24,25,26,27,28,29,30,
							  31,32,33 };//
	vector<int> resVec = vector<int>(VEC_ELEMS_NUM, 0);
	// vector<vector<int>> resVec10Vec = vector<vector<int>>();
	arrange::recursion(oriVec, resVec, 0, 0, static_cast<int>(resVec.size()), resSet10Vec);
	freeResource<vector<int>>(resVec);
	// convertVecVec2SetVec(resVec10Vec, resSet10Vec);
	// freeResource<vector<vector<int>>>(resVec10Vec);
	return;
}

void findBaseSqlDataFromSet10Vec(set<vector<int>> &resSet10Vec)
{
	arrange::startTime();
	printf("-------startTime[%s]\r\n", __FUNCTION__);
	createSetXVecFromSqlDb<set<vector<int>>>(FINALSET10VEC_DB, resSet10Vec, VEC_ELEMS_NUM);//生成  gResSet10Vec
	if(resSet10Vec.size() <= 0)
	{
		createBaseSqlDb_Select10From33(resSet10Vec);
	}
	arrange::endTime();

	printf("resSet10Vec.size=%ld\r\n", resSet10Vec.size());
	arrange::startTime();
	printf("--------startTime[%s]\r\n", __FUNCTION__);
	// findBaseSqlData4(resSet10Vec);
	findBaseSqlData5(resSet10Vec);
	arrange::endTime();

	/**
	 * 这里生成的就是 FINALSET10VEC_DB  所用到的文件
	*/
	system("rm " FINALSET10VEC_DB " -rf;");
	system("sync;");
	printf("--------startTime[creatVecVecResult]\r\n");
	creatVecVecResult<set<vector<int>>>(resSet10Vec, FINALSET10VEC_DB, VEC_ELEMS_NUM);
	arrange::endTime();
	freeResource<set<vector<int>>>(resSet10Vec);
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
		freeResource<set<set<int>>>(setSet);
	}

	for(const auto &elemVec:vecVec)
	{
		set<int> tmp(elemVec.begin(), elemVec.end());
		setSet.insert(tmp);
		freeResource<set<int>>(tmp);
	}
	return;
}

void convertVecVec2SetVec(const vector<vector<int>> &vecVec, set<vector<int>> &setVec)
{
	if(setVec.size() > 0)
	{//若不为空，则清空之前的数据，避免内存泄漏，或者得到额setSet里面会有脏数据~
		freeResource<set<vector<int>>>(setVec);
	}
	set<vector<int>> tmp(vecVec.begin(), vecVec.end());
	tmp.swap(setVec);
	freeResource<set<vector<int>>>(tmp);
	return;
}

void convertSetSet2VecVec(const set<set<int>> &setSet, vector<vector<int>> &vecVec)
{
	if(vecVec.size() > 0)
	{//若不为空，则清空之前的数据，避免内存泄漏，或者得到额vecVec里面会有脏数据~
		freeResource<vector<vector<int>>>(vecVec);
	}

	for(const auto &elemSet:setSet)
	{
		vector<int> tmp(elemSet.begin(), elemSet.end());
		vecVec.push_back(tmp);
		freeResource<vector<int>>(tmp);
	}
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
	creatVecVecResult<vector<vector<int>>>(vecBingoVecRet, BASESQLDBDATA "_13");
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
	creatVecVecResult<vector<vector<int>>>(vecBingoVecRet, BASESQLDBDATA "_13_1");
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
		freeResource<set<int>>(tmp);
	}
	printf("%s--%d--Start~--setBaseSet.size=%ld\r\n",__func__, __LINE__, setBaseSet.size());

	setBaseSetConst.insert(setBaseSet.begin(), setBaseSet.end());

	for(const auto &elemSetConst : setBaseSetConst)
	{
		if(setBaseSet.find(elemSetConst) == setBaseSet.end())
		{//未找到，不存在
			continue;
		}

		set<set<int>> setDeleteSet = set<set<int>>();

		for(const auto &elemSet : setBaseSet)
		{
			if(elemSet != elemSetConst)
			{
				set<int> tmp = set<int>();

				/**
				 * 取两个容器的相同的元素的，装入到tmp容器里面；
				*/
				set_intersection(elemSet.begin(), elemSet.end(), elemSetConst.begin(), elemSetConst.end(),
									insert_iterator<set<int>>(tmp, tmp.begin()));
				// printf("tmp.size=%ld\r\n", tmp.size());
				if(tmp.size() >= 3)//被删除的判断条件~相同的数字数量的判断条件
				{
					setDeleteSet.insert(elemSet);
				}

				freeResource<set<int>>(tmp);
			}
		}

		for(const auto &delElemSet : setDeleteSet)
		{
			setBaseSet.erase(delElemSet);
		}
		freeResource<set<set<int>>>(setDeleteSet);
		printf("setBaseSet.size=%ld\r\n", setBaseSet.size());
	}

	for(const auto &elemSet : setBaseSet)
	{
		vector<int> tmp(elemSet.begin(), elemSet.end());
		vecBaseRetVec.push_back(tmp);
		freeResource<vector<int>>(tmp);
	}

	freeResource<set<set<int>>>(setBaseSet);
	freeResource<set<set<int>>>(setBaseSetConst);

	printf("vecBaseRetVec.size=%ld\r\n", vecBaseRetVec.size());
	/**
	 * 这里生成的就是 BASESQLDBDATA  所用到的文件
	*/
	creatVecVecResult<vector<vector<int>>>(vecBaseRetVec, BASESQLDBDATA "_baseVecVec_Select10From33");
	printf("%s--%d--End~\r\n",__func__, __LINE__);
	return;
}

void findBaseSqlData5(set<vector<int>> &setBaseVec)
{
	vector<vector<int>> vecBaseVecConst = vector<vector<int>>();
	createSetXVecFromSqlDb<vector<vector<int>>>("./baseVec6/catchSigIntSaveFinalSet10VecToDbFile", vecBaseVecConst, VEC_ELEMS_NUM);
	printf("%s--%d--Start~--vecBaseVecConst.size=%ld\r\n",__func__, __LINE__, vecBaseVecConst.size());

	for(const auto &elemBaseVecConst : vecBaseVecConst)
	{
		if(setBaseVec.find(elemBaseVecConst) == setBaseVec.end())
		{//未找到，不存在
			continue;
		}

		vector<vector<int>> vecDeleteVec = vector<vector<int>>();

		for(const auto &elemBaseVec : setBaseVec)
		{
			if(elemBaseVecConst != elemBaseVec)
			{
				set<int> tmp = set<int>();
				set<int> a(elemBaseVec.begin(), elemBaseVec.end());
				set<int> b(elemBaseVecConst.begin(), elemBaseVecConst.end());

				/**
				 * 取两个容器的相同的元素的，装入到tmp容器里面；
				*/
				set_intersection(a.begin(), a.end(), b.begin(), b.end(),
									insert_iterator<set<int>>(tmp, tmp.begin()));
				// printf("tmp.size=%ld\r\n", tmp.size());
				if(tmp.size() > 6)//被删除的判断条件~相同的数字数量的判断条件
				{
					vecDeleteVec.push_back(elemBaseVec);
				}

				freeResource<set<int>>(a);
				freeResource<set<int>>(b);
				freeResource<set<int>>(tmp);
			}
		}

		for(const auto &delElemVec : vecDeleteVec)
		{
			setBaseVec.erase(delElemVec);
		}
		freeResource<vector<vector<int>>>(vecDeleteVec);
		printf("setBaseVec.size=%ld\r\n", setBaseVec.size());
	}

	printf("setBaseVec.size=%ld\r\n", setBaseVec.size());
	printf("%s--%d--End~\r\n",__func__, __LINE__);
	freeResource<vector<vector<int>>>(vecBaseVecConst);
	return;
}


/**
 * 转化成set容器取查找和删除和插入动作都是效率最高的
*/
void findBaseSqlData4(set<vector<int>> &setBaseVec)
{
	vector<vector<int>> vecBaseVecConst(setBaseVec.begin(), setBaseVec.end());
	printf("%s--%d--Start~--vecBaseVecConst.size=%ld\r\n",__func__, __LINE__, vecBaseVecConst.size());

	for(const auto &elemBaseVecConst : vecBaseVecConst)
	{
		if(setBaseVec.find(elemBaseVecConst) == setBaseVec.end())
		{//未找到，不存在
			continue;
		}

		vector<vector<int>> vecDeleteVec = vector<vector<int>>();

		for(const auto &elemBaseVec : setBaseVec)
		{
			if(elemBaseVecConst != elemBaseVec)
			{
				set<int> tmp = set<int>();
				set<int> a(elemBaseVec.begin(), elemBaseVec.end());
				set<int> b(elemBaseVecConst.begin(), elemBaseVecConst.end());

				/**
				 * 取两个容器的相同的元素的，装入到tmp容器里面；
				*/
				set_intersection(a.begin(), a.end(), b.begin(), b.end(),
									insert_iterator<set<int>>(tmp, tmp.begin()));
				// printf("tmp.size=%ld\r\n", tmp.size());
				if(tmp.size() >= 6)//被删除的判断条件~相同的数字数量的判断条件
				{
					vecDeleteVec.push_back(elemBaseVec);
				}

				freeResource<set<int>>(a);
				freeResource<set<int>>(b);
				freeResource<set<int>>(tmp);
			}
		}

		for(const auto &delElemVec : vecDeleteVec)
		{
			setBaseVec.erase(delElemVec);
		}
		freeResource<vector<vector<int>>>(vecDeleteVec);
		printf("setBaseVec.size=%ld\r\n", setBaseVec.size());
	}

	printf("setBaseVec.size=%ld\r\n", setBaseVec.size());
	printf("%s--%d--End~\r\n",__func__, __LINE__);
	freeResource<vector<vector<int>>>(vecBaseVecConst);
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
	sql->InsertDataFromSourceFile(SOURCE_FILE);
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


