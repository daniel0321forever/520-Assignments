#include <stdlib.h>
#include "solutions.h"

// calculate running total
int running_total(int num)
{
    static int total = 0;
    total += num;
    return total;
}

// reverse an array and return a new array
int *reverse(const int *array, int length)
{
    int *reversed_array = (int *)calloc(length, sizeof(int));
    if (!reversed_array)
        return NULL;

    for (int i = 0; i < length; i++)
    {
        reversed_array[i] = array[length - 1 - i];
    }
    return reversed_array;
}

// reverse an array in place
void reverse_in_place(int *array, int length)
{
    for (int i = 0; i < length / 2; i++)
    {
        int temp = array[i];
        array[i] = array[length - 1 - i];
        array[length - 1 - i] = temp;
    }
}

// count occurrences of a value in an array
int num_occurences(const int *array, int length, int value)
{
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        if (array[i] == value)
        {
            count++;
        }
    }
    return count;
}
