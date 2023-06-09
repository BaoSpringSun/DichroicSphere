#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "sqlitetbl.hpp"
#include <string>
#include <cstring>
#include <vector>
using namespace std;

#define     SOURCE_FILE     "./sourcefile"


sqlite_tb::sqlite_tb()
    :db(NULL)
{
}

sqlite_tb::~sqlite_tb()
{
    if(db)
    {
        sqlite3_close(db); //关闭数据库连接
        db = nullptr;
    }
}

//打开数据库，不存在，创建数据库db
bool sqlite_tb::OpenDB()
{
	int ret = sqlite3_open("./mydb", &db);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL; //定义错误信息
        errMsg = sqlite3_errmsg(db); //获取错误信息
        printf("open error:%s\r\n", errMsg);
        sqlite3_close(db); //关闭数据库连接
		return false;
	}
	printf("open db successful.\r\n");
	return true;
}

bool sqlite_tb::CloseDB()
{
    if(db)
    {
        sqlite3_close(db); //关闭数据库连接
        db = nullptr;
    }
    return true;
}

//创建数据库表
bool sqlite_tb::CreateTable()
{
    char *zerrMsg = NULL;
    const char* sqlcmd = "create table if not exists tbldatas(date int PRIMARY key not null default 2000,"
                            "red1 int not null default 1,red2 int not null default 1,red3 int not null default 1,"
                            "red4 int not null default 1,red5 int not null default 1,red6 int not null default 1,"
                            "blue1 int not null default 1);";
	int ret = sqlite3_exec(db, sqlcmd, 0, 0, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("create error:%s\r\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("create error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}
	printf("create success\n");
	return true;
}

bool sqlite_tb::InsertData()
{
    unsigned int k = 0;
    FILE* pfile = NULL;
    char buf[128] = {0};
    string datas = "";
    pfile = fopen(SOURCE_FILE, "r");
    if(NULL == pfile)
    {
        printf("fopen error\n");
        return false;
    }

    while(fgets(buf, sizeof(buf), pfile) != NULL)
    {
        if(k < 100)
        {
            k++;
            datas = datas + buf + ",";
        }
        else
        {
            k = 0;
            datas = datas + buf + ";";
            if(false == InsertData(datas))
            {
                printf("insert data error:%s\r\n", buf);
            }
            datas = "";
        }
        memset(buf, 0, sizeof(buf));
    }

    if(false == datas.empty())
    {
        if(false == InsertData(datas.substr(0, datas.length()-1)))//去掉“,”
        {
            printf("insert data error:%s\r\n", buf);
        }
        datas = "";
    }

    fclose(pfile);
    return true;
}

//插入数据
bool sqlite_tb::InsertData(const string &data)
{
    char *zerrMsg = NULL;
    /**
     * 使用insert into插入重复数据时，数据库会报错，但是使用insert or ignore into数据库就不会报错了。
     * insert or replace into table_name：是每次执行时，如果不存在，则添加，如果存在，则更新。
     * insert or ignore into table_name：是每次执行时，如果不存在，则添加，如果存在，则不操作。
    */
    string sqlcmd = "INSERT OR IGNORE INTO tbldatas VALUES " + data;

    int ret = sqlite3_exec(db, sqlcmd.c_str(), 0, 0, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("insert error:%s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("insert error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}
	// printf("insert success\n");
	return true;
}

//删除
bool sqlite_tb::DeleteData(unsigned int date)
{
    char *zerrMsg = NULL;
    string sqlcmd = "delete from tbldatas where ";

    sqlcmd = sqlcmd + "(date=" +  to_string(date) + ");";
	int ret = sqlite3_exec(db, sqlcmd.c_str(), 0, 0, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("delete error:%s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("delete error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}
	printf("delete success\n");
	return true;
}
//更新
bool sqlite_tb::UpdateData()
{
    char *zerrMsg = NULL;
	int ret = sqlite3_exec(db, "update tbldatas set red1=25 where date=2003001", 0, 0, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("update error:%s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("update error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}
	printf("update success\n");
	return true;
}

void sqlite_tb::GenRedCondition(const vector<uint8> &vec, string &cond)
{
    for(int i=1;i<7;i++)
    {
        cond = cond + "(";
        for(const auto &elem:vec)
        {
            cond = cond + "red" + to_string(i) +"=" + to_string(elem) + " or ";
        }
        cond = cond.substr(0, cond.length()-4) + ") and ";
    }
}


void sqlite_tb::GenBlueCondition(const vector<uint8> &vec, string &cond)
{
    cond = cond + "(";
    for(const auto &elem:vec)
    {
        cond = cond + "blue1=" + to_string(elem) + " or ";
    }
    cond = cond.substr(0, cond.length()-4) + ");"; //-4 是为了去掉"_or_"
}

//查找<去重复，筛选出所有都是唯一的数据> 关键字：distinct
bool sqlite_tb::SelectUniqueData()
{
    char *zerrMsg = NULL;
	int nrow = 0, ncolumn = 0;
    // int i = 0, j = 0;
	char** db_result = NULL;
    string sqlcmd = "select distinct red1,red2,red3,red4,red5,red6,blue1 from tbldatas;";

	int ret = sqlite3_get_table(db, sqlcmd.c_str(), &db_result, &nrow, &ncolumn, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("select error: %s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("select error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}

	// for (i = 0; i < (nrow + 1)*ncolumn; i += ncolumn)
	// {
	// 	for (j = 0; j < ncolumn; j++)
	// 	{
	// 		printf("%s\t", db_result[i + j]);
	// 	}
	// 	printf("\r\n");
	// }
    sqlite3_free_table(db_result);
    printf("[%s]--nrow = %d\r\n",__FUNCTION__, nrow);
    db_result = NULL;
	return true;
}

//查找所有重复的行大于等于2的所有行数据
bool sqlite_tb::SelectRepeatData()
{
    char *zerrMsg = NULL;
	int nrow = 0, ncolumn = 0;
    // int i = 0, j = 0;
	char** db_result = NULL;
    const char* sqlcmd = "select * from tbldatas "
                         "where (red1,red2,red3,red4,red5,red6,blue1) "
                         "in (select red1,red2,red3,red4,red5,red6,blue1 "
                         "from tbldatas group by red1,red2,red3,red4,red5,red6,blue1 having count(*) >= 2);";

	int ret = sqlite3_get_table(db, sqlcmd, &db_result, &nrow, &ncolumn, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("select error: %s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("select error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}

    int i, j;
    printf("[%s]--nrow = %d\r\n",__FUNCTION__, nrow);
	for (i = 0; i < (nrow + 1)*ncolumn; i += ncolumn)
	{
		for (j = 0; j < ncolumn; j++)
		{
			printf("%s\t", db_result[i + j]);
		}
		printf("\r\n");
	}
    sqlite3_free_table(db_result);

    db_result = NULL;
	return true;
}

//查找所有数据
bool sqlite_tb::SelectAllData()
{
    char *zerrMsg = NULL;
	int nrow = 0, ncolumn = 0;
	char** db_result = NULL;
    const char* sqlcmd = "select * from tbldatas;";

	int ret = sqlite3_get_table(db, sqlcmd, &db_result, &nrow, &ncolumn, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("select error: %s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("select error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}

    printf("[%s]--nrow = %d\r\n",__FUNCTION__, nrow);
    // int i = 0, j = 0;
	// for (i = 0; i < (nrow + 1)*ncolumn; i += ncolumn)
	// {
	// 	for (j = 0; j < ncolumn; j++)
	// 	{
	// 		printf("%s\t", db_result[i + j]);
	// 	}
	// 	printf("\r\n");
	// }
    sqlite3_free_table(db_result);

    db_result = NULL;
	return true;
}

//获取总条目数
bool sqlite_tb::SelectGetTotalRows()
{
    char *zerrMsg = NULL;
	int nrow = 0, ncolumn = 0;
	char** db_result = NULL;
    const char* sqlcmd = "select count(*) from tbldatas;";


	int ret = sqlite3_get_table(db, sqlcmd, &db_result, &nrow, &ncolumn, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("select error: %s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("select error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}

    int i = 0, j = 0;
    printf("[%s]\r\n",__FUNCTION__);
	for (i = 0; i < (nrow + 1)*ncolumn; i += ncolumn)
	{
		for (j = 0; j < ncolumn; j++)
		{
			printf("\t%s\t", db_result[i + j]);
		}
		printf("\r\n");
	}
    sqlite3_free_table(db_result);

    db_result = NULL;
	return true;
}

//获取统计字段linename列均不一样的数字有哪些，具体列出来
/**
 * linename可以是多个字段，中间用“，”逗号隔开；
*/
bool sqlite_tb::SelectDistinctDataByLineName(const char *linename)
{
    char *zerrMsg = NULL;
	int nrow = 0, ncolumn = 0;
	char** db_result = NULL;
    string sqlcmd = "select distinct ";

    sqlcmd = sqlcmd + linename + " from tbldatas;";

	int ret = sqlite3_get_table(db, sqlcmd.c_str(), &db_result, &nrow, &ncolumn, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("select error: %s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("select error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}

    printf("[%s]%s--nrow = %d\r\n",__FUNCTION__, linename, nrow);
    // int i = 0, j = 0;
	// for (i = 0; i < (nrow + 1)*ncolumn; i += ncolumn)
	// {
	// 	for (j = 0; j < ncolumn; j++)
	// 	{
	// 		printf("\t%s\t", db_result[i + j]);
	// 	}
	// 	printf("\r\n");
	// }
    sqlite3_free_table(db_result);

    db_result = NULL;
	return true;
}

//获取统计字段linename列均不一样的数字有多少个，因为得到的是count()返回的结果，是一个值；
//不会返回具体是哪些数据；具体列出是哪些数据的接口也有封装；
/**
 * linename只能是一个字段，不可以是多个字段
*/
bool sqlite_tb::SelectDistinctDataAmountByLineName(const char *linename)
{
    char *zerrMsg = NULL;
	int nrow = 0, ncolumn = 0;
	char** db_result = NULL;
    string sqlcmd = "select count(distinct ";

    sqlcmd = sqlcmd + linename + ") from tbldatas;";

	int ret = sqlite3_get_table(db, sqlcmd.c_str(), &db_result, &nrow, &ncolumn, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("select error: %s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("select error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}

    int i = 0, j = 0;
    printf("[%s]%s\r\n", __FUNCTION__, linename);
	for (i = 0; i < (nrow + 1)*ncolumn; i += ncolumn)
	{
		for (j = 0; j < ncolumn; j++)
		{
			printf("\t%s\t", db_result[i + j]);
		}
		printf("\r\n");
	}
    sqlite3_free_table(db_result);

    db_result = NULL;
	return true;
}


//查询<按条件查找>
bool sqlite_tb::SelectData(const vector<uint8> &vred, const vector<uint8> &vblue, uint32 &retcount)
{
    char *zerrMsg = NULL;
	int nrow = 0, ncolumn = 0;
	char** db_result = NULL;
    string sqlcmd = "select * from tbldatas where ";
    string condition = "";
    retcount = 0;

    GenRedCondition(vred, condition);
    GenBlueCondition(vblue, condition);

    sqlcmd = sqlcmd + condition;
	int ret = sqlite3_get_table(db, sqlcmd.c_str(), &db_result, &nrow, &ncolumn, &zerrMsg);
	if (ret != SQLITE_OK)
	{
        const char *errMsg = NULL;
        printf("select error: %s\n", zerrMsg);
        sqlite3_free(zerrMsg);
        zerrMsg = NULL;
        errMsg = sqlite3_errmsg(db);
        printf("select error:%s\r\n", errMsg);
		sqlite3_close(db);
		return false;
	}

    // printf("[%s]by condition --nrow = %d\r\n",__FUNCTION__, nrow);
	// int i, j;
	// for (i = 0; i < (nrow + 1)*ncolumn; i += ncolumn)
	// {
	// 	for (j = 0; j < ncolumn; j++)
	// 	{
	// 		printf("%s\t", db_result[i + j]);
	// 	}
	// 	printf("\n");
	// }
    sqlite3_free_table(db_result);

    retcount = nrow;
    db_result = NULL;
	return true;
}

/*
select count(red1,red2,red3,red4,red5,red6,blue1) from tbldatas;   等同于select count(*) from tbldatas;
也等同于 select count(red1) from tbldatas; 获取总行数；


1，//找出指定列出现重复的行的第一个行的具体数据
select * from tbldatas group by red1,red2,red3,red4,red5,red6,blue1 having count(*) >= 2;
//选出red1和red6字段的列数据相同的行的具体数据；
select red1,red6 from tbldatas group by red1,red2,red3,red4,red5,red6,blue1 having count(*) >= 2;
select red1,red6 from tbldatas group by red1,red6 having count(*) >= 2;
select * from tbldatas where (red1,red6) in (select red1,red6 from tbldatas group by red1,red6 having count(*) >= 2);
select red1,red2,red3,red4,red5,red6,blue1 from tbldatas group by red1,red2,red3,red4,red5,red6,blue1 having count(*) >= 2;


SELECT * FROM tbldatas Where tbldatas.red1 = red1 and red6 = red6;

2，//找出指定列重复的行的所有行的具体数据
select * from tbldatas where (red1,red2,red3,red4,red5,red6,blue1) in (select red1,red2,red3,red4,red5,red6,blue1 from tbldatas group by red1,red2,red3,red4,red5,red6,blue1 having count(*) >= 2);
3，//找出指定列重复的行的所有行的具体数据
select * from tbldatas where (red1,red2,red3) in (select red1,red2,red3 from tbldatas group by red1,red2,red3,red4,red5,red6,blue1 having count(*) >= 2);
4，//找出指定列<red1,red6>重复的行的所有行的具体数据
select * from tbldatas where (red1,red6) in (select red1,red6 from tbldatas group by red1,red2,red3,red4,red5,red6,blue1 having count(*) >= 2);

总结：2，3，4：//找出指定列<字段A列，字段B列，字段C列...>重复的行的所有行的具体数据
格式如下：
select * from 表名 where (【字段A列，字段B列，字段C列】) in (select 【字段A列，字段B列，字段C列】 from tbldatas group by 【表的所有字段的列，除了PRIMARY key的列，且一定不能用括号】 having count(*) >= 2);
                          这里必须需要括号 <=这两处的字段一定要相同=> 这里一定不能用括号



1,2,3,4点，如果red1~red6,blue1字段的列有相同数据的行存在count>=2,则1，2，3，4的公式一定可以找到，
但是如果一个相同的都没有，则1，2，3，4的公式一个都不管用；



选出red1,red2,red3,red4,red5,red6,blue1等字段列的行数据不相同的数据，<去重复，筛选出所有都是唯一的数据>
select distinct red1,red2,red3,red4,red5,red6,blue1 from tbldatas;
select distinct red6 from tbldatas;
select distinct red6 from tbldatas order by red6;


select count(red1) from tbldatas;获取总行数；count()-->count括号里面只能是一个字段的名，不能多个字段，或者是一个“*”，填“*” 则就不能跟distinct关键字一起使用了；
select count(distinct red6) from tbldatas;获取统计字段red6的列的数字不一样的有多少个；

*/
bool sqlite_tb::SelectUniqueDataAmount()
{
    // char *zerrMsg = NULL;
	// int nrow = 0, ncolumn = 0;
	// char** db_result = NULL;
    // // string sqlcmd = "select count(red1,red2,red3,red4,red5,red6,blue1) from tbldatas;";
    // string sqlcmd = "select * from tbldatas group by red1,red6,blue1 having count(*) >= 2;";

	// int ret = sqlite3_get_table(db, sqlcmd.c_str(), &db_result, &nrow, &ncolumn, &zerrMsg);
	// if (ret != SQLITE_OK)
	// {
    //     const char *errMsg = NULL;
    //     printf("select error: %s\n", zerrMsg);
    //     sqlite3_free(zerrMsg);
    //     zerrMsg = NULL;
    //     errMsg = sqlite3_errmsg(db);
    //     printf("select error:%s\r\n", errMsg);
	// 	sqlite3_close(db);
	// 	return false;
	// }
	// int i, j;
	// for (i = 0; i < (nrow + 1)*ncolumn; i += ncolumn)
	// {
	// 	for (j = 0; j < ncolumn; j++)
	// 	{
	// 		printf("%s\t", db_result[i + j]);
	// 	}
	// 	printf("\n");
	// }
    // sqlite3_free_table(db_result);
    // printf("nrow = %d\r\n", nrow);
    // db_result = NULL;
	return true;
}



/**
 * select count(distinct red2) from tbldatas;
 * select distinct red2 from tbldatas order by red2;
 * select * from tbldatas where (red1) in (select red1 from tbldatas group by red1,red2,red3,red4,red5,red6,blue1 having count(*) >= 2);
 *
 *
*/



