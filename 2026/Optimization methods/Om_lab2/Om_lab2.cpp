#include "minimization_algorithms.h"

using namespace std;

int main() {
    cout << fixed << setprecision(6);
    QuadraticFunction quadratic;
    double eps_f = 1e-7;
    double eps_x = 1e-7;
    HookJeeves hj(quadratic, eps_f, eps_x);
    GradientDescent gd(quadratic, eps_f, eps_x);
    gd.minimize(Point(5, 1), true);
}