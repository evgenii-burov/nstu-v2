def printMatrixDense(n):
    for i in range(n):
        for j in range(n):
            print(f'A{i+1}{j+1}\t', end='')
        print()
    print()


def printMatrixBanded(n):
    for i in range(n):
        for j in range(2*n-1):
            if i-j + 1 +n//2 < 1:
                print('0', end='\t')
            else:
                print(f'A{i +  1}{i-j + 3 +n//2}\t', end='')
        print()
    print()


printMatrixDense(5)
printMatrixBanded(5)