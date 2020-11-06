/*##############################################################################
############################### Hello World Example ############################
############################# Created: Moritz Travis Hof #######################
###############################################################################*/

#include<iostream>
#include<mpi.h>
//if you are using windows, must also include:
//#include<stdafx.h>

int main(int argc, char** argv){  //note can't have const here
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::printf("Hello world from processor %d of %d processors.\n", rank, size);

  MPI_Finalize();
  return 0;
}
