import matplotlib.pyplot as plt
import plotly.express as px
import numpy as np
import math
import pandas as pd


n=1000
x = np.linspace(-1,1,n)

y = []
x_i=0
a=90
for i in range(n):
    x_i=x[i]
    y.append( 1/(1 + math.exp(-1*a*x_i))  )

plt.plot(x,y)
plt.show()