#include <fstream>
#include <stddef.h>
#include <vector>
#include <string>
#include <iostream>
#include <omp.h>
#include <chrono>


using Range = std::pair<double, double>;
using Resolution = std::pair<int, int>;

class Mandelbrot
{
    std::vector<int> d_iters;
    Range d_xrange;
    Range d_yrange;
    Resolution d_res;
    int d_maxIter;
    
public:
    Mandelbrot(Range xrange, Range yrange, Resolution res, int maxIter);

    void writeImage(std::string const& fileName);
    void computeMandelbrot();
private:
    int calculateColor(int iter);
};

Mandelbrot::Mandelbrot(Range xrange, Range yrange, Resolution res, int maxIter)
:
    d_xrange{xrange},
    d_yrange{yrange},
    d_res{res},
    d_maxIter{maxIter}
{
    d_iters.reserve(d_res.first * d_res.second);
}

int Mandelbrot::calculateColor(int iter)
{
    // Change this function if you want to have a different coloring scheme
    return (255 * iter) / 33;
}

void Mandelbrot::writeImage(std::string const& fileName)
{
    std::ofstream myImage{fileName};
    if (myImage.is_open ()) 
    {
        myImage << "P3\n" << d_res.first << " " << d_res.second << " 255\n";
        for (int row = 0; row < d_res.first; ++row) 
        {
             for (int col = 0; col < d_res.second; ++col)  
             {
                  myImage << calculateColor(d_iters[row * d_res.first + col])
                          << ' ' << 0 << ' ' << 0 << '\n';
             }
        }
        myImage.close();
    }
}