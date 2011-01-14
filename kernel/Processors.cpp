#include "Processors.h"
#include "CpuWaker.h"
#include "Console.h"
#include <hal/lapic.h>
#include <hal/cpu.h>

CpuDesc::CpuDesc()
	: lapicId (-1)
	, state (CPU_DISABLED)
	, stack (NULL)
	, stackSize (0)
{
}

CpuDesc::CpuDesc(uint32_t lapic_id, bool enabled)
	: lapicId(lapic_id)
	, state (enabled ? CPU_STOPPED : CPU_DISABLED)
	, stack (NULL)
	, stackSize (0)
{

}

Processors::Processors()
	: mBspLapicId(0xFF)
	, mLock(false)
{
}

Processors::~Processors()
{
	for(Container::It i = mProcessors.First(); i != mProcessors.End(); ++i) {
		if (mProcessors[i].stack) {
			if (mProcessors[i].stack) {
				free(mProcessors[i].stack);
			}
			if (mProcessors[i].lapic) {
				free(mProcessors[i].lapic);
			}
		}
	}
}

CpuDesc*
Processors::GetCpu(uint32_t lapicId)
{
	for(Container::It i = mProcessors.First(); i != mProcessors.End(); ++i) {
		if (mProcessors[i].lapicId == lapicId) {
			return &mProcessors[i];
		}
	}
	return NULL;
}

Processors*
Processors::Instance()
{
	static Processors instance;
	return &instance;
}

void
Processors::Startup()
{
	InitBsp();

	CpuWaker waker(CpuWaker::Entry(Processors::Instance(), &Processors::InitAp));
	for(Container::It i = mProcessors.First(); i != mProcessors.End(); ++i) {
		CpuDesc& cpu = mProcessors[i];
		if (cpu.state == CpuDesc::CPU_DISABLED || cpu.lapicId == mBspLapicId) {
			continue;
		}
		(*console2) << "CPU " << (int)cpu.lapicId << ": ";
		if (cpu.state == CpuDesc::CPU_STOPPED) {
			(*console2) << "Starting...";
			cpu.state = CpuDesc::CPU_BOOTING;
			if (!waker.StartCpu(cpu.lapicId)) {
				(*console2) << esc << "[31m fail" << esc << "[m" << endl;
			}
			mLock.Wait();
		} else {
			static const char* labels[5] = { "Disabled", "Stopped", "Booting", "Init", "Ready" };
			(*console2) << labels[cpu.state] << endl;
		}
	}
}

void
Processors::InitBsp() {
	assert (mBspLapicId != 0xFF);
	CpuDesc* cpu = GetCpu(mBspLapicId);
	cpu->state = CpuDesc::CPU_INIT;
	cpu->stack = NULL;
	cpu->lapic = NULL;
	//cpu->lapic = aligned_alloc(0x400, 0x1000);
	//lapicSetBase((size_t)cpu->lapic);
	lapicStart();

	cpu->state = CpuDesc::CPU_READY;
}

void
Processors::InitAp() {
	mLock.Lock();
	(*console2) << esc << "[32m ok" << esc << "[m" << endl;
	CpuDesc* cpu = GetCpu(lapicGetID());
	cpu->state = CpuDesc::CPU_INIT;

	cpu->stackSize = 0x4096;
	cpu->stack = malloc(cpu->stackSize);
	cpu->lapic = NULL;

	//cpu->lapic = aligned_alloc(0x400, 0x1000);

	//lapicSetBase((size_t)cpu->lapic);
	lapicStart();

	cpu->state = CpuDesc::CPU_HALTED;
	mLock.Unlock();
	cpuHalt();

	fatal("executing after cpuHalt");
}

void
Processors::AddCpu (uint32_t lapicId, bool enabled, bool bsp)
{
	if (bsp) {
		mBspLapicId = lapicId;
	}
	mProcessors.Append(CpuDesc(lapicId, enabled));
}

int
Processors::GetReadyCount()const
{
	int result = 0;
	for(Container::It i = mProcessors.First(); i != mProcessors.End(); ++i) {
		if (mProcessors[i].state == CpuDesc::CPU_READY) {
			++result;
		}
	}
	return result;
}

int
Processors::GetCount()const
{
	return mProcessors.Count();
}

