#include <std/klib.h>
#include "../Heap.h"

Heap heap((void*)0x200000, 0x1000000);

void
free(void* p)
{
	heap.Free(p);
}

#ifdef DEBUG
void*
malloc_debug(size_t size, const char* file, int line)
{
	return heap.AllocateDebug(size, file, line);
}
#else

void*
malloc(size_t size)
{
	return heap.Allocate(size);
}
#endif


void*
mallocat(void* p ,size_t size)
{
	return heap.AllocateAt(p, size);
}

void*
calloc(size_t n, size_t size)
{
	void* p = heap.Allocate(n * size);
	if (p) {
		memzero(p, n * size);
	}
	return p;
}

void*
realloc(void* p, size_t size)
{
	return heap.Reallocate(p, size);
}

void*
aligned_alloc(size_t size, size_t alignment)
{
	alignment--;
	void* const pa=malloc((size+alignment)+sizeof(void*));
	if (!pa) {
		return NULL;
	}
	void* const ptr=(void*)(((size_t)pa+sizeof(void*)+alignment)&~alignment);
	*((void**)ptr-1)=pa;
	return ptr;
}

void
aligned_free(void* ptr)
{
	if (ptr) {
		free(*((void**)ptr-1));
	}
}

