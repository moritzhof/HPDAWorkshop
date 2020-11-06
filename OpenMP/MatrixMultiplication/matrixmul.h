// Matrix multiplication example using OpenMP

#include <vector>
#include <initializer_list>
#include <cstdlib>
#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <chrono>
#include <omp.h>

class Matrix
{
    std::vector<double> d_data;
    int d_nrows;
    int d_ncols;

public:
    Matrix(int nrows, int ncols);
    Matrix(std::initializer_list<double> entries, int nrows, int ncols);
    Matrix(std::vector<double> const& data, int nrows, int ncols);

    int nrows() const;
    int ncols() const;
    double* operator()(std::size_t idx);
    double const* operator()(std::size_t idx) const;
    double operator[](std::size_t idx);
    double operator[](std::size_t idx) const;
    friend std::ostream& operator<<(std::ostream& out, Matrix& mat);
    void randomEntries();
    static Matrix makeRandom(int nrows, int ncols);
    static Matrix makeRandom(int size);
};

Matrix Matrix::makeRandom(int size)
{
    return Matrix::makeRandom(size, size);
}

Matrix Matrix::makeRandom(int nrows, int ncols)
{
    std::random_device rndDevice;
    std::mt19937 mersenneEngine{rndDevice()};  // Generates random numbers
    std::uniform_real_distribution<double> dist{-10, 10};

    auto gen = [&dist, &mersenneEngine](){
                   return dist(mersenneEngine);
               };
    std::vector<double> entries(nrows * ncols);
    std::generate(std::begin(entries), std::end(entries), gen);
    return Matrix{entries, nrows, ncols};
}

void Matrix::randomEntries()
{
    std::random_device rndDevice;
    std::mt19937 mersenneEngine{rndDevice()};  // Generates random numbers
    std::uniform_real_distribution<double> dist{-10, 10};

    auto gen = [&dist, &mersenneEngine](){
                   return dist(mersenneEngine);
               };

    std::generate(std::begin(d_data), std::end(d_data), gen);
}

Matrix::Matrix(std::vector<double> const& data, int nrows, int ncols)
:
    d_data{data},
    d_nrows{nrows},
    d_ncols{ncols}
{}

std::ostream& operator<<(std::ostream& out, Matrix& mat)
{
    for (int row = 0; row != mat.d_nrows; ++row)
    {
        for (int col = 0; col != mat.d_ncols; ++col)
        {
            out << mat[row * mat.d_ncols + col] << ' ';
        }
        out << '\n';
    }
    return out;
}


Matrix::Matrix(int nrows, int ncols)
:
    d_data(nrows * ncols),
    d_nrows{nrows},
    d_ncols{ncols}
{}

double* Matrix::operator()(std::size_t idx)
{
    return &d_data[idx];
}

double const* Matrix::operator()(std::size_t idx) const
{
    return &d_data[idx];
}

double Matrix::operator[](std::size_t idx)
{
    return d_data[idx];
}

double Matrix::operator[](std::size_t idx) const
{
    return d_data[idx];
}

int Matrix::nrows() const
{
    return d_nrows;
}

int Matrix::ncols() const
{
    return d_ncols;
}

Matrix::Matrix(std::initializer_list<double> entries, int nrows, int ncols)
:
    d_data{entries},
    d_nrows{nrows},
    d_ncols{ncols}
{}