#include "cpu_structs.h"
#include <assert.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
	void halCpuFlushIdt(size_t);

	void isr0 ();
	void isr1 ();
	void isr2 ();
	void isr3 ();
	void isr4 ();
	void isr5 ();
	void isr6 ();
	void isr7 ();
	void isr8 ();
	void isr9 ();
	void isr10();
	void isr11();
	void isr12();
	void isr13();
	void isr14();
	void isr15();
	void isr16();
	void isr17();
	void isr18();
	void isr19();
	void isr20();
	void isr21();
	void isr22();
	void isr23();
	void isr24();
	void isr25();
	void isr26();
	void isr27();
	void isr28();
	void isr29();
	void isr30();
	void isr31();

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

	void isr128();

	void isr_handler();
#ifdef __cplusplus
}
#endif

IdtEntry idtEntries[MAX_INTERRUPTS];
isr_t interrupt_handlers[MAX_INTERRUPTS];

void halCpuSetIdtGate(uint8_t num, size_t base, uint16_t sel, uint8_t flags)
{
	idtEntries[num].base_lo = (uint16_t)(base & 0xFFFF);
	idtEntries[num].base_hi = (uint16_t)((base >> 16) & 0xFFFF);

	idtEntries[num].sel = sel;
	idtEntries[num].flags = flags;

	idtEntries[num].reserved0 = 0;
}

void halCpuInitIdt()
{
	memset(&interrupt_handlers, 0, sizeof(isr_t)*MAX_INTERRUPTS);
	memset(&idtEntries, 0, sizeof(IdtEntry)*MAX_INTERRUPTS);

	halCpuSetIdtGate( 0, (size_t)isr0 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate( 1, (size_t)isr1 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate( 2, (size_t)isr2 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate( 3, (size_t)isr3 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate( 4, (size_t)isr4 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate( 5, (size_t)isr5 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate( 6, (size_t)isr6 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate( 7, (size_t)isr7 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate( 8, (size_t)isr8 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate( 9, (size_t)isr9 , CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(10, (size_t)isr10, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(11, (size_t)isr11, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(12, (size_t)isr12, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(13, (size_t)isr13, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(14, (size_t)isr14, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(15, (size_t)isr15, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(16, (size_t)isr16, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(17, (size_t)isr17, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(18, (size_t)isr18, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(19, (size_t)isr19, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(20, (size_t)isr20, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(21, (size_t)isr21, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(22, (size_t)isr22, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(23, (size_t)isr23, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(24, (size_t)isr24, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(25, (size_t)isr25, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(26, (size_t)isr26, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(27, (size_t)isr27, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(28, (size_t)isr28, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(29, (size_t)isr29, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(30, (size_t)isr30, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(31, (size_t)isr31, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);

	halCpuSetIdtGate(32, (size_t)isr32, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(33, (size_t)isr33, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(34, (size_t)isr34, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(35, (size_t)isr35, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(36, (size_t)isr36, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(37, (size_t)isr37, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(38, (size_t)isr38, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(39, (size_t)isr39, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(40, (size_t)isr40, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(41, (size_t)isr41, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(42, (size_t)isr42, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(43, (size_t)isr43, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(44, (size_t)isr44, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(45, (size_t)isr45, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(46, (size_t)isr46, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);
	halCpuSetIdtGate(47, (size_t)isr47, CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);

	halCpuSetIdtGate(128,(size_t)isr128,CPU_GDT_SEL_CODE, IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_INT);

	cpuSetIDT(&idtEntries, sizeof(IdtEntry)*MAX_INTERRUPTS -1);
}

void halCpuRegisterISR(int n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}

void halCpuRegisterIRQ(int n, isr_t handler)
{
	interrupt_handlers[n+0x20] = handler;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler()
{
	register RegisterFrame_t* regs __asm ("ecx");
	regs->interrupt &= 0xFF;
	if (interrupt_handlers[regs->interrupt] != NULL) {
		interrupt_handlers[regs->interrupt](regs);
	} else {
		debug_print("[ISR] Unhandled interrupt %d\n", regs->interrupt);
	}
}
