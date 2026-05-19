import math

class OneDFunction():
    def __call__(self, x: float)->float:
        pass
        

class Quadratic1DFunction(OneDFunction):
    def __call__(self, x: float):
        return (x-7)**2


class GoldenRatio():
    def __init__(self, f:OneDFunction, a:float, b:float, eps: float):
        self.function_evaluations=0
        self.iterations=0
        self.f =f
        self.a = a
        self.b = b
        self.eps = eps
    
    def __call__(self)->float:
        a=self.a
        b=self.b
        x1 = a + (b-a)*(3-math.sqrt(5))/2
        x2 = a + (b-a)*(math.sqrt(5)-1)/2
        f1 = self.f(x1)
        f2 = self.f(x2)

        while b-a > self.eps:
            if self.f(x1) > self.f(x2):
                a = x1
                x1 = x2
                f1 = f2
                x2 = a + (b-a)*(math.sqrt(5)-1)/2
                f2 = self.f(x2)
            else:
                b = x2
                x2 = x1
                f2 = f1
                x1 = a + (b-a)*(3-math.sqrt(5))/2
                f1 = self.f(x1)
        
        return (b+a)/2
    
    
