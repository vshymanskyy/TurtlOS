#ifndef INC_STACK_HPP
#define INC_STACK_HPP

#if _MSC_VER > 1000
	#pragma once
#endif

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

/// Represents a generic Stack (LIFO) with fixed size
template <class T, unsigned SIZE>
class Stack
{

public:
	/// Constructor
	Stack();

	/// Destructor
	~Stack();

    /// Checks if stack is empty
    bool IsEmpty() const;

    /// Checks if stack is full
    bool IsFull() const;

	/// Inserts an item into the queue
	/// @param item an item to push
	/// @returns false if buffer overflow occurs
	void Push(const T& item);

	/// Reads an item from the queue
	/// @returns false if buffer is empty
	T Pop();

	/// Reads the top item from the queue, not removing it
	/// @param item an item to read to
	/// @returns false if buffer is empty
	bool Peek(T& item);

private:
	/// Data buffer
    T  _data[SIZE];
	/// The 'top' element of the stack
    T* _top;
};

template <class T, unsigned SIZE>
inline
Stack<T, SIZE>::Stack()
    : _top	(&_data[0])
{
}

template <class T, unsigned SIZE>
inline
Stack<T, SIZE>::~Stack()
{
}

template<class T, unsigned SIZE>
inline
bool
Stack<T, SIZE>::IsEmpty() const
{
    return (_top <= &_data[0]);
}

template<class T, unsigned SIZE>
inline
bool
Stack<T, SIZE>::IsFull() const
{
    return (_top >= &_data[SIZE]);
}

template <class T, unsigned SIZE>
inline
void
Stack<T, SIZE>::Push(const T& item)
{
    *(_top++) = item;
}

template <class T, unsigned SIZE>
inline
T
Stack<T, SIZE>::Pop()
{
    return *(--_top);
}

template <class T, unsigned SIZE>
inline
bool
Stack<T, SIZE>::Peek(T& item)
{
    if (_top <= &_data[0])		//underflow
		return false;
    item = *(_top-1);
	return true;
}

#endif // INC_STACK_HPP
