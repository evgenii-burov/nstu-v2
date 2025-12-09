#include "mpi_tasks.h"

void task3_sequential(int argc, char** argv)
{
	int n = 1000;

	double* vector_b = new double[n];
	double* matrix_a = new double [n * n];

	for (int i = 0; i < n; i++)
	{
		vector_b[i] = i % 10;
		for (int j = 0; j < n; j++)
		{
			matrix_a[i * n + j] = (i + j) % 20;
		}
	}

	double* resulting_vector = new double[n];

	for (int i = 0; i < n; i++)
	{
		resulting_vector[i] = 0;
		for (int j = 0; j < n; j++)
		{
			resulting_vector[i] += matrix_a[i * n + j] * vector_b[j];
		}
	}

	double resulting_vector_norm = 0;

	for (int i = 0; i < n; i++)
	{
		resulting_vector_norm += resulting_vector[i] * resulting_vector[i];
	}

	resulting_vector_norm = sqrt(resulting_vector_norm);

	std::cout << std::scientific << std::setprecision(15);
	std::cout << "Resulting vector norm is: " << resulting_vector_norm;

	delete[] vector_b;
	delete[] resulting_vector;
	delete[] matrix_a;
}

void task3_parallel(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, num_proc, n, band_length;

	n = 1000;

	double* matrix_a = new double[n * n];
	double* vector_b = new double[n];
	double* matrix_slice;
	double* vector_b_slice;
	double* resulting_vector = new double[n];
	double* resulting_vector_part;

	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (n % num_proc != 0)
	{
		std::cout << "Number of processes must divide n evenly";
		exit(0);
	}

	band_length = n / num_proc;
	matrix_slice = new double[band_length * n];
	resulting_vector_part = new double[band_length];

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
		{
			vector_b[i] = i % 10;
			for (int j = 0; j < n; j++)
			{
				matrix_a[i * n + j] = (i + j) % 20;
			}
		}
	}

	MPI_Scatter(matrix_a, band_length * n, MPI_DOUBLE, matrix_slice, band_length * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(vector_b, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	for (int i = 0; i < band_length; i++)
	{
		resulting_vector_part[i] = 0;
		for (int j = 0; j < n; j++)
		{
			resulting_vector_part[i] += matrix_slice[i * n + j] * vector_b[j];
		}
	}

	MPI_Gather(resulting_vector_part, band_length, MPI_DOUBLE, resulting_vector, band_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		double resulting_vector_norm = 0;

		for (int i = 0; i < n; i++)
		{
			resulting_vector_norm += resulting_vector[i] * resulting_vector[i];
		}

		resulting_vector_norm = sqrt(resulting_vector_norm);
		std::cout << std::scientific << std::setprecision(15);
		std::cout << "\nResulting vector norm is: " << resulting_vector_norm;

		delete[] matrix_a;
		delete[] vector_b;
		delete[] matrix_slice;
		delete[] resulting_vector;
		delete[] resulting_vector_part;
	}
	MPI_Finalize();
}