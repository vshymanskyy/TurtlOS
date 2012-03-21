#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void isr32(); // system timer (cannot be changed)
void isr33(); // keyboard controller (cannot be changed)
void isr34(); // cascaded signals from IRQs 8–15
void isr35(); // serial port controller for COM2 (shared with COM4, if present)
void isr36(); // serial port controller for COM1 (shared with COM3, if present)
void isr37(); // LPT port 2  or  sound card
void isr38(); // floppy disk controller
void isr39(); // LPT port 1  or  It is used for printers or for any parallel port if a printer is not present

void isr40(); // RTC Timer
void isr41(); // The Interrupt is left open for the use of peripherals. open interrupt / available  or  SCSI host adapter
void isr42(); // The Interrupt is left open for the use of peripherals. open interrupt / available  or  SCSI  or  NIC
void isr43(); // The Interrupt is left open for the use of peripherals. open interrupt / available  or  SCSI  or  NIC
void isr44(); // mouse on PS/2 connector
void isr45(); // math co-processor  or  integrated floating point unit  or  inter-processor interrupt (use depends on OS)
void isr46(); // primary ATA channel
void isr47(); // secondary ATA channel

#define PIC_IRQ_TIMER 0
#define PIC_IRQ_KEYBOARD 1
#define PIC_IRQ_CASCADE 2
#define PIC_IRQ_SERIAL2 3
#define PIC_IRQ_SERIAL1 4
#define PIC_IRQ_LPT2 5
#define PIC_IRQ_FLOPPY 6
#define PIC_IRQ_LPT1 7

#define PIC_IRQ_RTC 8
#define PIC_IRQ_INT1 9
#define PIC_IRQ_INT2 10
#define PIC_IRQ_INT3 11
#define PIC_IRQ_MOUSE 12
#define PIC_IRQ_MATH 13
#define PIC_IRQ_ATA_PRIMARY 14
#define PIC_IRQ_ATA_SECONDARY 15

void picSendEOI(uint8_t irq);
void picRemap(uint8_t masterOffset, uint8_t slaveOffset);
void picSetMask(uint8_t IRQline);
void picClearMask(uint8_t IRQline);

#ifdef __cplusplus
}
#endif
