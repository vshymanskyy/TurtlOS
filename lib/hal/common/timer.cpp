#include <hal/timer.h>
#include <hal/portio.h>

#define PIT_CHANNEL0_DATA 0x40    /* Channel 0 data port (read/write) */
#define PIT_CHANNEL1_DATA 0x41    /* Channel 1 data port (read/write) */
#define PIT_CHANNEL2_DATA 0x42    /* Channel 2 data port (read/write) */
#define PIT_MODE_CMD 0x43         /* Mode/Command register (write only, a read is ignored) */

void
timerSetFrequency(uint32_t frequency)
{
	uint32_t divisor;
	assert(frequency > 0);
	divisor = 1193181 / frequency;
	assert(divisor < 0x10000);


	outportb(PIT_MODE_CMD, 0x36);
	outportb(PIT_CHANNEL0_DATA, divisor & 0xFF);
	outportb(PIT_CHANNEL0_DATA, (divisor>>8) & 0xFF);
}
