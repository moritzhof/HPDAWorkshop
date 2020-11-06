// Numerical integration example using OpenMP

#include <cmath>
#include <iostream>
#include <chrono>

template <typename Func>
[[nodiscard]] double midpointRule(double xmin, double xmax, Func func, int numPts)
{
    double stepSize = (xmax - xmin) / numPts;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (int idx = 0; idx <= numPts; ++idx)
    {
        sum += func(xmin + (idx + 0.5) * stepSize);
    }

    return sum * stepSize;
}

template <typename Func>
[[nodiscard]] double trapezoidalRule(double xmin, double xmax, Func&& func, int numPts)
{
    double stepSize = (xmax - xmin) / numPts;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (int idx = 0; idx <= numPts; ++idx)
    {
        // (f(x(k+1)) + f(x(k))) / 2
        sum += (func(xmin + (idx + 1) * stepSize) + func(xmin + idx * stepSize)) / 2.0;
    }

    return sum * stepSize;
}

int main()
{
    auto riemannzeta = [](double x) { return std::riemann_zeta(x); };

    double xmin = 2;
    double xmax = 5;
    int numPts = 1000;

    auto beginMP = std::chrono::steady_clock::now();
    double midPnt = midpointRule(xmin, xmax, riemannzeta, numPts);
    auto endMP = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endMP - beginMP).count();

    std::cout << "Midpoint rule: The computation took " << duration << " nanoseconds.\n";
    std::cout << "The value is: " << midPnt << '\n';

    auto beginTrap = std::chrono::steady_clock::now();
    double trap = trapezoidalRule(xmin, xmax, riemannzeta, numPts);
    auto endTrap = std::chrono::steady_clock::now();
    auto durationTrap = std::chrono::duration_cast<std::chrono::nanoseconds>(endTrap - beginTrap).count();

    std::cout << "Trapezoidal rule: The computation took " << durationTrap << " nanoseconds.\n";
    std::cout << "The value is: " << trap << '\n';
}