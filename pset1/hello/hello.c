#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // get user input
    string name = get_string("What is your name?\n");
    // show result
    printf("hello, %s\n", name);
}