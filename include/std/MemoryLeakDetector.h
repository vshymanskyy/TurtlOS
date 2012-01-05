/**
 * @file MemoryLeakDetector.cpp
 * Contains declaration of memory leak detector
 */

#if !defined(MEMORY_LEAK_DETECTOR) && defined(_DEBUG)
#define MEMORY_LEAK_DETECTOR

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// Standard C functions
extern "C"
{
	void* _mld_malloc(size_t aSize, const char* aFile, int aLine);
	void* _mld_calloc(size_t aNum, size_t aSize, const char* aFile, int aLine);
	void* _mld_realloc(void* aPtr, size_t aSize, const char* aFile, int aLine);
	void  _mld_free(void* aPtr, const char* aFile, int aLine);

#if !__STDC__
	char* _mld_strdup(const char* s, const char* aFile, int aLine);
#endif

	void _mld_dump();
}

#ifdef __cplusplus
// C++ global allocator/deallocator operators
extern "C++"
{
	void* operator new(size_t aSize, const char* aFile, int aLine);
	void* operator new[](size_t aSize, const char* aFile, int aLine);
	void  operator delete(void* aPtr, const char* aFile, int aLine);
	void  operator delete[](void* aPtr, const char* aFile, int aLine);
	void  operator delete(void* aPtr);
	void  operator delete[](void* aPtr);
}
#endif

#ifndef MLD_INTERNAL_LOCK
	// This should be checked to disable overloaded new and delete operators
	#define DMALLOC

	// Redefine functions to get file and line info about allocation
	#define malloc(aSize) _mld_malloc(aSize, __FILE__, __LINE__)
	#define calloc(aNum, aSize) _mld_calloc(aNum, aSize, __FILE__, __LINE__)
	#define realloc(aPtr, aSize) _mld_realloc(aPtr, aSize, __FILE__, __LINE__)
	#define free(aPtr) _mld_free(aPtr, __FILE__, __LINE__)
	#define strdup(aStr) _mld_strdup(aStr, __FILE__, __LINE__)
	#define _strdup(aStr) _mld_strdup(aStr, __FILE__, __LINE__)

	#define new new(__FILE__, __LINE__)
#endif

#endif // MEMORY_LEAK_DETECTOR
