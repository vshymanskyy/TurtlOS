#ifndef _INC_CPU_H
#define _INC_CPU_H

#include <global.h>

#include INCLUDE_COMMON(../lib/hal,cpu.h)
#include INCLUDE_TARGET(../lib/hal,cpu.h)

typedef void (*isr_t)(RegisterFrame*);

#ifdef __cplusplus
extern "C" {
#endif

	void halCpuInitIdt(void);
	void halCpuRegisterISR(int n, isr_t handler);

	int cpuBcdToInt(const uint8_t bcd);
	uint8_t cpuIntToBcd(const int i);
	void cpuDisableInterrupts();
	void cpuEnableInterrupts();
	void cpuNoOperation();
	void cpuHalt();
	void cpuStop();

	size_t cpuGetPageTable();
	void cpuSetPageTable(size_t base);
	void cpuSetIDT(void* base, uint16_t limit);

	uint64_t cpuGetMSR(uint32_t msr);
	void cpuSetMSR(uint32_t msr, uint64_t val);

#ifdef __cplusplus
}
#endif

#include INCLUDE_COMMON(../lib/hal,cpu.inl)
#include INCLUDE_TARGET(../lib/hal,cpu.inl)

#endif  // _INC_CPU_H
