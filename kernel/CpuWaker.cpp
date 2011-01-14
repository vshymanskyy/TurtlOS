#include "CpuWaker.h"

#include "DeviceManager.h"
#include "Console.h"

#include "hal/lapic.h"
#include <hal/cpu.h>

#define CPU_WAKER_ADDR 0x1000
#define CPU_CONFIG_ADDR 0x600

struct CpuConfig {
	uint8_t mode;
	uint8_t id;
	size_t entry;
} PACKED;

static CpuWaker* instance = NULL;
static CpuConfig* const config = (CpuConfig*)CPU_CONFIG_ADDR;

CpuWaker::CpuWaker(Entry entry)
	: mLock(false)
	, mEntry(entry)
{
	assert(!instance);
	List<iDevFileSystem*> fsystems = DeviceManager::Instance()->GetFileSystems();
	for (List<iDevFileSystem*>::It its = fsystems.First(); its != fsystems.End(); ++its) {
		if (File waker = fsystems[its]->FindFile("waker.bin")) {
			(*console) << "Found waker.bin (" << waker->Length << " bytes) on " << fsystems[its]->GetDescription() << endl;
			waker->Read(0, waker->Length, (void*)CPU_WAKER_ADDR);
		}
	}
	instance = this;
}

bool
CpuWaker::StartCpu(uint32_t lapicId)
{
	assert((size_t)CpuWaker::Waked < 0xFFFFFFFF);
	assert(cpuGetPageTable() < 0xFFFFFFFF);

	mLock.Lock();

	config->mode	= TARGET;
	config->id		= (uint8_t)lapicId;
	config->entry	= (size_t)&Waked;

	lapicStartCpu(lapicId, CPU_WAKER_ADDR);

	const bool locked = mLock.Wait(10000000);
	mLock.Unlock();
	return locked;
}

void
CpuWaker::Waked()
{
	assert(instance);
	assert(config->id == lapicGetID());
	assert(config->mode == TARGET);
	instance->mLock.Unlock();
	instance->mEntry();

	cpuStop();
}

