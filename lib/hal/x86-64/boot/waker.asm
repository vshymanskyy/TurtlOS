
CONFIG equ 0x600

struc config
	.mode	resb	1
	.id		resb	1
	.entry	resq	1
	.stack	resq	1
endstruc

; ========================================================
;  Code
; ========================================================
[org 0x1000]
[bits 16]

jmp waker

%include "gdt.inc"
%include "pagetable.inc"

waker:
	cli
	xor ax, ax
	mov ds, ax
	mov ss, ax
	mov sp, 0x7C00

	; enable protected mode
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	; load gdt and jump to PMODE
	lgdt [gdtr]

	jmp	GDT_SEL_CODE32 : pmode

[bits 32]
pmode:
	mov	ax, GDT_SEL_DATA
	mov ds, ax
	mov	ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov esp, 0x7C00

	; enable PAE
	mov eax, cr4
	bts eax, 5
	mov cr4, eax

	; EFER MSR: enable long mode
	mov ecx, 0xc0000080
	rdmsr
	bts eax, 8
	wrmsr

[bits 64]
	; load page-map level-4
	mov rax, pml4
	mov cr3, rax
[bits 32]
	; enable paging
	mov eax, cr0
	or  eax, 0x80000000
	mov cr0, eax

	jmp GDT_SEL_CODE64 : lmode

[bits 64]
lmode:
	mov rsp, qword[CONFIG + config.stack]

	call qword[CONFIG + config.entry]
