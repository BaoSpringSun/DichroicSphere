#include "task2.hpp"
#include "arrange.hpp"

std::mutex MyTask2::mLock;
multiset<vector<int>> MyTask2::mVec6VecAll = multiset<vector<int>>();
map<vector<int>, int> MyTask2::mMapVec6TotalRet = map<vector<int>, int>();

MyTask2::MyTask2(const vector<int> &Vec, const unsigned int &index)
    :mVec6VecObjDb(nullptr)
    ,mVec6(Vec)
    ,mVec6Vec(vector<vector<int>>())
    ,mMapVec6Ret(map<vector<int>, int>())
    ,mOperateIndex(index)
    ,mTotalMapSecond(0)
{

}

MyTask2::MyTask2()
    :mVec6VecObjDb(nullptr)
    ,mVec6(vector<int>())
    ,mVec6Vec(vector<vector<int>>())
    ,mMapVec6Ret(map<vector<int>, int>())
    ,mOperateIndex(0)
    ,mTotalMapSecond(0)
{

}

MyTask2::~MyTask2()
{
    if(mVec6VecObjDb)
    {
        delete mVec6VecObjDb;
        mVec6VecObjDb = nullptr;
    }
    freeResource<vector<int>>(mVec6);
    freeResource<vector<vector<int>>>(mVec6Vec);
    freeResource<map<vector<int>, int>>(mMapVec6Ret);
}

bool MyTask2::getVec6VecFromDb()
{
    bool ret = false;
    sqlite_tb *sql = nullptr;
	string sqlDbFile = SAVE6RET_DB_FILEDIR;
	string dataStr = "";

	for(const auto &elem : mVec6)
	{
		dataStr += arrange::NumberToString<int>(elem) + "_";
	}
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
	ret = sql->SelectAllData(mVec6Vec);
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
	return ret;
}

void MyTask2::run()
{
    if(false == init())
        return;
    for(const auto &elemVec:mVec6Vec)
    {
        mMapVec6Ret.emplace(elemVec, mVec6VecAll.count(elemVec));
    }
    insertDatas();
    updateMapVec6TotalRet();
    return;
}

bool MyTask2::initVec6VecObjDb()
{
    bool ret = false;
    const char* sqlcmd = "create table if not exists tbldatas("
                            "elems text not null, result int not null default 0,"
                            "UNIQUE(elems, result));";
    string ObjectDirPath = SAVE6RET_OCCURETIME_DB_FILEDIR;
    for(const auto &elem:mVec6)
    {
        if(0 == elem)
        {
            printf("[%d]the input arg:mVec6 is error!\r\n", __LINE__);
            return ret;
        }
        ObjectDirPath += arrange::NumberToString<int>(elem) + "_";
    }
    // printf("ObjectDirPath=%s\r\n", ObjectDirPath.c_str());
    mVec6VecObjDb = new sqlite_tb(ObjectDirPath.c_str());

    if(nullptr == mVec6VecObjDb)
    {
        return ret;
    }

    // 打开数据库，不存在，创建数据库db
	ret = mVec6VecObjDb->OpenDB();
    // 创建表
	ret &= mVec6VecObjDb->CreateTable(sqlcmd);

    return ret;
}

bool MyTask2::init()
{
    bool ret = false;
    ret = getVec6VecFromDb();
    ret &= initVec6VecObjDb();
    return ret;
}


void MyTask2::insertDatas()
{
    unsigned int insertIndex = 0;
    string datas = "";
    // printf("mMapVec6Ret.size=%ld\r\n", mMapVec6Ret.size());
	if(mMapVec6Ret.size() <= 0)
	{
		printf("mMapVec6Ret.size()=0~\r\n");
		return;
	}

	for(const auto &mapElem:mMapVec6Ret)
	{
		string elemDatas = "(\"";
		for(const auto &elem:mapElem.first)
		{
			elemDatas += arrange::NumberToString<int>(elem) + "_";
		}
        elemDatas = elemDatas + "\"," + arrange::NumberToString<int>(mapElem.second);
		elemDatas = elemDatas + ")";
        mTotalMapSecond += mapElem.second;

		if(insertIndex < ONCE_INSERT_MAX_ITERM)
        {
            insertIndex++;
            datas = datas + elemDatas + ",";
        }
        else
        {
            datas = datas + elemDatas + ";";
            mVec6VecObjDb->InsertData(datas);
            insertIndex = 0;
            datas = "";
        }
	}

	if(false == datas.empty())
    {
        datas = datas.substr(0, datas.length()-1) + ";";//去掉“,” 加“;”
        mVec6VecObjDb->InsertData(datas);
        insertIndex = 0;
        datas = "";
    }

    datas = "(\"";
    for(const auto &elem:mVec6)
    {
        datas += arrange::NumberToString<int>(elem) + "_";
    }
    datas = datas + "\"," + arrange::NumberToString<int>(mTotalMapSecond);
    datas = datas + ");";
    mVec6VecObjDb->InsertData(datas);
    printf("[\t %s --%d\t]\r\n", datas.c_str(), mOperateIndex);
    datas = "";
}


void MyTask2::updateMapVec6TotalRet()
{
    std::unique_lock<std::mutex> munique(mLock);
    mMapVec6TotalRet.emplace(mVec6, mTotalMapSecond);
    return;
}
