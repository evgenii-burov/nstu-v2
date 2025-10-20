#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <utility>

typedef double precision;
typedef double precision_sum;

class LinearSystem
{
private:
	std::vector<precision> al, au, di, b;
	std::vector<int> ia, ja;
	double eps = 1e-7;
	int n;
	precision dense_element(int i, int j);

public:
	LinearSystem(std::string file_name);
	void decompose_ldu();
	void solve_Ly_b();
	void solve_DUx_y();
	void matrix_times_b();
	void print_ls();
	void solve_Gauss();
};