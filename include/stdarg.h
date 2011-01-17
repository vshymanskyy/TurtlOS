#ifndef INC_STDARG_H
#define INC_STDARG_H

#if _MSC_VER > 1000
	#pragma once
#endif

typedef char* va_list;
#define va_round(n)			((sizeof(n) + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1))
#define va_start(ap, v)		(ap = (va_list)&v + va_round(v))
#define va_arg(ap, t)		(*(t*)((ap += va_round(t)) - va_round(t)))
#define va_end(ap)			(ap = (va_list)0)

#endif /* INC_STDARG_H */
