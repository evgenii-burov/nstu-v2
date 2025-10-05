#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>

typedef float precision;

class LinearSystem
{
private:
	std::vector<precision> al, au, di, b;
	std::vector<int> ia;
	double eps = 1e-7;
	int n;
public:
	LinearSystem(std::string file_name);
	void decompose_ldu();
	void solve_Ly_b();
	void solve_Ux_y();
};