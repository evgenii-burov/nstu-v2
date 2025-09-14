#include "du_class.h"

int main()
{
    EulerFirst du(0.1, 0, 1, 1);
    du.solve_du("output.txt");
}
