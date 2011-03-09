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
int strlike (char *str, char *mask);
char *strtok (char *s1, const char *s2);
char *strstr (const char *s1, const char *s2);
char *strncpy (char *dest, const char *source, unsigned qty);
int strcmp (const char *s1, const char *s2);
int stricmp (const char *s1, const char *s2);
int strncmp(const char* s1, const char* s2, int n);
int strnicmp(const char* s1, const char* s2, int n);

/********************************************************
*  Operations
********************************************************/
void *memrev (void *b, void *e);
char memsum (void *const s, size_t n);
void *memcpy (void *dest, const void *src, size_t qty);
void *memset (void *dest, uint8_t val, size_t qty);
void *memsetw (void *dest, uint16_t val, size_t qty);
void *memsetd (void *dest, uint32_t val, size_t qty);
void *memzero (void *dest, size_t qty);
void *memmove (void *dest, const void *src, size_t qty);
int memcmp (const void *s1, const void *s2, size_t n);
void swab (const void *src, void *dest, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* INC_STRING_H */
