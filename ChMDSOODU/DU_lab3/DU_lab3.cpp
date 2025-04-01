#include "solution.h"

int main()
{
	EquationTwoRungeKutta du = EquationTwoRungeKutta("parameters.txt");
	du.calculateYAnalytic();
	du.calculateYNumerical();
	du.writeToFile("table.txt");
}
