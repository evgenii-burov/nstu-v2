#include "mpiaalab2.h"

int main()
{
    int n = 10;
    double* vector_a = new double[n];
    for (int i = 0; i < n; i++)
        vector_a[i] = i % 10;

    auto t1 = std::chrono::high_resolution_clock::now();
    double result_sequential = dot_product_parallel(n, vector_a, vector_a);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time_sequential = std::chrono::duration<double>(t2 - t1);
}
