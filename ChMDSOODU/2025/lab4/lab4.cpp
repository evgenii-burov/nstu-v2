#include "solver_class.h"

int main()
{
	AdamsPrognosisCorrection4 solver("input.txt");
	solver.solve("output.txt");
}
