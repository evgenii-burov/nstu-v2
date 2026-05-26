from OneDimensionalMinimization import *
from TwoDimensionalMinimization import *
from pathlib import Path

q = QuadraticFunction()
r = RosenbrockFunction()
v = Variant4Function()

eps_list = [1e-3,1e-4,1e-5,1e-6,1e-7]
start = Point(5, 10)

if __name__ == '__main__':
    project_dir = Path('.')

    print(q(Point(-99, -99)))
    qq = TwoDFunctionSection(q,Point(-99,-99),Point(-.707,-.707))
    print(qq(1))

    for txt_file in project_dir.glob('*.txt'):
        txt_file.unlink()

    for eps in eps_list:
        hj = HookJeeves(q, start, eps, eps)
        hj(2)