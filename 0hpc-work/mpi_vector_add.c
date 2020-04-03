#include <stdio.h>
#include<stdlib.h>
#include<mpi.h>

void Read_vector(double local_a[],int local_n,
                 int n,char vec_name[],
                 int my_rank,MPI_Comm comm)
{

    double* a=NULL;
    int i;
    if(my_rank==0)
    {
       a=malloc(n*sizeof(double));
       printf("Enter the vector : %s\n",vec_name);
       for(i=0;i<n;i++)
           scanf("%lf",&a[i]);
       MPI_Scatter(a,local_n,MPI_DOUBLE,local_a,local_n,MPI_DOUBLE,0,comm);
       free(a);
    }
    else
    {
       MPI_Scatter(a,local_n,MPI_DOUBLE,local_a,local_n,MPI_DOUBLE,0,comm);
    }

}

void Print_vector(double local_b[],
                  int local_n,
                  int n,
                  char title[],
                  int my_rank,
                  MPI_Comm comm)
{
    double* b=NULL;
    int i;
    if(my_rank==0)
    {

        b=malloc(n*sizeof(double));
        MPI_Gather(local_b,local_n,MPI_DOUBLE,b,local_n,MPI_DOUBLE,0,comm);
        printf("%s\n",title);
        for(i=0;i<n;i++)
           printf("%f ",b[i]);
        printf("\n");
        free(b);
     }
     else
     {
        MPI_Gather(local_b,local_n,MPI_DOUBLE,b,local_n,MPI_DOUBLE,0,comm);
     }
     
}

void  Parallel_vector_sum(double local_x[],double local_y[],
                          double local_z[],int local_n)

{

    int local_i;
    for(local_i=0;local_i<local_n;local_i++)
        
        local_z[local_i]=local_x[local_i]+local_y[local_i];

}

int main()
{

    int n=6,local_n;
    double *local_x,*local_y,*local_z;
    int comm_sz,my_rank;
    MPI_Comm comm;

    MPI_Init(NULL,NULL);
    comm=MPI_COMM_WORLD;
    MPI_Comm_size(comm,&comm_sz);
    MPI_Comm_rank(comm,&my_rank);
    
    local_n=n/comm_sz;

    local_x=malloc(local_n*sizeof(double));
    local_y=malloc(local_n*sizeof(double));
    local_z=malloc(local_n*sizeof(double));

    Read_vector(local_x,local_n,n,"x",my_rank,comm);
    Print_vector(local_x,local_n,n,"x is ",my_rank,comm);
    Read_vector(local_y,local_n,n,"y",my_rank,comm);
    Print_vector(local_y,local_n,n,"y is ",my_rank,comm);

    Parallel_vector_sum(local_x,local_y,local_z,local_n);
    Print_vector(local_z,local_n,n,"z is ",my_rank,comm);

    free(local_x);
    free(local_y);
    free(local_z);

    MPI_Finalize();

    return 0;

}

