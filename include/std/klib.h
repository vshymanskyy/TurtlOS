#ifndef _INC_KLIB_H
#define _INC_KLIB_H

//*******************************************************
//  New
//*******************************************************
void* operator new (size_t size);
void* operator new (size_t size, void* at);
void* operator new[] (size_t size);
void operator delete (void* p);
void operator delete[] (void* p);

#ifdef DEBUG
	void* operator new (size_t size, const char* file, int line);
	void* operator new[] (size_t size, const char* file, int line);
	#define new new(__FILE__, __LINE__)
#endif

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
//  Debug
//*******************************************************
void debug_print(const char* format, ...);
void __attribute__((noreturn)) assert_failed (const char *msg, const char *file, const int line, const char *func);
void __attribute__((noreturn)) fatal_soft (const char *msg, const char *file, const int line, const char *func);

#define fatal(msg)		fatal_soft(msg, __FILE__, __LINE__, __FUNC__);

#ifdef DEBUG
	#define assert(msg) {													\
		if (!(msg)) {														\
			assert_failed(__string(msg), __FILE__, __LINE__, __FUNC__);		\
		}																	\
	}
#else
	#define assert(msg)
#endif

//*******************************************************
//  StdArg
//*******************************************************
typedef char* va_list;
#define va_round(n)			((sizeof(n) + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1))
#define va_start(ap, v)		(ap = (va_list)&v + va_round(v))
#define va_arg(ap, t)		(*(t*)((ap += va_round(t)) - va_round(t)))
#define va_end(ap)			(ap = (va_list)0)

//*******************************************************
//  String
//*******************************************************
char *strlast (char *s);
char *strrev (char *s);
unsigned strlen (const char *s);
unsigned strnlen (const char *s, unsigned count);
char *strcpy (char *s1, const char *s2);
char *strdup (const char *s);
int strcmp (const char *s1, const char *s2);
char *strcat (char *s1, const char *s2);
char *strchr (const char *s, int c);
char *strrchr (const char *s, int c);
char *strpadr (char *s, const size_t fl, const char c);
bool strlike (char *str, char *mask);
char *strtok (char *s1, const char *s2);
char *strstr (const char *s1, const char *s2);
char *strncpy (char *dest, const char *source, unsigned count);
int stricmp (const char *s1, const char *s2);
int vsnprintf(char* buffer, unsigned count, const char* format, va_list args);
int snprintf(char* buffer, unsigned count, const char* format, ...);

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
