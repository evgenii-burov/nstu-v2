import matplotlib as mpl
import numpy as np

with open('input.txt', 'r') as input_file:
    file = input_file.read()
    lines = file.split('\n')

    x = []