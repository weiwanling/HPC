//gcc -g -Wall -fopenmp -o omp_queue2 omp_queue2.c && ./omp_queue2  2 2
#include <stdio.h>
#include<stdlib.h>
#include<omp.h>
#include "queue1.h"
const int MAX_MSG=10000;
#define DEBUG

void Usage(char* prog_name)
{
    fprintf(stderr,"usage: %s <number of threads> ,<number of messages>",prog_name);
    fprintf(stderr,"\tnumber of messages=number sent by each thread");
}

//发送消息函数，入队为临界区，两种方式保护，命名的critical指令和锁机制
void Send_msg(struct queue_s* msg_queues[],int my_rank,int thread_count,int msg_number)
{
    //int mesg=random()%MAX_MSG;
    int mesg=-msg_number;
    int dest=random()%thread_count;
    struct queue_s* q_p=msg_queues[dest];
//#   pragma omp critical(my_rank%dest)
    omp_set_lock(&q_p->lock);
    Enqueue(q_p,my_rank,mesg);
    omp_unset_lock(&q_p->lock);
#   ifdef DEBUG
    printf("Thread %d > sent %d to %d\n",my_rank,mesg,dest);
#   endif
}//Send_msg

//接收消息函数，出队为临界区，两种方式保护，命名的critical指令和锁机制
void Try_receive(struct queue_s* q_p,int my_rank)
{
    
    int src,mesg;
    int queue_size=q_p->enqueued-q_p->dequeued;

    if(queue_size==0) return;
    else if(queue_size==1)
    {
//#     pragma omp critical(Dequeue)
        omp_set_lock(&q_p->lock);
        Dequeue(q_p,&src,&mesg);
        omp_unset_lock(&q_p->lock);
    }
    else 
        Dequeue(q_p,&src,&mesg);
    printf("Thread %d > received %d from %d\n",my_rank,mesg,src);

}

// 接收消息终止检测函数，队列为空，并且所有线程都发送完消息，则返回真
int  Done(struct queue_s* q_p,int done_sending,int thread_count)
{
    int queue_size=q_p->enqueued-q_p->dequeued;
    if(queue_size==0&&done_sending==thread_count)
        return 1;
    else
        return 0;

}

int main(int argc,char* argv[])
{

    int thread_count;
    int send_max;
    struct queue_s** msg_queues;
    int done_sending=0;

    if(argc!=3) Usage(argv[0]);
    thread_count=strtol(argv[1],NULL,10);
    send_max=strtol(argv[2],NULL,10);
    if(thread_count<=0||send_max<0) Usage(argv[0]);

    msg_queues=malloc(thread_count*sizeof(struct queue_node_s*));
    //开启thread_count个线程的并行块,共享变量由shared子句指定
#   pragma omp parallel num_threads(thread_count)\
        default(none) shared(thread_count,send_max,msg_queues,done_sending)

    {
        int my_rank=omp_get_thread_num();
        int msg_number;
        srandom(my_rank);
        msg_queues[my_rank]=Allocate_queue();
        //设置显式路障，确保任何一个线程在所有线程完成队列分配后才开始发送消息
        //交替发送和接收消息
#       pragma omp barrier
        for(msg_number=0;msg_number<send_max;msg_number++)
        {
            Send_msg(msg_queues,my_rank,thread_count,msg_number);
            Try_receive(msg_queues[my_rank],my_rank);
        }
        //临界区，只保护一条由ｃ语言赋值语句形成的临界区
#       pragma omp atomic
        done_sending++;
        //发送完消息后，不断接收消息直到所有线程都已完成发送消息
        while(!Done(msg_queues[my_rank],done_sending,thread_count))
            Try_receive(msg_queues[my_rank],my_rank);
        
        Free_queue(msg_queues[my_rank]);
        free(msg_queues[my_rank]);
    }
    free(msg_queues);
    return 0;
}

