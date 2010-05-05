#include <hal/lapic.h>
#include <hal/cpu.h>

#define LAPIC_BROADCAST_ID	0xFF
#define LAPIC_SIZE			0x00000400

#define ID_SHIFT			24
#define ID_MASK				0x0F
#define VERSION_MASK		0xFF
#define TPR_MASK			0xFF
#define APR_MASK			0xFF
#define PPR_MASK			0xFF
#define LDR_SHIFT			24
#define DFR_FLAT			0xFFFFFFFF
#define DFR_CLUSTER			0x0FFFFFFF
#define DFR_SHIFT			28
#define SVR_MASK			0x0FF
#define SVR_ENABLE			0x100
#define SVR_FOCUS_OFF		0x200
#define ICR_VECTOR_MASK		0x000FF
#define ICR_DM_MASK			0x00700
#define ICR_DM_FIXED		0x00000
#define ICR_DM_LOWEST		0x00100
#define ICR_DM_SMI			0x00200
#define ICR_DM_REMOTE		0x00300
#define ICR_DM_NMI			0x00400
#define ICR_DM_INIT			0x00500
#define ICR_DM_STARTUP		0x00600
#define ICR_DM_LOGICAL		0x00800
#define ICR_DS_PENDING		0x01000
#define ICR_LEVEL_ASSERT	0x04000
#define ICR_TRIGGER_LEVEL	0x08000
#define ICR_RR_MASK			0x30000
#define ICR_RR_INVALID		0x00000
#define ICR_RR_INPROGRESS	0x10000
#define ICR_RR_VALID		0x20000
#define ICR_DSS_MASK		0xC0000
#define ICR_DSS_DEST		0x00000
#define ICR_DSS_SELF		0x40000
#define ICR_DSS_ALL			0x80000
#define ICR_DSS_OTHERS		0xC0000
#define ICRD_DEST_SHIFT		24
#define LVT_VECTOR_MASK		0x000FF
#define LVT_DM_SHIFT		8
#define LVT_DM_MASK			0x00007
#define LVT_DM_FIXED		0x00000
#define LVT_DM_NMI			0x00400
#define LVT_DM_EXTINT		0x00700
#define LVT_DS_PENDING		0x01000
#define LVT_IP_PLRITY_LOW	0x02000
#define LVT_REMOTE_IRR		0x04000
#define LVT_TM_LEVEL		0x08000
#define LVT_MASKED			0x10000
#define LVT_PERIODIC		0x20000

#define TIMER_DIVIDE_MASK	0x0000000F	//set in bit 0,1,3; bit 2 = false
#define TIMER_DIVIDE_2		0x00000000
#define TIMER_DIVIDE_4		0x00000001
#define TIMER_DIVIDE_8		0x00000002
#define TIMER_DIVIDE_16		0x00000003
#define TIMER_DIVIDE_32		0x00000008
#define TIMER_DIVIDE_64		0x00000009
#define TIMER_DIVIDE_128	0x0000000A
#define TIMER_DIVIDE_1		0x0000000B

#define DEFAULT_INTERRUPT_BASE		0xD0
#define REDUCED_INTERRUPT_BASE		0x50

enum lapicInterrupt {
	PERFCNT		= 0xF
	,TIMER			= 0xE
	,INTERPROCESSOR	= 0xD
	,THERMAL		= 0xC
	,ERROR			= 0xB
	,SPURIOUS		= 0xA
	,NMI			= 0x2
};

enum lapicRegister {
	ID						= 0x0020
	,VER					= 0x0030
	,TPR					= 0x0080
	,APR					= 0x0090
	,PPR					= 0x00A0
	,EOI					= 0x00B0
	,REMOTE_READ            = 0x00C0
	,LDR					= 0x00D0
	,DFR					= 0x00E0
	,SVR					= 0x00F0
	,ISR_BASE				= 0x0100
	,TMR_BASE				= 0x0180
	,IRR_BASE				= 0x0200
	,ERROR_STATUS			= 0x0280
	,ICR					= 0x0300
	,ICRD					= 0x0310
	,LVT_TIMER				= 0x0320
	,LVT_THERMAL			= 0x0330
	,LVT_PERFCNT			= 0x0340
	,LVT_LINT0				= 0x0350
	,LVT_LINT1				= 0x0360
	,LVT_ERROR				= 0x0370
	,TIMER_INITIAL_COUNT	= 0x0380
	,TIMER_CURRENT_COUNT	= 0x0390
	,TIMER_DIVIDE_CONFIG	= 0x03E0
};

size_t lapicAddress;
uint32_t lapicInterruptBase;

static
uint32_t
lapicGetInterruptVector(lapicInterrupt i)
{
	return lapicInterruptBase + i;
}

void
lapicClearErrorStatus(void)
{
	lapicWrite(ERROR_STATUS, 0);
	lapicWrite(ERROR_STATUS, 0);
}

uint32_t
lapicReadErrorStatus(void)
{
	lapicWrite(ERROR_STATUS, 0);
	return lapicRead(ERROR_STATUS);
}

void
lapicInit()
{
	lapicAddress = lapicGetBase();
	lapicInterruptBase = DEFAULT_INTERRUPT_BASE;
	// ExtINT for Master
	lapicWrite(LVT_LINT0, (lapicRead(LVT_LINT0) & (~LVT_MASKED)) | LVT_DM_EXTINT);
}

void
lapicStart()
{
	lapicWrite(DFR, DFR_FLAT);	// Set flat delivery model
	lapicWrite(LDR, lapicGetID() << LDR_SHIFT);

	lapicWrite(TPR, 0);			// Accept all
	lapicWrite(SVR, SVR | SVR_ENABLE);

	lapicWrite(LVT_TIMER, lapicGetInterruptVector(TIMER));		// Timer: unmasked, one-shot
	lapicWrite(LVT_PERFCNT, lapicGetInterruptVector(PERFCNT));
	lapicWrite(LVT_THERMAL, lapicGetInterruptVector(THERMAL));
	lapicClearErrorStatus();
	lapicWrite(LVT_ERROR, lapicGetInterruptVector(ERROR));
}

void
lapicSetBase(size_t addr)
{
	assert(!(addr & 0xFFF));
	//TODO: assert(addr < MAXPHYADDR);
	uint64_t base = cpuGetMSR(CPU_MSR_APICBASE);
	base &= 0xFFF;
	base |= addr;
	cpuSetMSR(CPU_MSR_APICBASE, base);
	lapicAddress = base;
}

size_t
lapicGetBase()
{
	return (size_t)cpuGetMSR(CPU_MSR_APICBASE) & ~0xFFF;
}

void
lapicStartCpu(uint32_t id, uint32_t entry)
{
	assert(entry < 0x100000);
	assert(!(entry & 0xFFF));

	lapicWrite(ICRD, id << ICRD_DEST_SHIFT);
	lapicWrite(ICR, ICR_DM_INIT);

	lapicWrite(ICRD, id << ICRD_DEST_SHIFT);
	lapicWrite(ICR, ICR_DM_STARTUP | (entry >> 12));

	lapicReadErrorStatus();
}
