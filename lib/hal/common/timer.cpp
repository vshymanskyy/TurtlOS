#include <hal/timer.h>
#include <hal/cpu.h>
#include <hal/lapic.h>
#include <hal/pic.h>
#include <hal/portio.h>

#define PIT_CHANNEL0_DATA 0x40    /* Channel 0 data port (read/write) */
#define PIT_CHANNEL1_DATA 0x41    /* Channel 1 data port (read/write) */
#define PIT_CHANNEL2_DATA 0x42    /* Channel 2 data port (read/write) */
#define PIT_MODE_CMD 0x43         /* Mode/Command register (write only, a read is ignored) */

unsigned tick=0;
char rotator[] = {'|','/','-','\\'};

static
void
TimerIrqHandler(RegisterFrame* regs)
{
	tick++;
	*((char*)0xB8000 + (79-lapicGetID())*2) = rotator[tick%4];

	picSendEOI(PIC_IRQ_TIMER);
}

void
timerInit(void)
{
	halCpuRegisterIRQ(PIC_IRQ_TIMER, &TimerIrqHandler);
	picClearMask(PIC_IRQ_TIMER);
}


void
timerSetFrequency(uint32_t frequency)
{
	uint32_t divisor;
	assert(frequency > 0);
	divisor = 1193181 / frequency;
	assert(divisor < 0x10000);


	outb(PIT_MODE_CMD, 0x36);
	outb(PIT_CHANNEL0_DATA, divisor & 0xFF);
	outb(PIT_CHANNEL0_DATA, (divisor>>8) & 0xFF);
}
