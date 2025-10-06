#include "lab2mpiaa.h"

double Computing::dot_product_sequential(const int n, const double* vector_a, const double* vector_b)
{
	double result = 0;
	for (int i = 0; i < n; i++)
	{
		result += vector_a[i] * vector_b[i];
	}
	return result;
}

double Computing::dot_product_parallel(const int n, const double* vector_a, const double* vector_b)
{
	double result = 0;
#pragma omp parallel for reduction(+: result)
	for (int i = 0; i < n; i++)
	{
		result += vector_a[i] * vector_b[i];
	}
	return result;
}

void Computing::initialize_vector(const int n, double*& vector)
{
	if (vector != NULL)
	{
		delete[] vector;
	}
	vector = new double[n];
	for (int i = 0; i < n; i++)
	{
		vector[i] = i % 10 + 1;
	}
}

void Computing::print_vector(const int n, const double* vector)
{
	for (int i = 0; i < n; i++)
	{
		std::cout << vector[i] << " ";
	}
	std::cout << "\n";
}