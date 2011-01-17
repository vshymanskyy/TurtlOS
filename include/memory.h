#ifndef INC_MEMORY_H
#define INC_MEMORY_H

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

void *malloc_debug (size_t size, const char* file, int line);

#ifdef DEBUG
	#define malloc(size) malloc_debug(size, __FILE__, __LINE__)
#endif

/********************************************************
*  Operations
********************************************************/
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


#ifdef __cplusplus
}
#endif

#endif /* INC_MEMORY_H */
