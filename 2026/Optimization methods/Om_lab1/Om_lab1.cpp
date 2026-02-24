#include "function_minimization.h"

int main()
{
    FunctionMinimization fm;
    double eps = 1e-6;
    fm.find_interval_containing_minimum(); 
    fm.dichotomy_method(eps);
    fm.golden_ratio_method(eps);
}
