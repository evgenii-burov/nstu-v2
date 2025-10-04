#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

class DifferentialEquation
{
	/*
	y' = 2ty
	h: 0.1 0.05 0.0025
	t = [0;1]
	y(0) = 1
	*/
protected:
	double h = .1, t_start = 0, t_end = 1, y_0 = 0, eps=1e-5;

	double y_derivative(double t, double y)
	{
		return 2 * t * y;
	}

	double y_analytic(double t)
	{
		return exp(t * t);
	}

	virtual double get_next_y(double t, double y) = 0;

public:
	DifferentialEquation(std::string file_name)
	{
		std::ifstream input_stream(file_name);
		input_stream >> h >> t_start >> t_end >> y_0;
	}

	DifferentialEquation(double h_0, double t_start_0, double t_end_0, double y_0_0) :
	h(h_0), t_start(t_start_0), t_end(t_end_0), y_0(y_0_0)
	{}

	void solve_du(std::string file_name)
	{
		std::ofstream output_stream(file_name);
		output_stream << std::setprecision(6) << std::scientific;
		std::cout << std::setprecision(6) << std::scientific;
		output_stream << "t\ty_n\ty_a\t|y_n-y_a|\n";
		std::cout << "t\ty_n\ty_a\t|y_n-y_a|\n";
		for (double t = t_start, y = y_0; t < t_end || abs(t - t_end) < eps; \
			y = get_next_y(t, y), t += h)
		{
			output_stream << t << "\t" << y << "\t" << y_analytic(t) << "\t" << abs(y - y_analytic(t)) << "\n";
			std::cout << t << "\t" << y << "\t" << y_analytic(t) << "\t" << abs(y - y_analytic(t)) << "\n";
		}
	}
};

class EulerSimple : public DifferentialEquation
{
protected:
	double get_next_y(double t, double y) override
	{
		return y + h * y_derivative(t, y);
	}
public:
	using DifferentialEquation::DifferentialEquation;
};

class EulerModified : public DifferentialEquation
{
protected:
	double get_next_y(double t, double y) override
	{
		return y + h * (y_derivative(t, y) + y_derivative(t + h, y + h * y_derivative(t, y))) / 2;
	}
public:
	using DifferentialEquation::DifferentialEquation;
};

class EulerImproved : public DifferentialEquation
{
protected:
	double get_next_y(double t, double y) override
	{
		return y + h * y_derivative(t + h / 2, y + h * y_derivative(t, y) / 2);
	}
public:
	using DifferentialEquation::DifferentialEquation;
};