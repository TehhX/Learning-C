// Linux and GCC only example
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

/*
    @brief A function to peform some action on a requested array element

        @param array_element The element that was either found or created
        @param value The requested value
*/
typedef void (*const sorted_find_insert_int_action_t)(int *const array_element, const int value);

/*
    @brief The general function for which all sorted_find_insert_int functions derive

        @param array A pointer to the array to search
        @param array_len A pointer to the length of the array. If 0, will malloc and insert `value`
        @param value The value to find/insert
        @param on_find The action to perform when finding `value`
        @param on_insert The action to perform when inserting `value`
*/
typedef void (*const sorted_find_insert_int)(int **const array, size_t *const array_len, const int value, sorted_find_insert_int_action_t on_find, sorted_find_insert_int_action_t on_insert);

// Searches linearly from the beginning
void sorted_find_insert_int_linear(int **const array, size_t *const array_len, const int value, sorted_find_insert_int_action_t on_find, sorted_find_insert_int_action_t on_insert)
{
    if (*array_len == 0)
    {
        *(*array = malloc(sizeof(int))) = value;
        on_insert(*array, value);
        ++*array_len;
        return;
    }

    for (size_t i = 0; i < *array_len; ++i)
    {
        if (array[0][i] > value)
        {
            *array = realloc(*array, sizeof(int) * ++*array_len);
            memmove((*array) + i + 1, (*array) + i, sizeof(int) * (*array_len - i));
            on_insert((*array) + i, value);
            return;
        }
        else if (array[0][i] == value)
        {
            on_find((*array) + i, value);
            return;
        }
    }

    *array = realloc(*array, sizeof(int) * ++*array_len);
    on_insert((*array) + (*array_len) - 1, value);
}

// Searches using binary search
void sorted_find_insert_int_binary(int **const array, size_t *const array_len, const int value, sorted_find_insert_int_action_t on_find, sorted_find_insert_int_action_t on_insert)
{
    if (*array_len == 0)
    {
        *(*array = malloc(sizeof(int))) = value;
        on_insert(*array, value);
        ++*array_len;
        return;
    }
    else if (array[0][0] > value)
    {
        *array = realloc(*array, sizeof(int) * ++*array_len);
        memmove(array[0] + 1, array[0], sizeof(int) * (*array_len - 1));
        on_insert(*array, value);
        return;
    }
    else if (array[0][0] == value)
    {
        on_find(*array, value);
        return;
    }
    else if (array[0][*array_len - 1] < value)
    {
        *array = realloc(*array, sizeof(int) * ++*array_len);
        on_insert(*array + *array_len - 1, value);
        return;
    }
    else if (array[0][*array_len - 1] == value)
    {
        on_find(*array + *array_len - 1, value);
        return;
    }
    else if (*array_len == 1)
    {
        if (array[0][0] == value)
        {
            on_find(*array, value);
            return;
        }
        else // Implicitly array[0][0] < value
        {
            *array = realloc(*array, sizeof(int) * ++*array_len);
            on_insert((*array) + 1, value);
            return;
        }
    }

    size_t low = 0, high = *array_len - 1;
    while (low < high)
    {
        const size_t mid = low + (high - low) / 2;
        if (array[0][mid] == value)
        {
            on_find(array[0] + mid, value);
            return;
        }
        else if (array[0][mid] < value)
        {
            low = mid + 1;
        }
        else // Implicitly array[0][mid] > value
        {
            high = mid - 1;
        }
    }

    // Didn't find value
    *array = realloc(*array, sizeof(int) * ++*array_len);
    memmove((*array) + low, (*array) + high, sizeof(int) * (*array_len - low));
    on_insert((*array) + low, value);
}

const int *element;

void list_on_find(int *const list_element, __attribute__((unused)) const int value)
{
    element = list_element;
}

void list_on_insert(int *const list_element, const int value)
{
    *list_element = value;
    element = list_element;
}

static inline void test_method(const char *const identifier, sorted_find_insert_int method, size_t list_len, const int value, const int *const expected_result, const size_t expected_result_len, const size_t expected_index)
{
    int *list = malloc(sizeof(int) * list_len);
    for (size_t i = 0; i < list_len; ++i)
    {
        list[i] = i * 4;
    }

    struct timeval start_tval;
    gettimeofday(&start_tval, NULL);

    method(&list, &list_len, value, list_on_find, list_on_insert);

    const int correct_index = (expected_index == (size_t) (element - list));

    struct timeval end_tval;
    gettimeofday(&end_tval, NULL);

    const double total = (end_tval.tv_usec / 1000000.0 + end_tval.tv_sec) - (start_tval.tv_usec / 1000000.0 + start_tval.tv_sec);

    int correct_contents = 1;
    if (list_len == expected_result_len)
    {
        for (size_t i = 0; i < list_len; ++i)
        {
            if (list[i] != expected_result[i])
            {
                correct_contents = 0;
                break;
            }
        }
    }
    else
    {
        correct_contents = 0;
    }

    free(list);

    printf("%20s | %8lfs | %15s | %12s\n", identifier, total, (correct_contents ? "Correct" : "  False"), (correct_index ? "Correct" : "  False"));
}

void test_cases(const char *const method_identifier, sorted_find_insert_int method)
{
    printf(
        "%s:\n"
        "          Identifier |      Time | Contents Status | Index Status\n"
        "---------------------+-----------+-----------------+-------------\n"
        ,
        method_identifier
    );

    test_method(   "Find Start", method, 4,   0, (int[]){ 0, 4, 8, 12 },      4, 0);
    test_method(  "Find Middle", method, 4,   8, (int[]){ 0, 4, 8, 12 },      4, 2);
    test_method(     "Find End", method, 4,  12, (int[]){ 0, 4, 8, 12 },      4, 3);
    test_method(   "Insert Pre", method, 4, -10, (int[]){ -10, 0, 4, 8, 12 }, 5, 0);
    test_method("Insert Middle", method, 4,   5, (int[]){ 0, 4, 5, 8, 12 },   5, 2);
    test_method(  "Insert Post", method, 4,  13, (int[]){ 0, 4, 8, 12, 13 },  5, 4);
    test_method(        "Empty", method, 0,  32, (int[]){ 32 },               1, 0);
}

int main()
{
    test_cases("Linear", sorted_find_insert_int_linear);
    test_cases("Binary", sorted_find_insert_int_binary);
}
