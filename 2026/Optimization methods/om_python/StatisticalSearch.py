from TwoDimensionalMinimization import *
import random
'''
Метод простого случайного поиска

три алгоритма глобального поиска

Исследовать метод ПСП:
eps | P | N | Point(x*, y*) | f(x*, y*)

Исследовать алгоритмы поиска глобального экстремума
исследовать при различных значениях числа попыток m

f(x,y) = sum{i=1, i<=6}( Ci / ( 1 + (x-ai)**2 + (y-bi)**2 ) )
x, y from (-10, 10)
'''

class TargetFunction(TwoDFunction):
    def __call__(self, p:Point)->float:
        C = [4, 9, 1, 7, 5, 6]
        a = [7, -9, 6, -8, -10, -2]
        b = [9, -1, 5, -2, -8, -4]
        value = 0
        for i in range(6):
            value += C[i] / ( 1 + (p.x - a[i])**2 + (p.y - b[i])**2 )
        return value


class StatisticalMinimization:
    def __init__(self, f:TwoDFunction, xmin:float, xmax:float, ymin:float, ymax:float, eps:float):
        self.f = f
        self.xmin = xmin
        self.xmax = xmax
        self.ymin = ymin
        self.ymax = ymax
        self.eps = eps

    def __call__(self, n:int, seed: int):
        random.seed(seed)
        min_point = Point(self.xmin + (self.xmax - self.xmin)*random.random(),
                          self.ymin + (self.ymax - self.ymin)*random.random())
        min_function_value = self.f(min_point)

        for i in range(n - 1):
            new_point = Point(self.xmin + (self.xmax - self.xmin)*random.random(),
                          self.ymin + (self.ymax - self.ymin)*random.random())
            new_function_value = self.f(new_point)
            if new_function_value < min_function_value:
                min_point = new_point
                min_function_value = new_function_value

        print(f'{min_point}\t{min_function_value}')


    def calculateP(self, n:int, xl:float, yl: float, eps:float):
        p_hits = (eps*eps) / (xl*yl)
        return 1 - (1 - p_hits)**n

f = TargetFunction()

sm = StatisticalMinimization(f, -10, 10, -10, 10, 1e-7)
sm(10, 9999)