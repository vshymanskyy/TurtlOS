#include <stdio.h>
#include <assert.h>

FILE* fopen(const char* fn, const char*mode)
{
}

void fclose(FILE* f)
{
}

int
printf (const char* format, ...)
{
	static char buffer[1024];
	va_list args;
	va_start(args, format);
	int result = vsnprintf(buffer, 1024, format, args);
	va_end(args);
	debug_puts(buffer);
	return result;
}

int fprintf(FILE* f, const char* format, ...)
{
	static char buffer[1024];
	va_list args;
	va_start(args, format);
	int result = vsnprintf(buffer, 1024, format, args);
	va_end(args);
	debug_puts(buffer);
	return result;
}

FILE* stdin = NULL;
FILE* stdout = NULL;
FILE* stderr = NULL;
