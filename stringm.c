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
    {
        length++;
    }
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
    char *copy = (char *)malloc(n + 1);  // +1 for null terminator
    if (!copy)
    {
        return NULL;  // In case malloc fails
    }

    for (size_t i = 0; i < n; i++)
    {
        copy[i] = string[i];
    }
    copy[n] = '\0';  // Null-terminate the copied string
    return copy;
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
    {
        return NULL;
    }

    size_t total_length = 0;
    for (size_t i = 0; i < strings.num_strings; i++)
    {
        total_length += strlen_m(strings.strings[i]);
    }
    total_length += strlen_m(delimiter) * (strings.num_strings - 1) + 1; // Add for delimiters and null terminator

    char *result = (char *)malloc(total_length);
    if (!result)
    {
        return NULL;  // In case malloc fails
    }

    size_t current_pos = 0;
    for (size_t i = 0; i < strings.num_strings; i++)
    {
        size_t str_len = strlen_m(strings.strings[i]);
        for (size_t j = 0; j < str_len; j++)
        {
            result[current_pos++] = strings.strings[i][j];
        }

        if (i < strings.num_strings - 1)
        {
            size_t delim_len = strlen_m(delimiter);
            for (size_t k = 0; k < delim_len; k++)
            {
                result[current_pos++] = delimiter[k];
            }
        }
    }
    result[current_pos] = '\0';  // Null-terminate the resulting string
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
    free(strings.strings);  // Free the array itself
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
**         - what is the outcome of adding or subtract pointers?
** hint 3.5: strstr_m will return a pointer to the first character of the next occurrence 
**           or NULL if not found
*/
Strings split_m(const char *string, const char *pattern)
{
    Strings result = { .num_strings = 0, .strings = NULL };
    const char *start = string;
    const char *next = strstr_m(start, pattern);

    while (next)
    {
        size_t length = next - start;
        result.strings = realloc(result.strings, (result.num_strings + 1) * sizeof(char *));
        result.strings[result.num_strings] = (char *)malloc(length + 1);
        strncpy_m(start, length); // Copy the substring to the result
        result.num_strings++;

        start = next + strlen_m(pattern);
        next = strstr_m(start, pattern);
    }

    if (start && *start != '\0') // Add the final part after the last delimiter
    {
        result.strings = realloc(result.strings, (result.num_strings + 1) * sizeof(char *));
        result.strings[result.num_strings] = strdup(start);
        result.num_strings++;
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
    size_t string_len = strlen_m(string);
    size_t pattern_len = strlen_m(pattern);
    size_t replacement_len = strlen_m(replacement);

    // First pass: count how many replacements we need
    size_t count = 0;
    const char *temp = string;
    while ((temp = strstr_m(temp, pattern)) != NULL)
    {
        count++;
        temp += pattern_len;
    }

    // Create a new string of appropriate size
    size_t new_len = string_len + count * (replacement_len - pattern_len);
    char *result = (char *)malloc(new_len + 1);  // +1 for null terminator
    if (!result)
    {
        return NULL;
    }

    size_t result_pos = 0;
    const char *start = string;
    while ((temp = strstr_m(start, pattern)) != NULL)
    {
        size_t prefix_len = temp - start;
        strncpy_m(start, prefix_len);
        memcpy(result + result_pos, start, prefix_len);
        result_pos += prefix_len;

        // Copy replacement string
        memcpy(result + result_pos, replacement, replacement_len);
        result_pos += replacement_len;

        start = temp + pattern_len;
    }

    // Copy remaining part of string
    strcpy(result + result_pos, start);

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
