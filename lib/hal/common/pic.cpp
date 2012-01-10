#include <hal/pic.h>
#include <hal/portio.h>

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
picSendEOI(uint8_t irq)
{
	if (irq >= 8) {
		outb(PIC2_CMD,PIC_EOI);
	}
	outb(PIC1_CMD,PIC_EOI);
}

void
picRemap(uint8_t masterOffset, uint8_t slaveOffset)
{
	uint8_t a1, a2;

	a1 = inb(PIC1_DATA);					/* save masks */
	a2 = inb(PIC2_DATA);

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

	outb(PIC1_DATA, a1);   /* restore saved masks */
	outb(PIC2_DATA, a2);
}

void
picSetMask(uint8_t IRQline)
{
    uint16_t port;
    uint8_t value;

    if (IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);
}

void
picClearMask(uint8_t IRQline)
{
    uint16_t port;
    uint8_t value;

    if (IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}
