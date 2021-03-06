PAGETABLE_PML4_FLAGS	equ 0x000000000000000f
PAGETABLE_PDP_FLAGS		equ 0x000000000000000f
PAGETABLE_PD_FLAGS		equ 0x000000000000018f

align 4096
pml4:
	dq PAGETABLE_PML4_FLAGS + pdp
	times 511 dq 0x0000000000000000

pdp:
	dq PAGETABLE_PDP_FLAGS + pd
	dq 0x0000000000000000
	dq 0x0000000000000000
	dq PAGETABLE_PDP_FLAGS + pd1
	times 508 dq 0x0000000000000000

pd:
	%assign addr 0x0000000
	%rep    512
		dq  addr | PAGETABLE_PD_FLAGS
	%assign addr addr+0x0200000
	%endrep

pd1:
	times 503 dq 0x0000000000000000
	dq  0xfee00000 | PAGETABLE_PD_FLAGS
	times 8 dq 0x0000000000000000

