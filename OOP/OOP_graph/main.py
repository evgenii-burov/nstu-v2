import math

import matplotlib.pyplot as plt
import numpy as np

input_values = open(r"C:\Users\evgen\Documents\НГТУ\ООП\OOPlab1\sample.txt")
entries=[]
for line in input_values:
    entries=line.removesuffix(' ').split(sep=' ')
input_values.close()
for i in range(len(entries)):
    entries[i]=float(entries[i])

input_values = open(r"C:\Users\evgen\Documents\НГТУ\ООП\OOPlab1\uniform.txt")
entries_uniform=[]
for line in input_values:
    entries_uniform=line.removesuffix(' ').split(sep=' ')
for i in range(len(entries_uniform)):
    entries_uniform[i] = float(entries_uniform[i])
input_values.close()

input_values = open(r"C:\Users\evgen\Documents\НГТУ\ООП\OOPlab1\empiric_sample.txt")
entries_uniform=[]
for line in input_values:
    entries_uniform=line.removesuffix(' ').split(sep=' ')
for i in range(len(entries_uniform)):
    entries_uniform[i] = float(entries_uniform[i])
input_values.close()

print(entries_uniform)
plt.hist(entries, color = 'blue', edgecolor = 'black',
         bins = math.floor(math.log2(len(entries)))+1)

# Add labels
plt.title('Histogram')
plt.xlabel('x')
plt.ylabel('n')
plt.show()

# x = np.arange(0, 5, 0.1)
# y = np.sin(x)
# plt.plot(x, y)
# plt.show()