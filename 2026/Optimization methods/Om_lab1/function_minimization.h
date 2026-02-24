#pragma once
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

class FunctionMinimization
{
private:
	double a0{ -2 }, b0{ 20 };

	double func(double x)
	{
		return (x - 4) * (x - 4);
	}

	double fib(int n)
	{
		return (pow((1 + sqrt(5)) / 2, n) - pow((1 - sqrt(5)) / 2, n)) / sqrt(5); 
	}

public:

	double dichotomy_method(double eps);
	double golden_ratio_method(double eps);
	double fibonacci_method(double eps);
	void find_interval_containing_minimum(double x0);
};
