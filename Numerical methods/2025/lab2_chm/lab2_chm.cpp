#include "linear_system.h"

int main()
{
    LinearSystem ls("input.txt");
    ls.solve_gauss_seidel(0.1);
}
