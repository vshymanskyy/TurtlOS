#ifndef INC_POOL_ALLOC_HPP
#define INC_POOL_ALLOC_HPP

#if _MSC_VER > 1000
	#pragma once
#endif

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <stddef.h>
#include <assert.h>
#include "Pool.h"

template <class T, int QTY>
class PoolAlloc {
#ifndef DMALLOC
public:

	void* operator new(size_t) {
		return entries->allocate();
	}

	void operator delete(void* ptr) {
		entries->release(ptr);
	}

private:
	static Pool<T, QTY>* entries;
#endif
};

#ifndef DMALLOC
template <class T, int QTY> Pool<T, QTY>* PoolAlloc<T, QTY>::entries = new Pool<T, QTY>();
#endif

#endif // INC_POOL_ALLOC_HPP
