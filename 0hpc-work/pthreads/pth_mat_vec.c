/*
 * 
 * compile&run gcc -g -Wall -o pth_mat_vec pth_mat_vec.c  -lpthread && ./pth_mat_vec 2
 *
 *
 *
 */
#include <stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define M 2
#define N 3
double A[M][N]={{1,2,3},{4,5,6}};
double x[N]={6,1,7};
double y[M]={0.0};
int thread_count; 
void* Pth_mat_vec(void* rank)
{

    long my_rank=(long)rank;
    int i,j;
    int local_m=M/thread_count;
    int mfr=my_rank*local_m;
    int mlr=mfr+local_m-1;

    for(i=mfr;i<=mlr;i++)
    {
        y[i]=0.0;
        for(j=0;j<N;j++)
            y[i]+=A[i][j]*x[j];
    }
    return NULL;
}
int  printResult()
{
    int i,j;
    printf("A:\n");
    for(i=0;i<M;i++)
    {
        for(j=0;j<N;j++)
            printf("%.2f\t",A[i][j]);
        printf("\n");
    }
    
    printf("x:\n");
    for(j=0;j<N;j++)
        printf("%.2f\t",x[j]);
    printf("\nAx:\n");
    
    for(i=0;i<M;i++)
        printf("%.2f\t",y[i]);
    printf("\n");
    return 0;
}
int main(int argc,char* argv[])
{

    long thread;

    pthread_t* thread_handles;

    thread_count=strtol(argv[1],NULL,10);

    thread_handles=malloc(thread_count*sizeof(pthread_t));
 
    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,Pth_mat_vec,(void*)thread);

    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread],NULL);
    free(thread_handles);
    
    printResult();
    return 0;
}

