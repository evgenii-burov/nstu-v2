#include "function-minimization.h"

real FunctionMinimization::function_xy(real x, real y)
{
	int A1 = 2;
	int A2 = 3;
	int a1 = 1;
	int a2 = 1;
	int b1 = 2;
	int b2 = 3;
	int c1 = 1;
	int c2 = 3;
	int d1 = 2;
	int d2 = 3;
	real F = 0;
	real i = (-pow(((x - a1) / b1), 2) - pow(((y - c1) / d1), 2));
	real j = (-pow(((x - a2) / b2), 2) - pow(((y - c2) / d2), 2));
	F = A1 * exp(i) + A2 * exp(j);
	return F;

	//return -x * x - y * y;
}

real FunctionMinimization::function(real x)
{
	return pow((x - 7), 2);
}

void FunctionMinimization::find_interval(real start, real step)
{
	if (function(start + step) >= function(start))
	{
		step = -step;
	}
	real previous=start;
	real next;
	bool function_flipped = false;
	while (!function_flipped)
	{
		if (function(start + step) > function(start))
		{
			start += step;
			break;
		}
		previous = start;
		start += step;
		step *= 2;
		std::cout << previous << " " << start << "\n";
	}
	std::cout << previous << " " << start << "\n";
}

std::pair<real, real> FunctionMinimization::goldenRatioMethod(std::pair<real, real> start, std::pair<real, real> direction, real distance)
{
	real normalising_coefficient = sqrt(pow(direction.first, 2) + pow(direction.second, 2));
	direction.first /= normalising_coefficient;
	direction.second /= normalising_coefficient;
	double delta = eps / 2;
	double left_edge = 0;
	double right_edge = distance;
	real x1;
	real x2;
	double f1;
	double f2;
	const double sqrt5 = sqrt(5);
	int iterations = 0;
	x1 = left_edge + (right_edge - left_edge) * (3 - sqrt5) / 2;
	x2 = left_edge + (right_edge - left_edge) * (sqrt5 - 1) / 2;
	f1 = function_xy(start.first + x1 * direction.first, start.second + x1 * direction.second);
	f2 = function_xy(start.first + x2 * direction.first, start.second + x2 * direction.second);
	for (int i = 1;right_edge - left_edge >= eps;i++)
	{
		if (f1 <= f2)
		{
			left_edge = x1;
			x1 = x2;
			f1 = f2;
			x2 = left_edge + (right_edge - left_edge) * (sqrt5 - 1) / 2;
			f2 = function_xy(start.first + x2 * direction.first, start.second + x2 * direction.second);
		}
		else
		{
			right_edge = x2;
			x2 = x1;
			f2 = f1;
			x1 = left_edge + (right_edge - left_edge) * (3 - sqrt5) / 2;
			f1 = function_xy(start.first + x1 * direction.first, start.second + x1 * direction.second);
		}
	}
	real distance_to_maximum = (right_edge + left_edge) / 2;
	std::pair<real, real> maximum = {start.first+distance_to_maximum*direction.first, start.second + distance_to_maximum * direction.second };
	return maximum;
}

real FunctionMinimization::minimize_function()
{
	std::pair<real, real> point = {10,10};
	for (int i = 0; i < 10; i++)
	{
		point = goldenRatioMethod(point, { 0,-1 }, 2);
		std::cout << point.first << " " << point.second << "\n";
		point = goldenRatioMethod(point, { -1,0 }, 2);
		std::cout << point.first << " " << point.second << "\n";
	}
	return function_xy(point.first, point.second);
}

real FunctionMinimization::rotating_coordinates_method(std::pair<real, real> start, real distance)
{
	real difference = eps * 10;
	std::pair<real, real> direction = { 1,0 };
	while(difference > eps)
	{
		start = goldenRatioMethod(start, direction, distance);
		if (direction.second == 0)
		{
			direction.first = 0;
			direction.second = 1;
		}
		else
		{
			direction.second = -(direction.first / direction.second);
			direction.first = 1;
		}
		std::cout << start.first << " " << start.second << "\n";
		auto previous_point = start;
		start = goldenRatioMethod(start, direction, distance);
		direction.first = start.first - previous_point.first;
		direction.second = start.second - previous_point.second;
		std::cout << start.first << " " << start.second << "\n";
	}
	return function_xy(start.first, start.second);
}