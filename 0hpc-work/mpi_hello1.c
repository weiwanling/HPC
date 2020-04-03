#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int maxSize = 100;                    // 传输数据的最大尺寸，单位是个而不是字节

int main(int argc, char* argv[])
{
    char greet[maxSize];
    int size, rank;
    MPI_Init(&argc, &argv);                 // MPI 初始化
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // 获取通信子进程总数
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // 获取当前通信子的编号
    
    if (rank != 0)                                                                      //  非主进程
    {
        sprintf(greet, maxSize, "\nGreet from process %d of %d", rank, size);
        MPI_Send(greet, strlen(greet) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);             // 发送信息给 0 号通信子
    }
    else                                                                                // 主进程
    
{
        char name[maxSize];
        int nameLength;
        MPI_Get_processor_name(name, &nameLength);                                      // 获取主机名
        printf("\nGreet from process %d of %d in %s (length:%d)\n", rank, size, name, nameLength);
        for (int q = 1; q < size; q++)
        {
            MPI_Recv(greet, maxSize, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);// 按顺序接受来自其他通信子的信息
            printf("%s\n", greet);
        }
    }
    MPI_Finalize();                         // MPI 资源释放
    //getchar();                            // 不用添加 getchar();，程序在命令提示符中用 mpiexec 命令运行
    return 0;
}
