#include "LinearSystem.h"

int main()
{
    int k = 10;
    LinearSystem ls("input.txt", 2, k);
    std::cout << "----k=" << k << "------\n";
    ls.print_ls();
    ls.solve_LDU();
    ls.compare_x();
}
