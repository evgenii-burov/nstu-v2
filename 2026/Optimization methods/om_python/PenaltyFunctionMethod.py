from TwoDimensionalMinimization import *

'''
f(x,y) = 2(x-y)**2 + 14(y-3)**2 -> min

a)  y-x>=1
b)  x=-y

penalty function | r0 | r change function | start point | eps | iter | f_eval | end point | f(end point)
'''

class PenaltyExp:
    def __init__(self, power: int):
        self.power = power

    def __call__(self, p:Point):
        return self.condition_expression(p)**self.power

    def condition_expression(self, p: Point) -> float:
        a = p.y - p.x - 1
        return (abs(a) - a)/2


class Coefficient:
    def __init__(self, r: float):
        self.r = r

    def __call__(self):
        return self.r

    def change_coefficient(self, factor: float):
        pass


class CoefficientMult(Coefficient):
    def change_coefficient(self, factor: float):
        self.r = self.r*factor


class TargetFunction(TwoDFunction):
    def __init__(self,
                 penalty:PenaltyExp,
                 penalty_power: int,
                 coefficient:Coefficient):
        self.penalty_power = penalty_power
        self.penalty = penalty()
        self.coefficient = coefficient

    def __call__(self, p:Point):
        return self.function_expression(p) + self.coefficient() * self.penalty(p)

    def function_expression(self, p:Point):
        return 2 * (p.x - p.y) ** 2 + 14 * (p.y - 3) ** 2


class PenaltyMinimization:
    def __init__(self,
                 minimize:TwoDimensionalMinimization,
                 function:TargetFunction,
                 penalty_power: int,
                 eps:float):
        self.minimize = minimize
        self.function = function
        self.penalty_power = penalty_power
        self.function.penalty
        self.eps = eps
        self.iterations = 0
        self.function_evaluations = 0

    def __call__(self, output_mode: int):
        #minimize
        #check if within bounds
        #if true, stop
        #else, change coefficient
        #penalty function | r0 | r change function | start point | eps | iter | f_eval | end point | f(end point)
        while True:
            self.iterations += 1
            resulting_point = self.minimize(0)
            self.function_evaluations += self.minimize.function_evaluations
            self.minimize.function_evaluations = 0
            print(self.function.penalty(resulting_point))
            if self.function.penalty(resulting_point) < self.eps or self.iterations > 100:
                if output_mode>0:
                    output = open("")
                break

            else:
                self.function.coefficient.change_coefficient(2)
                self.minimize.f = self.function