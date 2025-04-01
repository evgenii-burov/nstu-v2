#include <iostream>
#include <fstream>
#include <iomanip>
#include "functions.h"
#include <random>
#include <time.h>

int main()
{
	unsigned random_seed = unsigned(time(0));
	srand(random_seed);
	std::cout << "SEED: " << random_seed << '\n';
	//srand(unsigned(time(0)));
	//srand(5234); //12345

	int n = -1;
	int p = -1;
	int matrix_type = -1;
	int k = -1;
	readParameters(n, p, matrix_type, k);

	createMatrixFile(n, p, matrix_type, k);
	
	precision* b = new precision[n];
	precision** al = new precision* [n];
	for (int i = 0; i < n; i++)
	{
		al[i] = new precision[p + 1];
	}
	readMatrix(al, n, p, matrix_type);
	createVectorFile(al, n, matrix_type);
	readVector(b, n, matrix_type);
	printVariables(al, b, n, p);
	printDenseMatrix(al, n, p);
	solveUsingGauss(al, b, n, p);

	calculateDecomposition(al, n, p);
	printVariables(al, b, n, p);
	//PrintDenseMatrix(al, n, p);
	solveForX(al, b, n, p);
	//for (int i = 0; i < n; i++)
	//{
	//	delete[] al[i];
	//}
	//delete[] al;
	////delete[] b;
}
