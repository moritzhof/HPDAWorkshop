#include<iostream>
#include<mpi.h>

int main(int argc, char* argv[]){

  int tag = 1;
  int rank, size;
  int recv_data;

  MPI_Init(&argc, &argv);
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int left_rank  = (rank-1+size)%size;
  int right_rank = (rank+1)%size;

  MPI_Sendrecv(&rank, 1, MPI_INT, right_rank, tag, &recv_data, 1, MPI_INT, left_rank, tag, MPI_COMM_WORLD, &status);
  std::printf(" Process %d recevived from %d\n", rank, recv_data);

  MPI_Finalize();
  return 0;
}
