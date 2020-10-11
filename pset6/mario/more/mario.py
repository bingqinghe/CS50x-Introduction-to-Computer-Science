from cs50 import get_int
# get valid input
input = 0
while (input < 1 or input > 8):
    input = get_int("Height:\n")
# out put # * height i
for i in range(input):
    print(" " * (input - i - 1), end="")
    print("#" * (i + 1), end="  ")
    print("#" * (i + 1))