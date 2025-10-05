#include "LinearSystem.h"

LinearSystem::LinearSystem(std::string file_name) :
	n(-1), ia({}), di({}), al({}), au({})
{
	std::fstream input_stream(file_name);
	input_stream >> n;
	int element_int = 0;
	precision element_float = 0;
	for (int i = 0; i < n + 1; i++)
	{
		input_stream >> element_int;
		ia.push_back(element_int);
	}
	for (int i = 0; i < n; i++)
	{
		input_stream >> element_float;
		di.push_back(element_float);
	}
	for (int i = 0; i < element_int; i++)
	{
		input_stream >> element_float;
		al.push_back(element_float);
	}
	for (int i = 0; i < element_int; i++)
	{
		input_stream >> element_float;
		au.push_back(element_float);
	}
}