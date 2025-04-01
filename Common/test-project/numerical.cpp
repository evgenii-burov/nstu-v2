#include "numerical.h"

void SLAU::print_vector(std::vector<real>& vector)
{
	for (const auto elem : vector)
	{
		std::cout << elem << "\t";
	}
	std::cout << "\n";
}