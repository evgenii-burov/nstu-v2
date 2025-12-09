#include "mpi_tasks.h"

void task3_sequential(int argc, char** argv)
{
	int n = 4;

	double* vector_b = new double[n];
	double** matrix_a = new double* [n];

	for (int i = 0; i < n; i++)
	{
		vector_b[i] = i % 10;
		matrix_a[i] = new double[n];
		for (int j = 0; j < n; j++)
		{
			matrix_a[i][j] = (i % 10) + 1;
		}
	}

	double* resulting_vector = new double[n];

	for (int i = 0; i < n; i++)
	{
		resulting_vector[i] = 0;
		for (int j = 0; j < n; j++)
		{
			resulting_vector[i] += matrix_a[i][j] * vector_b[j];
		}
	}

	double resulting_vector_norm = 0;

	for (int i = 0; i < n; i++)
	{
		resulting_vector_norm += resulting_vector[i] * resulting_vector[i];
	}

	resulting_vector_norm = sqrt(resulting_vector_norm);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cout << matrix_a[i][j] << "\t";
		}
		std::cout << "\n";
	}

	std::cout << "\n";
	for (int i = 0; i < n; i++)
	{
		std::cout << vector_b[i] << "\n";
	}

	std::cout << "\n";
	for (int i = 0; i < n; i++)
	{
		std::cout << resulting_vector[i] << "\n";
	}

	delete[] vector_b;
	delete[] resulting_vector;
	for (int i = 0; i < n; i++)
	{
		delete[] matrix_a[i];
	}
}

void task3_parallel(int argc, char** argv)
{

}