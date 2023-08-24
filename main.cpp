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
#include "pailie5.hpp"
#include "pailie3.hpp"
#include "qixingcai.hpp"
#include "daletou.hpp"
#include "shuangseqiu.hpp"

using namespace std;

void play_test();

int main(int argc, char** argv)
{
	sample_pailie3_main(argc, argv);
	// play_test();
	return 0;
}

bool isLittleStyle(const char &a)
{
	return (a>='a' && a<='z');
}

static bool testSort(const char &a, const char &b)
{
	char litt_a = a;
	char litt_b = b;
	if(false == isLittleStyle(a))
	{
		litt_a = a - ('A'-'a');
	}

	if(false == isLittleStyle(b))
	{
		litt_b = b - ('A'-'a');
	}

	return (litt_a < litt_b);
}


int strToNum(const string &str)
{
	int ret = 0;
	istringstream iss(str);
	iss >> ret;
	return ret;
}

void test1(const char* str, int num)
{
	int dis = 0;
	int index = 0;
	while(*str != '\0')
	{
		string tmp = "";
		int ret = 0;
		int val = 0;

		while(*str == ' ')
		{
			str++;
		}
		if(*str == '\0')
		{
			break;
		}

		tmp = str;
		tmp = tmp.substr(0, tmp.find(" "));
		ret = strToNum(tmp);

		val = num - ret;
		if(val == 0)
		{
			index = ret;
			break;
		}
		else if(val > 0)
		{
			if(dis == 0)
			{
				dis = val;
			}
			else
			{
				if(dis > val)
				{
					dis = val;
					index = ret;
				}
			}
		}

		str += static_cast<int>(tmp.size()) + 1;
		tmp.clear();
		tmp = "";
	}

	printf("最终结果为%d\r\n", index);
}

void play_test()
{
	const char *str = "DabBcD";
	vector<char> vecChars = vector<char>();
	while(*str != '\0')
	{
		vecChars.push_back(*str);
		str++;
	}
	sort(vecChars.begin(), vecChars.end(), testSort);

	for(const auto &elem : vecChars)
	{
		printf("%c\r\n", elem);
	}

	vecChars.clear();
	vector<char>().swap(vecChars);

	string input = "545 21 4 5 9   10   16  87 54 3";
	int num = 60;
	test1(input.c_str(), num);
	input.clear();
	input = "";

	return;
}







