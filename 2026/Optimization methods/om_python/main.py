from OneDimensionalMinimization import *
from TwoDimensionalMinimization import *
from PenaltyFunctionMethod import *
from pathlib import Path

q = QuadraticFunction()
r = RosenbrockFunction()
v = Variant4Function()




if __name__ == '__main__':
    project_dir = Path('.')

    for txt_file in project_dir.glob('*.txt'):
        txt_file.unlink()

    #lab 2
    eps_list = [1e-3, 1e-4, 1e-5, 1e-6, 1e-7]
    # eps_list = [1e-3]
    start = Point(-4, -7)
    for eps in eps_list:
        hj = HookJeeves(q, start, eps, eps)
        # gd = GradientDescent(q, start, eps, eps)
        hj(2)
        # gd(2)

    #lab 3
    # start = Point(5,10)
    # p = PenaltyExp(1)
    # c = Coefficient(.1)
    # function = TargetFunction(p, c)
    # hj = HookJeeves(function, start, 1e-8, 1e-8)
    # pm = PenaltyMinimization(hj, function, 1e-7)
    # pm(1)