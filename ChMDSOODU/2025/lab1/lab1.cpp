#include "du_class.h"

int main()
{
    EulerSimple euler_simple("input.txt");
    EulerModified euler_modified("input.txt");
    EulerImproved euler_improved("input.txt");
    euler_simple.solve_du("euler_simple.txt");
    euler_modified.solve_du("euler_modified.txt");
    euler_improved.solve_du("euler_improved.txt");
}
