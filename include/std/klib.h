#ifndef _INC_KLIB_H
#define _INC_KLIB_H

extern "C" {

//*******************************************************
//  Memory
//*******************************************************
void free (void *p);
void *malloc (size_t size);
void *mallocat (void *p, size_t size);
void *calloc (size_t n, size_t size);
void *realloc (void *p, size_t size);
void *aligned_alloc (size_t size, size_t alignment);
void aligned_free (void *ptr);

void *malloc_debug (size_t size, const char* file, int line);

#ifdef DEBUG
	#define malloc(size) malloc_debug(size, __FILE__, __LINE__)
#endif

//*******************************************************
//  Memory
//*******************************************************
void *memrev (void *b, void *e);
char memsum (void *const s, size_t n);
void *memcpy (void *dest, const void *src, size_t count);
void *memset (void *dest, uint8_t val, size_t count);
void *memsetw (void *dest, uint16_t val, size_t count);
void *memsetd (void *dest, uint32_t val, size_t count);
void *memzero (void *dest, size_t count);
void *memmove (void *dest, const void *src, size_t count);
int memcmp (const void *s1, const void *s2, size_t n);
void swab (const void *src, void *dest, size_t n);

//*******************************************************
//  CType
//*******************************************************
int isalpha (int c);
int isupper (int c);
int islower (int c);
int isdigit (int c);
int isxdigit (int c);
int isspace (int c);
int ispunct (int c);
int isalnum (int c);
int isprint (int c);
int isgraph (int c);
int iscntrl (int c);
int toupper (int c);
int tolower (int c);

long strtol (const char *buf, char **endptr, int base);
char *itoa (int value, char *buf, int base);

}

#endif // _INC_KLIB_H
