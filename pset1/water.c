#include <cs50.h>
#include <stdio.h>

int  n;

int main(void)
{
    do
    {
        printf("How many minutes do you take for shower?  ");
        n = get_int();
    }
    while (n<= 0);
    int bottles = n * 12;
    printf("You are using %i bottles of water each time you take a shower.\n", bottles);
}