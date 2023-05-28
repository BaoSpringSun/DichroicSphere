
#pragma once
#include <vector>
#include "sqlitetbl.hpp"

using namespace std;

void sample_arrange_main();

void recursion( int *oriArr, int oriArrLen,
                int *resArr, int resArrLen,
                int start, int index, int remain);

void recursion( const vector<int> oriVec, vector<int> resVec,
                int start, int index, int remain);

void recursion( sqlite_tb *sql,
                const vector<int> oriVec,
                vector<int> resVec,
                int start, int index, int remain);


