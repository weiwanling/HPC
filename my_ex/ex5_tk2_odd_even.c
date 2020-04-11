//gcc -g -Wall -fopenmp -o ex5_tk2_odd_even ex5_tk2_odd_even.c && ./ex5_tk2_odd_even  4
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<omp.h>
//#define DEBUG

const int RMAX = 1000;

int Is_sorted(int a[], int n) 
{
   int i;
   
   for (i = 1; i < n; i++)
      if (a[i-1] > a[i]) return 0;
   return 1;
}

void Generate_list(int a[], int n) 
{
   int i;
   srandom(0);
   for (i = 0; i < n; i++)
      a[i] = random() % RMAX;
}  /* Generate_list */

void Odd_even_sort(int a[], int n,int thread_count) 
{
   int phase, i, temp;
#  pragma omp parallel num_threads(thread_count) default(none) shared(a,n) private(i,temp,phase) 
   for (phase = 0; phase < n; phase++) 
   {   
      if (phase % 2 == 0) { /* Even phase */

#        pragma omp  for
         for (i = 1; i < n; i += 2) 
            if (a[i-1] > a[i]) {
               temp = a[i];
               a[i] = a[i-1];
               a[i-1] = temp;
            }
      } else { /* Odd phase */
#        pragma omp for 
         for (i = 1; i < n-1; i += 2)
            if (a[i] > a[i+1]) {
               temp = a[i];
               a[i] = a[i+1];
               a[i+1] = temp;
            }
      }
#     ifdef DEBUG
      sprintf(title, "After phase %d", phase);
      Print_list(a, n, title);
#     endif
   }
}

int main(int argc,char* argv[])
{
    int n,thread_count,start,finish;
    printf("\nplease enter n:");
    scanf("%d",&n);
    int a[n];
    thread_count=strtol(argv[1],NULL,10);
    Generate_list(a,n);
    
    start=clock();
    Odd_even_sort(a,n,thread_count);
    finish=clock();
    
	
	//排序是否成功
	printf(Is_sorted(a,n) ? "\nOK!":"\nERROR!");
    printf("\n%.2fms\n",(double)(finish-start)*1000/CLOCKS_PER_SEC);
}  