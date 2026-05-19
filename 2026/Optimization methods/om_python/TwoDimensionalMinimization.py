import math

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
        return f'({self.x:.6e}; {self.y:.6e})'

    def vector_norm(self):
        return math.sqrt(self.x**2 + self.y**2)

class TwoDFunction():
    def __call__(self, p:Point)->float:
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

    def __str__(self):
        return 'Rosenbrock'

def append_line_to_file(file_name:str, values_to_write: tuple)->None:
    with open(file_name, 'a') as out:
        line_to_write = ''
        for value in values_to_write:
            line_to_write=line_to_write+str(value)+'\t'
        line_to_write = line_to_write + '\n'
        out.write(line_to_write)


class TwoDimensionalMinimization():
    def __init__(self, f:TwoDFunction, start:Point, eps_x:float, eps_f:float):
        self.function_evaluations = 0
        self.iterations = 0
        self.f = f
        self.start = start
        self.eps_x = eps_x
        self.eps_f = eps_f

    def function(self, p:Point):
        self.function_evaluations+=1
        return self.f(p)

    def __call__(self, output_mode:int)->float:
        """
        :param output_mode: 0: no output; 1: result out to file; 2: result and iterations out to file
        :return:
        """
        pass


class GradientDescent(TwoDimensionalMinimization):
    def __init__(self, f, start, eps_x, eps_f):
        super().__init__(f, start, eps_x, eps_f)

    def __str__(self):
        return 'Gradient descent'

    def __call__(self, output_mode:int):
        current_point = self.start
        self.function_evaluations = 0
        self.iterations = 0
        current_point_f = self.function(current_point)

        if output_mode > 0:
            output = open(f'{self} {self.f} ({self.start.x} {self.start.y}) {-math.log10(self.eps_x)} {-math.log10(self.eps_f)}.txt', 'w')

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
            self.function_evaluations+=2

            while next_f < current_f:
                previous_x = current_x
                current_x = next_x
                current_f = next_f
                step*=2
                next_x = current_x + step
                next_f = function_section(next_x)
                self.function_evaluations += 1

            a = previous_x
            b = next_x
            gr = GoldenRatio(function_section, a, b, self.eps_x)
            self.function_evaluations+=gr.function_evaluations
            # shouldn't be negative
            lambd = gr()
            shift = lambd * direction
            current_point += lambd * direction

            df = abs(current_point_f - self.function(current_point))
            current_point_f = self.f(current_point)

            iteration_output = (f'{current_point}\t'
                                f'{current_point_f:.6e}\t'
                                f'{direction}\t'
                                f'{lambd:.6e}\t'
                                f'{abs(current_point.x-shift.x):.6e}\t'
                                f'{abs(current_point.y-shift.y):.6e}\t'
                                f'{df:.6e}\t'
                                f'{gradient}\n')
            if output_mode > 1:
                output.write(iteration_output)

            if df < self.eps_f or lambd < self.eps_x or self.iterations > 10000:
            #if gradient.vector_norm() < self.eps_f or self.iterations > 10000:
                break

        method_output = (f'\n{'*'*20}\n{self.start}\t'
                         f'{-math.log10(self.eps_x)}\t'
                         f'{-math.log10(self.eps_f)}\t'
                         f'{self.iterations}\t'
                         f'{self.function_evaluations}\t'
                         f'{current_point}\t'
                         f'{self.f(current_point):.6e}\n')

        if output_mode > 0:
            output.write(method_output)

        return current_point


class HookJeeves(TwoDimensionalMinimization):
    def __init__(self, f, start, eps_x, eps_f):
        super().__init__(f, start, eps_x, eps_f)

    def __str__(self):
        return 'Hook-Jeeves'

    def __call__(self, output_mode: int):
        current_point = self.start
        self.function_evaluations = 0
        self.iterations = 0
        current_point_f = self.function(current_point)

        if output_mode > 0:
            output = open(
                f'{self} {self.f} ({self.start.x} {self.start.y}) {-math.log10(self.eps_x)} {-math.log10(self.eps_f)}.txt',
                'w')

        while True:
            self.iterations += 1
            # finding a direction
            step = 1
            moved = False

            point_before_trial = current_point
            while True:
                # probing on x
                trial_point = current_point + Point(step, 0)
                trial_point_f = self.function(trial_point)
                if trial_point_f < current_point_f:
                    moved = True
                    current_point = trial_point
                    current_point_f = trial_point_f
                else:
                    trial_point = current_point + Point(-step, 0)
                    trial_point_f = self.function(trial_point)
                    if trial_point_f < current_point_f:
                        moved = True
                        current_point = trial_point
                        current_point_f = trial_point_f
                # probing on y
                trial_point = current_point + Point(0, step)
                trial_point_f = self.function(trial_point)
                if trial_point_f < current_point_f:
                    moved = True
                    current_point = trial_point
                    current_point_f = trial_point_f
                else:
                    trial_point = current_point + Point(0, -step)
                    trial_point_f = self.function(trial_point)
                    if trial_point_f < current_point_f:
                        moved = True
                        current_point = trial_point
                        current_point_f = trial_point_f
                if moved or step < self.eps_x:
                    break
                else:
                    step/=2

            if step < self.eps_x:
                break

            direction = current_point + -1*point_before_trial

            function_section = TwoDFunctionSection(self.f, current_point, direction)

            # finding a and b for golden ratio (a:=prev b:=next)
            previous_x = 0
            current_x = 0
            step = 1
            next_x = step
            current_f = function_section(current_x)
            next_f = function_section(next_x)
            self.function_evaluations += 2

            while next_f < current_f:
                previous_x = current_x
                current_x = next_x
                current_f = next_f
                step *= 2
                next_x = current_x + step
                next_f = function_section(next_x)
                self.function_evaluations += 1

            a = previous_x
            b = next_x
            gr = GoldenRatio(function_section, a, b, self.eps_x)
            self.function_evaluations += gr.function_evaluations
            # shouldn't be negative
            lambd = gr()
            shift = lambd * direction
            current_point += lambd * direction

            df = abs(current_point_f - self.function(current_point))
            current_point_f = self.f(current_point)

            iteration_output = (f'{current_point}\t'
                                f'{current_point_f:.6e}\t'
                                f'{direction}\t'
                                f'{lambd:.6e}\t'
                                f'{abs(current_point.x - shift.x):.6e}\t'
                                f'{abs(current_point.y - shift.y):.6e}\t'
                                f'{df:.6e}\n'
                                )
            if output_mode > 1:
                output.write(iteration_output)

            if df < self.eps_f or lambd < self.eps_x or self.iterations > 10000:
                break

        method_output = (f'\n{'*' * 20}\n{self.start}\t'
                         f'{-math.log10(self.eps_x)}\t'
                         f'{-math.log10(self.eps_f)}\t'
                         f'{self.iterations}\t'
                         f'{self.function_evaluations}\t'
                         f'{current_point}\t'
                         f'{self.f(current_point):.6e}\n')

        if output_mode > 0:
            output.write(method_output)

        return current_point

rb = RosenbrockFunction()
g = GradientDescent(rb,Point(-7,5),1e-7,1e-7)
h = HookJeeves(rb,Point(-7,5),1e-7,1e-7)
print(h(2))