#include "solver_class.h"

int main()
{
	DE_SolverAdamsExplicit4 solver("input.txt");
	solver.solve("output.txt");
}
