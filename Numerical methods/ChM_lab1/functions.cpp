#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include "functions.h"

/*
matrix_type = 1: default test matrix (p is half-width)
2: matrix with a varying condition number (k:=p)
3: Gilbert matrix of dimension n
*/
void readParameters(int& n, int& p, int& matrix_type, int& k)
{
	std::ifstream input;
	input.open("dimension,p,matrix_type.txt");
	if (!input.is_open())
	{
		std::cout << "dimension,p,matrix_type.txt file couldn't be opened, exiting..";
		exit(0);
	}
	input >> n >> p >> matrix_type;
	input.close();
	if (n < 0 || p < 0)
	{
		std::cout << "Incorrect parameters, exiting..";
		exit(0);
	}
	if (matrix_type == 2 || matrix_type == 3)
	{
		k = p;
		p = n - 1;
	}
}

void createMatrixFile(int& n, int& p, int& matrix_type, int& k)
{
	std::ofstream output;
	output.open("matrix_al.txt");
	output << std::setprecision(20);
	switch (matrix_type)
	{
	default:
		std::cout << "Invalid matrix type, exiting..";
		output.close();
		exit(0);
	case 1:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < p + 1; j++)
				output << (p - i > j ? 0 : (j + i * 10 + 1)) << '\t';
			output << '\n';
		}
		break;
	case 2:
	{
		int unique_non_diag_elem_number = (n * n - n) / 2;
		int* unique_a_ijs = new int[unique_non_diag_elem_number];
		double r;
		double element_probability_distribution[5] = { .8, .05, .05, .05, .05 };
			//{.6, .1, .1, .1, .1}
			//{.2, .2, .2, .2, .2}
			//{.8, .05, .05, .05, .05}
		for (int i = 0; i < unique_non_diag_elem_number; i++)
		{
			r = double(rand()) / RAND_MAX;
			for (int k = 0; k < 5; k++)
			{
				r -= element_probability_distribution[k];
				if (r <= 0)
				{
					unique_a_ijs[i] = -k;
					break;
				}
			}
			//unique_a_ijs[i] = -int((5 * double(rand()) / RAND_MAX));
		}
		int counter = 0; // how many a_ij (i!=j) have been used up
		precision elem = 0;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n - 1 - i; j++)
			{
				output << 0 << '\t';
			}
			for (int j = n - 1 - i; j < n; j++)
			{
				elem = 0;
				if (j == n - 1)
				{
					for (int k = 0; k < unique_non_diag_elem_number; k++)
						elem += 2 * unique_a_ijs[k]; // summing up all a_ij of a symmetrical matrix
					elem *= -1;
					if (i == 0)
					{
						std::cout << std::setprecision(20) << "\nelem before: " << elem;
						elem += pow(10, -k);
						std::cout << std::setprecision(20) << "\nelem after: " << elem;
					}
				}
				else
				{
					if (counter > unique_non_diag_elem_number - 1) // ...
					{
						std::cout << "Something went wrong";
						exit(0);
					}
					elem = unique_a_ijs[counter];
					counter++;
				}
				output << elem << '\t';
			}
			output << '\n';
		}
		delete[] unique_a_ijs;
		break;
	}
	case 3:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < p + 1; j++)
				output << (p - i > j ? 0 : 1./( (2*i+j) - (n-2) )) << '\t';
			output << '\n';
		}
		break;
	case 4:
		break;
	}
	output.close();
}

void createVectorFile(precision** al, const int& n, const int& matrix_type)
{
	std::ofstream output;
	output.open("vector_b.txt");
	output << std::setprecision(20);
	switch (matrix_type)
	{
	case 1:
		for (int i = 0; i < n; i++)
			output << i * 1.5 + 1 << '\t';
	case 2: case 3: case 4:
	{
		precision* x = new precision[n];
		precision* result = new precision[n];
		for (int i = 0; i < n; i++)
		{
			x[i] = i + 1;
			result[i] = 0;
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = n - 1 - i; j < n; j++)
			{
				result[i] += al[i][j] * x[j + i - n + 1];
				if (j != n - 1)
				{
					result[j + i - n + 1] += al[i][j] * x[i];
				}
			}
		}
		for (int i = 0; i < n; i++)
		{
			output << result[i] << "\t";
		}

		delete[] x;
		delete[] result;
	}
	}
	output.close();
}

void readMatrix(precision** al, const int& n, const int& p, const int& matrix_type)
{
	std::ifstream input;
	if(matrix_type==4)
		input.open("custom_matrix_al.txt");
	else
		input.open("matrix_al.txt");
	input >> std::setprecision(20);
	if (!input.is_open())
	{
		std::cout << "Matrix file couldn\'t be opened";
		exit(0);
	}
	for (int i = 0; i < n; i++)
	{
		al[i] = new precision[p + 1];
		for (int j = 0; j < p + 1; j++)
			input >> al[i][j];
	}
	input.close();
}

void readVector(precision*& b, const int& n, const int& matrix_type)
{
	std::ifstream input;
	if (false) //matrix_type == 4
		input.open("custom_vector_b.txt");
	else
		input.open("vector_b.txt");
	input >> std::setprecision(20);
	if (!input.is_open())
	{
		std::cout << "Vector file couldn\'t be opened";
		exit(0);
	}
	for (int i = 0; i < n; i++)
	{
		input >> b[i];
	}
	input.close();
}

void calculateDecomposition(precision** al, const int& n, const int& p)
{
	precision sum_over_k = 0;
	//double sum_over_k = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0, lij=p-i; j < i; lij++, j++)
		{
			sum_over_k = 0;
			for (int k = j - 1, lik = p - i + j - 1, ljk = p - 1; k >= 0 && lik >= 0; k--, lik--, ljk--)
				//for (int k = j-1, lik = p - i+j-1, ljk = p - 1; k>=0 && lik >=0; k--, lik--, ljk--)
			{
				sum_over_k += al[i][lik] * al[k][p] * al[j][ljk];
					//sum_over_k += al[i][lik] * al[k][p] * al[j][ljk];
			}
			if (al[j][p] == 0)
			{
				std::cout << "\nMatrix cannot be decomposed, exiting..";
				exit(0);
			}
			al[i][lij] = (al[i][lij] - sum_over_k) / al[j][p];
		}
		sum_over_k = 0;
		for (int k = i - 1, lik = p - 1; k >= 0 && lik >= 0; k--, lik--)
		{
			sum_over_k += al[i][lik] * al[k][p] * al[i][lik];
		}
		al[i][p] = (al[i][p] - sum_over_k);
	}
}

void printVariables(precision** al, precision* b, const int& n,const int& p)
{
	std::cout << std::setprecision(20);
	std::cout << "n: " << n << "\tp: " << p;
	std::cout << "\nb: ";
	for (int i = 0; i < n; i++)
	{
		std::cout << b[i] << '\t';
	}
	std::cout << "\nal:\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j <p+1; j++)
			std::cout << al[i][j] << '\t';
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

void solveForX(precision** al, precision* &b, const int& n, const int& p)
{
	precision sum_over_k = 0;
	//double sum_over_k = 0;
	for (int i = 0; i < n; i++)
	{
		sum_over_k = 0;
		for (int lk = 0, k = i-p; lk < p; k++, lk++)
		{
			sum_over_k += al[i][lk] * b[k];
		}
		b[i] -= sum_over_k;
	}
	for (int i = n - 1; i >= 0; i--)
	{
		sum_over_k = 0;
		for (int k = i + 1, li = p - 1; k < n && k <=p+i; k++, li--)//int k = i + 1; k < n; k++
		{
			sum_over_k += al[k][li] * b[k];
		}
		if (al[i][p] == 0)
		{
			std::cout << "\nMatrix cannot be decomposed, exiting..";
			exit(0);
		}
		b[i] = b[i] / al[i][p] - sum_over_k;
	}
	std::cout << "x: ";
	for (int i = 0; i < n; i++)
		std::cout << b[i] << '\t';
	std::ofstream output;
	output.open("vector_x.txt");
	precision var = 1;
	if(sizeof(var) == 4)
		output << std::fixed <<std::setprecision(8); //<<std::fixed
	else
		output << std::fixed << std::setprecision(16);
	for (int i = 0; i < n; i++)
		output << b[i] << ' ';
	output.close();
	output.open("delta_x.txt");
	output << std::scientific << std::setprecision(2);
	for (int i = 0; i < n; i++)
		output << i+1-b[i] << ' ';
	output.close();
}

void printDenseMatrix(precision** al, const int& n, const int& p)
{
	precision elem;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (abs(i - j) > p)
				elem = 0;
			else
			{
				if (i - j > 0)
					elem = al[i][p - i + j];
				else
					elem = al[j][p - j + i];
			}
			std::cout << elem << '\t';
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void multiplyAlByVector(precision** al, const int& n)
{
	precision* x = new precision[n];
	precision* result = new precision[n];
	for (int i = 0; i < n; i++)
	{
		x[i] = i + 1;
		result[i] = 0;
	}

	for (int i = 0; i < n; i++)
	{
		std::cout<< x[i] << "\t";
	}
	std::cout << '\n';

	precision al_i_j, x_j;
	for (int i = 0; i < n; i++)
	{
		for (int j = n - 1 - i; j < n; j++)
		{
			al_i_j = al[i][j];
			x_j = x[j+i-n+1];
			result[i] += al_i_j * x_j; //result[i] += al[i][j] * x[i];
			if (j!=n-1)
			{
				result[j + i - n + 1] += al[i][j] * x[i];
			}
		}
	}
	std::cout << "\nresult: ";
	for (int i = 0; i < n; i++)
	{
		std::cout << result[i] << "\t";
	}
	std::cout << '\n';

	delete[] x;
	delete[] result;
}

//not used
void manageInput(precision**& al, precision*& b, int& n, int& p)
{
	std::ifstream input;
	int matrix_type;
	input.open("dimension,p,matrix_type.txt");
	input >> n >> p >> matrix_type;
	input.close();
	if (matrix_type == 5)
		input.open("custom_matrix.txt");
	else
		input.open("matrix_al.txt");
	if (matrix_type == 2)
		p = n - 1;
	al = new precision* [n];
	for (int i = 0; i < n; i++)
	{
		al[i] = new precision[p + 1];
		for (int j = 0; j < p + 1; j++)
			input >> al[i][j];
	}
	input.close();
	b = new precision[n];
	precision al_i_k;
	/*if (matrix_type == 2)
	{
		for (int i = 0; i < n; i++)
		{
			b[i] = 0;
			for (int k = 0; k < n; k++)
			{
				al_i_k = al[i][p-i+k]
				b[i]+= al_i_k
			}
		}
		return;
	}*/
	input.open("vector_b.txt");
	for (int i = 0; i < n; i++)
		input >> b[i];
	input.close();
}

void solveUsingGauss(precision** al, precision*& b, const int& n, const int& p)
{
	precision** dense_al_with_b = new precision * [n];
	for (int i = 0; i < n; i++)
	{
		dense_al_with_b[i] = new precision[n + 1];
		for (int j = 0; j < n; j++)
		{
			if (abs(i - j) > p)
				dense_al_with_b[i][j] = 0;
			else
			{
				if(i >= j)
					dense_al_with_b[i][j] = al[i][p-i+j];
				else
					dense_al_with_b[i][j] = al[j][p-j+i];
			}
		}
		dense_al_with_b[i][n] = b[i];
	}
	//forward elimination
	precision max_leading_elem = 0;
	precision coefficient;
	int max_leading_elem_index = -1;
	precision temporary;
	for (int i = 0; i < n-1; i++) //n-1
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
		for (int j = 0; j < n+1; j++)
		{
			temporary = dense_al_with_b[i][j];
			dense_al_with_b[i][j] = dense_al_with_b[max_leading_elem_index][j];
			dense_al_with_b[max_leading_elem_index][j] = temporary;
		}
		//row operation
		for (int j = i+1; j < n; j++)
		{
			coefficient = -(dense_al_with_b[j][i] / dense_al_with_b[i][i]);
			for (int k = i; k < n+1; k++)
			{
				dense_al_with_b[j][k] += coefficient * dense_al_with_b[i][k];
			}
		}
		/*for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n + 1; j++)
			{
				std::cout << dense_al_with_b[i][j] << '\t';
			}
			std::cout << '\n';
		}
		std::cout << '\n';*/
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
		output << std::fixed << std::setprecision(8); //<<std::fixed
	else
		output << std::fixed << std::setprecision(16);
	for (int i = 0; i < n; i++)
		output << dense_al_with_b[i][n] << ' ';
	output.close();
	output.open("gauss_delta_x.txt");
	output << std::scientific << std::setprecision(2);
	for (int i = 0; i < n; i++)
		output << i + 1 - dense_al_with_b[i][n] << ' ';
	output.close();
	//print answer
	std::cout << "Gauss x: ";
	for (int i = 0; i < n; i++)
	{
		std::cout << dense_al_with_b[i][n] << '\t';
	}
	std::cout << '\n';
	for (int i = 0; i < n; i++)
	{
		delete[] dense_al_with_b[i];
	}
	delete[] dense_al_with_b;
}