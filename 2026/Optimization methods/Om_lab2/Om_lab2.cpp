#include "minimization_algorithms.h"

using namespace std;

int main() {
    cout << fixed << setprecision(7);
    QuadraticFunction quadratic;
    Rosenbrock rosenbrock;
    Variant4 v;
    double eps = 1e-3;
    double eps_f = eps;
    double eps_x = eps;
    HookJeeves hj(v, eps_f, eps_x);
    GradientDescent gd(v, eps_f, eps_x);
    gd.minimize(Point(2.5, 10), true);
}