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

// Copies up to `n` characters from `src` to `dest`
char *strncpy_m(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    while (i < n) {
        dest[i++] = '\0';
    }
    return dest;
}

// Joins an array of strings `strs` with the given delimiter `sep`
char *join_m(const char **strs, size_t num_strs, char sep) {
    if (num_strs == 0) return NULL;
    size_t total_length = 0;
    for (size_t i = 0; i < num_strs; i++) {
        total_length += strlen_m(strs[i]);
    }
    total_length += (num_strs - 1) + 1; // for separators and null terminator
    char *result = malloc(total_length);
    if (!result) return NULL;
    size_t pos = 0;
    for (size_t i = 0; i < num_strs; i++) {
        size_t len = strlen_m(strs[i]);
        strncpy_m(result + pos, strs[i], len);
        pos += len;
        if (i < num_strs - 1) {
            result[pos++] = sep;
        }
    }
    result[pos] = '\0';
    return result;
}

// Frees an array of strings `strs`
void free_strings(char **strs, size_t num_strs) {
    for (size_t i = 0; i < num_strs; i++) {
        free(strs[i]);
    }
    free(strs);
}

// Splits a string `s` into an array using the delimiter `delim`
char **split_m(const char *s, char delim, size_t *num_tokens) {
    *num_tokens = 0;
    size_t length = strlen_m(s);
    for (size_t i = 0; i < length; i++) {
        if (s[i] == delim) (*num_tokens)++;
    }
    (*num_tokens)++;
    char **result = malloc((*num_tokens) * sizeof(char *));
    if (!result) return NULL;
    size_t start = 0, token_idx = 0;
    for (size_t i = 0; i <= length; i++) {
        if (s[i] == delim || s[i] == '\0') {
            size_t token_length = i - start;
            result[token_idx] = malloc(token_length + 1);
            if (!result[token_idx]) {
                free_strings(result, token_idx);
                return NULL;
            }
            strncpy_m(result[token_idx], s + start, token_length);
            result[token_idx][token_length] = '\0';
            start = i + 1;
            token_idx++;
        }
    }
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
