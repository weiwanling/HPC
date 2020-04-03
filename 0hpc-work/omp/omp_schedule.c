/*
 export OMP_NUM_THREADS=4
 export OMP_SCHEDULE="guided,1"
 gcc  -g -Wall -fopenmp -o omp_schedule omp_schedule.c && ./omp_schedule 
 *
 *
 *
 */

#include <stdio.h>
#include <omp.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
    int j=0;
#   pragma omp parallel for schedule(runtime) 
    for(j=0;j<8;j++)
    { 
        sleep(3); 
        printf("j=%d,ThreadId=%d\n",j,omp_get_thread_num());
    }
    return 0;
}
