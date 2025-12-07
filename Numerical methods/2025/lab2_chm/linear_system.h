#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

typedef double precision;

class LinearSystem
/*
7-диагональна€ матрица c параметрами
m Ц количество нулевых диагоналей,
n Ц размерность матрицы.
–азмер блока в реализации блочной  релаксации Ќ≈фиксированный.
*/
{
private:
	int n;
	int num_diagonals = 3;
	int m;
	int max_iterations = 100000;
	precision eps;

	std::vector<int> diagonal_offset;
	std::vector<std::vector<precision>> al;
	std::vector<std::vector<precision>> au;
	std::vector<precision> di;
	std::vector<precision> b;
	std::vector<precision> x;
	std::vector<precision> new_x;

	precision get_next_x_ith(int i, precision omega);

public:
	precision get_relative_discrepancy();

	LinearSystem(std::string file_name);

	void matrix_times_b();

	void solve_jacobi(precision omega);

	void solve_gauss_seidel(precision omega);

	void reset_x();
};


