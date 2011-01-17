%include "../../common/boot/linker.inc"

MULTIBOOT_PAGE_ALIGN	equ 1<<0
MULTIBOOT_MEMORY_INFO	equ 1<<1
MULTIBOOT_AOUT_KLUDGE	equ 1<<16

MULTIBOOT_SET_VIDEO		equ 1<<2
MULTIBOOT_MMAP_INFO		equ 1<<6

MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
MULTIBOOT_CHECKSUM		equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

; ========================================================
;  Multiboot structure
; ========================================================
align 4
mboot:
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM
	dd mboot
	dd __textSectStart
	dd __dataSectEnd
	dd __kernelEnd
	dd loader
	dd 0
	dd 1024
	dd 768
	dd 24

