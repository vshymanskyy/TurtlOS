#include <linker.h>
#include <cpu_structs.h>

extern void __stack_chk_guard_setup ();
extern void main();

void entry()
{
	debug_puts("Kernel entry executed\n");

	//__stack_chk_guard_setup();

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
#elif TARGET == TARGET_ARM
	assert(sizeof(char)			== 1);
	assert(sizeof(short)		== 2);
	assert(sizeof(int)			== 4);
	assert(sizeof(long)			== 4);
	assert(sizeof(long long)	== 8);
	assert(sizeof(size_t)		== 4);
#else
#error "Target not supported"
#endif

	// Call all static constructors
	size_t* call;
	for (call = &__ctorsStart; call < &__ctorsEnd; call++) {
		if (*call) {
			debug_print("Calling ctor 0x%p\n", *call);
			((pproc_t)*call)();
		}
	}

	debug_puts("Calling main\n");
	main();

	// Call all static destructors
	for (call = &__dtorsStart; call < &__dtorsEnd; call++) {
		if (*call) {
			debug_print("Calling dtor 0x%p\n", *call);
			((pproc_t)*call)();
		}
	}

	cpuStop();
}
