#ifndef INC_STDARG_H
#define INC_STDARG_H

#if _MSC_VER > 1000
	#pragma once
#endif


typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v,l) __builtin_va_arg(v,l)
#define __va_copy(d,s) __builtin_va_copy(d,s)

#if !defined(__STRICT_ANSI__)
	#define va_copy(d,s) __builtin_va_copy(d,s)
#endif

/*
typedef char* va_list;
#define va_round(n)			((sizeof(n) + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1))
#define va_start(ap, v)		(ap = (va_list)&v + va_round(v))
#define va_arg(ap, t)		(*(t*)((ap += va_round(t)) - va_round(t)))
#define va_end(ap)			(ap = (va_list)0)
*/

#endif /* INC_STDARG_H */
