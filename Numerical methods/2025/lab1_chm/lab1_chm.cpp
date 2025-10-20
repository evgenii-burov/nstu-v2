#include "LinearSystem.h"

int main()
{
    LinearSystem ls("input.txt");
    ls.print_ls();
    std::cout << "\n";
    ls.decompose_ldu();
    ls.print_ls();
}
