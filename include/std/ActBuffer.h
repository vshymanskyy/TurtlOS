#pragma once
#include <stdlib.h>
#include <assert.h>

class LockableStub {
public:
	LockableStub()	{}
	~LockableStub()	{}
	void lock()		{}
	void unlock()	{}
};

template < typename T, class LOCKABLE = LockableStub >
class ActBuffer
	: private LOCKABLE
{

public:
	typedef void ProcessCbk(T* buffer, size_t count);

	ActBuffer(size_t size, ProcessCbk* callback)
		: _data		(new T[size])
		, _size		(size)
		, _callback	(callback)
		, _count	(0)
	{
		assert(_data);
		assert(_callback);
	}

	~ActBuffer() {
		this->lock();
		InnerFlush();
		if (_data) {
			delete[] _data;
		}
		this->unlock();
	}

	void Process(const T& item) {
		this->lock();
		_data[_count++] = item;
		if (_count >= _size) {
			InnerFlush();
		}
		this->unlock();
	}

	void Flush() {
		this->lock();
		InnerFlush();
		this->unlock();
	}

private:

	void InnerFlush() {
		if (_count > 0) {
			(*_callback)(_data, _count);
			_count = 0;
		}
	}

private:

	T*					_data;
	const size_t		_size;
	ProcessCbk* const	_callback;
	size_t				_count;
};

