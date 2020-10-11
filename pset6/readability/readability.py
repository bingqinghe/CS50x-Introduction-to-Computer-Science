from cs50 import get_string
# user input
input = get_string("Text: ")
letter = 0
word = 0
sentence = 0

for i in range(len(input)):
    # letters ASCII 65-90 97-122
    if (ord(input[i]) > 64 and ord(input[i]) < 123):
        letter += 1
    # words space=32, !=33 .=46 ?=63
    elif (ord(input[i]) == 32 and (ord(input[i - 1]) != 33 and ord(input[i - 1]) != 46 and ord(input[i - 1]) != 63)):
        word += 1
    # .!?
    elif (ord(input[i]) == 33 or ord(input[i]) == 46 or ord(input[i]) == 63):
        word += 1
        sentence += 1
# output calculation
L = letter * 100 / word
S = sentence * 100 / word
index = round(0.0588 * L - 0.296 * S - 15.8)

# output
if (index < 1):
    print("Before Grade 1")
elif (index > 15):
    print("Grade 16+")
else:
    print(f"Grade {index}")
