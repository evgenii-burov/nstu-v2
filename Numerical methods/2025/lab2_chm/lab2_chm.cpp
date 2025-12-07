#include "linear_system.h"

int main()
{
    LinearSystem ls("input.txt");
    ls.solve_jacobi();
}
