/*
** -----------------------------------------------------------------
** IMPORTANT NOTE: For this assignment, you are not allowed to use
** string.h or any header file other than what is defined below
** -----------------------------------------------------------------
*/
#include "stringm.h"

/*
** strlen_m calculates the length of a string
** const char *string - string to calculate length of
** return the size of the string
**
** note: you can assume string is not NULL
*/
size_t strlen_m(const char *string)
{
    size_t length = 0;
    while (string[length] != '\0')
        length++;
    return length;
}

/*
** strncpy_m copies n characters of string and returns it
** const char *string - string to copy
** size_t n - number of characters to copy (not including null character)
** return a copy of first n characters of string
**
** note: you can assume string is not NULL
** hint: you will need to malloc a size n + 1 string to accommodate the null character
*/
char *strncpy_m(const char *string, size_t n)
{
    char *copy = malloc(n + 1);  // Allocate memory for n + 1 characters
    if (copy == NULL)
        return NULL;

    for (size_t i = 0; i < n; i++)
        copy[i] = string[i];

    copy[n] = '\0';  // Null-terminate the copied string
    return copy;
}

/*
** memcpy_m copies n bytes of data from source to destination
** void *dest - destination memory location
** const void *src - source memory location
** size_t n - number of bytes to copy
** return a pointer to the destination
*/
void *memcpy_m(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    for (size_t i = 0; i < n; i++)
        d[i] = s[i];

    return dest;
}

/*
** join_m joins an array of strings separated by a delimiter
** Strings strings - structure that stores an array of strings
** const char *delimiter - delimiter string which joins each string
** return the string created by joining all strings with the delimiter
**
** note: you can assume delimiter is not NULL
** hint: return NULL if strings.num_strings is 0
*/
char *join_m(Strings strings, const char *delimiter)
{
    if (strings.num_strings == 0)
        return NULL;

    // Calculate the length of the resulting string
    size_t total_length = 0;
    for (size_t i = 0; i < strings.num_strings; i++)
    {
        total_length += strlen_m(strings.strings[i]);
        if (i < strings.num_strings - 1)
            total_length += strlen_m(delimiter);  // Add delimiter length
    }

    // Allocate memory for the resulting string
    char *result = malloc(total_length + 1);
    if (result == NULL)
        return NULL;

    size_t pos = 0;
    for (size_t i = 0; i < strings.num_strings; i++)
    {
        size_t len = strlen_m(strings.strings[i]);
        for (size_t j = 0; j < len; j++)
            result[pos++] = strings.strings[i][j];

        if (i < strings.num_strings - 1)
        {
            size_t delim_len = strlen_m(delimiter);
            for (size_t j = 0; j < delim_len; j++)
                result[pos++] = delimiter[j];
        }
    }

    result[pos] = '\0';  // Null-terminate the result
    return result;
}

/*
** free_strings frees all allocated elements in strings
** String strings - free each string in strings.strings and strings.strings itself
*/
void free_strings(Strings strings)
{
    for (size_t i = 0; i < strings.num_strings; i++)
    {
        free(strings.strings[i]);
    }
    free(strings.strings);
}

/*
** split_m splits a string at any occurrence of pattern
** const char *string - string that is searched for the pattern
** const char *pattern - pattern which string should be split
** return a String structure which contains an array of each string
**
** note: you may assume string and pattern are not NULL
** hint 1: TA solution uses strlen_m, strstr_m, and strncpy_m
** hint 2: first calculate how many strings are needed, which is: 
**         (the number of times the delimiter appears + 1)
** hint 3: when trying to store a substring, think about how the length of 
**         that substring might be calculated in terms of pointer arithmetic
**         - what is the outcome of adding or subtracting pointers?
** hint 3.5: strstr_m will return a pointer to the first character of the next occurrence 
**           or NULL if not found
**          
*/
Strings split_m(const char *string, const char *pattern)
{
    Strings result = { .num_strings = 0, .strings = NULL };
    size_t pattern_len = strlen_m(pattern);
    const char *start = string;

    // Count how many substrings we need
    while ((start = strstr_m(start, pattern)) != NULL)
    {
        result.num_strings++;
        start += pattern_len;  // Move past the pattern
    }

    // Allocate memory for the result
    result.strings = malloc(result.num_strings * sizeof(char *));
    if (result.strings == NULL)
        return result;

    // Extract the substrings
    start = string;
    size_t result_pos = 0;
    while ((start = strstr_m(start, pattern)) != NULL)
    {
        size_t prefix_len = start - string;  // Length of the substring before the pattern
        result.strings[result_pos++] = strncpy_m(string, prefix_len);
        start += pattern_len;  // Move past the pattern
    }

    return result;
}

/*
** find_and_replace_all_m finds each occurrence of the pattern in the string and replaces it
** const char *string - string to search through
** const char *pattern - pattern to search for in the string
** const char *replacement - replacement string for each occurrence of the pattern in the string
** return a string in which every occurrence of pattern is replaced with replacement
**
** note: you may assume string, pattern, and replacement are all not NULL
** hint: there are two main ways of implementing this function, one involves many lines, one involves four
*/
char *find_and_replace_all_m(const char *string, const char *pattern, const char *replacement)
{
    size_t pattern_len = strlen_m(pattern);
    size_t replacement_len = strlen_m(replacement);
    size_t total_length = 0;
    const char *start = string;

    // Calculate the length of the resulting string
    while ((start = strstr_m(start, pattern)) != NULL)
    {
        total_length += replacement_len;
        start += pattern_len;
    }

    // Allocate memory for the result
    char *result = malloc(total_length + strlen_m(string) + 1);
    if (result == NULL)
        return NULL;

    size_t pos = 0;
    start = string;
    while ((start = strstr_m(start, pattern)) != NULL)
    {
        // Copy text before the pattern
        size_t prefix_len = start - string;
        memcpy_m(result + pos, string, prefix_len);
        pos += prefix_len;

        // Copy the replacement string
        memcpy_m(result + pos, replacement, replacement_len);
        pos += replacement_len;

        start += pattern_len;
    }

    result[pos] = '\0';  // Null-terminate the result
    return result;
}

/*
** The strstr function is implemented for you to use -- DO NOT MODIFY
** If you are curious about the algorithm used, look up the Knuth-Morris-Pratt (KMP)
** algorithm that can find a substring inside another string 'blazingly fast'
*/
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
