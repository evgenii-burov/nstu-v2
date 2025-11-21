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

class AdamsMethod : public DE_Solver
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
		output_stream << "t\ty_num\ty_an\t|y_num-y_an|\n";

		double t = t0, y = y0;
		std::vector<double> initial_values = { f(t, y0) };

		for (int i = 0; (t < t1 || abs(t - t1) < 1e-7) && i < initial_values_count; t += h, i++)
		{
			output_stream << std::setprecision(5);
			output_stream << t << "\t";
			output_stream << std::setprecision(15);
			output_stream << y << "\t";
			output_stream << y_analytic(t) << "\t";
			output_stream << std::setprecision(5);
			output_stream << abs(y_analytic(t) - y) << "\n";

			double next_y = runge_kutta4(t, y);
			y = next_y;
			initial_values.push_back(f(t+h, next_y));
		}
		//initial_values.erase(initial_values.end() - 1);
		for (; t < t1 || abs(t - t1) < 1e-10; t += h)
		{
			output_stream << std::setprecision(5);
			output_stream << t << "\t";
			output_stream << std::setprecision(15);
			output_stream << y << "\t";
			output_stream << y_analytic(t) << "\t";
			output_stream << std::setprecision(5);
			output_stream << abs(y_analytic(t) - y) << "\n";

			double next_y = adams(initial_values, t, y);
			initial_values.push_back(f(t + h, next_y));
			initial_values.erase(initial_values.begin());
			y = next_y;
		}
		output_stream.close();
	}

	AdamsMethod(std::string file_name) : DE_Solver(file_name)
	{
		initial_values_count = 0;
	}
};

class AdamsExplicit3 : public AdamsMethod
{
private:
	double adams(std::vector<double>& initial_values, double t, double y) override
	{
		double linear_combination = 0;
		linear_combination += 5 * initial_values[0] - 16 * initial_values[1] + 23 * initial_values[2];
		double next_y = y + h * linear_combination / 12;
		return next_y;
	}
public:
	AdamsExplicit3(std::string file_name) : AdamsMethod(file_name)
	{
		initial_values_count = 2;
	}
};

class AdamsExplicit4 : public AdamsMethod
{
private:
	double adams(std::vector<double>& initial_values, double t, double y) override
	{
		double linear_combination = 0;
		linear_combination += -9 * initial_values[0] + 37 * initial_values[1] - 59 * initial_values[2] + 55 * initial_values[3];
		double next_y = y + h * linear_combination / 24;
		return next_y;
	}
public:
	AdamsExplicit4(std::string file_name) : AdamsMethod(file_name)
	{
		initial_values_count = 3;
	}
};

class AdamsImplicitMethod : public AdamsMethod
{
protected:
	virtual double f_of_x(std::vector<double>& initial_values, double t, double y, double x) = 0;
	virtual double df_over_dx(double t) = 0;

	double adams(std::vector<double>& initial_values, double t, double y) override
	{
		double x = 1;
		double f_x = 1;
		double df_dx = 1;
		do
		{
			df_dx = df_over_dx(t);
			f_x = f_of_x(initial_values, t, y, x);
			x -= f_x / df_dx;
		} while (abs(f_x / df_dx) > 10e-15);
		return x;
	}
private:
	using AdamsMethod::AdamsMethod;
};

class AdamsImplicit3 : public AdamsImplicitMethod
{
/*
x = y + h( 10x(t+h) + 16ty - 2(t-h)(y_n-1) )/12
0 = x(h/12 * 10(t+h) - 1) + y + h( 16ty - 2(t-h)(y_n-1) )/12 = F(x)
dF(x)/dx = (h/12 * 10(t+h) - 1)

x = y + h(5f(t+h,x)+8initial_values[1]-initial_values[0])/12
0 = y + x(h(10(t+h))/12 - 1) + h(8f(t,y)-f(t-h,initial_values[0]))/12  | f(t,y)=2ty
*/
private:
	double f_of_x(std::vector<double>& initial_values, double t, double y, double x) override
	{
		return x * (h  * 5 * (t + h) / 6 - 1) + y + h * (8 * initial_values[1] - initial_values[0]) / 12;
	}
	double df_over_dx(double t) override
	{
		return h * 5 * (t + h) / 6 - 1;
	}
public:
	AdamsImplicit3(std::string file_name) : AdamsImplicitMethod(file_name)
	{
		initial_values_count = 1;
	}
};

class AdamsImplicit4 : public AdamsImplicitMethod
{
	/*
	x = y + h( 18x(t+h) + 38ty - 10(t-h)Y0 + 2(t-2h)Y1 )/24
	0 = y + x(9h(t+h)/12 - 1) + h( 38ty - 10(t-h)Y0 + 2(t-2h)Y1 )/24 = F(x)
	dF/dx = 9h(t+h)/12 - 1
	*/
private:
	double f_of_x(std::vector<double>& initial_values, double t, double y, double x) override
	{
		return y + x * (9 * h * (t + h) / 12 - 1) + h * (19 * initial_values[2] - 5 * initial_values[1] + initial_values[0]) / 24;
	}
	double df_over_dx(double t) override
	{
		return 9 * h * (t + h) / 12 - 1;
	}
public:
	AdamsImplicit4(std::string file_name) : AdamsImplicitMethod(file_name)
	{
		initial_values_count = 2;
	}
};

class AdamsPrognosisCorrection3 : public AdamsMethod
{
private:
	double adams(std::vector<double>& initial_values, double t, double y) override
	{
		double linear_combination = 0;
		linear_combination = 5 * initial_values[0] - 16 * initial_values[1] + 23 * initial_values[2];
		linear_combination = y + h * linear_combination / 12;
		linear_combination = 5 * f(t + h, linear_combination) + 8 * initial_values[2] - initial_values[1];
		double next_y = y + h * linear_combination / 12;
		return next_y;
	}
public:
	AdamsPrognosisCorrection3(std::string file_name) : AdamsMethod(file_name)
	{
		initial_values_count = 2;
	}
};

class AdamsPrognosisCorrection4 : public AdamsMethod
{
private:
	double adams(std::vector<double>& initial_values, double t, double y) override
	{
		double linear_combination = 0;
		linear_combination = -9 * initial_values[0]
			+ 37 * initial_values[1] - 59 * initial_values[2] + 55 * initial_values[3];
		linear_combination = y + h * linear_combination / 24;
		linear_combination = 9 * f(t+h,linear_combination) + 19 * initial_values[3] - 5 * initial_values[2] + initial_values[1];
		double next_y = y + h * linear_combination / 24;
		return next_y;
	}
public:
	AdamsPrognosisCorrection4(std::string file_name) : AdamsMethod(file_name)
	{
		initial_values_count = 3;
	}
};