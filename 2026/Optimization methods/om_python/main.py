from OneDimensionalMinimization import *
from TwoDimensionalMinimization import *

q = QuadraticFunction()
r = RosenbrockFunction()
v = Variant4Function()

eps = 1e-7
start = Point(-10, 10)

hj = HookJeeves(v, start, eps, eps)

if __name__ == '__main__':
    hj(3)