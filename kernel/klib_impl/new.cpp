#include <new.h>
#include <stdlib.h>

#undef new

void* operator new (size_t size) {
	return malloc(size);
}

void* operator new (size_t size, void* at) {
	return mallocat(at, size);
}

void* operator new[] (size_t size) {
	return malloc(size);
}

void operator delete (void* p) {
	free(p);
}

void operator delete[] (void* p) {
	free(p);
}

