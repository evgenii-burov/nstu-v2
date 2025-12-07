#include "linear_system.h"

LinearSystem::LinearSystem(std::string file_name)
{
	std::ifstream input_file(file_name);
	input_file >> n >> m >> eps;
	al.resize(n);
	au.resize(n);
	b.resize(n);
	di.resize(n);
	x.resize(n);
	new_x.resize(n);
	for (int i = 0; i < n; i++)
	{
		precision element;
		al[i].resize(num_diagonals);
		au[i].resize(num_diagonals);
		for (int j = 0; j < num_diagonals; j++)
		{
			input_file >> element;
			al[i][j] = element;
		}
		input_file >> element;
		di[i] = element;
		for (int j = 0; j < num_diagonals; j++)
		{
			input_file >> element;
			au[i][j] = element;
		}
	}

	for (int i = 0; i < n; i++)
	{
		precision element;
		input_file >> element;
		b[i] = element;
	}

	for (int i = 0; i < n; i++)
	{
		precision element;
		input_file >> element;
		x[i] = element;
	}
	diagonal_offset = {0, 1, m + 2,m + 3, n };
}

void LinearSystem::matrix_times_b()
{
	std::vector<precision> result(10, 0);
	//di
	for (int i = 0; i < n; i++)
	{
		result[i] += di[i] * b[i];
	}
	//al
	for (int k = 0; k <= num_diagonals; k++)
	{
		for (int i = diagonal_offset[k]; i < diagonal_offset[k + 1]; i++)
		{
			for (int j = num_diagonals - k; j < num_diagonals; j++)
			{
				//al
				result[i] += al[i][j] * b[i - diagonal_offset[num_diagonals - j]];
				//au
				result[i - diagonal_offset[num_diagonals - j]] += au[i][j] * b[i];
			}
		}
	}
	//au
	//for (int k = 0; k <= num_diagonals; k++)
	//{
	//	for (int i = diagonal_offset[k]; i < diagonal_offset[k + 1]; i++)
	//	{
	//		for (int j = num_diagonals - k; j < num_diagonals; j++)
	//		{
	//			result[i - diagonal_offset[num_diagonals - j]] += au[i][j] * b[i];
	//		}
	//	}
	//}

	for (int i = 0; i < n; i++)
	{
		std::cout << result[i] << "\t";
	}
}

precision LinearSystem::get_next_x_ith(int i, precision omega)
{
	int offset_al = 0;
	int offset_au = 0;
	for (int k = 0; k <= num_diagonals; k++)
	{
		if (i < diagonal_offset[k + 1])
		{
			offset_al = k;
			break;
		}
	}
	for (int k = 0; k <= num_diagonals; k++)
	{
		if ((n - 1 - i) < diagonal_offset[k + 1])
		{
			offset_au = k;
			break;
		}
	}

	precision next_x_ith = b[i] - di[i] * x[i];
	//al
	for (int j = num_diagonals - offset_al; j < num_diagonals; j++)
	{
		next_x_ith -= al[i][j] * x[i - diagonal_offset[num_diagonals - j]];
	}

	//au
	for (int j = 0; j < offset_au; j++)
	{
		next_x_ith -= au[i][j] * x[i + diagonal_offset[j + 1]];
	}
	//diagonal_offset = {0, 1, m + 2,m + 3, n };
	//{0,1,4,5,7}

	return x[i] + omega * next_x_ith / di[i];
}

precision LinearSystem::get_relative_discrepancy()
{
	precision discrepancy = 0;
	precision ax_ith = 0;

	int k_al = 0;
	int counter_al = diagonal_offset[k_al + 1] - diagonal_offset[k_al];
	int k_au = num_diagonals;
	int counter_au = diagonal_offset[k_au + 1] - diagonal_offset[k_au];

	for (int i = 0; i < n; i++)
	{
		ax_ith = di[i] * x[i];
		//al
		if (counter_al == 0)
		{
			k_al += 1;
			counter_al = diagonal_offset[k_al + 1] - diagonal_offset[k_al];
		}

		for (int j = num_diagonals - k_al; j < num_diagonals; j++)
		{
			ax_ith += al[i][j] * x[i - diagonal_offset[num_diagonals - j]];
		}

		counter_al--;
		//au
		if (counter_au == 0)
		{
			k_au -= 1;
			counter_au = diagonal_offset[k_au + 1] - diagonal_offset[k_au];
		}

		for (int j = 0; j < k_au; j++)
		{
			ax_ith += au[i][j] * x[i + diagonal_offset[j + 1]];
		}

		counter_au--;

		discrepancy += (b[i] - ax_ith) * (b[i] - ax_ith);
	}

	precision vector_b_norm = 0;
	for (int i = 0; i < n; i++)
	{
		vector_b_norm += b[i] * b[i];
	}

	discrepancy = sqrt(discrepancy / vector_b_norm);
	return discrepancy;
}

void LinearSystem::solve_jacobi()
{
	precision omega = 1;
	int iterations = 0;

	do
	{
		for (int i = 0; i < n; i++)
		{
			new_x[i] = get_next_x_ith(i, omega);
		}
		for (int i = 0; i < n; i++)
		{
			x[i] = new_x[i];
		}
	} while (get_relative_discrepancy() > eps || iterations > max_iterations);

	std::ofstream output_stream("jacobi_x.txt");
	for (int i = 0; i < n; i++)
	{
		//std::cout << x[i] << "\t";
		output_stream << std::scientific << std::setprecision(15);
		output_stream << x[i] << "\n";
	}


}
