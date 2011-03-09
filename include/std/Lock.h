#pragma once

class LockableStub {
public:
	LockableStub()	{}
	~LockableStub()	{}
	void lock()		{}
	void unlock()	{}
};

#define LOCKABLE class LOCK = LockableStub
