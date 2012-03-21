#ifndef _INC_ATOMIC_H
#define _INC_ATOMIC_H

INLINE size_t atomicExchange(size_t *dest, size_t value);
INLINE size_t atomicCompareExchange(size_t *dest, size_t comperand, size_t newValue);

#include "atomic.inl"

#endif  // _INC_ATOMIC_H
