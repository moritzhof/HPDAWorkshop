// Hello World example using OpenMP

#include <iostream>
#include <omp.h>

int main()
{
    int threadId = -1;
    // start parallel region
    #pragma omp parallel 
    {
        // get the id of the thread
        threadId = omp_get_thread_num();

        if (threadId == 0)
        {
            int numThreads = omp_get_num_threads();
            //std::printf("We have %d threads!\n", numThreads);
            std::cout << "We have " << numThreads << " threads available\n";
        }
        std::cout << "Hello world from thread number " << threadId << "!\n";
        //std::printf("Hello world from thread %d.\n", threadId);
    } // implicit barrier
}

