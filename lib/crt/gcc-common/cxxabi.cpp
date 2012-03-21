#include <global.h>

namespace __cxxabiv1
{
	extern "C" {
		__extension__ typedef int __guard __attribute__((mode (__DI__)));

		int __cxa_guard_acquire (__guard* g);
		void __cxa_guard_release (__guard* g);
		void __cxa_guard_abort (__guard*);

		int __cxa_atexit(void(*f)(void*), void* p, void* d);
		void __cxa_finalize(void* d);
		void __cxa_pure_virtual();
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

void
__attribute__((noreturn))
__cxxabiv1::__cxa_pure_virtual ()
{
	fatal("pure virtual function called");
}

#define MAX_ATEXIT_HANDLERS 128

typedef struct {
	void (*f)(void*);
	void* p;
	void* d;
} PACKED atexit_func;

static atexit_func object[MAX_ATEXIT_HANDLERS];
static unsigned objectQty;

int
__cxxabiv1::__cxa_atexit (void (*f)(void*), void* p, void* d)
{
	assert(objectQty < MAX_ATEXIT_HANDLERS);
	object[objectQty].f = f;
	object[objectQty].p = p;
	object[objectQty].d = d;
	++objectQty;
	return 0;
}

void
__cxxabiv1::__cxa_finalize (void* d)
{
	unsigned i;
	for (i = objectQty; i > 0; --i) {
		--objectQty;
		object[objectQty].f(object[objectQty].p);
	}
}
