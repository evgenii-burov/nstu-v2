import random

def run_centrifuge_cycle(centrifuge_number: int)->tuple:
    rare_chance = .007
    uranium_rare = 0
    uranium_common = 0
    for i in range(centrifuge_number):
        if random.random() < rare_chance:
            uranium_rare+=1
        else:
            uranium_common+=1
    return (uranium_rare, uranium_common)

cycles= 300
centrifuges_count = 1

uranium_rare = 0
uranium_common = 0

avg_rare_per_cycle = 0

for i in range(cycles):
    uranium_got = run_centrifuge_cycle(centrifuges_count)
    uranium_rare += uranium_got[0]
    uranium_common += uranium_got[1]

print(f'Rare: {uranium_rare}\nCommon: {uranium_common}\nAVG: {uranium_rare/(cycles*centrifuges_count)}')