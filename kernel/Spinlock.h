#pragma once

#include <atomic.h>

/// @brief Spinlock for CPU synchronization
class Spinlock
{
public:
	/// @brief Constructor
	Spinlock(bool locked = false);

	/// @brief Destructor
	virtual ~Spinlock();

	/// @brief Locks the spinlock
	void Lock();

	/// @brief Unlocks the spinlock
	void Unlock();

	/// @brief Waits the lock to unlock
	void Wait();

	bool Wait(size_t cycles);

private:
	enum State{
		UNLOCKED	= 0,
		LOCKED		= 1
	};

	size_t _value;
};

inline
Spinlock::Spinlock(bool locked)
	: _value(locked ? LOCKED : UNLOCKED)
{
}

inline
Spinlock::~Spinlock()
{
}

inline
void
Spinlock::Lock()
{
	while(atomicCompareExchange(&_value, UNLOCKED, LOCKED) == LOCKED) {
	};
}

inline
void
Spinlock::Unlock()
{
	atomicExchange(&_value, UNLOCKED);
}

inline
void
Spinlock::Wait()
{
	Lock();
	Unlock();
}

inline
bool
Spinlock::Wait(size_t cycles)
{
	size_t cycle = 0;
	while(atomicCompareExchange(&_value, UNLOCKED, LOCKED) == LOCKED) {
		if (cycle++ > cycles) {
			return false;
		}
	}
	return true;
}
