#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const char* symbCaps = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char* symbNorm = "0123456789abcdefghijklmnopqrstuvwxyz";

static
char*
putSigned (long long value, char* buf, long base, unsigned capt, unsigned plus)
{
	long long ud;
	if ((base == 10 || base == 8) && value < 0) {
		*buf++ = '-';
		ud = -value;
	} else {
		if (plus)
			*buf++ = '+';
		ud = value;
	}
	char* p = buf;
	if (capt) {
		do {
			*p++ = symbCaps[ud%base];
		} while (ud /= base);
	} else {
		do {
			*p++ = symbNorm[ud%base];
		} while (ud /= base);
	}
	memrev(buf, p - 1);
	return p;
}

static
char*
putUnsigned (unsigned long long value, char* buf, long base, unsigned capt)
{
	char* p = buf;
	if (capt) {
		do {
			*p++ = symbCaps[value%base];
		} while (value /= base);
	} else {
		do {
			*p++ = symbNorm[value%base];
		} while (value /= base);
	}
	memrev(buf, p - 1);
	return p;
}

int
vsnprintf (char* buffer, size_t buffSize, const char* format, va_list args)
{
	// TODO: implement buffSize handling

	char* p = buffer;
	for (; *format; format++) {
		if (*format == '%') {
			format++;
			if (*format == '%') {
				*p++ = *format++;
				continue;
			}

			unsigned space = 0;
			unsigned left = 0;
			unsigned plus = 0;
			unsigned zero = 0;
			unsigned spec = 0;
			unsigned sign = 0;
			unsigned capt = 0;
			unsigned expo = 0;
			long width = -1;
			long prec = -1;
			long length = -1;
			long base = -1;

			// flags
			while (1) {
				switch (*format++) {
					case ' ': space = 1; continue;
					case '^': capt = 1;  continue;
					case '-': left = 1;  continue;
					case '+': plus = 1;  continue;
					case '0': zero = 1;  continue;
					case '#': spec = 1;  continue;
				}
				format--;
				break;
			}

			// width
			if (*format == '*') {
				format++;
				width = va_arg(args, int);
			} else {
				width = strtol(format, (char**)&format, 10);
			}
			if (width < 0) {
				width = -width;
				left = 1;
			}

			// precision
			if (*format == '.') {
				if (*(++format) == '*') {
					format++;
					prec = va_arg(args, int);
				} else {
					prec = strtol(format, (char**)&format, 10);
				}
				if (prec <= 0) {
					prec = -1;
				}
			} else {
				prec = -1;
			}

			// length
			switch (*format) {
			case 'h': // Short
				format++;
				length = 2;
				break;
			case 'l': // Long
				format++;
				length = 8;
				break;
			case 'L': // Long double
				format++;
				length = 8;
				break;
			default:
				length = 4;
			}

			switch (*format) {

			case 'c': // Character
				*p++  = (unsigned char)va_arg(args, int);
				continue;
			case 's': // String of characters
				goto string;

			case 'd': // Signed decimal integer
			case 'i':
				base = 10;
				sign = 1;
				goto integer;
			case 'o': // Signed octal
				base = 8;
				sign = 1;
				goto integer;
			case 'u': // Unsigned decimal integer
				base = 10;
				sign = 0;
				goto integer;
			case 'X': // Unsigned hexadecimal integer (capital letters)
				base = 16;
				capt = 1;
				sign = 0;
				goto integer;
			case 'x': // Unsigned hexadecimal integer
				base = 16;
				capt = 0;
				sign = 0;
				goto integer;
			case 'b': // Unsigned binary integer
				base = 2;
				capt = 0;
				sign = 0;
				goto integer;
			case 'p': // Pointer address
				base = 16;
				sign = 0;
				zero = 1;
				width = sizeof(size_t)*2;
				goto integer;

			case 'E': // Floating point variations
				capt = 1;
			case 'e':
				expo = 1;
				goto floating;
			case 'f':
				goto floating;
			case 'G':
				goto floating;
			case 'g':
				goto floating;

			case 'n': // Pointer to a signed int, where the number of characters written so far is stored
				goto report;
			default:
				*p++ = *format;
				continue;
			}

			string: {
				const char* value = va_arg(args, const char*);
				const size_t len = strlen(value);
				memcpy(p, value, len);
				p += len;
				continue;
			}
			integer: {
				char* prevP = p;
				if (sign) {
					if (length == 2) {
						int value = va_arg(args, int);
						p = putSigned(value, p, base, capt, plus);
					} else if (length == 4) {
						int32_t value = va_arg(args, int32_t);
						p = putSigned(value, p, base, capt, plus);
					} else {
						int64_t value = va_arg(args, int64_t);
						p = putSigned(value, p, base, capt, plus);
					}
				} else {
					if (length == 2) {
						int value = va_arg(args, int);
						p = putUnsigned(value, p, base, capt);
					} else if (length == 4) {
						uint32_t value = va_arg(args, uint32_t);
						p = putUnsigned(value, p, base, capt);
					} else {
						uint64_t value = va_arg(args, uint64_t);
						p = putUnsigned(value, p, base, capt);
					}
				}
				if (width > 0) {
					if (left) {
						const ptrdiff_t curWidth = p - prevP;
						width -= curWidth;
						while (width-- > 0) {
							*p++ = (zero)?'0': ' ';
						}
					} else {
						const ptrdiff_t curWidth = p - prevP;
						width -= curWidth;

						if (width > 0) {
							memmove(prevP+width, prevP, curWidth);
							p += width;
							while (width-- > 0) {
								*prevP++ = (zero)?'0': ' ';
							}
						}
					}
				}
				continue;
			}
			floating: {
				continue;
			}
			report: {
				if (length == 8) {
					int64_t* ptr = va_arg(args, int64_t*);
					*ptr = (int64_t)(p - buffer);
				} else if (length == 4) {
					int32_t* ptr = va_arg(args, int32_t*);
					*ptr = (int32_t)(p - buffer);
				} else if (length == 2) {
					int16_t* ptr = va_arg(args, int16_t*);
					*ptr = (int16_t)(p - buffer);
				}
				continue;
			}
		} else {
			*p++ = *format;
		}
	}
	*p = '\0';
	return (int)(p - buffer);
}

