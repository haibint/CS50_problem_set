/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


#include "dictionary.h"

typedef struct node
{
    char word[LENGTH + 1];   // I guess we need to say LENGTH + 1 because we need to put \0 at the end of a string.
    struct node *next;
}
node;

node *hashtable[26];  // creating an array of 26 node pointers, indicating 26 buckets for words in the dictionary.
char word[LENGTH+1];


int hash_func(char *string)
{
    int alphabet_index = tolower(string[0]) - 97;
    return alphabet_index;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // Doing
    int first_char_from_text = tolower(word[0]) - 97;
    // printf("this word starts with %i and\n", first_char_from_text);
    if (hashtable[first_char_from_text] != NULL)
    {
        // it seems codes below for this function never got excuted. the hash table is always Null.
        node* cursor = hashtable[first_char_from_text];
        do
        {
            printf("the word checking is %s", word);
            printf("the word looking in dict is %s", cursor->word);
            if (word == cursor->word)   // if we use small dictionary here, we will have segmentation fault because of cursor is pointing NULL.
            {
                return true;
            }
            cursor = cursor->next;   // Why will I get error when I put cursor = *cursor->next
        }while ( cursor->next != NULL); // Why will I get error when I put *cursor->next != NULL
    }
    return false;  //false here means the word is misspelled.
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // I think I have done this fuction. But it seems it is not loading the dict properly.
    // initialize all value in hashtable to be null pointer.
    for (int i=0; i<26; i++)
    {
        hashtable[i]=NULL;
    }
    FILE* dict = fopen(dictionary, "r");
    if (dict != NULL)
    {
        // I should read the dictionary into a linked list here(maybe).
        while (fscanf(dict, "%s", word) != EOF)
        {
            node *new_node = malloc(sizeof(node));
            if (new_node == NULL)
            {
                unload();
                return false;
            }
            // if the new_node is successfully created, we need to put information in it.
            strcpy(new_node->word, word);  // putting the word into the creating node->word.
            // new_node->next = NULL // putting null pointer to the end of linked list.

            // you need to use hash function to return an ascii number here:
            int index = hash_func(word);
            // printf("the index is %i and the current hashing word is %s\n",index, word);

            if (hashtable[index]==NULL)   // if the a bucket in the hashtable is empty, insert a head pointer of the linked-list.
            {
                hashtable[index]=new_node;
            }
            new_node->next=hashtable[index];
            hashtable[index]=new_node;
        }
        // printf("%s\n", hashtable[15]->word);  //it seems the hash table is successfully created
        // printf("%s\n", hashtable[15]->next->word);

        // try to print out the loaded linked list/
        node* printing_cursor = hashtable[0];
        while (printing_cursor->next != NULL)
        {
            printf("%s\n", printing_cursor->word);
            printing_cursor = printing_cursor->next;
        }
        return true;
    }
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // I think I am done implementing this function.
    unsigned int counter=0;
    for (int i=0;i<26;i++)
    {
        if (hashtable[i] != NULL)
        {
            do
            {
                counter++;
            } while (hashtable[i]->next != NULL);
        }
    }
    return counter;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    for (int i=0; i<26; i++)
    {
        node* cursor = hashtable[i];
        while (cursor!=NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return false;
}
