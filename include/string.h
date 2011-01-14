#ifndef STRING_H
#define STRING_H

#ifdef __cplusplus
extern "C" {
#endif

char *strlast (char *s);
char *strrev (char *s);
unsigned strlen (const char *s);
unsigned strnlen (const char *s, unsigned count);
char *strcpy (char *s1, const char *s2);
char *strdup (const char *s);
char *strcat (char *s1, const char *s2);
char *strchr (const char *s, int c);
char *strrchr (const char *s, int c);
char *strpadr (char *s, const size_t fl, const char c);
bool strlike (char *str, char *mask);
char *strtok (char *s1, const char *s2);
char *strstr (const char *s1, const char *s2);
char *strncpy (char *dest, const char *source, unsigned count);
int strcmp (const char *s1, const char *s2);
int stricmp (const char *s1, const char *s2);
int strncmp(const char* s1, const char* s2, int n);
int strnicmp(const char* s1, const char* s2, int n);
int vsnprintf(char* buffer, unsigned count, const char* format, va_list args);
int snprintf(char* buffer, unsigned count, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif // STRING_H
