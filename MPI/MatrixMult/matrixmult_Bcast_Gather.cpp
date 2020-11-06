#include<iostream>
#include<mpi.h>

const int SIZE = 16;
int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];


//############################# Useful Matrix Functions ########################
void build_matrix(int M[SIZE][SIZE]){
  static int n = 0; //static since I want to the matrices to be different. A from 0 - 255 B from 256 -5 11
  for(int i = 0; i < SIZE; ++i){
    for(int j = 0; j < SIZE; ++j){
      M[i][j] = n++;
    }
  }
}



void print_matrix(int M[SIZE][SIZE]){
  for(auto i = 0; i < SIZE; ++i){
    for(auto j = 0; j < SIZE; ++j){
      std::cout << M[i][j] << " ";
    }
    std::cout<<'\n';
  }
}


// Takes the serial multiplication and parallel multiplication and subtracts them. When the
// resulting matrix the Zero-Matrix is, the result is correct
void verify(int A[SIZE][SIZE], int B[SIZE][SIZE]){
  for(auto i = 0; i < SIZE; ++i){
    for(auto j=0; j < SIZE; ++j){
        A[i][j] = A[i][j]-B[i][j];
    }
  }
  print_matrix(A);
}

//################################## Main ######################################
auto main(int argc, char* argv[])->int{

int rank, numproc;
MPI_Status status;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &numproc);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

if(SIZE%numproc != 0){
  if(rank == 0){ std::printf("Matrix size must be divisible by number of processors");}
  MPI_Finalize();
}

//Create the Matrices
if(rank == 0){
  build_matrix(A);
  //print_matrix(A);
  build_matrix(B);
  //print_matrix(B);
}

int from = rank*SIZE/numproc;
int to = (rank+1)*SIZE/numproc;

MPI_Bcast(B, SIZE*SIZE, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Scatter(A, SIZE*SIZE/numproc, MPI_INT, A[from], SIZE*SIZE/numproc, MPI_INT, 0, MPI_COMM_WORLD);

std::printf("COmputing slice %d from row %d, to %d\n", rank, from, to-1);
for(int i = from; i < to; ++i){
  for(int j = 0; j < SIZE; ++j){
    C[i][j] = 0;
    for(int k = 0; k < SIZE; ++k){
      C[i][j] += A[i][k]*B[k][j];
    }
  }
}

MPI_Gather(C[from], SIZE*SIZE/numproc, MPI_INT, C, SIZE*SIZE/numproc, MPI_INT, 0, MPI_COMM_WORLD);

if(rank == 0){
  print_matrix(C);
}


MPI_Finalize();
return 0;
}
