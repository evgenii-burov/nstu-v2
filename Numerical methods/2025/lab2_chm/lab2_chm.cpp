#include "linear_system.h"

int main()
{
    LinearSystem ls("input.txt");
    ls.matrix_times_b();
}
