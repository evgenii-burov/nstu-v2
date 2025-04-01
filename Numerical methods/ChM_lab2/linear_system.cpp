#include "linear_system.h"

LinearSystem::LinearSystem(std::string input_file_name)
{
	std::ifstream input_file;
	input_file.open(input_file_name);
	if (!input_file.is_open())
		throw("Input file couldn't be opened: " + input_file_name);
	int n0, m0, matrix_type;
	input_file >> n0 >> m0 >> matrix_type;
	
	if (n0 < 3)
		throw("Incorrect parameter: n must be 3 or greater");
	if (m0 < 2 || m0 > n0-1)
		// 2<=m <= n-1
		throw("Incorrect parameter: m must satisfy 2<=m<=n-1");
	n = n0;
	m = m0;

	shift = { -m, -1, 0, 1, m};

	precision var;
	if (sizeof(var) == 4)
		eps = 1e-6;
	else
		eps = 1e-14;
	initializeMatrix(matrix_type);
	initializeVectorB(matrix_type);
	initializeVectorX(matrix_type);
}

void LinearSystem::initializeMatrix(int matrix_type)
{
	matrix.resize(n);
	switch (matrix_type)
	{
	case 1:
		for (int i = 0; i < n; i++)
		{
			//matrix[i].push_back(i > m - 1 ? i+1 : 0);
			//matrix[i].push_back(i > 0 ? i +2 : 0);
			//matrix[i].push_back(i + 3);
			//matrix[i].push_back(i < n - 1 ? i + 4: 0);
			//matrix[i].push_back(i < n - m ? i + 5: 0);
			matrix[i].push_back(i > m - 1 ? 1+int(10 * double(rand()) / RAND_MAX) : 0);
			matrix[i].push_back(i > 0 ? 1 + int(10 * double(rand()) / RAND_MAX) : 0);
			matrix[i].push_back(1 + int(10 * double(rand()) / RAND_MAX));
			matrix[i].push_back(i < n - 1 ? 1 + int(10 * double(rand()) / RAND_MAX) : 0);
			matrix[i].push_back(i < n - m ? 1 + int(10 * double(rand()) / RAND_MAX) : 0);
		}
		break;
	case 2:
	{
		std::ifstream input_file;
		input_file.open("matrix.txt");
		for (int i = 0; i < n; i++)
		{
			matrix[i].resize(5);
			for (int j = 0; j < 5; j++)
			{
				input_file >> matrix[i][j];
			}
		}
	}
	break;
	}
}

void LinearSystem::initializeVectorB(int matrix_type)
{
	switch (matrix_type)
	{
	case 1:
		for (int i = 0; i < n; i++)
		{
			b.push_back(i + 1);
		}
		break;
	case 2:
		//for (int i = 0; i < n; i++)
		//{
		//	b.push_back(i + 1);
		//}
		//break;
		for (int i = 0; i < n; i++)
		{
			b.push_back(0);
			for (int j = 0; j < n; j++)
			{
				b[i] += ithJthDenseElem(i, j) * (j + 1);
			}
		}
	}
}

void LinearSystem::initializeVectorX(int matrix_type)
{
	for (int i = 0; i < n; i++)
	{
		x.push_back(0);
	}
}

precision LinearSystem::getRelativeDiscrepancy() const
{
	precision b_norm=0;
	precision b_minus_ax_norm=0;
	precision b_minus_ax_norm_ith = 0;
	for (int i = 0; i < n; i++)
	{
		b_minus_ax_norm_ith = b[i];
		int first_diag_start_offset = (i > 0 ? 1 : 0);
		int second_diag_start_offset = (i > m - 1 ? 1 : 0);
		int first_diag_end_offset = (i > n - 1 - 1 ? 1 : 0);
		int second_diag_end_offset = (i > n - m - 1 ? 1 : 0);
		//j only goes over non-zero elements
		for (int j = 2 - first_diag_start_offset - second_diag_start_offset,j_dense= 0; j < 5 - first_diag_end_offset - second_diag_end_offset;j++)
		{
			j_dense = i + shift[j];
			b_minus_ax_norm_ith -= matrix[i][j] * x[j_dense];
		}
		b_minus_ax_norm += pow(b_minus_ax_norm_ith,2);
		b_norm += pow(b[i],2);
	}
	return sqrt(b_minus_ax_norm/b_norm);
}

//precision LinearSystem::getScalarProductOfIthRowAndX(int i, std::vector<precision> x) const
//{
//	precision scalar_product = 0;
//	int first_diag_start_offset = (i > 0 ? 1 : 0);
//	int second_diag_start_offset = (i > m - 1 ? 1 : 0);
//	int first_diag_end_offset = (i > n - 1 - 1 ? 1 : 0);
//	int second_diag_end_offset = (i > n - m - 1 ? 1 : 0);
//	//j only goes over non-zero elements
//	for (int j = 2 - first_diag_start_offset - second_diag_start_offset, j_dense = 0; j < 5 - first_diag_end_offset - second_diag_end_offset; j++)
//	{
//		j_dense = i + shift[j];
//		scalar_product += matrix[i][j] * x[j_dense];
//	}
//	return scalar_product;
//}

precision LinearSystem::getNextIterationOfXIth(int i, precision omega) const
{
	precision x_next = b[i];
	int first_diag_start_offset = (i > 0 ? 1 : 0);
	int second_diag_start_offset = (i > m - 1 ? 1 : 0);
	int first_diag_end_offset = (i > n - 1 - 1 ? 1 : 0);
	int second_diag_end_offset = (i > n - m - 1 ? 1 : 0);
	//j only goes over non-zero elements
	for (int j = 2 - first_diag_start_offset - second_diag_start_offset, j_dense = 0; j < 5 - first_diag_end_offset - second_diag_end_offset; j++)
	{
		j_dense = i + shift[j];
		x_next -= matrix[i][j] * x[j_dense];
	}
	x_next = x_next * omega / matrix[i][2] + x[i];
	return x_next;
}

void LinearSystem::solveJacobi()
{
	std::vector<precision> x_new;
	x_new.resize(n);
	precision relative_discrepancy;
	int iterations = 0;
	do
	{
		for (int i = 0; i < n; i++)
		{
			x_new[i] = getNextIterationOfXIth(i, omega_jacobi);
		}
		
		for (int i = 0; i < n; i++)
		{
			x[i] = x_new[i];
		}
		iterations++;
		relative_discrepancy = getRelativeDiscrepancy();
		//std::cout << "Jacobi iteration #" << iterations << "; relative discrepancy: " << relative_discrepancy << "\n";
	} while (relative_discrepancy > eps && iterations < max_iterations); //while (norm > eps && iterations < max_iterations);
	std::cout << "\nJacobi iterations: " << iterations << "\n";

	precision error_norm = 0, true_solution_norm=0;
	for (int i = 0; i < n; i++)
	{
		error_norm += pow(x[i] - (i + 1), 2);
		true_solution_norm += pow((i + 1), 2);
	}
	std::cout << "\nJacobi conditionality number: " << sqrt(error_norm / true_solution_norm)/getRelativeDiscrepancy() << "\n";


	std::ofstream output_stream("x_jacobi.txt");
	if (!output_stream)
		throw std::string("Unable to open output file");

	for (int i = 0; i < n; i++)
	{
		std::cout << x[i] << "\t";
		output_stream << x[i] << "\t";
	}
	std::cout << "\n***";
	for (int i = 0; i < n; i++)
	{
		std::cout << i+1-x[i] << "\t";
		//output_stream << x[i] << "\t";
		x[i] = 0;
	}
	std::cout << "\n";
	output_stream.close();
}

void LinearSystem::solveGaussSeidel()
{
	precision relative_discrepancy;
	int iterations = 0;
	do
	{
		for (int i = 0; i < n; i++)
		{
			x[i] = getNextIterationOfXIth(i, omega_gauss_seidel);
		}
		iterations++;
		relative_discrepancy = getRelativeDiscrepancy();
		//std::cout << "G-S iteration #" << iterations << "; relative discrepancy: " << relative_discrepancy << "\n";
	} while (relative_discrepancy > eps && iterations < max_iterations); //while (norm > eps && iterations < max_iterations);
	std::cout << "\nGauss-Seidel iterations: " << iterations << "\n";

	precision error_norm = 0, true_solution_norm = 0;
	for (int i = 0; i < n; i++)
	{
		error_norm += pow(x[i] - (i + 1), 2);
		true_solution_norm += pow((i + 1), 2);
	}
	std::cout << "\nGauss-Seidel condition number: " << sqrt(error_norm / true_solution_norm) / getRelativeDiscrepancy() << "\n";

	std::ofstream output_stream("x_gauss_seidel.txt");
	if (!output_stream)
		throw std::string("Unable to open output file");
	for (int i = 0; i < n; i++)
	{
		std::cout << x[i] << "\t";
		output_stream << x[i] << "\t";
	}
	std::cout << "\n***";
	for (int i = 0; i < n; i++)
	{
		std::cout << i + 1 - x[i] << "\t";
		//output_stream << x[i] << "\t";
		x[i] = 0;
	}
	std::cout << "\n";
	output_stream.close();
}

void LinearSystem::solveBlockRelaxation(int block_size)
{
	if (n % block_size != 0)
		throw("Incorrect parameter: block_size must divide the matrix dimension evenly");
	//block-slau matrix
	std::vector<std::vector<precision>> matrix_block;
	std::vector<precision> vector_y;
	std::vector<precision> vector_r;
	matrix_block.resize(block_size);
	for (int i = 0; i < block_size; i++)
	{
		matrix_block[i].resize(block_size);
	}
	vector_r.resize(block_size);
	vector_y.resize(block_size);
	//i is block slau index
	//for (int i = 0; i < n/block_size; i++)
	//{
	//	matrix_block.resize(block_size);
	//	//setting up the ith matrix block
	//	//j is matrix block row index
	//	for (int j = 0; j < block_size; j++)
	//	{
	//		matrix_block[j].resize(0);
	//		// k is matrix block column index
	//		for (int k = 0; k < block_size; k++)
	//		{
	//			int i_to_push, j_to_push;
	//			i_to_push = block_size * i + j;
	//			j_to_push =getDiagonalJFromDenseJ(i_to_push, block_size * i + k);
	//			matrix_block[j].push_back(matrix[i_to_push][j_to_push]);
	//		}
	//	}
	//	//printing matrix block
	//	for (int j = 0; j < block_size; j++)
	//	{
	//		// k is matrix block column index
	//		for (int k = 0; k < block_size; k++)
	//		{
	//			std::cout << matrix_block[j][k] << "\t";
	//		}
	//		std::cout << "\n";
	//	}
	//}
	for (int block_shift = 0; block_shift<n; block_shift+=block_size)
	{
		//initialize vector_r
		for (int i = 0; i < block_size; i++)
		{
			vector_r[i] = b[i + block_shift];
			precision AijTimesXj_i = 0;
			//for (int k = 0; k < n; k++)
			//{
			//	AijTimesXj_i += ithJthDenseElem(block_shift + i, k) * x[k];
			//}
			int first_diag_start_offset = (block_shift + i > 0 ? 1 : 0);
			int second_diag_start_offset = (block_shift + i > m - 1 ? 1 : 0);
			int first_diag_end_offset = (block_shift + i > n - 1 - 1 ? 1 : 0);
			int second_diag_end_offset = (block_shift + i > n - m - 1 ? 1 : 0);
			for (int k = 2 - first_diag_start_offset - second_diag_start_offset; k < 5 - first_diag_end_offset - second_diag_end_offset; k++)
			{
				AijTimesXj_i += matrix[block_shift + i][k] * x[block_shift+i+shift[k]];
			}
			vector_r[i] -= AijTimesXj_i;
			vector_r[i] *= omega_block_relaxation;
		}

		//set up the block slau matrix
		for (int i = block_shift; i < block_shift + block_size; i++)
		{
			for (int j = block_shift; j < block_shift + block_size; j++)
			{
				matrix_block[i - block_shift][j - block_shift] = ithJthDenseElem(i, j);
			}
		}

		//LU-decompose the block matrix
		precision sum_over_k = 0;
		for (int i = 0; i < block_size; i++)
		{
			for (int j = 0; j < block_size; j++)
			{
				sum_over_k = 0;
				//l ij
				if (j<=i)
				{
					for (int k = 0; k < j; k++)
					{
						sum_over_k += matrix_block[i][k] * matrix_block[k][j];
					}
					matrix_block[i][j] -= sum_over_k;
				}
				//u ij
				else
				{
					for (int k = 0; k < i; k++)
					{
						sum_over_k += matrix_block[i][k] * matrix_block[k][j];
					}
					matrix_block[i][j] -= sum_over_k;
					if (matrix_block[i][i] == 0)
						throw std::string("Unable to LU-decompose a block matrix");
					matrix_block[i][j] /= matrix_block[i][i];
				}
			}
		}
		

		for (int i = block_shift; i < block_shift + block_size; i++)
		{
			sum_over_k = 0;
			for (int k = block_shift; k < block_shift + block_size; k++)
			{
				sum_over_k += 999;
			}
		}
	}
}

precision LinearSystem::ithJthDenseElem(int i, int j) const
{
	if (abs(i - j) > 1 && abs(i - j) != m)
	{
		return 0;
	}

	if (abs(i - j) <= 1)
	{
		return matrix[i][2 - (i - j)];
	}

	if (i - j == m)
	{
		return matrix[i][0];
	}

	if (i - j == -m)
	{
		return matrix[i][4];
	}
}

void LinearSystem::printAnotha() const
{
	std::cout << "\n\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cout << ithJthDenseElem(i, j) << "\t";
		}
		std::cout << "\n";
	}
}

void LinearSystem::print() const
{
	std::cout << "\nn: " << n << "\tm: " << m;
	std::cout << "\nMatrix (diagonal format):\n";
	for (const auto row : matrix)
	{
		for (const auto elem : row)
		{
			std::cout << elem << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "\nMatrix (dense format):\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//if not the non-zero diagonal
			if (abs(i - j) > 1 && abs(i - j) != m)
			{
				std::cout << 0 << "\t";
				continue;
			}

			if (abs(i - j) <= 1)
			{
				std::cout << matrix[i][2 - (i - j)] << "\t";
				continue;
			}

			if (i - j == m)
			{
				std::cout << matrix[i][2 - 2] << "\t";
				continue;
			}

			if (i - j == -m)
			{
				std::cout << matrix[i][2 + 2] << "\t";
				continue;
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "\nVector b:\n";
	for (const auto elem : b)
	{
		std::cout << elem << "\t";
	}
	std::cout << "\nVector x:\n";
	for (const auto elem : x)
	{
		std::cout << elem << "\t";
	}
}
