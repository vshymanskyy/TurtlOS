#pragma once

INLINE
size_t
atomicExchange(size_t *dest, size_t value)
{
	return __sync_lock_test_and_set(dest, value);
}

INLINE
size_t
atomicCompareExchange(size_t *dest, size_t comperand, size_t newValue)
{
	return __sync_val_compare_and_swap(dest, comperand, newValue);
}
