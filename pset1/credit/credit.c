#include <stdio.h>
#include <cs50.h>

// AM: 15       34/37
// VS: 13/16    4
// MC: 16       51/52/53/54/55

int main(void)
{
    long long int number = get_long_long("Number: ");
    int cardlength = 0;
    int sum = 0;
    int digit = 0;
    int second = 0;
    int even = 0;

    while (number > 0)
    {
        cardlength++;
        // variable for AM/MC
        second = digit;
        // start from last
        digit = number % 10;
        number = (number - digit) / 10;

        // each even digit
        if (cardlength % 2 == 0)
        {
            even = digit * 2;
            if (even >= 10)
            {
                // from 2 * 5 to 2 * 9
                sum += even % 10;
                sum += 1;
            }
            else
            {
                sum += even;
            }
        }
        else
        {
            sum += digit;
        }
    }
    // digit == the start number
    // initial the answer
    string result = "INVALID";
    // do the start number check with sum
    if (sum % 10 == 0)
    {
        // VISA 13/16 digits with 4
        if ((cardlength == 13 || cardlength == 16) && digit == 4)
        {
            result = "VISA";
        }
        // AMEX 15 digits with 34/37
        else if (cardlength == 15 && digit == 3 && (second == 4 || second == 7))
        {
            result = "AMEX";
        }
        // MC 16 digits with 51-55
        else if (cardlength == 16 && digit == 5 && (second >= 1 && second <= 5))
        {
            result = "MASTERCARD";
        }
    }

    printf("%s\n", result);
}