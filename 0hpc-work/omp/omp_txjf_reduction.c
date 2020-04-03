#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>
double f(double a)
{
    return sin(a);
}

double local_trap(double a,double b,int n)
{
    
    double h,x,my_result;
    double local_a,local_b;
    int i,local_n;
    int my_rank=omp_get_thread_num();  //thread_id
    int thread_count=omp_get_num_threads();

    h=(b-a)/n;
    local_n=n/thread_count;
    local_a=a+my_rank*local_n*h;
    local_b=local_a+local_n*h;

    my_result=(f(local_a)+f(local_b))/2.0;
    for(i=1;i<=local_n-1;i++)
    {
        x=local_a+i*h;
        my_result+=f(x);
    }
    
    my_result*=h;
    
    return my_result;

}

int main(int argc,char* argv[])
{
    
   double a,b,global_result=0;
   int n,thread_count=strtol(argv[1],NULL,10);
   printf("please enter a,b and n:\n");
   scanf("%lf %lf %d",&a,&b,&n);
   
#  pragma omp parallel num_threads(thread_count) reduction(+:global_result)
   global_result+=local_trap(a,b,n);

   printf("With n=%d trapezoids,our estimate\n",n);
   printf("of the integral %f to %f =%.14e\n",a,b,global_result);
    
    return 0;
}

