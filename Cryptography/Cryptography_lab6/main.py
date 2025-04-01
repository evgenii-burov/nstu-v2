import random
import math
import time
from tkinter import *
from tkinter import messagebox


from pyexpat.errors import messages


def primalityTestTrue(prime_number: int)->bool:
    for i in range(2, int(math.sqrt(prime_number))+1):
        if prime_number % i == 0:
            return False
    return True


def binaryToDecimal(b:str)->int:
    decimal_result = 0
    for i in range(len(b) - 1, -1, -1):
        decimal_result += int(b[i])*2**(len(b)-1-i)
    return decimal_result


def decimalToBinary(d: int) ->str:
    result = ""
    while d > 0:
        result = str(d % 2) + result
        d = d // 2
    return result
    

def primalityTest(prime_number: int, target_probability: float) -> bool:
    probability_is_not_prime = 1
    max_checks = 100
    k=0
    while(probability_is_not_prime > target_probability and k < max_checks):
        number_to_check = random.randint(2, prime_number-1)
        a = number_to_check**((prime_number-1)//2)
        t=a % prime_number
        if(t!= 1 and t != prime_number-1):
            return False
        probability_is_not_prime=probability_is_not_prime/2
        k+=1
    print(f"k: {k}")
    return True

def start():
    start = time.time()
    #Input processing
    # with open("parameters.txt", "r") as input:
    #     for line in input:
    #         bits = int(line.split()[0])
    #         target_probability = float(line.split()[1])
    #         break
    bits = mode_en.get()
    try:
        if(float(bits) != int(bits)):
            messagebox.showinfo("Ошибка", "Разрядность числа должна быть натуральным числом")
            exit(0)
        if (int(bits) < 1):
            messagebox.showinfo("Ошибка", "Разрядность числа должна быть натуральным числом")
            exit(0)
    except ValueError:
        messagebox.showinfo("Ошибка", "Разрядность числа должна быть натуральным числом")
        exit(0)
    target_probability = distr_en.get()
    try:
        if (float(target_probability) <= 0  or float(target_probability) >= 1):
            messagebox.showinfo("Ошибка", "Вероятность должна быть числом из открытого промежутка (0, 1)")
            exit(0)
    except ValueError:
        messagebox.showinfo("Ошибка", "Вероятность должна быть числом из открытого промежутка (0, 1)")
        exit(0)
    bits = int(bits)
    target_probability = float(target_probability)
    #Generate a random number
    iterations = 0
    while True:
        prime_number_binary = "1"
        for i in range(bits-2):
            prime_number_binary += str(random.randint(0,1))
        prime_number_binary += "1"
        prime_number = binaryToDecimal(prime_number_binary)
        iterations+=1
        if(primalityTest(prime_number, target_probability)):
            m1 = f"Сгенерировано число {prime_number}, которое может быть составным с вероятностью не более {target_probability}"
            m2 = f"В двоичном представлении: {prime_number_binary}"
            break
    end = time.time()
    m3 = f"Время работы программы в секундах: {end-start}"
    m4 = f"Итераций главного цикла: {iterations}"
    m5 = f"Является ли число на самом деле простым: {primalityTestTrue(prime_number)}"
    messagebox.showinfo("Число сгенерировано", f"{m1}\n{m2}\n{m3}\n{m4}\n{m5}")


if __name__ == "__main__":
    window = Tk()
    window.title('Лабораторная работа №5')
    window.geometry('800x300+400+200')
    frame = Frame(
        window,
        padx=10,
        pady=10
    )
    frame.pack(expand=True)

    mode_lb = Label(frame, text="Разрядность простого числа в двоичной системе: ")
    mode_lb.grid(row=4, column=1)

    distr_lb = Label(frame,
                     text="Наибольшая допустимая вероятность того, что сгенерированное число окажется составным: ")
    distr_lb.grid(row=8, column=1)

    mode_en = Entry(frame)
    mode_en.grid(row=4, column=2, pady=5)

    distr_en = Entry(frame)
    distr_en.grid(row=8, column=2, pady=5)

    cal_btn = Button(frame, text='Запустить программу', command=start)
    cal_btn.grid(row=10, column=2)

    window.mainloop()

