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

void Get_input1(int my_rank,int comm_sz,double* a_p,double* b_p,int* n_p)
{
    int dest;
    if(my_rank==0)
    {
        printf("Enter a,b,and n\n");
        scanf("%lf %lf %d",a_p,b_p,n_p);

    }
    MPI_Bcast(a_p,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(b_p,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(n_p,1,MPI_INT,0,MPI_COMM_WORLD);

}
int main()
{
    double a=0.0,b=3.0;
    int n=2048,my_rank,comm_sz,local_n,source;
    double h,local_int,total_int,local_b,local_a;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    Get_input1(my_rank,comm_sz,&a,&b,&n);
    
    h=(b-a)/n;
    local_n=n/comm_sz;

    local_a=a+my_rank*local_n*h;
    local_b=local_a+local_n*h;
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