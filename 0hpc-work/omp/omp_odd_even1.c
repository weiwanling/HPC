#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<omp.h>   //并行所用库函数
using namespace std;
#define N 10000     //排序数据个数
#define PRINT_DATA  //注释此句则不输出数据

void swap(int *a,int *b)
{
	int temp;
	temp=*a;
	*a=*b;
	*b=temp;
}

int main(int argc, char *argv[])
{
	int a[N],flag=1;
	clock_t start,finish;
	srand((int)time(0));
	for(int i=0;i<N;i++)   //随机产生N个值在[1,N]的随机数
	{
		a[i]=rand()%N+1;
		#ifdef PRINT_DATA 
		printf("%d\t",a[i]);
		#endif
	}
	start=clock();  //计时开始
	while(flag)     //奇偶排序,flag为1表示进行了排序
	{
		flag=0;
		omp_set_num_threads(4);      //设置线程个数为4
		#pragma omp for schedule(guided,1)  //并行优化,分配给每个线程的N/4次连续的迭代计算
		for(int i=0;i<N-1;i+=2)      //奇排序
			if(a[i]>a[i+1])
			{
			  swap(&a[i],&a[i+1]);
			  flag=1;
			}
		#pragma omp for schedule(guided,1)
		for(int i=1;i<N-1;i+=2)    //偶排序
			if(a[i]>a[i+1])
			{
			  swap(&a[i],&a[i+1]);
			  flag=1;
			}
	}
	finish=clock();				//计时结束
	
	#ifdef PRINT_DATA
	printf("\n-----------------------------------------------------------\n");
	for (int i=0;i<N ;i++ )
		printf("%d\t",a[i]);
	#endif
	
	flag=1;
	for (int i=0;i<N-1 ;i++ )   //检验排序是否成功
		if(a[i]>a[i+1])
	    {
		   flag=0;
		   break;
		}
	
	//排序是否成功
	printf(flag ? "\nOK!":"\nERROR!");

	//输出所用毫秒数
	printf("\n%.2fms\n",(double)(finish-start)*1000/CLOCKS_PER_SEC);
	
	return 0;
}



