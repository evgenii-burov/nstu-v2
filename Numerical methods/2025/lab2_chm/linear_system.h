#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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
	int num_diagonals = 7;
	int m;
	int max_iterations = 100000;
	std::vector<int> diagonal_offset;
	std::vector<std::vector<precision>> matrix;
	std::vector<precision> b;
	std::vector<precision> old_x;
public:
	LinearSystem(std::string file_name);
	void matrix_times_b();
};


