#include <stdarg.h>
#include <assert.h>

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
