#include "LinearSystem.h"

int main()
{
    LinearSystem ls("input.txt");
    ls.print_ls();
    ls.solve_DUx_y();
    ls.print_ls();
}
