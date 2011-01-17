#include <new.h>
#include <memory.h>

#undef new

#ifdef DEBUG
void* operator new (size_t size, const char* file, int line) {
	return malloc_debug(size, file, line);
}

void* operator new[] (size_t size, const char* file, int line) {
	return malloc_debug(size, file, line);
}
#endif

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

