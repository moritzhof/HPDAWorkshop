/* Created Moritz Hof DLR - HPC - Computational Engineer
Modern C++ objects with MPI C++ Binding
Note: This will also work with the C-syntax
*/

#include<iostream>
#include<mpi.h>
#include<vector>
#include<chrono>
#include<random>
#include<algorithm>
#include<tuple>

const int size = 16;

std::vector<double> a(size*size, 0);
std::vector<double> b(size*size, 0);
std::vector<double> c(size*size, 0);

//UPdate later to create randomw matrices
void fill_matrix(std::vector<double>& A, int s = size){

  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(1.0, 10.0);

  for(auto i(0); i < s; ++i){
    for(auto j(0); j < s; ++j){
        A[i*s+j] = dis(gen);
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

std::vector<double> copy_vector(std::vector<double>& input_vector){
  std::vector<double> vector;
  for(auto iter = input_vector.begin(); iter != input_vector.end(); ++iter){
    vector.push_back(*iter);
  }
  return vector;
}
// could also do:   std::vector<int> copied_vector;
//                  std::copy(input_vector.begin(), input_vector.end(), std::back_inserter(copied_vector));
//in main or write a wrapper function for it

std::vector<double> Parallel_MatMult(){
    int i , j, k;
    int from, to;
    int rank     = MPI::COMM_WORLD.Get_rank();
    int numproc  = MPI::COMM_WORLD.Get_size();

    if(rank == 0){ fill_matrix(a); fill_matrix(b);}

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
      // if(rank == 0){
      //   print_matrix(c);
      // }
     return c;
}

std::vector<double> Serial_MatMul(std::vector<double>& a, std::vector<double>& b, std::vector<double>& c){

  for(auto i = 0; i < size; ++i){
    for(auto j=0; j < size; ++j){
      for(auto k=0; k < size; ++k){
        c[i*size+j] += a[i*size+k]*b[k*size+j];
      }
    }
  }
  //print_matrix(c);
  return c;
}

// Takes the serial multiplication and parallel multiplication and subtracts them. When the
// resulting matrix the Zero-Matrix is, the result is correct
void verify(std::vector<double>& A, std::vector<double>& B, int s = size){
  for(auto i = 0; i < size; ++i){
    for(auto j=0; j < size; ++j){
        A[i*size+j] = A[i*size+j]-B[i*size+j];
    }
  }
  print_matrix(A);
}


bool verify_modern(std::vector<double>& A, std::vector<double>& B){

 std::vector<double> vector;

  for(std::pair<std::vector<double>::iterator, std::vector<double>::iterator> iter(A.begin(), B.begin()); iter.first!=A.end(); ++iter.first, ++iter.second){
    vector.push_back(*iter.first - *iter.second);
  }

  auto result = all_of(vector.begin(), vector.end(), [](int i){ return i == 0; });
  if(result){std::cout << " The solution is correct" << '\n';}
  else{ std::cout << "The solution is NOT correct" << '\n';}

  print_matrix(vector);
  std::cout << std::endl;
  return result;
}

void verify_modern2(std::vector<double>& A, std::vector<double>& B){
//without creating new vector/matrix;
  for(std::pair<std::vector<double>::iterator, std::vector<double>::iterator> iter(A.begin(), B.begin()); iter.first!=A.end(); ++iter.first, ++iter.second){
    std::replace(A.begin(), A.end(), *iter.first, (*iter.first-*iter.second));
  }
  auto result = all_of(A.begin(), A.end(), [](int i){ return i == 0; });
  print_matrix(A);
  result == true ? std::cout << "The solution is correct" << '\n' : std::cout << "The solution is NOT correct" << std::endl;
}

// for verification one could also use std::transform to also do the subtraction step. what is nice about std::transform, is that one could also use the c++ parallel
// execution policy:
// std::transform(std::execution::par_unseq, std::begin(a[i]), std::end(a[i]), std::begin(b[i]), std::begin(a[i]), std::minus<double>());
// you need c++17 or above for this to work with a current compiler.
auto main(int argc, char* argv[])->int{

 std::vector<double> sol_parallel;
 std::vector<double> sol_serial;
 std::vector<double> a_serial;
 std::vector<double> b_serial;
 std::vector<double> c_serial(size*size, 0);
//int result = 0;
// ###################### Parallel #############################################
  MPI::Init(argc, argv);
  auto time = MPI::Wtime();
     sol_parallel=Parallel_MatMult();
  auto time1 = MPI::Wtime();

  if(MPI::COMM_WORLD.Get_rank() == 0){
    std::cout << "MPI time: " << time1-time <<std::endl;
    a_serial = copy_vector(a); b_serial = copy_vector(b);
    sol_serial = Serial_MatMul(a_serial,b_serial,c_serial);
    verify_modern(c, c_serial);
  }

  MPI::Finalize();
 return 0;
}
