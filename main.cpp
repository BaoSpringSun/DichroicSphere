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


int main(int argc, char** argv)
{
	sample_qixingcai_main(argc, argv);
	return 0;
}