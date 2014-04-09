#include <pic.h>
#include <portio.h>

#define PIC1_BASE 0x20			/* IO base address for master PIC */
#define PIC1_CMD  (PIC1_BASE + 0)
#define PIC1_DATA (PIC1_BASE + 1)

#define PIC2_BASE 0xA0			/* IO base address for slave PIC */
#define PIC2_CMD  (PIC2_BASE + 0)
#define PIC2_DATA (PIC2_BASE + 1)

#define PIC_EOI		0x20		/* End-of-interrupt command code */

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04	/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08	/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C	/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

void
picInit(uint8_t masterOffset, uint8_t slaveOffset)
{
	// Remap PIC
	outb(PIC1_CMD, ICW1_INIT+ICW1_ICW4);	/* starts the initialization sequence */
	io_wait();
	outb(PIC2_CMD, ICW1_INIT+ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, masterOffset);          /* define the PIC vectors */
	io_wait();
	outb(PIC2_DATA, slaveOffset);
	io_wait();
	outb(PIC1_DATA, 4);                     /* continue initialization sequence */
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	// Mask all interrupts
	outb(PIC1_DATA, 0xFB); // Due to PIC cascading (we want it enabled)
	outb(PIC2_DATA, 0xFF);
}

void
picSetMask(uint8_t irq, bool masked)
{
	uint16_t port = PIC1_DATA;
	
	//debug_print("[PIC] Setting mask IRQ %d to %d\n", irq, masked);

	if (irq > 7) {
		port = PIC2_DATA;
		irq -= 8;
	}
	uint8_t val = inb(port);
	//debug_print("[PIC] data before: 0b%08b\n", val);
	val &= ~(1 << irq);
	val |= (!!masked) << irq;
	//debug_print("[PIC] data after: 0b%08b\n", val);
	outb(port, val);
}

void
picSendEOI(uint8_t irq)
{
	if (irq > 7) {
		outb(PIC2_CMD,PIC_EOI);
	}
	outb(PIC1_CMD,PIC_EOI);
}