#include "solver_class.h"

int main()
{
	AdamsImplicit4 solver("input.txt");
	solver.solve("output.txt");
}
