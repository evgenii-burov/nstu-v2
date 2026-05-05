#include <iostream>
#include "functions.h"
#include "minimization_algorithms.h"

void adjust_penalty(double& penalty) {
	penalty = penalty * 2;
}

int main() {
	PenaltyA penalty;
	PenaltyFunction p_f(penalty);
	double eps = 1e-7;
	HookJeeves hj(p_f, eps, eps);
	Point start = { 10,10 };

	hj.func.penalty.penalty_coefficient = 1;

	Point minimum_point = hj.minimize(start, false);

	int iterations = 0;

	while (!hj.func.penalty.check(minimum_point) && iterations < 10) {
		adjust_penalty(hj.func.penalty.penalty_coefficient);
		minimum_point = hj.minimize(start, false);
		iterations++;
	}

	std::cout << "\n==================\n" << minimum_point;
}