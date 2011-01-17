#ifndef CPUWAKER_H_
#define CPUWAKER_H_

#include "Spinlock.h"
#include <std/Delegate.h>

class CpuWaker {

public:
	typedef Delegate<void(void)> Entry;

	CpuWaker(Entry entry);
	bool StartCpu(uint32_t lapicId);

private:
	static void Waked();

private:
	Spinlock	_lock;
	Entry		_entry;
};

#endif /* CPUWAKER_H_ */
