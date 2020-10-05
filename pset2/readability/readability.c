#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cs50.h>

int main(void)
{
    // get user input
    string input = get_string("Text: \n");
    float letter = 0;
    float word = 0;
    float sentence = 0;

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        // letters ASCII 65-90 97-122
        if (input[i] > 64 && input[i] < 123)
        {
            letter++;
        }
        // words space=32, !=33 .=46 ?=63
        else if (input[i] == 32 && (input[i - 1] != 33 && input[i - 1] != 46 && input[i - 1] != 63))
        {
            word++;
        }
        else if (input[i] == 33 || input[i] == 46 || input[i] == 63)
        {
            word++;
            sentence++;
        }
    }

    // apply formula index = 0.0588 * L - 0.296 * S - 15.8
    float L = letter * 100 / word;
    float S = sentence * 100 / word;
    float index = round(0.0588 * L - 0.296 * S - 15.8);

    // output
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int)index);
    }
}