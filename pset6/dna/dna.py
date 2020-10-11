from sys import argv
from csv import reader, DictReader
# validate input
if len(argv) < 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

# read dna sequence
with open(argv[2]) as dna_file:
    dna_reader = reader(dna_file)
    # store each sequence
    for row in dna_reader:
        dna_list = row
# compare the dna_sequence as string
dna_string = dna_list[0]
sequence = {}  # dictionary

# read name list
with open(argv[1]) as name_file:
    name_reader = reader(name_file)
    for row in name_reader:
        dna_sequence = row
        dna_sequence.pop(0)  # remove the first
        break

# initial the key-value pair
for item in dna_sequence:
    sequence[item] = 1

# loop and add the apperance time
for key in sequence:
    length = len(key)
    temp = 0
    maximum = 0
    for i in range(len(dna_string)):
        while temp > 0:
            temp -= 1  # count++
            continue
        if dna_string[i: i + length] == key:  # key found
            while dna_string[i - length: i] == dna_string[i: i + length]:
                temp += 1
                i += length  # next start
                # change the maximum time
                if temp > maximum:
                    maximum = temp
    sequence[key] += maximum

# output
with open(argv[1], newline="") as name_file:
    name = DictReader(name_file)
    for people in name:
        match = 0
        for dna in sequence:
            if sequence[dna] == int(people[dna]):
                match += 1
        if match == len(sequence):
            print(people["name"])
            exit(0)
    print("No match")