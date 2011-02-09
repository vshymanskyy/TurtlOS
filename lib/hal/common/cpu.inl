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
	asm("cli");
}

inline
void
cpuEnableInterrupts()
{
	asm("sti");
}

inline
void
cpuNoOperation()
{
	asm("nop");
}

inline
void
cpuStop()
{
	for(;;) {
		asm("cli");
		asm("hlt");
	}
}

inline
void
cpuHalt()
{
	asm("hlt");
}

inline
void
cpuWriteBackInvalidateCache()
{
	asm("wbinvd");
}

inline
void
cpuInvalidateCache()
{
	asm("invd");
}

inline
size_t
cpuGetPageTable()
{
	unsigned long cr3;
	asm ("mov %%cr3, %0" : "=r" (cr3) :);
	return cr3;
}

inline
uint64_t
cpuGetMSR(uint32_t msr)
{
	uint32_t hi, lo;
	asm ("rdmsr":"=a"(*&lo),"=d"(*&hi):"c"(msr));
	return (((uint64_t)hi) << 32) | lo;
}

inline
void
cpuSetMSR(uint32_t msr, uint64_t val)
{
	uint32_t hi = (uint32_t)(val >> 32);
	uint32_t lo = val & 0xFFFFFFFF;
	asm("wrmsr"::"a"(lo),"d"(hi),"c"(msr));
}

