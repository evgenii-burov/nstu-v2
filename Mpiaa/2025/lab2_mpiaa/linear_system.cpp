#include "mpiaalab2.h"

void solve_Uy_b_sequential(const int n, const double* matrix_U, double*& vector_y, const double* vector_b)
{
	for (int i = n - 1; i >= 0; i--)
	{
		double sum_over_j = 0;
		for (int j = i + 1; j < n; j++)
		{
			sum_over_j += matrix_U[i * n + j] * vector_y[j];
		}
		vector_y[i] = (vector_b[i] - sum_over_j) / matrix_U[i * (n + 1)];
	}
}

void solve_Uy_b_parallel(const int n, const double* matrix_U, double*& vector_y, const double* vector_b, const int num_threads)
{
	for (int i = n - 1; i >= 0; i--)
	{
		double sum_over_j = 0;
#pragma omp parallel for reduction(+:sum_over_j)
		for (int j = i + 1; j < n; j++)
		{
			sum_over_j += matrix_U[i * n + j] * vector_y[j];
		}
		vector_y[i] = (vector_b[i] - sum_over_j) / matrix_U[i*(n+1)];
	}
}

void linear_system_task(const int n, const int num_threads)
{
	double* matrix_U = new double[n * n];
	double* vector_x = new double[n];
	double* vector_y = new double[n];
	double* vector_b = new double[n];
	for (int i = 0; i < n; i++)
	{
		vector_x[i] = i % 10;
		for (int j = 0; j < i; j++)
		{
			matrix_U[i * n + j] = 0;
		}
		matrix_U[i * (n + 1)] = 10;
		for (int j = i+1; j < n; j++)
		{
			matrix_U[i * n + j] = 1;
		}
	}

	matrix_multiplication_sequential(n, n, 1, matrix_U, vector_x, vector_b);

	double vector_x_norm = sqrt(dot_product_sequential(n, vector_x, vector_x));
	double vector_y_norm;
	if (num_threads == 1)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		solve_Uy_b_sequential(n, matrix_U, vector_y, vector_b);
		auto t2 = std::chrono::high_resolution_clock::now();
		auto time = std::chrono::duration<double, std::milli>(t2 - t1).count();
		double vector_y_norm = sqrt(dot_product_sequential(n, vector_y, vector_y));

		std::ofstream output_stream("vectors_x_and_y.txt");
		output_stream << "Vector x:\n";
		for (int i = 0; i < n; i++)
		{
			output_stream << vector_x[i] << "\t";
		}
		output_stream << "\n\nVector y:\n";
		for (int i = 0; i < n; i++)
		{
			output_stream << vector_y[i] << "\t";
		}

		std::cout << "N=" << n;
		std::cout << std::fixed << std::setprecision(3);
		std::cout << "\n* Sequential *\n";
		std::cout << "Time elapsed: " << time;
		std::cout << "\nVector x norm: " << vector_x_norm;
		std::cout << "\nVector y norm: " << vector_y_norm << "\n";
	}

	if (num_threads > 1)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		solve_Uy_b_parallel(n, matrix_U, vector_y, vector_b, num_threads);
		auto t2 = std::chrono::high_resolution_clock::now();
		auto time = std::chrono::duration<double, std::milli>(t2 - t1).count();
		double vector_y_norm = sqrt(dot_product_sequential(n, vector_y, vector_y));

		std::ofstream output_stream("vectors_x_and_y.txt");
		output_stream << "Vector x:\n";
		for (int i = 0; i < n; i++)
		{
			output_stream << vector_x[i] << "\t";
		}
		output_stream << "\n\nVector y:\n";
		for (int i = 0; i < n; i++)
		{
			output_stream << vector_y[i] << "\t";
		}

		std::cout << "N=" << n;
		std::cout << std::fixed << std::setprecision(3);
		std::cout << "\n* Parallel, threads=" << num_threads << " *\n";
		std::cout << "Time elapsed: " << time;
		std::cout << "\nVector x norm: " << vector_x_norm;
		std::cout << "\nVector y norm: " << vector_y_norm << "\n";
	}

	delete[] matrix_U;
	delete[] vector_x;
	delete[] vector_y;
	delete[] vector_b;
}
