#include "mpiaalab2.h"

double* matrix_multiplication_sequential(const int n, const double* matrix_a,
	const double* matrix_b)
{
	double* result = new double[n * n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			result[i * n + j] = 0;
			for (int k = 0; k < n; k++)
			{
				result[i * n + j] += matrix_a[i * n + k] * matrix_b[k * n + j];
			}
		}
	}
	return result;
}

double* matrix_multiplication_parallel(const int n, const double* matrix_a,
	const double* matrix_b, const int num_threads)
{
	double* result = new double[n * n];
#pragma omp parallel for num_threads(num_threads)
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			result[i*n+j] = 0;
			for (int k = 0; k < n; k++)
			{
				result[i*n+j] += matrix_a[i * n + k] * matrix_b[k * n + j];
			}
		}
	}
	return result;
}


double matrix_norm(const int n, const double* matrix)
{
	double result = 0;
	for (int i = 0; i < n * n; i++)
	{
		result += matrix[i] * matrix[i];
	}
	return sqrt(result);
}

void matrices_task(const int n, const int num_threads)
{
	double* matrix_a = new double[n*n];
	for (int i = 0; i < n*n; i++)
		matrix_a[i] = i % 10;
	double* resulting_matrix = NULL;
	double resulting_matrix_norm = 0;

	auto t1 = std::chrono::high_resolution_clock::now();
	resulting_matrix = matrix_multiplication_sequential(n, matrix_a, matrix_a);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration<double>(t2 - t1).count();
	resulting_matrix_norm = matrix_norm(n, resulting_matrix);
	//delete[] resulting_matrix;
	resulting_matrix = NULL;

	std::cout << std::scientific;
	std::cout << "N=" << n;
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "* Sequential *\n";
	std::cout << "Time elapsed: " << time << "\nResult: " << resulting_matrix_norm << "\n";

	t1 = std::chrono::high_resolution_clock::now();
	resulting_matrix = matrix_multiplication_parallel(n, matrix_a, matrix_a, num_threads);
	t2 = std::chrono::high_resolution_clock::now();
	auto time_parallel = std::chrono::duration<double>(t2 - t1).count();
	resulting_matrix_norm = matrix_norm(n, resulting_matrix);
	//delete[] resulting_matrix;
	std::cout << "* Parallel, threads=" << num_threads << " *\n";
	std::cout << "Time elapsed: " << time_parallel << "\nResult: " << resulting_matrix_norm << "\n";
	std::cout << "Speedup: " << time / time_parallel << ", speedup/threads: " << time / time_parallel / num_threads << "\n";
}