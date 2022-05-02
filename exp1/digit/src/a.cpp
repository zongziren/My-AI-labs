#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <ctime>

using namespace std;
void A_h1(const int start[5][5], const int target[5][5]);
void A_h2(const int start[5][5], const int target[5][5]);
void IDA_h1(const int start[5][5], const int target[5][5]);
void IDA_h2(const int start[5][5], const int target[5][5]);

enum Tactic
{
    Ah1,
    Ah2,
    IDAh1,
    IDAh2,
};
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
    Tactic tatic;
    if (argv[0] == "A_h1")
        tatic = Ah1;
    else if (argv[0] == "A_h1")
        tatic = Ah2;
    else if (argv[0] == "IDA_h1")
        tatic = IDAh1;
    else if (argv[0] == "IDA_h2")
        tatic = IDAh2;

    // open input file
    fstream input_file;
    fstream target_file;
    string inputname, targetname;
    inputname = argv[1];
    targetname = argv[2];
    inputname = "../data/" + inputname;
    targetname = "../data/" + targetname;
    input_file.open(inputname, ios::in);
    target_file.open(targetname, ios::in);

    // A_h2_f.open("output_A_h2.txt", ios::app | ios::in | ios::out);
    // IDA_h1_f.open("output_A_h2.txt", ios::app | ios::in | ios::out);
    // IDA_h2_f.open("output_A_h2.txt", ios::app | ios::in | ios::out);

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
    if (tatic == Ah1)
        A_h1(start, target);
    return 0;
}

void A_h1(const int start[5][5], const int target[5][5])
{
    fstream A_h1_f;
    A_h1_f.open("output_A_h1.txt", ios::app | ios::in | ios::out);
    clock_t begin, end;
    begin = clock();

    //利用优先队列存储点的xy坐标,h,g
    // priority_queue<Node> node_queue;
    priority_queue<Node, vector<Node>, cmp> node_queue;
    //初始找到为0的点 统计初始点的h值
    int now_x, now_y, now_h, now_g = 0;
    string now_op;
    Node now_node;
    Node new_node;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (start[i][j] == 0)
            {
                now_x = i;
                now_y = j;
                now_g = 0;
            }
            if (start[i][j] != target[i][j])
                now_h++;
        }
    }
    Node init_node = {now_x, now_y, now_h, 0};
    memcpy(init_node.state, start, sizeof(start));
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
            if (now_node.state[now_x][now_y - 1] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = now_y - 1;
                new_node.g = now_g + 1;
                new_node.op = now_op + "U";

                //交换结点
                memcpy(new_node.state, now_node.state, sizeof(now_node.state));
                new_node.state[now_x][now_y] = now_node.state[now_x][now_y - 1];
                new_node.state[now_x][now_y - 1] = now_node.state[now_x][now_y];

                //计算h
                int a = (target[now_x][now_y] != 0) + (now_node.state[now_x][now_y - 1] != target[now_x][now_y - 1]);
                int b = (target[now_x][now_y] != new_node.state[now_x][now_y]) + (target[now_x][now_y - 1] != 0);
                new_node.h = now_h - a + b;
                if (now_node.h == 0)
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
                memcpy(new_node.state, now_node.state, sizeof(now_node.state));
                new_node.state[now_x][now_y] = now_node.state[now_x][4];
                new_node.state[now_x][4] = now_node.state[now_x][now_y];

                //计算h
                int a = (target[now_x][now_y] != 0) + (now_node.state[now_x][4] != target[now_x][4]);
                int b = (target[now_x][now_y] != new_node.state[now_x][now_y]) + (target[now_x][4] != 0);
                new_node.h = now_h - a + b;
                if (now_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //常规DOWN
        if (now_y != 4)
        {
            if (now_node.state[now_x][now_y + 1] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = now_y + 1;
                new_node.g = now_g + 1;
                new_node.op = now_op + "D";

                //交换结点
                memcpy(new_node.state, now_node.state, sizeof(now_node.state));
                new_node.state[now_x][now_y] = now_node.state[now_x][now_y + 1];
                new_node.state[now_x][now_y + 1] = now_node.state[now_x][now_y];

                //计算h
                int a = (target[now_x][now_y] != 0) + (now_node.state[now_x][now_y + 1] != target[now_x][now_y + 1]);
                int b = (target[now_x][now_y] != new_node.state[now_x][now_y]) + (target[now_x][now_y + 1] != 0);
                new_node.h = now_h - a + b;
                if (now_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道Down
        if (now_y == 4 && now_x == 2)
        {
            if (now_node.state[now_x][0] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x;
                new_node.y = 0;
                new_node.g = now_g + 1;
                new_node.op = now_op + "D";

                //交换结点
                memcpy(new_node.state, now_node.state, sizeof(now_node.state));
                new_node.state[now_x][now_y] = now_node.state[now_x][0];
                new_node.state[now_x][0] = now_node.state[now_x][now_y];

                //计算h
                int a = (target[now_x][now_y] != 0) + (now_node.state[now_x][0] != target[now_x][0]);
                int b = (target[now_x][now_y] != new_node.state[now_x][now_y]) + (target[now_x][0] != 0);
                new_node.h = now_h - a + b;
                if (now_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //常规LEFT
        if (now_x != 0)
        {
            if (now_node.state[now_x - 1][now_y] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x - 1;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "L";

                //交换结点
                memcpy(new_node.state, now_node.state, sizeof(now_node.state));
                new_node.state[now_x][now_y] = now_node.state[now_x - 1][now_y];
                new_node.state[now_x - 1][now_y] = now_node.state[now_x][now_y];

                //计算h
                int a = (target[now_x][now_y] != 0) + (now_node.state[now_x - 1][now_y] != target[now_x - 1][now_y]);
                int b = (target[now_x][now_y] != new_node.state[now_x][now_y]) + (target[now_x - 1][now_y] != 0);
                new_node.h = now_h - a + b;
                if (now_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道LEFT
        if (now_x == 0 && now_y == 2)
        {
            if (now_node.state[4][now_y] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = 4;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "L";

                //交换结点
                memcpy(new_node.state, now_node.state, sizeof(now_node.state));
                new_node.state[now_x][now_y] = now_node.state[4][now_y];
                new_node.state[4][now_y] = now_node.state[now_x][now_y];

                //计算h
                int a = (target[now_x][now_y] != 0) + (now_node.state[4][now_y] != target[4][now_y]);
                int b = (target[now_x][now_y] != new_node.state[now_x][now_y]) + (target[4][now_y] != 0);
                new_node.h = now_h - a + b;
                if (now_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //常规RIGHT
        if (now_x != 4)
        {
            if (now_node.state[now_x + 1][now_y] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = now_x + 1;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "R";

                //交换结点
                memcpy(new_node.state, now_node.state, sizeof(now_node.state));
                new_node.state[now_x][now_y] = now_node.state[now_x + 1][now_y];
                new_node.state[now_x + 1][now_y] = now_node.state[now_x][now_y];

                //计算h
                int a = (target[now_x][now_y] != 0) + (now_node.state[now_x + 1][now_y] != target[now_x + 1][now_y]);
                int b = (target[now_x][now_y] != new_node.state[now_x][now_y]) + (target[now_x + 1][now_y] != 0);
                new_node.h = now_h - a + b;
                if (now_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
        //隧道RIGHT
        if (now_x == 4 && now_y == 2)
        {
            if (now_node.state[0][now_y] >= 0)
            //不是黑洞
            {
                //新x,y,g,op
                new_node.x = 0;
                new_node.y = now_y;
                new_node.g = now_g + 1;
                new_node.op = now_op + "R";

                //交换结点
                memcpy(new_node.state, now_node.state, sizeof(now_node.state));
                new_node.state[now_x][now_y] = now_node.state[0][now_y];
                new_node.state[0][now_y] = now_node.state[now_x][now_y];

                //计算h
                int a = (target[now_x][now_y] != 0) + (now_node.state[0][now_y] != target[0][now_y]);
                int b = (target[now_x][now_y] != new_node.state[now_x][now_y]) + (target[0][now_y] != 0);
                new_node.h = now_h - a + b;
                if (now_node.h == 0)
                    break;

                //入队
                node_queue.push(new_node);
            }
        }
    }
    end = clock();
    A_h1_f << new_node.op << '+' << end - begin << endl;
    A_h1_f.close();
}