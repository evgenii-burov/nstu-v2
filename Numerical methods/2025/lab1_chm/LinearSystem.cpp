#include "LinearSystem.h"

LinearSystem::LinearSystem(std::string file_name) :
	n(-1), ia({}), di({}), al({}), au({}), b({})
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
	for (int i = 0; i < n; i++)
	{
		input_stream >> element_float;
		b.push_back(element_float);
	}
}

void LinearSystem::print_ls()
{
	std::cout << "n: " << n << "\nia: ";
	for (int i = 0; i < n + 1; i++)
	{
		std::cout << ia[i] << " ";
	}
	std::cout << "\ndi: ";
	for (int i = 0; i < n; i++)
	{
		std::cout << di[i] << " ";
	}
	std::cout << "\nal: ";
	for (int i = 0; i < ia[n]; i++)
	{
		std::cout << al[i] << " ";
	}
	std::cout << "\nau: ";
	for (int i = 0; i < ia[n]; i++)
	{
		std::cout << au[i] << " ";
	}
	std::cout << "\nb: ";
	for (int i = 0; i < n; i++)
	{
		std::cout << b[i] << " ";
	}
	std::cout << "\n";
}

void LinearSystem::solve_Ux_y()
{
	int elements_in_line = 0;
	for (int i = n - 1; i >= 0; i--)
	{
		precision sum_over_j = 0;
		elements_in_line = ia[i + 1] - ia[i];
	}

	//upper triangle
	for (int i = 1; i < n; i++)
	{
		elements_in_line = ia[i + 1] - ia[i];
		for (int j = 0; j < elements_in_line; j++)
		{
			std::cout << i << " " << au[ia[i] + j] << " " << b[i] << "\n";
			result[i - elements_in_line + j] += au[ia[i] + j] * b[i];
		}
	}

}

void LinearSystem::solve_Ly_b()
{
	int elements_in_line = 0;
	for (int i = 1; i < n; i++)
	{
		precision sum_over_j = 0;
		elements_in_line = ia[i + 1] - ia[i];
		for (int j = 0; j < elements_in_line; j++)
		{
			sum_over_j += al[ia[i] + j] * b[i - elements_in_line + j];
		}
		b[i] -= sum_over_j;
	}
}

void LinearSystem::matrix_times_b()
{
	std::vector<precision> result;
	//diagonal
	result = di;
	for (int i = 0; i < n; i++)
	{
		result[i] *= b[i];
	}
	//lower triangle
	int elements_in_line = 0;
	for (int i = 1; i < n; i++)
	{
		elements_in_line = ia[i + 1] - ia[i];
		for (int j = 0; j < elements_in_line; j++)
		{
			result[i] += al[ia[i] + j] * b[i - elements_in_line + j];
		}
	}
	//upper triangle
	for (int i = 1; i < n; i++)
	{
		elements_in_line = ia[i + 1] - ia[i];
		for (int j = 0; j < elements_in_line; j++)
		{
			std::cout << i << " " << au[ia[i] + j] << " " << b[i] << "\n";
			result[i - elements_in_line + j] += au[ia[i] + j] * b[i];
		}
	}
	std::cout << "u\n";
	for (int i = 0; i < n; i++)
	{
		std::cout << result[i] << " ";
	}
	std::cout << "\nu";
}