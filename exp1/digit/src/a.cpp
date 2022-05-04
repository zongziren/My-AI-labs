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
void A_h1(const int start[5][5], const int target[5][5]);
void A_h2(const int start[5][5], const int target[5][5]);
void IDA_h1(const int start[5][5], const int target[5][5]);
void IDA_h2(const int start[5][5], const int target[5][5]);

typedef struct
{
    //坐标
    int x;
    int y;
    // h值和g值 f=h+g
    int h;
    int g;
    //当前图状态
    int state[5][5];
    //存储到这个状态的操作
    string op;
} Node;

struct cmp
{
    bool operator()(Node a, Node b) { return a.g + a.h > b.g + b.h; }
};

int main(int argc, char *argv[])
{

    // INPUT tatic

    int tatic;
    if (strcmp(argv[1], "A_h1") == 0)
        tatic = 0;
    else if (strcmp(argv[1], "A_h2") == 0)
        tatic = 1;
    else if (strcmp(argv[1], "IDA_h1") == 0)
        tatic = 2;
    else if (strcmp(argv[1], "IDA_h2") == 0)
        tatic = 3;

    // open input file
    fstream input_file;
    fstream target_file;
    string inputname, targetname;
    inputname = argv[2];
    targetname = argv[3];
    inputname = "../data/" + inputname;
    targetname = "../data/" + targetname;
    input_file.open(inputname, ios::in);
    target_file.open(targetname, ios::in);

    //读入start target
    int start[5][5];
    int target[5][5];
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            input_file >> start[i][j];
            target_file >> target[i][j];
        }
    }

    if (tatic == 0)
        A_h1(start, target);
    if (tatic == 1)
        A_h2(start, target);
    if (tatic == 2)
        IDA_h1(start, target);
    if (tatic == 3)
        IDA_h2(start, target);

    return 0;
}

/*
void Judge_push(priority_queue<Node, vector<Node>, cmp> &node_queue, Node new_node)
{
    if (visit[new_node.y][new_node.x] == 0)
    {
        node_queue.push(new_node);
        visit[new_node.y][new_node.x] = 1;
        return;
    }
    vector<Node> tmp;
    Node tmp_node;
    while (!node_queue.empty())
    {
        tmp_node = node_queue.top();
        if (tmp_node.x == new_node.x && tmp_node.y == new_node.y)
        {

            auto f1 = tmp_node.g + tmp_node.h;
            auto f2 = new_node.g + new_node.h;
            if (f1 >= f2)
            {
                node_queue.pop();
                node_queue.push(new_node);
            }

            int flag = 0;
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (new_node.state[j][i] != tmp_node.state[j][i])
                    {
                        node_queue.push(new_node);
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1)
                    break;
            }
            break;
        }
        tmp.push_back(tmp_node);
        node_queue.pop();
    }
    for (auto n = tmp.begin(); n < tmp.end(); n++)
        node_queue.push(*n);
    return;
}
*/

void A_h1(const int start[5][5], const int target[5][5])
{
    auto size = sizeof(int) * 5 * 5;
    fstream A_h1_f;
    A_h1_f.open("../output/output_A_h1.txt", ios::app | ios::in | ios::out);
    auto begin = std::chrono::high_resolution_clock::now();

    // 添加visit对帮助判断当前x，y是否在queue中
    int visit[5][5] = {0};

    //利用优先队列存储点的xy坐标,h,g
    priority_queue<Node, vector<Node>, cmp> node_queue;
    //初始找到为0的点 统计初始点的h值
    int now_x, now_y, now_g = 0;
    int now_h = 0;
    string now_op;
    Node now_node;
    Node new_node;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (start[i][j] == 0)
            {
                now_x = j;
                now_y = i;
                now_g = 0;
            }
            else if (start[i][j] != target[i][j])
                now_h++;
        }
    }
    Node init_node;
    init_node.x = now_x;
    init_node.y = now_y;
    init_node.g = 0;
    init_node.h = now_h;
    memcpy(init_node.state, start, size);
    init_node.op = "";
    node_queue.push(init_node);
    visit[now_x][now_y] = 1;

    //每次从优先队列中出队一个点 探寻周围的点 加入队列
    while (!node_queue.empty())
    {
        now_node = node_queue.top();
        node_queue.pop();
        now_x = now_node.x;
        now_y = now_node.y;
        now_g = now_node.g;
        now_h = now_node.h;
        now_op = now_node.op;
        visit[now_x][now_y] = 0;
        //常规up
        if (now_y != 0)
        {
            if (now_node.state[now_y - 1][now_x] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = now_y - 1;
                new_node.g = now_g + 1;
                new_node.op = now_op + "U";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y - 1][now_x];
                new_node.state[now_y - 1][now_x] = now_node.state[now_y][now_x];

                //计算h
                int a = now_node.state[now_y - 1][now_x] != target[now_y - 1][now_x];
                int b = target[now_y][now_x] != new_node.state[now_y][now_x];
                new_node.h = now_h - a + b;
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道UP
        if (now_y == 0 && now_x == 2)
        {
            if (now_node.state[4][now_x] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = 4;
                new_node.g = now_g + 1;
                new_node.op = now_op + "U";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[4][now_x];
                new_node.state[4][now_x] = now_node.state[now_y][now_x];

                //计算h
                int a = (now_node.state[4][now_x] != target[4][now_x]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]);
                new_node.h = now_h - a + b;
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //常规DOWN
        if (now_y != 4)
        {
            if (now_node.state[now_y + 1][now_x] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = now_y + 1;
                new_node.g = now_g + 1;
                new_node.op = now_op + "D";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y + 1][now_x];
                new_node.state[now_y + 1][now_x] = now_node.state[now_y][now_x];

                //计算h
                int a = (now_node.state[now_y + 1][now_x] != target[now_y + 1][now_x]);
                int b = target[now_y][now_x] != new_node.state[now_y][now_x];
                new_node.h = now_h - a + b;
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道Down
        if (now_y == 4 && now_x == 2)
        {
            if (now_node.state[0][now_x] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = 0;
                new_node.g = now_g + 1;
                new_node.op = now_op + "D";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[0][now_x];
                new_node.state[0][now_x] = now_node.state[now_y][now_x];

                //计算h
                int a = (now_node.state[0][now_x] != target[0][now_x]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]);
                new_node.h = now_h - a + b;
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //常规LEFT
        if (now_x != 0)
        {
            if (now_node.state[now_y][now_x - 1] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x - 1;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "L";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y][now_x - 1];
                new_node.state[now_y][now_x - 1] = now_node.state[now_y][now_x];

                //计算h
                int a = (now_node.state[now_y][now_x - 1] != target[now_y][now_x - 1]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]);
                new_node.h = now_h - a + b;
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道LEFT
        if (now_x == 0 && now_y == 2)
        {
            if (now_node.state[now_y][4] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = 4;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "L";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y][4];
                new_node.state[now_y][4] = now_node.state[now_y][now_x];

                //计算h
                int a = (now_node.state[now_y][4] != target[now_y][4]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]);
                new_node.h = now_h - a + b;
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //常规RIGHT
        if (now_x != 4)
        {
            if (now_node.state[now_y][now_x + 1] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x + 1;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "R";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y][now_x + 1];
                new_node.state[now_y][now_x + 1] = now_node.state[now_y][now_x];

                //计算h
                int a = (now_node.state[now_y][now_x + 1] != target[now_y][now_x + 1]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]);
                new_node.h = now_h - a + b;
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道RIGHT
        if (now_x == 4 && now_y == 2)
        {
            if (now_node.state[now_y][0] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = 0;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "R";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y][0];
                new_node.state[now_y][0] = now_node.state[now_y][now_x];

                //计算h
                int a = (now_node.state[now_y][0] != target[now_y][0]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]);
                new_node.h = now_h - a + b;
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = end - begin;
    A_h1_f << new_node.op << ',' << fp_ms.count() / 1000 << "s" << endl;
    A_h1_f.close();
}

bool IDA_h1_DFS(int state[5][5], const int target[5][5], int maxH, int x, int y, int h, int g, string op)
{
    //剪枝

    if (h == 0)
    {
        fstream IDA_h1_f;
        IDA_h1_f.open("../output/output_IDA_h1.txt", ios::app | ios::in | ios::out);
        IDA_h1_f << op;
        IDA_h1_f.close();
        return 1;
    }
    //深度优先搜索
    int new_state[5][5];
    auto size = sizeof(int) * 5 * 5;
    //常规up
    if (y != 0)
    {
        if (state[y - 1][x] >= 0)
        //不是黑洞
        {

            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y - 1][x];
            new_state[y - 1][x] = state[y][x];

            //计算h
            int a = (state[y - 1][x] != target[y - 1][x]);
            int b = (target[y][x] != new_state[y][x]);

            if (h - a + b + g + 1 <= maxH)
            {
                if (IDA_h1_DFS(new_state, target, maxH, x, y - 1, h - a + b, g + 1, op + "U"))
                    return 1;
            }
        }
    }
    //隧道UP
    if (y == 0 && x == 2)
    {
        if (state[4][x] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[4][x];
            new_state[4][x] = state[y][x];

            //计算h
            int a = (state[4][x] != target[4][x]);
            int b = (target[y][x] != new_state[y][x]);

            if (h - a + b + g + 1 <= maxH)
            {

                if (IDA_h1_DFS(new_state, target, maxH, x, 4, h - a + b, g + 1, op + "U"))
                    return 1;
            }
        }
    }
    //常规DOWN
    if (y != 4)
    {
        if (state[y + 1][x] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y + 1][x];
            new_state[y + 1][x] = state[y][x];

            //计算h
            int a = (state[y + 1][x] != target[y + 1][x]);
            int b = (target[y][x] != new_state[y][x]);

            if (h - a + b + g + 1 <= maxH)
            {
                if (IDA_h1_DFS(new_state, target, maxH, x, y + 1, h - a + b, g + 1, op + "D"))
                    return 1;
            }
        }
    }
    //隧道Down
    if (y == 4 && x == 2)
    {
        if (state[0][x] >= 0)
        //不是黑洞
        {

            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[0][x];
            new_state[0][x] = state[y][x];

            //计算h
            int a = (state[0][x] != target[0][x]);
            int b = (target[y][x] != new_state[y][x]);

            if (h - a + b + g + 1 <= maxH)
            {
                if (IDA_h1_DFS(new_state, target, maxH, x, 0, h - a + b, g + 1, op + "D"))
                    return 1;
            }
        }
    }
    //常规LEFT
    if (x != 0)
    {
        if (state[y][x - 1] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y][x - 1];
            new_state[y][x - 1] = state[y][x];

            //计算h
            int a = (state[y][x - 1] != target[y][x - 1]);
            int b = (target[y][x] != new_state[y][x]);

            if (h - a + b + g + 1 <= maxH)
            {
                if (IDA_h1_DFS(new_state, target, maxH, x - 1, y, h - a + b, g + 1, op + "L"))
                    return 1;
            }
        }
    }
    //隧道LEFT
    if (x == 0 && y == 2)
    {
        if (state[y][4] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y][4];
            new_state[y][4] = state[y][x];

            //计算h
            int a = (state[y][4] != target[y][4]);
            int b = (target[y][x] != new_state[y][x]);

            if (h - a + b + g + 1 <= maxH)
            {
                if (IDA_h1_DFS(new_state, target, maxH, 4, y, h - a + b, g + 1, op + "L"))
                    return 1;
            }
        }
    }
    //常规RIGHT
    if (x != 4)
    {
        if (state[y][x + 1] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y][x + 1];
            new_state[y][x + 1] = state[y][x];

            //计算h
            int a = (state[y][x + 1] != target[y][x + 1]);
            int b = (target[y][x] != new_state[y][x]);

            if (h - a + b + g + 1 <= maxH)
            {
                if (IDA_h1_DFS(new_state, target, maxH, x + 1, y, h - a + b, g + 1, op + "R"))
                    return 1;
            }
        }
    }
    //隧道RIGHT
    if (x == 4 && y == 2)
    {
        if (state[y][0] >= 0)
        //不是黑洞
        {

            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y][0];
            new_state[y][0] = state[y][x];

            //计算h
            int a = (state[y][0] != target[y][0]);
            int b = (target[y][x] != new_state[y][x]);

            if (h - a + b + g + 1 <= maxH)
            {
                if (IDA_h1_DFS(new_state, target, maxH, 0, y, h - a + b, g + 1, op + "R"))
                    return 1;
            }
        }
    }
    return 0;
}

void IDA_h1(const int start[5][5], const int target[5][5])
{
    auto begin = std::chrono::high_resolution_clock::now();

    //初始找到为0的点 统计初始点的h值
    int now_x, now_y, now_h = 0;
    int maxH = 0; //阈值
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (start[i][j] == 0)
            {
                now_x = j;
                now_y = i;
            }
            else if (start[i][j] != target[i][j])
                now_h++;
        }
    }
    maxH = now_h;
    int state[5][5];
    memcpy(state, start, sizeof(int) * 5 * 5);
    //开始深度优先搜索
    while (!IDA_h1_DFS(state, target, maxH, now_x, now_y, now_h, 0, ""))
    {
        IDA_h1_DFS(state, target, maxH++, now_x, now_y, now_h, 0, "");
    }
    fstream IDA_h1_f;
    IDA_h1_f.open("../output/output_IDA_h1.txt", ios::app | ios::in | ios::out);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = end - begin;
    IDA_h1_f << ',' << fp_ms.count() / 1000 << "s" << endl;
    IDA_h1_f.close();
}

int cal_h2(int state[5][5], unordered_map<int, int> map_target)
{
    /*
    int hamiton = 0;
    int x, y;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (state[i][j] == 0)
            {
                y = i;
                x = j;
            }
        }
    }
    auto tar = map_target.find(0);
    auto x_tar = (*tar).second % 5;
    auto y_tar = ((*tar).second - x_tar) / 5;
    int re1 = abs(x - x_tar) + abs(y - y_tar);
    int re2 = abs(x - 2) + abs(y - 0) + 1 + abs(x_tar - 2) + abs(y_tar - 4);
    int re3 = abs(x - 2) + abs(y - 4) + 1 + abs(x_tar - 2) + abs(y_tar - 0);
    int re4 = abs(x - 0) + abs(y - 2) + 1 + abs(x_tar - 4) + abs(y_tar - 2);
    int re5 = abs(x - 4) + abs(y - 2) + 1 + abs(x_tar - 0) + abs(y_tar - 2);
    hamiton = min(min(min(re1, re2), min(re3, re4)), re5);
    return hamiton;
    */

    int rt;
    unordered_map<int, int> map_state;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (state[i][j] >= 0)
                map_state.insert(pair<int, int>(state[i][j], i * 5 + j));
        }
    }
    int x, y, x_tar, y_tar;
    int cnt = 0;
    int re = 0;
    int hamiton = 0;

    for (auto lt = map_state.begin(); lt != map_state.end(); lt++)
    {

        x = (*lt).second % 5;
        y = ((*lt).second - x) / 5;
        auto tar = map_target.find((*lt).first);
        x_tar = (*tar).second % 5;
        y_tar = ((*tar).second - x_tar) / 5;

        if ((x != x_tar || y != y_tar) && (*lt).first != 0)
            cnt++;

        int re1 = abs(x - x_tar) + abs(y - y_tar);
        int re2 = abs(x - 2) + abs(y - 0) + 1 + abs(x_tar - 2) + abs(y_tar - 4);
        int re3 = abs(x - 2) + abs(y - 4) + 1 + abs(x_tar - 2) + abs(y_tar - 0);
        int re4 = abs(x - 0) + abs(y - 2) + 1 + abs(x_tar - 4) + abs(y_tar - 2);
        int re5 = abs(x - 4) + abs(y - 2) + 1 + abs(x_tar - 0) + abs(y_tar - 2);
        re = re + min(min(min(re1, re2), min(re3, re4)), re5);
        if ((*lt).first == 0)
            hamiton = min(min(min(re1, re2), min(re3, re4)), re5);
    }
    return cnt;
}

int cal_h20(int state[5][5], unordered_map<int, int> map_target)
{
    int rt;
    unordered_map<int, int> map_state;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (state[i][j] >= 0)
                map_state.insert(pair<int, int>(state[i][j], i * 5 + j));
        }
    }
    int x, y, x_tar, y_tar;
    int re = 0;
    for (auto lt = map_state.begin(); lt != map_state.end(); lt++)
    {
        x = (*lt).second % 5;
        y = ((*lt).second - x) / 5;
        auto tar = map_target.find((*lt).first);
        x_tar = (*tar).second % 5;
        y_tar = ((*tar).second - x_tar) / 5;

        int re1 = abs(x - x_tar) + abs(y - y_tar);
        int re2 = abs(x - 2) + abs(y - 0) + 1 + abs(x_tar - 2) + abs(y_tar - 4);
        int re3 = abs(x - 2) + abs(y - 4) + 1 + abs(x_tar - 2) + abs(y_tar - 0);
        int re4 = abs(x - 0) + abs(y - 2) + 1 + abs(x_tar - 4) + abs(y_tar - 2);
        int re5 = abs(x - 4) + abs(y - 2) + 1 + abs(x_tar - 0) + abs(y_tar - 2);
        re += min(min(min(re1, re2), min(re3, re4)), re5);
    }
    return re;
}

void A_h2(const int start[5][5], const int target[5][5])
{
    auto size = sizeof(int) * 5 * 5;
    fstream A_h2_f;
    A_h2_f.open("../output/output_A_h2.txt", ios::app | ios::in | ios::out);
    auto begin = std::chrono::high_resolution_clock::now();

    unordered_map<int, int> map_target;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (target[i][j] >= 0)
                map_target.insert(pair<int, int>(target[i][j], i * 5 + j));
        }
    }

    //利用优先队列存储点的xy坐标,h,g
    priority_queue<Node, vector<Node>, cmp> node_queue;
    //初始找到为0的点 统计初始点的h值
    int now_x, now_y, now_g = 0;
    int now_h = 0;
    string now_op;
    Node now_node;
    Node new_node;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (start[i][j] == 0)
            {
                now_x = j;
                now_y = i;
                now_g = 0;
            }
        }
    }
    Node init_node;
    init_node.x = now_x;
    init_node.y = now_y;
    init_node.g = 0;
    memcpy(init_node.state, start, size);
    init_node.h = cal_h20(init_node.state, map_target);
    init_node.op = "";
    node_queue.push(init_node);

    //每次从优先队列中出队一个点 探寻周围的点 加入队列
    while (!node_queue.empty())
    {
        now_node = node_queue.top();
        node_queue.pop();
        now_x = now_node.x;
        now_y = now_node.y;
        now_g = now_node.g;
        now_h = now_node.h;
        now_op = now_node.op;
        //常规up
        if (now_y != 0)
        {
            if (now_node.state[now_y - 1][now_x] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = now_y - 1;
                new_node.g = now_g + 1;
                new_node.op = now_op + "U";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y - 1][now_x];
                new_node.state[now_y - 1][now_x] = now_node.state[now_y][now_x];

                //计算h
                new_node.h = cal_h20(new_node.state, map_target);
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道UP
        if (now_y == 0 && now_x == 2)
        {
            if (now_node.state[4][now_x] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = 4;
                new_node.g = now_g + 1;
                new_node.op = now_op + "U";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[4][now_x];
                new_node.state[4][now_x] = now_node.state[now_y][now_x];

                //计算h
                new_node.h = cal_h20(new_node.state, map_target);
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //常规DOWN
        if (now_y != 4)
        {
            if (now_node.state[now_y + 1][now_x] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = now_y + 1;
                new_node.g = now_g + 1;
                new_node.op = now_op + "D";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y + 1][now_x];
                new_node.state[now_y + 1][now_x] = now_node.state[now_y][now_x];

                //计算h
                new_node.h = cal_h20(new_node.state, map_target);
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道Down
        if (now_y == 4 && now_x == 2)
        {
            if (now_node.state[0][now_x] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = 0;
                new_node.g = now_g + 1;
                new_node.op = now_op + "D";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[0][now_x];
                new_node.state[0][now_x] = now_node.state[now_y][now_x];

                //计算h
                new_node.h = cal_h20(new_node.state, map_target);
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //常规LEFT
        if (now_x != 0)
        {
            if (now_node.state[now_y][now_x - 1] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x - 1;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "L";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y][now_x - 1];
                new_node.state[now_y][now_x - 1] = now_node.state[now_y][now_x];

                //计算h
                new_node.h = cal_h20(new_node.state, map_target);
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道LEFT
        if (now_x == 0 && now_y == 2)
        {
            if (now_node.state[now_y][4] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = 4;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "L";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y][4];
                new_node.state[now_y][4] = now_node.state[now_y][now_x];

                //计算h
                new_node.h = cal_h20(new_node.state, map_target);
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //常规RIGHT
        if (now_x != 4)
        {
            if (now_node.state[now_y][now_x + 1] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x + 1;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "R";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y][now_x + 1];
                new_node.state[now_y][now_x + 1] = now_node.state[now_y][now_x];

                //计算h
                new_node.h = cal_h20(new_node.state, map_target);
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道RIGHT
        if (now_x == 4 && now_y == 2)
        {
            if (now_node.state[now_y][0] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = 0;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "R";

                //交换结点
                memcpy(new_node.state, now_node.state, size);
                new_node.state[now_y][now_x] = now_node.state[now_y][0];
                new_node.state[now_y][0] = now_node.state[now_y][now_x];

                //计算h
                new_node.h = cal_h20(new_node.state, map_target);
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = end - begin;
    A_h2_f << new_node.op << ',' << fp_ms.count() / 1000 << "s" << endl;
    A_h2_f.close();
}

int compare(int state[5][5], unordered_map<int, int> map_target)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            if (state[i][j] >= 0)
            {
                auto lt = map_target.find(state[i][j]);
                if ((*lt).second != 5 * i + j)
                {
                    return 0;
                }
            }
        }
    return 1;
}

bool IDA_h2_DFS(int state[5][5], unordered_map<int, int> map_target, int maxH, int x, int y, int h, int g, string op)
{
    //剪枝
    if (compare(state, map_target) == 1)
    {
        fstream IDA_h2_f;
        IDA_h2_f.open("../output/output_IDA_h2.txt", ios::app | ios::in | ios::out);
        IDA_h2_f << op;
        IDA_h2_f.close();
        return 1;
    }
    //深度优先搜索
    int new_state[5][5];
    auto size = sizeof(int) * 5 * 5;
    int new_h;
    //常规up
    if (y != 0)
    {
        if (state[y - 1][x] >= 0)
        //不是黑洞
        {

            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y - 1][x];
            new_state[y - 1][x] = state[y][x];

            //计算h
            if ((new_h = cal_h2(new_state, map_target)) + g <= maxH)
            {
                if (IDA_h2_DFS(new_state, map_target, maxH, x, y - 1, new_h, g + 1, op + "U"))
                    return 1;
            }
        }
    }
    //隧道UP
    if (y == 0 && x == 2)
    {
        if (state[4][x] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[4][x];
            new_state[4][x] = state[y][x];

            //计算h
            if ((new_h = cal_h2(new_state, map_target)) + g <= maxH)
            {
                if (IDA_h2_DFS(new_state, map_target, maxH, x, 4, new_h, g + 1, op + "U"))
                    return 1;
            }
        }
    }
    //常规DOWN
    if (y != 4)
    {
        if (state[y + 1][x] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y + 1][x];
            new_state[y + 1][x] = state[y][x];

            //计算h
            if ((new_h = cal_h2(new_state, map_target)) + g <= maxH)
            {
                if (IDA_h2_DFS(new_state, map_target, maxH, x, y + 1, new_h, g + 1, op + "D"))
                    return 1;
            }
        }
    }
    //隧道Down
    if (y == 4 && x == 2)
    {
        if (state[0][x] >= 0)
        //不是黑洞
        {

            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[0][x];
            new_state[0][x] = state[y][x];

            //计算h
            if ((new_h = cal_h2(new_state, map_target)) + g <= maxH)
            {
                if (IDA_h2_DFS(new_state, map_target, maxH, x, 0, new_h, g + 1, op + "D"))
                    return 1;
            }
        }
    }
    //常规LEFT
    if (x != 0)
    {
        if (state[y][x - 1] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y][x - 1];
            new_state[y][x - 1] = state[y][x];

            //计算h
            if ((new_h = cal_h2(new_state, map_target)) + g <= maxH)
            {
                if (IDA_h2_DFS(new_state, map_target, maxH, x - 1, y, new_h, g + 1, op + "L"))
                    return 1;
            }
        }
    }
    //隧道LEFT
    if (x == 0 && y == 2)
    {
        if (state[y][4] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y][4];
            new_state[y][4] = state[y][x];

            //计算h
            if ((new_h = cal_h2(new_state, map_target)) + g <= maxH)
            {
                if (IDA_h2_DFS(new_state, map_target, maxH, 4, y, new_h, g + 1, op + "L"))
                    return 1;
            }
        }
    }
    //常规RIGHT
    if (x != 4)
    {
        if (state[y][x + 1] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y][x + 1];
            new_state[y][x + 1] = state[y][x];

            //计算h
            if ((new_h = cal_h2(new_state, map_target)) + g <= maxH)
            {
                if (IDA_h2_DFS(new_state, map_target, maxH, x + 1, y, new_h, g + 1, op + "R"))
                    return 1;
            }
        }
    }
    //隧道RIGHT
    if (x == 4 && y == 2)
    {
        if (state[y][0] >= 0)
        //不是黑洞
        {

            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[y][0];
            new_state[y][0] = state[y][x];

            //计算h
            if ((new_h = cal_h2(new_state, map_target)) + g <= maxH)
            {
                if (IDA_h2_DFS(new_state, map_target, maxH, 0, y, new_h, g + 1, op + "R"))
                    return 1;
            }
        }
    }
    return 0;
}

void IDA_h2(const int start[5][5], const int target[5][5])
{
    auto begin = std::chrono::high_resolution_clock::now();

    unordered_map<int, int> map_target;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (target[i][j] >= 0)
                map_target.insert(pair<int, int>(target[i][j], i * 5 + j));
        }
    }

    //初始找到为0的点 统计初始点的h值
    int now_x, now_y, now_h = 0;
    int maxH = 0; //阈值
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (start[i][j] == 0)
            {
                now_x = j;
                now_y = i;
            }
        }
    }
    int state[5][5];
    memcpy(state, start, sizeof(int) * 5 * 5);
    now_h = maxH = cal_h2(state, map_target);
    //开始深度优先搜索
    while (!IDA_h2_DFS(state, map_target, maxH, now_x, now_y, now_h, 0, ""))
    {
        IDA_h2_DFS(state, map_target, maxH++, now_x, now_y, now_h, 0, "");
    }
    fstream IDA_h2_f;
    IDA_h2_f.open("../output/output_IDA_h2.txt", ios::app | ios::in | ios::out);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = end - begin;
    IDA_h2_f << ',' << fp_ms.count() / 1000 << "s" << endl;
    IDA_h2_f.close();
}
