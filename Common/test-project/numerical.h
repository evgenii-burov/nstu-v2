#pragma once
#include <iostream>
#include <vector>
#include <iomanip>

using real = double;

class SLAU
{
private:
	int n;
	//half bandwidth
	int p;
	std::vector<std::vector<real>> L;
	std::vector<real> D;
	std::vector<std::vector<real>> U;

	std::vector<real> b;

	void print_vector(std::vector<real>& vector);
public:

};