#ifndef STRING_H
#define STRING_H

unsigned int strlen(char* str);
void itoa(int n, char* str, int radix);
int strcmp(char* str1, char* str2);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
char* strncpy(char* dst, const char* src, unsigned int n);
char* strncat(char* s1, const char* s2, unsigned int n);

#endif