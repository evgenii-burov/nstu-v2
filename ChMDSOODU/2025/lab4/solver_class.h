#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <stack>

class DE_Solver
{
protected:
	double t0, t1, y0, h;

	double f(double t, double y)
	{
		return 2 * t * y;
	}
	double y_analytic(double t)
	{
		return exp(t * t);
	}
public:
	DE_Solver(std::string file_name)
	{
		std::ifstream input_stream(file_name);
		double _t0, _t1, _y0, _h;
		input_stream >> _t0 >> _t1 >> _y0 >> _h;
		t0 = _t0, t1 = _t1, y0 = _y0, h = _h;
		input_stream.close();
	}

	virtual void solve(std::string file_name) = 0;
};

class DE_SolverAdamsMethod : public DE_Solver
{
protected:
	int initial_values_count;
	double runge_kutta4(double t, double y)
	{
		double k, k1, k2, k3, k4;
		k1 = f(t, y);
		k2 = f(t + h / 2, y + h * k1 / 2);
		k3 = f(t + h / 2, y + h * k2 / 2);
		k4 = f(t + h, y + h * k3);
		k = (k1 + 2 * k2 + 2 * k3 + k4) / 6;
		return y + h * k;
	}

	virtual double adams(std::vector<double>& initial_values, double t, double y) = 0;
public:
	void solve(std::string file_name) override
	{
		std::ofstream output_stream(file_name);
		output_stream << std::scientific << std::setprecision(15);
		output_stream << "t\t\ty_num\t\ty_an\t\t|y_num-y_an|\n";

		double t = t0, y = y0;
		std::vector<double> initial_values = {y0};

		for (int i = 0; (t < t1 || abs(t - t1) < 1e-7) && i < initial_values_count; t += h, i++)
		{
			output_stream << t << "\t\t";
			output_stream << y << "\t\t";
			output_stream << y_analytic(t) << "\t\t";
			output_stream << abs(y_analytic(t) - y) << "\n";

			double next_y = runge_kutta4(t, y);
			y = next_y;
			initial_values.push_back(next_y);
		}
		//initial_values.erase(initial_values.end() - 1);
		for (; t < t1 || abs(t - t1) < 1e-10; t += h)
		{
			output_stream << t << "\t\t";
			output_stream << y << "\t\t";
			output_stream << y_analytic(t) << "\t\t";
			output_stream << abs(y_analytic(t) - y) << "\n";

			double next_y = adams(initial_values, t, y);
			y = next_y;
			initial_values.push_back(next_y);
			initial_values.erase(initial_values.begin());

		}
		output_stream.close();
	}

	DE_SolverAdamsMethod(std::string file_name) : DE_Solver(file_name)
	{
		initial_values_count = 0;
	}
};

class DE_SolverAdamsExplicit3 : public DE_SolverAdamsMethod
{
private:
	double adams(std::vector<double>& initial_values, double t, double y) override
	{
		double linear_combination = 0;
		linear_combination += 5 * f(t-2*h, initial_values[0]) - 16 * f(t-h, initial_values[1]) + 23 * f(t, y);
		return y + h * linear_combination / 12;
	}
public:
	DE_SolverAdamsExplicit3(std::string file_name) : DE_SolverAdamsMethod(file_name)
	{
		initial_values_count = 2;
	}
};

class DE_SolverAdamsExplicit4 : public DE_SolverAdamsMethod
{
private:
	double adams(std::vector<double>& initial_values, double t, double y) override
	{
		double linear_combination = 0;
		linear_combination += -9 * f(t - 3 * h, initial_values[0])
			+ 37 * f(t - 2 * h, initial_values[1]) - 59 * f(t - h, initial_values[2]) + 55 * f(t, y);
		return y + h * linear_combination / 24;
	}
public:
	DE_SolverAdamsExplicit4(std::string file_name) : DE_SolverAdamsMethod(file_name)
	{
		initial_values_count = 3;
	}
};