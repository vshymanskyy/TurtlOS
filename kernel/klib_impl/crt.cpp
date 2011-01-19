#include "crt.h"

#include <linker.h>
#include <hal/cpu.h>

#define MAX_ATEXIT_HANDLERS 128

int main();
extern "C" void entry();

struct atexit_func {
	void (*f)(void*);
	void* p;
	void* d;
} PACKED;

static atexit_func object[MAX_ATEXIT_HANDLERS];
static unsigned objectQty;

void* __dso_handle;
void* __stack_chk_guard;

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

static void
sanityChecks()
{
	assert(sizeof(int8_t)  == 1);
	assert(sizeof(int16_t) == 2);
	assert(sizeof(int32_t) == 4);
	assert(sizeof(int64_t) == 8);
	assert(sizeof(uint8_t)  == 1);
	assert(sizeof(uint16_t) == 2);
	assert(sizeof(uint32_t) == 4);
	assert(sizeof(uint64_t) == 8);
#if TARGET == TARGET_X86
	assert(sizeof(char)			== 1);
	assert(sizeof(short)		== 2);
	assert(sizeof(int)			== 4);
	assert(sizeof(long)			== 4);
	assert(sizeof(long long)	== 8);
	assert(sizeof(size_t)		== 4);
#elif TARGET == TARGET_X86_64
	assert(sizeof(char)			== 1);
	assert(sizeof(short)		== 2);
	assert(sizeof(int)			== 4);
	assert(sizeof(long)			== 8);
	assert(sizeof(long long)	== 8);
	assert(sizeof(size_t)		== 8);
#else
#error "Target not supported"
#endif
	debug_print("Sanity checks passed\n");
}

void
entry()
{
	debug_print("Kernel entry executed\n");
	__stack_chk_guard_setup();

	sanityChecks();

	// Call all static constructors
	for (size_t* call = &__ctorsStart; call < &__ctorsEnd; call++) {
		if (*call) {
			debug_print("Calling ctor 0x%p\n", *call);
			((pproc_t)*call)();
		}
	}

	debug_print("Calling main\n");
	main();

	// Call all static destructors
	for (size_t* call = &__dtorsStart; call < &__dtorsEnd; call++) {
		if (*call) {
			debug_print("Calling dtor 0x%p\n", *call);
			((pproc_t)*call)();
		}
	}

	cpuStop();
}
