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
//  Misc
//*******************************************************
#define __concat(x,y) x ## y
#define __string(x) #x

extern class Console* console;
extern class Console* console2;
extern class CliDesktop* desktop;

#include <new.h>
#include <assert.h>
#include <stddef.h>
#include <stdarg.h>

#include <std/klib.h>

#endif //_INC_GLOBAL_H

