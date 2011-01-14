#ifndef NEW_H
#define NEW_H

#ifdef __cplusplus
#include <stddef.h>

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

#endif

#endif // NEW_H
