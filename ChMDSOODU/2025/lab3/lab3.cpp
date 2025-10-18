#include "solver_class.h"

int main()
{
	Equation2 eq(0, 2, 1);
	std::vector<double> h_list({ 0.1, 0.05, 0.025 });
	EulerImplicit m(h_list);
	Solver<Equation2, EulerImplicit> s(eq, m);
	s.solve("output.txt");
}
