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
	std::cout << "\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cout << dense_element(i, j) << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
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
		std::cout << "\nI " << i;
		int elements_in_row = ia[i + 1] - ia[i];
		int row_offset = i - elements_in_row;
		for (int il = 0; il < elements_in_row; il++)
		{
			std::cout << "\n\tIL " << il;
			int j = row_offset + il;
			std::cout << " J" << j << "AL " << al[ia[i] + il];
			int elements_in_col = ja[j + 1] - ja[j];
			int col_offset = j - elements_in_col;
			int max_offset = std::max(row_offset, col_offset);
			int sum_elements_count = j - max_offset;
			precision_sum sum_over_k = 0;
			for (int k = 0; k < sum_elements_count; k++)
			{
				sum_over_k += al[ia[i] + k + max_offset - row_offset] * di[max_offset + k] * au[ja[j] + k + max_offset - col_offset];
				std::cout << "\n\t\t" << al[ia[i] + k + max_offset - row_offset] << "*";
				std::cout << di[max_offset + k] << "*";
				std::cout << au[ja[j] + k + max_offset - col_offset] << "*";
			}
			std::cout << "\tDI " << di[row_offset + il];
			//al[ia[i] + il] = (al[ia[i] + il] - sum_over_k) / di[row_offset + il];
		}
		//au
		std::cout << "\nI " << i;
		elements_in_row = ja[i + 1] - ja[i];
		row_offset = i - elements_in_row;
		for (int il = 0; il < elements_in_row; il++)
		{
			std::cout << "\n\tIL " << il;
			int j = row_offset + il;
			std::cout << " J" << j << "AU " << au[ja[i] + il];
			int elements_in_col = ia[j + 1] - ia[j];
			int col_offset = j - elements_in_col;
			int max_offset = std::max(row_offset, col_offset);
			int sum_elements_count = j - max_offset;
			precision_sum sum_over_k = 0;
			for (int k = 0; k < sum_elements_count; k++)
			{
				sum_over_k += al[ia[j] + k + max_offset - col_offset] * di[max_offset + k] * au[ja[i] + k + max_offset - row_offset];
				std::cout << "\n\t\t" << al[ia[j] + k + max_offset - col_offset] << "*";
				std::cout << di[max_offset + k] << "*";
				std::cout << au[ja[i] + k + max_offset - row_offset] << "*";
			}
			std::cout << "\tDI " << di[col_offset + il];
			//au[ja[i] + il] = (au[ja[i] + il] - sum_over_k) / di[row_offset + il];
		}

		//di
		std::cout << "\nI " << i << "\tDI " << di[i];
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
			std::cout << "\n\t\t" << al[ia[i] + j + max_offset - row_offset] << "*";
			std::cout << di[max_offset + j] << "*";
			std::cout << au[ja[i] + j + max_offset - col_offset] << "*";
		}
		//di[i] -= sum_over_j; 
	}
}

//void LinearSystem::decompose_ldu()
//{
//	/*
//	d_i = a_ii - sum{j=1; j<i-1} (l_ij*d_j*u_ji)
//	l_ij = 1/d_j [ a_ij - sum{k=1; k<j-1} (l_ik*d_k*u_kj) ]
//	u_ij = 1/d_i [ a_ij - sum{k=1; k<i-1} (l_ik*d_k*u_kj) ]
//	*/
//	//Обход по матрице
//	for (int i = 0; i < n; i++) {
//		//Обход по i-ой строке
//		std::cout << "I:" << i << "\n";
//		int elements_in_row = ia[i + 1] - ia[i];
//		int row_offset = i - elements_in_row;
//		//l_ij = 1/d_j [ a_ij - sum{k=1; k<j-1} (l_ik*d_k*u_kj) ]
//		for (int il = 0; il < elements_in_row; il++)
//		{
//			//col number
//			std::cout << "\tIL:" << il << "\n";
//			int j = row_offset + il;
//			precision_sum sum_over_k = 0;
//			int elements_in_col = ja[j + 1] - ja[j];
//			int col_offset = j - elements_in_col;
//			int lowest_elem_count = std::min(elements_in_row, elements_in_col);
//			for (int k = 0; k < lowest_elem_count; k++)
//			{
//				std::cout << "\t\tK:" << k << "\n";
//				sum_over_k += al[ia[i] + k + (il - lowest_elem_count)] \
//					* di[j - lowest_elem_count + k] * au[ja[j] + k + (elements_in_col - lowest_elem_count)];
//		
//				std::cout << "+=al*di*au: " << al[ia[i] + k + (il - lowest_elem_count)] << "*";
//				std::cout << di[j - lowest_elem_count + k] << "*";
//				std::cout << au[ja[j] + k + (elements_in_col - lowest_elem_count)] << "\n";
//			}
//			std::cout << "al modified: " << al[ia[i] + il] << " d_j: " << di[j] << "\n";
//			if (di[j] == 0)
//			{
//				std::cout << "Unable to decompose the matrix.";
//				exit(0);
//			}
//			al[ia[i] + il] = (al[ia[i] + il] - sum_over_k) / di[j];
//		}
//		
//		//u_ij = 1 / d_i[a_ij - sum{ k = 1; k < i - 1 } (l_ik* d_k* u_kj)]
//		std::cout << "I:" << i << "\n";
//		elements_in_row = ja[i + 1] - ja[i];
//		row_offset = i - elements_in_row;
//		for (int il = 0; il < elements_in_row; il++)
//		{
//			//col number
//			std::cout << "\tIL:" << il << "\n";
//			int j = row_offset + il;
//			precision_sum sum_over_k = 0;
//			int elements_in_col = ia[j + 1] - ia[j];
//			int col_offset = j - elements_in_col;
//			int lowest_elem_count = std::min(elements_in_row, elements_in_col);
//			for (int k = 0; k < lowest_elem_count; k++)
//			{
//				std::cout << "\t\tK:" << k << "\n";
//				sum_over_k += au[ja[i] + k + (il - lowest_elem_count)] \
//					* di[j - lowest_elem_count + k] * al[ia[j] + k + (elements_in_col - lowest_elem_count)];
//		
//				std::cout << "+=al*di*au: " << au[ja[i] + k + (il - lowest_elem_count)] << "*";
//				std::cout << di[j - lowest_elem_count + k] << "*";
//				std::cout << al[ia[j] + k + (elements_in_col - lowest_elem_count)] << "\n";
//			}
//			std::cout << "au modified: " << au[ja[i] + il] << " d_j: " << di[j] << "\n";
//			if (di[j] == 0)
//			{
//				std::cout << "Unable to decompose the matrix.";
//				exit(0);
//			}
//			au[ja[i] + il] = (au[ja[i] + il] - sum_over_k) / di[j];
//		}
//		//d_i = a_ii - sum{ j = 1; j < i - 1 } (l_ij* d_j* u_ji)
//		std::cout << "I:" << i << "\n";
//		elements_in_row = ia[i + 1] - ia[i];
//		row_offset = i - elements_in_row;
//		
//		//col number
//		int j = i;
//		precision_sum sum_over_k = 0;
//		int elements_in_col = ja[j + 1] - ja[j];
//		int col_offset = j - elements_in_col;
//		int lowest_elem_count = std::min(elements_in_row, elements_in_col);
//		for (int k = 0; k < lowest_elem_count; k++)
//		{
//			std::cout << "\t\tK:" << k << "\n";
//			sum_over_k += al[ia[i] + k + (elements_in_row - lowest_elem_count)] \
//				* di[j - lowest_elem_count + k] * au[ja[j] + k + (elements_in_col - lowest_elem_count)];
//		//sum_over_k += al[ia[i] + k + (i - lowest_elem_count)] \
//				* di[j - lowest_elem_count + k] * au[ja[j] + k + (elements_in_col - lowest_elem_count)];
//			std::cout << "+=al*di*au: " << al[ia[i] + k + (elements_in_row - lowest_elem_count)] << "*";
//			std::cout << di[j - lowest_elem_count + k] << "*";
//			std::cout << au[ja[j] + k + (elements_in_col - lowest_elem_count)] << "\n";
//		}
//		std::cout << "di modified: " << di[i] << "\n";
//		di[i] -= sum_over_k;
//	}
//}

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
			sum_over_j += au[ja[j] + column_start_to_target_distance ] * b[j] * 
				 (column_start_to_target_distance >= 0);
			
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

}