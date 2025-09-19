#include "du_class.h"

int main()
{
    EulerImplicit du("input.txt");
    du.solve_du("euler_implicit.txt");
}