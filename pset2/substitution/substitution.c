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
        printf("Usage: ./substitution key");
        return 1;
    }
    // argv length not 26
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.");
        return 1;
    }
    // valid input
    char key[26];
    char newAlphabet[26];
    // valid key
    for (int i = 0; i < 26; i++)
    {
        key[i] = toupper(argv[1][i]);
        // valid inputkey
        if (key[i] > 64 && key[i] < 91)
        {
            newAlphabet[i] = 65 + i - key[i];
        }
        // error for key
        else
        {
            printf("Usage: ./subtitution key\n");
            return 1;
        }
        // duplicated key
        for (int j = 0; j < i; j++)
        {
            if (key[i] == key[j])
            {
                printf("Usage: ./subtitution key\n");
                return 1;
            }
        }
    }

    // get input
    string input = get_string("plaintext: \n");
    // convert
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        // capital
        if (input[i] > 64 && input[i] < 91)
        {
            input[i] = input[i] - newAlphabet[input[i] - 65];
        }
        else if (input[i] > 96 && input[i] < 123)
        {
            input[i] = input[i] - newAlphabet[input[i] - 97];
        }
    }

    printf("ciphertext: %s\n", input);
    return 0;
}