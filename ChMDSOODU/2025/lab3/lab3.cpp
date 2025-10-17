#include "solver_class.h"

int main()
{
	Equation1 eq(0, 1, 1);
	std::vector<double> h_list({ 0.1,0.05, 0.025 });
	Method m(h_list);
	Solver<Equation1, Method> s(eq, m);
	s.solve("output.txt");
}
