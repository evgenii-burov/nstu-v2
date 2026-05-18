from OneDimensionalMinimization import *

q = Quadratic1DFunction()
gr = GoldenRatio(q, -10,10,1e-7)

print(gr())