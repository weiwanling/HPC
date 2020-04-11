/*
 * gcc -g -Wall -lm -o ex4_tk3_1 ex4_tk3_1.c && ./ex4_tk3_1
 */
#include <stdio.h>
#include<stdlib.h>
#include<math.h>

double f(double a)
{
    return sin(a);
}

void Trap(double a,double b,int n,double* global_result)
{
    
    double h,x,my_result;
    int i;
    h=(b-a)/n;
    

    my_result=(f(a)+f(b))/2.0;
    for(i=1;i<=n-1;i++)
    {
        x=a+i*h;
        my_result+=f(x);
    }
    
    my_result*=h;
    *global_result+=my_result;


}

int main(int argc,char* argv[])
{
    
   double a,b,global_result=0;
   int n;
   printf("please enter a,b and n:\n");
   scanf("%lf %lf %d",&a,&b,&n);
   
   Trap(a,b,n,&global_result);

   printf("With n=%d trapezoids,our estimate\n",n);
   printf("of the integral %f to %f =%.14e\n",a,b,global_result);
    
    return 0;
}
