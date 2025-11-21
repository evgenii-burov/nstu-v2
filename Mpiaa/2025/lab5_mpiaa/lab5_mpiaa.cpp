#include <iostream>
#include <mpi.h>

int main(int argc, char** argv)
{
	//#1
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	std::cout << "\nSize: " << size;
	std::cout << "\nRank: " << rank;

	MPI_Finalize();
	//getchar();
	return 0;
}


