#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string name = get_string();
    for (int i=0; i < strlen(name); i++)
    {
        int character = name[i];
        if (character == 32)
        {
            if (name[i + 1] != 32)
            {
                printf("%c", toupper(name[i + 1]));
            // printf("%c", (char)character);
            }
        }
        else if (i==0)
        {
            printf("%c", toupper(name[i]));
        }

    }
    printf("\n");
    // :( Outputs HJ for     hailey James
    // expected "HJ\n", not "HJ\x00\n"
    // See https://cs50.me/checks/b53aeb044933e7ce9fc230ce338fe06e9a0de795 for more detail.
}