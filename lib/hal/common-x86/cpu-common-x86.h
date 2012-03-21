#ifndef _INC_CPU_H
#define _INC_CPU_H

#include <global.h>

#define MAX_INTERRUPTS		256

#define IDT_DESC_PRESENT	0x80	//10000000
#define IDT_DESC_RING0		0x00	//00000000
#define IDT_DESC_RING1		0x40	//01000000
#define IDT_DESC_RING2		0x20	//00100000
#define IDT_DESC_RING3		0x60	//01100000
#define IDT_DESC_TSS_AVAIL	0x09	//00001001
#define IDT_DESC_TSS_BUSY	0x0B	//00001011
#define IDT_DESC_CALL		0x0C	//00001100
#define IDT_DESC_INT		0x0E	//00001110
#define IDT_DESC_TRAP		0x0F	//00001111

#define CPU_MSR_APICBASE 0x1B
#define CPU_MSR_PERFCTR0 0xC1
#define CPU_MSR_PERFCTR1 0xC2

typedef void (*isr_t)(struct RegisterFrame*);

INLINE int cpuBcdToInt(const uint8_t bcd);
INLINE uint8_t cpuIntToBcd(const int i);
INLINE void cpuDisableInterrupts(void);
INLINE void cpuEnableInterrupts();
INLINE int cpuInterruptsEnabled();
INLINE void cpuNoOperation();
INLINE void cpuHalt();
INLINE void cpuStop();
INLINE void cpuWriteBackInvalidateCache();
INLINE void cpuInvalidateCache();
INLINE size_t cpuGetCR0(void);
INLINE size_t cpuGetCR1(void);
INLINE size_t cpuGetCR2(void);
INLINE size_t cpuGetCR3(void);
INLINE size_t cpuGetStackPtr();
INLINE void cpuSetStackPtr(void* ptr, size_t size);
INLINE uint64_t cpuGetMSR(uint32_t msr);
INLINE void cpuSetMSR(uint32_t msr, uint64_t val);
INLINE uint64_t cpuReadTimeStamp(void);
INLINE void cpuSetIDT(void* base, uint16_t limit);

#ifdef __cplusplus
extern "C" {
#endif

void halCpuInitIdt();

void halCpuRegisterIRQ(int n, isr_t handler);
void halCpuRegisterISR(int n, isr_t handler);

#ifdef __cplusplus
}
#endif

#include "cpu-common-x86.inl"

#endif  // _INC_CPU_H
