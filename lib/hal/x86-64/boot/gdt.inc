GDT_SEL_DATA	equ gdt.data - gdt
GDT_SEL_CODE32	equ gdt.code32 - gdt
GDT_SEL_CODE64	equ gdt.code64 - gdt

align 8
gdtr:
	dw gdt-gdt.end-1
	dq gdt

align 8
gdt:
	dq 0x0000000000000000
.data:
	dw 0xFFFF,0,0x9200,0x00CF
.code32:
	dw 0xFFFF,0,0x9A00,0x00CF
.code64:
	dw 0xFFFF,0,0x9A00,0x00AF
.end:

