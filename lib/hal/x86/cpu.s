extern isr_handler

global halCpuFlushIdt
global halCpuFlushGdt
global halCpuFlushTss

[section .text]
[bits 32]

halCpuFlushIdt:
	mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter.
	lidt [eax]        ; Load the IDT pointer.
	ret


; This macro creates a stub for an ISR which does NOT pass it's own
; error code (adds a dummy errcode byte).
%macro ISR_NOERR 1
global isr%1
isr%1:
	cli                         ; Disable interrupts firstly.
	push byte 0                 ; Push a dummy error code.
	push  %1                    ; Push the interrupt number.
	jmp isr_common_stub         ; Go to our common handler code.
%endmacro

; This macro creates a stub for an ISR which passes it's own
; error code.
%macro ISR_ERROR 1
global isr%1
isr%1:
	cli                         ; Disable interrupts.
	push %1                     ; Push the interrupt number
	jmp isr_common_stub
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERROR 8
ISR_NOERR 9
ISR_ERROR 10
ISR_ERROR 11
ISR_ERROR 12
ISR_ERROR 13
ISR_ERROR 14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

ISR_NOERR 32
ISR_NOERR 33
ISR_NOERR 34
ISR_NOERR 35
ISR_NOERR 36
ISR_NOERR 37
ISR_NOERR 38
ISR_NOERR 39
ISR_NOERR 40
ISR_NOERR 41
ISR_NOERR 42
ISR_NOERR 43
ISR_NOERR 44
ISR_NOERR 45
ISR_NOERR 46
ISR_NOERR 47

ISR_NOERR 128

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
	pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

	mov ecx, esp

	;mov ax, ds               ; Lower 16-bits of eax = ds.
	;push eax                 ; save the data segment descriptor

	;mov ax, 0x08  ; load the kernel data segment descriptor
	;mov ds, ax
	;mov es, ax
	;mov fs, ax
	;mov gs, ax


	call isr_handler

	;pop ebx        ; reload the original data segment descriptor
	;mov ds, bx
	;mov es, bx
	;mov fs, bx
	;mov gs, bx

	popa                     ; Pops edi,esi,ebp...
	add esp, 8    ; Cleans up the pushed error code and pushed ISR number
	sti
	iretd         ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
