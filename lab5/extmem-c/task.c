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
 * �Ӵ��̿��ȡָ��Ԫ���ĳ������ת����int���ͺ󷵻�
 *
 * @param blk       ����ȡ�Ĵ��̿�
 * @param tid       Ԫ���
 * @param aid       ���Ժţ�0��ʾ��һ�����ԣ�1��ʾ�ڶ�������
*/
int readNumFromBlk(unsigned char *blk, int tid, int aid);

/**
 * @brief
 * ʵ�ֻ������������Ĺ�ϵѡ���㷨��
 * ����ExtMem����⣬ʹ��C����ʵ�����������㷨
 * ѡ��S.C=128��Ԫ�飬��¼IO��д����������ѡ��������ڴ��̿�101,102,......��
 *��ģ��ʵ�� select S.C, S.D from S where S.C = 128��
 *
 * @param buf ������
 * @param val ����ѡ���ֵ
*/
void lnearSearch(Buffer *buf, int val);

/**
 * @brief
 * TPMMS�ĵ�һ�ˣ������Ӽ��ϲ������Ӽ��������Ӽ��Ϻ�8�����̿�
 * �м�������200+ԭ�����̿�ŵ��ڴ���
 *
 * @param buf   �ڴ滺����
 * @param start ������ȫ������ʼ���
 * @param end   ������ȫ������ֹ���
*/
void divideAndSort(Buffer *buf, int start, int end);

/**
 * @brief �������
*/
int arrSum(int array[], int size);

/**
 * @brief
 * TPMMS�ĵڶ��ˣ����Ӽ���Ĺ鲢����
 * �������300+ԭ�����̺���
 *
 * @param buf   �ڴ滺����
 * @param start �������Ӽ�����ʼ���
 * @param end   �������Ӽ�����ֹ���
*/
void mergeSort(Buffer *buf, int start, int end);

/**
 * @brief
 * ���׶ζ�·�鲢�����㷨
 *
 * @param buf �ڴ滺����
 * @param relation ��Ҫ��������Ĺ�ϵ
*/
void TPMMS(Buffer *buf, char relation);

/**
 * @brief
 * ������ϵS�������ļ������д����Ϊ400+��ŵĴ�����
 *
 * @param buf �ڴ滺����
 *
*/
void creatIndexOfS(Buffer *buf);

/**
 * @brief
 * ���������Ĺ�ϵѡ���㷨�����д��501.blk,502.blk,......
 *
 * @param buf �ڴ滺����
 * @param val ����ѡ���ֵ
*/
void indexBasedSelect(Buffer *buf, int val);

/**
 * @brief
 * ʵ�ֻ�����������Ӳ����㷨��Sort-Merge-Join��
 * �Թ�ϵS��R����S.C����R.A ����ͳ�����Ӵ����������ӽ������ڴ���601.blk,602.blk,......
 * ��ģ��ʵ�� select S.C, S.D, R.A, R.B from S innerjoin R on S.C = R.A��
 *
 * @param buf �ڴ滺����
*/
void sortMergeJoin(Buffer *buf);

/**
 * @brief
 * �������������ɨ���㷨��ʵ�ּ��ϵĲ����
 * ����������1001.blk, 1002.blk, ......
 *
 * @param buf �ڴ滺����
*/
void sortBasedDifference(Buffer *buf);

/**
 * @brief
 * �������������ɨ���㷨��ʵ�ּ��ϵĽ�����
 * ����������2001.blk, 2002.blk, ......
 *
 * @param buf �ڴ滺����
*/
void sortBasedIntersection(Buffer *buf);

/**
 * @brief
 * �������������ɨ���㷨��ʵ�ּ��ϵĲ�����
 * ����������1001.blk, 1002.blk, ......
 *
 * @param buf �ڴ滺����
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
    unsigned char *rblk;   // ���뻺���
    unsigned char *wblk;   // ��������
    int usage = 0;          // �����������ÿռ�
    int wnum = 0;           // д���̵Ĵ���
    int S_start = 17;       // ��ϵS����ʼ��
    int S_end = 48;         // ��ϵS����ֹ��
    int i, j, k;
    int C = -1;
    int D = -1;
    char str[5];
    int cnt = 0;            // ����������Ԫ�����

    printf("------------------------------\n");
    printf("�������������Ĺ�ϵѡ���㷨 S.C = 128\n");
    printf("------------------------------\n");

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    rblk = getNewBlockInBuffer(buf);
    wblk = getNewBlockInBuffer(buf);

    // ���ζ�ȡ�������ݿ�
    for (i = S_start; i <= S_end; i++)
    {
        if ((rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("�������ݿ�%d\n", i);

        // �����ݿ��е�7��Ԫ��
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
            // ��ѡ����������������
            memcpy(wblk + usage*8, rblk + j*8, 8);
            usage++;
            // ��������д����д�ش���
            if(usage == 7)
            {
                wnum++;
                itoa(100+wnum+1, wblk + usage*8, 10);   // ��̿�ĵ�ַ
                if (writeBlockToDisk(wblk, 100+wnum, buf) != 0)
                {
                    perror("Writing Block Failed!\n");
                    return -1;
                }
                printf("д����̿�: %d\n", 100+wnum);
                wblk = getNewBlockInBuffer(buf);
                memset(wblk, 0, 64);
                usage = 0;
            }
        }
        // �ͷſռ�
        freeBlockInBuffer(rblk, buf);
    }
    if (usage != 0)
    {
        wnum++;
        itoa(100+wnum+1, wblk + usage*8, 10);   // ��̿�ĵ�ַ
        if (writeBlockToDisk(wblk, 100+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("д����̿�: %d\n", 100+wnum);
    }
    printf("\n����������Ԫ����� %d\n", cnt);
    printf("IO��дһ��%d��\n", buf->numIO);
}

void divideAndSort(Buffer *buf, int start, int end)
{
    printf("----Step1.�����Ӽ�&�Ӽ�����---\n");

    int outer, inner;
    int i, j;
    int bid, tid;       // ð�����������Ԫ�����ڵĿ�š�Ԫ���
    int bid1, tid1;     // ��һ��Ԫ�صĿ�š�Ԫ���
    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    unsigned char *blks[buf->numAllBlk];    // �ڴ����п����ڴ����Ӽ���
    char temp[10];

    // ÿ�δ���һ���Ӽ���(8��)
    for (outer = start; outer <= end; outer += 8)
    {
        // �Ӽ���ȫ�������ڴ�
        for (inner = 0; inner < 8; inner++)
        {
            if ((blks[inner] = readBlockFromDisk(outer + inner, buf)) == NULL) {
                perror("Reading Block Failed!\n");
                return -1;
            }
            printf("�������ݿ�%d\n", outer+inner);
        }

        // ʹ��ð�����򷨶��Ӽ�������������Ϊ��һ������(A��C)
        // �Ӽ��Ϲ�56��Ԫ��
//        printf("����ð������\n");
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
                    // ���н���
                    memcpy(temp, blks[bid] + tid*8, 8); // temp��ָ��ʱ�����
                    memcpy(blks[bid] + tid*8, blks[bid1] + tid1*8, 8);
                    memcpy(blks[bid1] + tid1*8, temp, 8);
                }
            }
        }
        // printf("�������\n");
        // д�ش���200+ԭ�����ڿ��
        for(inner = 0; inner < 8; inner++)
        {
            itoa(200+outer+inner+1, blks[inner]+56, 10);   // ��̿�ĵ�ַ
            if (writeBlockToDisk(blks[inner], 200 + outer + inner, buf) != 0)
            {
                perror("Writing Block Failed!\n");
                return -1;
            }
            memset(blks[inner], 0, 64);
            printf("д�ش���%d\n", 200+outer+inner);
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
    printf("\n--------Step2.�鲢����--------\n");

    int sub_num = (end - start + 1) / 8;            // �Ӽ��ϸ���
    // if (!initBuffer(520, 64, buf))
    // {
    //     perror("Buffer Initialization Failed!\n");
    //     return -1;
    // }
    unsigned char *rblks[sub_num];                 // ���뻺�������
    int bid[sub_num];                               // �Ӽ��ϵ�ǰ�����ڴ�Ŀ��[0,7]
    int tid[sub_num];                               // ������н�����һ�αȽϵ�Ԫ���[0,6]
    BOOL finish[sub_num];                           // �Ӽ��ϴ������
    unsigned char *wblk = getNewBlockInBuffer(buf);// ��������
    int usage = 0;                                  // ����������װ��Ԫ����[0,7]
    int i;
    int small;          // �Ӽ����н��бȽϵ�Ԫ�ص���Сֵ
    int small_id;       // small��Ӧ���Ӽ��Ϻ�
    int wnum = 0;       // ��д����

    // ����ÿ���Ӽ��ϵĵ�һ��
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
        printf("�������ݿ�%d\n", start + i*8);
    }

    while(arrSum(finish, sub_num) < sub_num)
    {
        small = 1 << 30;
        small_id = -1;
        // �ҵ���ǰ�鲢��sub_num·Ԫ�����Сֵ
        for (i = 0; i < sub_num; i++)
        {
            if (finish[i] == TRUE)
            {
                // ����Ӽ����Ѿ�ȫ���������
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
        // �ж����������Ƿ�д��
        if(usage == 7)
        {
            itoa(100+start+wnum+1, wblk + usage*8, 10);   // ��̿�ĵ�ַ
            if (writeBlockToDisk(wblk, 100+start+wnum, buf) != 0)
            {
                perror("Writing Block Failed!\n");
                return -1;
            }
            printf("д�ش���: %d\n", 100+start+wnum);
            wblk = getNewBlockInBuffer(buf);
            memset(wblk, 0, 64);
            usage = 0;
            wnum++;
        }

        tid[small_id]++;    // ������һ��Ԫ�飬�����һλ
        // �жϸ��Ӽ�������Ĵ��̿��Ƿ�ȫ��������
        if(tid[small_id] == 7)
        {
            bid[small_id]++;
            // �жϸ��Ӽ����Ƿ�ȫ���������
            if (bid[small_id] == 8)
            {
                finish[small_id] = TRUE;
            }
            else
            {
                freeBlockInBuffer(rblks[small_id], buf);
                // �����Ӧ�Ӽ��ϵ���һ�����̿�
                if ((rblks[small_id] = readBlockFromDisk(start+small_id*8+bid[small_id], buf)) == NULL)
                {
                    perror("Reading Block Failed!\n");
                    return -1;
                }
                printf("�������ݿ�: %d\n", start+small_id*8+bid[small_id]);
                tid[small_id] = 0;
            }
        }
    }

    // �ͷ��ڴ�ռ�
    freeBuffer(buf);
}

void TPMMS(Buffer *buf, char relation)
{
    int start, end;
    printf("\n------------------------------\n");
    printf("���׶ζ�·�鲢����\n");
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
    printf("���������ļ�\n");
    printf("------------------------------\n");

    int start = 17;
    int end = 48;
    unsigned char *rblk;    // ���뻺����
    unsigned char *wblk;    // ���������
    int usage = 0;          // �����������дԪ�����
    int wnum = 0;           // ��д����
    int idx_field = 0;      // �����ֶ�
    char pointer[4];        // ָ�������ı��а��������ֶεļ�¼���ڴ��̿��
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
        // ����������ļ��ж�ȡһ�����ݿ�
        if ((rblk = readBlockFromDisk(300+i, buf)) == NULL)
        {
            perror("Reading Sorted Block Failed!\n");
            return -1;
        }
        printf("�������ݿ�%d\n", 300 + i);
        for (j = 0; j < 7; j++)
        {
            if(idx_field != readNumFromBlk(rblk, j, 0))
            {
                // �����µ������ֶ�
//                printf("idx: %d, new: %d, j: %d\n", idx_field, readNumFromBlk(rblk, j, 0), j);
                idx_field = readNumFromBlk(rblk, j, 0);
                itoa(300+i, pointer, 10);

                memcpy(wblk + usage*8, rblk + j*8, 4);      // �������ֶ�д�����������
                memcpy(wblk + usage*8 + 4, pointer, 4);     // ��ָ��д�����������
                memset(pointer, 0, 4);
                usage++;
                // �ж����������Ƿ�д��
                if(usage == 7)
                {
                    itoa(400+start+wnum+1, wblk + usage*8, 10);   // ��̿�ĵ�ַ
                    if (writeBlockToDisk(wblk, 400+start+wnum, buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("д�ش���: %d\n", 400+start+wnum);
                    wblk = getNewBlockInBuffer(buf);
                    memset(wblk, 0, 64);
                    usage = 0;
                    wnum++;
                }
            }
        }
        freeBlockInBuffer(rblk, buf);
    }
    // �������������Ƿ������ݣ����У�����д�ش���
    if (usage != 0)
    {
        itoa(400+start+wnum+1, wblk + 56, 10);   // ��̿�ĵ�ַ
        if (writeBlockToDisk(wblk, 400+start+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("д�ش���: %d\n", 400+start+wnum);
        wnum++;
    }
    printf("�����ļ�ռ��%d��\n", wnum);

}


void indexBasedSelect(Buffer *buf, int val)
{
    printf("\n------------------------------\n");
    printf("���������Ĺ�ϵѡ��\n");
    printf("------------------------------\n");

    int istart = 417;   // �����ļ�����ʼ��
    int tstart;         // Ŀ��Ԫ�����ڵ���ʼ��(������)
    unsigned char *rblk;    // ���뻺����
    unsigned char *wblk;    // ���������
    int usage = 0;      // �����������дԪ�����
    int wnum = 0;       // ��д����
    int cnt = 0;        // ����������Ԫ�����
    BOOL find = FALSE;  // �Ƿ��������ļ����ҵ�Ŀ�������ֶ�
    BOOL end = FALSE;   // ��������
    int C;
    int i, j;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);
    i = istart;
    // �������ļ������������ֶ�
    while(!find)
    {
        if ((rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("�������ݿ�%d\n", i);

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
    // �����ҵ��������ֶε�ָ�������ļ�������
    for(i = tstart; !end; i++)
    {
        if ((rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("�������ݿ�%d\n", i);

        for (j = 0; j < 7; j++)
        {
            C = readNumFromBlk(rblk, j, 0);
            if (C < val)
            {
                continue;
            }
            else if (C == val)
            {
                // �ҵ�����������Ԫ�飬д�����������
                memcpy(wblk + usage*8, rblk + j*8, 8);
                usage++;
                cnt++;
                // �ж����������Ƿ�д��
                if(usage == 7)
                {
                    itoa(500+wnum+2, wblk + usage*8, 10);   // ��̿�ĵ�ַ
                    if (writeBlockToDisk(wblk, 500+1+wnum, buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("д�ش���: %d\n", 500+1+wnum);
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
    // �������������Ƿ������ݣ����У�����д�ش���
    if (usage != 0)
    {
        itoa(500+wnum+2, wblk + 56, 10);   // ��̿�ĵ�ַ
        if (writeBlockToDisk(wblk, 500+1+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("д�ش���: %d\n", 500+1+wnum);
        wnum++;
    }
    printf("\n����������Ԫ����� %d\n", cnt);
    printf("IO��дһ��%d��\n", buf->numIO);
}

void sortMergeJoin(Buffer *buf)
{
    printf("\n------------------------------\n");
    printf("������������Ӳ���\n");
    printf("------------------------------\n");

    int s_start = 317, s_end = 348; // �����Ĺ�ϵS����ʼ��ź���ֹ���
    int r_start = 301, r_end = 316; // �����Ĺ�ϵR����ʼ��ź���ֹ���
    unsigned char *s_rblk;  // ��ϵS�����뻺����
    unsigned char *r_rblk;  // ��ϵR�����뻺����
    unsigned char *wblk;    // ���������
    int r_left_bid = r_start, r_right_bid = r_start;    // ��ϵR����ָ���ӦԪ��Ŀ��
    int r_left_tid = 0, r_right_tid = 0;                // ��ϵR����ָ���ӦԪ���Ԫ���
    int A, C;
    int last_val = 0;   // ��һ��S.C��ֵ
    int usage = 0;      // �����������дԪ�����
    int wnum = 0;       // ��д����
    int cnt = 0;        // ���Ӵ���
    int i, j;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);

    // �ӹ�ϵR����һ�����ݿ�
    if ((r_rblk = readBlockFromDisk(r_right_bid, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }
//    printf("�������ݿ�%d\n", r_right_bid);
    for (i = s_start; i <= s_end; i++)
    {
        if ((s_rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
//        printf("�������ݿ�%d\n", i);

        for (j = 0; j < 7; j++)
        {
            C = readNumFromBlk(s_rblk, j, 0);
            // ���S.C��ֵ����һ��Ԫ����ͬ����ָ���ϵR����ָ����л���
            if (C == last_val)
            {
                // �����Ų�ͬ����Ҫ����ǰ��Ŀ�
                if (r_left_bid != r_right_bid)
                {
                    freeBlockInBuffer(r_rblk, buf);
                    if ((r_rblk = readBlockFromDisk(r_left_bid, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
//                    printf("�������ݿ�%d\n", r_left_bid);
                    r_right_bid = r_left_bid;
                }
                r_right_tid = r_left_tid;
            }
            // S.C��ֵ��ͬ����һ��Ԫ����ͬ
            else
            {
                last_val = C;
                r_left_bid = r_right_bid;
                r_left_tid = r_right_tid;
            }

            // �ӹ�ϵR������R.A = S.C��Ԫ��
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
                    // ��S.C,S.Dд�����������
                    memcpy(wblk + usage*8, s_rblk + j*8, 8);
                    usage++;
                    // �ж����������Ƿ�д��
                    if(usage == 7)
                    {
                        itoa(600+wnum+2, wblk + 56, 10);   // ��̿�ĵ�ַ
                        if (writeBlockToDisk(wblk, 600+1+wnum, buf) != 0)
                        {
                            perror("Writing Block Failed!\n");
                            return -1;
                        }
                        printf("д�ش���: %d\n", 600+1+wnum);
                        wblk = getNewBlockInBuffer(buf);
                        memset(wblk, 0, 64);
                        usage = 0;
                        wnum++;
                    }
                    // ��R.A,R.Bд�����������
                    memcpy(wblk + usage*8, r_rblk + r_right_tid*8, 8);
                    usage++;
                    // �ж����������Ƿ�д��
                    if(usage == 7)
                    {
                        itoa(600+wnum+2, wblk + 56, 10);   // ��̿�ĵ�ַ
                        if (writeBlockToDisk(wblk, 600+1+wnum, buf) != 0)
                        {
                            perror("Writing Block Failed!\n");
                            return -1;
                        }
                        printf("д�ش���: %d\n", 600+1+wnum);
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
                    // ��ǰ���ݿ鴦����ϣ��ͷſռ䣬������һ�����ݿ�
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
//                    printf("�������ݿ�%d\n", r_right_bid);
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
    // �������������Ƿ������ݣ����У�����д�ش���
    if (usage != 0)
    {
        itoa(600+wnum+2, wblk + 56, 10);   // ��̿�ĵ�ַ
        if (writeBlockToDisk(wblk, 600+1+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("д�ش���: %d\n", 600+1+wnum);
        wnum++;
    }
    printf("д���̴���: %d\n", wnum);
    printf("\n�ܹ�����%d��\n", cnt);
}

void sortBasedDifference(Buffer *buf)
{
    printf("\n------------------------------\n");
    printf("��������ļ��ϲ����(S-R)\n");
    printf("------------------------------\n");

    int s_start = 317, s_end = 348; // �����Ĺ�ϵS����ʼ��ź���ֹ���
    int r_start = 301, r_end = 316; // �����Ĺ�ϵR����ʼ��ź���ֹ���
    unsigned char *s_rblk;  // ��ϵS�����뻺����
    unsigned char *r_rblk;  // ��ϵR�����뻺����
    unsigned char *wblk;    // ���������
    int r_left_bid = r_start, r_right_bid = r_start;    // ��ϵR����ָ���ӦԪ��Ŀ��
    int r_left_tid = 0, r_right_tid = 0;                // ��ϵR����ָ���ӦԪ���Ԫ���
    int A, B, C, D;
    int last_val = 0;   // ��һ��S.C��ֵ
    int usage = 0;      // �����������дԪ�����
    int wnum = 0;       // ��д����
    BOOL valid;         // Ԫ���Ƿ���Ч
    int cnt = 0;        // �����
    int i, j;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);

    // �ӹ�ϵR����һ�����ݿ�
    if ((r_rblk = readBlockFromDisk(r_right_bid, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }
    // printf("�������ݿ�%d\n", r_right_bid);

    // ������ϵS���ҳ�����������Ԫ��
    for (i = s_start; i <= s_end; i++)
    {
        if ((s_rblk = readBlockFromDisk(i, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        // printf("�������ݿ�%d\n", i);

        for (j = 0; j < 7; j++)
        {
            valid = TRUE;
            C = readNumFromBlk(s_rblk, j, 0);
            D = readNumFromBlk(s_rblk, j, 1);
            // ���S.C��ֵ����һ��Ԫ����ͬ����ָ���ϵR����ָ����л���
            if (C == last_val)
            {
                // �����Ų�ͬ����Ҫ����ǰ��Ŀ�
                if (r_left_bid != r_right_bid)
                {
                    freeBlockInBuffer(r_rblk, buf);
                    if ((r_rblk = readBlockFromDisk(r_left_bid, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
                    // printf("�������ݿ�%d\n", r_left_bid);
                    r_right_bid = r_left_bid;
                }
                r_right_tid = r_left_tid;
            }
            // S.C��ֵ��ͬ����һ��Ԫ����ͬ
            else
            {
                last_val = C;
                r_left_bid = r_right_bid;
                r_left_tid = r_right_tid;
            }

            // �ӹ�ϵR������R.A = S.C��Ԫ��
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
            // �ڹ�ϵR���ҵ�R.A = S.C��Ԫ��
            // ���R.B = S.DҲ���������轫���S�е�Ԫ��ȥ��
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
                    // ��ǰ���ݿ鴦����ϣ��ͷſռ䣬������һ�����ݿ�
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
                    // printf("�������ݿ�%d\n", r_right_bid);
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
            // ������������ԭ��д�����
            if(valid)
            {
                cnt++;
                memcpy(wblk + usage*8, s_rblk + j*8, 8);
                usage++;
                // �ж����������Ƿ�д��
                if(usage == 7)
                {
                    itoa(1002+wnum, wblk + 56, 10);   // ��̿�ĵ�ַ
                    if (writeBlockToDisk(wblk, 1001+wnum, buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("д�ش���: %d\n", 1001+wnum);
                    wblk = getNewBlockInBuffer(buf);
                    memset(wblk, 0, 64);
                    usage = 0;
                    wnum++;
                }
            }
        }
        freeBlockInBuffer(s_rblk, buf);
    }
    // �������������Ƿ������ݣ����У�����д�ش���
    if (usage != 0)
    {
        itoa(1002+wnum, wblk + 56, 10);   // ��̿�ĵ�ַ
        if (writeBlockToDisk(wblk, 1001+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        memset(wblk, 0, 64);
        printf("д�ش���: %d\n", 1001+wnum);
        wnum++;
    }
    printf("д���̴���: %d\n", wnum);
    printf("\nS��R�Ĳ(S-R)��%d��Ԫ��\n", cnt);
}

void sortBasedIntersection(Buffer *buf)
{
    printf("\n------------------------------\n");
    printf("��������ļ��Ͻ�����(S��R)\n");
    printf("------------------------------\n");

    int s_start = 317, s_end = 348; // �����Ĺ�ϵS����ʼ��ź���ֹ���
    int r_start = 301, r_end = 316; // �����Ĺ�ϵR����ʼ��ź���ֹ���
    unsigned char *s_rblk;  // ��ϵS�����뻺����
    unsigned char *r_rblk;  // ��ϵR�����뻺����
    unsigned char *wblk;    // ���������
    int r_left_bid = r_start, r_right_bid = r_start;    // ��ϵR����ָ���ӦԪ��Ŀ��
    int r_left_tid = 0, r_right_tid = 0;                // ��ϵR����ָ���ӦԪ���Ԫ���
    int A, B, C, D;
    int last_val = 0;   // ��һ��S.C��ֵ
    int usage = 0;      // �����������дԪ�����
    int wnum = 0;       // ��д����
    BOOL valid;         // Ԫ���Ƿ���Ч
    int cnt = 0;        // �����
    int i, j;

    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    wblk = getNewBlockInBuffer(buf);

    // �ӹ�ϵR����һ�����ݿ�
    if ((r_rblk = readBlockFromDisk(r_right_bid, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }

    // ������ϵS���ҳ�����������Ԫ��
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
            // ���S.C��ֵ����һ��Ԫ����ͬ����ָ���ϵR����ָ����л���
            if (C == last_val)
            {
                // �����Ų�ͬ����Ҫ����ǰ��Ŀ�
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
            // S.C��ֵ��ͬ����һ��Ԫ����ͬ
            else
            {
                last_val = C;
                r_left_bid = r_right_bid;
                r_left_tid = r_right_tid;
            }

            // �ӹ�ϵR������R.A = S.C��Ԫ��
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
            // �ڹ�ϵR���ҵ�R.A = S.C��Ԫ��
            // ���R.B = S.DҲ���������轫���S�е�Ԫ����Ϊ��Ч
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
                    // ��ǰ���ݿ鴦����ϣ��ͷſռ䣬������һ�����ݿ�
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
            // ������������ԭ��д�����
            if(valid)
            {
                cnt++;
                memcpy(wblk + usage*8, s_rblk + j*8, 8);
                usage++;
                // �ж����������Ƿ�д��
                if(usage == 7)
                {
                    itoa(2002+wnum, wblk + 56, 10);   // ��̿�ĵ�ַ
                    if (writeBlockToDisk(wblk, 2001+wnum, buf) != 0)
                    {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("д�ش���: %d\n", 2001+wnum);
                    wblk = getNewBlockInBuffer(buf);
                    memset(wblk, 0, 64);
                    usage = 0;
                    wnum++;
                }
            }
        }
        freeBlockInBuffer(s_rblk, buf);
    }
    // �������������Ƿ������ݣ����У�����д�ش���
    if (usage != 0)
    {
        itoa(2002+wnum, wblk + 56, 10);   // ��̿�ĵ�ַ
        if (writeBlockToDisk(wblk, 2001+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        memset(wblk, 0, 64);
        printf("д�ش���: %d\n", 2001+wnum);
        wnum++;
    }
    printf("д���̴���: %d\n", wnum);
    printf("\nS��R�Ľ���(S��R)��%d��Ԫ��\n", cnt);
}

void sortBasedUnion(Buffer *buf)
{
    printf("\n------------------------------\n");
    printf("��������ļ��ϲ�(R��S)\n");
    printf("------------------------------\n");

    int start[2] = {301, 317};  // �����Ĺ�ϵR��S����ʼ���
    int end[2] = {316, 348};    // �����Ĺ�ϵR��S����ֹ���
    unsigned char *rblks[2];   // rblks[0]ΪR�����뻺������rblks[1]ΪS�����뻺����
    int bid[2];                 // ��ϵ�����ڴ�Ŀ��
    int tid[2];                 // ������н�����һ�αȽϵ�Ԫ���[0,6]
    int r_left_bid = start[0], r_right_bid = start[1];  // ��ϵR����ָ���ӦԪ��Ŀ��
    int r_left_tid = 0, r_right_tid = 0;                // ��ϵR����ָ���ӦԪ���Ԫ���
    unsigned char *temp_rblk;  // ��ʱ����飬����ȥ��
    unsigned char *wblk;       // ���������
    BOOL finish[2];
    int A, B, last_A, last_B;
    int last_X = -1;    // ��һ����ѡ���Ԫ��ĵ�һ�����Ե�ֵ
    int last_Y = -1;    // ��һ����ѡ���Ԫ��ĵڶ������Ե�ֵ
    int Y;
    int usage = 0;      // �����������дԪ�����
    int wnum = 0;       // ��д����
    int cnt = 0;        // ���������Ԫ�����
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

    // ����ÿ���Ӽ��ϵĵ�һ��
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
        // printf("�������ݿ�%d\n", bid[i]);
    }
    A = readNumFromBlk(rblks[0], 0, 0);
    B = readNumFromBlk(rblks[0], 0, 1);
    last_A = A;
    last_B = B;

    while (arrSum(finish, 2) < 2)
    {
        small = 1 << 30;
        small_id = -1;  // 0��ʾ��ϵR��1��ʾ��ϵS
        nodup = FALSE;
        // �ҵ���·�еĽ�С��
        for (i = 0; i < 2; i++)
        {
            if (finish[i] == TRUE)
            {
                // �ù�ϵȫ���������
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
            // �����µ�Aֵ���ƶ���ָ�뵽��һ���µ�Aֵ
            if (A != last_A)
            {
                r_left_bid = r_right_bid;
                r_left_tid = r_right_tid;
            }
            last_A = A;
        }
        // ȥ��
        if (small_id == 1)
        {
            if (small == last_A)
            {
                // printf("ȥ��1��X = %d, Y = %d\n", small, Y);
                // printf("r_left_bid: %d, r_left_tid: %d, r_right_bid: %d, r_left_tid: %d\n", r_left_bid, r_left_tid, r_right_bid, r_right_tid);
                for (j = r_left_bid; j <= r_right_bid && !nodup; j++)
                {
                    // printf("ȥ��2��X = %d, Y = %d\n", small, Y);
                    if ((temp_rblk = readBlockFromDisk(j, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        return -1;
                    }
                    for (k = 0; k < 7; k++)
                    {
                        // printf("ȥ��3��X = %d, Y = %d ", small, Y);
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

        // �����ǰԪ����Ч������д����������
        if (valid)
        {
            memcpy(wblk + usage*8, rblks[small_id] + tid[small_id]*8, 8);
            // printf("(X = %d, Y = %d)\n", last_X, last_Y);
            usage++;
            cnt++;
            if(usage == 7)
            {
                itoa(3002+wnum, wblk + 56, 10);   // ��̿�ĵ�ַ
                if (writeBlockToDisk(wblk, 3001+wnum, buf) != 0)
                {
                    perror("Writing Block Failed!\n");
                    return -1;
                }
                printf("д�ش���: %d\n", 3001+wnum);
                wblk = getNewBlockInBuffer(buf);
                memset(wblk, 0, 64);
                usage = 0;
                wnum++;
            }
        }

        tid[small_id]++;    // ������һ��Ԫ���ȡ��һ��
        if(tid[small_id] == 7)
        {
            // �������ݿ��е�Ԫ��ȫ���������
            bid[small_id]++;
            // �жϸ��Ӽ����Ƿ�ȫ���������
            if (bid[small_id] > end[small_id])
            {
                finish[small_id] = TRUE;
                // printf("��%d����ϵ�������\n", small_id);
            }
            else
            {
                freeBlockInBuffer(rblks[small_id], buf);
                // �����Ӧ�Ӽ��ϵ���һ�����̿�
                if ((rblks[small_id] = readBlockFromDisk(bid[small_id], buf)) == NULL)
                {
                    perror("Reading Block Failed!\n");
                    return -1;
                }
                // printf("�������ݿ�: %d\n", bid[small_id]);
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
    // �������������Ƿ������ݣ����У�����д�ش���
    if (usage != 0)
    {
        itoa(3002+wnum, wblk + 56, 10);   // ��̿�ĵ�ַ
        if (writeBlockToDisk(wblk, 3001+wnum, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("д�ش���: %d\n", 3001+wnum);
        wnum++;
    }
    freeBuffer(buf);
    printf("д���̴���: %d\n", wnum);
    printf("\nR��S�Ĳ�������%d��Ԫ��\n", cnt);
}

