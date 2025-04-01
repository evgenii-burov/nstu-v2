#include "du.h"

DifferentialEquation::DifferentialEquation(std::string input_file_name) :
	y_analytic(),
	y_numerical()
{
	std::ifstream input_file;
	input_file.open(input_file_name);
	if (!input_file.is_open())
		throw "Unable to open input file";
	double t0, t1, y0, h0;
	input_file >> t0 >> t1 >> y0 >> h0;
	t_start = t0;
	t_target = t1 >= t0 ? t1 : throw("Incorrect parameter: t1 must be greater than or equal to t0");
	y_of_0 = y0;
	h = h0 > 0 ? h0 : throw("Incorrect parameter: t1 must be greater than 0");
	input_file.close();
}

void DifferentialEquation::calculateYAnalytic()
{
	double t = t_start;
	bool flag_last = false;
	while (true)
	{
		if (t > t_target || abs(t_target - t) < t_eps)
		{
			t = t_target;
			flag_last = true;
		}
		y_analytic.push_back(getAnalyticSolutionFunctionValue(t));
		t += h;
		if (flag_last)
			break;
	}
}

void DifferentialEquation::calculateYNumerical()
{
	double t = t_start, y = y_of_0;
	double y_next, k;
	bool flag_last = false;
	while (true)
	{
		if (t > t_target || abs(t_target - t) < t_eps)
		{
			t = t_target;
			flag_last = true;
		}
		y_numerical.push_back(y);
		y_next = getNextY(t, y);
		y = y_next;
		t += h;
		if (flag_last)
			break;
	}
}

void DifferentialEquation::printParameters() const
{
	std::cout << "t_start: " << t_start;
	std::cout << "\nt_target: " << t_target;
	std::cout << "\ny(0): " << y_of_0;
	std::cout << "\nh: " << h << '\n';
}

void DifferentialEquation::writeToFile(std::string output_file_name) const
{
	std::ofstream output_file;
	output_file.open(output_file_name);
	if (!output_file.is_open())
		throw "Unable to open output file";

	double t = t_start;
	bool flag_last = false;

	std::cout << std::setprecision(6) << std::scientific;
	output_file << std::setprecision(6) << std::scientific;
	std::cout << "t\t\ty_n\t\ty_t\t\t|y_n-y_t|\n";
	output_file << "t\t\ty_n\t\ty_t\t\t|y_n-y_t|\n";

	for (int i = 0; i < y_analytic.size(); i++)
	{
		if (t > t_target || abs(t_target - t) < t_eps)
		{
			t = t_target;
		}
		output_file << t << '\t' << y_numerical[i] << '\t' << y_analytic[i] << '\t' << abs(y_numerical[i] - y_analytic[i]) << '\n';
		std::cout << t << '\t' << y_numerical[i] << '\t' << y_analytic[i] << '\t' << abs(y_numerical[i] - y_analytic[i]) << '\n';
		t += h;
	}
	output_file.close();
}