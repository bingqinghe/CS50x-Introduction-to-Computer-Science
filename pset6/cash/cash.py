from cs50 import get_float
# keep looking for positive input
dollar = 0
while (dollar <= 0):
    dollar = get_float("Change owed: ")
    
total = 0
cent = round(dollar * 100, 0)

while (cent > 0):
    # 0.25
    if (cent >= 25):
        cent -= 25
    # 0.1
    elif (cent >= 10):
        cent -= 10
    # 0.05
    elif (cent >= 5):
        cent -= 5
    # 0.01
    else:
        cent -= 1
    total += 1
    
# output
print(f"{total}")