#ifndef STDARG_H
#define STDARG_H

typedef char* va_list;
#define va_round(n)			((sizeof(n) + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1))
#define va_start(ap, v)		(ap = (va_list)&v + va_round(v))
#define va_arg(ap, t)		(*(t*)((ap += va_round(t)) - va_round(t)))
#define va_end(ap)			(ap = (va_list)0)

#endif // STDARG_H
