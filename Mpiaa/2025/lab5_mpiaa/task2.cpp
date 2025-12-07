#include "mpi_tasks.h"

void task2_1(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == size - 1)
	{
		std::string text = "Hello world!";
		MPI_Send(text.c_str(), text.size(), MPI_CHAR, 0, 1, MPI_COMM_WORLD);
	}

	if (rank >= 1 && rank <= size - 2)
	{
		int vector_size = 100000;
		std::vector<double> vector_a(100000, 0);
		for (int i = 0; i < vector_size; i++)
		{
			vector_a[i] = (i + rank) % 12;
		}

		double vector_norm = 0;
		for (int i = 0; i < vector_size; i++)
		{
			vector_norm += vector_a[i] * vector_a[i];
		}
		vector_norm = sqrt(vector_norm);

		MPI_Send(&vector_norm, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}

	if (rank == 0)
	{
		double vector_norm;
		const int max_message_size = 1024;
		char text[max_message_size];
		MPI_Status status;
		for (int i = 1; i < size - 1; i++)
		{
			MPI_Recv(&vector_norm, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			std::cout << "\n" << "Message from process #" << i << ": " << vector_norm;
		}
		MPI_Recv(&text, max_message_size, MPI_CHAR, size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int message_size;
		MPI_Get_count(&status, MPI_CHAR, &message_size);
		text[message_size] = '\0';
		std::cout << "\n" << "Message from process #" << size - 1 << ": " << text;
	}

	MPI_Finalize();
}

void task2_2(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == size - 1)
	{
		std::string text = "Hello world!";
		MPI_Send(text.c_str(), text.size(), MPI_CHAR, 0, 1, MPI_COMM_WORLD);
	}

	if (rank >= 1 && rank <= size - 2)
	{
		int vector_size = 100000;
		std::vector<double> vector_a(100000, 0);
		for (int i = 0; i < vector_size; i++)
		{
			vector_a[i] = (i + rank) % 12;
		}
			
		double vector_norm = 0;
		int num_threads = omp_get_max_threads();
#pragma omp parallel for reduction(+:vector_norm) num_threads(num_threads)
		for (int i = 0; i < vector_size; i++)
		{
			vector_norm += vector_a[i] * vector_a[i];
		}
		vector_norm = sqrt(vector_norm);

		MPI_Send(&vector_norm, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}

	if (rank == 0)
	{
		double vector_norm;
		const int max_message_size = 1024;
		char text[max_message_size];
		MPI_Status status;
		for (int i = 1; i < size - 1; i++)
		{
			MPI_Recv(&vector_norm, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			std::cout << "\n" << "Message from process #" << i << ": " << vector_norm;
		}
		MPI_Recv(&text, max_message_size, MPI_CHAR, size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int message_size;
		MPI_Get_count(&status, MPI_CHAR, &message_size);
		text[message_size] = '\0';
		std::cout << "\n" << "Message from process #" << size - 1 << ": " << text;
	}

	MPI_Finalize();
}

void task2_3(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == size - 1)
	{
		std::string text = "Hello world!";
		MPI_Send(text.c_str(), text.size(), MPI_CHAR, 0, 1, MPI_COMM_WORLD);
	}

	if (rank >= 1 && rank <= size - 2)
	{
		int vector_size = 100000;
		std::vector<double> vector_a(100000, 0);
		for (int i = 0; i < vector_size; i++)
		{
			vector_a[i] = (i + rank) % 12;
		}

		double vector_norm = 0;
		for (int i = 0; i < vector_size; i++)
		{
			vector_norm += vector_a[i] * vector_a[i];
		}
		vector_norm = sqrt(vector_norm);
		//if (rank == 1)
		//{
		//	Sleep(2000);
		//}
		MPI_Send(&vector_norm, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}

	if (rank == 0)
	{
		double vector_norm;
		const int max_message_size = 1024;
		char text[max_message_size];
		MPI_Status status;
		MPI_Request request;
		int message_received = 0;
		bool not_all_received = false;
		for (int i = 1; i < size - 1; i++)
		{
			MPI_Irecv(&vector_norm, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
		}
		do
		{
			not_all_received = false;
			for (int i = 1; i < size - 1; i++)
			{
				//MPI_Irecv(&vector_norm, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
				MPI_Test(&request, &message_received, &status);
				if (message_received != 0)
				{
					std::cout << "\n" << "Message from process #" << i << ": " << vector_norm;
				}
				else
				{
					not_all_received = true;
				}
			}
			//MPI_Irecv(&vector_norm, 1, MPI_DOUBLE, size-1, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
			//MPI_Test(&request, &message_received, &status);
			//if (message_received != 0)
			//{
			//	int message_size;
			//	MPI_Get_count(&status, MPI_CHAR, &message_size);
			//	text[message_size] = '\0';
			//	std::cout << "\n" << "Message from process #" << size - 1 << ": " << text;
			//}
			//else
			//{
			//	not_all_received = true;
			//}
		} while (not_all_received == true);
		//for (int i = 1; i < size - 1; i++)
		//{
		//	MPI_Recv(&vector_norm, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		//	std::cout << "\n" << "Message from process #" << i << ": " << vector_norm;
		//}
		//MPI_Recv(&text, max_message_size, MPI_CHAR, size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		//int message_size;
		//MPI_Get_count(&status, MPI_CHAR, &message_size);
		//text[message_size] = '\0';
		//std::cout << "\n" << "Message from process #" << size - 1 << ": " << text;
	}

	MPI_Finalize();
}