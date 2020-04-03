/*
 *
 *gcc -g -Wall -o pth_pi pth_pi.c -lpthread && ./pth_pi 5
 *
 */

#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>

int thread_count;
long long n;
double sum=0;
 pthread_mutex_t*  mutex_p;
void* Thread_sum(void* rank)
{
   double factor,my_sum=0.0;
   long long i;
   long my_rank=(long)rank;
   long long my_n=n/thread_count;
   long long mfi=my_n*my_rank;
   long long mli=mfi+my_n;
   factor=mfi%2==0 ? 1.0:-1.0;
 
   for(i=mfi;i<mli;i++,factor*=-1.0)
       my_sum+=factor/(2*i+1);
   pthread_mutex_lock(&mutex_p);
   sum+=my_sum;
   pthread_mutex_unlock(&mutex_p);
   return NULL;
}

int main(int argc,char* argv[])
{
    
    long thread;
    printf("please enter n:\t");
    scanf("%lld",&n);

    pthread_t* thread_handles;
    
    pthread_mutex_init(&mutex_p,NULL);
    thread_count=strtol(argv[1],NULL,10);

    thread_handles=malloc(thread_count*sizeof(pthread_t));
 
    for(thread=0;thread<thread_count;thread++)
        pthread_create(&thread_handles[thread],NULL,Thread_sum,(void*)thread);

    for(thread=0;thread<thread_count;thread++)
        pthread_join(thread_handles[thread],NULL);
    free(thread_handles);
    
    printf("pi: %lf\n",4.0*sum);
    return 0;

}

