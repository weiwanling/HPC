
// 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1
// 16 15 14 13 1 2 3 4 12 11 10 9 8 7 6 5 
#include <stdio.h>
#include<stdlib.h>
#include<mpi.h>
//#include<algorithm.h>
void Read_list(int local_A[], int local_n, int my_rank, int p,MPI_Comm comm)
{
  int* a=NULL;
  int i;                                                                    
   if(my_rank==0)
   {
        a=malloc(p*local_n*sizeof(int));
        printf("Enter the vector :\n");
        for(i=0;i<p*local_n;i++)
           scanf("%d",&a[i]);
        MPI_Scatter(a,local_n,MPI_INT,local_A,local_n,MPI_INT,0,comm);
        free(a);
   }
   else
        MPI_Scatter(a,local_n,MPI_INT,local_A,local_n,MPI_INT,0,comm);
    
}

//点对点通信
void Print_local_list(int local_A[], int local_n, int my_rank, int p,MPI_Comm comm)
{
   char message[100];
   int i,q;
   if (my_rank != 0)
   {
    //sprintf(message, "Proc %d of %d : ", my_rank, comm_sz);
        //printf("Proc %d of %d \n", my_rank, comm_sz);
        MPI_Send(local_A, local_n, MPI_INT, 0, 0, comm);
   }
   else 
   {
        printf("各进程分配的数字: \n");
        for(i=0;i<local_n;i++) 
               printf("%d ",local_A[i]);
        printf("\n");
        for(q = 1; q < p; q++)
        {
            MPI_Recv(local_A, local_n, MPI_INT, q, 0,comm, MPI_STATUS_IGNORE);
            for(i=0;i<local_n;i++) 
                printf("%d ",local_A[i]);
            printf("\n");
        }
    }
}

void Print_global_list(int local_A[], int local_n, int my_rank, int p, MPI_Comm comm)
{
    int* b=NULL;
    int i;
    int n=p*local_n;
    if(my_rank==0)
    {
        printf("\n排序后的数据：\n");
        b=malloc(n*sizeof(int));
        MPI_Gather(local_A,local_n,MPI_INT,b,local_n,MPI_INT,0,comm);
        
        for(i=0;i<n;i++)
           printf("%d ",b[i]);
        printf("\n");
        free(b);
     }
     else
        MPI_Gather(local_A,local_n,MPI_INT,b,local_n,MPI_INT,0,comm); 
    
}

int Compare(int a,int b)
{
  return a<b;
}

/* Merge_ high */
void Merge_high(int local_A[], int temp_B[], int temp_C[],int local_n)
{

    int ai, bi, ci;
    
    ai = local_n-1;
    bi = local_n-1;
    ci = local_n-1;
    
    while(ci>=0)
    {
    
        if(local_A[ai] >= temp_B[bi]) 
        {
          temp_C[ci] = local_A[ai];
           ci-- ;
           ai-- ;
        }
        else
        {
           temp_C[ci] = temp_B[bi];
           ci--; bi--;
        }
   }
    memcpy(local_A, temp_C, local_n*sizeof(int));
}

/* Merge_ low */
void Merge_low(int local_A[], int temp_B[], int temp_C[],int local_n)
{

    int ai, bi, ci;
    
    ai = 0;
    bi = 0;
    ci = 0;
    
    while(ci<local_n)
    {
    
        if(local_A[ai] <= temp_B[bi]) 
        {
          temp_C[ci] = local_A[ai];
           ci++ ;ai++ ;
        }
        else
        {
           temp_C[ci] = temp_B[bi];
           ci++; bi++;
        }
   }
    memcpy(local_A, temp_C, local_n*sizeof(int));
}

/* Odd_even_iter */
void Odd_even_iter(int local_A[], int temp_B[], int temp_C[], int local_n, int phase, 
                   int even_partner, int odd_partner, int my_rank, int p, MPI_Comm comm) 
{
    MPI_Status status;
    if(phase % 2 == 0)
    { 
       if (even_partner >= 0) 
       { 
             MPI_Sendrecv(local_A, local_n, MPI_INT, even_partner, 0, 
                          temp_B,  local_n, MPI_INT, even_partner, 0, comm, &status);
 
            if (my_rank % 2 != 0) 
                Merge_high(local_A, temp_B, temp_C, local_n);
            else
                Merge_low(local_A, temp_B, temp_C, local_n);
       } 
    }
   else /* odd phase */
   { 
    if (odd_partner >= 0) 
    { 
        MPI_Sendrecv(local_A, local_n, MPI_INT, odd_partner, 0, 
                     temp_B,  local_n, MPI_INT, odd_partner, 0, comm, &status); 
        if (my_rank % 2 != 0) 
            Merge_low(local_A, temp_B, temp_C, local_n);
        else
            Merge_high(local_A, temp_B, temp_C, local_n); 
    } 
   }
} 
void Sort(int local_A[], int local_n, int my_rank, int p, MPI_Comm comm)
{
     int phase;
     int *temp_B, *temp_C;
     int even_partner; /* phase is even or left-looking */
     int odd_partner; /* phase is odd or right-looking */
     temp_B = (int*) malloc(local_n*sizeof(int));
     temp_C = (int*) malloc(local_n*sizeof(int));
     
     if (my_rank % 2 != 0) 
     {
         even_partner = my_rank - 1;
         odd_partner = my_rank + 1;
         if (even_partner == -1) even_partner = MPI_PROC_NULL; // Idle during even phase
         if (odd_partner == p) odd_partner = MPI_PROC_NULL; // Idle during even phase
     } 
     else 
     {
         even_partner = my_rank + 1;
         odd_partner = my_rank-1; 
         if (even_partner == p) even_partner = MPI_PROC_NULL; // Idle during even phase
         if (odd_partner == -1) odd_partner = MPI_PROC_NULL; // Idle during even phase
     }
     
     qsort(local_A, local_n, sizeof(int), Compare); /* Sort local list using built-in quick sort */

     for (phase = 0; phase < p; phase++)
         Odd_even_iter(local_A, temp_B, temp_C, local_n, phase, 
                       even_partner, odd_partner, my_rank, p, comm);
     
     free(temp_B);
     free(temp_C);
} //Sort 

int main(int argc,char* argv[])
{
     int my_rank, p;
     int *local_A;
     int n,local_n;
     
     MPI_Comm comm;
     MPI_Init(&argc, &argv);
     
     comm = MPI_COMM_WORLD;
     
     MPI_Comm_size(comm, &p);
     MPI_Comm_rank(comm, &my_rank);
     
     n = 16;
     local_n = n/p;
     local_A = (int*) malloc(local_n*sizeof(int));
     
     Read_list(local_A, local_n, my_rank, p, comm);
     Print_local_list(local_A, local_n, my_rank, p, comm);
     //printf("\n输入的数据：\n");
     //Print_global_list(local_A, local_n, my_rank, p, comm);
     
     Sort(local_A, local_n, my_rank, p, comm);
      
     //Print_local_list(local_A, local_n, my_rank, p, comm);
     
     Print_global_list(local_A, local_n, my_rank, p, comm);
     
     free(local_A);
     
     MPI_Finalize();
     
     return 0;

} //main

