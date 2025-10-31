#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <utility>
#include <random>

typedef float precision;
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
	LinearSystem(std::string file_name, int matrix_type = 1, int optional_k = 1);
	LinearSystem(int matrix_size, int matrix_type, int optional_k = 1);
	/*
	matrix type 1: diagonal significance matrix
	matrix type 2: dense matrix
	matrix type 3: Hilbert's matrix
	*/
	void decompose_ldu();
	void solve_Ly_b();
	void solve_DUx_y();
	void matrix_times_b();
	void print_ls();
	void solve_LDU()
	{
		decompose_ldu();
		solve_Ly_b();
		solve_DUx_y();
	}
	void solve_Gauss();
	void compare_x();
};

/*
di: n-1 + 3 * (1 + n-1)*(n-1)/2
lij / uij: n + (n-1) + 3 * (1 + n-1)*(n-1)/2
sum di+lij+uij: n-1+3n(n-1)/2+4n-2+3n(n-1)

ly = b: n*n-1
ux = d-1 y: n*n-1+n
*/