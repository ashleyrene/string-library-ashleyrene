#include <stdlib.h>
#include "stringm.h"

// Returns the length of the string `s`
size_t strlen_m(const char *s) {
    size_t length = 0;
    while (s[length] != '\0') {
        length++;
    }
    return length;
}

// Copies up to `n` characters from `string` to a newly allocated string
char *strncpy_m(const char *string, size_t n) {
    char *new_str = malloc(n + 1);
    if (!new_str) return NULL;
    size_t i;
    for (i = 0; i < n && string[i] != '\0'; i++) {
        new_str[i] = string[i];
    }
    while (i < n) {
        new_str[i++] = '\0';
    }
    new_str[n] = '\0';
    return new_str;
}

// Joins an array of strings `strings` using the given `delimiter`
char *join_m(Strings strings, const char *delimiter) {
    if (strings.count == 0) return NULL;
    size_t total_length = 0, delim_len = strlen_m(delimiter);
    for (size_t i = 0; i < strings.count; i++) {
        total_length += strlen_m(strings.data[i]);
    }
    total_length += (strings.count - 1) * delim_len + 1;
    char *result = malloc(total_length);
    if (!result) return NULL;
    size_t pos = 0;
    for (size_t i = 0; i < strings.count; i++) {
        size_t len = strlen_m(strings.data[i]);
        for (size_t j = 0; j < len; j++) {
            result[pos++] = strings.data[i][j];
        }
        if (i < strings.count - 1) {
            for (size_t j = 0; j < delim_len; j++) {
                result[pos++] = delimiter[j];
            }
        }
    }
    result[pos] = '\0';
    return result;
}

// Frees an array of strings `strings`
void free_strings(Strings strings) {
    for (size_t i = 0; i < strings.count; i++) {
        free(strings.data[i]);
    }
    free(strings.data);
}

// Splits a string `string` using the given `pattern`
Strings split_m(const char *string, const char *pattern) {
    Strings result = {NULL, 0};
    size_t pattern_len = strlen_m(pattern), count = 1;
    for (const char *p = string; (p = strstr_m(p, pattern)); p += pattern_len) {
        count++;
    }
    result.data = malloc(count * sizeof(char *));
    if (!result.data) return result;
    const char *start = string;
    size_t idx = 0;
    for (const char *p = string; (p = strstr_m(start, pattern)); start = p + pattern_len) {
        size_t len = p - start;
        result.data[idx] = strncpy_m(start, len);
        if (!result.data[idx]) {
            free_strings(result);
            return result;
        }
        idx++;
    }
    result.data[idx] = strdup(start);
    result.count = count;
    return result;
}

// Replaces all occurrences of `old_sub` with `new_sub` in `s`
char *find_and_replace_all_m(const char *s, const char *old_sub, const char *new_sub) {
    size_t old_len = strlen_m(old_sub), new_len = strlen_m(new_sub);
    size_t count = 0;
    for (const char *pos = strstr_m(s, old_sub); pos; pos = strstr_m(pos + old_len, old_sub)) {
        count++;
    }
    size_t new_size = strlen_m(s) + count * (new_len - old_len) + 1;
    char *result = malloc(new_size);
    if (!result) return NULL;
    char *res_ptr = result;
    while (*s) {
        if (strstr_m(s, old_sub) == s) {
            strncpy_m(res_ptr, new_sub, new_len);
            res_ptr += new_len;
            s += old_len;
        } else {
            *res_ptr++ = *s++;
        }
    }
    *res_ptr = '\0';
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
