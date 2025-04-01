#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
/*
Base class:
has parameters, a constructor,
and method- and equation-independent functions*/
class DifferentialEquation
{
protected:
	double t_start, t_target;
	double y_of_0;
	double h;
	double t_eps = 1e-5;
	double eps = 1e-14;
	std::vector<double> y_analytic;
	std::vector<double> y_numerical;

	virtual double getNextY(double t0, double y0) const = 0;
	virtual double getYDerivative(double t0, double y0) const = 0;
	virtual double getAnalyticSolutionFunctionValue(double t0) const = 0;
public:
	DifferentialEquation(std::string input_file_name);

	void calculateYNumerical();

	void calculateYAnalytic();

	void printParameters() const;

	void writeToFile(std::string output_file_name) const;
};

class EquationOne : public DifferentialEquation
{
protected:
	double getYDerivative(double t0, double y0) const override
	{
		return 2 * t0 * y0;
	}
	double getAnalyticSolutionFunctionValue(double t0) const override
	{
		return exp(pow(t0, 2));
	}
public:
	using DifferentialEquation::DifferentialEquation;
};

class EquationOneRungeKutta : public EquationOne
{
	double getNextY(double t0, double y0) const override
	{
		double k_1, k_2, k_3, k_4, k;
		k_1 = getYDerivative(t0, y0);
		k_2 = getYDerivative(t0 + h / 2, y0 + h * k_1 / 2);
		k_3 = getYDerivative(t0 + h / 2, y0 + h * k_2 / 2);
		k_4 = getYDerivative(t0 + h, y0 + h * k_3);
		k = (k_1 + 2 * k_2 + 2 * k_3 + k_4) / 6;
		return y0 + h * k;
	}
public:
	using EquationOne::EquationOne;
};
//--------------------------------------------
class EquationTwo : public DifferentialEquation
{
protected:
	double getYDerivative(double t0, double y0) const override
	{
		return -25 * y0 + cos(t0) + 25 * sin(t0);
	}
	double getAnalyticSolutionFunctionValue(double t0) const override
	{
		return exp(-25 * t0) + sin(t0);
	}
public:
	using DifferentialEquation::DifferentialEquation;
};

class EquationTwoEulerExplicit : public EquationTwo
{
	double getNextY(double t0, double y0) const override
	{
		return y0 + h * getYDerivative(t0, y0);
	}
public:
	using EquationTwo::EquationTwo;
};

class EquationTwoEulerImplicit : public EquationTwo
{
	double getNextY(double t0, double y0) const override
	{
		double f_of_x, df_over_dx, delta_x;
		double x = 1;
		delta_x = DBL_MAX;
		while (delta_x > eps)
		{
			f_of_x = -x * (1 + 25 * h) + y0 + h * cos(t0 + h) + 25 * h * sin(t0 + h);
			df_over_dx = -1 * (1 + 25 * h);
			delta_x = f_of_x / df_over_dx;
			x -= delta_x;
		}
		return x;
	}
public:
	using EquationTwo::EquationTwo;
};

class EquationTwoEulerModified : public EquationTwo
{
	double getNextY(double t0, double y0) const override
	{
		return y0 + h * (getYDerivative(t0, y0) + getYDerivative(t0 + h, y0 + h * getYDerivative(t0, y0))) / 2;
	}
public:
	using EquationTwo::EquationTwo;
};

class EquationTwoTrapezoid : public EquationTwo
{
	double getNextY(double t0, double y0) const override
	{
		double f_of_x, df_over_dx, delta_x;
		double x = 1;
		delta_x = DBL_MAX;
		while (delta_x > eps)
		{
			f_of_x = -x * (1 + 25 * h / 2) + y0 + h * (-25 * y0 + cos(t0 + h) + 25 * sin(t0 + h) + cos(t0) + 25 * sin(t0)) / 2;
			df_over_dx = -1 * (1 + 25 * h / 2);
			delta_x = f_of_x / df_over_dx;
			x -= delta_x;
		}
		return x;
	}
public:
	using EquationTwo::EquationTwo;
};

class EquationTwoRungeKutta : public EquationTwo
{
	double getNextY(double t0, double y0) const override
	{
		double k_1, k_2, k_3, k_4, k;
		k_1 = getYDerivative(t0, y0);
		k_2 = getYDerivative(t0 + h / 2, y0 + h * k_1 / 2);
		k_3 = getYDerivative(t0 + h / 2, y0 + h * k_2 / 2);
		k_4 = getYDerivative(t0 + h, y0 + h * k_3);
		k = (k_1 + 2 * k_2 + 2 * k_3 + k_4) / 6;
		return y0 + h * k;
	}
public:
	using EquationTwo::EquationTwo;
};