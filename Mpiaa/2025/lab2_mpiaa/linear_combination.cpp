#include "mpiaalab2.h"

void linear_combination_sequential(const int n, double*& vector_y, const double* vector_x, const double scalar)
{
	for (int i = 0; i < n; i++)
	{
		vector_y[i] += scalar * vector_x[i];
	}
}

void linear_combination_parallel(const int n, double*& vector_y, const double* vector_x, const double scalar, const int num_threads)
{
#pragma omp parallel for num_threads(num_threads)
	for (int i = 0; i < n; i++)
	{
		vector_y[i] += scalar * vector_x[i];
	}
}

void linear_combination_task(const int n, const int num_threads)
{
	double* vector_x = new double[n];
	double* vector_y = new double[n];
	double scalar = 12.34567;

	for (int i = 0; i < n; i++)
	{
		vector_x[i] = i % 10;
		vector_y[i] = i % 8;
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	linear_combination_sequential(n, vector_y, vector_x, scalar);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration<double>(t2 - t1).count();
	double vector_y_norm = sqrt(dot_product_parallel(n, vector_y, vector_y, 12));
	delete[] vector_y;
	vector_y = new double[n];
	for (int i = 0; i < n; i++)
		vector_y[i] = i % 8;

	std::cout << "N=" << n;
	std::cout << std::fixed << std::setprecision(15);
	std::cout << "* Sequential *\n";
	std::cout << "Time elapsed: " << time << "\nResult: " << vector_y_norm << "\n";

	t1 = std::chrono::high_resolution_clock::now();
	linear_combination_parallel(n, vector_y, vector_x, scalar, num_threads);
	t2 = std::chrono::high_resolution_clock::now();
	auto time_parallel = std::chrono::duration<double>(t2 - t1).count();
	vector_y_norm = sqrt(dot_product_parallel(n, vector_y, vector_y, 12));

	std::cout << "* Parallel, threads=" << num_threads << " *\n";
	std::cout << "Time elapsed: " << time_parallel << "\nResult: " << vector_y_norm << "\n";
	std::cout << "Speedup: " << time / time_parallel << ", speedup/threads: " << time / time_parallel / num_threads << "\n";

	delete[] vector_x;
	delete[] vector_y;
}