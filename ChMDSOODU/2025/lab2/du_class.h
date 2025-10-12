#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

class DifferentialEquationImplicit
{
	/*
	y' = 2ty
	h: 0.1 0.05 0.0025
	t = [0;1]
	y(0) = 1
	*/
protected:
	double h = .1, t_start = 0, t_end = 1, y_0 = 0, eps = 1e-5;

	double y_derivative(double t, double y)
	{
		return 2 * t * y;
	}

	double y_analytic(double t)
	{
		return exp(t * t);
	}

	virtual double f_of_x(double x, double t, double y) = 0;

	virtual double df_over_dx(double x, double t, double y) = 0;

	virtual double get_next_y(double t, double y)
	{
		double x = 1;
		double f_x = 0;
		double df_dx = 0;
		do
		{
			df_dx = df_over_dx(x, t, y);
			f_x = f_of_x(x, t, y);
			x -= f_x / df_dx;
		} while (abs(f_x / df_dx) > eps);
		return x;
	}

public:
	DifferentialEquationImplicit(std::string file_name)
	{
		std::ifstream input_stream(file_name);
		input_stream >> h >> t_start >> t_end >> y_0;
	}

	DifferentialEquationImplicit(double h_0, double t_start_0, double t_end_0, double y_0_0) :
		h(h_0), t_start(t_start_0), t_end(t_end_0), y_0(y_0_0)
	{
	}

	void solve_du(std::string file_name)
	{
		std::ofstream output_stream(file_name);
		output_stream << "t\ty_n\ty_a\t|y_n-y_a|\n" << std::scientific << std::setprecision(7);
		std::cout << "t\ty_n\ty_a\t|y_n-y_a|\n" << std::scientific << std::setprecision(7);
		for (double t = t_start, y = y_0; t < t_end || abs(t - t_end) < eps; \
			y = get_next_y(t, y), t += h)
		{
			output_stream << t << "\t" << y << "\t" << y_analytic(t) << "\t" << abs(y - y_analytic(t)) << "\n";
			std::cout << t << "\t" << y << "\t" << y_analytic(t) << "\t" << abs(y - y_analytic(t)) << "\n";
		}
	}
};

class EulerSimpleImplicit : public DifferentialEquationImplicit
{
	/*
		f(t,y) = 2ty
		x = y + h * f(t+h,x) = y + h * (2x(t+h))
		x = y + 2xth + 2x(h^2)
		x(2th + 2(h^2) - 1) + y = 0

		F(x) = x(2th + 2(h^2) - 1) + y
		dF(x)/dx = 2th + 2(h^2) - 1
		*/
private:
	double f_of_x(double x, double t, double y) override
	{
		return x * (2 * t * h + 2 * h * h - 1) + y;
	}

	double df_over_dx(double x, double t, double y) override
	{
		return 2 * t * h + 2 * h * h - 1;
	}

public:
	using DifferentialEquationImplicit::DifferentialEquationImplicit;

};

class EulerSecondImplicit : public DifferentialEquationImplicit
{
	/*
		f(t,y) = 2ty
		x = y + h * ( f(t,y)+f(t+h,x) )/2 = y + h * ( 2ty+2x(t+h) )/2 = y+h*(ty+x(t+h))
		x=y + hty + hxt + x(h^2)
		x(ht+h^2-1) + y(ht+1) = 0

		F(x) = x(ht+h^2-1) + y(ht+1)
		dF(x)/dx = ht+h^2-1
		*/
private:
	double f_of_x(double x, double t, double y) override
	{
		return x * (h * t + h * h - 1) + y * (h * t + 1);
	}

	double df_over_dx(double x, double t, double y) override
	{
		return h * t + h * h - 1;
	}

public:
	using DifferentialEquationImplicit::DifferentialEquationImplicit;

};

class EulerThirdImplicit : public DifferentialEquationImplicit
{
	/*
		f(t,y) = 2ty
		y_n+1 = y + hf(t+h/2, y_n+1/2); y_n+1/2 = y + hf(t+h/2,y_n+1/2)/2
		x = y + hf(t+h/2,x)/2 = y + h(t+h/2)x
		x = y + h(t+h/2)x
		x(ht + (h^2)/2 - 1) + y = 0

		F(x) = x(ht + (h^2)/2 - 1) + y
		dF(x)/dx = ht + (h^2)/2 - 1
		*/
private:
	double f_of_x(double x, double t, double y) override
	{
		return x * (h * t + h * h / 2 - 1) + y;
	}

	double df_over_dx(double x, double t, double y) override
	{
		return h * t + h * h / 2 - 1;
	}

	double get_next_y(double t, double y) override
	{
		double x = 1;
		double f_x = 0;
		double df_dx = 0;
		int a = 1;
		do
		{
			df_dx = df_over_dx(x, t, y);
			f_x = f_of_x(x, t, y);
			x -= f_x / df_dx;
		} while (abs(f_x / df_dx) > eps);
		
		return y + h * y_derivative(t + h / 2, x);
	}
public:
	using DifferentialEquationImplicit::DifferentialEquationImplicit;

};