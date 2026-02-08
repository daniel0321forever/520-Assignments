#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* private functions *********************************************************/

/* Position in the buffer of the array element at position index */
static int index_to_offset(const DynamicArray *da, int index)
{
    return index + da->origin;
}

/* Position of the element at buffer position 'offset' */
static int offset_to_index(const DynamicArray *da, int offset)
{
    return offset - da->origin;
}

/* Non-zero if and only if offset lies ouside the buffer */
static int out_of_buffer(DynamicArray *da, int offset)
{
    return offset < 0 || offset >= da->capacity;
}

/* Makes a new buffer that is twice the size of the old buffer,
   copies the old information into the new buffer, and deletes
   the old buffer */
static void extend_buffer(DynamicArray *da)
{

    double *temp = (double *)calloc(2 * da->capacity, sizeof(double));
    int new_origin = da->capacity - (da->end - da->origin) / 2,
        new_end = new_origin + (da->end - da->origin);

    for (int i = 0; i < DynamicArray_size(da); i++)
    {
        temp[new_origin + i] = DynamicArray_get(da, i);
    }

    free(da->buffer);
    da->buffer = temp;

    da->capacity = 2 * da->capacity;
    da->origin = new_origin;
    da->end = new_end;

    return;
}

/* public functions **********************************************************/

static DynamicArray *allocated_arrays = NULL; // Head of allocated array list
static int num_allocated_arrays = 0;          // Counter for allocated arrays

DynamicArray *DynamicArray_new(void)
{
    DynamicArray *da = (DynamicArray *)malloc(sizeof(DynamicArray));
    da->capacity = DYNAMIC_ARRAY_INITIAL_CAPACITY;
    da->buffer = (double *)calloc(da->capacity, sizeof(double));
    da->origin = da->capacity / 2;
    da->end = da->origin;

    // Add to global list
    da->next = allocated_arrays;
    allocated_arrays = da;
    num_allocated_arrays++;
    return da;
}

void DynamicArray_destroy(DynamicArray *da)
{
    free(da->buffer);
    da->buffer = NULL;

    // Remove from linked list
    DynamicArray **ptr = &allocated_arrays;
    while (*ptr)
    {
        if (*ptr == da)
        {
            *ptr = da->next;
            break;
        }
        ptr = &((*ptr)->next);
    }

    num_allocated_arrays--;
    return;
}

int DynamicArray_size(const DynamicArray *da)
{
    assert(da->buffer != NULL);
    return da->end - da->origin;
}

char *DynamicArray_to_string(const DynamicArray *da)
{
    assert(da->buffer != NULL);
    char *str = (char *)calloc(20, DynamicArray_size(da)),
         temp[20];
    int j = 1;
    str[0] = '[';
    for (int i = 0; i < DynamicArray_size(da); i++)
    {
        if (DynamicArray_get(da, i) == 0)
        {
            snprintf(temp, 20, "0");
        }
        else
        {
            snprintf(temp, 20, "%.5lf", DynamicArray_get(da, i));
        }
        if (i < DynamicArray_size(da) - 1)
        {
            sprintf(str + j, "%s,", temp);
            j += strlen(temp) + 1;
        }
        else
        {
            sprintf(str + j, "%s", temp);
            j += strlen(temp);
        }
    }
    str[j] = ']';
    return str;
}

void DynamicArray_print_debug_info(const DynamicArray *da)
{

    char *s = DynamicArray_to_string(da);
    printf("  %s\n", s);
    printf("  capacity: %d\n  origin: %d\n  end: %d\n  size: %d\n\n",
           da->capacity,
           da->origin,
           da->end,
           DynamicArray_size(da));

    free(s);
}

void DynamicArray_set(DynamicArray *da, int index, double value)
{ // change the value of the array
    assert(da->buffer != NULL);
    assert(index >= 0);
    while (out_of_buffer(da, index_to_offset(da, index)))
    {
        extend_buffer(da);
    }
    da->buffer[index_to_offset(da, index)] = value;
    if (index >= DynamicArray_size(da))
    {
        da->end = index_to_offset(da, index + 1);
    }
}

double DynamicArray_get(const DynamicArray *da, int index)
{ //
    assert(da->buffer != NULL);
    assert(index >= 0);
    if (index >= DynamicArray_size(da))
    { // index out of bounds
        return 0;
    }
    else
    {
        return da->buffer[index_to_offset(da, index)];
    }
}

void DynamicArray_push(DynamicArray *da, double value)
{
    DynamicArray_set(da, DynamicArray_size(da), value);
}

void DynamicArray_push_front(DynamicArray *da, double value)
{
    assert(da->buffer != NULL);
    while (da->origin == 0)
    {
        extend_buffer(da);
    }
    da->origin--;
    DynamicArray_set(da, 0, value);
}

double DynamicArray_pop(DynamicArray *da)
{
    assert(DynamicArray_size(da) > 0);
    double value = DynamicArray_get(da, DynamicArray_size(da) - 1);
    DynamicArray_set(da, DynamicArray_size(da) - 1, 0.0);
    da->end--;
    return value;
}

double DynamicArray_pop_front(DynamicArray *da)
{
    assert(DynamicArray_size(da) > 0);
    double value = DynamicArray_get(da, 0);
    da->origin++;
    return value;
}

DynamicArray *DynamicArray_map(const DynamicArray *da, double (*f)(double))
{
    assert(da->buffer != NULL);
    DynamicArray *result = DynamicArray_new();
    for (int i = 0; i < DynamicArray_size(da); i++)
    {
        DynamicArray_set(result, i, f(DynamicArray_get(da, i)));
    }
    return result;
}

DynamicArray *DynamicArray_subarray(const DynamicArray *da, int a, int b)
{

    assert(da->buffer != NULL);
    assert(b >= a);

    DynamicArray *result = DynamicArray_new();

    for (int i = a; i < b; i++)
    {
        DynamicArray_push(result, DynamicArray_get(da, i));
    }

    return result;
}

// =================================================================
// homework 3
// =================================================================

// min
double DynamicArray_min(const DynamicArray *da)
{
    assert(da->buffer != NULL);
    double min = DynamicArray_get(da, 0);
    for (int i = 1; i < DynamicArray_size(da); i++)
    {
        if (DynamicArray_get(da, i) < min)
        {
            double value = DynamicArray_get(da, i);
            if (value < min)
            {
                min = value;
            }
        }
    }
    return min;
}

// max
double DynamicArray_max(const DynamicArray *da)
{
    assert(da->buffer != NULL);
    double max = DynamicArray_get(da, 0);
    for (int i = 1; i < DynamicArray_size(da); i++)
    {
        if (DynamicArray_get(da, i) > max)
        {
            double value = DynamicArray_get(da, i);
            if (value > max)
            {
                max = value;
            }
        }
    }
    return max;
}

// sum
double DynamicArray_sum(const DynamicArray *da)
{
    // assert(da->buffer!= NULL);
    double sum = 0.0;
    for (int i = 0; i < DynamicArray_size(da); i++)
    {
        sum += DynamicArray_get(da, i);
    }
    return sum;
}

// mean
double DynamicArray_mean(const DynamicArray *da)
{
    assert(da->buffer != NULL);
    double sum = DynamicArray_sum(da);
    return sum / DynamicArray_size(da);
}

// Comparison function for qsort
int compare_doubles(const void *a, const void *b)
{
    double da = *(const double *)a;
    double db = *(const double *)b;
    return (da > db) - (da < db); // Returns -1, 0, or 1
}

// median
double DynamicArray_median(const DynamicArray *da)
{
    assert(da->buffer != NULL); // Ensure array is not empty
    int size = DynamicArray_size(da);
    double *sorted = (double *)malloc(size * sizeof(double));
    if (sorted == NULL)
    {
        return 0.0;
    }
    for (int i = 0; i < size; i++)
    {
        sorted[i] = DynamicArray_get(da, i);
    }

    // find the median value
    qsort(sorted, size, sizeof(double), compare_doubles);
    double median = (size % 2) ? sorted[size / 2] : (sorted[size / 2 - 1] + sorted[size / 2]) / 2.0;
    free(sorted);

    return median;
}

// last
double DynamicArray_last(const DynamicArray *da)
{
    assert(da->buffer != NULL);
    return DynamicArray_get(da, DynamicArray_size(da) - 1);
}

// first
double DynamicArray_first(const DynamicArray *da)
{
    assert(da->buffer != NULL);
    return DynamicArray_get(da, 0);
}

// copy
DynamicArray *DynamicArray_copy(const DynamicArray *da)
{
    assert(da->buffer != NULL);
    DynamicArray *result = DynamicArray_new();
    for (int i = 0; i < DynamicArray_size(da); i++)
    {
        DynamicArray_push(result, DynamicArray_get(da, i));
    }
    return result;
}

// range
DynamicArray *DynamicArray_range(double a, double b, double step)
{
    assert(step != 0);
    DynamicArray *result = DynamicArray_new();
    double value = a;
    while (value <= b)
    {
        // if (value > b) break;
        DynamicArray_push(result, value);
        value += step;
    }
    return result;
}

// concat
DynamicArray *DynamicArray_concat(const DynamicArray *da, const DynamicArray *db)
{
    assert(da != NULL && db != NULL);

    DynamicArray *result = DynamicArray_new();
    if (!result)
        return NULL;

    // Append all elements from array 'a'
    for (int i = 0; i < DynamicArray_size(da); i++)
    {
        DynamicArray_push(result, DynamicArray_get(da, i));
    }

    // Append all elements from array 'b'
    for (int i = 0; i < DynamicArray_size(db); i++)
    {
        DynamicArray_push(result, DynamicArray_get(db, i));
    }

    return result;
}

// take

DynamicArray *DynamicArray_take(const DynamicArray *da, int n)
{
    assert(da != NULL);

    int size = DynamicArray_size(da); // 5
    int abs_n = (n < 0) ? abs(n) : n; // 7
    DynamicArray *result = DynamicArray_new();

    // for (int i = 0; i < abs_n; i++) {
    //     DynamicArray_push(result, 0.0);
    // }

    if (n < 0 && abs_n > size)
    {
        printf("Error: Cannot take %d elements from an array of size %d\n", abs_n, size);
        // return NULL;
        return DynamicArray_new();
    }

    if (abs_n <= size)
    {
        if (n > 0)
        {
            result = DynamicArray_subarray(da, 0, abs_n); // take from beginning
        }
        else
        {
            result = DynamicArray_subarray(da, size - abs_n, size);
        }
    }
    else
    { // not enough elements
        DynamicArray *sub = DynamicArray_subarray(da, 0, size);

        for (int i = 0; i < DynamicArray_size(sub); i++)
        {
            DynamicArray_push(result, DynamicArray_get(sub, i));
        }

        // Fill remaining elements with zero
        while (DynamicArray_size(result) < abs_n)
        {
            DynamicArray_push(result, 0.0);
        }

        DynamicArray_destroy(sub);
    }

    return result;
}

//================================================================
// bonus
//================================================================

int DynamicArray_num_arrays()
{
    return num_allocated_arrays;
}

int DynamicArray_destroy_all()
{
    printf("Destroying all arrays. Initial count: %d\n", num_allocated_arrays);
    DynamicArray *current = allocated_arrays;
    while (current)
    {
        DynamicArray *next = current->next;
        DynamicArray_destroy(current); // Frees internal data but not the struct itself
        current = next;
    }
    allocated_arrays = NULL;
    num_allocated_arrays = 0;
    printf("Final count after destroy_all: %d\n", num_allocated_arrays);
    return 0;
}

int DynamicArray_is_valid(const DynamicArray *da)
{
    return da && da->buffer != NULL;
}
