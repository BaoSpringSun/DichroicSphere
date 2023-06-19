
#pragma once
#include <vector>
#include <unistd.h>
#include <sys/time.h>
#include <atomic>
#include "sqlitetbl.hpp"

using namespace std;
/**
 * 比 long long 更大的数据类型:__int128
 *
 * 科学计数法的输出格式为：%e
 * Ps:不可以用int，因为精度不够， float 和 double 皆可
*/

class arrange
{
public:
    explicit arrange(const char* dbFile, const vector<int> oriVec, const int size);
    ~arrange();

    bool init();

    void sample_arrange_main();
    void sqlRecursion(int start, int index);
    static void startTime();
    static void endTime();
    static void recursion( const vector<int> &oriVec,
                            vector<int> resVec,
                            int start, int index, int remain,
                            vector<vector<int>> &resVecVec, const vector<int> &constVec);

    /**
     * 或者用string中的to_string接口可以将int转化为string；
    */
template <class T>
    static string NumberToString(T value);
template <class T>
    static T stringToNumber(const string str);

private:
    /**
     * 从一个含有m个元素的数组中选出n个元素的组合；
    */
    void recursionDemo( int *oriArr, int oriArrLen,
                    int *resArr, int resArrLen,
                    int start, int index, int remain);

    void recursionDemo( const vector<int> oriVec, vector<int> resVec,
                    int start, int index, int remain);

    bool recursion(int start, int index, int remain);

    void sample_vec_main();
    void sample_arr_main();
    void sample_combination_main();

    /**
     * 从m个数中选择n个数的排列和组合，阶乘m！和n！
    */
    __int128 factorial(int n);
    __int128 Arrangement(int m, int n);
    __int128 Combination(int m, int n);
    void print(__int128 x);
    void printf__int128(__int128 x);

private:
    string mDatas;
    sqlite_tb *mSql;
    const vector<int> mOriVec;
    vector<int> mResVec;
    int mResSize;
    int mInsertIndex;

public:
    static struct timeval mTv;
    static bool mTimeOnOff;
    static atomic<bool> mUsed;
};
