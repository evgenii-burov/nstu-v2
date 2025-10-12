#include "du_class.h"

int main()
{
    EulerSimpleImplicit du1("input.txt");
    du1.solve_du("euler_first.txt");

    EulerSecondImplicit du2("input.txt");
    du2.solve_du("euler_second.txt");

    EulerThirdImplicit du3("input.txt");
    du3.solve_du("euler_third.txt");

}