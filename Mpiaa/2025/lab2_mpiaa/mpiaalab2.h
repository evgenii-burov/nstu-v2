#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>

double dot_product_sequential(const int n, const double* vector_a, const double* vector_b);

double dot_product_parallel(const int n, const double* vector_a, const double* vector_b, const int num_threads);

void matrix_multiplication_sequential(const int n, const int m, const int k,
	const double* matrix_a, const double* matrix_b, double*& matrix_c);

void matrix_multiplication_parallel(const int n, const int m, const int k,
	const double* matrix_a, const double* matrix_b, double*& matrix_c, const int num_threads);

double matrix_norm(const int n, const double* matrix);

void vectors_task(const int n, const int num_threads);

void matrices_task(const int n, const int num_threads);