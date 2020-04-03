#include <stdio.h>
#include<omp.h>
#include<stdlib.h>
int main(int argc,char* argv[])
{

    int i;
    int thread_count=strtol(argv[1],NULL,10);
#   pragma omp parallel for num_threads(thread_count)
    for(i=0;i<10;i++)
        printf("i=%d\n",i);
    printf("Finished\n");
    return 0;
}

