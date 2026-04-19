#include "minimization_algorithms.h"

using namespace std;

int main() {
    cout << fixed << setprecision(6);
    QuadraticFunction quadratic;
    double eps_f = 1e-3;
    double eps_x = 1e-3;
    HookJeeves hj(quadratic, eps_f, eps_x);
    GradientDescent gd(quadratic, eps_f, eps_x);
    gd.minimize(Point(5, 1), true);
}