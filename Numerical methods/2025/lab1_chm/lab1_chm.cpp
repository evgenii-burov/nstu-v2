#include "LinearSystem.h"

int main()
{
    LinearSystem ls("input.txt");
    ls.print_ls();
    std::cout << "\n";

    ls.print_ls();
}
