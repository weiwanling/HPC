#include <stdio.h>
#include<math.h>
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
int main()
{
    double a=0.0,b=3.0,estimate;
    int n=2048;
    double h;

    h=(b-a)/n;
    estimate=Trap(a,b,n,h);
    
    printf("%f\n",estimate);
    return 0;
}

