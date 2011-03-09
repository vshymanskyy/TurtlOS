#include "Processors.h"
#include "CpuWaker.h"
#include "Console.h"
#include <hal/lapic.h>
#include <hal/cpu.h>
#include <stdlib.h>

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
	: _bspLapicId(0xFF)
	, _lock(false)
{
}

Processors::~Processors()
{
	for(CpuList::It i = _processors.First(); i != _processors.End(); ++i) {
		if (_processors[i].stack) {
			if (_processors[i].stack) {
				free(_processors[i].stack);
			}
			if (_processors[i].lapic) {
				free(_processors[i].lapic);
			}
		}
	}
}

CpuDesc*
Processors::GetCpu(uint32_t lapicId)
{
	for(CpuList::It i = _processors.First(); i != _processors.End(); ++i) {
		if (_processors[i].lapicId == lapicId) {
			return &_processors[i];
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
	for(CpuList::It i = _processors.First(); i != _processors.End(); ++i) {
		CpuDesc& cpu = _processors[i];
		if (cpu.state == CpuDesc::CPU_DISABLED || cpu.lapicId == _bspLapicId) {
			continue;
		}
		(*console2) << "CPU " << (int)cpu.lapicId << ": ";
		if (cpu.state == CpuDesc::CPU_STOPPED) {
			(*console2) << "Starting...";
			cpu.state = CpuDesc::CPU_BOOTING;
			if (!waker.StartCpu(cpu.lapicId)) {
				(*console2) << esc << "[31m fail" << esc << "[m" << endl;
			}
			_lock.Wait();
		} else {
			static const char* labels[5] = { "Disabled", "Stopped", "Booting", "Init", "Ready" };
			(*console2) << labels[cpu.state] << endl;
		}
	}
}

void
Processors::InitBsp() {
	assert (_bspLapicId != 0xFF);
	CpuDesc* cpu = GetCpu(_bspLapicId);
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
	_lock.Lock();
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
	_lock.Unlock();
	cpuHalt();

	fatal("executing after cpuHalt");
}

void
Processors::AddCpu (uint32_t lapicId, bool enabled, bool bsp)
{
	if (bsp) {
		_bspLapicId = lapicId;
	}
	_processors.Append(CpuDesc(lapicId, enabled));
}

int
Processors::GetReadyQty()const
{
	int result = 0;
	for(CpuList::It i = _processors.First(); i != _processors.End(); ++i) {
		if (_processors[i].state == CpuDesc::CPU_READY) {
			++result;
		}
	}
	return result;
}

int
Processors::GetCpuQty()const
{
	return _processors.Count();
}

