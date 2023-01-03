/*
 * Cong Li
 * Database system experiment VI
 * December 19, 2022
 */

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"

#define BOOL int
#define TRUE 1
#define FALSE 0

/**
 * @brief
 * 从磁盘块读取指定元组的某个属性转换成int类型后返回
 *
 * @param blk       待读取的磁盘块
 * @param tid       元组号
 * @param aid       属性号，0表示第一个属性，1表示第二个属性
*/
int readNumFromBlk(unsigned char *blk, int tid, int aid);

/**
 * @brief
 * 实现基于线性搜索的关系选择算法：
 * 基于ExtMem程序库，使用C语言实现线性搜索算法
 * 选出S.C=128的元组，记录IO读写次数，并将选择结果存放在磁盘块101,102,......上
 *（模拟实现 select S.C, S.D from S where S.C = 128）
 *
 * @param buf 缓冲区
 * @param val 进行选择的值
*/
void lnearSearch(Buffer *buf, int val);

/**
 * @brief
 * TPMMS的第一趟：划分子集合并进行子集合排序，子集合含8个磁盘块
 * 中间结果存入200+原来磁盘块号的内存中
 *
 * @param buf   内存缓冲区
 * @param start 待排序全集的起始块号
 * @param end   待排序全集的终止块号
*/
void divideAndSort(Buffer *buf, int start, int end);

/**
 * @brief 数组求和
*/
int arrSum(int array[], int size);

/**
 * @brief
 * TPMMS的第二趟：各子集间的归并排序
 * 结果放在300+原来磁盘号上
 *
 * @param buf   内存缓冲区
 * @param start 已排序子集的起始块号
 * @param end   已排序子集的终止块号
*/
void mergeSort(Buffer *buf, int start, int end);

/**
 * @brief
 * 两阶段多路归并排序算法
 *
 * @param buf 内存缓冲区
 * @param relation 需要进行排序的关系
*/
void TPMMS(Buffer *buf, char relation);

/**
 * @brief
 * 创建关系S的索引文件，结果写入块号为400+序号的磁盘中
 *
 * @param buf 内存缓冲区
 *
*/
void creatIndexOfS(Buffer *buf);

/**
 * @brief
 * 基于索引的关系选择算法，结果写入501.blk,502.blk,......
 *
 * @param buf 内存缓冲区
 * @param val 进行选择的值
*/
void indexBasedSelect(Buffer *buf, int val);

/**
 * @brief
 * 实现基于排序的连接操作算法（Sort-Merge-Join）
 * 对关系S和R计算S.C连接R.A ，并统计连接次数，将连接结果存放在磁盘601.blk,602.blk,......
 * （模拟实现 select S.C, S.D, R.A, R.B from S innerjoin R on S.C = R.A）
 *
 * @param buf 内存缓冲区
*/
void sortMergeJoin(Buffer *buf);

/**
 * @brief
 * 基于排序的两趟扫描算法，实现集合的差操作
 * 将结果存放在1001.blk, 1002.blk, ......
 *
 * @param buf 内存缓冲区
*/
void sortBasedDifference(Buffer *buf);

/**
 * @brief
 * 基于排序的两趟扫描算法，实现集合的交操作
 * 将结果存放在2001.blk, 2002.blk, ......
 *
 * @param buf 内存缓冲区
*/
void sortBasedIntersection(Buffer *buf);

/**
 * @brief
 * 基于排序的两趟扫描算法，实现集合的并操作
 * 将结果存放在1001.blk, 1002.blk, ......
 *
 * @param buf 内存缓冲区
*/
void sortBasedUnion(Buffer *buf);

int main(int argc, char **argv)
{
    Buffer buf;
    int val = 128;

    lnearSearch(&buf, val);
    TPMMS(&buf, 'R');
    TPMMS(&buf, 'S');
    creatIndexOfS(&buf);
    indexBasedSelect(&buf, val);
    sortMergeJoin(&buf);
    sortBasedDifference(&buf);
    sortBasedIntersection(&buf);
    sortBasedUnion(&buf);

    return 0;
}

int readNumFromBlk(unsigned char *blk, int tid, int aid)
{
    char str[5];
    int i;
    for (i = 0; i < 4; i++)
    {
        str[i] = *(blk + tid*8 + aid*4 + i);
    }
    return atoi(str);
}

void lnearSearch(Buffer *buf, int val)
{
    unsigned char *rblk;   // 输入缓冲块
    unsigned char *wblk;   // 输出缓冲块
    int usage = 0;          // 输出缓冲块已用空间
    int wnum = 0;           // 写磁盘的次数
    int S_start = 17;       // 关系S的起始块
    int S_end = 48;         // 关系S的终止块
    int i, j, k;
    int C = -1;
    int D = -1;
    char str[5];
    int cnt = 0;            // 满足条件的元组个数

    printf("------------------------------\n");
    printf("基于线性搜索的关系选择算法 S.C = 128\n");
    printf("------------------------------\n");

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    rblk = getNewBlockInBuffer(buf);
    wblk = getNewBlockInBuffer(buf);

    // 依次读取磁盘数据块
    for (i = S_start; i <= S_end; i++)
    {
        if ((rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("读入数据块%d\n", i);

        // 读数据块中的7个元组
        for (j = 0; j < 7; j++)
        {
            for (k = 0; k < 4; k++)
            {
                str[k] = *(rblk + j*8 + k);
            }
            C = atoi(str);
            if (C != val)
            {
                continue;
            }
            cnt++;
            for (k = 0; k < 4; k++)
            {
                str[k] = *(rblk + j*8 + 4 + k);
            }
            D = atoi(str);
            printf("(%d, %d) \n", C, D);
            // 将选择结果放入输出缓冲块
            memcpy(wblk + usage*8, rblk + j*8, 8);
            usage++;
            // 输出缓冲块写满后写回磁盘
            if(usage == 7)
            {
                wnum++;
                itoa(100+wnum+1, wblk + usage*8, 10);   // 后继块的地址
                if (writeBlockToDisk(wblk, 100+wnum, buf) != 0)
                {
                    perror("Writing Block Failed!\n");
                    return -1;
                }
                printf("写入磁盘块: %d\n", 100+wnum);
                wblk = getNewBlockInBuffer(buf);
                memset(wblk, 0, 64);
                usage = 0;
            }
        }
        // 释放空间
        freeBlockInBuffer(rblk, buf);
    }
    if (usage != 0)
    {
        wnum++;
        itoa(100+wnum+1, wblk + usage*8, 10);   // 后继块的地址
        if (writeBlockToDisk(wblk, 100+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("写入磁盘块: %d\n", 100+wnum);
    }
    printf("\n满足条件的元组个数 %d\n", cnt);
    printf("IO读写一共%d次\n", buf->numIO);
}

void divideAndSort(Buffer *buf, int start, int end)
{
    printf("----Step1.划分子集&子集排序---\n");

    int outer, inner;
    int i, j;
    int bid, tid;       // 冒泡排序遍历的元素所在的块号、元组号
    int bid1, tid1;     // 下一个元素的块号、元组号
    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    unsigned char *blks[buf->numAllBlk];    // 内存所有块用于处理子集合
    char temp[10];

    // 每次处理一个子集合(8块)
    for (outer = start; outer <= end; outer += 8)
    {
        // 子集合全部读入内存
        for (inner = 0; inner < 8; inner++)
        {
            if ((blks[inner] = readBlockFromDisk(outer + inner, buf)) == NULL) {
                perror("Reading Block Failed!\n");
                return -1;
            }
            printf("读入数据块%d\n", outer+inner);
        }

        // 使用冒泡排序法对子集合排序，排序码为第一个属性(A或C)
        // 子集合共56个元组
//        printf("进行冒泡排序\n");
        for (i = 0; i < 56; i++)
        {
            for (j = 0; j < 55 - i; j++)
            {
                bid = j / 7;
                tid = j % 7;
                bid1 = (j+1) / 7;
                tid1 = (j+1) % 7;
                if(readNumFromBlk(blks[bid], tid, 0) > readNumFromBlk(blks[bid1], tid1, 0))
                {
                    // 进行交换
                    memcpy(temp, blks[bid] + tid*8, 8); // temp是指针时会出错
                    memcpy(blks[bid] + tid*8, blks[bid1] + tid1*8, 8);
                    memcpy(blks[bid1] + tid1*8, temp, 8);
                }
            }
        }
        // printf("排序完成\n");
        // 写回磁盘200+原来所在块号
        for(inner = 0; inner < 8; inner++)
        {
            itoa(200+outer+inner+1, blks[inner]+56, 10);   // 后继块的地址
            if (writeBlockToDisk(blks[inner], 200 + outer + inner, buf) != 0)
            {
                perror("Writing Block Failed!\n");
                return -1;
            }
            memset(blks[inner], 0, 64);
            printf("写回磁盘%d\n", 200+outer+inner);
        }

    }
}

int arrSum(int array[], int size)
{
    int i;
    int sum = 0;
    for (i = 0; i < size; i++)
    {
        sum = sum + array[i];
    }
    return sum;
}

void mergeSort(Buffer *buf, int start, int end)
{
    printf("\n--------Step2.归并排序--------\n");

    int sub_num = (end - start + 1) / 8;            // 子集合个数
    // if (!initBuffer(520, 64, buf))
    // {
    //     perror("Buffer Initialization Failed!\n");
    //     return -1;
    // }
    unsigned char *rblks[sub_num];                 // 输入缓冲块数组
    int bid[sub_num];                               // 子集合当前输入内存的块号[0,7]
    int tid[sub_num];                               // 输入块中进行下一次比较的元组号[0,6]
    BOOL finish[sub_num];                           // 子集合处理完毕
    unsigned char *wblk = getNewBlockInBuffer(buf);// 输出缓冲块
    int usage = 0;                                  // 输出缓冲块已装入元组数[0,7]
    int i;
    int small;          // 子集合中进行比较的元素的最小值
    int small_id;       // small对应的子集合号
    int wnum = 0;       // 已写块数

    // 读入每个子集合的第一块
    for (i = 0; i < sub_num; i++)
    {
        bid[i] = 0;
        tid[i] = 0;
        finish[i] = FALSE;
        if ((rblks[i] = readBlockFromDisk(start + i*8, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("读入数据块%d\n", start + i*8);
    }

    while(arrSum(finish, sub_num) < sub_num)
    {
        small = 1 << 30;
        small_id = -1;
        // 找到当前归并的sub_num路元组的最小值
        for (i = 0; i < sub_num; i++)
        {
            if (finish[i] == TRUE)
            {
                // 这个子集合已经全部处理完毕
                continue;
            }
            else if (readNumFromBlk(rblks[i], tid[i], 0) < small)
            {
                small = readNumFromBlk(rblks[i], tid[i], 0);
                small_id = i;
            }
        }
        // printf("small: %d\n", small);
        if (small_id == -1)
        {
            perror("MergeSort Error!\n");
            return -1;
        }
        memcpy(wblk + usage*8, rblks[small_id] + tid[small_id]*8, 8);

        usage++;
        // 判断输出缓冲块是否写满
        if(usage == 7)
        {
            itoa(100+start+wnum+1, wblk + usage*8, 10);   // 后继块的地址
            if (writeBlockToDisk(wblk, 100+start+wnum, buf) != 0)
            {
                perror("Writing Block Failed!\n");
                return -1;
            }
            printf("写回磁盘: %d\n", 100+start+wnum);
            wblk = getNewBlockInBuffer(buf);
            memset(wblk, 0, 64);
            usage = 0;
            wnum++;
        }

        tid[small_id]++;    // 处理完一个元组，向后移一位
        // 判断该子集合输入的磁盘块是否全部处理完
        if(tid[small_id] == 7)
        {
            bid[small_id]++;
            // 判断该子集合是否全部处理完毕
            if (bid[small_id] == 8)
            {
                finish[small_id] = TRUE;
            }
            else
            {
                freeBlockInBuffer(rblks[small_id], buf);
                // 输入对应子集合的下一个磁盘块
                if ((rblks[small_id] = readBlockFromDisk(start+small_id*8+bid[small_id], buf)) == NULL)
                {
                    perror("Reading Block Failed!\n");
                    return -1;
                }
                printf("读入数据块: %d\n", start+small_id*8+bid[small_id]);
                tid[small_id] = 0;
            }
        }
    }

    // 释放内存空间
    freeBuffer(buf);
}

void TPMMS(Buffer *buf, char relation)
{
    int start, end;
    printf("\n------------------------------\n");
    printf("两阶段多路归并排序\n");
    printf("------------------------------\n");

    switch (relation)
    {
        case 'R':
            start = 1;
            end = 16;
            break;
        case 'S':
            start = 17;
            end = 48;
            break;
        default:
            perror("Selecting Relation Faild\n");
            return -1;
    }
    divideAndSort(buf, start, end);
    mergeSort(buf, 200+start, 200+end);
}

void creatIndexOfS(Buffer *buf)
{
    printf("\n------------------------------\n");
    printf("创建索引文件\n");
    printf("------------------------------\n");

    int start = 17;
    int end = 48;
    unsigned char *rblk;    // 输入缓冲区
    unsigned char *wblk;    // 输出缓冲区
    int usage = 0;          // 输出缓冲区已写元组个数
    int wnum = 0;           // 已写块数
    int idx_field = 0;      // 索引字段
    char pointer[4];        // 指向排序后的表中包含索引字段的记录所在磁盘块号
    int i, j;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);
    memset(wblk, 0, 64);
    memset(pointer, 0, 4);

    for (i = start; i <= end; i++)
    {
        // 从已排序的文件中读取一个数据块
        if ((rblk = readBlockFromDisk(300+i, buf)) == NULL)
        {
            perror("Reading Sorted Block Failed!\n");
            return -1;
        }
        printf("读入数据块%d\n", 300 + i);
        for (j = 0; j < 7; j++)
        {
            if(idx_field != readNumFromBlk(rblk, j, 0))
            {
                // 出现新的索引字段
//                printf("idx: %d, new: %d, j: %d\n", idx_field, readNumFromBlk(rblk, j, 0), j);
                idx_field = readNumFromBlk(rblk, j, 0);
                itoa(300+i, pointer, 10);

                memcpy(wblk + usage*8, rblk + j*8, 4);      // 将索引字段写入输出缓冲区
                memcpy(wblk + usage*8 + 4, pointer, 4);     // 将指针写入输出缓冲区
                memset(pointer, 0, 4);
                usage++;
                // 判断输出缓冲块是否写满
                if(usage == 7)
                {
                    itoa(400+start+wnum+1, wblk + usage*8, 10);   // 后继块的地址
                    if (writeBlockToDisk(wblk, 400+start+wnum, buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("写回磁盘: %d\n", 400+start+wnum);
                    wblk = getNewBlockInBuffer(buf);
                    memset(wblk, 0, 64);
                    usage = 0;
                    wnum++;
                }
            }
        }
        freeBlockInBuffer(rblk, buf);
    }
    // 检查输出缓冲区是否有数据，若有，将其写回磁盘
    if (usage != 0)
    {
        itoa(400+start+wnum+1, wblk + 56, 10);   // 后继块的地址
        if (writeBlockToDisk(wblk, 400+start+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("写回磁盘: %d\n", 400+start+wnum);
        wnum++;
    }
    printf("索引文件占用%d块\n", wnum);

}


void indexBasedSelect(Buffer *buf, int val)
{
    printf("\n------------------------------\n");
    printf("基于索引的关系选择\n");
    printf("------------------------------\n");

    int istart = 417;   // 索引文件的起始块
    int tstart;         // 目标元组所在的起始块(已排序)
    unsigned char *rblk;    // 输入缓冲区
    unsigned char *wblk;    // 输出缓冲区
    int usage = 0;      // 输出缓冲区已写元组个数
    int wnum = 0;       // 已写块数
    int cnt = 0;        // 满足条件的元组个数
    BOOL find = FALSE;  // 是否在索引文件中找到目标索引字段
    BOOL end = FALSE;   // 搜索结束
    int C;
    int i, j;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);
    i = istart;
    // 在索引文件中搜索索引字段
    while(!find)
    {
        if ((rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("读入数据块%d\n", i);

        for (j = 0; j < 7; j++)
        {
            if(val == readNumFromBlk(rblk, j, 0))
            {
                tstart = readNumFromBlk(rblk, j, 1);
                find = TRUE;
                freeBlockInBuffer(rblk, buf);
                break;
            }
        }
        i++;
        freeBlockInBuffer(rblk, buf);
    }
    // 根据找到的索引字段的指针在主文件中搜索
    for(i = tstart; !end; i++)
    {
        if ((rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("读入数据块%d\n", i);

        for (j = 0; j < 7; j++)
        {
            C = readNumFromBlk(rblk, j, 0);
            if (C < val)
            {
                continue;
            }
            else if (C == val)
            {
                // 找到符合条件的元组，写入输出缓冲区
                memcpy(wblk + usage*8, rblk + j*8, 8);
                usage++;
                cnt++;
                // 判断输出缓冲块是否写满
                if(usage == 7)
                {
                    itoa(500+wnum+2, wblk + usage*8, 10);   // 后继块的地址
                    if (writeBlockToDisk(wblk, 500+1+wnum, buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("写回磁盘: %d\n", 500+1+wnum);
                    wblk = getNewBlockInBuffer(buf);
                    memset(wblk, 0, 64);
                    usage = 0;
                    wnum++;
                }
            }
            else
            {
                end = TRUE;
                break;
            }
        }
    }
    // 检查输出缓冲区是否有数据，若有，将其写回磁盘
    if (usage != 0)
    {
        itoa(500+wnum+2, wblk + 56, 10);   // 后继块的地址
        if (writeBlockToDisk(wblk, 500+1+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("写回磁盘: %d\n", 500+1+wnum);
        wnum++;
    }
    printf("\n满足条件的元组个数 %d\n", cnt);
    printf("IO读写一共%d次\n", buf->numIO);
}

void sortMergeJoin(Buffer *buf)
{
    printf("\n------------------------------\n");
    printf("基于排序的连接操作\n");
    printf("------------------------------\n");

    int s_start = 317, s_end = 348; // 排序后的关系S的起始块号和终止块号
    int r_start = 301, r_end = 316; // 排序后的关系R的起始块号和终止块号
    unsigned char *s_rblk;  // 关系S的输入缓冲区
    unsigned char *r_rblk;  // 关系R的输入缓冲区
    unsigned char *wblk;    // 输出缓冲区
    int r_left_bid = r_start, r_right_bid = r_start;    // 关系R左、右指针对应元组的块号
    int r_left_tid = 0, r_right_tid = 0;                // 关系R左、右指针对应元组的元组号
    int A, C;
    int last_val = 0;   // 上一个S.C的值
    int usage = 0;      // 输出缓冲区已写元组个数
    int wnum = 0;       // 已写块数
    int cnt = 0;        // 连接次数
    int i, j;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);

    // 从关系R读入一个数据块
    if ((r_rblk = readBlockFromDisk(r_right_bid, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }
//    printf("读入数据块%d\n", r_right_bid);
    for (i = s_start; i <= s_end; i++)
    {
        if ((s_rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
//        printf("读入数据块%d\n", i);

        for (j = 0; j < 7; j++)
        {
            C = readNumFromBlk(s_rblk, j, 0);
            // 如果S.C的值与上一个元组相同，将指向关系R的右指针进行回退
            if (C == last_val)
            {
                // 如果块号不同，需要读回前面的块
                if (r_left_bid != r_right_bid)
                {
                    freeBlockInBuffer(r_rblk, buf);
                    if ((r_rblk = readBlockFromDisk(r_left_bid, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
//                    printf("读入数据块%d\n", r_left_bid);
                    r_right_bid = r_left_bid;
                }
                r_right_tid = r_left_tid;
            }
            // S.C的值不同于上一个元组相同
            else
            {
                last_val = C;
                r_left_bid = r_right_bid;
                r_left_tid = r_right_tid;
            }

            // 从关系R中搜索R.A = S.C的元组
            if (r_right_bid > r_end)
            {
                A = 1 << 30;
            }
            else
            {
                A = readNumFromBlk(r_rblk, r_right_tid, 0);
            }

            while (A <= C)
            {
                if (A == C)
                {
                    cnt++;
                    // 将S.C,S.D写入输出缓冲区
                    memcpy(wblk + usage*8, s_rblk + j*8, 8);
                    usage++;
                    // 判断输出缓冲块是否写满
                    if(usage == 7)
                    {
                        itoa(600+wnum+2, wblk + 56, 10);   // 后继块的地址
                        if (writeBlockToDisk(wblk, 600+1+wnum, buf) != 0)
                        {
                            perror("Writing Block Failed!\n");
                            return -1;
                        }
                        printf("写回磁盘: %d\n", 600+1+wnum);
                        wblk = getNewBlockInBuffer(buf);
                        memset(wblk, 0, 64);
                        usage = 0;
                        wnum++;
                    }
                    // 将R.A,R.B写入输出缓冲区
                    memcpy(wblk + usage*8, r_rblk + r_right_tid*8, 8);
                    usage++;
                    // 判断输出缓冲块是否写满
                    if(usage == 7)
                    {
                        itoa(600+wnum+2, wblk + 56, 10);   // 后继块的地址
                        if (writeBlockToDisk(wblk, 600+1+wnum, buf) != 0)
                        {
                            perror("Writing Block Failed!\n");
                            return -1;
                        }
                        printf("写回磁盘: %d\n", 600+1+wnum);
                        wblk = getNewBlockInBuffer(buf);
                        memset(wblk, 0, 64);
                        usage = 0;
                        wnum++;
                    }
                }

                r_right_tid ++;
                if (r_right_tid == 7)
                {
                    r_right_bid++;
                    r_right_tid = 0;
                    // 当前数据块处理完毕，释放空间，读入下一个数据块
                    freeBlockInBuffer(r_rblk, buf);
                    if (r_right_bid > r_end)
                    {
                        A = 1 << 30;
                    }
                    else if ((r_rblk = readBlockFromDisk(r_right_bid, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
//                    printf("读入数据块%d\n", r_right_bid);
                }
                if (r_right_bid > r_end)
                {
                    A = 1 << 30;
                }
                else
                {
                    A = readNumFromBlk(r_rblk, r_right_tid, 0);
                }
            }
        }
        freeBlockInBuffer(s_rblk, buf);
    }
    // 检查输出缓冲区是否有数据，若有，将其写回磁盘
    if (usage != 0)
    {
        itoa(600+wnum+2, wblk + 56, 10);   // 后继块的地址
        if (writeBlockToDisk(wblk, 600+1+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("写回磁盘: %d\n", 600+1+wnum);
        wnum++;
    }
    printf("写磁盘次数: %d\n", wnum);
    printf("\n总共连接%d次\n", cnt);
}

void sortBasedDifference(Buffer *buf)
{
    printf("\n------------------------------\n");
    printf("基于排序的集合差操作(S-R)\n");
    printf("------------------------------\n");

    int s_start = 317, s_end = 348; // 排序后的关系S的起始块号和终止块号
    int r_start = 301, r_end = 316; // 排序后的关系R的起始块号和终止块号
    unsigned char *s_rblk;  // 关系S的输入缓冲区
    unsigned char *r_rblk;  // 关系R的输入缓冲区
    unsigned char *wblk;    // 输出缓冲区
    int r_left_bid = r_start, r_right_bid = r_start;    // 关系R左、右指针对应元组的块号
    int r_left_tid = 0, r_right_tid = 0;                // 关系R左、右指针对应元组的元组号
    int A, B, C, D;
    int last_val = 0;   // 上一个S.C的值
    int usage = 0;      // 输出缓冲区已写元组个数
    int wnum = 0;       // 已写块数
    BOOL valid;         // 元组是否有效
    int cnt = 0;        // 差集个数
    int i, j;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);

    // 从关系R读入一个数据块
    if ((r_rblk = readBlockFromDisk(r_right_bid, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }
    // printf("读入数据块%d\n", r_right_bid);

    // 遍历关系S，找出符合条件的元组
    for (i = s_start; i <= s_end; i++)
    {
        if ((s_rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        // printf("读入数据块%d\n", i);

        for (j = 0; j < 7; j++)
        {
            valid = TRUE;
            C = readNumFromBlk(s_rblk, j, 0);
            D = readNumFromBlk(s_rblk, j, 1);
            // 如果S.C的值与上一个元组相同，将指向关系R的右指针进行回退
            if (C == last_val)
            {
                // 如果块号不同，需要读回前面的块
                if (r_left_bid != r_right_bid)
                {
                    freeBlockInBuffer(r_rblk, buf);
                    if ((r_rblk = readBlockFromDisk(r_left_bid, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
                    // printf("读入数据块%d\n", r_left_bid);
                    r_right_bid = r_left_bid;
                }
                r_right_tid = r_left_tid;
            }
            // S.C的值不同于上一个元组相同
            else
            {
                last_val = C;
                r_left_bid = r_right_bid;
                r_left_tid = r_right_tid;
            }

            // 从关系R中搜索R.A = S.C的元组
            if (r_right_bid > r_end)
            {
                A = 1 << 30;
                B = 1 << 30;
            }
            else
            {
                A = readNumFromBlk(r_rblk, r_right_tid, 0);
                B = readNumFromBlk(r_rblk, r_right_tid, 1);
            }
            // 在关系R中找到R.A = S.C的元组
            // 如果R.B = S.D也成立，则需将这个S中的元组去掉
            while (A <= C)
            {
                if (A == C && B == D)
                {
                    valid = FALSE;
                }

                r_right_tid++;
                if (r_right_tid == 7)
                {
                    r_right_bid++;
                    r_right_tid = 0;
                    // 当前数据块处理完毕，释放空间，读入下一个数据块
                    freeBlockInBuffer(r_rblk, buf);
                    if (r_right_bid > r_end)
                    {
                        A = 1 << 30;
                    }
                    else if ((r_rblk = readBlockFromDisk(r_right_bid, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
                    // printf("读入数据块%d\n", r_right_bid);
                }
                if (r_right_bid > r_end)
                {
                    A = 1 << 30;
                    B = 1 << 30;
                }
                else
                {
                    A = readNumFromBlk(r_rblk, r_right_tid, 0);
                    B = readNumFromBlk(r_rblk, r_right_tid, 1);
                }
            }
            // 将符合条件的原则写入磁盘
            if(valid)
            {
                cnt++;
                memcpy(wblk + usage*8, s_rblk + j*8, 8);
                usage++;
                // 判断输出缓冲块是否写满
                if(usage == 7)
                {
                    itoa(1002+wnum, wblk + 56, 10);   // 后继块的地址
                    if (writeBlockToDisk(wblk, 1001+wnum, buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("写回磁盘: %d\n", 1001+wnum);
                    wblk = getNewBlockInBuffer(buf);
                    memset(wblk, 0, 64);
                    usage = 0;
                    wnum++;
                }
            }
        }
        freeBlockInBuffer(s_rblk, buf);
    }
    // 检查输出缓冲区是否有数据，若有，将其写回磁盘
    if (usage != 0)
    {
        itoa(1002+wnum, wblk + 56, 10);   // 后继块的地址
        if (writeBlockToDisk(wblk, 1001+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        memset(wblk, 0, 64);
        printf("写回磁盘: %d\n", 1001+wnum);
        wnum++;
    }
    printf("写磁盘次数: %d\n", wnum);
    printf("\nS和R的差集(S-R)有%d个元组\n", cnt);
}

void sortBasedIntersection(Buffer *buf)
{
    printf("\n------------------------------\n");
    printf("基于排序的集合交操作(S∩R)\n");
    printf("------------------------------\n");

    int s_start = 317, s_end = 348; // 排序后的关系S的起始块号和终止块号
    int r_start = 301, r_end = 316; // 排序后的关系R的起始块号和终止块号
    unsigned char *s_rblk;  // 关系S的输入缓冲区
    unsigned char *r_rblk;  // 关系R的输入缓冲区
    unsigned char *wblk;    // 输出缓冲区
    int r_left_bid = r_start, r_right_bid = r_start;    // 关系R左、右指针对应元组的块号
    int r_left_tid = 0, r_right_tid = 0;                // 关系R左、右指针对应元组的元组号
    int A, B, C, D;
    int last_val = 0;   // 上一个S.C的值
    int usage = 0;      // 输出缓冲区已写元组个数
    int wnum = 0;       // 已写块数
    BOOL valid;         // 元组是否有效
    int cnt = 0;        // 差集个数
    int i, j;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);

    // 从关系R读入一个数据块
    if ((r_rblk = readBlockFromDisk(r_right_bid, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }

    // 遍历关系S，找出符合条件的元组
    for (i = s_start; i <= s_end; i++)
    {
        if ((s_rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }

        for (j = 0; j < 7; j++)
        {
            valid = FALSE;
            C = readNumFromBlk(s_rblk, j, 0);
            D = readNumFromBlk(s_rblk, j, 1);
            // 如果S.C的值与上一个元组相同，将指向关系R的右指针进行回退
            if (C == last_val)
            {
                // 如果块号不同，需要读回前面的块
                if (r_left_bid != r_right_bid)
                {
                    freeBlockInBuffer(r_rblk, buf);
                    if ((r_rblk = readBlockFromDisk(r_left_bid, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
                    r_right_bid = r_left_bid;
                }
                r_right_tid = r_left_tid;
            }
            // S.C的值不同于上一个元组相同
            else
            {
                last_val = C;
                r_left_bid = r_right_bid;
                r_left_tid = r_right_tid;
            }

            // 从关系R中搜索R.A = S.C的元组
            if (r_right_bid > r_end)
            {
                A = 1 << 30;
                B = 1 << 30;
            }
            else
            {
                A = readNumFromBlk(r_rblk, r_right_tid, 0);
                B = readNumFromBlk(r_rblk, r_right_tid, 1);
            }
            // 在关系R中找到R.A = S.C的元组
            // 如果R.B = S.D也成立，则需将这个S中的元组设为有效
            while (A <= C)
            {
                if (A == C && B == D)
                {
                    valid = TRUE;
                }

                r_right_tid++;
                if (r_right_tid == 7)
                {
                    r_right_bid++;
                    r_right_tid = 0;
                    // 当前数据块处理完毕，释放空间，读入下一个数据块
                    freeBlockInBuffer(r_rblk, buf);
                    if (r_right_bid > r_end)
                    {
                        A = 1 << 30;
                    }
                    else if ((r_rblk = readBlockFromDisk(r_right_bid, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
                }
                if (r_right_bid > r_end)
                {
                    A = 1 << 30;
                    B = 1 << 30;
                }
                else
                {
                    A = readNumFromBlk(r_rblk, r_right_tid, 0);
                    B = readNumFromBlk(r_rblk, r_right_tid, 1);
                }
            }
            // 将符合条件的原则写入磁盘
            if(valid)
            {
                cnt++;
                memcpy(wblk + usage*8, s_rblk + j*8, 8);
                usage++;
                // 判断输出缓冲块是否写满
                if(usage == 7)
                {
                    itoa(2002+wnum, wblk + 56, 10);   // 后继块的地址
                    if (writeBlockToDisk(wblk, 2001+wnum, buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("写回磁盘: %d\n", 2001+wnum);
                    wblk = getNewBlockInBuffer(buf);
                    memset(wblk, 0, 64);
                    usage = 0;
                    wnum++;
                }
            }
        }
        freeBlockInBuffer(s_rblk, buf);
    }
    // 检查输出缓冲区是否有数据，若有，将其写回磁盘
    if (usage != 0)
    {
        itoa(2002+wnum, wblk + 56, 10);   // 后继块的地址
        if (writeBlockToDisk(wblk, 2001+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        memset(wblk, 0, 64);
        printf("写回磁盘: %d\n", 2001+wnum);
        wnum++;
    }
    printf("写磁盘次数: %d\n", wnum);
    printf("\nS和R的交集(S∩R)有%d个元组\n", cnt);
}

void sortBasedUnion(Buffer *buf)
{
    printf("\n------------------------------\n");
    printf("基于排序的集合并(R∪S)\n");
    printf("------------------------------\n");

    int start[2] = {301, 317};  // 排序后的关系R和S的起始块号
    int end[2] = {316, 348};    // 排序后的关系R和S的终止块号
    unsigned char *rblks[2];   // rblks[0]为R的输入缓冲区，rblks[1]为S的输入缓冲区
    int bid[2];                 // 关系输入内存的块号
    int tid[2];                 // 输入块中进行下一次比较的元组号[0,6]
    int r_left_bid = start[0], r_right_bid = start[1];  // 关系R左、右指针对应元组的块号
    int r_left_tid = 0, r_right_tid = 0;                // 关系R左、右指针对应元组的元组号
    unsigned char *temp_rblk;  // 临时输入块，用于去重
    unsigned char *wblk;       // 输出缓冲区
    BOOL finish[2];
    int A, B, last_A, last_B;
    int last_X = -1;    // 上一个被选择的元组的第一个属性的值
    int last_Y = -1;    // 上一个被选择的元组的第二个属性的值
    int Y;
    int usage = 0;      // 输出缓冲区已写元组个数
    int wnum = 0;       // 已写块数
    int cnt = 0;        // 并操作后的元组个数
    int small, small_id;
    BOOL valid = FALSE;
    BOOL nodup = FALSE;
    int i, j, k;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);

    // 读入每个子集合的第一块
    for (i = 0; i < 2; i++)
    {
        bid[i] = start[i];
        tid[i] = 0;
        finish[i] = FALSE;
        if ((rblks[i] = readBlockFromDisk(bid[i], buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        // printf("读入数据块%d\n", bid[i]);
    }
    A = readNumFromBlk(rblks[0], 0, 0);
    B = readNumFromBlk(rblks[0], 0, 1);
    last_A = A;
    last_B = B;

    while (arrSum(finish, 2) < 2)
    {
        small = 1 << 30;
        small_id = -1;  // 0表示关系R，1表示关系S
        nodup = FALSE;
        // 找到两路中的较小者
        for (i = 0; i < 2; i++)
        {
            if (finish[i] == TRUE)
            {
                // 该关系全部处理完毕
                continue;
            }
            else if (readNumFromBlk(rblks[i], tid[i], 0) < small)
            {
                small = readNumFromBlk(rblks[i], tid[i], 0);
                small_id = i;
            }
        }
        if (small_id == -1)
        {
            perror("MergeSort Error!\n");
            return -1;
        }

        Y = readNumFromBlk(rblks[small_id], tid[small_id], 1);
        if (small_id == 0)
        {
            // 出现新的A值，移动左指针到第一个新的A值
            if (A != last_A)
            {
                r_left_bid = r_right_bid;
                r_left_tid = r_right_tid;
            }
            last_A = A;
        }
        // 去重
        if (small_id == 1)
        {
            if (small == last_A)
            {
                // printf("去重1：X = %d, Y = %d\n", small, Y);
                // printf("r_left_bid: %d, r_left_tid: %d, r_right_bid: %d, r_left_tid: %d\n", r_left_bid, r_left_tid, r_right_bid, r_right_tid);
                for (j = r_left_bid; j <= r_right_bid && !nodup; j++)
                {
                    // printf("去重2：X = %d, Y = %d\n", small, Y);
                    if ((temp_rblk = readBlockFromDisk(j, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
                    for (k = 0; k < 7; k++)
                    {
                        // printf("去重3：X = %d, Y = %d ", small, Y);
                        if(small == readNumFromBlk(temp_rblk, k, 0) && Y == readNumFromBlk(temp_rblk, k, 1))
                        {
                            valid = FALSE;
                            nodup = TRUE;
                            break;
                        }
                        else
                        {
                            valid = TRUE;
                        }
                    }
                    freeBlockInBuffer(temp_rblk, buf);
                }
            }
            else
            {
                valid = TRUE;
            }
        }
        else
        {
            valid = TRUE;
        }
        last_X = small;
        last_Y = Y;

        // 如果当前元组有效，将其写入输出缓冲块
        if (valid)
        {
            memcpy(wblk + usage*8, rblks[small_id] + tid[small_id]*8, 8);
            // printf("(X = %d, Y = %d)\n", last_X, last_Y);
            usage++;
            cnt++;
            if(usage == 7)
            {
                itoa(3002+wnum, wblk + 56, 10);   // 后继块的地址
                if (writeBlockToDisk(wblk, 3001+wnum, buf) != 0)
                {
                    perror("Writing Block Failed!\n");
                    return -1;
                }
                printf("写回磁盘: %d\n", 3001+wnum);
                wblk = getNewBlockInBuffer(buf);
                memset(wblk, 0, 64);
                usage = 0;
                wnum++;
            }
        }

        tid[small_id]++;    // 处理完一个元组后取下一个
        if(tid[small_id] == 7)
        {
            // 读入数据块中的元组全部处理完毕
            bid[small_id]++;
            // 判断该子集合是否全部处理完毕
            if (bid[small_id] > end[small_id])
            {
                finish[small_id] = TRUE;
                // printf("第%d个关系处理完毕\n", small_id);
            }
            else
            {
                freeBlockInBuffer(rblks[small_id], buf);
                // 输入对应子集合的下一个磁盘块
                if ((rblks[small_id] = readBlockFromDisk(bid[small_id], buf)) == NULL)
                {
                    perror("Reading Block Failed!\n");
                    return -1;
                }
                // printf("读入数据块: %d\n", bid[small_id]);
                tid[small_id] = 0;
            }
        }
        if (small_id == 0)
        {
            r_right_bid = bid[0];
            r_right_tid = tid[0];
            if (!finish[0])
            {
                A = readNumFromBlk(rblks[0], tid[0], 0);
            }
            else
            {
                A = 1 << 30;
            }
        }
    }
    // 检查输出缓冲区是否有数据，若有，将其写回磁盘
    if (usage != 0)
    {
        itoa(3002+wnum, wblk + 56, 10);   // 后继块的地址
        if (writeBlockToDisk(wblk, 3001+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("写回磁盘: %d\n", 3001+wnum);
        wnum++;
    }
    freeBuffer(buf);
    printf("写磁盘次数: %d\n", wnum);
    printf("\nR和S的并集共有%d个元组\n", cnt);
}

