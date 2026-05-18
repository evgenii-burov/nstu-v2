from OneDimensionalMinimization import *

class Point():
    def __init__(self, x:float, y:float):
        self.x=x
        self.y=y

    def __rmul__(self, scalar:float):
        return Point(scalar*self.x, scalar*self.y)

    def __mul__(self, scalar:float):
        return self.__rmul__(scalar)

    def __add__(self, other):
        return Point(self.x+other.x, self.y+other.y)
    
    def __str__(self):
        return f'({self.x}; {self.y})'

class TwoDFunciton():
    def __call__(self, p:Point):
        pass

    def gradient(self, p:Point)->Point:
        pass


class TwoDFunctionSection(OneDFunction):
    def __init__(self, f:TwoDFunciton, direction:Point):
        self.f = f
        self.direction = direction
    
    def __call__(self, x):
        return self.f(x*self.direction)


class RosenbrockFunction(TwoDFunciton):
    def __call__(self, p:Point):
        return p.x**2+p.y**2
    
    def gradient(self, p:Point):
        return Point(0, 0)


class TwoDimensionalMinimization():
    def __init__(self, f:TwoDFunciton, start:Point, eps_x:float, eps_f:float):
        self.function_evaluations = 0
        self.iterations = 0
        self.f = f
        self.start = start
        self.eps_x = eps_x
        self.eps_f = eps_f

    def __call__(self):
        pass


class GradientDescent(TwoDimensionalMinimization):
    def __init__(self, f, start, eps_x, eps_f):
        super().__init__(f, start, eps_x, eps_f)

    def __call__(self):
        current_point = self.start
        self.function_evaluations = 0
        self.iterations = 0
        current_f = self.f(current_point)

        while True:
            gradient = self.f.gradient(current_point)
            direction = -1 * gradient * (1/(gradient.x**2+gradient.y**2))

            function_section = TwoDFunctionSection(self.f, direction)
            
            gr = GoldenRatio(function_section, )
            if df < self.eps_f or dx < self.eps_x or self.iterations > 10000:
                break

        return current_point
