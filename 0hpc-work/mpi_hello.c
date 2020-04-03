#include <stdio.h>
#include<string.h>
#include<mpi.h>

int main(int argc,char *argv[])
{
    int rc,i;
    int comm_sz,my_rank;
    char message[100];
    MPI_Status status;
    int MAX_STRING = 100;
    rc=MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    //printf("hello ,rc=%d,MPI_SUCCESS=%d\n",rc,MPI_SUCCESS);
    printf("hello ,I am %d of %d\n",my_rank,comm_sz);
    if(my_rank!=0)
    {
        strcpy(message,"hello");
        //MPI_Send(message,strlen(message)+1,MPI_CHAR,0,99,MPI_COMM_WORLD);
        MPI_Send(message,MAX_STRING,MPI_CHAR,0,99,MPI_COMM_WORLD);
    }
    else
    {
        for(i=1;i<comm_sz;i++)
        {
          // MPI_Recv(message,100,MPI_CHAR,MPI_ANY_SOURCE,99,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          
            MPI_Recv(message,100,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            printf("%s from %d\n",message,i);
        }
    }

    MPI_Finalize();
 
    return 0;
}

