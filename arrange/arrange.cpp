#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include<sstream>
#include "arrange.hpp"

using namespace std;
/**
 * 比 long long 更大的数据类型:__int128
 *
 * 科学计数法的输出格式为：%e
 * Ps:不可以用int，因为精度不够， float 和 double 皆可
*/


/**
 * 从m个数中选择n个数的排列和组合，阶乘m！和n！
*/

__int128 factorial(int n);
__int128 Arrangement(int m, int n);
__int128 Combination(int m, int n);

void print(__int128 x)
{
	if(x<0){
        putchar('-');
        x=-x;
    }
    if(x>9)
        print(x/10);

    putchar(x%10+'0');
}

void printf__int128(__int128 x)
{
    print(x);
    printf("\r\n");
    return;
}

int sample_combination_main()
{
	int m=33, n=6;

	// printf("n的阶乘为:%lld; m的阶乘为:%lld; m和n的排列为:%ld; m和n的组合为:%ld.\r\n",
    //         factorial(n), factorial(m), Arrangement(m, n), Combination(m, n));
    printf("m的阶乘为:");
    printf__int128(factorial(m));
    printf("n的阶乘为:");
    printf__int128(factorial(n));
    printf("m和n的排列为:");
    printf__int128(Arrangement(m, n));
    printf("m和n的组合为:");
    printf__int128(Combination(m, n));
	return 0;
}

/**
 * 阶乘
*/
__int128 factorial(int n)
{
    __int128 ret = 0;
    if(n == 1)
    {
        return (ret = 1);
    }
    else
    {
        ret = factorial(n - 1);
        return (n * ret);
    }
}

/**
 * 排列
*/
__int128 Arrangement(int m, int n)
{
	return (factorial(m) / factorial(m-n));
}

/**
 * 组合
*/
__int128 Combination(int m, int n)
{
	return (factorial(m) / (factorial(n) * factorial(m-n)));
}


/**
 * 从一个含有m个元素的数组中选出n个元素的组合；
*/

void sample_arr_main();
void sample_vec_main();


void sample_arrange_main()
{
    sample_arr_main();
    // sample_vec_main();
    sample_combination_main();
    return;
}

void sample_arr_main()
{
    int max = 0, num = 3;
    int arr[] = {32,31,29,27,15,12,8,6,3};
    int res[num] = {0};

    max = sizeof(arr)/sizeof(int);
    //组合数据
    recursion(arr, max, res, num, 0, 0, num);
    printf("the total combination number=");
    printf__int128(Combination(max, num));
    return;
}

void sample_vec_main()
{
    const vector<int> oriVec{32,31,29,27,15,12,8,6,3};
    vector<int> resVec(3, 0);
    recursion(oriVec, resVec, 0, 0, resVec.size());
    printf("the total combination number=");
    printf__int128(Combination(oriVec.size(), resVec.size()));
    return;
}


/**
 * oriVec 含有m个元素的待组合的原始数据容器
 * resVec 保存组合结果的容器---resVecr是oriVec的子集
 * start 当前递归开始位置
 * index 当前要写入结果的key
 * remain 当前尚需组合(还未参与组合/剩下)的元素个数
 *
 * 容器写法
*/
void recursion( const vector<int> oriVec,
                vector<int> resVec,
                int start, int index, int remain)
{
    if (remain == 0)
    {
        //当待填充的个数为0时，表示结束
        for(const auto &elem : resVec)
        {
            printf("%d\t", elem);
        }
        printf("\n");
    }
    else
    {
        for(int i=start; i<static_cast<int>(oriVec.size()); i++)
        {
            //针对当前index, 遍历剩余可用的数填充递归
            resVec[index] = oriVec[i];
            recursion(oriVec, resVec, i+1, index+1, remain-1);
        }
    }
}



string NumberToString(int x)
{
	stringstream ss;
	ss<<x;
	return ss.str();
}


void recursion( sqlite_tb *sql,
                const vector<int> oriVec,
                vector<int> resVec,
                int start, int index, int remain)
{
    if (remain == 0)
    {
        static int k = 0;
        if(k < 100)
        {
            k++;

        }
        else
        {
            k = 0;
        }

        string data = "(";
        //当待填充的个数为0时，表示结束
        for(const auto &elem : resVec)
        {
            // printf("%d\t", elem);
            data += NumberToString(elem) + ",";
        }
        data = data.substr(0, data.length()-1);//去掉“,”
        data = data + ");";
        // printf("\n");
        // printf("datas = %s\r\n", data.c_str());
        sql->InsertData(data);
    }
    else
    {
        for(int i=start; i<static_cast<int>(oriVec.size()); i++)
        {
            //针对当前index, 遍历剩余可用的数填充递归
            resVec[index] = oriVec[i];
            recursion(sql, oriVec, resVec, i+1, index+1, remain-1);
        }
    }
}


/**
 * oriArr 含有m个元素的待组合的原始数据
 * oriArrLen 待组合的原始数据所含的元素个数m
 * resArr 保存组合结果的数组---resArr是oriArr的子集
 * resArrLen 保存组合结果的数组的元素个数，方便输出
 * start 当前递归开始位置
 * index 当前要写入结果的key
 * remain 当前尚需组合(还未参与组合/剩下)的元素个数
 *
 * 数组写法
 */
void recursion( int *oriArr, int oriArrLen,
                int *resArr, int resArrLen,
                int start, int index, int remain)
{
    if (remain == 0)
    {
        //当待填充的个数为0时，表示结束
        for(int i=0; i<resArrLen; i++)
        {
            printf("%d\t", resArr[i]);
        }
        printf("\n");
    }
    else
    {
        for(int i=start; i<oriArrLen; i++)
        {
            //针对当前index, 遍历剩余可用的数填充递归
            resArr[index] = oriArr[i];
            recursion(oriArr, oriArrLen, resArr, resArrLen, i+1, index+1, remain-1);
        }
    }
}



