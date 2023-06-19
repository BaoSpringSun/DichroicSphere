#include "task2.hpp"
#include "arrange.hpp"

std::mutex MyTask2::mLock;
vector<vector<int>> MyTask2::mVec6VecAll = vector<vector<int>>();
vector<vector<int>> MyTask2::mVec6VecUseFull = vector<vector<int>>();
vector<vector<int>> MyTask2::mVec6VecUseLess = vector<vector<int>>();

MyTask2::MyTask2(const vector<int> &Vec, const unsigned int &index)
    :mVec6(Vec)
    ,mVec6Vec(vector<vector<int>>())
    ,mOperaIndex(index)
{

}

MyTask2::MyTask2()
    :mVec6(vector<int>())
    ,mVec6Vec(vector<vector<int>>())
    ,mOperaIndex(0)
{

}

MyTask2::~MyTask2()
{

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
	printf("[\t %s \t]\r\n", dataStr.c_str());
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
    for(const auto &elemVec : mVec6Vec)
    {
        std::unique_lock<std::mutex> munique(mLock);
        unsigned long dataInVec6VecAllSize = mVec6VecAll.size();
        vector<vector<int>>::iterator itor = std::find(mVec6VecAll.begin(), mVec6VecAll.end(), elemVec);
        if(itor != mVec6VecAll.end())
        {//找到就删除
            mVec6VecAll.erase(itor);
        }
        if((dataInVec6VecAllSize - mVec6VecAll.size()) > 84)//算有变化
        {
            mVec6VecUseFull.push_back(elemVec);
            printf("mVec6VecAll.size=%ld\r\n", mVec6VecAll.size());
        }
        else
        {
            mVec6VecUseLess.push_back(elemVec);
        }

        if((mVec6VecAll.size() < 1000)&&(mVec6VecAll.size() >= 50))
        {
            printf("\tindex=%d\r\n", mOperaIndex);
        }
        else if((mVec6VecAll.size() < 50)&&(mVec6VecAll.size() >= 0))
        {
            printf("\tindex=%d\r\n", mOperaIndex);
            break;
        }
    }
    return;
}

bool MyTask2::init()
{
    return getVec6VecFromDb();
}