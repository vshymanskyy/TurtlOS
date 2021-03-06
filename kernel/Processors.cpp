#include "Processors.h"
#include "CpuWaker.h"
#include "Console.h"
#include <lapic.h>
#include <cpu_structs.h>
#include <pic.h>
#include <timer.h>
#include <stdlib.h>
#include <stdio.h>

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

Spinlock isrLock;

static
void
DefaultIsrHandler(RegisterFrame* regs)
{
	isrLock.Lock();
	debug_print("[CPU %d] Unhandled int %d\n", lapicGetID(), regs->interrupt);
	isrLock.Unlock();
}

#if TARGET == TARGET_X86

char exceptionBuffer[1024];

static
void
HandleException(RegisterFrame* regs)
{
	char* buff = exceptionBuffer;
	buff += snprintf(buff, 1024, "Exception on CPU%d: %d (%s)\n", lapicGetID(), regs->interrupt, ExceptionMsgs[regs->interrupt]);
	buff += snprintf(buff, 1024, "CS:IP=0x%x:%x ERROR:0x%08x\n", regs->cs, regs->eip, regs->error);
	buff += snprintf(buff, 1024, "AX=%08x BX=%08x CX=%08x DX=%08x FL=%08x\n", regs->eax, regs->ebx, regs->ecx, regs->edx, regs->eflags);
	buff += snprintf(buff, 1024, "BP=%08x SP=%08x DI=%08x SI=%08x\n", regs->ebp, regs->esp, regs->edi, regs->esi);

	debug_puts(exceptionBuffer);
	cpuHalt();
}

#elif TARGET == TARGET_X86_64

char exceptionBuffer[1024];

static
void
HandleException(RegisterFrame* regs)
{
	char* buff = exceptionBuffer;
	buff += snprintf(buff, 1024, "Exception on CPU%d: %d (%s)\n", lapicGetID(), regs->interrupt, ExceptionMsgs[regs->interrupt]);
	buff += snprintf(buff, 1024, "CS:IP=0x%x:%x ERROR=0x%016x\n", regs->cs, regs->rip, regs->error);
	buff += snprintf(buff, 1024, "AX=%016x BX=%016x CX=%016x DX=%016x\n", regs->rax, regs->rbx, regs->rcx, regs->rdx);
	buff += snprintf(buff, 1024, " 9=%016x 10=%016x 11=%016x 12=%016x\n", regs->r9 , regs->r10, regs->r11, regs->r12);
	buff += snprintf(buff, 1024, "13=%016x 14=%016x 15=%016x FL=%016x\n", regs->r13, regs->r14, regs->r15, regs->rflags);
	buff += snprintf(buff, 1024, "BP=%016x SP=%016x DI=%016x SI=%016x\n", regs->rbp, regs->rsp, regs->rdi, regs->rsi);

	debug_puts(exceptionBuffer);
	cpuHalt();
}
#elif TARGET == TARGET_ARM

static
void
HandleException(RegisterFrame* regs)
{

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
	//, _lock(false)
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
/*
	CpuWaker waker(CpuWaker::Entry(Processors::Instance(), &Processors::InitAp));
	for(CpuList::It i = _processors.First(); i != _processors.End(); ++i)
	{
		CpuDesc& cpu = _processors[i];
		if (cpu.state == CpuDesc::CPU_DISABLED || cpu.lapicId == _bspLapicId)
		{
			continue;
		}
		if (cpu.state == CpuDesc::CPU_STOPPED)
		{
			cpu.state = CpuDesc::CPU_BOOTING;
			cpu.stackSize = 0x4096;
			cpu.stack = malloc(cpu.stackSize);

			if (!waker.StartCpu(cpu.lapicId, (void*)((size_t)cpu.stack) + cpu.stackSize - 16))
			{
				debug_print("[CPU %d] Failed to start CPU %d\n", lapicGetID(), cpu.lapicId);
			}
			//_lock.Wait();
		}
	}
*/
}

void
Processors::InitBsp() {
	assert (_bspLapicId != 0xFF);


	halCpuInitIdt();
	picInit(32, 40);

	SetExceptionHandlers();
	cpuEnableInterrupts();
	lapicStart();

	timerSetFrequency(20);
	timerInit();


	if (CpuDesc* cpu = GetCpu(_bspLapicId))
	{
		cpu->stack = NULL;
		cpu->lapic = NULL;
		cpu->state = CpuDesc::CPU_READY;
		//cpu->lapic = aligned_alloc(0x400, 0x1000);
		//lapicSetBase((size_t)cpu->lapic);
	}
}

void
Processors::InitAp() {
	//_lock.Lock();
	debug_print("[CPU %d] Started\n", lapicGetID());
	CpuDesc* cpu = GetCpu(lapicGetID());

	cpu->state = CpuDesc::CPU_INIT;
	cpu->lapic = NULL;

	halCpuInitIdt();
	picInit(0x20, 0x28);

	SetExceptionHandlers();
	cpuEnableInterrupts();

	timerInit();

	//cpu->lapic = aligned_alloc(0x400, 0x1000);
	//lapicSetBase((size_t)cpu->lapic);
	lapicStart();

	//for(;;) {
	//	(*console) << cpu->lapicId << endl;
	//}


	cpu->state = CpuDesc::CPU_READY;

	//_lock.Unlock();

	//cpuStop();

	//for(;;) {
		//for(int i=0; i<10000000; i++);
		//asm ("int $128");
	//}

	cpuStop();

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

