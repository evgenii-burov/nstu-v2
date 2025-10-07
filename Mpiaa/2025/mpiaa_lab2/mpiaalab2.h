#pragma once
#include <iostream>
#include <chrono>

double dot_product_sequential(const int n, const double* vector_a, const double* vector_b);

double dot_product_parallel(const int n, const double* vector_a, const double* vector_b);
