#include "function_minimization.h"

double FunctionMinimization::dichotomy_method(double eps)
{
	double a{ a0 }, b{ b0 };
	double x1{ 0 }, x2{ 0 };

	std::cout << "a\t\tx1\t\tx2\t\tb\n";

	while (b - a >= eps)
	{
		x1 = (a + b - eps / 2) / 2;
		x2 = (a + b + eps / 2) / 2;

		std::cout << a << "\t\t" << x1 << "\t\t" << x2 << "\t\t" << b << "\n";

		if (f(x1) >= f(x2))
		{
			a = x1;
		}
		else
		{
			b = x2;
		}
	}

	return (b - a) / 2;
}

double FunctionMinimization::golden_ratio_method(double eps)
{
	double a{ a0 }, b{ b0 };
	double x1{ 0 }, x2{ 0 };

	std::cout << "a\t\tx1\t\tx2\t\tb\n";

	x1 = a + (3 - sqrt(5)) * (b - a) / 2;
	x2 = a + (sqrt(5) - 1) * (b - a) / 2;
	
	double f_x1 = f(x1);
	double f_x2 = f(x2);

	while (b - a >= eps)
	{

		std::cout << a << "\t\t" << x1 << "\t\t" << x2 << "\t\t" << b << "\n";

		if (f_x1 >= f_x2)
		{
			a = x1;
			x1 = x2;
			x2 = a + (sqrt(5) - 1) * (b - a) / 2;
			f_x1 = f_x2;
			f_x2 = f(x2);
		}
		else
		{
			b = x2;
			x2 = x1;
			x1 = a + (3 - sqrt(5)) * (b - a) / 2;
			f_x2 = f_x1;
			f_x1 = f(x1);
		}
	}
	return (b - a) / 2;
}

void FunctionMinimization::find_interval_containing_minimum()
{
	bool failed = false;
	for (double x = -1000; x < 1000 && !failed; x++)
	{
		double delta = 1;
		double h;
		if (f(x) > f(x + delta))
		{
			x += delta;
			h = delta;
		}
		if (f(x) < f(x + delta))
		{
			x -= delta;
			h = -delta;
		}
		if (f(x) == f(x + delta))
		{
			a0 = x;
			b0 = x + delta;
		}
		while (f(x) > f(x + h))
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
	}
	//std::cout << "a0: " << a0 << "\tb0: " << b0 << '\n';
}