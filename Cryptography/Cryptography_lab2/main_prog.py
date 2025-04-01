import math as m
from tkinter import *
from tkinter import messagebox


def ManageInput(mode_choice, p_distribution_choice) -> tuple:
    sequence = ""
    probabilities = []
    with open(f"probabilities{p_distribution_choice}.txt", "r") as probabilities_file:
        for line in probabilities_file:
            probabilities = line.split(sep=' ')
            break
        for i in range(len(probabilities)):
            probabilities[i] = float(probabilities[i])

    if mode_choice=='1':
        with open("sequence_to_encode.txt", "r") as sequence_file:
            for line in sequence_file:
                sequence = line
                break
    else:
        with open("sequence_to_decode.txt", "r") as sequence_file:
            for line in sequence_file:
                sequence = line
                break
    result = (sequence, probabilities)
    return result


def ManageOutput(sequence:str)->None:
    with open("resulting_sequence.txt", "w") as sequence_file:
        sequence_file.write(sequence)
    return None


def GetCodes(probabilities: list) -> list:
    codes = [""] * alphabet_len
    cumulative_probabilities = [0] * alphabet_len
    for i in range(1, len(probabilities)):
        cumulative_probabilities[i] += cumulative_probabilities[i - 1] + probabilities[i - 1]
    sigma = 0
    binary_places = 0
    for i in range(alphabet_len):
        sigma = cumulative_probabilities[i] + probabilities[i] / 2
        # print(f"Starting sigma: {sigma}", end=' ')
        binary_places = m.ceil(-m.log2(probabilities[i] / 2))
        # print(f"Binary places: {binary_places}", end=' ')
        for j in range(binary_places):
            codes[i] += str(int(sigma * 2))
            sigma *= 2
            sigma = sigma - int(sigma)
    return codes


def GetEncodedStr(sequence_to_encode: str, codes: list) -> str:
    encoded_str = ""
    for i in range(1, len(sequence_to_encode), 2):
        symbol = sequence_to_encode[i]
        encoded_str += str(codes[int(symbol) - 1])
    return encoded_str


def GetDecodedStr(sequence_to_decode: str, codes: list):
    decoded_str = ""
    substr_to_compare = ""
    for i in range(len(sequence_to_decode)):
        substr_to_compare += sequence_to_decode[i]
        for j in range(len(codes)):
            if substr_to_compare == codes[j]:
                decoded_str += alphabet[j]
                substr_to_compare = ""
                break
    if substr_to_compare != "":
        raise TypeError()
    return decoded_str


def GetValues(probabilities: list, codes: list) -> tuple:
    avg_code_len = 0
    redundancy = 0
    entropy = 0
    Kraft = 0
    for code in codes:
        avg_code_len += len(code)
    avg_code_len /= len(codes)
    for p in probabilities:
        entropy -= p * m.log2(p)
    print(entropy)
    redundancy = avg_code_len - entropy
    for code in codes:
        Kraft += 2 ** (-len(code))
    return avg_code_len, redundancy, Kraft


def Start():
    mode_choice = mode_en.get()
    if mode_choice not in ('1', '2'):
        messagebox.showinfo('Ошибка', 'Введёно неверное значение режима работы')
        exit(0)
    p_distribution_choice = distr_en.get()
    if p_distribution_choice not in ('1', '2', '3'):
        messagebox.showinfo('Ошибка', 'Введёно неверное значение используемого распределения')
        exit(0)
    try:
        sequence, probabilities = ManageInput(mode_choice, p_distribution_choice)
    except FileNotFoundError:
        messagebox.showinfo('Ошибка', 'Не удалось открыть все файлы с данными для работы')
        exit(0)
    probability_sum = 0.
    if len(probabilities) != alphabet_len:
        messagebox.showinfo('Ошибка', 'Количество вероятностей не соответствует количеству символов алфавита')
        exit(0)
    for p in probabilities:
        probability_sum += p
    if abs(probability_sum - 1.) > (eps := 1e-5):
        messagebox.showinfo('Ошибка', 'Сумма вероятностей распределения не равна 1')
        exit(0)
    if mode_choice == '1':
        if len(sequence) % 2 != 0:
            messagebox.showinfo('Ошибка', 'Строка, подлежащая кодированию, некорректна')
            exit(0)
        for i in range(0, len(sequence) - 1, 2):
            if sequence[i] != '1' or sequence[i + 1] < '1' or sequence[i + 1] > '7':
                messagebox.showinfo('Ошибка', 'Строка, подлежащая кодированию, некорректна')
                exit(0)
    codes = GetCodes(probabilities)
    try:
        resulting_sequence = GetEncodedStr(sequence, codes) if mode_choice == '1' else GetDecodedStr(sequence, codes)
    except TypeError:
        messagebox.showinfo('Ошибка', 'Строка, подлежащая декодированию, некорректна')
    message1 = ""
    for i in range(alphabet_len):
        message1 += f'{alphabet[i]}: {codes[i]}\n'
    if mode_choice == '1':
        message2 = "Строка закодирована!\n"
    else:
        message2 = "Строка декодирована!\n"
    values = GetValues(probabilities, codes)
    messagebox.showinfo('Результат', f'{message2}Кодовые слова:\n{message1}Длина полученной последовательности: {len(resulting_sequence)}\nСредняя длина кодового слова: {values[0]}\nИзбыточность: {values[1]}\nЗначение, сравниваемое с единицей по неравенству Крафта: {values[2]}')
    print(f"Codes: {codes}")
    print(f"Length of the resulting sequence: {len(resulting_sequence)}")
    print(f"Avg code length: {values[0]}")
    print(f"Redundancy: {values[1]}")
    print(f"Kraft value: {values[2]}")
    ManageOutput(resulting_sequence)


if __name__ == "__main__":
    alphabet = ["11", "12", "13", "14", "15", "16", "17"]
    alphabet_len = len(alphabet)
    window = Tk()
    window.title('Лабораторная работа №1')
    window.geometry('600x300+400+200')
    frame = Frame(
        window,
        padx=10,
        pady=10
    )
    frame.pack(expand=True)

    mode_lb = Label(frame, text="Режим работы ('1' - кодирование, '2' - декодирование): ")
    mode_lb.grid(row=4, column=1)

    distr_lb = Label(frame,
                 text="Распределение вероятностей для кодирования\n'1' - P_1(A), '2' - P_1(A), '3' - равномерное:  ")
    distr_lb.grid(row=8, column=1)

    mode_en = Entry(frame)
    mode_en.grid(row=4, column=2, pady=5)

    distr_en = Entry(frame)
    distr_en.grid(row=8, column=2, pady=5)

    cal_btn = Button(frame, text='Запустить программу', command=Start)
    cal_btn.grid(row=10, column=2)

    window.mainloop()


