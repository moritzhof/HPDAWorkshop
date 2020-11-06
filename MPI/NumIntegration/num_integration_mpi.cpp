#include<iostream>
#include<math.h>
#include<mpi.h>

double func(double x){
  return (sin(x)/x);
} // my test function between [-50,50]: answer should be: 3.10323

double func2(double x){
  return -3*x*x+3;
} //[-1,1]

double func3(double x){
  return sin(10*x)*x;
} //[0, 2*M_PI]


double TrapezoidRule(int level, double xleft, double xright, double (*func)(double)){
  int nsteps = (int) pow(2.0,level)-1;
  double h = (xright-xleft)/pow(2.0,level);
  long double sum = 0.0;

  for(int i=1;i<nsteps;++i){
    sum += func(xleft + i*h);
  }
  sum *= 2;
  sum += func(xleft) + func(xright);
  sum *= 0.5*h;
  return sum;
} // doesnt work for func()

double MidpointRule(int level, double xleft, double xright, double (*func)(double)){
  int nsteps = (int) pow(2.0,level)-1;
  double h = (xright-xleft)/pow(2.0,level);
  double sum = 0.0;

  for(int i=0;i<=nsteps;++i)
    sum += func(xleft + (i+0.5)*h);
  sum *= h;

  return sum;
}

auto main(int argc, char* argv[])->int{
  int mynode, totalnodes;
  const double global_a = -50;  //0
  const double global_b =  50; //2*M_PI;
  const int levels = 10;
  double local_a,local_b,local_sum,answer;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &totalnodes);
  MPI_Comm_rank(MPI_COMM_WORLD, &mynode);

  local_a = global_a + mynode    *(global_b-global_a)/totalnodes;
  local_b = global_a + (mynode+1)*(global_b-global_a)/totalnodes;

  local_sum = MidpointRule(levels, local_a, local_b, func);
  //local_sum = TrapezoidRule(levels, local_a, local_b, func);

  MPI_Reduce(&local_sum,&answer,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

  if(mynode == 0){
    std::cout << "The value of the integral is: " << answer << std::endl;
  }

  MPI_Finalize();
  return 0;
}
