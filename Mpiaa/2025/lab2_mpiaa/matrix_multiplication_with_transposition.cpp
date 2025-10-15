#include "mpiaalab2.h"

void matrix_transposition(const int n, const double* matrix, double*& matrix_T)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matrix_T[i * n + j] = matrix[j * n + i];
		}
	}
}

void matrix_multiplication_T_sequential(const int n, const int m, const int k,
	const double* matrix_a, const double* matrix_b_T, double*& matrix_c)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < k; j++)
		{
			matrix_c[i * k + j] = 0;
			for (int l = 0; l < m; l++)
			{
				matrix_c[i * k + j] += matrix_a[i * m + l] * matrix_b_T[j * m + l];
			}
		}
	}
}

void matrix_multiplication_T_parallel(const int n, const int m, const int k,
	const double* matrix_a, const double* matrix_b_T, double*& matrix_c, const int num_threads)
{
#pragma omp parallel for num_threads(num_threads)
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < k; j++)
		{
			double sum_over_l = 0;
#pragma omp parallel reduction(+:sum_over_l)
			for (int l = 0; l < m; l++)
			{
				sum_over_l += matrix_a[i * m + l] * matrix_b_T[j * m + l];
			}
			matrix_c[i * k + j] = sum_over_l;
		}
	}
}

void matrix_transposition_task(const int n, const int num_threads)
{
	double* matrix_a = new double[n * n];
	double* matrix_b = new double[n * n];
	double* matrix_c = new double[n * n];
	for (int i = 0; i < n * n; i++)
	{
		matrix_a[i] = i % 10;
		matrix_b[i] = i % 8;
	}

	double matrix_c_norm = 0;

	double* matrix_b_T = new double[n * n];
	matrix_transposition(n, matrix_b, matrix_b_T);

	if (num_threads == 1)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		matrix_multiplication_T_sequential(n, n, n, matrix_a, matrix_b_T, matrix_c);
		auto t2 = std::chrono::high_resolution_clock::now();
		auto time = std::chrono::duration<double, std::milli>(t2 - t1).count();
		matrix_c_norm = matrix_norm(n, matrix_c);
		delete[] matrix_c;
		matrix_c = new double[n * n];

		std::cout << "N=" << n;
		std::cout << std::fixed << std::setprecision(15);
		std::cout << "\n* Sequential *\n";
		std::cout << "Time elapsed: " << time << "\nResult: " << matrix_c_norm << "\n";
	}
	if (num_threads > 1)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		matrix_multiplication_T_parallel(n, n, n, matrix_a, matrix_b_T, matrix_c, num_threads);
		auto t2 = std::chrono::high_resolution_clock::now();
		auto time_parallel = std::chrono::duration<double, std::milli>(t2 - t1).count();
		matrix_c_norm = matrix_norm(n, matrix_c);

		std::cout << "N=" << n;
		std::cout << std::fixed << std::setprecision(15);
		std::cout << "\n* Parallel, threads=" << num_threads << " *\n";
		std::cout << "Time elapsed: " << time_parallel << "\nResult: " << matrix_c_norm << "\n";
	}


	
	delete[] matrix_a;
	delete[] matrix_b;
	delete[] matrix_c;
	delete[] matrix_b_T;
}