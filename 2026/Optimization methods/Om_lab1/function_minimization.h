#pragma once
#include <iostream>
#include <cmath>

class FunctionMinimization
{
private:
	double a0{ -2 }, b0{ 20 };

	double f(double x)
	{
		return (x - 4) * (x - 4);
	}

public:
	double dichotomy_method(double eps);
	double golden_ratio_method(double eps);
	void find_interval_containing_minimum();
};
