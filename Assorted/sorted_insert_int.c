// Linux only example
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

void list_on_find(int *const list_element, const int value)
{
    *list_element = value;
    printf("Found %d, changing to 12345\n", value);
    *list_element = 12345;
}

void list_on_insert(int *const list_element, const int value)
{
    *list_element = value;
    printf("Inserted %d\n", *list_element);
}

static inline void test_method(const char *const identifier, sorted_find_insert_int method, size_t list_len, const int value)
{
    int *list = malloc(sizeof(int) * list_len);
    for (size_t i = 0; i < list_len; ++i)
    {
        list[i] = i * 4;
    }

    struct timeval start_tval;
    gettimeofday(&start_tval, NULL);

    method(&list, &list_len, value, list_on_find, list_on_insert);

    struct timeval end_tval;
    gettimeofday(&end_tval, NULL);

    const double total = (end_tval.tv_usec / 1000000.0 + end_tval.tv_sec) - (start_tval.tv_usec / 1000000.0 + start_tval.tv_sec);

    for (size_t i = 0; i < list_len; ++i)
    {
        printf("list[%zu]=%d\n", i, list[i]);
    }

    free(list);

    printf("Time taken for \"%s\": %lf\n", identifier, total);
}

int main()
{
    test_method("Linear", sorted_find_insert_int_linear, 0, 32);
    test_method("Binary", sorted_find_insert_int_binary, 5, 16);
}
