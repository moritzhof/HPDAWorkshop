/*##############################################################################
##################### Send Recv Example: #######################################
#################### Created: Moritz Hof #######################################
###############################################################################*/

/*  ╰─ mpic++ sendrecv.cpp -o sendrecv -std=c++17

    ╰─ mpiexec -n 2 sendrecv

    if you are using the ubuntu subsystem in windows please you:

    ╰─ mpiexec --mca btl_vader_single_copy_mechanism none -np 2 sendrecv

    as there is a security issue that has to be overlooked between the communication
    of the two OSs
*/



// For this example number of processor must be 2

#include<iostream>
#include<mpi.h>
#include<memory>

auto main(int argc, char* argv[])->int{

  int numprocs, rank, tag = 100, msg_size = 64;
  char* buf = nullptr; //if C API: NULL;

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);


  if(numprocs!=2){
    std::printf("The number of process must be equal to 2\n");
    MPI_Finalize();
    return(0);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::printf("The process %d started ... \n", rank);
  fflush(stdout);
  //The flush isnt really necessary. I am just putting it in since for std::endl,
  // performs newline and the does a flush. So you will see a flush after every
  // printf statement.
  while(msg_size < 1000000){

    msg_size *=2;
    // if C API
    //buf = (char*)malloc(msg_size*sizeof(char));
    buf = new char[msg_size];
    if(rank == 0){

      MPI_Send(buf, msg_size, MPI_BYTE, rank+1, tag, MPI_COMM_WORLD);
      std::printf("Message of length %d sent to process %d\n", msg_size, rank+1);
      fflush(stdout);
      MPI_Recv(buf, msg_size, MPI_BYTE, rank+1, tag, MPI_COMM_WORLD, &status);
      std::printf("Message of length %d received from process %d\n", msg_size, rank+1);
      fflush(stdout);
    }
    if(rank == 1){
      //Correspnding resv to the send comming from rank-1, aka 0:
      // commit this out and and uncomment out the bottom MPI_Recv to get a deadlock ...
      MPI_Recv(buf, msg_size, MPI_BYTE, rank-1, tag, MPI_COMM_WORLD, &status);
      std::printf("Message of length %d received from process %d\n", msg_size, rank-1);
      fflush(stdout);

      MPI_Send(buf, msg_size, MPI_BYTE, rank-1, tag, MPI_COMM_WORLD);
      std::printf("Message of length %d send to process %d\n", msg_size, rank-1);
      fflush(stdout);

      //MPI_Recv(buf, msg_size, MPI_BYTE, rank-1, tag, MPI_COMM_WORLD, &status);
      //std::printf("Message of length %d received from process %d\n", msg_size, rank-1);
      //fflush(stdout);
    }
  // if C API
  //free(buf)
  delete[] buf;
  }
  MPI_Finalize();
  return 0;
}
