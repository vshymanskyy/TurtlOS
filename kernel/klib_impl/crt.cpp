#include <std/klib.h>
#include <linker.h>
#include <hal/cpu.h>
#include "../Heap.h"

#define MAX_ATEXIT_HANDLERS 128

#ifdef DEBUG
extern int SmartPtrGlobalCounter;
extern Heap heap;
#endif

extern int main();

struct atexit_func {
	void (*f)(void*);
	void* p;
	void* d;
} PACKED;

static atexit_func object[MAX_ATEXIT_HANDLERS];
static unsigned objectQty;

void* __dso_handle;
void* __stack_chk_guard;

extern "C" {

	int __cxa_atexit(void(*f)(void*), void* p, void* d);
	void __cxa_finalize(void* d);
	void __cxa_pure_virtual();
	void __stack_chk_guard_setup();
	void __stack_chk_fail();

	void entry();

	namespace __cxxabiv1 {
		__extension__ typedef int __guard __attribute__((mode (__DI__)));

		int __cxa_guard_acquire (__guard* g);
		void __cxa_guard_release (__guard* g);
		void __cxa_guard_abort (__guard*);
	}
}

int
__cxxabiv1::__cxa_guard_acquire (__guard* g)
{
	return !*(char*)(g);
}

void
__cxxabiv1::__cxa_guard_release (__guard* g)
{
	*(char*)g = 1;
}

void
__cxxabiv1::__cxa_guard_abort (__guard*)
{
}

int
__cxa_atexit (void (*f)(void*), void* p, void* d)
{
	assert(objectQty < MAX_ATEXIT_HANDLERS);
	object[objectQty].f = f;
	object[objectQty].p = p;
	object[objectQty].d = d;
	++objectQty;
	return 0;
}

void
__cxa_finalize (void* d)
{
	argused(d);
	for (unsigned i = objectQty; i > 0; --i) {
		--objectQty;
		object[objectQty].f(object[objectQty].p);
	}
}

void
__stack_chk_guard_setup ()
{
	unsigned char* p = (unsigned char*) &__stack_chk_guard;
	p[sizeof(size_t)-1] = 255;  /* <- this should be probably randomized */
	p[sizeof(size_t)-2] = '\n';
	p[0] = 0;
}

void
__attribute__((noreturn))
__stack_chk_fail ()
{
	fatal("stack broken");
}

void
__attribute__((noreturn))
__cxa_pure_virtual ()
{
	fatal("pure virtual function called");
}

void
entry()
{
	__stack_chk_guard_setup();

	//call all static constructors
	for (size_t* call = &__ctorsStart; call < &__ctorsEnd; call++) {
		if (*call) {
			((pproc_t)*call)();
		}
	}

	//call main
	main();

	//call all static destructors
	for (size_t* call = &__dtorsStart; call < &__dtorsEnd; call++) {
		if (*call) {
			((pproc_t)*call)();
		}
	}

	#ifdef DEBUG
	if (SmartPtrGlobalCounter != 0) {
		debug_print("CHECK SMART POINTERS, %d OBJECTS LEFT\n", SmartPtrGlobalCounter);
	} else if (!heap.IsEmpty()) {
		debug_print("MEMORY LEAKS:\n");
		heap.DumpDebug();
	}
	#endif

	cpuStop();
}

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

