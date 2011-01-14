#include <std/klib.h>
#include <hal/cpu.h>
#include <string.h>

static
void
debug_putc (const char c)
{
	static uint16_t* buff = (uint16_t*)0xB8000;
	switch (c) {
	case '\n':
		{
		size_t len = 80 - (((size_t)buff-0xB8000)/2 + 80) % 80;
		memsetw(buff, 0x4E00, len);
		buff += len;
		break;
		}
	default:
		*buff++ = static_cast<uint16_t>(0x4E00 | c);
		break;
	}
}

static
void
debug_puts (const char* s)
{
	if (s != NULL) {
		while(*s) {
			debug_putc(*s++);
		}
	}
}

void
debug_print (const char* format, ...)
{
	static char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);
	debug_puts(buffer);
}

void
fatal_soft(const char* msg, const char* file, const int line, const char* func)
{
	debug_print("Fatal soft:\n '%s' at %s:%d (in %s)", msg, file, line, func);
	for (;;) {
		cpuStop();
	}
}

void
assert_failed(const char* msg, const char* file, const int line, const char* func)
{
	debug_print("Assertion failed:\n '%s' at %s:%d (in %s)", msg, file, line, func);
	for (;;) {
		cpuStop();
	}
}
