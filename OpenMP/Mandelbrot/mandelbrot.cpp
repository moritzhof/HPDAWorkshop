#include "mandelbrot.h"

void Mandelbrot::computeMandelbrot()
{
    double dx = (d_xrange.second - d_xrange.first) / d_res.first;
    double dy = (d_yrange.second - d_yrange.first) / d_res.second;

    #pragma omp parallel for schedule(dynamic)
    for (int row = 0; row < d_res.second; ++row)
    {
        double c_imag = d_yrange.first + row * dy;
        for (int col = 0; col != d_res.first; ++col)
        {
            double real = 0.0;
            double imag = 0.0;
            double realSquared = 0.0;
            double imagSquared = 0.0;
            double c_real = d_xrange.first + col * dx;

            int iter = 0;
            while (iter < d_maxIter && realSquared + imagSquared < 4.0)
            {
                realSquared = real * real;
                imagSquared = imag * imag;
                imag = 2 * real * imag + c_imag;
                real = realSquared - imagSquared + c_real;
                ++iter;
            }
            d_iters[row * d_res.first + col] = iter;
        }   
    }
}

int main()
{
    int resX = 2048;
    int resY = 2048;
    Range xrange = {-2.3, 0.7};
    Range yrange = {-1.5, 1.5};
    Resolution res = {resX, resY};
    int maxIter = 2500;
    
    Mandelbrot mandelbrot{xrange, yrange, res, maxIter};

    auto begin = std::chrono::steady_clock::now();
    mandelbrot.computeMandelbrot();
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    std::cout << "Mandelbrot OpenMP: The computation took " << duration << " nanoseconds.\n";
    std::cout << "We get: " << 1e9 * resX * resY / duration << " pixels per second\n";

    mandelbrot.writeImage("mandelbrotOpenMP.ppm");
}
