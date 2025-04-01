import random


def writeKeyTableToFile(key_seed:float=5)->None:
    random.seed(key_seed)
    alphabet = [chr(i) for i in range(ord('A'), ord('D')+1) ]
    #alphabet.extend('_')
    alphabet_size = len(alphabet)
    letter_combinations = [(i,j) for j in range(alphabet_size) for i in range(alphabet_size)]
    random.shuffle(letter_combinations)
    print(letter_combinations)
    letter_counter = 0
    with open('key_table.txt', 'w') as table_file:
        for i in range(alphabet_size):
            for j in range(alphabet_size):
                table_file.write(f'{alphabet[letter_combinations[letter_counter][0]]}{alphabet[letter_combinations[letter_counter][1]]} ')
                letter_counter+=1
            table_file.write('\n')
    letter_counter = 0
    with open('key_table_with_headlines.txt', 'w') as table_file:
        for i in range(alphabet_size):
            table_file.write(f'   {alphabet[i]}')
        table_file.write('\n')
        for i in range(alphabet_size):
            table_file.write(f'{alphabet[i]}  ')
            for j in range(alphabet_size):
                table_file.write(f'{alphabet[letter_combinations[letter_counter][0]]}{alphabet[letter_combinations[letter_counter][1]]}  ')
                letter_counter+=1
            table_file.write('\n')
    with open('key_table.txt', 'r') as table_file:
        letter_pairs = []
        for line in table_file:
            letter_pairs.extend(line.removesuffix('\n').split())
        print(letter_pairs)
        if(len(letter_pairs) != len(set(letter_pairs))):
            print("Has duplicates!")
        else:
            print("Has no duplicates!")
    return None


if __name__=="__main__":
    writeKeyTableToFile(123)