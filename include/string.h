#ifndef STRING_H
#define STRING_H
#include <stdbool.h>
#include "types.h"
#define swap(a, b) (a ^= b ^= a ^= b)
void *memset(void *dst, char c, uint32 n);
char *strstr(char *s1, const char *s2);
void *memcpy(void *dst, const void *src, uint32 n);
void parse_string(char *parser, char *string, char c);
int memcmp(uint8 *s1, uint8 *s2, uint32 n);
uint32_t atoi(const char *str);
int strlen(const char *s);

int strcmp(const char *s1, char *s2);

int strcpy(char *dst, const char *src);
char *ctos(char s[2], const char c);
void strcat(char *dest, const char *src);

int isspace(char c);

int isalpha(char c);
char upper(char c);
char lower(char c);
bool backspace(char buffer[]);

void itoa(char *buf, int base, int d);

int string_length(char s[]);
void append(char s[], char n);
#endif
