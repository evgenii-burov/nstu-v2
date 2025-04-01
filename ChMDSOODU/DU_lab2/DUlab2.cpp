#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

using precision = double;

precision find_x(const precision& t, const precision& h, const precision& y_n, const int& method, const precision& eps)
{
	precision x = 1;
	precision delta_x = 1;
	precision f_of_x;
	precision df_over_dx;
	if (method == 1)
	{
		while (abs(delta_x) > eps)
		{
			f_of_x = x * (2 * h * t + 2 * pow(h, 2) - 1) + y_n;
			df_over_dx = (2 * h * t + 2 * pow(h, 2) - 1);
			delta_x = (f_of_x / df_over_dx);
			x = x - delta_x;
		}
	}
	if (method == 2)
	{
		while (abs(delta_x) > eps)
		{
			f_of_x = y_n + h * t * y_n + h * x * (t + h - (1 / h));
			df_over_dx = h * (t + h - (1 / h));
			delta_x = (f_of_x / df_over_dx);
			x = x - delta_x;
		}
	}
	return x;
}

void readParameters(precision& t_start, precision& t_target, precision& h, int& method)
{
	std::ifstream input;
	input.open("parameters.txt");
	if (!input.is_open())
	{
		std::cout << "parameters.txt file coudln\'t be opened, exiting..";
		exit(0);
	}
	input >> t_start >> t_target >> h >> method;
	if (t_start < 0)
	{
		std::cout << "Invalid parameter: t_start must be non negative, exiting..";
		exit(0);
	}
	if (h <= 0)
	{
		std::cout << "Invalid parameter: h must be positive, exiting..";
		exit(0);
	}
	if (t_start > t_target)
	{
		std::cout << "Invalid parameter: t_target must be greater than or equal to t_start, exiting..";
		exit(0);
	}
}

int main()
{
	precision var = 1;
	precision eps;
	if (sizeof(var) == sizeof(float))
	{
		eps = 1e-6;
	}
	else
	{
		eps = 1e-14;
	}

	precision t_start = -1;
	precision t_target = -1;
	precision h = -1;
	int method = -1;
	readParameters(t_start, t_target, h, method);
	precision t = 0;
	precision y_n = 1;
	precision y_t;

	std::ofstream output_file;
	output_file.open("values.txt");
	std::cout << std::setprecision(3) << std::scientific;
	output_file << std::setprecision(3) << std::scientific;
	std::cout << "t\t\ty_n\t\ty_t\t\t|y_n-y_t|\n";
	output_file << "t\t\ty_n\t\ty_t\t\t|y_n-y_t|\n";
	bool flag_last = false;
	while (true)
	{
		if (t > 1 || abs(t - 1) <= eps)
		{
			t = 1;
			flag_last = true;
		}
		y_t = exp(pow(t, 2));
		output_file << t << '\t' << y_n << '\t' << y_t << '\t' << abs(y_n - y_t) << '\n';
		std::cout << t << '\t' << y_n << '\t' << y_t << '\t' << abs(y_n - y_t) << '\n';
		if (flag_last)
			break;
		y_n = find_x(t, h, y_n, method, eps);
		t += h;
	}
	output_file.close();
}
