#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#ifndef NULL
#define NULL ((void *)0)
#endif

int str_compare(const char *str1, const char *str2);
int str_length(const char *str);
void str_copy(char *dest, const char *src, int max_len);
void str_concat(char *dest, const char *src, int max_len);
char *str_find(const char *haystack, const char *needle);
void mem_set(void *ptr, int value, int num);
void mem_copy(void *dest, const void *src, int num);
int mem_compare(const void *s1, const void *s2, int num);

#endif // STRING_UTILS_H