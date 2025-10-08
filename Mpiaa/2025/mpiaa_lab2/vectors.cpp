#include "mpiaalab2.h"

double dot_product_sequential(const int n, const double* vector_a, const double* vector_b)
{
	double result = 0;
	for (int i = 0; i < n; i++)
	{
		result += vector_a[i] * vector_b[i];
	}
	return result;
}

double dot_product_parallel(const int n, const double* vector_a, const double* vector_b)
{
	double result = 0;
#pragma omp parallel for reduction(+: result)
	for (int i = 0; i < n; i++)
	{
		result += vector_a[i] * vector_b[i];
	}
	return result;
}

void vectors_task(const int n)
{
	double* vector_a = new double[n];
	for (int i = 0; i < n; i++)
		vector_a[i] = i % 10;
	double dot_product = 0;
	auto t1 = std::chrono::high_resolution_clock::now();
	dot_product = dot_product_sequential(n, vector_a, vector_a);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration<double>(t2-t1).count();
	std::cout << "* Sequential, N=" << n << " *\n";
	std::cout << "Time elapsed: " << time << "\nResult: " << dot_product << "\n";
}