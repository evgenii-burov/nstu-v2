import random


def GenerateSequence(length:int, p_distribution_choice) -> None:
    sequence = ""
    if p_distribution_choice not in ('1', '2', '3'):
        print("Invalid distribution choice, exiting..")
        exit(0)
    probabilities = []
    try:
        with open(f"probabilities{p_distribution_choice}.txt", "r") as probabilities_file:
            for line in probabilities_file:
                probabilities = line.split(sep=' ')
                break
            for i in range(len(probabilities)):
                probabilities[i] = float(probabilities[i])
    except FileNotFoundError:
        print(f"Couldn\'t open the \"probabilities{p_distribution_choice}.txt\" file, exiting..")
        exit(0)
    probability_sum = 0.
    if len(probabilities) != alphabet_len:
        print(f"Invalid probabilities: there must be {alphabet_len} probabilities, got {len(probabilities)}, exiting..")
        exit(0)
    for p in probabilities:
        probability_sum += p
    if abs(probability_sum - 1.) > (eps := 1e-5):
        print("Invalid probabilities: probabilities must add up to 1, exiting..")
        exit(0)
    for i in range(length):
        r = random.random()
        for j in range(alphabet_len):
            r -= probabilities[j]
            if r <= 0:
                sequence += alphabet[j]
                break
    with open("sequence_to_encode.txt", "w") as sequence_file:
        sequence_file.write(sequence)
    return None


if __name__=="__main__":
    alphabet = ["11", "12", "13", "14", "15", "16", "17"]
    alphabet_len = len(alphabet)
    GenerateSequence(5, '3')
