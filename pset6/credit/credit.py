from cs50 import get_string
from math import floor

# AM: 15       34/37
# VS: 13/16    4
# MC: 16       51/52/53/54/55

result = "INVALID"
# get input
card_number = get_string("Number: ")
card_number = card_number.strip()
card_length = len(card_number)

# length == 15
if (card_length == 15):
    if (card_number[:2] == "34" or card_number[:2] == "37"):
        result = "AMEX"
# length == 13
elif (card_length == 13):
    if (card_number[:1] == "4"):
        result = "VISA"
# length == 16
elif (card_length == 16):
    # VISA
    if (card_number[:1] == "4"):
        result = "VISA"
    # MC
    elif (card_number[:2] == "51" or card_number[:2] == "52" or card_number[:2] == "53" or card_number[:2] == "54" or card_number[:2] == "55"):
        result = "MASTERCARD"

#############################
# do the digits calculation #
#############################
if (result != "INVALID"):
    card_number = int(card_number)
    last = 0
    temp = 0
    seco = 0

    while (card_number > 0):
        last += card_number % 10  # last digit
        card_number /= 10
        card_number = floor(card_number)
        temp = (card_number % 10) * 2  # do the digit multiple
        # result >= 10
        if (temp >= 10):
            seco += temp % 10
            seco += 1
        else:
            seco += temp
        card_number /= 10
        card_number = floor(card_number)
    # valid card
    if ((last + seco) % 10 == 0):
        print(result)
    else:
        print("INVALID")
else:
    print(result)