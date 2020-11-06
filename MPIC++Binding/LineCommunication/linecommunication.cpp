/* Created Moritz Hof DLR: Computational Engineer
  Communction is a ring:

  Depiction if considering 4 processes
     0 --> 1 -->2 -->3 --> 4 --> no where

*/

#include<iostream>
#include<mpi.h>


auto main(int argc, char* argv[]) -> int{

  int tag = 30;
  MPI::Init(argc, argv);
  int rank = MPI::COMM_WORLD.Get_rank();
  int size = MPI::COMM_WORLD.Get_size();

  int right_rank = rank+1;
  if(rank == size-1){
    right_rank = MPI::PROC_NULL;
  }

  int left_rank = rank-1;
  if(rank == 0){
    left_rank = MPI::PROC_NULL;
  }
  int recv_data = 123; // only process 0 will hold this value;

  MPI::COMM_WORLD.Sendrecv(&rank, 1, MPI::INT, right_rank, tag, &recv_data, 1, MPI::INT, left_rank, MPI::ANY_TAG);

  std::cout << "Process " << rank << " received from " << recv_data << std::endl;
  MPI::Finalize();

  return 0;
}
