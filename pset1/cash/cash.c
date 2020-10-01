#include <stdio.h>
#include <math.h>
#include <cs50.h>

int main(void)
{
    float input = 0;
    // get valid input
    do
    {
        input = get_float("Change Owed: ");
    }
    while (input <= 0);
    // round the input cents to dollars
    int cents = round(input * 100);
    // final number of coins
    int result = 0;
    // check from the most largest cents to the smallest
    while (cents > 0)
    {
        if (cents >= 25)
        {
            cents -= 25;
        }
        else if (cents >= 10)
        {
            cents -= 10;
        }
        else if (cents >= 5)
        {
            cents -= 5;
        }
        else
        {
            cents -= 1;
        }
        // each loop will increase one coin
        result++;
    }
    printf("%i\n", result);
}