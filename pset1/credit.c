#include <cs50.h>
#include <stdio.h>

long long number;
long long counter = 1;
int j = 0;
int every_each_sum1 = 0;
int every_each_sum2 = 0;

int main(void)
{
    do
    {
        printf("What's your credit card number?  ");
        number = get_long_long();
    }
    while (number<= 0);

    for(int i=0; i<16; i++)
    {
        long long every_other_digit = number / counter;
        counter = counter * 10;
        int molodul = every_other_digit % 10;
        // printf("%i", molodul);
        // if this is a odd digit.
        if (j%2 == 0)
        {
            every_each_sum1 = every_each_sum1 + molodul;
        }
        // if this is a even digit.
        else
        {
            int doubled_molodul = molodul * 2;
            if (doubled_molodul >= 10)
            {
                doubled_molodul = doubled_molodul%10 + 1;
            }
            every_each_sum2 = every_each_sum2 + doubled_molodul;
            // printf("%i\n", doubled_molodul);
        }
        j++;
    }
    int final_sum = every_each_sum1 + every_each_sum2;
    // printf("%i  ", every_each_sum1);
    // printf("%i  ", every_each_sum2);
    // printf("%i\n", final_sum);
    if (final_sum % 10 == 0 && number / 100000000000000 == 3)
    {
      printf("AMEX\n");
    }
    else if (final_sum % 10 == 0 && number / 1000000000000000 == 5)
    {
      printf("MASTERCARD\n");
    }
    else if (final_sum % 10 == 0 && number / 1000000000000000 == 4)
    {
      printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}