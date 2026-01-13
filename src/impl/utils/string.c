#include "string.h"

#ifndef NULL
// #define NULL ((void *)0)
#endif

int str_compare(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int str_length(const char *str)
{
    if (!str)
        return 0;

    int len = 0;
    while (*str++)
    {
        len++;
    }
    return len;
}

void str_copy(char *dest, const char *src, int max_len)
{
    if (!dest || !src || max_len <= 0)
        return;

    int i = 0;
    while (src[i] && i < max_len - 1)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void str_concat(char *dest, const char *src, int max_len)
{
    if (!dest || !src || max_len <= 0)
        return;

    int dest_len = str_length(dest);
    int i = 0;

    while (src[i] && (dest_len + i) < max_len - 1)
    {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';
}

char *str_find(const char *haystack, const char *needle)
{
    if (!haystack || !needle)
        return NULL;

    int needle_len = str_length(needle);
    if (needle_len == 0)
        return (char *)haystack;

    while (*haystack)
    {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && (*h == *n))
        {
            h++;
            n++;
        }

        if (!*n)
            return (char *)haystack;
        haystack++;
    }

    return NULL;
}

void mem_set(void *ptr, int value, int num)
{
    if (!ptr)
        return;

    unsigned char *p = (unsigned char *)ptr;
    unsigned char val = (unsigned char)value;

    for (int i = 0; i < num; i++)
    {
        p[i] = val;
    }
}

void mem_copy(void *dest, const void *src, int num)
{
    if (!dest || !src)
        return;

    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (int i = 0; i < num; i++)
    {
        d[i] = s[i];
    }
}

int mem_compare(const void *s1, const void *s2, int num)
{
    if (!s1 || !s2)
        return -1;

    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    for (int i = 0; i < num; i++)
    {
        if (p1[i] != p2[i])
        {
            return p1[i] - p2[i];
        }
    }

    return 0;
}
