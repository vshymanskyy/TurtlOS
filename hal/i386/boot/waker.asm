
CONFIG equ 0x600

struc config
	.mode	resb	1
	.id		resb	1
	.entry	resd	1
endstruc

; ========================================================
;  Code
; ========================================================
[org 0x1000]
[bits 16]

jmp waker

%include "gdt.inc"


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

	jmp	GDT_SEL_CODE : pmode

[bits 32]
pmode:
	mov	ax, GDT_SEL_DATA
	mov ds, ax
	mov	ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov esp, 0x7C00

	call dword[CONFIG + config.entry]

