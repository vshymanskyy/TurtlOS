#include <hal/cpu.h>
#include <hal/portio.h>
#include <memory.h>

BEGIN_PACKED_REGION

struct RegisterFrame {
	// PushAll
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	// Push from handler
	uint32_t interrupt, error;
	// Pushed on interrupt
	uint32_t eip, cs, eflags, espPrev, ss;
} PACKED;

/// Pointer to global descriptor table
struct GdtPtr {
	uint16_t limit; // The upper 16 bits of all selector limits.
	uint32_t base; // The address of the first gdt_entry_t struct.
} PACKED;

/// Pointer to interrupt descriptor table
struct IdtPtr {
	uint16_t limit;
	uint32_t base; // The address of the first element in our idt_entry_t array.
} PACKED;

/// Global descriptor table entry
struct GdtEntry {
	uint16_t limit_low; // The lower 16 bits of the limit.
	uint16_t base_low; // The lower 16 bits of the base.
	uint8_t base_middle; // The next 8 bits of the base.
	uint8_t access; // Access flags, determine what ring this segment can be used in.
	uint8_t granularity;
	uint8_t base_high; // The last 8 bits of the base.
} PACKED;

/// Interrupt descriptor table entry
struct IdtEntry {
	uint16_t base_lo;	// The lower 16 bits of the address to jump to when this interrupt fires.
	uint16_t sel;		// Kernel segment selector.
	uint8_t reserved0;	// This must always be zero.
	uint8_t flags;		// More flags. See documentation.
	uint16_t base_hi;	// The upper 16 bits of the address to jump to.
} PACKED;

/// Task state segment entry
struct TssEntry {
	uint32_t prev_tss;	// The previous TSS - if we used hardware task switching this would form a linked list.
	uint32_t esp0;		// The stack pointer to load when we change to kernel mode.
	uint32_t ss0;		// The stack segment to load when we change to kernel mode.
	uint32_t esp1;		// Unused...
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es; // The value to load into ES when we change to kernel mode.
	uint32_t cs; // The value to load into CS when we change to kernel mode.
	uint32_t ss; // The value to load into SS when we change to kernel mode.
	uint32_t ds; // The value to load into DS when we change to kernel mode.
	uint32_t fs; // The value to load into FS when we change to kernel mode.
	uint32_t gs; // The value to load into GS when we change to kernel mode.
	uint32_t ldt; // Unused...
	uint16_t trap;
	uint16_t iomap_base;
} PACKED;

union CpuIdRegister {
	uint32_t value;
	uint16_t words[2];
	uint8_t bytes[4];
} PACKED;

struct CpuIdResult {
	CpuIdRegister a, b, c, d;
} PACKED;

union ControlRegister0 {
	struct __cr0_bitfield {
		uint32_t
			PE			:1,
			MP			:1,
			EM			:1,
			TS			:1,
			ET			:1,
			NE			:1,
			reserved0	:10,
			WP			:1,
			reserved1	:1,
			AM			:1,
			reserved2	:10,
			NW			:1,
			CD			:1,
			PG			:1;
	};
	uint32_t value;
} PACKED;

union ControlRegister3 {
	struct __cr3_bitfield {
		uint32_t
			reserved0	:3,
			writethough	:1,
			disable		:1;
	};
	uint32_t value;
} PACKED;

union ControlRegister4 {
	struct __cr4_bitfield {
		uint32_t
			VME			:1,
			PVI			:1,
			TSD			:1,
			DE			:1,
			PSE			:1,
			PAE			:1,
			MCE			:1,
			PGE			:1,
			PCE			:1,
			OSFXSR		:1,
			OSXMMEXCPT	:1,
			reserved0	:2,
			VMXE		:1,
			SMXE		:1;
	};
	uint32_t value;
} PACKED;

END_PACKED_REGION

extern "C" {
	void halCpuFlushGdt(uint32_t);
	void halCpuFlushIdt(uint32_t);
	void halCpuFlushTss();

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
	void irq0 ();
	void irq1 ();
	void irq2 ();
	void irq3 ();
	void irq4 ();
	void irq5 ();
	void irq6 ();
	void irq7 ();
	void irq8 ();
	void irq9 ();
	void irq10();
	void irq11();
	void irq12();
	void irq13();
	void irq14();
	void irq15();
	void isr128();

}

static void halCpuInitGdt();
static void halCpuInitIdt();
static void halCpuSetGdtGate(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t);
static void halCpuSetTssGate(uint32_t num, uint16_t ss0, uint32_t esp0);
static void halCpuSetIdtGate(uint8_t, uint32_t, uint16_t, uint8_t);

typedef void (*isr_t)(RegisterFrame*);

GdtEntry gdtEntries[6];
GdtPtr   gdtPtr;
IdtEntry idtEntries[256];
IdtPtr   idtPtr;
TssEntry tssEntry;
isr_t interrupt_handlers[256];

// Initialisation routine - zeroes all the interrupt service routines,
// initialises the GDT and IDT.
void halCpuInit()
{
	halCpuInitGdt();
	halCpuInitIdt();
	memset(&interrupt_handlers, 0, sizeof(isr_t)*256);
}

void halCpuInitGdt()
{
	gdtPtr.limit = (sizeof(GdtEntry) * 5) - 1;
	gdtPtr.base  = (uint32_t)&gdtEntries;

	halCpuSetGdtGate(0, 0, 0, 0, 0);                // Null segment
	halCpuSetGdtGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	halCpuSetGdtGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	halCpuSetGdtGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	halCpuSetGdtGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

	halCpuSetGdtGate(0, 0, 0, 0, 0);                // Null segment
	halCpuSetGdtGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	halCpuSetGdtGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	halCpuSetGdtGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	halCpuSetGdtGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	halCpuSetTssGate(5, 0x10, 0x0);

	halCpuFlushGdt((uint32_t)&gdtPtr);
	halCpuFlushTss();
}

// Set the value of one GDT entry.
void halCpuSetGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdtEntries[num].base_low    = (base & 0xFFFF);
	gdtEntries[num].base_middle = (base >> 16) & 0xFF;
	gdtEntries[num].base_high   = (base >> 24) & 0xFF;

	gdtEntries[num].limit_low   = (limit & 0xFFFF);
	gdtEntries[num].granularity = (limit >> 16) & 0x0F;

	gdtEntries[num].granularity |= gran & 0xF0;
	gdtEntries[num].access      = access;
}

// Initialise our task state segment structure.
void halCpuSetTssGate(uint32_t num, uint16_t ss0, uint32_t esp0)
{
	// Firstly, let's compute the base and limit of our entry into the GDT.
	uint32_t base = (uint32_t) &tssEntry;
	uint32_t limit = base + sizeof(tssEntry);

	// Now, add our TSS descriptor's address to the GDT.
	halCpuSetGdtGate(num, base, limit, 0xE9, 0x00);

	// Ensure the descriptor is initially zero.
	memset(&tssEntry, 0, sizeof(tssEntry));

	tssEntry.ss0  = ss0;  // Set the kernel stack segment.
	tssEntry.esp0 = esp0; // Set the kernel stack pointer.

	// Here we set the cs, ss, ds, es, fs and gs entries in the TSS. These specify what
	// segments should be loaded when the processor switches to kernel mode. Therefore
	// they are just our normal kernel code/data segments - 0x08 and 0x10 respectively,
	// but with the last two bits set, making 0x0b and 0x13. The setting of these bits
	// sets the RPL (requested privilege level) to 3, meaning that this TSS can be used
	// to switch to kernel mode from ring 3.
	tssEntry.cs   = 0x0b;
	tssEntry.ss = tssEntry.ds = tssEntry.es = tssEntry.fs = tssEntry.gs = 0x13;
}

void halCpuInitIdt()
{
	idtPtr.limit = sizeof(IdtEntry) * 256 -1;
	idtPtr.base  = (uint32_t)&idtEntries;

	// Remap the irq table.
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);

	memset(&idtEntries, 0, sizeof(IdtEntry)*256);

	halCpuSetIdtGate( 0, (uint32_t)isr0 , 0x08, 0x8E);
	halCpuSetIdtGate( 1, (uint32_t)isr1 , 0x08, 0x8E);
	halCpuSetIdtGate( 2, (uint32_t)isr2 , 0x08, 0x8E);
	halCpuSetIdtGate( 3, (uint32_t)isr3 , 0x08, 0x8E);
	halCpuSetIdtGate( 4, (uint32_t)isr4 , 0x08, 0x8E);
	halCpuSetIdtGate( 5, (uint32_t)isr5 , 0x08, 0x8E);
	halCpuSetIdtGate( 6, (uint32_t)isr6 , 0x08, 0x8E);
	halCpuSetIdtGate( 7, (uint32_t)isr7 , 0x08, 0x8E);
	halCpuSetIdtGate( 8, (uint32_t)isr8 , 0x08, 0x8E);
	halCpuSetIdtGate( 9, (uint32_t)isr9 , 0x08, 0x8E);
	halCpuSetIdtGate(10, (uint32_t)isr10, 0x08, 0x8E);
	halCpuSetIdtGate(11, (uint32_t)isr11, 0x08, 0x8E);
	halCpuSetIdtGate(12, (uint32_t)isr12, 0x08, 0x8E);
	halCpuSetIdtGate(13, (uint32_t)isr13, 0x08, 0x8E);
	halCpuSetIdtGate(14, (uint32_t)isr14, 0x08, 0x8E);
	halCpuSetIdtGate(15, (uint32_t)isr15, 0x08, 0x8E);
	halCpuSetIdtGate(16, (uint32_t)isr16, 0x08, 0x8E);
	halCpuSetIdtGate(17, (uint32_t)isr17, 0x08, 0x8E);
	halCpuSetIdtGate(18, (uint32_t)isr18, 0x08, 0x8E);
	halCpuSetIdtGate(19, (uint32_t)isr19, 0x08, 0x8E);
	halCpuSetIdtGate(20, (uint32_t)isr20, 0x08, 0x8E);
	halCpuSetIdtGate(21, (uint32_t)isr21, 0x08, 0x8E);
	halCpuSetIdtGate(22, (uint32_t)isr22, 0x08, 0x8E);
	halCpuSetIdtGate(23, (uint32_t)isr23, 0x08, 0x8E);
	halCpuSetIdtGate(24, (uint32_t)isr24, 0x08, 0x8E);
	halCpuSetIdtGate(25, (uint32_t)isr25, 0x08, 0x8E);
	halCpuSetIdtGate(26, (uint32_t)isr26, 0x08, 0x8E);
	halCpuSetIdtGate(27, (uint32_t)isr27, 0x08, 0x8E);
	halCpuSetIdtGate(28, (uint32_t)isr28, 0x08, 0x8E);
	halCpuSetIdtGate(29, (uint32_t)isr29, 0x08, 0x8E);
	halCpuSetIdtGate(30, (uint32_t)isr30, 0x08, 0x8E);
	halCpuSetIdtGate(31, (uint32_t)isr31, 0x08, 0x8E);
	halCpuSetIdtGate(32, (uint32_t)irq0, 0x08, 0x8E);
	halCpuSetIdtGate(33, (uint32_t)irq1, 0x08, 0x8E);
	halCpuSetIdtGate(34, (uint32_t)irq2, 0x08, 0x8E);
	halCpuSetIdtGate(35, (uint32_t)irq3, 0x08, 0x8E);
	halCpuSetIdtGate(36, (uint32_t)irq4, 0x08, 0x8E);
	halCpuSetIdtGate(37, (uint32_t)irq5, 0x08, 0x8E);
	halCpuSetIdtGate(38, (uint32_t)irq6, 0x08, 0x8E);
	halCpuSetIdtGate(39, (uint32_t)irq7, 0x08, 0x8E);
	halCpuSetIdtGate(40, (uint32_t)irq8, 0x08, 0x8E);
	halCpuSetIdtGate(41, (uint32_t)irq9, 0x08, 0x8E);
	halCpuSetIdtGate(42, (uint32_t)irq10, 0x08, 0x8E);
	halCpuSetIdtGate(43, (uint32_t)irq11, 0x08, 0x8E);
	halCpuSetIdtGate(44, (uint32_t)irq12, 0x08, 0x8E);
	halCpuSetIdtGate(45, (uint32_t)irq13, 0x08, 0x8E);
	halCpuSetIdtGate(46, (uint32_t)irq14, 0x08, 0x8E);
	halCpuSetIdtGate(47, (uint32_t)irq15, 0x08, 0x8E);
	halCpuSetIdtGate(128, (uint32_t)isr128, 0x08, 0x8E);

	halCpuFlushIdt((uint32_t)&idtPtr);
}

void halCpuSetIdtGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idtEntries[num].base_lo = base & 0xFFFF;
	idtEntries[num].base_hi = (base >> 16) & 0xFFFF;

	idtEntries[num].sel     = sel;
	idtEntries[num].reserved0 = 0;
	// We must uncomment the OR below when we get to using user-mode.
	// It sets the interrupt gate's privilege level to 3.
	idtEntries[num].flags   = flags /* | 0x60 */;
}

void register_interrupt_handler(int n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
extern "C"
void isr_handler(RegisterFrame regs)
{
	// This line is important. When the processor extends the 8-bit interrupt number
	// to a 32bit value, it sign-extends, not zero extends. So if the most significant
	// bit (0x80) is set, regs.int_no will be very large (about 0xffffff80).
	int int_no = regs.interrupt & 0xFF;
	if (interrupt_handlers[int_no] != 0) {
		isr_t handler = interrupt_handlers[int_no];
		handler(&regs);
	} else {
		for(;;);
	}
}

// This gets called from our ASM interrupt handler stub.
extern "C"
void irq_handler(RegisterFrame regs)
{
	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	if (regs.interrupt >= 40) {
		// Send reset signal to slave.
		outportb(0xA0, 0x20);
	}
	// Send reset signal to master. (As well as slave, if necessary).
	outportb(0x20, 0x20);

	if (interrupt_handlers[regs.interrupt] != 0) {
		interrupt_handlers[regs.interrupt](&regs);
	}

}

