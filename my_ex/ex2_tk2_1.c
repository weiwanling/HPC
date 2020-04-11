/*
 * mpicc -lm -g -Wall -o ex2_tk2_1 ex2_tk2_1.c && mpiexec -n 5 ./ex2_tk2_1
 */

#include <stdio.h>
#include<math.h>
#include<mpi.h>
double f(double x)
{
    return sin(x);
}
double Trap(double a,double b,double n,double h)
{
    double estimate,x;
    int i;
    estimate=(f(a)+f(b))/2.0;
    for(i=1;i<n;i++)
    {
       x=a+i*h;
       estimate+=f(x);
       
    }
    return estimate*h;
}
void Get_input(int my_rank,int comm_sz,double* a_p,double* b_p,int* n_p)
{
    int dest;
    if(my_rank==0)
    {
        printf("Enter a,b,and n\n");
        scanf("%lf %lf %d",a_p,b_p,n_p);
        for(dest=1;dest<comm_sz;dest++)
        {
           MPI_Send(a_p,1,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
           MPI_Send(b_p,1,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
           MPI_Send(n_p,1,MPI_INT,dest,0,MPI_COMM_WORLD);

        }
    }
    else
    {
        MPI_Recv(a_p,1,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(b_p,1,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(n_p,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    }

}

int main()
{
    double a=0.0,b=3.0;
    int n=2048,my_rank,comm_sz,local_n,source,q,r;
    double h,local_int,total_int,local_b,local_a;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    Get_input(my_rank,comm_sz,&a,&b,&n);
    
    h=(b-a)/n;

    q=n/comm_sz;
    r=n%comm_sz;
    
    if(my_rank<r)
    {
        local_n=q+1;
        local_a=a+my_rank*local_n*h;
        local_b=local_a+local_n*h;
    }
    else
    {
        local_n=q;
        local_a=a+my_rank*local_n*h+r*h;
        local_b=local_a+local_n*h;
    }


    local_int=Trap(local_a,local_b,n,h);


    if(my_rank!=0)
    {
        MPI_Send(&local_int,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    }
    else
    {
        total_int=local_int;
        for(source=1;source<comm_sz;source++)
        {
            MPI_Recv(&local_int,1,MPI_DOUBLE,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            total_int+=local_int;
        }
        printf("n=%d,a=%.2f b=%.2f area=%f\n",n,a,b,total_int);

    }


    MPI_Finalize();
    return 0;
}
