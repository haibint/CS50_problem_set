/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <stdio.h>

#include "helpers.h"


/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // This is now a linear search.
    // for (int i=0; i<n; i++)
    // {
    //     if (values[i] == value)
    //     {
    //         return true;
    //     }
    // }
    // return false;

    // Now, This is a binary search attempt.
    int array_size = n;
    int index_to_evaluate = n/2;
    do
    {
        // int middle_number = array_size/2;
        printf("%i\n", index_to_evaluate);
        if (values[index_to_evaluate] == value)
        {
            return true;
        }
        else if (values[index_to_evaluate] < value)  // the value is on the right hand side.
        {
            // values = values[middle_number:-0];   This will not work in C.
            index_to_evaluate = index_to_evaluate + (index_to_evaluate)/2 + 1;
            array_size = array_size/2;
        }
        else if (values[index_to_evaluate] > value)
        {
            index_to_evaluate = index_to_evaluate - (index_to_evaluate)/2 + 1;
            array_size = array_size/2;
        }
    }
    while(array_size != 0);
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // This is an attempt of bubble sort. I don't know how should I check if the arrary is sorted and then stop it.
    int swapped_time = 1;
    do
    {
        swapped_time = 0;
        for (int i=0; i<n; i++)
        {
            int left = values[i];
            int right = values[i+1];
            if (right<left)
            {
                values[i] = right;
                values[i+1] = left;
                swapped_time +=1;
            }
        }
    }
    while(swapped_time != 0);
    // print out the sorted array to check.
    // for (int k=0; k<n; k++)
    // {
    //     printf("%i\n", values[k]);

    // }
    return;
}
