#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <cpu_structs.h>

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
fatal_soft(const char* msg, const char* file, const int line, const char* func)
{
	debug_puts("Fatal soft: '");
	debug_puts(msg);
	debug_puts("' at ");
	debug_puts(file);
	debug_puts(", ");
	debug_puts(func);
	debug_puts("\n");
	cpuStop();
	for(;;) {
	}
}

void
assert_failed(const char* msg, const char* file, const int line, const char* func)
{
	debug_puts("Assertion failed: '");
	debug_puts(msg);
	debug_puts("' at ");
	debug_puts(file);
	debug_puts(", ");
	debug_puts(func);
	debug_puts("\n");
	cpuStop();
	for(;;) {
	}
}
