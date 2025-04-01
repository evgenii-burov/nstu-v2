#include "functions.h"

int main()
{
    FunctionMinimization f_m("parameters.txt");
    std::cout << f_m.dichotomyMethod();
    std::cout << "\n";
    std::cout << f_m.goldenRatioMethod();
    std::cout << "\n";
    std::cout << f_m.fibonacciMethod();

    f_m.intervalSearch(-1000, 1);

    std::cout << "\n";
    
}
