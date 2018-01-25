#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(int argc, string argv[2])
{
    if (argc == 2)
    {
        // I should write a script to make all the argv into uppper case.
        // work to do.
        string key = argv[1];
        printf("plaintext:");
        string text = get_string();
        printf("ciphertext: ");
        int key_in_number[strlen(key)];
        int special_character_couter = 0;
        for (int i=0; i < strlen(key); i++)
        {
            if (argc == 2 && argv[1][i] >= 65 && argv[1][i] <= 90 ) // character is uppercase.
            {
                key_in_number[i] = argv[1][i] - 65;
            }
            else if (argc == 2 && argv[1][0] >= 97 && argv[1][0] <= 122 ) // In this case, the key is lowercase letter
            {
                key_in_number[i] = argv[1][i] - 97;
            }
            else if (argc == 2 && argv[1][0] >= 48 && argv[1][0] <= 57 ) // In this case, the key is lowercase letter
            {
                key_in_number[i] = argv[1][i] - 48;
            }
        }
        // printf("%i\n", key_in_number[3]);   // key_in_number is an array with int in it and the length is the same as the key
        for (int n=0;  n < strlen(text); n++)
        {
            if (text[n] >= 65 && text[n] <= 90) // if the character in iteration is a uppercase letter.
            {
                int cipher_char = text[n] - 65 + key_in_number[(n - special_character_couter) % strlen(key)];  // (0-25) + shift unit
                char mol_cipher_char = cipher_char % 26 + 65;   // convert back to ascii representation
                printf("%c", mol_cipher_char);
            }
            else if (text[n] >= 97 && text[n] <= 122) // if the character in iteration is a lowercase letter.
            {
                int cipher_char = text[n] - 97 + key_in_number[(n - special_character_couter) % strlen(key)];
                char mol_cipher_char = cipher_char % 26 + 97;
                printf("%c", mol_cipher_char);
            }
            else
            {
                printf("%c", text[n]);
                special_character_couter += 1;
            }
        }
        printf("\n");
        return 0;
    }
    printf("cml argurement not correct!\n");
    return 1;
}