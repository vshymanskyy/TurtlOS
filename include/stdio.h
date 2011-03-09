#ifndef INC_STDIO_H
#define INC_STDIO_H

#if _MSC_VER > 1000
	#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* FILE;

FILE* fopen(const char* fn, const char*mode);
void fclose(FILE* f);
int fprintf(FILE* f, const char* format, ...);

int printf(const char* format, ...);

int vsnprintf(char* buffer, size_t length, const char* format, va_list args);
int snprintf(char* buffer, size_t qty, const char* format, ...);

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

#define stdin stdin
#define stdout stdout
#define stderr stderr

#ifdef __cplusplus
}
#endif

#endif /* INC_STDIO_H */
