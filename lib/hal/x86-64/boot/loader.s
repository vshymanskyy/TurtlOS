
global loader
global mbiMagic
global mbiTable

extern entry

[section .loader progbits exec alloc nowrite]
[bits 32]

; ========================================================
;  Data
; ========================================================

%include "../../common-x86/boot/mboot.inc"
%include "gdt.inc"
%include "pagetable.inc"

mbiMagic dd 0
mbiTable dd 0

STACKSIZE equ 0x1000

; ========================================================
;  Code
; ========================================================

loader:
	cli
	mov esp, stack+STACKSIZE
	mov [mbiMagic], eax
	mov [mbiTable], ebx

	mov dword[0xb8000], 'L_o_'
	mov dword[0xb8004], 'a_d_'
	mov dword[0xb8008], 'i_n_'
	mov dword[0xb800c], 'g_._'
	mov dword[0xb8010], '._._'

	; test x86-64 extensions
	mov eax, 80000000h
	cpuid
	cmp eax, 80000000h
	jbe no64
	mov eax, 80000001h
	cpuid
	bt edx, 29				; 64-bit if bit 29 is set
	jc ok64

no64:
	mov dword[0xb8000], 'N_o_'
	mov dword[0xb8004], ' _x_'
	mov dword[0xb8008], '8_6_'
	mov dword[0xb800c], '-_6_'
	mov dword[0xb8010], '4_._'
	hlt

ok64:
	; enable PAE
	mov eax, cr4
	bts eax, 5
	mov cr4, eax

	; EFER MSR: enable long mode
	mov ecx, 0xc0000080
	rdmsr
	bts eax, 8
	wrmsr

	; load page-map level-4
	mov eax, pml4
	mov cr3, eax

	; enable paging
	mov eax, cr0
	or  eax, 0x80000000
	mov cr0, eax

	; load new gdt
	lgdt [gdtr]

	jmp GDT_SEL_CODE64 : lmode

[bits 64]
lmode:
	mov ax, GDT_SEL_DATA
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov rsp, stack+STACKSIZE
	mov dword[0xb8000], 'x_8_'
	mov dword[0xb8004], '6_-_'
	mov dword[0xb8008], '6_4_'
	mov dword[0xb800c], ' _o_'
	mov dword[0xb8010], 'k_._'

	jmp entry

; ========================================================
;  Stack
; ========================================================
[section .bss]
align 32
stack:
	resb STACKSIZE			; reserve stack on a quadword boundary
