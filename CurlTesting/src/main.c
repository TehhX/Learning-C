#include "curl/curl.h"
#include "stdlib.h"
#include "string.h"

struct string
{
    char *contents;
    size_t len;
};

// Same prototype as fwrite
static size_t curl_write_func(void *input, size_t size, size_t count, void *void_string)
{
    struct string *const output_string = void_string;

    const size_t
        old_len = output_string->len,
        new_len = old_len + count
    ;

    output_string->contents = realloc(output_string->contents, new_len);
    output_string->len = new_len;

    memcpy(output_string->contents + old_len, input, count);

    return count;
}

int main()
{
    CURL *const curl = curl_easy_init();

    if (!curl)
    {
        fputs("Failed Initializing Curl\n", stderr);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "example.com");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

    struct string output_string = { NULL, 0 };
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output_string);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_func);

    const CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "Could Not Fetch Webpage\nError: %s\n", curl_easy_strerror(res));
        return 1;
    }

    // Allocate space for, add null terminator
    (output_string.contents = realloc(output_string.contents, output_string.len + 1))[output_string.len] = '\0';

    // Add a newline if one is not present
    printf("%s%s", output_string.contents, (output_string.contents[output_string.len - 1] == '\n' ? "" : "\n"));

    free(output_string.contents);
    curl_easy_cleanup(curl);
}
