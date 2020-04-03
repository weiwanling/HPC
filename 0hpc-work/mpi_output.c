#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(){
  int my_rank, comm_sz, q;
  char message[100];

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  //printf("Proc %d of %d \n", my_rank, comm_sz);

  if (my_rank != 0){
    sprintf(message, "Proc %d of %d ", my_rank, comm_sz);
    MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  } else {
    printf("Proc %d of %d \n", my_rank, comm_sz);
    for(q = 1; q < comm_sz; q++){
      MPI_Recv(message, 100, MPI_CHAR, q, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", message);
    }
  }

  MPI_Finalize();
  return 0;
}
