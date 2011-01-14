#include <string.h>
static const char* symbSmall = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char* symbCapital = "0123456789abcdefghijklmnopqrstuvwxyz";


static
char*
putSigned (long long value, char* buf, long base, bool capt, bool plus)
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
	do {
		*p++ = (capt?symbCapital:symbSmall)[ud%base];
	} while (ud /= base);
	memrev(buf, p - 1);
	return p;
}

static
char*
putUnsigned (unsigned long long value, char* buf, long base, bool capt)
{
	char* p = buf;
	do {
		*p++ = (capt?symbCapital:symbSmall)[value%base];
	} while (value /= base);
	memrev(buf, p - 1);
	return p;
}

int
vsnprintf (char* buffer, unsigned count, const char* format, va_list args)
{
	argused(count);
	char* p = buffer;
	for (; *format; format++) {
		if (*format == '%') {
			format++;
			if (*format == '%') {
				*p++ = *format++;
				continue;
			}

			bool space = false;
			bool left = false;
			bool plus = false;
			bool zero = false;
			bool spec = false;
			bool sign = false;
			bool capt = false;
			bool expo = false;
			long width = -1;
			long prec = -1;
			long length = -1;
			long base = -1;

			// flags
			while (true) {
				switch (*format++) {
					case ' ': space = true; continue;
					case '-': left = true;  continue;
					case '+': plus = true;  continue;
					case '0': zero = true;  continue;
					case '#': spec = true;  continue;
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
				left = true;
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
				sign = true;
				goto integer;
			case 'o': // Signed octal
				base = 8;
				sign = true;
				goto integer;
			case 'u': // Unsigned decimal integer
				base = 10;
				sign = false;
				goto integer;
			case 'X': // Unsigned hexadecimal integer (capital letters)
				capt = true;
			case 'x': // Unsigned hexadecimal integer
				base = 16;
				sign = false;
				goto integer;
			case 'p': // Pointer address
				base = 16;
				sign = false;
				zero = true;
				width = sizeof(size_t)*2;
				goto integer;

			case 'E': // Floating point variations
				capt = true;
			case 'e':
				expo = true;
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
				p += length;
				continue;
			}
			integer: {
				char* prevP = p;
				if (sign) {
					if (length == 2) {
						int16_t value = va_arg(args, int16_t);
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
						uint16_t value = va_arg(args, uint16_t);
						p = putUnsigned(value, p, base, capt);
					} else if (length == 4) {
						uint32_t value = va_arg(args, uint32_t);
						p = putUnsigned(value, p, base, capt);
					} else {
						uint64_t value = va_arg(args, uint64_t);
						p = putUnsigned(value, p, base, capt);
					}
				}
				ptrdiff_t curWidth = p - prevP;
				width -= curWidth;
				while (width-- > 0) {
					*p++ = (zero)?'0': ' ';
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

