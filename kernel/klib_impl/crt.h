#ifndef INC_CRT_H
#define INC_CRT_H

#if _MSC_VER > 1000
	#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

int __cxa_atexit(void(*f)(void*), void* p, void* d);
void __cxa_finalize(void* d);
void __cxa_pure_virtual();
void __stack_chk_guard_setup();
void __stack_chk_fail();

namespace __cxxabiv1
{
	__extension__ typedef int __guard __attribute__((mode (__DI__)));

	int __cxa_guard_acquire (__guard* g);
	void __cxa_guard_release (__guard* g);
	void __cxa_guard_abort (__guard*);
}

#ifdef __cplusplus
}
#endif

#endif // INC_CRT_H
