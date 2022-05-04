//变量集合X = {(i,j) 第i个工人第j天工作与否}
//值域集合D = {0:不工作，1:工作}
//约束集合C = {
// 1.每个工人每周必须休息2天或2天以上 => 任意a,(a,j) 7个里面至少有2个0
// 2.每个工人每周不可以连续休息3天(不考虑跨周情况) => 任意a,不能出现连续三个(a,j)为0
// 3.需要每天安排至少 k 个人值班 => 任意j,至少k个a满足(a,j)=1
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
int DFS(int a, int i, int n, int k, int (&arrange)[NMAX][NMAX], int worker[NMAX], int conflict[NMAX][NMAX]);
int main()
{
    // INPUT worker情况
    cout << "输入工人数和职位:";
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
    for (int i = 0; i < num_co; i++)
    {
        int a, b;
        cin >> a;
        cin >> b;
        conflict[a - 1][b - 1] = conflict[b - 1][a - 1] = 1;
    }

    //安排情况
    int arrange[NMAX][NMAX] = {0};
    DFS(0, 0, n, k, arrange, worker, conflict);

    for (int j = 0; j < 7; j++)
        for (int i = 0; i < n; i++)
        {
            if (arrange[i][j] == 1)
                cout << i + 1 << " ";
            if (i == n - 1)
                cout << endl;
        }
    return 0;
}

//深度优先搜索
int DFS(int a, int i, int n, int k, int (&arrange)[NMAX][NMAX], int worker[NMAX], int conflict[NMAX][NMAX])
//为第a个worker  安排第i天值班情况
{
    int one = 1;  // one 可取值
    int zero = 1; // zero可取值
    //前向检测1:每个工人每周必须休息2天或2天以上
    if (i >= 5)
    {
        int cnt = 0;
        for (int j = 0; j < i; j++)
        {
            if (arrange[a][j] == 1)
                cnt++;
        }
        if (cnt == 5)
            one = 0;
    }
    //前向检测2:每个工人每周不可以连续休息3天
    if (i >= 3)
    {
        if (!(arrange[a][i - 3] || arrange[a][i - 2] || arrange[a][i - 1]))
            zero = 0;
    }
    if (!(one || zero))
        return 0;

    //前向检测3:需要每天安排至少 k 个人值班
    if (a >= n - k && zero == 1)
    {
        int cnt = 0;
        for (int j = 0; j < a; j++)
        {
            if (arrange[j][i] == 1)
                cnt++;
        }
        if (cnt + n - a == k)
            zero = 0;
    }
    if (!(one || zero))
        return 0;

    //前向检测4:每天至少要有一名级别为 senior 的工人值班
    if (zero == 1)
    {
        int flag = 1;
        for (int j = a + 1; j < n; j++)
        {
            if (worker[j] == 0)
            {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            int flag2 = 1;
            for (int j = 0; j < a; j++)
            {
                if (worker[j] == 0 && arrange[j][i] == 1)
                {
                    flag2 = 0;
                    break;
                }
            }
            if (flag2)
                zero = 0;
        }
    }
    if (!(one || zero))
        return 0;

    //前向检测5:工人冲突
    if (one == 1)
    {
        for (int j = 0; j < a; j++)
        {
            if (arrange[j][i] == 1 && conflict[j][i] == 1)
            {
                one = 0;
                break;
            }
        }
    }
    if (!(one || zero))
        return 0;

    if (one)
    {
        int re1;
        arrange[a][i] = 1;
        if (a == n - 1 && i == 6)
            return 1;
        else if (i == 6)
            re1 = DFS(a + 1, 0, n, k, arrange, worker, conflict);
        else
            re1 = DFS(a, i + 1, n, k, arrange, worker, conflict);
        if (re1 == 1)
            return 1;
    }
    if (zero)
    {
        int re2;
        arrange[a][i] = 0;
        if (a == n - 1 && i == 6)
            return 1;
        else if (i == 6)
            re2 = DFS(a + 1, 0, n, k, arrange, worker, conflict);
        else
            re2 = DFS(a, i + 1, n, k, arrange, worker, conflict);
        if (re2 == 1)
            return 1;
    }
    return 0;
}
