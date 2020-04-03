#include <stdio.h>
#include<stdlib.h>
#include<omp.h>
int main(int argc,char* argv[])
{
    
    double sum=0.0,factor;
    int i,n,thread_count;

    thread_count=strtol(argv[1],NULL,10);
    printf("please enter n:\n");
    scanf("%d",&n);
#   pragma omp parallel for default(none) reduction(+:sum) \
                            private(factor,i) shared(n) num_threads(thread_count)
    for(i=0;i<n;i++)
    {
        factor=(i%2==0)? 1.0: -1.0;
        sum+=factor/(1+2*i);
    }
    
    printf("pi=%lf\n",4*sum);
    return 0;

}

