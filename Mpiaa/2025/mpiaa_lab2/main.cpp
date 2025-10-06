#include "lab2mpiaa.h"

int main()
{
    Computing c;
    double* vector_a = NULL,* vector_b = NULL;
    c.initialize_vector(vector_a);
    c.initialize_vector(vector_b);
    c.print_vector(vector_a);
    c.print_vector(vector_b);
    std::cout << "Sequential: " << c.dot_product_sequential(vector_a, vector_b);
    std::cout << "\nParallel: " << c.dot_product_parallel(vector_a, vector_b);
}
