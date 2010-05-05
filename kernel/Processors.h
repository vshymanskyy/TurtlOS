#pragma once

#include <std/List.h>
#include <std/Array.h>
#include "Spinlock.h"

struct CpuDesc {
	enum CpuState {
		CPU_DISABLED,
		CPU_STOPPED,
		CPU_BOOTING,
		CPU_INIT,
		CPU_READY,
		CPU_HALTED
	};

	uint32_t lapicId;
	CpuState state;
	void* stack;
	void* lapic;
	size_t stackSize;

//	CpuInfo* mInfo;
//	Interrupts* mInterrupts;
//	Exceptions* mExceptions;
//	FloatingPointUnit* mFpu;

	CpuDesc();
	CpuDesc(uint32_t lapic_id, bool enabled);
};

class Processors {

public:
	static Processors* Instance();

	void AddCpu (uint32_t lapicId, bool enabled, bool bsp);

	void Startup();

	int GetCount()const;
	int GetReadyCount()const;
	CpuDesc* GetCpu(uint32_t lapicId);

	~Processors();

private:
	Processors();

	void InitAp();
	void InitBsp();

private:
	uint32_t mBspLapicId;
	typedef Array<CpuDesc, 256> Container;
	Container mProcessors;
	Spinlock mLock;
};
