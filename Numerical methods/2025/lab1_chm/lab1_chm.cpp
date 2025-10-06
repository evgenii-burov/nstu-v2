#include "LinearSystem.h"

int main()
{
    LinearSystem ls("input.txt");
    ls.print_ls();
    ls.solve_Ly_b();
    ls.print_ls();
}
