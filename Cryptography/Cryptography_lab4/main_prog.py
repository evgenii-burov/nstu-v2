import math as m
from functools import partial
from tkinter import *
from tkinter import messagebox


def ManageInput(mode_choice: str) -> tuple:
    """
    :param mode_choice:
    :return: tuple (sequence, key_table)
    """
    sequence = ""
    key_table = []
    with open(f"key_table.txt", "r") as table_file:
        for line in table_file:
            key_table.extend(line.removesuffix('\n').split())

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
    result = (sequence, key_table)
    return result


def ManageOutput(sequence:str)->None:
    with open("resulting_sequence.txt", "w") as sequence_file:
        sequence_file.write(sequence)
    return None


def GetEncodedStr(sequence_to_encode: str, key_table: list) -> str:
    encoded_str = ""
    symbol_to_index = dict()
    for i in range(alphabet_len):
        symbol_to_index[alphabet[i]] = i
    for i in range(0, len(sequence_to_encode), 2):
        pair_index = symbol_to_index[sequence_to_encode[i]] + 27*symbol_to_index[sequence_to_encode[i+1]]
        encoded_str+=key_table[pair_index]
    return encoded_str


def GetDecodedStr(sequence_to_decode: str, key_table: list) -> str:
    decoded_str = ""
    for i in range(0, len(sequence_to_decode), 2):
        key_table_index = key_table.index(sequence_to_decode[i]+sequence_to_decode[i+1])
        decoded_str+=alphabet[key_table_index-27*(key_table_index//27)]+alphabet[key_table_index//27]
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
    redundancy = avg_code_len - entropy
    for code in codes:
        Kraft += 2 ** (-len(code))
    return avg_code_len, redundancy, Kraft


def Start() -> None:
    mode_choice = mode_en.get()
    if mode_choice not in ('1', '2'):
        messagebox.showinfo('Ошибка', 'Введёно неверное значение режима работы')
        exit(0)
    try:
        sequence, key_table = ManageInput(mode_choice)
    except FileNotFoundError:
        messagebox.showinfo('Ошибка', 'Не удалось открыть все файлы с данными для работы')
        exit(0)

    if len(sequence) % 2 != 0:
        sequence += '_'
    for i in range(0, len(sequence), 2):
        if sequence[i] < 'A' or sequence[i] > 'Z':
            if sequence[i] != '_':
                messagebox.showinfo('Ошибка', 'Строка, подлежащая кодированию/декодированию, некорректна')
                exit(0)
    #
    try:
        resulting_sequence = GetEncodedStr(sequence, key_table) if mode_choice == '1' else GetDecodedStr(sequence, key_table)
    except TypeError:
        messagebox.showinfo('Ошибка', 'Строка, подлежащая декодированию, некорректна')

    if mode_choice == '1':
        message1 = "Строка закодирована!\n"
    else:
        message1 = "Строка декодирована!\n"
    ManageOutput(resulting_sequence)

    sub_window = Toplevel()
    sub_window.title('Выполнено')
    sub_window.geometry('600x300+800+400')
    sub_frame = Frame(
        sub_window,
        padx=1,
        pady=1
    )
    sub_frame.pack(expand=True)

    msg_lb = Label(sub_frame, text=f'{message1}')
    msg_lb.grid(row=1, column=1)

    write_lb = Label(sub_frame, text="Записать ли получившуюся строку в соответствующий файл\nдля последующего кодирования/декодирования?")
    write_lb.grid(row=4, column=1)

    def yesButton(resulting_sequence: str, user_choice: str)->None:
        if user_choice== '1':
            with open("sequence_to_decode.txt", "w") as sequence_file:
                sequence_file.write(resulting_sequence)
        if user_choice == '2':
            with open("sequence_to_encode..txt", "w") as sequence_file:
                sequence_file.write(resulting_sequence)
        print("Записано")
        #exit(0)
        sub_window.destroy()
        return None


    yes_write_button = Button(sub_frame, text='Записать', command=partial(yesButton, resulting_sequence, mode_choice))
    yes_write_button.grid(row=20, column=2)

    def noButton()->None:
        print("Не записано")
        #exit(0)
        sub_window.destroy()
        return None

    no_write_button = Button(sub_frame, text='Не записывать', command=partial(noButton))
    no_write_button.grid(row=20, column=3)


if __name__ == "__main__":
    #THE_QUICK_BROWN_FOX_JUMPS_OVER_THE_LAZY_DOG

    #LOREM_IPSUM_DOLOR_SIT_AMET_CONSECTETUR_ADIPISCING_ELIT_CURABITUR_SED_TORTOR_SIT_AMET_ERAT_CONSEQUAT_RUTRUM_UT_ENIM_VELIT_CONGUE_SED_ODIO_A_TINCIDUNT_TRISTIQUE_FELIS_VESTIBULUM_ID_DUI_LECTUS_SUSPENDISSE_VITAE_FAUCIBUS_ORCI_DONEC_ORCI_LIBERO_CONVALLIS_EU_INTERDUM_VITAE_GRAVIDA_NON_SAPIEN_AENEAN_NEQUE_TORTOR_ACCUMSAN_EGET_DICTUM_EU_VESTIBULUM_IN_EX_MORBI_NULLA_NULLA_CONVALLIS_NON_MAXIMUS_AT_SOLLICITUDIN_HENDRERIT_NISI_CURABITUR_VEL_QUAM_SEM_DONEC_ID_PRETIUM_TELLUS_AC_TRISTIQUE_IPSUM_CRAS_VEL_AUGUE_QUIS_LACUS_RUTRUM_CONDIMENTUM_EGET_ID_DUI_UT_TEMPUS_VIVERRA_FRINGILLA_DUIS_EROS_NISI_SAGITTIS_EU_ARCU_DICTUM_FERMENTUM_ALIQUET_RISUS

    alphabet = [chr(i) for i in range(ord('A'), ord('Z') + 1)]
    alphabet.extend('_')
    alphabet_len = len(alphabet)
    window = Tk()
    window.title('Лабораторная работа №2')
    window.geometry('600x300+400+200')
    frame = Frame(
        window,
        padx=10,
        pady=10
    )
    frame.pack(expand=True)

    mode_lb = Label(frame, text="Режим работы ('1' - кодирование, '2' - декодирование): ")
    mode_lb.grid(row=4, column=1)

    mode_en = Entry(frame)
    mode_en.grid(row=4, column=2, pady=5)

    cal_btn = Button(frame, text='Запустить программу', command=Start)
    cal_btn.grid(row=10, column=2)

    window.mainloop()


