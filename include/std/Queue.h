#ifndef INC_QUEUE_HPP
#define INC_QUEUE_HPP

#if _MSC_VER > 1000
	#pragma once
#endif

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

/// Represents a generic circular (FIFO) index-based Queue with fixed size
/// Recommendation: Use this Queue with SIZE = 2^n to get fast results
template<class T, unsigned SIZE>
class Queue
{

public:
	/// Constructor
	Queue();
	/// Destructor
	~Queue();

	/// Checks if Queue is empty
	bool IsEmpty() const;

	/// Clears the Queue
	void Clear();

	/// Inserts an item into the Queue
	void Push(const T& info);

	/// Reads an item from the Queue
	T Pop();

	/// Reads the top item from the Queue, not removing it
	T Peek();

private:
	/// Data buffer
	T	_data[SIZE];
	/// Index of writer
	int _inIdx;
	/// Index of reader
	int _outIdx;
};

template<class T, unsigned SIZE>
inline
Queue<T, SIZE>::Queue()
	: _inIdx(0)
	, _outIdx(SIZE - 1)
{
}

template<class T, unsigned SIZE>
inline
Queue<T, SIZE>::~Queue()
{
}

template<class T, unsigned SIZE>
inline
bool
Queue<T, SIZE>::IsEmpty() const
{
	return (((_outIdx+1)%SIZE) == _inIdx);
}

template<class T, unsigned SIZE>
inline
void
Queue<T, SIZE>::Clear()
{
	_inIdx = 0;
	_outIdx = SIZE - 1;
}

template<class T, unsigned SIZE>
inline
void
Queue<T, SIZE>::Push(const T& info)
{
	AutoSpinLock(this);
	_data[_inIdx] = info;
	_inIdx = (_inIdx+1)%SIZE;
	assert(_inIdx != _outIdx);		//overflow
}

template<class T, unsigned SIZE>
inline
T
Queue<T, SIZE>::Pop()
{
	AutoSpinLock(this);
	_outIdx = (_outIdx+1)%SIZE;
	assert(_outIdx != _inIdx);		//underflow
	return _data[_outIdx];
}

template<class T, unsigned SIZE>
inline
T
Queue<T, SIZE>::Peek()
{
	const int p = (_outIdx+1)%SIZE;
	assert(p != _inIdx);			//underflow
	return _data[p];
}

#endif // INC_QUEUE_HPP
