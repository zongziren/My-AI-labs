#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cstring>
#include <ctime>

using namespace std;
void A_h1(const int start[5][5], const int target[5][5]);
void A_h2(const int start[5][5], const int target[5][5]);
void IDA_h1(const int start[5][5], const int target[5][5]);
void IDA_h2(const int start[5][5], const int target[5][5]);

typedef struct
{
    //左边
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
    if (tatic == 2)
        IDA_h1(start, target);

    return 0;
}

void A_h1(const int start[5][5], const int target[5][5])
{
    auto size = sizeof(int) * 5 * 5;
    fstream A_h1_f;
    A_h1_f.open("../output/output_A_h1.txt", ios::app | ios::in | ios::out);
    clock_t begin, end;
    begin = clock();

    //利用优先队列存储点的xy坐标,h,g
    // priority_queue<Node> node_queue;
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
            if (start[i][j] != target[i][j])
                now_h++;
        }
    }
    Node init_node;
    init_node.x = now_x;
    init_node.y = now_y;
    init_node.h = now_h;
    memcpy(init_node.state, start, size);
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
                int a = (target[now_y][now_x] != 0) + (now_node.state[now_y - 1][now_x] != target[now_y - 1][now_x]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]) + (target[now_y - 1][now_x] != 0);
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
            if (now_node.state[now_x][4] >= 0)
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
                int a = (target[now_y][now_x] != 0) + (now_node.state[4][now_x] != target[4][now_x]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]) + (target[4][now_x] != 0);
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
                int a = (target[now_y][now_x] != 0) + (now_node.state[now_y + 1][now_x] != target[now_y + 1][now_x]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]) + (target[now_y + 1][now_x] != 0);
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
                int a = (target[now_y][now_x] != 0) + (now_node.state[0][now_x] != target[0][now_x]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]) + (target[0][now_x] != 0);
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
                int a = (target[now_y][now_x] != 0) + (now_node.state[now_y][now_x - 1] != target[now_y][now_x - 1]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]) + (target[now_y][now_x - 1] != 0);
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
                int a = (target[now_y][now_x] != 0) + (now_node.state[now_y][4] != target[now_y][4]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]) + (target[now_y][4] != 0);
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
                int a = (target[now_y][now_x] != 0) + (now_node.state[now_y][now_x + 1] != target[now_y][now_x + 1]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]) + (target[now_y][now_x + 1] != 0);
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
                int a = (target[now_y][now_x] != 0) + (now_node.state[now_y][0] != target[now_y][0]);
                int b = (target[now_y][now_x] != new_node.state[now_y][now_x]) + (target[now_y][0] != 0);
                new_node.h = now_h - a + b;
                if (new_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
    }
    end = clock();
    A_h1_f << new_node.op << '+' << (float)(end - begin) / 1000000 << "s" << endl;
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
            int a = (target[y][x] != 0) + (state[y - 1][x] != target[y - 1][x]);
            int b = (target[y][x] != new_state[y][x]) + (target[y - 1][x] != 0);

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
        if (state[x][4] >= 0)
        //不是黑洞
        {
            //交换结点
            memcpy(new_state, state, size);
            new_state[y][x] = state[4][x];
            new_state[4][x] = state[y][x];

            //计算h
            int a = (target[y][x] != 0) + (state[4][x] != target[4][x]);
            int b = (target[y][x] != new_state[y][x]) + (target[4][x] != 0);

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
            int a = (target[y][x] != 0) + (state[y + 1][x] != target[y + 1][x]);
            int b = (target[y][x] != new_state[y][x]) + (target[y + 1][x] != 0);

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
            int a = (target[y][x] != 0) + (state[0][x] != target[0][x]);
            int b = (target[y][x] != new_state[y][x]) + (target[0][x] != 0);

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
            int a = (target[y][x] != 0) + (state[y][x - 1] != target[y][x - 1]);
            int b = (target[y][x] != new_state[y][x]) + (target[y][x - 1] != 0);

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
            int a = (target[y][x] != 0) + (state[y][4] != target[y][4]);
            int b = (target[y][x] != new_state[y][x]) + (target[y][4] != 0);

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
            int a = (target[y][x] != 0) + (state[y][x + 1] != target[y][x + 1]);
            int b = (target[y][x] != new_state[y][x]) + (target[y][x + 1] != 0);

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
            int a = (target[y][x] != 0) + (state[y][0] != target[y][0]);
            int b = (target[y][x] != new_state[y][x]) + (target[y][0] != 0);

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
    clock_t begin, end;
    begin = clock();

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
            if (start[i][j] != target[i][j])
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
    end = clock();
    fstream IDA_h1_f;
    IDA_h1_f.open("../output/output_IDA_h1.txt", ios::app | ios::in | ios::out);
    IDA_h1_f << '+' << (float)(end - begin) / 1000000 << "s" << endl;
    IDA_h1_f.close();
}