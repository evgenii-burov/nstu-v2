#include "LinearSystem.h"

LinearSystem::LinearSystem(std::string file_name, int matrix_type, int optional_k) :
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
	for (int i = 0; i < element_int; i++)
	{
		input_stream >> element_float;
		al.push_back(element_float);
	}
	for (int i = 0; i < n + 1; i++)
	{
		input_stream >> element_int;
		ja.push_back(element_int);
	}
	for (int i = 0; i < element_int; i++)
	{
		input_stream >> element_float;
		au.push_back(element_float);
	}
	for (int i = 0; i < n; i++)
	{
		input_stream >> element_float;
		di.push_back(element_float);
	}
	if(matrix_type == 1)
	{
		for (int i = 0; i < n; i++)
		{
			input_stream >> element_float;
			b.push_back(element_float);
		}
	}
	if (matrix_type == 2)
	{
		di[0] += pow(10, -optional_k);
		//b
	//diagonal
		b = di;
		for (int i = 0; i < n; i++)
		{
			b[i] *= i + 1;
		}
		//lower triangle
		int elements_in_line = 0;
		for (int i = 1; i < n; i++)
		{
			elements_in_line = ia[i + 1] - ia[i];
			for (int j = 0; j < elements_in_line; j++)
			{
				b[i] += al[ia[i] + j] * (i - elements_in_line + j + 1);
			}
		}
		//upper triangle
		for (int i = 1; i < n; i++)
		{
			elements_in_line = ja[i + 1] - ja[i];
			for (int j = 0; j < elements_in_line; j++)
			{
				b[i - elements_in_line + j] += au[ja[i] + j] * (i + 1);
			}
		}
	}
}

void LinearSystem::print_ls()
{
	std::cout << std::fixed << std::setprecision(15);
	std::cout << "n: " << n << "\nia: ";
	for (int i = 0; i < n + 1; i++)
	{
		std::cout << ia[i] << " ";
	}
	std::cout << "\nja: ";
	for (int i = 0; i < n+1; i++)
	{
		std::cout << ja[i] << " ";
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
	for (int i = 0; i < ja[n]; i++)
	{
		std::cout << au[i] << " ";
	}
	std::cout << "\nb: ";
	for (int i = 0; i < n; i++)
	{
		std::cout << b[i] << " ";
	}
	std::cout << "\n\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cout << dense_element(i, j) << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::ofstream output_stream("b_vertical.txt");
	output_stream << std::scientific << std::setprecision(15);
	for (int i = 0; i < n; i++)
	{
		output_stream << b[i] << "\n";
	}
	output_stream.close();
	output_stream = std::ofstream("b_horizontal.txt");
	output_stream << std::scientific << std::setprecision(15);
	for (int i = 0; i < n; i++)
	{
		output_stream << b[i] << "\t";
	}
	output_stream.close();
}

LinearSystem::LinearSystem(int matrix_size, int matrix_type, int optional_k) : n(matrix_size)
{
	/*
	matrix type 1: diagonal significance matrix
	matrix type 2: dense matrix
	matrix type 3: Hilbert's matrix
	*/
	if (matrix_size < 1)
	{
		std::cerr << "Matrix size must be positive\n";
		exit(0);
	}
	if (matrix_type < 1 || matrix_type > 3)
	{
		std::cout << "Unknown matrix type\n";
		exit(0);
	}
	std::random_device rd;
	if (matrix_type == 1)
	{
		std::vector<double> a_ij_values = { 0, -1, -2, -3, -4 };
		std::uniform_int_distribution<> distr(0, a_ij_values.size() - 1);
		ia.push_back(0);
		ja.push_back(0);
		for (int i = 0; i < matrix_size; i++)
		{
			int row_offset = 0;
			int col_offset = 0;
			for (int offset = 0; offset < i; offset++)
			{
				if (a_ij_values[distr(rd)] == 0)
				{
					row_offset++;
				}
				if (a_ij_values[distr(rd)] == 0)
				{
					col_offset++;
				}
			}
			ia.push_back(ia[i] + i - row_offset);
			ja.push_back(ja[i] + i - col_offset);
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < ia[i + 1] - ia[i]; j++)
			{
				int element_chosen = a_ij_values[distr(rd)];
				while( j==0 && element_chosen == 0)
					element_chosen = a_ij_values[distr(rd)];
				al.push_back(a_ij_values[distr(rd)]);
			}
			for (int j = 0; j < ja[i + 1] - ja[i]; j++)
			{
				int element_chosen = a_ij_values[distr(rd)];
				while (j == 0 && element_chosen == 0)
					element_chosen = a_ij_values[distr(rd)];
				au.push_back(a_ij_values[distr(rd)]);
			}
		}
		for (int i = 0; i < n; i++)
		{
			precision di_i = 0;
			if (i == 0)
				di_i -= pow(10, -optional_k);
			for (int j = ia[i]; j < ia[i + 1]; j++)
			{
				di_i += al[j];
			}
			for (int k = i + 1; k < n; k++)
			{
				int elements_in_col = ja[k + 1] - ja[k];
				int col_offset = k - elements_in_col;
				if (col_offset <= i)
				{
					di_i += au[ja[k] + i - col_offset];
				}
			}
			di.push_back(-di_i);
		}
	}
	if (matrix_type == 3)
	{
		ia.push_back(0);
		ja.push_back(0);
		for (int i = 0; i < matrix_size; i++)
		{
			ia.push_back(ia[i] + i);
			ja.push_back(ja[i] + i);
		}
		for (int i = 0; i < n; i++)
		{
			di.push_back(1.0 / (2 * i + 1));
			for (int j = 0; j < i; j++)
			{
				al.push_back(1.0 / (i + j + 1));
				au.push_back(1.0 / (i + j + 1));
			}
		}
	}
	//b
	//diagonal
	b = di;
	for (int i = 0; i < n; i++)
	{
		b[i] *= i+1;
	}
	//lower triangle
	int elements_in_line = 0;
	for (int i = 1; i < n; i++)
	{
		elements_in_line = ia[i + 1] - ia[i];
		for (int j = 0; j < elements_in_line; j++)
		{
			b[i] += al[ia[i] + j] * (i - elements_in_line + j + 1);
			std::cout << "\nal*b" << al[ia[i] + j] << "*" << (i - elements_in_line + j + 1);
		}
	}
	//upper triangle
	for (int i = 1; i < n; i++)
	{
		elements_in_line = ja[i + 1] - ja[i];
		for (int j = 0; j < elements_in_line; j++)
		{
			b[i - elements_in_line + j] += au[ja[i] + j] * (i+1);
			std::cout << "\nau*b" << au[ja[i] + j] << "*" << (i + 1);
		}
	}
}

void LinearSystem::decompose_ldu()
{
	/*
	d_i = a_ii - sum{j=1; j<i-1} (l_ij*d_j*u_ji)
	l_ij = 1/d_j [ a_ij - sum{k=1; k<j-1} (l_ik*d_k*u_kj) ]
	u_ij = 1/d_i [ a_ij - sum{k=1; k<i-1} (l_ik*d_k*u_kj) ]
	*/
	//al
	for (int i = 0; i < n; i++)
	{
		int elements_in_row = ia[i + 1] - ia[i];
		int row_offset = i - elements_in_row;
		for (int il = 0; il < elements_in_row; il++)
		{
			int j = row_offset + il;
			int elements_in_col = ja[j + 1] - ja[j];
			int col_offset = j - elements_in_col;
			int max_offset = std::max(row_offset, col_offset);
			int sum_elements_count = j - max_offset;
			precision_sum sum_over_k = 0;
			for (int k = 0; k < sum_elements_count; k++)
			{
				sum_over_k += al[ia[i] + k + max_offset - row_offset] * di[max_offset + k] * au[ja[j] + k + max_offset - col_offset];
			}
			if (di[row_offset + il] == 0)
			{
				std::cout << "Matrix cannot be LDU decomposed\n";
				exit(0);
			}
			al[ia[i] + il] = (al[ia[i] + il] - sum_over_k) / di[row_offset + il];
		}
		//au
		elements_in_row = ja[i + 1] - ja[i];
		row_offset = i - elements_in_row;
		for (int il = 0; il < elements_in_row; il++)
		{
			int j = row_offset + il;
			int elements_in_col = ia[j + 1] - ia[j];
			int col_offset = j - elements_in_col;
			int max_offset = std::max(row_offset, col_offset);
			int sum_elements_count = j - max_offset;
			precision_sum sum_over_k = 0;
			for (int k = 0; k < sum_elements_count; k++)
			{
				sum_over_k += al[ia[j] + k + max_offset - col_offset] * di[max_offset + k] * au[ja[i] + k + max_offset - row_offset];
			}
			if (di[row_offset + il] == 0)
			{
				std::cout << "Matrix cannot be LDU decomposed\n";
				exit(0);
			}
			au[ja[i] + il] = (au[ja[i] + il] - sum_over_k) / di[row_offset + il];
		}

		//di
		elements_in_row = ia[i + 1] - ia[i];
		row_offset = i - elements_in_row;
		int elements_in_col = ja[i + 1] - ja[i];
		int col_offset = i - elements_in_col;
		int max_offset = std::max(row_offset, col_offset);
		int sum_elements_count = i - max_offset;
		precision_sum sum_over_j = 0;
		for (int j = 0; j < sum_elements_count; j++)
		{
			sum_over_j += al[ia[i] + j + max_offset - row_offset] * di[max_offset + j] * au[ja[i] + j + max_offset - col_offset];
		}
		di[i] -= sum_over_j; 
	}
}

void LinearSystem::solve_DUx_y()
{
	//Ux=(D^-1)y= {y1/di1; y2/di2, ...}
	for (int i = 0; i < n; i++)
	{
		b[i] /= di[i];
	}
	//x_i = y_i - sum{j=i+1; j<n} (U_ij*x_j)
	precision_sum sum_over_j = 0;
	for (int i = n - 1; i >= 0; i--)
	{
		sum_over_j = 0;
		int elements_in_column = 0;
		int column_start_offset = 0;
		int column_start_to_target_distance = 0;
		for (int j = i + 1; j < n; j++)
		{
			elements_in_column = ja[j + 1] - ja[j];
			column_start_offset = j - elements_in_column;
			column_start_to_target_distance = i - column_start_offset;
			//if distance is negative, multiply by zero
			if (ja[j] + column_start_to_target_distance < 0)
				continue;
			sum_over_j += au[ja[j] + column_start_to_target_distance ] * b[j] * 
				 int(column_start_to_target_distance >= 0);
			
		}
		b[i] -= sum_over_j;
	}
}

void LinearSystem::solve_Ly_b()
{
	int elements_in_line = 0;
	for (int i = 1; i < n; i++)
	{
		precision_sum sum_over_j = 0;
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
		elements_in_line = ja[i + 1] - ja[i];
		for (int j = 0; j < elements_in_line; j++)
		{
			std::cout << i << " " << au[ja[i] + j] << " " << b[i] << "\n";
			result[i - elements_in_line + j] += au[ja[i] + j] * b[i];
		}
	}
	std::cout << "u\n";
	for (int i = 0; i < n; i++)
	{
		std::cout << result[i] << " ";
	}
	std::cout << "\nu";
}

precision LinearSystem::dense_element(int i, int j)
{
	bool value_set = false;
	precision element = 0;
	if (i > j)
	{
		int row_offset = i - (ia[i + 1] - ia[i]);
		if (j >= row_offset)
		{
			value_set = true;
			return al[ia[i]+j-row_offset];
		}
	}
	if (i < j)
	{
		int col_offset = j - (ja[j + 1] - ja[j]);
		if (i >= col_offset)
		{
			value_set = true;
			return au[ja[j]+i-col_offset];
		}
	}
	return i == j ? di[i] : 0;
}

void LinearSystem::solve_Gauss()
{
	std::vector <std::vector<precision>> dense_al_with_b;
	for (int i = 0; i < n; i++)
	{
		dense_al_with_b.push_back({});
		for (int j = 0; j < n; j++)
		{
			dense_al_with_b[i].push_back(dense_element(i, j));
		}
		dense_al_with_b[i].push_back(b[i]);
	}
	//forward elimination
	precision max_leading_elem = 0;
	precision coefficient;
	int max_leading_elem_index = -1;
	precision temporary;
	for (int i = 0; i < n - 1; i++) //n-1
	{
		max_leading_elem = 0;
		//find the leading element (in absolute value)
		for (int j = i; j < n; j++)
		{
			if (max_leading_elem < abs(dense_al_with_b[j][i]))
			{
				max_leading_elem = abs(dense_al_with_b[j][i]);
				max_leading_elem_index = j;
			}
		}
		//std::cout << "\nleading:" << max_leading_elem_index << '\n';
		if (max_leading_elem == 0)
		{
			continue;
		}
		//swapping ith row with the row with the leading elem
		for (int j = 0; j < n + 1; j++)
		{
			temporary = dense_al_with_b[i][j];
			dense_al_with_b[i][j] = dense_al_with_b[max_leading_elem_index][j];
			dense_al_with_b[max_leading_elem_index][j] = temporary;
		}
		//row operation
		for (int j = i + 1; j < n; j++)
		{
			coefficient = -(dense_al_with_b[j][i] / dense_al_with_b[i][i]);
			for (int k = i; k < n + 1; k++)
			{
				dense_al_with_b[j][k] += coefficient * dense_al_with_b[i][k];
			}
		}
		//for (int i = 0; i < n; i++)
		//{
		//	for (int j = 0; j < n + 1; j++)
		//	{
		//		std::cout << dense_al_with_b[i][j] << '\t';
		//	}
		//	std::cout << '\n';
		//}
		//std::cout << '\n';
	}
	//back substitution
	precision sum_over_k;
	for (int i = n - 1; i >= 0; i--)
	{
		sum_over_k = 0;
		for (int k = i + 1; k < n; k++)
		{
			sum_over_k += dense_al_with_b[i][k] * dense_al_with_b[k][n];
		}

		if (dense_al_with_b[i][i] == 0)
		{
			if (dense_al_with_b[i][n] == 0)
				continue;
			std::cout << "No solutions";
			exit(0);
		}
		dense_al_with_b[i][n] = (dense_al_with_b[i][n] - sum_over_k) / dense_al_with_b[i][i];
	}
	//write answer to a file
	std::ofstream output;
	output.open("gauss_vector_x.txt");
	precision var = 1;
	if (sizeof(var) == 4)
		output << std::scientific << std::setprecision(15); //<<std::fixed
	else
		output << std::scientific << std::setprecision(15);
	for (int i = 0; i < n; i++)
		output << dense_al_with_b[i][n] << '\n';
	output.close();
	output.open("gauss_delta_x.txt");
	output << std::scientific << std::setprecision(3);
	for (int i = 0; i < n; i++)
		output << i + 1 - dense_al_with_b[i][n] << '\n';
	output.close();
	//print answer
	//std::cout << "Gauss x: ";
	//for (int i = 0; i < n; i++)
	//{
	//	std::cout << dense_al_with_b[i][n] << '\t';
	//}
	//std::cout << '\n';
}

void LinearSystem::compare_x()
{
	std::ofstream output_stream("LDU_vector_x.txt");
	output_stream << std::scientific << std::setprecision(15);
	for (int i = 0; i < n; i++)
	{
		output_stream << b[i] << "\n";
	}
	output_stream.close();
	output_stream = std::ofstream("LDU_delta_x.txt");
	output_stream << std::scientific << std::setprecision(3);
	for (int i = 0; i < n; i++)
	{
		output_stream << (i + 1) - b[i] << "\n";
	}
	output_stream.close();
}