m = 30
a= 13

for i in range(2, 6):
    print(((a % 30)**i)%30)
    print(a**i % 30)
    print('-----------')
