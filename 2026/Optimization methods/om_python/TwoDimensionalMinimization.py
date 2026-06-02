import math
from OneDimensionalMinimization import *
import numpy as np
import matplotlib.pyplot as plt


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
        self.current_point = current_point
        self.direction = direction
    def __call__(self, x):
        return self.f(self.current_point + x*self.direction)


class Variant4Function(TwoDFunction):
    def __call__(self, p:Point):
        A1 = 2
        A2 = 1
        a1 = 1
        a2 = 3
        b1 = 2
        b2 = 3
        c1 = 2
        c2 = 1
        d1 = 1
        d2 = 3

        term1 = 1.0 + pow((p.x - a1) / b1, 2) + pow((p.y - c1) / d1, 2)
        term2 = 1.0 + pow((p.x - a2) / b2, 2) + pow((p.y - c2) / d2, 2)
        return (A1 / term1) + (A2 / term2)

    def gradient(self, p:Point) ->Point:
        A1 = 2
        A2 = 1
        a1 = 1
        a2 = 3
        b1 = 2
        b2 = 3
        c1 = 2
        c2 = 1
        d1 = 1
        d2 = 3
        t1 = 1.0 + pow((p.x - a1) / b1, 2) + pow((p.y - c1) / d1, 2)
        t2 = 1.0 + pow((p.x - a2) / b2, 2) + pow((p.y - c2) / d2, 2)

        dfdx = -A1 * (2.0 * (p.x - a1) / (b1 * b1)) / (t1 * t1) - A2 * (2.0 * (p.x - a2) / (b2 * b2)) / (t2 * t2)

        dfdy = -A1 * (2.0 * (p.y - c1) / (d1 * d1)) / (t1 * t1) - A2 * (2.0 * (p.y - c2) / (d2 * d2)) / (t2 * t2)

        return Point(-dfdx, -dfdy)

    def __str__(self):
        return 'Variant4'


class QuadraticFunction(TwoDFunction):
    def __call__(self, p:Point):
        return 100*(p.y-p.x)**2+(1-p.x)**2
    
    def gradient(self, p:Point):
        return Point(200*(p.x-p.y)+2*(p.x-1),200*(p.y-p.x))

    def __str__(self):
        return 'Quadratic'


class RosenbrockFunction(TwoDFunction):
    def __call__(self, p:Point)->float:
        return 100 * (p.y - p.x**2) ** 2 + (1 - p.x) ** 2

    def gradient(self, p:Point) ->Point:
        return Point(-400.0 * p.x * (p.y - p.x * p.x) - 2.0 * (1.0 - p.x),200.0 * (p.y - p.x * p.x))

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

    def __call__(self, output_mode:int)->Point:
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

        if output_mode == 3:
            points = [self.start]

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
            gr = GoldenRatio(function_section, a, b, 1e-8)
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

            if output_mode == 3:
                points.append(current_point)

            if lambd < self.eps_x or self.iterations > 10000:
            #if gradient.vector_norm() < self.eps_f or self.iterations > 10000:
                break

        method_output = (f'\n{'*'*20}\n{self.start}\t'
                         f'{-math.log10(self.eps_x)}\t'
                         f'{-math.log10(self.eps_f)}\t'
                         f'{self.iterations}\t'
                         f'{self.function_evaluations}\t'
                         f'{current_point.x:.6e}\t'
                         f'{current_point.y:.6e}\t'
                         f'{self.f(current_point):.6e}\n')

        if output_mode > 0:
            output.write(method_output)
            if output_mode == 3:
                boundary = max(abs(self.start.x), abs(self.start.y))
                plot_descent_path(self.f, points, (-boundary, boundary), (-boundary, boundary), 10)

        return current_point


class HookJeeves(TwoDimensionalMinimization):
    def __init__(self, f, start, eps_x, eps_f):
        super().__init__(f, start, eps_x, eps_f)

    def __str__(self):
        return 'Hook-Jeeves'

    def _exploratory_move(self, base_point, step_x, step_y):
        """
        Perform exploratory moves around base_point along each axis.
        Returns the best point found (may equal base_point if no improvement).
        """
        current = base_point
        current_f = self.function(current)

        for delta in [Point(step_x, 0), Point(-step_x, 0)]:
            trial = current + delta
            trial_f = self.function(trial)
            if trial_f < current_f:
                current = trial
                current_f = trial_f
                break

        for delta in [Point(0, step_y), Point(0, -step_y)]:
            trial = current + delta
            trial_f = self.function(trial)
            if trial_f < current_f:
                current = trial
                current_f = trial_f
                break

        return current

    def __call__(self, output_mode: int):
        self.function_evaluations = 0
        self.iterations = 0

        if output_mode > 0:
            output = open(
                f'{self} {self.f} ({self.start.x} {self.start.y}) {-math.log10(self.eps_x)} {-math.log10(self.eps_f)}.txt',
                'w')

        if output_mode == 3:
            points = [self.start]

        base_point = self.start
        step_x = 1.0
        step_y = 1.0

        while True:
            self.iterations += 1

            # --- Exploratory phase ---
            explored = self._exploratory_move(base_point, step_x, step_y)

            # If no improvement, halve both steps and retry from base
            if explored == base_point:
                step_x /= 2
                step_y /= 2
                if step_x < self.eps_x and step_y < self.eps_x:
                    break
                if self.iterations > 10000:
                    break
                continue

            # --- Pattern move phase ---
            # Direction from base to explored point
            diff = explored + -1 * base_point
            length = math.sqrt(diff.x ** 2 + diff.y ** 2)

            if length < 1e-15:
                # Degenerate direction; just accept the exploratory point
                current_point = explored
            else:
                direction = diff * (1.0 / length)

                # Find bracket [a, b] for line search along direction
                function_section = TwoDFunctionSection(self.f, explored, direction)

                previous_x = 0.0
                current_x = 0.0
                step = 1.0
                next_x = step

                current_f_ls = function_section(current_x)
                next_f_ls = function_section(next_x)
                self.function_evaluations += 2

                while next_f_ls < current_f_ls:
                    previous_x = current_x
                    current_x = next_x
                    current_f_ls = next_f_ls
                    step *= 2
                    next_x = current_x + step
                    next_f_ls = function_section(next_x)
                    self.function_evaluations += 1

                a = previous_x
                b = next_x

                gr = GoldenRatio(function_section, a, b, 1e-8)
                self.function_evaluations += gr.function_evaluations
                lambd = gr()

                current_point = explored + direction * lambd

            # --- Convergence check ---
            change_from_base = current_point + -1 * base_point
            l = math.sqrt(change_from_base.x ** 2 + change_from_base.y ** 2)

            df = abs(self.function(base_point) - self.function(current_point))
            current_point_f = self.f(current_point)

            iteration_output = (f'{current_point.x:.6e}\t'
                                f'{current_point.y:.6e}\t'
                                f'{current_point_f:.6e}\t'
                                f'{direction.x:.6e}\t'
                                f'{direction.y:.6e}\t'
                                f'{l:.6e}\t'
                                f'{abs(current_point.x - (current_point.x - change_from_base.x)):.6e}\t'
                                f'{abs(current_point.y - (current_point.y - change_from_base.y)):.6e}\t'
                                f'{df:.6e}\n'
                                )

            if output_mode > 1:
                output.write(iteration_output)
                if output_mode == 3:
                    points.append(current_point)

            # Advance base for next iteration
            base_point = current_point

            if l < self.eps_x or (df < self.eps_f and l < self.eps_x):
                break
            if self.iterations > 10000:
                break

        method_output = (f'\n{"*" * 20}\n{self.start}\t'
                         f'{-math.log10(self.eps_x)}\t'
                         f'{-math.log10(self.eps_f)}\t'
                         f'{self.iterations}\t'
                         f'{self.function_evaluations}\t'
                         f'{base_point.x:.6e}\t'
                         f'{base_point.y:.6e}\t'
                         f'{self.f(base_point):.6e}\n')

        if output_mode > 0:
            output.write(method_output)
            output.close()
            if output_mode == 3:
                boundary = max(abs(self.start.x), abs(self.start.y))
                plot_descent_path(self.f, points, (-boundary, boundary), (-boundary, boundary), 10)

        return base_point


def plot_descent_path(f, points, x_range, y_range, n_levels=20):
    """
    Draw contour map of f and descent path of points.

    Parameters:
    -----------
    f : callable
        Function f(x, y) taking two floats
    points : list of Point
        List of Point objects representing the descent path
    x_range, y_range : tuple
        Plot boundaries
    n_levels : int
        Number of contour levels
    """

    # Extract x, y coordinates from Point objects
    x_coords = [p.x for p in points]
    y_coords = [p.y for p in points]

    # Create grid
    x = np.linspace(x_range[0], x_range[1], 200)
    y = np.linspace(y_range[0], y_range[1], 200)
    X, Y = np.meshgrid(x, y)

    Z = np.zeros_like(X)
    for i in range(X.shape[0]):
        for j in range(X.shape[1]):
            Z[i, j] = f(Point(X[i, j], Y[i, j]))

    # Plot
    fig, ax = plt.subplots(figsize=(8, 6))

    # Level map
    ax.contour(X, Y, Z, levels=n_levels, cmap='viridis')

    # Path with lines connecting points
    ax.plot(x_coords, y_coords, 'r-o', markersize=4, linewidth=1.5)

    # Mark start and end
    ax.plot(x_coords[0], y_coords[0], 'go', markersize=8)
    ax.plot(x_coords[-1], y_coords[-1], 'ro', markersize=8)

    ax.set_xlim(x_range)
    ax.set_ylim(y_range)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_title('Descent Path')
    ax.grid(True, alpha=0.3)

    plt.show()
