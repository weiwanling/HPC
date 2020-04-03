#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
double f(double x)
{
    return sin(x);
}
int main(int argc,char* argv[])
{

    double a,b,h,x_i,approx;
    int n,i,thread_count;
    printf("please enter a,b and n:\n");
    scanf("%lf %lf %d",&a,&b,&n);
    
    h=(b-a)/n;
    thread_count=strtol(argv[1],NULL,10);
    approx=(f(a)+f(b))/2.0;
#   pragma omp parallel for num_threads(thread_count) reduction(+:approx)
    for(i=1;i<=n-1;i++)
    {
        x_i=a+i*h;
        approx+=f(x_i);
    }
    approx*=h;
    printf("With n=%d trapezoids,our estimate\n",n);
    printf("of the integral %f to %f =%.14e\n",a,b,approx);
    


}