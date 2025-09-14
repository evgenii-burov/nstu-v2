#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class DURAQ
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

	virtual double get_next_y(double t, double y);

public:
	DURAQ(std::string file_name)
	{
		std::ifstream input_stream(file_name);
		input_stream >> h, t_start, t_end, y_0;
	}

	void solve_du(std::string file_name)
	{
		std::ofstream output_stream(file_name);
		for (double t = t_start, y = y_0; t<t_end && abs(t - t_end)>eps;t += h, y=get_next_y(t,y))
		{
			output_stream << t << "\t" << y << "\t" << y_analytic(t) << "\t" << abs(y - y_analytic(t)) << "\n";
		}
	}
};

class EulerFirst : public DURAQ
{
protected:
	double get_next_y(double t, double y) override
	{
		return y + h * DURAQ::y_derivative(t, y);
	}
public:
	using DURAQ::DURAQ;
};