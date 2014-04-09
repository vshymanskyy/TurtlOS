#include <cpu_structs.h>
#include <stdint.h>

#if 0

void
debug_putc (const char c)
{
	asm ( "outb %0, %1" : : "a"(c), "Nd"(0xe9) );
}

#else

void
debug_putc (const char c)
{
	static uint16_t* buff = (uint16_t*)0xB8000;
	switch (c) {
	case '\t':
		{
		memsetw(buff, 0x4E00, 4);
		buff += 4;
		break;
		}
	case '\n':
		{
		size_t len = 80 - (((size_t)buff-0xB8000)/2 + 80) % 80;
		memsetw(buff, 0x4E00, len);
		buff += len;
		break;
		}
	default:
		*buff++ = (uint16_t)(0x4E00 | c);
		break;
	}
	if ((size_t)buff >= 0xB8000+(80*25*2)) {
		buff = (uint16_t*)0xB8000;
	}
}

#endif
