#ifndef INC_STRING_H
#define INC_STRING_H

#if _MSC_VER > 1000
	#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

char *strlast (char *s);
char *strrev (char *s);
unsigned strlen (const char *s);
unsigned strnlen (const char *s, unsigned qty);
char *strcpy (char *s1, const char *s2);
char *strdup (const char *s);
char *strcat (char *s1, const char *s2);
char *strchr (const char *s, int c);
char *strrchr (const char *s, int c);
char *strpadr (char *s, const size_t fl, const char c);
bool strlike (char *str, char *mask);
char *strtok (char *s1, const char *s2);
char *strstr (const char *s1, const char *s2);
char *strncpy (char *dest, const char *source, unsigned qty);
int strcmp (const char *s1, const char *s2);
int stricmp (const char *s1, const char *s2);
int strncmp(const char* s1, const char* s2, int n);
int strnicmp(const char* s1, const char* s2, int n);
int vsnprintf(char* buffer, size_t length, const char* format, va_list args);
int snprintf(char* buffer, size_t qty, const char* format, ...);

long strtol (const char *buf, char **endptr, int base);
char *itoa (int value, char *buf, int base);

#ifdef __cplusplus
}
#endif

#endif /* INC_STRING_H */
