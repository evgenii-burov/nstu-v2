#include "functions.h"

double FunctionMinimization::function(double x)
{
	return pow(x - 7, 2);
}

FunctionMinimization::FunctionMinimization(std::string file_name)
{
	std::ifstream input_stream(file_name);
	if (!input_stream)
	{
		throw std::string("Couldn\'t open file: " + file_name);
	}
	double a0;
	double b0;
	double eps0;
	input_stream >> a0 >> b0 >> eps0;
	if (a0 > b0)
	{
		throw std::string("Incorrect parameters: a must be less than b");
	}
	if (eps0 <= 0)
	{
		throw std::string("Incorrect parameters: epsilon must be positive");
	}
	a = a0;
	b = b0;
	eps = eps0;
}

double FunctionMinimization::dichotomyMethod()
{
	double minimum = 0;
	double delta = eps / 2;
	double left_edge = a;
	double right_edge = b;
	double left_edge_prev=a;
	double right_edge_prev=b;
	double x1;
	double x2;
	double f1;
	double f2;
	int iterations=0;
	std::ofstream output_stream("dichotomy.txt");
	output_stream << std::setprecision(7);
	for(int i=1;right_edge - left_edge >= eps;i++)
	{
		x1 = (left_edge + right_edge - delta) / 2;
		x2 = (left_edge + right_edge + delta) / 2;
		f1 = function(x1);
		f2 = function(x2);
		if (f1 >= f2)
		{
			left_edge_prev = left_edge;
			left_edge = x1;
		}
		else
		{
			right_edge_prev = right_edge;
			right_edge = x2;
		}
		output_stream << i << "\t" << x1 << "\t" << x2 << "\t" << f1 << "\t" \
			<< f2 << "\t" << left_edge << "\t" << right_edge << "\t" \
			<< right_edge - left_edge << "\t" << (right_edge_prev - left_edge_prev) / (right_edge - left_edge) << "\n";
		iterations = i;
	}
	minimum = (right_edge + left_edge) / 2;
	output_stream.close();
	std::cout << "eps: " << eps << " iterations:" << iterations << "\n";
	return minimum;
}

double FunctionMinimization::goldenRatioMethod()
{
	double minimum = 0;
	double delta = eps / 2;
	double left_edge = a;
	double right_edge = b;
	double left_edge_prev = a;
	double right_edge_prev = b;
	double x1;
	double x2;
	double f1;
	double f2;
	const double sqrt5 = sqrt(5);
	int iterations = 0;
	x1 = left_edge + (right_edge - left_edge) * (3 - sqrt5) / 2;
	x2 = left_edge + (right_edge - left_edge) * (sqrt5 - 1) / 2;
	f1 = function(x1);
	f2 = function(x2);
	std::ofstream output_stream("golden_ratio.txt");
	output_stream << std::setprecision(7);
	for (int i=1;right_edge - left_edge >= eps;i++)
	{
		if (f1 >= f2)
		{
			left_edge = x1;
			x1 = x2;
			f1 = f2;
			x2 = left_edge + (right_edge - left_edge) * (sqrt5 - 1) / 2;
			f2 = function(x2);
		}
		else
		{
			right_edge = x2;
			x2 = x1;
			f2 = f1;
			x1 = left_edge + (right_edge - left_edge) * (3 - sqrt5) / 2;
			f1 = function(x1);
		}
		output_stream << i << "\t" << x1 << "\t" << x2 << "\t" << f1 << "\t" \
			<< f2 << "\t" << left_edge << "\t" << right_edge << "\t" \
			<< right_edge - left_edge << "\t" << (right_edge_prev - left_edge_prev) / (right_edge - left_edge) << "\n";
		iterations = i;
	}
	minimum = (right_edge + left_edge) / 2;
	output_stream.close();
	std::cout << "eps: " << eps << " iterations:" << iterations << "\n";
	return minimum;
}

double FunctionMinimization::nthFibonacciNumber(int n)
{
	double fibonacci_number = 0;
	const double sqrt5 = sqrt(5);
	fibonacci_number = (pow((1+sqrt5)/2, n) - pow((1-sqrt5)/2, n)) / sqrt5;
	return fibonacci_number;
}

double FunctionMinimization::smallestFibonacciNumber()
{
	const double sqrt5 = sqrt(5);
	double result = 1;
	double number_to_compare = (b-a) / eps;
	for (int n = 2;result <= number_to_compare;n++)
	{
		result = (pow((1 + sqrt5) / 2, n) - pow((1 - sqrt5) / 2, n)) / sqrt5;
	}
	return result;
}

double FunctionMinimization::fibonacciMethod()
{
	double minimum = 0;
	double delta = eps / 2;
	double left_edge = a;
	double right_edge = b;
	double left_edge_prev = a;
	double right_edge_prev = b;
	double x1;
	double x2;
	double f1;
	double f2;

	const double sqrt5 = sqrt(5);
	double fib_n_plus_2 = 0;
	double number_to_compare = (b - a) / eps;
	int n = 0;
	int iterations = 0;
	do
	{
		n++;
		fib_n_plus_2 = (pow((1 + sqrt5) / 2, n) - pow((1 - sqrt5) / 2, n)) / sqrt5;
	} while (fib_n_plus_2 <= number_to_compare);
	n -= 2;

	x1 = a + (b - a) * (nthFibonacciNumber(n)/ fib_n_plus_2);
	x2 = a+b-x1;
	f1 = function(x1);
	f2 = function(x2);

	std::ofstream output_stream("fibonacci.txt");
	output_stream << std::setprecision(7);
	for (int k = 2,i=1;k<n;k++,i++)
	{
		if (f1 >= f2)
		{
			left_edge = x1;
			x1 = x2;
			f1 = f2;
			x2 = left_edge + (b - a) * (nthFibonacciNumber(n-k+2) / fib_n_plus_2);
			f2 = function(x2);
		}
		else
		{
			right_edge = x2;
			x2 = x1;
			f2 = f1;
			x1 = left_edge + (b - a) * (nthFibonacciNumber(n-k+1) / fib_n_plus_2);
			f1 = function(x1);
		}
		output_stream << i << "\t" << x1 << "\t" << x2 << "\t" << f1 << "\t" \
			<< f2 << "\t" << left_edge << "\t" << right_edge << "\t" \
			<< right_edge - left_edge << "\t" << (right_edge_prev - left_edge_prev) / (right_edge - left_edge) << "\n";
		iterations = i;
	}
	minimum = (right_edge + left_edge) / 2;
	output_stream.close();
	std::cout << "eps: " << eps << " iterations:" << iterations << "\n";
	return minimum;
}

void FunctionMinimization::intervalSearch(double x0, double delta)
{
	std::ofstream output_stream("interval.txt");
	output_stream << std::setprecision(7);

	//if (function(x0 + delta) >= function(x0 - delta))
	//{
	//	do
	//	{
	//		output_stream << x0 << "\t" << function(x0) <<"\n";
	//		x0 -= delta;
	//	} while (function(x0 + delta) >= function(x0 - delta));
	//	std::cout << "a:" << x0 << " b:" << x0 + delta  << "\n";
	//}
	//else
	//{
	//	do
	//	{
	//		output_stream << x0 << "\t" << function(x0) << "\n";
	//		x0 += delta;
	//	} while (function(x0 + delta) < function(x0 - delta));
	//	std::cout << "a:" << x0 - delta << " b:" << x0  << "\n"; 
	//}
	int k=1;
	double h=0;
	output_stream << x0 << "\t" << function(x0) << "\n";
	if (function(x0) > function(x0 + delta))
	{
		x0 += delta;
		h = delta;
		output_stream << x0 << "\t" << function(x0) << "\n";
	}
	else
	{
		x0 -= delta;
		h = -delta;
		output_stream << x0 << "\t" << function(x0) << "\n";
	}
	h *= 2;
	x0 += h;
	output_stream << x0 << "\t" << function(x0) << "\n";
	while (function(x0 - h) > function(x0))
	{
		h *= 2;
		x0 += h;
		k += 1;
		output_stream << x0 << "\t" << function(x0) <<"\n";
	}

	if (h > 0)
	{
		output_stream << "a:" << x0-h << " b:" << x0 + h << "\n";
	}
	else
	{
		output_stream << "a:" << x0+h << " b:" << x0 - h << "\n";
	}
	
	output_stream.close();
}