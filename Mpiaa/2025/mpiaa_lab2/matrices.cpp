#include <cmath>

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
	const double* matrix_b)
{
	double* result = new double[n];
#pragma omp parallel for
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
