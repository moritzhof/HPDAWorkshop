// Created Moritz Hof, DLR-HPC

// This a c++ api for mpi but unfortuntely the c++ binding has been discontuned as
// for openMPI with version 2.3 (I believe). It still works but new functions for
// versions 2.3 and higher will only have the C style syntax
// but for the interested persons, here a small example.

// compile: mpic++ helloworldmodern.cpp -o helloworldmodern
// execute: mpirun --mca btl_vader_single_copy_mechanism none -np 4 helloworldmodern
#include<iostream>
#include<mpi.h>


auto main(int argc, char* argv[])->int{

  MPI::Init(argc, argv);

  int num_procs = MPI::COMM_WORLD.Get_size();
  int rank      = MPI::COMM_WORLD.Get_rank();

  std::cout << "Hello world from process " << rank << " of " << num_procs << '\n';
  MPI::Finalize();
  return 0;
}
