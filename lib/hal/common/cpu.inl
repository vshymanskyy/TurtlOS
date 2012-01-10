#pragma once

#define CPU_MSR_APICBASE 0x1B
#define CPU_MSR_PERFCTR0 0xC1
#define CPU_MSR_PERFCTR1 0xC2

inline
int
cpuBcdToInt(const uint8_t bcd)
{
	return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

inline
uint8_t
cpuIntToBcd(const int i)
{
	return (uint8_t)(((i / 10) << 4) + (i % 10));
}

inline
void
cpuDisableInterrupts()
{
	asm ("cli");
}

inline
void
cpuEnableInterrupts()
{
	asm ("sti");
}

inline
int cpuInterruptsEnabled()
{
	int f;
	asm ("pushf\n popl %0" : "=g"(f));
	return f & ( 1 << 9 );
}

inline
void
cpuNoOperation()
{
	asm ("nop");
}

inline
void
cpuStop()
{
	for(;;) {
		asm ("cli");
		asm ("hlt");
	}
}

inline
void
cpuHalt()
{
	asm ("hlt");
}

inline
void
cpuWriteBackInvalidateCache()
{
	asm ("wbinvd");
}

inline
void
cpuInvalidateCache()
{
	asm ("invd");
}

inline
size_t cpuGetCR0(void)
{
	size_t val;
    asm ( "mov %%cr0, %0" : "=r"(val) );
    return val;
}

inline
size_t cpuGetCR1(void)
{
	size_t val;
    asm ( "mov %%cr1, %0" : "=r"(val) );
    return val;
}

inline
size_t cpuGetCR2(void)
{
	size_t val;
    asm ( "mov %%cr2, %0" : "=r"(val) );
    return val;
}

inline
size_t cpuGetCR3(void)
{
	size_t val;
    asm ( "mov %%cr3, %0" : "=r"(val) );
    return val;
}

inline
size_t
cpuGetStackPtr()
{
	size_t result;
	asm ("mov %%esp, (%0)" : "=r"(result));
	return result;
}

inline
void
cpuSetStackPtr(void* ptr, size_t size)
{
	asm ("mov (%0), %%esp; push $0; push $0; push $0" : : "r"((size_t)ptr+size-8));
}

inline
uint64_t
cpuGetMSR(uint32_t msr)
{
	uint32_t hi, lo;
	asm ("rdmsr":"=a"(lo),"=d"(hi):"c"(msr));
	return (((uint64_t)hi) << 32) | lo;
}

inline
void
cpuSetMSR(uint32_t msr, uint64_t val)
{
	uint32_t hi = (uint32_t)(val >> 32);
	uint32_t lo = val & 0xFFFFFFFF;
	asm ("wrmsr"::"a"(lo),"d"(hi),"c"(msr));
}

inline
uint64_t
cpuReadTimeStamp(void) {
	uint32_t hi, lo;
	asm ("rdtsc":"=a"(lo),"=d"(hi));
	return (((uint64_t)hi) << 32) | lo;
}

inline
void
cpuSetIDT(void* base, uint16_t limit)
{
	struct {
		uint16_t limit;
		size_t base;
	} __attribute__((__packed__)) IDTR;

	IDTR.limit = limit;
	IDTR.base = (size_t)base;
	asm ("lidt (%0)" : : "p"((size_t)&IDTR));
}
