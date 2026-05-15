with open('dt.txt', 'r') as input_file:
    integral = 0
    text = input_file.read()
    lines = text.split('\n')
    print(len(lines))
    y0 = lines[0:100]
    dy = lines[100:200]
    for i in range(100):
        values_y0 = y0[i].split(' ')
        values_dy = dy[i].split(' ')
        integral += abs(float(values_dy[2])-float(values_y0[2]))/(float(values_dy[1])-float(values_y0[1]))

    print(integral/100)