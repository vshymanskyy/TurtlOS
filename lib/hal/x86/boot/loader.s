
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

	; load new gdt
	lgdt [gdtr]
	jmp GDT_SEL_CODE:.flush   ; Far jump!

.flush:
	mov ax, GDT_SEL_DATA
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov esp, stack+STACKSIZE
	mov dword[0xb8000], 'x_8_'
	mov dword[0xb8004], '6_ _'
	mov dword[0xb8008], ' _o_'
	mov dword[0xb800c], 'k_._'

	jmp entry

; ========================================================
;  Stack
; ========================================================
[section .bss]
align 32
stack:
	resb STACKSIZE			; reserve stack on a quadword boundary
