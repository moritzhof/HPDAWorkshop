/* Created Moritz Hof DLR - HPC - Computational Engineer
Modern C++ objects with MPI C++ Binding
Note: This will also work with the C-syntax
*/

#include<iostream>
#include<mpi.h>
#include<vector>


const int size = 32;

std::vector<double> a(size*size, 0);
std::vector<double> b(size*size, 0);
std::vector<double> c(size*size, 0);


//UPdate later to create randomw matrices
void fill_matrix(std::vector<double>& A, int s = size){
  for(auto i(0); i < s; ++i){
    for(auto j(0); j < s; ++j){
        A[i*s+j] = 1.0;
    }
  }
}

void print_matrix(std::vector<double>& A, int s = size){
  for(auto i(0); i < s; ++i){
    for(auto j(0); j < s; ++j){
        std::cout<< A[i*s+j] << ' ';
    }
    std::cout<<'\n';
  }
}


auto main(int argc, char* argv[])->int{

  int i , j, k;

  int from, to;

  MPI::Init(argc, argv);

  int rank     = MPI::COMM_WORLD.Get_rank();
  int numproc  = MPI::COMM_WORLD.Get_size();

  if(rank == 0){
    fill_matrix(a);
    fill_matrix(b);
    //print_matrix(a);
    //print_matrix(b);
    //print_matrix(c);
  }

  from = rank*size/numproc;
  to   = (rank+1)*size/numproc;

    unsigned len = b.size();

    MPI::COMM_WORLD.Bcast(&len, 1, MPI::UNSIGNED, 0);
    if(len!=0){
       MPI::COMM_WORLD.Bcast(b.data(), len, MPI::DOUBLE, 0);
    }

    MPI::COMM_WORLD.Scatter(a.data(), size*size/numproc, MPI::DOUBLE, &a[from*size], size*size/numproc, MPI::DOUBLE, 0);


    for(i = from; i < to; ++i){
      for(j=0; j < size; ++j){
        for(k=0; k < size; ++k){
          c[i*size+j] += a[i*size+k]*b[k*size+j];
        }
      }
    }

  MPI::COMM_WORLD.Gather(&c[from*size], size*size/numproc, MPI::DOUBLE, c.data(), size*size/numproc, MPI::DOUBLE, 0);

   if(rank == 0){
     print_matrix(c);
   }

  MPI::Finalize();
}
