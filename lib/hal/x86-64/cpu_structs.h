#pragma once
#include <platform.h>

#include "../common-x86/cpu-common-x86.h"

#define CPU_GDT_SEL_DATA 0x08
#define CPU_GDT_SEL_CODE32 0x10
#define CPU_GDT_SEL_CODE 0x18

BEGIN_PACKED_REGION

typedef struct RegisterFrame {
	// PushAll
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
	// Push from handler
	uint64_t interrupt, error;
	// Pushed on interrupt
	uint64_t rip, cs, rflags, rspPrev, ss;
} PACKED RegisterFrame_t;

/// Pointer to global descriptor table
typedef struct {
	uint16_t limit; // The upper 16 bits of all selector limits.
	uint64_t base; // The address of the first gdt_entry_t struct.
} PACKED GdtPtr;

/// Pointer to interrupt descriptor table
typedef struct IdtPtr {
	uint16_t limit;
	uint64_t base; // The address of the first element in our idt_entry_t array.
} PACKED IdtPtr;

/// Global descriptor table entry
typedef struct {
	uint16_t limit_low; // The lower 16 bits of the limit.
	uint16_t base_low; // The lower 16 bits of the base.
	uint8_t base_middle; // The next 8 bits of the base.
	uint8_t access; // Access flags, determine what ring this segment can be used in.
	uint8_t granularity;
	uint8_t base_high; // The last 8 bits of the base.
} PACKED GdtEntry;

/// Interrupt descriptor table entry
typedef struct {
	uint16_t base_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
	uint16_t sel; // Kernel segment selector.
	uint8_t reserved0; // This must always be zero.
	uint8_t flags; // More flags. See documentation.
	uint16_t base_hi; // The upper 16 bits of the address to jump to.
	uint32_t base_hi2;
	uint32_t reserved1;
} PACKED IdtEntry;

typedef union {
	uint32_t value;
	uint16_t words[2];
	uint8_t bytes[4];
} PACKED CpuIdRegister;

typedef struct {
	CpuIdRegister a, b, c, d;
} PACKED CpuIdResult;

typedef union {
	struct __cr0_bitfield {
		uint64_t
			PE : 1,
			MP : 1,
			EM : 1,
			TS : 1,
			ET : 1,
			NE : 1,
			reserved0 : 10,
			WP : 1,
			reserved1 : 1,
			AM : 1,
			reserved2 : 10,
			NW : 1,
			CD : 1,
			PG : 1;
	};
	uint64_t value;
} PACKED ControlRegister0;

typedef union {
	struct __cr3_bitfield {
		uint64_t
			reserved0 : 3,
			writethough : 1,
			disable : 1;
	};
	uint64_t value;
} PACKED ControlRegister3;

typedef union {
	struct __cr4_bitfield {
		uint64_t
			VME : 1,
			PVI : 1,
			TSD : 1,
			DE : 1,
			PSE : 1,
			PAE : 1,
			MCE : 1,
			PGE : 1,
			PCE : 1,
			OSFXSR : 1,
			OSXMMEXCPT : 1,
			reserved0 : 2,
			VMXE : 1,
			SMXE : 1;
	};
	uint64_t value;
} ControlRegister4;

typedef union {
	struct __pml4_entry_bitfield {
		uint64_t
			present : 1,
			writable : 1,
			user : 1,
			writethough : 1,
			cache_disable :1,
			accessed : 1,
			ign : 1,
			mbz : 2,
			avl : 3,
			address : 40,
			avaliable : 11,
			nx : 1;
	};
	uint64_t value;
} PACKED PagePml4Entry;

typedef union {
	struct __pdp_entry_bitfield {
		uint64_t
			present : 1,
			writable : 1,
			user : 1,
			writethough : 1,
			cache_disable :1,
			accessed : 1,
			ign : 1,
			big : 1,
			mbz : 1,
			avl : 3,
			address : 40,
			avaliable : 11,
			nx : 1;
	};
	uint64_t value;
} PACKED PagePdpEntry;

typedef union {
	struct __pd_entry_bitfield {
		uint64_t
			present : 1,
			writable : 1,
			user : 1,
			writethough : 1,
			cache_disable :1,
			accessed : 1,
			dirty : 1,
			big : 1,
			global : 1,
			avl : 3,
			pat : 1,
			mbz : 8,
			address : 31,
			avaliable : 11,
			nx : 1;
	};
	uint64_t value;
} PACKED PagePdEntry;

END_PACKED_REGION
