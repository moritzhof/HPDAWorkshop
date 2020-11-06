#include "matrixmul.h"

Matrix matrixMul(Matrix const& matrixA, Matrix const& matrixB)
{
    auto ncolsA = matrixA.ncols();
    auto nrowsA = matrixA.nrows();
    auto ncolsB = matrixB.ncols();
    auto nrowsB = matrixB.nrows();

    if (ncolsA != ncolsB or nrowsA != nrowsB)
    {
        std::exit(0);
    }
    
    Matrix result{nrowsA, ncolsB};
    #pragma omp parallel for
    for (int row = 0; row < result.nrows(); ++row)
    {
        for (int col = 0; col != result.ncols(); ++col)
        {
            for (int el = 0; el != nrowsB; ++el)
            {
                // result[row][col] += matrixA[row][el] * matrixB[el][col]
                *result(row * nrowsA + col) += matrixA[row * nrowsA + el] 
                                               * 
                                               matrixB[el * ncolsB + col];
            }
        }
    }
    return result;
}

int main()
{
    constexpr int size = 2'000;

    auto matA = Matrix::makeRandom(size);
    auto matB = Matrix::makeRandom(size);

    auto begin = std::chrono::steady_clock::now();
    auto res = matrixMul(matA, matB);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    std::cout << "Matrix multiplication: The computation took " << duration << " nanoseconds.\n";
}
