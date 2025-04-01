a = [1, 2, 4, 4, 4, 5, 5]

s = 0
for elem in a:
    s+= 2**(-elem)
print(s)