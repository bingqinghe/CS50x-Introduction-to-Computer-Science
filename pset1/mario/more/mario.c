#include <stdio.h>
#include <cs50.h>

void printresult(int height);
void printspace(int number);
void printhash(int hashnum);

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

void printspace(int number)
{
    for (int i = 0; i < number; i++)
    {
        printf(" ");
    }
}

void printhash(int hashnum)
{
    for (int i = 0; i <= hashnum; i++)
    {
        printf("#");
    }
}

void printresult(int height)
{
    // outer: total lines
    for (int i = 0; i < height; i++)
    {
        // print left space
        printspace(height - 1 - i);
        // print left #
        printhash(i);
        // space between two part
        printf("  ");
        // print right #
        printhash(i);
        // endofline newline
        printf("\n");
    }
}