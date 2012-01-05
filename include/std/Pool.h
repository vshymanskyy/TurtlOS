#ifndef INC_POOL_HPP
#define INC_POOL_HPP

#if _MSC_VER > 1000
	#pragma once
#endif

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <assert.h>
#include "Stack.h"

template <class T, int QTY, unsigned ALIGN = 0>
class Pool {

public:
    Pool() {
        for (int i=0; i<QTY; i++) {
            _free.Push(_data+i*sizeof(T));
        };
    };

    T* allocate() {
        assert(!_free.IsEmpty());
        return (T*)_free.Pop();
    }

    void release(void* ptr) {
        assert(!_free.IsFull());
        _free.Push(ptr);
    }

private:
    char              _data[sizeof(T)*QTY];
    Stack<void*, QTY> _free;
};

#endif // INC_POOL_HPP
