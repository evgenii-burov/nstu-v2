#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>

typedef double precision;
/*
ћатрица из варианта 1. –азмер блока в реализации блочной
релаксации переменный. »сследовать зависимость скорости сходимости от размеров блока.
*/

//j_dense = i + shift(j_diag)
class LinearSystem
{
private:
	//matrix dimension
	int n;
	//distance between -2nd and -1st, and 1st and 2nd diagonals, 
	// including both (if there are no 0-diagonals in-between, m=2)
	int m;
	//in diagonal format (n*5)
	std::vector<std::vector<precision>> matrix;
	std::vector<precision> x;
	std::vector<precision> b;
	std::vector<int> shift;
	int max_iterations = 10000;
	precision eps = .00001;
	precision omega_jacobi = 2. / 3;
	precision omega_gauss_seidel = .5;
	precision omega_block_relaxation = .5;
	
	void initializeMatrix(int matrix_type);
	void initializeVectorB(int matrix_type);
	void initializeVectorX(int matrix_type);

	precision getRelativeDiscrepancy() const;
	precision getScalarProductOfIthRowAndX(int i, std::vector<precision> x) const;
	precision getNextIterationOfXIth(int i, precision omega) const;
	//receives i and j indeces of an element in dense format,
	//returns the corresponding element from the diagonal matrix
	precision ithJthDenseElem(int i, int j) const;
public:
	// reads n0, m0, matrix_type
	LinearSystem(std::string input_file_name);

	void solveJacobi();

	void solveGaussSeidel();

	void solveBlockRelaxation(int block_size);

	void print() const;

	void printAnotha() const;

	void setWeights(precision w_j, precision w_g_s, precision w_b_r)
	{
		omega_jacobi = w_j;
		omega_gauss_seidel = w_g_s;
		omega_block_relaxation = w_b_r;
	}
};
