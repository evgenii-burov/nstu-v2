#pragma once
#include <iostream>

class Computing
{
public:
	double dot_product_sequential(const int n, const double* vector_a, const double* vector_b);
	double dot_product_sequential(const double* vector_a, const double* vector_b)
	{
		return dot_product_sequential(default_size, vector_a, vector_b);
	}
	double dot_product_parallel(const int n, const double* vector_a, const double* vector_b);
	double dot_product_parallel(const double* vector_a, const double* vector_b)
	{
		return dot_product_parallel(default_size, vector_a, vector_b);
	}
	void set_n(int n)
	{
		default_size = n;
	}
	void initialize_vector(const int n, double*& vector);
	void initialize_vector(double*& vector)
	{
		initialize_vector(default_size, vector);
	}
	void print_vector(const int n, const double* vector);
	void print_vector(const double* vector)
	{
		print_vector(default_size, vector);
	}
private:
	int default_size = 10;
};