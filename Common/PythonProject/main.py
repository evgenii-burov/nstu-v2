def makeLowerTriangle(n: int):
    matrix = []
    for _ in range(n):
        matrix.append([])
    for i in range(n):
        for j in range(n):
            if j>i:
                matrix[i].append('0')
            else:
                if i==j:
                    matrix[i].append('1')
                else:
                    matrix[i].append(f'L{i+1}{j+1}')
    return matrix


def makeUpperTriangle(n: int):
    matrix = []
    for _ in range(n):
        matrix.append([])
    for i in range(n):
        for j in range(n):
            if j<i:
                matrix[i].append('0')
            else:
                if i==j:
                    matrix[i].append('1')
                else:
                    matrix[i].append(f'U{i+1}{j+1}')
    return matrix


def makeDiagonal(n):
    matrix = []
    for i in range(n):
        matrix.append([])
        for j in range(n):
            if j != i:
                matrix[i].append('0')
            else:
                matrix[i].append(f'D{i + 1}{j + 1}')
    return matrix


def multiplyStrs(s1,s2):
    if s1 == '0' or s2 =='0':
        return '0'
    elif s1=='1':
        return s2
    elif s2=='1':
        return s1
    else:
        return f'{s1}{s2}'


def sumStrs(strs):
    result=''
    for s in strs:
        if s != '0':
            result=result+f'+{s}'
    result = result.removeprefix('+')
    if not result:
        return '0'
    return result


def multiplyMatrices(m1,m2):
    result=[]
    n=len(m1)
    for i in range(n):
        result.append([])
        for j in range(n):
            strs=[]
            for k in range(n):
                strs.append(multiplyStrs(m1[i][k], m2[k][j]))
            result[i].append(sumStrs(strs))
    return result


def printMatrix(matrix):
    n=len(matrix)
    for i in range(n):
        for j in range(n):
            print(matrix[i][j], end='\t')
        print()
    print()


n=4
matrix_L = makeLowerTriangle(n)
matrix_U = makeUpperTriangle(n)
matrix_D = makeDiagonal(n)
printMatrix(matrix_U)
printMatrix(matrix_L)
printMatrix(matrix_D)
printMatrix(multiplyMatrices(matrix_L, matrix_D))
printMatrix(multiplyMatrices(multiplyMatrices(matrix_L,matrix_D),matrix_U))
