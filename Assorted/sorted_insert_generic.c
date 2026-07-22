// Linux and GCC only example
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define BIG_ARRAY_LEN 262144

typedef void (*const sorted_find_insert_action_t)(void *const array_element, const size_t element_size, const void *value);

typedef void (*const sorted_find_insert)(void **const array, size_t *const array_len, const size_t element_size, const void *const value, sorted_find_insert_action_t on_find, sorted_find_insert_action_t on_insert, const __compar_fn_t compare);

static void sorted_find_insert_linear(void **const array, size_t *const array_len, const size_t element_size, const void *const value, sorted_find_insert_action_t on_find, sorted_find_insert_action_t on_insert, const __compar_fn_t compare)
{
    if (*array_len == 0)
    {
        memcpy(*array = malloc(element_size), value, element_size);
        on_insert(*array, element_size, value);
        ++*array_len;
        return;
    }

    for (size_t i = 0; i < *array_len; ++i)
    {
        if (compare(*array + element_size * i, value) > 0)
        {
            *array = realloc(*array, sizeof(element_size) * ++*array_len);
            memmove(*array + element_size * (i + 1), *array + element_size * i, sizeof(element_size) * (*array_len - i - 1));
            on_insert(*array + element_size * i, element_size, value);
            return;
        }
        else if (compare(*array + element_size * i, value) == 0)
        {
            on_find(*array + element_size * i, element_size, value);
            return;
        }
    }

    *array = realloc(*array, element_size * ++*array_len);
    on_insert(*array + element_size * (*array_len - 1), element_size, value);
}

static void sorted_find_insert_binary(void **const array, size_t *const array_len, const size_t element_size, const void *const value, sorted_find_insert_action_t on_find, sorted_find_insert_action_t on_insert, const __compar_fn_t compare)
{
    if (*array_len == 0)
    {
        memcpy(*array = malloc(element_size), value, element_size);
        on_insert(*array, element_size, value);
        ++*array_len;
        return;
    }
    else if (compare(*array, value) > 0)
    {
        *array = realloc(*array, element_size * ++*array_len);
        memmove(*array + element_size, *array, element_size * (*array_len - 1));
        on_insert(*array, element_size, value);
        return;
    }
    else if (compare(*array + element_size * (*array_len - 1), value) < 0)
    {
        *array = realloc(*array, element_size * ++*array_len);
        on_insert(*array + element_size * (*array_len - 1), element_size, value);
        return;
    }

    size_t low = 0, high = *array_len - 1;
    while (low < high)
    {
        const size_t mid = low + (high - low) / 2;
        if (compare(*array + element_size * mid, value) == 0)
        {
            on_find(*array + element_size * mid, element_size, value);
            return;
        }
        else if (compare(*array + element_size * mid, value) < 0)
        {
            low = mid + 1;
        }
        else // Implicitly compare(*array + element_size * mid, value) > 0
        {
            high = mid - 1;
        }
    }

    if (compare(*array + element_size * low, value) == 0)
    {
        on_find(*array + element_size * low, element_size, value);
        return;
    }
    else
    {
        *array = realloc(*array, element_size * ++*array_len);
        memmove(*array + element_size * (low + 1), *array + element_size * low, element_size * (*array_len - low - 1));
        on_insert(*array + element_size * low, element_size, value);
    }
}

static int compare_ints(const void *const a, const void *const b)
{
    return *(int *) a - *(int *) b;
}

static void sorted_find_insert_bsearchqsort(void **const array, size_t *const array_len, const size_t element_size, const void *const value, sorted_find_insert_action_t on_find, sorted_find_insert_action_t on_insert, const __compar_fn_t compare)
{
    if (*array_len == 0)
    {
        memcpy(*array = malloc(element_size), value, element_size);
        on_insert(*array, element_size, value);
        ++*array_len;
        return;
    }

    int *const result = bsearch(value, *array, *array_len, element_size, compare);
    if (result)
    {
        on_find(result, element_size, value);
        return;
    }
    else
    {
        *array = realloc(*array, element_size * ++*array_len);
        memcpy(*array + element_size * (*array_len - 1), value, element_size);
        qsort(*array, *array_len, element_size, compare);
        on_insert(bsearch(value, *array, *array_len, element_size, compare), element_size, value);
        return;
    }
}

static const int *result_element_int;

static void list_on_find(void *const list_element, __attribute__((unused)) const size_t element_size, __attribute__((unused)) const void *const value)
{
    result_element_int = list_element;
}

static void list_on_insert(void *const list_element, const size_t element_size, const void *const value)
{
    memcpy(list_element, value, element_size);
    result_element_int = list_element;
}

static inline void test_method_int(const char *const identifier, sorted_find_insert method, size_t list_len, const int value, const int *const expected_result, const size_t expected_result_len, const size_t expected_index)
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

    method((void *) &list, &list_len, sizeof(int), &value, list_on_find, list_on_insert, compare_ints);

    struct timeval end_tval;
    gettimeofday(&end_tval, NULL);

    const int correct_index = (expected_index == (size_t) (result_element_int - list));

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

static void test_cases_int(const char *const method_identifier, sorted_find_insert method)
{
    printf(
        "\n%s:\n"
        "                  Identifier |       Time | Contents Status | Index Status\n"
        "-----------------------------+------------+-----------------+-------------\n"
        ,
        method_identifier
    );

    test_method_int(        "Find Start", method, 4,   0, (int[]){ 0, 4, 8, 12 },      4, 0);
    test_method_int(       "Find Middle", method, 4,   8, (int[]){ 0, 4, 8, 12 },      4, 2);
    test_method_int(          "Find End", method, 4,  12, (int[]){ 0, 4, 8, 12 },      4, 3);
    test_method_int(        "Insert Pre", method, 4, -10, (int[]){ -10, 0, 4, 8, 12 }, 5, 0);
    test_method_int( "Insert Middle Odd", method, 4,   5, (int[]){ 0, 4, 5, 8, 12 },   5, 2);
    test_method_int("Insert Middle Even", method, 4,   6, (int[]){ 0, 4, 6, 8, 12 },   5, 2);
    test_method_int(       "Insert Post", method, 4,  13, (int[]){ 0, 4, 8, 12, 13 },  5, 4);
    test_method_int(             "Empty", method, 0,  32, (int[]){ 32 },               1, 0);

    int big_array[BIG_ARRAY_LEN + 1] = { 0 };
    for (size_t i = 0; i < 212122; ++i)
    {
        big_array[i] = i * 4;
    }
    big_array[212122] = 848487;
    for (size_t i = 212123; i <= BIG_ARRAY_LEN; ++i)
    {
        big_array[i] = (i - 1) * 4;
    }
    test_method_int("Big Array Insert Middle Odd", method, BIG_ARRAY_LEN, 848487, big_array, BIG_ARRAY_LEN + 1, 212122);
    big_array[212122] = 848486;
    test_method_int("Big Array Insert Middle Even", method, BIG_ARRAY_LEN, 848486, big_array, BIG_ARRAY_LEN + 1, 212122);
    big_array[212122] = 192000;
    for (size_t i = 48001; i <= BIG_ARRAY_LEN; ++i)
    {
        big_array[i] = i * 4;
    }
    test_method_int("Big Array Find Middle", method, BIG_ARRAY_LEN, 192000, big_array, BIG_ARRAY_LEN, 48000);
}

int main()
{
    test_cases_int(       "Linear", sorted_find_insert_linear);
    test_cases_int(       "Binary", sorted_find_insert_binary);
    test_cases_int("Bsearch Qsort", sorted_find_insert_bsearchqsort);
}
