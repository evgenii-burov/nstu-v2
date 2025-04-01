import random
import math
import time
from tkinter import *
from tkinter import messagebox

from pandas.core.interchange.from_dataframe import primitive_column_to_ndarray
from pyexpat.errors import messages


def primalityTestTrue(prime_number: int) -> bool:
    for i in range(2, int(math.sqrt(prime_number)) + 1):
        if prime_number % i == 0:
            return False
    return True


def binaryToDecimal(b: str) -> int:
    decimal_result = 0
    for i in range(len(b) - 1, -1, -1):
        decimal_result += int(b[i]) * 2 ** (len(b) - 1 - i)
    return decimal_result


def decimalToBinary(d: int) -> str:
    result = ""
    while d > 0:
        result = str(d % 2) + result
        d = d // 2
    return result

def RabinMillerTest(n: int, k: int) -> bool:
    if n == 2 or n == 3:
        return True
    if n % 2 == 0:
        return False
    if n!=5 and n%5 == 0:
        return False
    if n!=7 and n%7 == 0:
        return False
    r, s = 0, n - 1
    while s % 2 == 0:
        r += 1
        s //= 2
    for _ in range(k):
        a = random.randint(2, n - 1)
        x = pow(a, s, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True


def findPrimefactors(s, n):
    while (n % 2 == 0):
        s.append(2)
        n = n // 2
    for i in range(3, int(math.sqrt(n)), 2):
        while (n % i == 0):
            s.append(i)
            n = n // i
    if (n > 2):
        s.append(n)


def firstKPrimitiveRoots(n:int, k:int)->list:
    phi=n-1
    prime_factors_of_phi=[]
    findPrimefactors(prime_factors_of_phi, phi)
    primitive_roots = []
    counter=0
    for i in range(2,n):
        flag=False
        for elem in prime_factors_of_phi:
            to_check=pow(i,phi//elem, n)
            if(to_check==1):
                flag = True
                break
        if(flag==False):
            primitive_roots.append(i)
            counter+=1
            if(counter==k):
                break
    return primitive_roots


def start1():
    start = time.time()
    # Input processing
    # with open("parameters.txt", "r") as input:
    #     for line in input:
    #         bits = int(line.split()[0])
    #         target_probability = float(line.split()[1])
    #         break
    bits = mode_en.get()
    try:
        if (float(bits) != int(bits)):
            messagebox.showinfo("Ошибка", "Разрядность числа должна быть натуральным числом")
            exit(0)
        if (int(bits) < 1):
            messagebox.showinfo("Ошибка", "Разрядность числа должна быть натуральным числом")
            exit(0)
    except ValueError:
        messagebox.showinfo("Ошибка", "Разрядность числа должна быть натуральным числом")
        exit(0)
    num_tests = distr_en.get()
    try:
        if (float(bits) != int(bits)):
            messagebox.showinfo("Ошибка", "Количество тество должно быть натуральным числом")
            exit(0)
        if (int(bits) < 1):
            messagebox.showinfo("Ошибка", "Количество тество должно быть натуральным числом")
            exit(0)
    except ValueError:
        messagebox.showinfo("Ошибка", "Количество тество должно быть натуральным числом")
        exit(0)
    bits = int(bits)
    num_tests = int(num_tests)
    # Generate a random number
    iterations = 0
    while True:
        prime_number_binary = "1"
        for i in range(bits - 2):
            prime_number_binary += str(random.randint(0, 1))
        prime_number_binary += "1"
        prime_number = binaryToDecimal(prime_number_binary)
        iterations += 1
        if (RabinMillerTest(prime_number, num_tests)):
            m1 = f"Сгенерировано число {prime_number}"
            m2 = f"В двоичном представлении: {prime_number_binary}"
            break
    end = time.time()
    m3 = f"Время работы программы в секундах: {end - start}"
    m4 = f"Итераций главного цикла: {iterations}"
    #m5 = f"Является ли число на самом деле простым: {primalityTestTrue(prime_number)}"
    messagebox.showinfo("Число сгенерировано", f"{m1}\n{m2}\n{m3}\n{m4}")
    distr_en2.delete(0, len(distr_en2.get()))
    distr_en2.insert(0,str(prime_number))


def start2():
    start= time.time()
    a = int(mode_en1.get())
    b = int(distr_en1.get())
    primes_in_range=[]
    for i in range(a,b+1):
        if(RabinMillerTest(i,5)):
            primes_in_range.append(i)
    with open(r"primes.txt", "w") as file:
        for i in range(len(primes_in_range)):
            file.write(f"{primes_in_range[i]}\n")
    end = time.time()
    m1=f"\nВремя работы программы: {end - start}"
    messagebox.showinfo("Числа сгенерированы", f"Простые числа из диапазона записаны в файл{m1}")


def start3():
    start = time.time()
    k=int(mode_en2.get())
    n=int(distr_en2.get())
    print(n,k)
    roots=firstKPrimitiveRoots(n,k)
    with open(r"roots.txt", "w") as file:
        for entry in roots:
            file.write(f"{entry}\n")
    end= time.time()
    m1=f"\n\nВремя работы программы: {end-start}"
    messagebox.showinfo("Корни найдены", f"Первообразные корни числа записаны в файл{m1}")


def start4():
    bits = 40
    num_tests = 5
    nums_needed = 3
    while True:
        prime_numbers=[]
        for _ in range(nums_needed):
            while True:
                prime_number_binary = "1"
                for i in range(bits - 2):
                    prime_number_binary += str(random.randint(0, 1))
                prime_number_binary += "1"
                prime_number = binaryToDecimal(prime_number_binary)
                if (RabinMillerTest(prime_number, num_tests)):
                    prime_numbers.append(prime_number)
                    break
        prime_numbers=sorted(prime_numbers)
        if(prime_numbers[2]!=prime_numbers[1]):
            break
    mode_en3.delete(0, len(mode_en3.get()))
    mode_en3.insert(0, str(prime_numbers[0]))
    distr_en3.delete(0, len(distr_en3.get()))
    distr_en3.insert(0, str(prime_numbers[1]))
    distr_en4.delete(0, len(distr_en4.get()))
    distr_en4.insert(0, str(prime_numbers[2]))


def start5():
    x_a=int(mode_en3.get())
    x_b=int(distr_en3.get())
    n=int(distr_en4.get())
    g=firstKPrimitiveRoots(n,1)[0]
    y_a = pow(g, x_a, n)
    y_b = pow(g, x_b, n)
    k_a=pow(y_b,x_a,n)
    k_b = pow(y_a, x_b, n)
    m1=f"Открытое значение y_a:{y_a}\nОткрытое значение y_b:{y_b}\n"
    m2=f"Ключ абонента a:{k_a}\nКлюч абонента b:{k_b}\n"
    messagebox.showinfo("Ключи сгенерированы", f"Произведён обмен\n{m1}{m2}")


if __name__ == "__main__":
    window = Tk()
    window.title('Лабораторная работа №6')
    window.geometry('800x600+400+200')
    frame = Frame(
        window,
        padx=10,
        pady=10
    )
    frame.pack(expand=True)
    # prime number generation
    mode_lb = Label(frame, text="Разрядность простого числа в двоичной системе: ")
    mode_lb.grid(row=4, column=1)

    distr_lb = Label(frame,
                     text="Количество проверок на простоту методом Рабина Миллера:")
    distr_lb.grid(row=8, column=1)

    mode_en = Entry(frame)
    mode_en.grid(row=4, column=2, pady=5)

    distr_en = Entry(frame)
    distr_en.grid(row=8, column=2, pady=5)

    cal_btn = Button(frame, text='Получить простое число', command=start1)
    cal_btn.grid(row=10, column=2)
    #prime numbers from a range
    mode_lb1 = Label(frame, text="Начало диапазона:")
    mode_lb1.grid(row=4, column=4)

    distr_lb1 = Label(frame,
                     text="Конец диапазона:")
    distr_lb1.grid(row=8, column=4)

    mode_en1 = Entry(frame)
    mode_en1.grid(row=4, column=5, pady=5)

    distr_en1 = Entry(frame)
    distr_en1.grid(row=8, column=5, pady=5)

    cal_btn1 = Button(frame, text='Найти простые числа', command=start2)
    cal_btn1.grid(row=10, column=5)
    #First k primitive roots
    mode_lb2 = Label(frame, text="Сколько первых первообразных корней найти:")
    mode_lb2.grid(row=16, column=1)

    distr_lb2 = Label(frame,
                      text="Простое число:")
    distr_lb2.grid(row=15, column=1)

    mode_en2 = Entry(frame)
    mode_en2.grid(row=16, column=2, pady=5)

    distr_en2 = Entry(frame)
    distr_en2.grid(row=15, column=2, pady=5)

    cal_btn2 = Button(frame, text='Найти корни', command=start3)
    cal_btn2.grid(row=17, column=2)
    #Diffie-Hellman key exchange
    mode_lb3 = Label(frame, text="X_A:")
    mode_lb3.grid(row=15, column=4)

    distr_lb3 = Label(frame,
                      text="X_B:")
    distr_lb3.grid(row=16, column=4)

    distr_lb4 = Label(frame,
                      text="n:")
    distr_lb4.grid(row=17, column=4)

    mode_en3 = Entry(frame)
    mode_en3.grid(row=15, column=5, pady=5)

    distr_en3 = Entry(frame)
    distr_en3.grid(row=16, column=5, pady=5)

    distr_en4 = Entry(frame)
    distr_en4.grid(row=17, column=5, pady=5)

    cal_btn3 = Button(frame, text='Подставить простые числа', command=start4)
    cal_btn3.grid(row=18, column=5)

    cal_btn4 = Button(frame, text='Провести обмен', command=start5)
    cal_btn4.grid(row=19, column=5)
    window.mainloop()

