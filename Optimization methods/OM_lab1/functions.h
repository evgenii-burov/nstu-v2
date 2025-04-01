#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

class FunctionMinimization
{
private:
	double a;
	double b;
	double eps;
	double function(double x);
	double nthFibonacciNumber(int n);
	double smallestFibonacciNumber();
public:
	FunctionMinimization(std::string file_name);

	double dichotomyMethod();
	double goldenRatioMethod();
	double fibonacciMethod();
	void intervalSearch(double x0, double delta);
};