#pragma once
#include <iostream>
#include <vector>
#include <iomanip>

typedef double real;

class FunctionMinimization
{
private:
	real function_xy(real x, real y);
	real function(real x);
	std::pair<real, real> goldenRatioMethod(std::pair<real, real> start, std::pair<real, real> direction, real distance);
public:
	real eps = 1e-7;
	real x0 = 0;
	real y0 = 0;
	real minimize_function();
	real rotating_coordinates_method(std::pair<real, real> start, real distance);
	void find_interval(real start, real step);
};