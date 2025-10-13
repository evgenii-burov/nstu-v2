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

double dot_product_parallel(const int n, const double* vector_a, const double* vector_b, const int num_threads)
{
	double result = 0;
#pragma omp parallel for reduction(+: result) num_threads(num_threads)
	for (int i = 0; i < n; i++)
	{
		result += vector_a[i] * vector_b[i];
	}
	return result;
}

void vectors_task(const int n, const int num_threads)
{
	double* vector_a = new double[n];
	double* vector_b = new double[n];
	for (int i = 0; i < n; i++)
	{
		vector_a[i] = i % 10;
		vector_b[i] = i % 8;
	}
	double dot_product = 0;

	auto t1 = std::chrono::high_resolution_clock::now();
	dot_product = dot_product_sequential(n, vector_a, vector_b);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration<double>(t2-t1).count();

	std::cout << "N=" << n;
	std::cout << std::fixed << std::setprecision(15);
	std::cout << "* Sequential *\n";
	std::cout << "Time elapsed: " << time << "\nResult: " << dot_product << "\n";

	t1 = std::chrono::high_resolution_clock::now();
	dot_product = dot_product_parallel(n, vector_a, vector_b, num_threads);
	t2 = std::chrono::high_resolution_clock::now();
	auto time_parallel = std::chrono::duration<double>(t2 - t1).count();

	std::cout << "* Parallel, threads=" << num_threads << " *\n";
	std::cout << "Time elapsed: " << time_parallel << "\nResult: " << dot_product << "\n";
	std::cout << "Speedup: " << time / time_parallel << ", speedup/threads: " << time/time_parallel/num_threads << "\n";

	delete[] vector_a;
	delete[] vector_b;
}