#include "function-minimization.h"

real FunctionMinimization::function(real x)
{
	return pow((x - 7), 2);
}

void FunctionMinimization::find_interval(real start, real step)
{
	if (function(start + step) > function(start))
	{
		step = -step;
	}
	real two_steps_ago = start;
}