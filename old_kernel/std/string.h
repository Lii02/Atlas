#ifndef STRING_H
#define STRING_H

unsigned int strlen(char* str);
void itoa(int n, char* str, int radix);
int strcmp(char* str1, char* str2);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
char* strchr(const char* p, int ch);

#endif