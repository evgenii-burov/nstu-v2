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

class TwoDFunction():
    def __call__(self, p:Point):
        pass

    def gradient(self, p:Point)->Point:
        pass


class TwoDFunctionSection(OneDFunction):
    def __init__(self, f:TwoDFunction, current_point:Point, direction:Point):
        self.f = f
        self.direction = direction
        self.current_point = current_point
    def __call__(self, x):
        return self.f(self.current_point + x*self.direction)


class RosenbrockFunction(TwoDFunction):
    def __call__(self, p:Point):
        return 100*(p.y-p.x)**2+(1-p.x)**2
    
    def gradient(self, p:Point):
        return Point(200*(p.x-p.y)+2*(p.x-1),200*(p.y-p.x))


class TwoDimensionalMinimization():
    def __init__(self, f:TwoDFunction, start:Point, eps_x:float, eps_f:float):
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
        current_point_f = self.f(current_point)

        while True:
            self.iterations+=1
            gradient = self.f.gradient(current_point)
            direction = -1 * gradient * (1/math.sqrt(gradient.x**2+gradient.y**2))

            function_section = TwoDFunctionSection(self.f, current_point, direction)
            
            #finding a and b for golden ratio (a:=prev b:=next)
            previous_x = 0
            current_x = 0
            step = 1
            next_x = step
            current_f = function_section(current_x)
            next_f = function_section(next_x)

            while next_f < current_f:
                previous_x = current_x
                current_x = next_x
                current_f = next_f
                step*=2
                next_x = current_x + step

            a = previous_x
            b = next_x
            gr = GoldenRatio(function_section, a, b, self.eps_x)

            # shouldn't be negative
            dx = gr()
            current_point += dx * direction

            df = abs(current_point_f - self.f(current_point))   
            if df < self.eps_f or dx < self.eps_x or self.iterations > 10000:
                break

        return current_point


rb = RosenbrockFunction()
g = GradientDescent(rb,Point(-7,5),1e-3,1e-3)
print(g())