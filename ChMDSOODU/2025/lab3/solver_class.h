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
	{
	}

	void solve(std::string file_name)
	{
		std::vector<double> h_list(method.get_h_list());
		std::ofstream output_stream(file_name);
		output_stream << std::scientific << std::setprecision(5) << "Method used: " << method.method_name();
		std::vector<double> parameters = equation.get_parameters();
		for (double h : h_list)
		{
			double t_start = parameters[0], t_end = parameters[1], y0 = parameters[2];

			output_stream << "\nt\ty_num\ty_a\t|y_num-y_a|\n";
			for (double t = t_start, y = y0; t < t_end || abs(t - t_end) < eps; y = method.get_next_y(t, y, h, equation), t += h)
			{
				double y_a = equation.analytical_solution(t);
				output_stream << t << "\t" << y << "\t" << y_a << "\t" << abs(y - y_a) << "\n";
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
	{
	}

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

class Equation2
{
private:
	double t_start, t_end, y0;
public:
	Equation2(double t0, double t1, double y_0)
		: t_start(t0), t_end(t1), y0(y_0)
	{
	}

	std::vector<double> get_parameters()
	{
		return { t_start, t_end, y0 };
	}

	double f_of_ty(double t, double y)
	{
		return -25 * y + cos(t) + 25 * sin(t);
	}

	double analytical_solution(double t)
	{
		return exp(-25 * t) + sin(t);
	}

	double f_of_x_s(double x, double t, double y, double h)
	{
		return y - x * (25 * h + 1) + h * (cos(t + h) + sin(t + h));
	}

	double df_over_dx_s(double x, double t, double y, double h)
	{
		return -1*(25 * h + 1);
	}

	double f_of_x_t(double x, double t, double y, double h)
	{
		//x=y+(h/2)(f(t,y)+f(t+h,x))
		//	x=y+(h/2)(-25y+cost+25sint-25x+cos(t+h)+25sin(t+h))
		//	x=y-25hx/2+(h/2)(-25y + cost + 25sint + cos(t + h) + 25sin(t + h))
		//	F(x)=y-x(1+25h/2) + (h / 2)(-25y + cost + 25sint + cos(t + h) + 25sin(t + h))
		return y - x * (1 + 25 * h / 2) + h * (-25 * y + cos(t) + 25 * sin(t) + cos(t + h) + 25 * sin(t + h)) / 2;
	}

	double df_over_dx_t(double x, double t, double y, double h)
	{
		return -1*(1 + 25 * h / 2);
	}
};


class Method
{
protected:
	std::vector<double> h_list;
	double eps = 1e-7;
public:
	Method(std::vector<double>& h)
		: h_list(h)
	{
	}

	std::vector<double> get_h_list()
	{
		return h_list;
	}

	virtual std::string method_name() const
	{
		return "DefaultMethod";
	}

	template<class Equation>
	double get_next_y(double t, double y, double h, Equation& eq)
	{
		return y + h * eq.f_of_ty(t, y);
	}
};

class EulerSimple : public Method
{
public:
	using Method::Method;

	std::string method_name() const override
	{
		return "EulerSimple";
	}
};

class EulerImplicit : public Method
{
public:
	using Method::Method;

	std::string method_name() const override
	{
		return "EulerSimpleImplicit";
	}

	template<class Equation>
	double get_next_y(double t, double y, double h, Equation& eq)
	{
		double x = 1;
		double f_x = 1;
		double df_dx = 1;
		do
		{
			df_dx = eq.df_over_dx_s(x, t, y, h);
			f_x = eq.f_of_x_s(x, t, y, h);
			x -= f_x / df_dx;
		} while (abs(f_x / df_dx) > eps);
		return x;
	}
};

class EulerTrapezoid : public Method
{
public:
	using Method::Method;

	std::string method_name() const override
	{
		return "EulerTrapezoid";
	}

	template<class Equation>
	double get_next_y(double t, double y, double h, Equation& eq)
	{
		double x = 1;
		double f_x = 1;
		double df_dx = 1;
		do
		{
			df_dx = eq.df_over_dx_t(x, t, y, h);
			f_x = eq.f_of_x_t(x, t, y, h);
			x -= f_x / df_dx;
		} while (abs(f_x / df_dx) > eps);
		return x;
	}
};

class RungeKutta4 : public Method
{
public:
	using Method::Method;

	std::string method_name() const override
	{
		return "RungeKutta4";
	}

	template<class Equation>
	double get_next_y(double t, double y, double h, Equation& eq)
	{
		double k, k1, k2, k3, k4;
		k1 = eq.f_of_ty(t, y);
		k2 = eq.f_of_ty(t + h / 2, y + h * k1 / 2);
		k3 = eq.f_of_ty(t + h / 2, y + h * k2 / 2);
		k4 = eq.f_of_ty(t + h, y + h * k3);
		k = (k1 + 2 * k2 + 2 * k3 + k4) / 6;
		return y + h * k;
	}
};

class EulerModified : public Method
{
public:
	using Method::Method;

	std::string method_name() const override
	{
		return "EulerModified";
	}

	template<class Equation>
	double get_next_y(double t, double y, double h, Equation& eq)
	{
		return y + h * (eq.f_of_ty(t, y) + eq.f_of_ty(t + h, y + h * eq.f_of_ty(t, y))) / 2;
	}
};