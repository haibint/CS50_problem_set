#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, string argv[2])
{
    if (argc == 2)
    {
        string key = argv[1];
        int int_key = atoi(key) % 26;
        printf("plaintext:");
        string text = get_string();
        printf("ciphertext: ");
        for (int i=0; i < strlen(text); i++)
        {
            if (65 <= text[i] && text[i] <= 90)
            {
                int middle_num = text[i] - 65 + int_key;
                int cipher_char = middle_num % 26;
                printf("%c", cipher_char + 65);
            }
            else if (97 <= text[i] && text[i] <= 122)
            {
                int middle_num = text[i] - 97 + int_key;
                int cipher_char = middle_num % 26;
                printf("%c", cipher_char + 97);
            }
            else
            {
                int cipher_char = text[i];
                printf("%c", cipher_char);
            }
        }
        printf("\n");
        // printf("%s\n", key);
        // printf("%s\n", text);
        return 0;
    }
    return 1;
}