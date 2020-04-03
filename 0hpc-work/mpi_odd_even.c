#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>

//int array[] = {15, 11, 9, 16, 3, 14, 8, 7, 4, 6, 12, 10, 5, 2, 13, 1};
void merge(int mykeys[], int receive[], int n, int flag);
void odd_even_sort(int a[], int n);
void doSort(int myid, int local_n, int np);
void printMatrix(int array[], int n);
void init(int n, int myid, int np);
int getPartner(int phase, int myid, int comm_sz);
int cmp(const void *a, const void *b);

int *array, *mykeys, *receive;
int n, partner;

int main(int argc, char **argv)
{
    int i, j;
    int myid, np, namelen;
    char proc_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Get_processor_name(proc_name, &namelen);

    double beginTime, endTime;
    beginTime = MPI_Wtime();

    n = atoi(argv[1]);
    int local_n = (n + (np - n % np)) / np; // fill with the matrix
    //初始化随机数。
    init(n, myid, np);

    // 2. 把数据广播出去。
    MPI_Scatter(array, local_n, MPI_INT, mykeys, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // 3. local sort
    qsort(mykeys, local_n, sizeof(int), cmp);

    // 4. 进行np轮排序。
    doSort(myid, local_n, np);

    // 5. 0号进程收集排序好的数据
    MPI_Gather(mykeys, local_n, MPI_INT, array, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    endTime = MPI_Wtime();
    if (myid == 0)
    {
        printMatrix(array, n);
        printf("spent time = %lf second\n", endTime - beginTime);
    }

    free(array);
    free(mykeys);
    free(receive);
    MPI_Finalize();
}

void merge(int mykeys[], int receive[], int n, int flag)
{

    int mi, ti, ri;
    int *temp = malloc(sizeof(int) * n * 2);
    if (temp == NULL)
    {
        exit(-1);
    }

    mi = ri = ti = 0;
    while (mi < n && ri < n)
    {
        if (mykeys[mi] >= receive[ri])
        {
            temp[ti] = receive[ri];
            ri++;
            ti++;
        }
        else
        {
            temp[ti] = mykeys[mi];
            ti++;
            mi++;
        }
    }

    while (mi < n)
    {
        temp[ti] = mykeys[mi];
        ti++;
        mi++;
    }
    while (ri < n)
    {
        temp[ti] = receive[ri];
        ti++;
        ri++;
    }

    ti = flag > 0 ? n : 0;

    for (mi = 0; mi < n; mi++)
        mykeys[mi] = temp[ti++];
    free(temp);
}
void printMatrix(int array[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d\n", array[i]);
}
void doSort(int myid, int local_n, int np)
{
    int i;
    for (i = 0; i < np; i++)
    {
        partner = getPartner(i, myid, np);
        if (partner != MPI_PROC_NULL)
        {
            MPI_Sendrecv(mykeys, local_n, MPI_INT, partner, 0, receive, local_n, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            merge(mykeys, receive, local_n, myid - partner);
        }
    }
}
void init(int n, int myid, int np)
{
    int i;
    int total = n + np - n % np;
    if (!myid)
    {
        srand(time(NULL));
        array = (int *)malloc(sizeof(int) * total);
        for (i = 0; i < n; i++)
        {
            *(array + i) = random();
        }

        for (i = n; i < total; i++)
        {
            *(array + i) = 0x7fffffff;
        }
    }
    receive = (int *)malloc(sizeof(int) * total / np);
    mykeys = (int *)malloc(sizeof(int) * total / np);
}

int getPartner(int phase, int myid, int comm_sz)
{
    int partner;
    if (phase % 2 == 0)
    {
        if (myid % 2 != 0)
        {
            partner = myid - 1;
        }
        else
        {
            partner = myid + 1;
        }
    }
    else
    {
        if (myid % 2 != 0)
        {
            partner = myid + 1;
        }
        else
        {
            partner = myid - 1;
        }
    }
    if (partner == -1 || partner == comm_sz)
    {
        partner = MPI_PROC_NULL;
    }
    return partner;
}
int cmp(const void *a, const void *b)
{
    return *((int *)a) > *((int *)b);
}
