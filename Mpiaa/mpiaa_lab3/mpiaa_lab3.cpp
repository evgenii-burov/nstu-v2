#include <iostream>
#include <stdio.h>
#include <omp.h>
#include "mpi.h"
#include <Windows.h>
#include <chrono>
#include <thread>
//#1
// 
int main(int argc, char** argv)
{
	int rank, num_proc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Request* requests = new MPI_Request[num_proc];
	double vector_norm;
	int vector_size = 100;
	if (rank != 0)
	{
		double* vector = new double[vector_size];
		vector_norm = 0;
		for (int i = 0; i < vector_size; i++)
		{
			vector[i] = i % 10 + rank;
			vector_norm += vector[i];
		}
		vector_norm = sqrt(vector_norm);
		if (rank == 1)
		{
			Sleep(10000);
		}
		MPI_Isend(&vector_norm, 1, MPI_DOUBLE, 0, 77, MPI_COMM_WORLD, &requests[rank]);
		delete[] vector;
	}
	if (rank == 0)
	{
		double* data_recvd = new double[num_proc];
		MPI_Status* statuses = new MPI_Status[num_proc];
		for (int i = 1; i < num_proc; i++)
		{
			MPI_Irecv(&data_recvd[i], 1, MPI_DOUBLE, i, 77, MPI_COMM_WORLD, &requests[i]);
		}
		int* flags_recvd = new int[num_proc];
		int* flags_written = new int[num_proc];
		for (int i = 0; i < num_proc; i++)
		{
			flags_written[i] = 0;
		}
		int flag_all_recvd = 1;
		do
		{
			flag_all_recvd = 1;
			for (int i = 1; i < num_proc; i++)
			{
				std::cout << std::flush;
				MPI_Test(&requests[i], &flags_recvd[i], &statuses[i]);
				if (flags_recvd[i] && !flags_written[i])
				{
					std::cout << "Received data from " << i << ", data: " << data_recvd[i] << "\n";
					flags_written[i] = 1;
				}
				if (!flags_recvd[i])
				{
					flag_all_recvd = 0;
				}
			}
		} while (!flag_all_recvd);
	}
	MPI_Finalize();
	if(rank==0)
		std::cout << "\n";
	return 0;
}
