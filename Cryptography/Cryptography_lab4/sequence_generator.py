import random


def getSequence(size: int, seed: float) -> str:
    random.seed(seed)
    sequence = ""
    alphabet = [chr(i) for i in range(ord('A'), ord('D') + 1)]
    #alphabet.extend('_')
    alphabet_size = len(alphabet)
    dash_probability = 0
    for i in range(size):
        # if random.random()<dash_probability:
        #     symbol_to_add = '_'
        #     dash_probability = 0
        # else:
        symbol_to_add =alphabet[random.randint(0, alphabet_size-1)]
        dash_probability += .05
        sequence+=symbol_to_add
    return sequence


if __name__ == "__main__":
    size = 1000
    seed = 1
    print(getSequence(size, seed))