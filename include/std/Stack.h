#ifndef _INC_Stack_H
#define _INC_Stack_H

/// Represents a generic Stack (LIFO) with fixed size
template <class T, unsigned SIZE>
class Stack
{

public:
	/// Constructor
	Stack();

	/// Destructor
	~Stack();

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
	T  mData[SIZE];
	/// The 'top' element of the stack
	T* mTop;
};

template <class T, unsigned SIZE>
inline
Stack<T, SIZE>::Stack()
	: mTop(&mData[0])
{
}

template <class T, unsigned SIZE>
inline
Stack<T, SIZE>::~Stack()
{
}

template <class T, unsigned SIZE>
inline
void
Stack<T, SIZE>::Push(const T& item)
{
// 	if (mTop >= &mData[SIZE])	//overflow
// 		return false;
	*(mTop++) = item;
}

template <class T, unsigned SIZE>
inline
T
Stack<T, SIZE>::Pop()
{
//	if (mTop <= &mData[0])		//underflow
	return *(--mTop);
}

template <class T, unsigned SIZE>
inline
bool
Stack<T, SIZE>::Peek(T& item)
{
	if (mTop <= &mData[0])		//underflow
		return false;
	item = *(mTop-1);
	return true;
}

#endif // _INC_Stack_H

