#ifndef _INC_ATOMIC_H
#define _INC_ATOMIC_H

size_t atomicExchange(size_t *dest, size_t value);
size_t atomicCompareExchange(size_t *dest, size_t comperand, size_t newValue);

#include <../hal/common/atomic.inl>

#endif  // _INC_ATOMIC_H
