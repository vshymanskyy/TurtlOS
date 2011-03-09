#ifndef INC_STDLIB_H
#define INC_STDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************
*  Heap
********************************************************/
void free (void *p);
void *malloc (size_t size);
void *mallocat (void *p, size_t size);
void *calloc (size_t n, size_t size);
void *realloc (void *p, size_t size);
void *aligned_alloc (size_t size, size_t alignment);
void aligned_free (void *ptr);


void __attribute__((noreturn)) abort();

long strtol (const char *buf, char **endptr, int base);
char *itoa (int value, char *buf, int base);

#ifdef __cplusplus
}
#endif

#endif /* INC_STDLIB_H */
