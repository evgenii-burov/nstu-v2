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