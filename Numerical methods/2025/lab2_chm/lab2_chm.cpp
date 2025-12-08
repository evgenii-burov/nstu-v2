#include "linear_system.h"

int main()
{
    LinearSystem ls("input.txt");
    ls.solve_gauss_seidel(1.6);
}
