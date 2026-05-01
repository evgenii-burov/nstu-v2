got_value=False
while(not got_value):
    axis_to_fix = input('Which axis to fix? (x or y):\t')
    if axis_to_fix != 'x' and axis_to_fix != 'y':
        print('Invalid input')
    else:
        got_value=True

got_value=False
while(not got_value):
    fixed_value = input('At what value to fix the axis? :\t')
    try:
        fixed_value = float(fixed_value)
    except ValueError:
        print('Invalid input')
        continue
    got_value=True

print(f'Fixing {axis_to_fix} at {fixed_value}')

eps = 1e-6

with open('input.txt', 'r') as input_file:
    file = input_file.read()
    lines = file.split('\n')

    if axis_to_fix == 'x':
        offset=0
    elif axis_to_fix == 'y':
        offset=1

    #Determine closest value
    under_eps = float('inf')
    over_eps = float('inf')
    under_value = 0
    over_value = 0
    for line in lines:
        values = line.split(' ')
        try:
            values = [float(x) for x in values]
        except ValueError:
            continue
        if values[offset] - fixed_value <= 0 and abs(values[offset] -fixed_value) < under_eps:
            under_value = values[offset]
            under_eps = abs(values[offset] -fixed_value)
        if values[offset] - fixed_value >= 0 and abs(values[offset] -fixed_value) < over_eps:
            over_value = values[offset]
            over_eps = abs(values[offset] -fixed_value)
        chosen_value = 0
    while(True):
        try:
            choices = [float(under_value), float(over_value)]
            chosen_value = int(input(f'{choices}:\t'))
            chosen_value = choices[chosen_value]
            print(f'Value chosen: {chosen_value}')
            break
        except ValueError:
            print('Invalid input')
            continue
        except IndexError:
            print('Invalid value number')
            continue
    #Process file
    with open('output.txt','w') as output_file:
        fixed_value = chosen_value
        lines_read = 0
        lines_matched = 0
        for line in lines:
            values = line.split(' ')
            if len(values)<3:
                break
            try:
                values = [float(x) for x in values]
            except ValueError:
                continue
            lines_read+=1
            if abs(fixed_value - values[offset]) < eps:
                lines_matched+=1
                output_file.write(f'{values[(offset+1)%2]}\t{values[2]}\n')
        print(f'Value:{fixed_value}\nLines read:{lines_read}\nLines matched:{lines_matched}')

    