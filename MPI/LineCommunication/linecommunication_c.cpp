#include<iostream>
#include<mpi.h>


int main(int argc, char* argv[]){

  int rank, size, recv_data = 123;
  int tag = 1;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Status status;

  int right_rank = rank+1;
  if(rank == size-1){
    right_rank = MPI_PROC_NULL;
  }

  int left_rank = rank-1;
  if(rank == 0){
    left_rank = MPI_PROC_NULL;
  }

  MPI_Sendrecv(&rank, 1, MPI_INT, right_rank, tag, &recv_data, 1, MPI_INT, left_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

  std::printf("Process %d recevived from %d\n", rank, recv_data);

  MPI_Finalize();
  return 0;
}
