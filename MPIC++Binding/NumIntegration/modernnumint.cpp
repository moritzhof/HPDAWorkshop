#include<iostream>
#include<mpi.h>
#include<math.h>
#include<functional>


double func(double x){
  return (std::sin(x)/x);
} // my test function between [-50,50]: answer should be: 3.10323

double MidpointRule(int level, double xleft, double xright, std::function<double(double)> func){
  int nsteps = static_cast<int>(std::pow(2.0,level)-1);
  double h = (xright-xleft)/pow(2.0,level);
  double sum = 0.0;

  for(int i=0;i<=nsteps;++i)
    sum += func(xleft + (i+0.5)*h);
  sum *= h;

  return sum;
}

auto main(int argc, char* argv[])->int{

  int mynode, totalnodes;
  const int levels = 10;
  const double global_a = -50, global_b = 50;
  double local_a, local_b, local_sum,answer;

  MPI::Init(argc, argv);

  mynode = MPI::COMM_WORLD.Get_rank();
  totalnodes = MPI::COMM_WORLD.Get_size();

  local_a = global_a + mynode    *(global_b-global_a)/totalnodes;
  local_b = global_a + (mynode+1)*(global_b-global_a)/totalnodes;

  local_sum = MidpointRule(levels, local_a, local_b, func);


  MPI::COMM_WORLD.Reduce(&local_sum,&answer,1, MPI::DOUBLE, MPI::SUM, 0);

  if(mynode == 0){ // could also do if(MPI::COMM_WORLD.Get_rank == 0)
      std::cout << "The value of the integral is: " << answer << std::endl;
}


  MPI::Finalize();
  return 0;
}
