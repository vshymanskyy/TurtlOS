#ifndef _INC_CPU_H
#define _INC_CPU_H

int cpuBcdToInt(const uint8_t bcd);
uint8_t cpuIntToBcd(const int i);
void cpuDisableInterrupts();
void cpuEnableInterrupts();
void cpuNoOperation();
void cpuHalt();
void cpuStop();

size_t cpuGetPageTable();
void cpuSetPageTable(size_t base);
void cpuSetGDT(size_t base, uint16_t limit);
void cpuSetIDT(size_t base, uint16_t limit);

uint64_t cpuGetMSR(uint32_t msr);
void cpuSetMSR(uint32_t msr, uint64_t val);

#include <../hal/common/cpu.inl>

#if TARGET == I386
#  include <../hal/x86-64/cpu.inl>
#elif TARGET == X86_64
#  include <../hal/i386/cpu.inl>
#else
#  error "target platform not supported"
#endif

#endif  // _INC_CPU_H

