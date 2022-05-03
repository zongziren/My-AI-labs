//变量集合X = {(i,j) 第i个工人第j天工作与否}
//值域集合D = {0:不工作，1:工作}
//约束集合C = {
// 1.每个工人每周必须休息2天或2天以上 => 任意a,(a,j) 7个里面至少有2个0
// 2.每个工人每周不可以连续休息3天(不考虑跨周情况) => 任意a,不能出现连续三个(a,j)为0
// 3.需要每天安排至少 n 个人值班 => 任意j,至少n个a满足(a,j)=1
// 4.每天至少要有一名级别为 senior 的工人值班 =>  任意j,至少1个a满足(a,j)=1 且 a是senior
// 5.工人a不想与工人b同一天工作=>(a,j)和(b,j)不能同时为1
// }
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <chrono>

using namespace std;
#define NMAX 200 //最大工人数
int main()
{
    // INPUT worker情况
    cout << "输入工人数和职位：";
    int n;            //工人数
    int worker[NMAX]; // 0:senior 1:junior
    int sen_cnt = 0, jun_cnt = 0;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> worker[i];
        if (worker[i] == 0)
            sen_cnt++;
        else
            jun_cnt++;
    }

    // INPUT 每天多少人值班
    int k;
    cout << "输入多少人值班：";
    cin >> k;

    // INPUT 矛盾情况
    int num_co;
    cout << "输入多少矛盾组：";
    cin >> num_co;
    int conflict[NMAX][NMAX] = {0};
    for (int i = 0; i < n; i++)
    {
        int a, b;
        conflict[a][b] = conflict[b][a] = 1;
    }
}