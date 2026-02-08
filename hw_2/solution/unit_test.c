#include <stdio.h>
#include <assert.h>
#include "solutions.h"

void test_running_total()
{
    assert(running_total(1) == 1);
    assert(running_total(1) == 2);
    assert(running_total(5) == 7);
    assert(running_total(-3) == 4);
    printf("Running Total Tests Passed\n");
}

void test_reverse()
{
    int x[] = {10, 20, 30, 40, 50};
    int *y = reverse(x, 5);
    assert(y[0] == 50);
    assert(y[1] == 40);
    assert(y[2] == 30);
    assert(y[3] == 20);
    assert(y[4] == 10);
    free(y);
    printf("Reverse Tests Passed\n");
}

void test_reverse_in_place()
{
    int x[] = {10, 20, 30, 40, 50};
    reverse_in_place(x, 5);
    assert(x[0] == 50);
    assert(x[1] == 40);
    assert(x[2] == 30);
    assert(x[3] == 20);
    assert(x[4] == 10);
    printf("Reverse In Place Tests Passed\n");
}

void test_num_occurences()
{
    int a[] = {1, 1, 2, 3, 1, 4, 5, 2, 20, 5};
    assert(num_occurences(a, 10, 1) == 3);
    assert(num_occurences(a, 10, 2) == 2);
    assert(num_occurences(a, 10, 5) == 2);
    assert(num_occurences(a, 10, 10) == 0);
    printf("Num Occurrences Tests Passed\n");
}

// int main() {
//     test_running_total();
//     test_reverse();
//     test_reverse_in_place();
//     test_num_occurences();
//     printf("All Tests Passed\n");
//     return 0;
// }