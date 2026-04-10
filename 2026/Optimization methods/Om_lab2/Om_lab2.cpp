#include "minimization_algorithms.h"

using namespace std;

int main() {
    cout << fixed << setprecision(6);
    QuadraticFunction quadratic;
    double eps_f = 1e-3;
    double eps_x = 1e-3;
    HookJeeves hj(quadratic, eps_f, eps_x);
    hj.minimize(Point(5, 1), true);
}