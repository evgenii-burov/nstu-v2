double dot_product_sequential(const int n, const double* vector_a, const double* vector_b)
{
	double result = 0;
	for (int i = 0; i < n; i++)
	{
		result += vector_a[i] * vector_b[i];
	}
	return result;
}

double dot_product_parallel(const int n, const double* vector_a, const double* vector_b)
{
	double result = 0;
#pragma omp parallel for reduction(+: result)
	for (int i = 0; i < n; i++)
	{
		result += vector_a[i] * vector_b[i];
	}
	return result;
}
