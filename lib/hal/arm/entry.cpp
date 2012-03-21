#include <assert.h>
#include <cpu_structs.h>

extern "C"
void
cpuStop()
{
	for(;;) {
	}
}

static const char* a = "Hello world!\n";

class A {
public:
	A() {
		debug_puts("Created!\n");
	}

	void print() {
		debug_puts(a);
	}

	~A() {
		debug_puts("Destroyed!\n");
	}
};

A aasd;

extern "C"
void
main()
{
	aasd.print();
	cpuStop();
}

extern "C"
void
free(void* p)
{
}

extern "C"
void*
malloc(size_t size)
{
	return NULL;
}

extern "C"
void*
mallocat(void* p ,size_t size)
{
	return NULL;
}

extern "C"
void*
calloc(size_t n, size_t size)
{
	return NULL;
}

extern "C"
void*
realloc(void* p, size_t size)
{
	return NULL;
}
