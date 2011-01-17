#ifndef INC_STDDEF_H
#define INC_STDDEF_H

#if _MSC_VER > 1000
	#pragma once
#endif

#include <stdint.h>

/********************************************************
 *  size_t
 *******************************************************/
#if		TARGET == TARGET_X86
	typedef uint32_t			size_t;
	typedef int32_t				ptrdiff_t;

	#define SIZE_T_MIN			0
	#define SIZE_T_MAX			0xffffffffUL
	#define SIZE_T_SIZE			4
#elif		TARGET == TARGET_X86_64
	typedef long unsigned int	size_t;
	typedef long signed int		ptrdiff_t;

	#define SIZE_T_MIN			0
	#define SIZE_T_MAX			0xffffffffffffffffULL
	#define SIZE_T_SIZE			8

#else
	#error "Platform not supported."
#endif

/********************************************************
 *  NULL
 *******************************************************/
#ifdef __cplusplus
#	define NULL 0
#else
#	define NULL ((void*)0)
#endif

/********************************************************
 *  StdDefs
 *******************************************************/
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
	#define MAX(a,b) ((a > b)? (a) : (b))
	#define MIN(a,b) ((a < b)? (a) : (b))
	#define ARG_USED(arg) (void)(arg)
#endif


#endif /* INC_STDDEF_H */

