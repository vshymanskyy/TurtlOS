#ifndef _INC_CPU_H
#define _INC_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

	void cpuInit();

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

#ifdef __cplusplus
}
#endif

#include INCLUDE_COMMON(../lib/hal,cpu.inl)
#include INCLUDE_TARGET(../lib/hal,cpu.inl)

#endif  // _INC_CPU_H
