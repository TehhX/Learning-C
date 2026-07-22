// Linux and GCC only example
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define BIG_ARRAY_LEN 262144

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
static void sorted_find_insert_int_linear(int **const array, size_t *const array_len, const int value, sorted_find_insert_int_action_t on_find, sorted_find_insert_int_action_t on_insert)
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
        if ((*array)[i] > value)
        {
            *array = realloc(*array, sizeof(int) * ++*array_len);
            memmove(*array + i + 1, *array + i, sizeof(int) * (*array_len - i - 1));
            on_insert(*array + i, value);
            return;
        }
        else if ((*array)[i] == value)
        {
            on_find(*array + i, value);
            return;
        }
    }

    *array = realloc(*array, sizeof(int) * ++*array_len);
    on_insert(*array + *array_len - 1, value);
}

// Searches using binary search
static void sorted_find_insert_int_binary(int **const array, size_t *const array_len, const int value, sorted_find_insert_int_action_t on_find, sorted_find_insert_int_action_t on_insert)
{
    if (*array_len == 0)
    {
        *(*array = malloc(sizeof(int))) = value;
        on_insert(*array, value);
        ++*array_len;
        return;
    }
    else if ((*array)[0] > value)
    {
        *array = realloc(*array, sizeof(int) * ++*array_len);
        memmove(*array + 1, *array, sizeof(int) * (*array_len - 1));
        on_insert(*array, value);
        return;
    }
    else if ((*array)[*array_len - 1] < value)
    {
        *array = realloc(*array, sizeof(int) * ++*array_len);
        on_insert(*array + *array_len - 1, value);
        return;
    }

    size_t low = 0, high = *array_len - 1;
    while (low < high)
    {
        const size_t mid = low + (high - low) / 2;
        if ((*array)[mid] == value)
        {
            on_find(*array + mid, value);
            return;
        }
        else if ((*array)[mid] < value)
        {
            low = mid + 1;
        }
        else // Implicitly (*array)[mid] > value
        {
            high = mid - 1;
        }
    }

    if ((*array)[low] == value)
    {
        on_find(*array + low, value);
        return;
    }
    else
    {
        *array = realloc(*array, sizeof(int) * ++*array_len);
        memmove(*array + low + 1, *array + low, sizeof(int) * (*array_len - low - 1));
        on_insert(*array + low, value);
    }
}

static int compare_ints(const void *const a, const void *const b)
{
    return *(int *) a - *(int *) b;
}

// A very slow version using standard library functions
static void sorted_find_insert_int_bsearchqsort(int **const array, size_t *const array_len, const int value, sorted_find_insert_int_action_t on_find, sorted_find_insert_int_action_t on_insert)
{
    if (*array_len == 0)
    {
        *(*array = malloc(sizeof(int))) = value;
        on_insert(*array, value);
        ++*array_len;
        return;
    }

    int *result = bsearch(&value, *array, *array_len, sizeof(int), compare_ints);
    if (result)
    {
        on_find(result, value);
        return;
    }
    else
    {
        *array = realloc(*array, sizeof(int) * ++*array_len);
        (*array)[*array_len - 1] = value;
        qsort(*array, *array_len, sizeof(int), compare_ints);
        on_insert(bsearch(&value, *array, *array_len, sizeof(int), compare_ints), value);
        return;
    }
}

static const int *result_element;

static void list_on_find(int *const list_element, __attribute__((unused)) const int value)
{
    result_element = list_element;
}

static void list_on_insert(int *const list_element, const int value)
{
    *list_element = value;
    result_element = list_element;
}

static inline void test_method(const char *const identifier, sorted_find_insert_int method, size_t list_len, const int value, const int *const expected_result, const size_t expected_result_len, const size_t expected_index)
{
    int *list;
    if (list_len)
    {
        list = malloc(sizeof(int) * list_len);
    }
    for (size_t i = 0; i < list_len; ++i)
    {
        list[i] = i * 4;
    }

    struct timeval start_tval;
    gettimeofday(&start_tval, NULL);

    method(&list, &list_len, value, list_on_find, list_on_insert);

    struct timeval end_tval;
    gettimeofday(&end_tval, NULL);

    const int correct_index = (expected_index == (size_t) (result_element - list));

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

    printf("%28s | %9.6lfs | %15s | %12s\n", identifier, (end_tval.tv_usec / 1000000.0 + end_tval.tv_sec) - (start_tval.tv_usec / 1000000.0 + start_tval.tv_sec), (correct_contents ? "Correct" : "  False"), (correct_index ? "Correct" : "  False"));
}

static void test_cases(const char *const method_identifier, sorted_find_insert_int method)
{
    printf(
        "\n%s:\n"
        "                  Identifier |       Time | Contents Status | Index Status\n"
        "-----------------------------+------------+-----------------+-------------\n"
        ,
        method_identifier
    );

    test_method(        "Find Start", method, 4,   0, (int[]){ 0, 4, 8, 12 },      4, 0);
    test_method(       "Find Middle", method, 4,   8, (int[]){ 0, 4, 8, 12 },      4, 2);
    test_method(          "Find End", method, 4,  12, (int[]){ 0, 4, 8, 12 },      4, 3);
    test_method(        "Insert Pre", method, 4, -10, (int[]){ -10, 0, 4, 8, 12 }, 5, 0);
    test_method( "Insert Middle Odd", method, 4,   5, (int[]){ 0, 4, 5, 8, 12 },   5, 2);
    test_method("Insert Middle Even", method, 4,   6, (int[]){ 0, 4, 6, 8, 12 },   5, 2);
    test_method(       "Insert Post", method, 4,  13, (int[]){ 0, 4, 8, 12, 13 },  5, 4);
    test_method(             "Empty", method, 0,  32, (int[]){ 32 },               1, 0);

    int big_array[BIG_ARRAY_LEN + 1] = { 0 };
    for (size_t i = 0; i < 48000; ++i)
    {
        big_array[i] = i * 4;
    }
    big_array[48000] = 191997;
    for (size_t i = 48001; i <= BIG_ARRAY_LEN; ++i)
    {
        big_array[i] = (i - 1) * 4;
    }
    test_method( "Big Array Insert Middle Odd", method, BIG_ARRAY_LEN, 191997, big_array, BIG_ARRAY_LEN + 1, 48000);
    big_array[48000] = 191998;
    test_method("Big Array Insert Middle Even", method, BIG_ARRAY_LEN, 191998, big_array, BIG_ARRAY_LEN + 1, 48000);
    big_array[48000] = 192000;
    for (size_t i = 48001; i <= BIG_ARRAY_LEN; ++i)
    {
        big_array[i] = i * 4;
    }
    test_method("Big Array Find Middle", method, BIG_ARRAY_LEN, 192000, big_array, BIG_ARRAY_LEN, 48000);
}

int main()
{
    test_cases(       "Linear", sorted_find_insert_int_linear);
    test_cases(       "Binary", sorted_find_insert_int_binary);
    test_cases("Bsearch Qsort", sorted_find_insert_int_bsearchqsort);
}
