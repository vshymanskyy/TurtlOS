#include "Processors.h"
#include "CpuWaker.h"
#include "Console.h"
#include <hal/lapic.h>
#include <hal/cpu.h>
#include <hal/pic.h>
#include <hal/timer.h>
#include <stdlib.h>


static
const char* ExceptionMsgs[] = {
	"Division by zero",
	"Debug",
	"Non maskable interrupt",
	"Breakpoint",
	"Into detected overflow",
	"Out of bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault ->",
	"Coprocessor segment overrun",
	"Bad TSS ->",
	"Segment not present ->",
	"Stack fault ->",
	"General protection fault ->",
	"Page fault ->",
	"Unknown interrupt exception",
	"Coprocessor fault",
	"Alignment check",
	"Machine check"
};

static
void
DefaultIsrHandler(RegisterFrame* regs)
{
	(*console) << "Unhandled int on CPU" << (size_t)lapicGetID() <<  " (" << (size_t)(regs->interrupt) << ")" << endl;
}

#if TARGET == TARGET_X86

static
void
HandleException(RegisterFrame* regs)
{
	(*console)
		<< esc << "[41;93m"
		<< "Exception on CPU" << (size_t)lapicGetID() << ": "
		<< ExceptionMsgs[regs->interrupt]
		<< " (" << (size_t)regs->interrupt << ") at "
		<< (int)regs->cs << ':' << (ptr_t)regs->eip << endl
		<< " Error code: " << (ptr_t)regs->error << endl
		<< " AX:" << (ptr_t)regs->eax
		<< " BX:" << (ptr_t)regs->ebx
		<< " CX:" << (ptr_t)regs->ecx
		<< " DX:" << (ptr_t)regs->edx
		<< " FL:" << (ptr_t)regs->eflags << endl
		<< " BP:" << (ptr_t)regs->ebp
		<< " SP:" << (ptr_t)regs->esp
		<< " DI:" << (ptr_t)regs->edi
		<< " SI:" << (ptr_t)regs->esi << endl
		<< esc << "[m";
	cpuHalt();
}

#elif TARGET == TARGET_X86_64

static
void
HandleException(RegisterFrame* regs)
{
	(*console)
		<< esc << "[41;93m"
		<< "Exception on CPU" << (size_t)lapicGetID() << ": "
		<< ExceptionMsgs[regs->interrupt]
		<< " (" << (size_t)regs->interrupt << ") at "
		<< (int)regs->cs << ':' << (ptr_t)regs->rip << endl
		<< " Error code: " << (ptr_t)regs->error << endl
		<< " AX:" << (ptr_t)regs->rax
		<< " BX:" << (ptr_t)regs->rbx
		<< " CX:" << (ptr_t)regs->rcx
		<< " DX:" << (ptr_t)regs->rdx
		<< " 9 :" << (ptr_t)regs->r9
		<< " 10:" << (ptr_t)regs->r10
		<< " 11:" << (ptr_t)regs->r11
		<< " 12:" << (ptr_t)regs->r12
		<< " 13:" << (ptr_t)regs->r13
		<< " 14:" << (ptr_t)regs->r14
		<< " 15:" << (ptr_t)regs->r15
		<< " FL:" << (ptr_t)regs->rflags
		<< " BP:" << (ptr_t)regs->rbp
		<< " SP:" << (ptr_t)regs->rsp
		<< " DI:" << (ptr_t)regs->rdi
		<< " SI:" << (ptr_t)regs->rsi << endl
		<< esc << "[m";
	cpuHalt();
}

#else
	#error "Target not supported"
#endif


void SetExceptionHandlers(void) {
	for(int i=0; i<MAX_INTERRUPTS; ++i) {
		halCpuRegisterISR(i, &DefaultIsrHandler);
	}
	for(int i=0; i<_countof(ExceptionMsgs); ++i) {
		halCpuRegisterISR(i, &HandleException);
	}
}

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
		(*console) << "CPU " << (int)cpu.lapicId << ": ";
		if (cpu.state == CpuDesc::CPU_STOPPED) {
			(*console) << "Starting...";
			cpu.state = CpuDesc::CPU_BOOTING;
			if (!waker.StartCpu(cpu.lapicId)) {
				(*console) << esc << "[31m fail" << esc << "[m" << endl;
			}
			_lock.Wait();
		} else {
			static const char* labels[5] = { "Disabled", "Stopped", "Booting", "Init", "Ready" };
			(*console) << labels[cpu.state] << endl;
		}
	}
}

void
Processors::InitBsp() {
	assert (_bspLapicId != 0xFF);
	CpuDesc* cpu = GetCpu(_bspLapicId);
	cpu->stack = NULL;
	cpu->lapic = NULL;

	//halCpuInitIdt();
	picRemap(0x20, 0x28);
	for(int i = 0; i<16; i++) {
		picSetMask(i);
	}
	timerSetFrequency(50);
	SetExceptionHandlers();
	//cpuEnableInterrupts();
	lapicStart();

	cpu->state = CpuDesc::CPU_INIT;

	//cpu->lapic = aligned_alloc(0x400, 0x1000);
	//lapicSetBase((size_t)cpu->lapic);

	cpu->state = CpuDesc::CPU_READY;
}

void
Processors::InitAp() {
	_lock.Lock();
	(*console) << esc << "[32m ok" << esc << "[m" << endl;
	CpuDesc* cpu = GetCpu(lapicGetID());

	cpu->state = CpuDesc::CPU_INIT;
	cpu->stackSize = 0x4096;
	cpu->stack = malloc(cpu->stackSize);
	cpu->lapic = NULL;

	//cpu->lapic = aligned_alloc(0x400, 0x1000);
	//lapicSetBase((size_t)cpu->lapic);
	lapicStart();

	//for(;;) {
	//	(*console) << cpu->lapicId << endl;
	//}


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

