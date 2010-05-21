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

#define HAL_INCLUDE_COMMON_IMPL(FILENAME) <../hal/common/FILENAME>
#define HAL_INCLUDE_TARGET_IMPL(FILENAME) <../hal/CURTARGET/FILENAME>

#include HAL_INCLUDE_COMMON_IMPL(cpu.inl)
#include HAL_INCLUDE_TARGET_IMPL(cpu.inl)

#endif  // _INC_CPU_H
