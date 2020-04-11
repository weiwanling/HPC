/*
    mpicc -g -Wall -o ex3_tk3_1 ex3_tk3_1.c && mpiexec -n 2  ./ex3_tk3_1
    Enter the matrix(2X2) : A
    1 2 3 4
    Enter the vector(2X1) : x
    2 2
    A
    1.000000	2.000000	
    3.000000	4.000000	
    x
    2.000000 2.000000 
    b(Ax)
    6.000000 14.000000 
*/
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

void Read_matrix(char prompt[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm comm);
void Read_vector(char prompt[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm);
void Print_matrix(char title[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm comm);
void Print_vector(char title[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm);
void Mat_vect_mult(double local_A[], double local_x[], double local_y[], int local_m, int n, int local_n, MPI_Comm comm);

int main(int argc,char* argv[])
{
    
    int comm_sz,my_rank,local_m,local_n,m=2,n=2;
    double *local_A,*local_x,*local_b;
    MPI_Comm comm;

    MPI_Init(NULL,NULL);
    comm=MPI_COMM_WORLD;
    MPI_Comm_size(comm,&comm_sz);
    MPI_Comm_rank(comm,&my_rank);
            
  
    local_m=m/comm_sz;
    local_n=n/comm_sz;

    local_A=malloc(local_m*n*sizeof(double));
    local_x=malloc(local_n*sizeof(double));
    local_b=malloc(local_n*sizeof(double));

    Read_matrix("A",local_A,m,local_m,n,my_rank,comm);
    Read_vector("x",local_x,n,local_n,my_rank,comm);
    Mat_vect_mult(local_A, local_x,local_b, local_m, n, local_n, comm);
    
    Print_matrix("A", local_A, m,local_m, n, my_rank, comm);
    Print_vector("x", local_x, n,local_n, my_rank, comm);
    Print_vector("b(Ax)", local_b, n,local_n, my_rank, comm);
    
    MPI_Finalize();

    return 0;
}

void Read_matrix(char prompt[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm comm)
{
    double* a=NULL;
    int i,j;
    if(my_rank==0)
    {
       a=malloc(m*n*sizeof(double));
       printf("Enter the matrix(%dX%d) : %s\n",m,n,prompt);
       for(i=0;i<m;i++)
           for(j=0;j<n;j++)
                scanf("%lf",&a[i*n+j]);
       MPI_Scatter(a,local_m*n,MPI_DOUBLE,local_A,local_m*n,MPI_DOUBLE,0,comm);
       free(a);
    }
    else
    {
       MPI_Scatter(a,local_m*n,MPI_DOUBLE,local_A,local_m*n,MPI_DOUBLE,0,comm);
    }    

}
void Read_vector(char prompt[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm)
{
    double* a=NULL;
    int i;
    if(my_rank==0)
    {
       a=malloc(n*sizeof(double));
       printf("Enter the vector(%dX1) : %s\n",n,prompt);
       for(i=0;i<n;i++)
           scanf("%lf",&a[i]);
       MPI_Scatter(a,local_n,MPI_DOUBLE,local_vec,local_n,MPI_DOUBLE,0,comm);
       free(a);
    }
    else
    {
       MPI_Scatter(a,local_n,MPI_DOUBLE,local_vec,local_n,MPI_DOUBLE,0,comm);
    }    

}
void Print_matrix(char title[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm comm)
{
    double* b=NULL;
    int i,j;
    if(my_rank==0)
    {

        b=malloc(m*n*sizeof(double));
        MPI_Gather(local_A,local_m*n,MPI_DOUBLE,b,local_m*n,MPI_DOUBLE,0,comm);
        printf("%s\n",title);
        for(i=0;i<m;i++)
        {
            for(j=0;j<n;j++)
                printf("%f\t",b[i*n+j]);
            printf("\n");
        }
        free(b);
     }
     else
     {
        MPI_Gather(local_A,local_m*n,MPI_DOUBLE,b,local_m*n,MPI_DOUBLE,0,comm);
     }
}
void Print_vector(char title[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm)
{
    double* b=NULL;
    int i;
    if(my_rank==0)
    {

        b=malloc(n*sizeof(double));
        MPI_Gather(local_vec,local_n,MPI_DOUBLE,b,local_n,MPI_DOUBLE,0,comm);
        printf("%s\n",title);
        for(i=0;i<n;i++)
           printf("%f ",b[i]);
        printf("\n");
        free(b);
     }
     else
     {
        MPI_Gather(local_vec,local_n,MPI_DOUBLE,b,local_n,MPI_DOUBLE,0,comm);
     }

}
void Mat_vect_mult(
                    double local_A[],double local_x[],double local_y[], 
                    int local_m, int n, int local_n, 
                    MPI_Comm comm
                  )
{
        double* x;
        int local_i,j;
        x=malloc(n*sizeof(double));
        MPI_Allgather(local_x,local_n,MPI_DOUBLE,x,local_n,MPI_DOUBLE,comm);
        for(local_i=0;local_i<local_m;local_i++)
        {
            local_y[local_i]=0.0;
            for(j=0;j<n;j++)
                 local_y[local_i]+=local_A[local_i*n+j]*x[j];        
        }
        free(x);
}
