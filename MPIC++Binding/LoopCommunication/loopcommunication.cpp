/* Created Moritz Hof DLR: Computational Engineer
  Communction in a ring:

  Depiction if considering 4 processes
    ... --> 0 --> 1 -->2 -->3 --> 4 --> 0 --> ....
*/

#include<iostream>
#include<mpi.h>


auto main(int argc, char* argv[]) -> int{
  int tag = 1;
  MPI::Init(argc, argv);
  int rank = MPI::COMM_WORLD.Get_rank();
  int size = MPI::COMM_WORLD.Get_size();

  int left_rank  = (rank-1+size)%size;
  int right_rank = (rank+1)%size;
  int recv_data;
  MPI::COMM_WORLD.Sendrecv(&rank, 1, MPI::INT, right_rank, tag, &recv_data, 1, MPI::INT, left_rank, tag );

  std::cout << "Process " << rank << " received from " << recv_data << std::endl;
  MPI::Finalize();

  return 0;
}
