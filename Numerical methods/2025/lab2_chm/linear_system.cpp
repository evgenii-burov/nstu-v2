#include "linear_system.h"

LinearSystem::LinearSystem(std::string file_name)
{
	std::ifstream input_file(file_name);
	input_file >> n >> m;
	matrix.resize(n);
	b.resize(n);
	for (int i = 0; i < n; i++)
	{
		matrix[i].resize(num_diagonals);
		for (int j = 0; j < num_diagonals; j++)
		{
			precision element;
			input_file >> element;
			matrix[i][j] = element;
		}
	}

	for (int i = 0; i < n; i++)
	{
		precision element;
		input_file >> element;
		b[i] = element;
	}

	diagonal_offset = { -m - 3,-m - 2,-1,0,1,m + 2,m + 3 };

	for (int i = 0; i < n; i++)
	{
		
		for (int j = 0; j < num_diagonals; j++)
		{
			
			std::cout << matrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void LinearSystem::matrix_times_b()
{
	std::vector<precision> result(10);
	std::cout << result.size();


	for (int j = 3; j < num_diagonals; j++)
	{
		result[i] += matrix[i][j] * b[diagonal_offset[j] + i];
	}

	for (int j = 3; j < num_diagonals - i; j++)
	{
		result[i] += matrix[i][j] * b[diagonal_offset[j] + i];
	}

	for (int i = m+1; i < n - m; i++)
	{
		for (int j = 3 - i + m; j < num_diagonals - i + m; j++)
		{
			result[i] += matrix[i][j] * b[diagonal_offset[j] + i];
		}
	}
	for (int i = n-m; i < n; i++)
	{
		for (int j = 3 - i; j < num_diagonals - i; j++)
		{
			result[i] += matrix[i][j] * b[diagonal_offset[j] + i];
		}
	}

	for (int i = 0; i < n; i++)
	{
		std::cout << result[i] << "\t";
	}
}