#include "function_minimization.h"

double FunctionMinimization::dichotomy_method(double eps)
{
	double a{ a0 }, b{ b0 };
	double x1{ 0 }, x2{ 0 };
	int i = 0;
	double a_prev{ 1 }, b_prev{ 2 };
	int times_function_computed = 0;

	std::ofstream output_stream("dichotomy_method.txt");
	output_stream << std::scientific << std::setprecision(8);
	output_stream << "i\tx1\t\tx2\t\tfx1\t\tfx2\t\tai\t\tbi\t\tli\t\tli-1 / li\n";

	while (b - a >= eps)
	{
		x1 = (a + b - eps / 2) / 2;
		x2 = (a + b + eps / 2) / 2;

		double f_x1 = func(x1);
		double f_x2 = func(x2);
		times_function_computed += 2;

		a_prev = a;
		b_prev = b;

		if (f_x1 >= f_x2)
		{
			a = x1;
		}
		else
		{
			b = x2;
		}
		
		i++;
		output_stream << i << "\t" << x1 << "\t" << x2 << "\t" << f_x1 << "\t" << f_x2 << "\t";
		output_stream << a << "\t" << b << "\t" << b - a << "\t" << (b_prev - a_prev) / (b - a) << "\n";
	}
	output_stream << "Middle of [a;b]: " << (b + a) / 2 << '\n';
	output_stream << "Times function computed: " << times_function_computed << '\n';
	output_stream.close();

	return (b + a) / 2;
}

double FunctionMinimization::golden_ratio_method(double eps)
{
	double a{ a0 }, b{ b0 };
	double x1{ 0 }, x2{ 0 };
	double a_prev{ 1 }, b_prev{ 2 };
	int i = 0;
	int times_function_computed = 0;

	std::ofstream output_stream("golden_ratio_method.txt");
	output_stream << std::scientific << std::setprecision(8);
	output_stream << "i\tx1\t\tx2\t\tfx1\t\tfx2\t\tai\t\tbi\t\tli\t\tli-1 / li\n";

	x1 = a + (3 - sqrt(5)) * (b - a) / 2;
	x2 = a + (sqrt(5) - 1) * (b - a) / 2;
	
	double f_x1 = func(x1);
	double f_x2 = func(x2);
	times_function_computed += 2;

	output_stream << i << "\t" << x1 << "\t" << x2 << "\t" << f_x1 << "\t" << f_x2 << "\t";
	output_stream << a << "\t" << b << "\t" << b - a << "\t" << (b_prev - a_prev) / (b - a) << "\n";
	
	i++;

	while (b - a >= eps)
	{
		a_prev = a;
		b_prev = b;
		if (f_x1 >= f_x2)
		{
			a = x1;
			x1 = x2;
			x2 = a + (sqrt(5) - 1) * (b - a) / 2;
			f_x1 = f_x2;
			f_x2 = func(x2);
			times_function_computed ++;
		}
		else
		{
			b = x2;
			x2 = x1;
			x1 = a + (3 - sqrt(5)) * (b - a) / 2;
			f_x2 = f_x1;
			f_x1 = func(x1);
			times_function_computed++;
		}

		output_stream << i << "\t" << x1 << "\t" << x2 << "\t" << f_x1 << "\t" << f_x2 << "\t";
		output_stream << a << "\t" << b << "\t" << b - a << "\t" << (b_prev - a_prev) / (b - a) << "\n";
		
		i++;
	}
	output_stream << "Middle of [a;b]: " << (b + a) / 2 << '\n';
	output_stream << "Times function computed: " << times_function_computed << '\n';
	output_stream.close();

	return (b + a) / 2;
}

//double FunctionMinimization::fibonacci_method(double eps)
//{
//	double fib_condition = (b0 - a0) / eps;
//	std::cout << fib_condition;
//	int n = 1;
//	for (int i = 1, double fib_n_plus_2 = fib(i); fib_n_plus_2 <= fib_condition; i++, fib_n_plus_2 = fib(i))
//	{
//		n = i;
//	}
//	n--;
//	std::cout << "\n" << fib(n+2) << "\n" << fib(n+1);
//	return 0;
//}

void FunctionMinimization::find_interval_containing_minimum(double x0)
{
	bool failed = false;
	double x = x0;
	double delta = 1;
	double h;
	if (func(x) > func(x + delta))
	{
		x += delta;
		h = delta;
	}
	if (func(x) < func(x + delta))
	{
		x -= delta;
		h = -delta;
	}
	if (func(x) == func(x + delta))
	{
		a0 = x;
		b0 = x + delta;
	}
	while (func(x) > func(x + h))
	{
		//std::cout << "x: " << x << "\txh: " << x + h << '\n';
		x += h;
		h *= 2;
	}
	if (h > 0)
	{
		a0 = x - h;
		b0 = x + h;
	}
	else
	{
		b0 = x - h;
		a0 = x + h;
	}
	if (a0 > 4 || b0 < 4)
	{
		std::cout << "Failed a0 " << a0 << " b0 " << b0 << " x " << x;
		failed = true;
	}
	//std::cout << "a0: " << a0 << "\tb0: " << b0 << '\n';
}