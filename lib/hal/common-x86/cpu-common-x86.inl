#pragma once

INLINE
int
cpuBcdToInt(const uint8_t bcd)
{
	return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

INLINE
uint8_t
cpuIntToBcd(const int i)
{
	return (uint8_t)(((i / 10) << 4) + (i % 10));
}

FORCE_INLINE
void
cpuDisableInterrupts()
{
	asm ("cli");
}

FORCE_INLINE
void
cpuEnableInterrupts()
{
	asm ("sti");
}

FORCE_INLINE
int cpuInterruptsEnabled()
{
	int f;
	asm ("pushf\n popl %0" : "=g"(f));
	return f & ( 1 << 9 );
}

FORCE_INLINE
void
cpuNoOperation()
{
	asm ("nop");
}


INLINE
void
cpuHalt()
{
	asm ("hlt");
}

INLINE
void
cpuStop()
{
	for(;;) {
		asm ("cli");
		asm ("hlt");
	}
}

FORCE_INLINE
void
cpuWriteBackInvalidateCache()
{
	asm ("wbinvd");
}

FORCE_INLINE
void
cpuInvalidateCache()
{
	asm ("invd");
}

FORCE_INLINE
size_t cpuGetCR0(void)
{
	size_t val;
    asm ( "mov %%cr0, %0" : "=r"(val) );
    return val;
}

FORCE_INLINE
size_t cpuGetCR1(void)
{
	size_t val;
    asm ( "mov %%cr1, %0" : "=r"(val) );
    return val;
}

FORCE_INLINE
size_t cpuGetCR2(void)
{
	size_t val;
    asm ( "mov %%cr2, %0" : "=r"(val) );
    return val;
}

FORCE_INLINE
size_t cpuGetCR3(void)
{
	size_t val;
    asm ( "mov %%cr3, %0" : "=r"(val) );
    return val;
}

FORCE_INLINE
size_t
cpuGetStackPtr()
{
	size_t result;
	asm ("mov %%esp, (%0)" : "=r"(result));
	return result;
}

FORCE_INLINE
void
cpuSetStackPtr(void* ptr, size_t size)
{
	asm ("mov (%0), %%esp; push $0; push $0; push $0" : : "r"((size_t)ptr+size-8));
}

FORCE_INLINE
uint64_t
cpuGetMSR(uint32_t msr)
{
	uint32_t hi, lo;
	asm ("rdmsr":"=a"(lo),"=d"(hi):"c"(msr));
	return (((uint64_t)hi) << 32) | lo;
}

FORCE_INLINE
void
cpuSetMSR(uint32_t msr, uint64_t val)
{
	uint32_t hi = (uint32_t)(val >> 32);
	uint32_t lo = val & 0xFFFFFFFF;
	asm ("wrmsr"::"a"(lo),"d"(hi),"c"(msr));
}

FORCE_INLINE
uint64_t
cpuReadTimeStamp(void)
{
	uint32_t hi, lo;
	asm ("rdtsc":"=a"(lo),"=d"(hi));
	return (((uint64_t)hi) << 32) | lo;
}

INLINE
void
cpuSetIDT(void* base, uint16_t limit)
{
	struct {
		uint16_t limit;
		size_t base;
	} __attribute__((__packed__)) IDTR;

	IDTR.limit = limit;
	IDTR.base = (size_t)base;
	asm ("lidt (%0)" : : "a"(&IDTR));
}
