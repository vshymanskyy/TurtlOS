#ifndef _INC_QUEUE_H
#define _INC_QUEUE_H

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
	T	mData[SIZE];
	/// Index of writer
	int mIn;
	/// Index of reader
	int mOut;
};

template<class T, unsigned SIZE>
inline
Queue<T, SIZE>::Queue()
	: mIn(0)
	, mOut(SIZE - 1)
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
	return (((mOut+1)%SIZE) == mIn);
}

template<class T, unsigned SIZE>
inline
void
Queue<T, SIZE>::Clear()
{
	mIn = 0;
	mOut = SIZE - 1;
}

template<class T, unsigned SIZE>
inline
void
Queue<T, SIZE>::Push(const T& info)
{
	AutoSpinLock(this);
	mData[mIn] = info;
	mIn = (mIn+1)%SIZE;
	assert(mIn != mOut);		//overflow
}

template<class T, unsigned SIZE>
inline
T
Queue<T, SIZE>::Pop()
{
	AutoSpinLock(this);
	mOut = (mOut+1)%SIZE;
	assert(mOut != mIn);		//underflow
	return mData[mOut];
}

template<class T, unsigned SIZE>
inline
T
Queue<T, SIZE>::Peek()
{
	const int p = (mOut+1)%SIZE;
	assert(p != mIn);			//underflow
	return mData[p];
}

#endif // _INC_QUEUE_H

