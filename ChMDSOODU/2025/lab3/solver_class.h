#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

template<class Equation, class Method>
class Solver
{
private:
	Equation equation;
	Method method;
	double eps = 1e-7;
public:
	Solver(Equation& equation0, Method& method0)
		: equation(equation0), method(method0)
	{ }

	void solve(std::string file_name)
	{
		std::vector<double> h_list(method.get_h_list());
		std::ofstream output_stream(file_name);
		std::vector<double> parameters = equation.get_parameters();
		for (double h : h_list)
		{
			double t_start = parameters[0], t_end = parameters[1], y0 = parameters[2];

			output_stream << "\nt\t\ty_num\t\ty_a\t\t|y_num-y_a|\n";
			for (double t = t_start, y = y0; t < t_end || abs(t - t_end) < eps; t += h, y = method.get_next_y(t, y, h, equation))
			{
				double y_a = equation.analytical_solution(t);
				output_stream << t << "\t\t" << y << "\t\t" << y_a << "\t\t" << abs(y-y_a) << "\n";
			}
		}
		output_stream.close();
	}
};

class Equation1
{
private:
	double t_start, t_end, y0;
public:
	Equation1(double t0, double t1, double y_0)
		: t_start(t0), t_end(t1), y0(y_0)
	{ }

	std::vector<double> get_parameters()
	{
		return { t_start, t_end, y0 };
	}

	double f_of_ty(double t, double y)
	{
		return 2 * t * y;
	}

	double analytical_solution(double t)
	{
		return exp(t * t);
	}
};

class Method
{
private:
	std::vector<double> h_list;

public:
	Method(std::vector<double>& h)
		: h_list(h)
	{ }

	std::vector<double> get_h_list()
	{
		return h_list;
	}

	template<class Equation>
	double get_next_y(double t, double y, double h, Equation& eq)
	{
		return y + h * eq.f_of_ty(t, y);
	}
};
