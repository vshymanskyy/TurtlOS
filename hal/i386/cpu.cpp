#include <hal/cpu.h>

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
	uint16_t base_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
	uint16_t sel; // Kernel segment selector.
	uint8_t reserved; // This must always be zero.
	uint8_t flags; // More flags. See documentation.
	uint16_t base_hi; // The upper 16 bits of the address to jump to.
} PACKED;

/// Task state segment entry
struct TssEntry {
	uint32_t prev_tss; // The previous TSS - if we used hardware task switching this would form a linked list.
	uint32_t esp0; // The stack pointer to load when we change to kernel mode.
	uint32_t ss0; // The stack segment to load when we change to kernel mode.
	uint32_t esp1; // Unused...
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
		size_t PE :1, MP :1, EM :1, TS :1, ET :1, NE :1, reserved0 :10, WP :1, reserved1 :1, AM :1, reserved2 :10, NW :1, CD :1, PG :1;
	};
	size_t value;
} PACKED;

union ControlRegister3 {
	struct __cr3_bitfield {
		size_t reserved :3, writethough :1, cache_disable :1;
	};
	size_t value;
} PACKED;

union ControlRegister4 {
	struct __cr4_bitfield {
		size_t VME :1, PVI :1, TSD :1, DE :1, PSE :1, PAE :1, MCE :1, PGE :1, PCE :1, OSFXSR :1, OSXMMEXCPT :1, reserved :2, VMXE :1, SMXE :1;
	};
	size_t value;
} PACKED;

END_PACKED_REGION
