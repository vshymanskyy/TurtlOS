GDT_SEL_DATA	equ gdt.data - gdt
GDT_SEL_CODE	equ gdt.code - gdt

align 8
gdtr:
	dw gdt-gdt.end-1
	dd gdt

align 8
gdt:
	dq 0x0000000000000000
.data:
	dw 0xFFFF,0,0x9200,0x00CF
.code:
	dw 0xFFFF,0,0x9A00,0x00CF
.end:

