#include <stdio.h>
#include <cs50.h>

void printresult(int height);

int main(void)
{
    // initial variable
    int height = 0;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    // get valid input and output the result
    printresult(height);
}

void printresult(int height)
{
    // outer: total lines
    for (int i = 0; i < height; i++)
    {
        // space = (height-1)-#line
        for (int j = 0; j < height - i - 1; j++)
        {
            printf(" ");
        }
        // # = #line
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        // endofline newline
        printf("\n");
    }
}