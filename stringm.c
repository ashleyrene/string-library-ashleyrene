#include "stringm.h"

// Function to calculate the length of a string
size_t strlen_m(const char *string)
{
    size_t length = 0;
    while (string[length] != '\0')
    {
        length++;
    }
    return length;
}

// Function to copy n characters from one string to another
char *strncpy_m(const char *string, size_t n)
{
    // Allocate memory for the new string (+1 for the null terminator)
    char *copy = malloc(n + 1);
    if (copy == NULL)
    {
        return NULL;
    }

    // Copy characters from source string to the new string
    for (size_t i = 0; i < n; i++)
    {
        copy[i] = string[i];
    }
    // Null-terminate the new string
    copy[n] = '\0';
    return copy;
}

// Function to split a string by a delimiter (pattern)
Strings split_m(const char *string, const char *pattern)
{
    Strings result = {0};
    size_t pattern_len = strlen_m(pattern);

    // Edge case: if the pattern is an empty string, return an empty Strings struct
    if (pattern_len == 0)
    {
        return result;
    }

    const char *start = string;
    const char *match;
    size_t match_count = 0;

    // Count the number of substrings in the string by finding pattern occurrences
    while ((match = strstr_m(start, pattern)) != NULL)
    {
        match_count++;
        start = match + pattern_len;
    }

    // Allocate memory for the array of substrings
    result.strings = malloc((match_count + 1) * sizeof(char *));
    if (result.strings == NULL)
    {
        return result;
    }

    start = string;
    size_t index = 0;

    // Split the string into substrings
    while ((match = strstr_m(start, pattern)) != NULL)
    {
        size_t len = match - start;
        result.strings[index] = strncpy_m(start, len);
        index++;
        start = match + pattern_len;
    }

    // Add the last substring after the final delimiter
    result.strings[index] = strdup(start);
    result.num_strings = match_count + 1;

    return result;
}

// Function to join strings in the Strings struct with a delimiter
char *join_m(Strings strings, const char *delimiter)
{
    size_t delimiter_len = strlen_m(delimiter);
    size_t total_len = 0;

    // Calculate the total length of the resulting string
    for (int i = 0; i < strings.num_strings; i++)
    {
        total_len += strlen_m(strings.strings[i]) + delimiter_len;
    }
    total_len -= delimiter_len; // Remove the last delimiter

    // Allocate memory for the resulting string
    char *result = malloc(total_len + 1);
    if (result == NULL)
    {
        return NULL;
    }

    size_t pos = 0;
    for (int i = 0; i < strings.num_strings; i++)
    {
        size_t str_len = strlen_m(strings.strings[i]);
        memcpy(result + pos, strings.strings[i], str_len);
        pos += str_len;

        if (i < strings.num_strings - 1)
        {
            memcpy(result + pos, delimiter, delimiter_len);
            pos += delimiter_len;
        }
    }

    result[total_len] = '\0'; // Null-terminate the result string
    return result;
}

// Function to free the memory allocated for strings
void free_strings(Strings strings)
{
    for (int i = 0; i < strings.num_strings; i++)
    {
        free(strings.strings[i]);
    }
    free(strings.strings);
}

// Function to find and replace all occurrences of a pattern in a string
char *find_and_replace_all_m(const char *string, const char *pattern, const char *replacement)
{
    size_t pattern_len = strlen_m(pattern);
    size_t replacement_len = strlen_m(replacement);
    size_t total_len = 0;

    // First, calculate the total length for the new string
    const char *start = string;
    const char *match;
    while ((match = strstr_m(start, pattern)) != NULL)
    {
        total_len += match - start;
        total_len += replacement_len;
        start = match + pattern_len;
    }
    total_len += strlen_m(start);

    // Allocate memory for the new string
    char *result = malloc(total_len + 1);
    if (result == NULL)
    {
        return NULL;
    }

    size_t pos = 0;
    start = string;
    while ((match = strstr_m(start, pattern)) != NULL)
    {
        size_t len = match - start;
        memcpy(result + pos, start, len);
        pos += len;
        memcpy(result + pos, replacement, replacement_len);
        pos += replacement_len;
        start = match + pattern_len;
    }

    // Copy the remainder of the string after the last pattern occurrence
    memcpy(result + pos, start, strlen_m(start));
    result[total_len] = '\0';

    return result;
}

// Do NOT modify this function
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
