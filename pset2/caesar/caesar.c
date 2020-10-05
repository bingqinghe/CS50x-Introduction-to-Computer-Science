#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // more than 2 argv
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // invalid combination fo argv like 2x
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (argv[1][i] < 48 || argv[1][i] > 57)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    int key = 0;
    // get the valid key
    if (argc == 2)
    {
        key = atoi(argv[1]) % 26;
    }
    // error command-line

    // get input
    string input = get_string("plaintext: \n");
    // valid input, encrypt
    if (argc == 2)
    {
        for (int i = 0, n = strlen(input); i < n; i++)
        {
            // capital
            if (input[i] > 64 && input[i] < 91)
            {
                if (input[i] + key > 90)
                {
                    input[i] -= 26;
                }
                input[i] += key;
            }
            // lower
            else if (input[i] > 96 && input[i] < 122)
            {
                if (input[i] + key > 121)
                {
                    input[i] -= 26;
                }
                input[i] += key;
            }
        }
    }
    // output
    printf("ciphertext: %s\n", input);
    return 0;
}