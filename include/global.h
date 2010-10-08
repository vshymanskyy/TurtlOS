#ifndef _INC_GLOBAL_H
#define _INC_GLOBAL_H

#include <platform.h>

#if COMPILER == COMPILER_GCC

	#define __int8 char
	#define __int16 short
	#define __int32 long int
	#define __int64 long long

	typedef signed __int8		int8_t;
	typedef signed __int16		int16_t;
	typedef signed __int32		int32_t;
	typedef signed __int64		int64_t;

	typedef unsigned __int8		uint8_t;
	typedef unsigned __int16	uint16_t;
	typedef unsigned __int32	uint32_t;
	typedef unsigned __int64	uint64_t;

#elif COMPILER == MSVC

	typedef signed __int8		int8_t;
	typedef signed __int16		int16_t;
	typedef signed __int32		int32_t;
	typedef signed __int64		int64_t;

	typedef unsigned __int8		uint8_t;
	typedef unsigned __int16	uint16_t;
	typedef unsigned __int32	uint32_t;
	typedef unsigned __int64	uint64_t;

#else

	#error "Compiler not supported."

#endif

typedef void     (*pproc_t)(void);
typedef int      (*pfunc_t)(void);

typedef void*    ptr;

//*******************************************************
//  size_t
//*******************************************************
#if		TARGET == TARGET_X86
	typedef unsigned int		size_t;
	typedef signed int			ptrdiff_t;

	#define SIZE_T_MIN			0
	#define SIZE_T_MAX			0xffffffffUL
	#define SIZE_T_SIZE			4
#elif		TARGET == TARGET_X86_64
	typedef unsigned long		size_t;
	typedef signed long			ptrdiff_t;

	#define SIZE_T_MIN			0
	#define SIZE_T_MAX			0xffffffffffffffffULL
	#define SIZE_T_SIZE			8

#else
	#error "Platform not supported."
#endif

//*******************************************************
//  Null
//*******************************************************
#ifdef __cplusplus
#	define NULL 0
#else
#	define NULL ((void*)0)
#endif

//*******************************************************
//  StdDefs
//*******************************************************
#define offsetof(type, member) ((size_t)&(((type*)0)->member))
#define _bit(val, i) ((size_t)(*(val)) & ((size_t)1 << (i)))
#define _countof(arr) (sizeof(arr)/sizeof(arr[0]))
#define asm __asm__ __volatile__

#ifdef __cplusplus
	template<typename T>
	inline
	const T& max (const T& a, const T& b) {
		return (a > b)? a : b;
	}

	template<typename T>
	inline
	const T& min (const T& a, const T& b) {
		return (a < b)? a : b;
	}

	template<typename T>
	inline
	void argused (const T& arg) {
		(void)(arg);
	}
#else
	#define max(a,b) ((a > b)? (a) : (b))
	#define min(a,b) ((a < b)? (a) : (b))
	#define argused(arg) (void)(arg)
#endif

//*******************************************************
//  StdArg
//*******************************************************
typedef char* va_list;
#define va_round(n) ((sizeof(n) + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1))
#define va_start(ap, v) (ap = (va_list)&v + va_round(v))
#define va_arg(ap, t) (*(t*)((ap += va_round(t)) - va_round(t)))
#define va_end(ap) (ap = (va_list)0)

//*******************************************************
//  Misc
//*******************************************************
#define __concat(x,y) x ## y
#define __string(x) #x

//*******************************************************
//  Assert
//*******************************************************


extern class Console* console;
extern class Console* console2;
extern class CliDesktop* desktop;

#include <std/klib.h>

#endif //_INC_GLOBAL_H

