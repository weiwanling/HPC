# HPC
## my_ex
1. 编写Hello World!的串行、并行程序
2. 任务一：采用点对点的通信方式编写梯形积分法的MPI程序：
    1) The endpoints of the interval and the number of trapezoids are hardwired
    2) Accepts input of the endpoints of the interval and the number of trapezoids
    
   任务二：修改梯形积分法，使其能够在comm_sz无法被n整除的情况下，正确估计积分值(假设n>=comm_sz);
3. 任务一：编写梯形积分法的MPI程序：
      1) 广播MPI_Bcast（输入数据a,b,n）
      2) 规约MPI_Reduce/MPI_Allreduce（部分积分值求和）
      
   任务二：编写一个并行的向量加法程序
      1) 读向量，并利用MPI_Scatter将读取的向量分发给各进程
      void Read_vector(double local_a[], int local_n, int n, char vec_name[], int my_rank, MPI_Comm comm);
      2) 打印向量，并利用MPI_Gather将各进程的结果汇总
      void Print_vector(double local_b[], int local_n, int n, char title[], int my_rank, MPI_Comm comm);
      3) 实现向量加法并行函数
      void Parallel_vector_sum(double local_x[], double local_y[], double local_z[], int local_n);
   
   任务三：编写一个完整的并行矩阵向量乘法程序：
      1) 使用MPI_Allgather
      2) 实现如下函数：
      
        void Read_matrix(char prompt[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm comm);
        void Read_vector(char prompt[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm);
        void Print_matrix(char title[], double local_A[], int m, int local_m, nt n, int my_rank, MPI_Comm comm);
        void Print_vector(char title[], double local_vec[], int n, int local_n, int my_rank, MPI_Comm comm);
        void Mat_vect_mult(double local_A[], double local_x[], double local_y[], int local_m, int n, int local_n, MPI_Comm comm);
4.  任务一：编写OpenMP的Hello,world程序，并编译运行

    任务二：编写Pthreads的Hello,world程序，并编译运行
    
    任务三：按下述要求实现梯形积分的OpenMP并行程序：
    
      1) 并行采用Trap函数编写串行程序
        Trap(double a, double b, int n, double* global_result_p);
      2) 并行采用Local_trap函数编写的并行程序：
        a)double Local_trap(double a, double b, int n);
        b)利用critical
      3) 采用Local_trap函数改写梯形积分的OpenMP并行程序
        a)double Local_trap(double a, double b, int n);
        b)利用reduction
      4) 采用trap改写梯形积分的OpenMP并行程序
        a)double Trap(double a, double b, int n, int thread_count)
        b)利用reduction
        c)利用parallel for指令并行for循环
        
5. 任务一：利用OpenMP编写pi值估计的并行程序

   任务二：利用OpenMP编写并行奇偶排序算法
   
       要求：利用一个parallel指令和两个for指令 
## 0hpc_work
学习笔记
## ex progs
教材源码

## pdf
课后题答案
