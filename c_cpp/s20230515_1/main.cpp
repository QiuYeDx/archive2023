#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;
// 123412512345
const int test[12] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
// 存放页面访问序列的数组
const int total_instruction = 12;
int Acess_Series[total_instruction];

// 用一个结构数组M_Frame[]记录为进程分配的内存页面的使用情况
struct one_frame
{
    int page_no;
    char flag;
};

// 初始化M_Frame[]
void init_M_Frame(one_frame *M_Frame, int frame_num)
{
    for (int i = 0; i < frame_num; i++)
    {
        M_Frame[i].page_no = -1; // 初始化为-1表示该页为空闲页
        M_Frame[i].flag = 'N';   // 初始化为未被访问
    }
}

// FIFO页面置换算法
int FIFO(one_frame *M_Frame, int frame_num)
{
    int head = 0;      // M_Frame[]数组头指针
    int tail = 0;      // M_Frame[]数组尾指针
    int disaffect = 0; // 缺页次数
    for (int i = 0; i < total_instruction; i++)
    {
        int page_no = Acess_Series[i]; // 取出下一个要访问的页面
        bool hit = false;              // 是否命中标志
        for (int j = 0; j < frame_num; j++)
        {
            if (M_Frame[j].page_no == page_no)
            { // 页面命中
                hit = true;
                break;
            }
        }
        if (!hit)
        {                // 页面缺失
            disaffect++; // 缺页次数加一
            if (tail - head < frame_num && tail < frame_num)
            {                                                // M_Frame[]还有空闲页面
                M_Frame[tail % frame_num].page_no = page_no; // 将页面装入M_Frame[]
                M_Frame[tail % frame_num].flag = 'Y';
                tail++;
            }
            else
            {                                                // M_Frame[]没有空闲页面，需要替换掉头部页面
                M_Frame[head % frame_num].page_no = page_no; // 将页面装入M_Frame[]
                M_Frame[head % frame_num].flag = 'Y';
                head++;
            }
        }

        cout << i << "状态: " << endl;
        for(int i = 0; i < frame_num; i++){
            if(M_Frame[(head + i) % frame_num].page_no == -1)
                cout << " - ";
            else
                cout << " " << M_Frame[(head + i) % frame_num].page_no << " ";
        }
        if(hit)
            cout << " hit " << page_no << endl;
        else
            cout << " miss " << page_no << endl;
    }
    return disaffect; // 返回缺页次数
}


// LRU页面置换算法
int LRU(one_frame *M_Frame, int frame_num) {
    int disaffect = 0; // 缺页次数
    for (int i = 0; i < total_instruction; i++) {
        int page_no = Acess_Series[i]; // 取出下一个要访问的页面
        bool hit = false; // 是否命中标志
        for (int j = 0; j < frame_num; j++) {
            if (M_Frame[j].page_no == page_no) { // 页面命中
                hit = true;
                // 将该页面移动到M_Frame[]的末尾，即访问时间最近
                for (int k = j; k < frame_num - 1; k++) {
                    M_Frame[k].page_no = M_Frame[k + 1].page_no;
                    M_Frame[k].flag = M_Frame[k + 1].flag;
                }
                M_Frame[frame_num - 1].page_no = page_no;
                M_Frame[frame_num - 1].flag = 'Y';
                break;
            }
        }
        if (!hit) { // 页面缺失
            disaffect++; // 缺页次数加一
            if (i < frame_num) { // M_Frame[]还有空闲页面
                M_Frame[i].page_no = page_no; // 将页面装入M_Frame[]
                M_Frame[i].flag = 'Y';
            } else { // M_Frame[]没有空闲页面，需要替换掉最早访问的页面
                int min_index = 0;
                for (int j = 1; j < frame_num; j++) {
                    if (M_Frame[j].flag < M_Frame[min_index].flag) {
                        min_index = j;
                    }
                }
                M_Frame[min_index].page_no = page_no; // 将页面装入M_Frame[]
                M_Frame[min_index].flag = 'Y';
            }
            // 更新M_Frame[]中页面的访问时间戳
            for (int j = 0; j < frame_num; j++) {
                if (M_Frame[j].page_no != -1) {
                    M_Frame[j].flag--;
                }
            }
        }

        cout << i << "状态: " << endl;
        for(int i = 0; i < frame_num; i++){
            if(M_Frame[i].page_no == -1)
                cout << " - ";
            else
                cout << " " << M_Frame[i].page_no << " ";
        }
        if(hit)
            cout << " hit " << page_no << endl;
        else
            cout << " miss " << page_no << endl;
    }
    
    return disaffect; // 返回缺页次数
}


int main()
{
    srand((unsigned)time(NULL)); // 随机数种子
    cout << "Acess_Series: ";
    for (int i = 0; i < total_instruction; i++)
    {
        // Acess_Series[i] = rand() % 5; // 随机生成0~4之间的整数
        Acess_Series[i] = test[i];
        cout << Acess_Series[i];
    }
    cout << endl;

    int frame_num = 3; // 为进程分配的最大的内存页面数
    cout << "\ninput frame_num:" << endl;
    while (cin >> frame_num)
    {
        one_frame M_Frame[frame_num];

        for (int i = 0; i < 2; i++)
        {
            int pid = fork(); // 创建子进程
            if (pid < 0)
            { // 子进程创建失败
                cout << "Error: fork() failed!" << endl;
                return -1;
            }
            else if (pid == 0 && i == 0)
            {                                     // 子进程1
                init_M_Frame(M_Frame, frame_num); // 初始化M_Frame[]
                int ans = FIFO(M_Frame, frame_num);
                cout << "FIFO算法缺页次数: " << ans << " 命中率: " << (12 - ans) * 1.0 / 12 << endl << endl;
                exit(0);
            }
            else if (pid == 0 && i == 1)
            {                                     // 子进程2
                init_M_Frame(M_Frame, frame_num); // 初始化M_Frame[]
                int ans2 = LRU(M_Frame, frame_num);
                cout << "LRU算法缺页次数: " << ans2 << " 命中率: " << (12 - ans2) * 1.0 / 12 << endl;
                exit(0);
            }
            wait(&pid); // 等待子进程结束
        }

        cout << "\ninput frame_num:" << endl;
    }

    return 0;
}