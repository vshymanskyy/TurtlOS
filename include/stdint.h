#ifndef INC_STDINT_H
#define INC_STDINT_H

#if _MSC_VER > 1000
	#pragma once
#endif

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

#elif COMPILER == COMPILER_MSVC

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

#endif /* INC_STDINT_H */
