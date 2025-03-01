#include "stringm.h"

// Custom implementation of strlen_m
size_t strlen_m(const char *string) {
    size_t len = 0;
    while (string[len] != '\0') {
        len++;
    }
    return len;
}

// Custom implementation of strncpy_m
char *strncpy_m(const char *string, size_t n) {
    char *copy = malloc(n + 1);  // Allocate space for the string plus null terminator
    if (copy == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        copy[i] = string[i];
    }
    copy[n] = '\0';  // Ensure null-termination

    return copy;
}

// Custom implementation of join_m
char *join_m(Strings strings, const char *delimiter) {
    if (strings.num_strings == 0) {
        return NULL;
    }

    size_t delimiter_len = strlen_m(delimiter);
    size_t total_length = 0;

    // Calculate total length required
    for (int i = 0; i < strings.num_strings; i++) {
        total_length += strlen_m(strings.strings[i]);
    }
    total_length += (strings.num_strings - 1) * delimiter_len;  // Space for delimiters

    char *result = malloc(total_length + 1);  // Plus one for the null terminator
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    for (int i = 0; i < strings.num_strings; i++) {
        size_t str_len = strlen_m(strings.strings[i]);
        for (size_t j = 0; j < str_len; j++) {
            result[pos++] = strings.strings[i][j];
        }

        // Add delimiter if it's not the last string
        if (i < strings.num_strings - 1) {
            for (size_t j = 0; j < delimiter_len; j++) {
                result[pos++] = delimiter[j];
            }
        }
    }

    result[pos] = '\0';  // Null-terminate the result string
    return result;
}

// Custom implementation of free_strings
void free_strings(Strings strings) {
    for (int i = 0; i < strings.num_strings; i++) {
        free(strings.strings[i]);
    }
    free(strings.strings);
}

// Custom implementation of split_m
Strings split_m(const char *string, const char *pattern) {
    Strings result = {0, NULL};

    size_t pattern_len = strlen_m(pattern);
    size_t string_len = strlen_m(string);

    size_t count = 1;  // There's at least one string in the result
    const char *current = string;

    // Count occurrences of the pattern
    while ((current = strstr_m(current, pattern)) != NULL) {
        count++;
        current += pattern_len;
    }

    result.strings = malloc(count * sizeof(char *));
    if (result.strings == NULL) {
        return result;
    }

    current = string;
    size_t index = 0;

    // Split the string
    while ((current = strstr_m(current, pattern)) != NULL) {
        size_t len = current - string;
        result.strings[index] = malloc(len + 1);
        if (result.strings[index] == NULL) {
            free_strings(result);  // Free allocated strings
            result.strings = NULL;
            return result;
        }
        strncpy_m(result.strings[index], string, len);
        result.strings[index][len] = '\0';
        index++;

        current += pattern_len;
        string = current;
    }

    // Handle the final part after the last delimiter
    result.strings[index] = malloc(string_len - (current - string) + 1);
    if (result.strings[index] != NULL) {
        strncpy_m(result.strings[index], string, string_len - (current - string));
        result.strings[index][string_len - (current - string)] = '\0';
    }

    result.num_strings = count;
    return result;
}

// Custom implementation of find_and_replace_all_m
char *find_and_replace_all_m(const char *string, const char *pattern, const char *replacement) {
    size_t string_len = strlen_m(string);
    size_t pattern_len = strlen_m(pattern);
    size_t replacement_len = strlen_m(replacement);

    size_t result_len = string_len;
    const char *current = string;

    // Calculate the new string length after replacements
    while ((current = strstr_m(current, pattern)) != NULL) {
        result_len += replacement_len - pattern_len;
        current += pattern_len;
    }

    char *result = malloc(result_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t pos = 0;
    current = string;
    while ((current = strstr_m(current, pattern)) != NULL) {
        size_t len = current - string;
        strncpy_m(result + pos, string, len);
        pos += len;
        strncpy_m(result + pos, replacement, replacement_len);
        pos += replacement_len;

        current += pattern_len;
        string = current;
    }

    strcpy(result + pos, string);  // Copy the remaining part of the string
    return result;
}

// This function is already implemented for you, do not modify
const char *strstr_m(const char *haystack, const char *needle) {
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
