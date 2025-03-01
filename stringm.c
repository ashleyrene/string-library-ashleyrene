#include "stringm.h"

size_t strlen_m(const char *string)
{
    size_t length = 0;
    while (string[length] != '\0')
        length++;
    return length;
}

char *strncpy_m(const char *string, size_t n)
{
    char *copy = malloc(n + 1); // Allocate memory for n characters + null-terminator
    if (!copy) return NULL; // Check for malloc failure

    for (size_t i = 0; i < n; i++)
        copy[i] = string[i];
    copy[n] = '\0'; // Null-terminate the copied string

    return copy;
}

char *join_m(Strings strings, const char *delimiter)
{
    if (strings.num_strings == 0)
        return NULL; // If there are no strings, return NULL

    // Calculate the total length needed
    size_t total_length = 0;
    for (size_t i = 0; i < strings.num_strings; i++)
        total_length += strlen_m(strings.strings[i]);

    // Add space for delimiters between strings
    total_length += (strings.num_strings - 1) * strlen_m(delimiter);
    
    // Add space for the null-terminator
    total_length += 1;

    char *result = malloc(total_length);
    if (!result) return NULL; // Check for malloc failure

    size_t pos = 0;
    for (size_t i = 0; i < strings.num_strings; i++)
    {
        size_t len = strlen_m(strings.strings[i]);
        for (size_t j = 0; j < len; j++)
            result[pos++] = strings.strings[i][j];

        // Add delimiter if it's not the last string
        if (i < strings.num_strings - 1)
        {
            size_t delimiter_len = strlen_m(delimiter);
            for (size_t k = 0; k < delimiter_len; k++)
                result[pos++] = delimiter[k];
        }
    }

    result[pos] = '\0'; // Null-terminate the result
    return result;
}

void free_strings(Strings strings)
{
    for (size_t i = 0; i < strings.num_strings; i++)
        free(strings.strings[i]);
    free(strings.strings);
}

Strings split_m(const char *string, const char *pattern)
{
    Strings result = { .num_strings = 0, .strings = NULL };
    size_t pattern_len = strlen_m(pattern);

    const char *start = string;
    while (start)
    {
        const char *match = strstr_m(start, pattern);
        if (match)
        {
            size_t len = match - start;
            char *substring = strncpy_m(start, len);
            result.num_strings++;
            result.strings = realloc(result.strings, result.num_strings * sizeof(char *));
            result.strings[result.num_strings - 1] = substring;
            start = match + pattern_len;
        }
        else
        {
            // If no more matches are found, add the last substring
            result.num_strings++;
            result.strings = realloc(result.strings, result.num_strings * sizeof(char *));
            result.strings[result.num_strings - 1] = strdup(start); // Handle the final part of the string
            break;
        }
    }

    return result;
}

char *find_and_replace_all_m(const char *string, const char *pattern, const char *replacement)
{
    Strings parts = split_m(string, pattern);
    size_t replacement_len = strlen_m(replacement);

    // Calculate the total length for the new string
    size_t total_length = 0;
    for (size_t i = 0; i < parts.num_strings; i++)
        total_length += strlen_m(parts.strings[i]);

    total_length += (parts.num_strings - 1) * replacement_len;
    total_length += 1; // For null-terminator

    char *result = malloc(total_length);
    if (!result) return NULL;

    size_t pos = 0;
    for (size_t i = 0; i < parts.num_strings; i++)
    {
        size_t part_len = strlen_m(parts.strings[i]);
        for (size_t j = 0; j < part_len; j++)
            result[pos++] = parts.strings[i][j];

        if (i < parts.num_strings - 1) // Add replacement string between parts
        {
            for (size_t k = 0; k < replacement_len; k++)
                result[pos++] = replacement[k];
        }
    }

    result[pos] = '\0'; // Null-terminate the result
    free_strings(parts); // Free allocated parts

    return result;
}

const char *strstr_m(const char *haystack, const char *needle)
{
    size_t haystack_len = 0, needle_len = 0;
    for (const char *h = haystack; *h; h++)
        haystack_len++;
    for (const char *n = needle; *n; n++)
        needle_len++;

    if (needle_len > haystack_len)
        return NULL;

    char *lps_str = malloc(haystack_len + needle_len + 1);
    size_t i = 0;
    for (const char *n = needle; *n; n++, i++)
        lps_str[i] = *n;
    lps_str[i++] = '\1';
    for (const char *h = haystack; *h; h++, i++)
        lps_str[i] = *h;

    int *lps_arr = calloc((haystack_len + needle_len + 1), sizeof *lps_arr);
    size_t l = 0, r = 1;
    bool success = false;

    while (r < haystack_len + needle_len + 1)
    {
        if (lps_str[l] == lps_str[r])
        {
            l++;
            lps_arr[r] = l;
            r++;
        }
        else if (l)
            l = lps_arr[l - 1];
        else
        {
            lps_arr[r] = 0;
            r++;
        }

        if (l == needle_len)
        {
            success = true;
            break;
        }
    }

    free(lps_arr);
    free(lps_str);
    if (success)
        return haystack + (r - l - needle_len - 1);
    return NULL;
}
