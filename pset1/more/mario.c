#include <cs50.h>
#include <stdio.h>

int n;

int main(void)
{
    do
    {
        printf("How many layers do you want to have for mario pyramids?  ");
        n = get_int();
    }
    while (n< 0 || n > 23);


    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j< n-i-1; j++)
        {
            printf(" ");
        }
        for (int j = 0; j<i+1; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int j = 0; j<i+1; j++)
        {
            printf("#");
        }
        // for (int j = 0; j< n-i-1; j++)
        // {
        //     printf(" ");
        // }
        printf("\n");
    }
}