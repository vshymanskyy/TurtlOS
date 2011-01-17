
global loader
global mbiMagic
global mbiTable

extern entry

[section .loader progbits exec alloc nowrite]
[bits 32]

; ========================================================
;  Data
; ========================================================

%include "../../common/boot/mboot.inc"
%include "gdt.inc"


mbiMagic dd 0
mbiTable dd 0

STACKSIZE equ 0x4000

; ========================================================
;  Code
; ========================================================

loader:
	cli
	mov esp, stack+STACKSIZE
	mov [mbiMagic], eax
	mov [mbiTable], ebx

	mov dword[0xB8000], 'L_o_'
	mov dword[0xB8004], 'a_d_'
	mov dword[0xB8008], 'i_n_'
	mov dword[0xB800C], 'g_._'
	mov dword[0xB8010], '._._'

	jmp entry

; ========================================================
;  Stack
; ========================================================
[section .bss]
align 32
stack:
	resb STACKSIZE			; reserve 16k stack on a quadword boundary
