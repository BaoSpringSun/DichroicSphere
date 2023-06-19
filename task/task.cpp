#include "task.hpp"
#include "arrange.hpp"

sqlite_tb* MyTask::mMyDb = nullptr;
std::mutex MyTask::mLock;
map<vector<int>, int> MyTask::mSave8RetMap;//->统计gVecBingoVec中每个子集合和对应命中结果的表
MyTask::MyTask(const vector<int> &bingoVec)
    :mVecBingo(bingoVec)
    ,mSaveVec6RetDb("")
    ,mSaveVec8RetDb("")
    ,mDb8Sql(nullptr)
    ,mTotalBingo(0)
    ,mVecBingoToStrs("")
{
    mVec8Vec.clear();
}

MyTask::MyTask()
    :mSaveVec6RetDb("")
    ,mSaveVec8RetDb("")
    ,mDb8Sql(nullptr)
    ,mTotalBingo(0)
    ,mVecBingoToStrs("")
{
    mVecBingo.clear();
    mVec8Vec.clear();
}

MyTask::~MyTask()
{
    if(mDb8Sql)
    {
        delete mDb8Sql;
        mDb8Sql = nullptr;
    }
}

bool MyTask::init()
{
    bool ret = false;
    const char* sqlcmd = "create table if not exists tbldatas("
                            "elems text not null, result int not null default 0,"
                            "UNIQUE(elems, result));";
    mSaveVec6RetDb = SAVE6RET_DB_FILEDIR;
    mSaveVec8RetDb = SAVE8RET_DB_FILEDIR;
    for(const auto &elem:mVecBingo)
    {
        if(0 == elem)
        {
            printf("[%d]the input arg:mVecBingo is error!\r\n", __LINE__);
            return ret;
        }
        mVecBingoToStrs += arrange::NumberToString<int>(elem) + "_";
    }
    mSaveVec6RetDb += mVecBingoToStrs;
    mSaveVec8RetDb += mVecBingoToStrs;
    // printf("mSaveVec8RetDb=%s\r\n", mSaveVec8RetDb.c_str());
    mDb8Sql = new sqlite_tb(mSaveVec8RetDb.c_str());

    if((nullptr == mDb8Sql)||(nullptr == mMyDb))
    {
        return ret;
    }

    // 打开数据库，不存在，创建数据库db
	ret = mDb8Sql->OpenDB();
    // 创建表
	ret &= mDb8Sql->CreateTable(sqlcmd);

    return ret;
}

void MyTask::getComplementVec(const vector<int> inputVec, vector<int> &outputVec)
{
	const vector<int> oriVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
							  11,12,13,14,15,16,17,18,19,20,
							  21,22,23,24,25,26,27,28,29,30,
							  31,32,33 };//

	for(const auto &oElem:oriVec)
	{
		bool isIn = false;
		for(const auto &iElem:inputVec)
		{
			if(oElem == iElem)
			{
				isIn = true;
				break;
			}
		}

		if(false == isIn)
		{
			outputVec.push_back(oElem);
		}
	}

	return;
}

void MyTask::createChild6MembersSql(const vector<int> &elemVec)
{
	arrange* arrg = nullptr;
	const char* dbName = mSaveVec6RetDb.c_str();
	arrg = new arrange(dbName, elemVec, 6);
	if(nullptr == arrg)
	{
		printf("arrg new false.\r\n");
		return;
	}

	if(false == arrg->init())
	{
        delete arrg;
	    arrg = nullptr;
		printf("arrange.init false.\r\n");
		return;
	}
    arrg->sqlRecursion(0, 0);

	delete arrg;
	arrg = nullptr;
	return;
}

bool MyTask::findVec8RetFromMyDb(const vector<int> vec8, int &ret)
{
    std::unique_lock<std::mutex> munique(mLock);
    bool retBool = false;
    const vector<int> bluebingo{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    retBool = mMyDb->SelectData(vec8, bluebingo, ret);
    return retBool;
}

void MyTask::updateVec8RetMap(const vector<int> &vecBingo, const int &retInt)
{
    std::unique_lock<std::mutex> munique(mLock);
    mSave8RetMap.insert(std::make_pair(vecBingo, retInt));
    return;
}


bool MyTask::genChild8MembersDb(const vector<int> vec8)
{
    bool retBool = false;
    int retInt = 0;
    string insertData = "(\"";
    if((nullptr == mDb8Sql)||(nullptr == mMyDb))
    {
        return retBool;
    }

    retBool = findVec8RetFromMyDb(vec8, retInt);
    if(false == retBool)
    {
        printf("error:findVec8RetFromMyDb failed.\r\n");
        return retBool;
    }
    mTotalBingo += retInt;

    for(const auto &elem:vec8)
    {
        if(0 == elem)
        {
            printf("[%d]the input arg:mVecBingo is error!\r\n", __LINE__);
            return false;
        }
        insertData += arrange::NumberToString<int>(elem) + "_";
    }

    insertData += "\"," + arrange::NumberToString<int>(retInt) + ");";
    // printf("insertData=%s\r\n", insertData.c_str());
    retBool = mDb8Sql->InsertData(insertData);

    return retBool;
}

void MyTask::createChild8MembersSql()
{
    string insertData = "(\"";
	vector<int> oriVec;
	vector<int> resVec(2, 0);
	//1,找出补集
	getComplementVec(mVecBingo, oriVec);

	arrange::recursion(oriVec, resVec, 0, 0, static_cast<int>(resVec.size()), mVec8Vec, mVecBingo);

	for(const auto &vec8:mVec8Vec)
	{
        createChild6MembersSql(vec8);
        genChild8MembersDb(vec8);
	}

    insertData += mVecBingoToStrs + "\"," + arrange::NumberToString<int>(mTotalBingo) + ");";
    printf("insertData=%s\r\n", insertData.c_str());
    mDb8Sql->InsertData(insertData);
    updateVec8RetMap(mVecBingo, mTotalBingo);

	return;
}

void MyTask::run()
{
    if(false != init())
    {
        createChild8MembersSql();
    }
    return;
}

