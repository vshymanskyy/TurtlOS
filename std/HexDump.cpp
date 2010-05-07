#include <std/HexDump.h>
#include <std/klib.h>

static
bool
isprint(unsigned char c)
{
	switch (c) {
	case '\n':
	case '\r':
	case '\b':
	case '\t':
	case '\0':
		return false;
	}
	return true;
}

void
hexdump(const void* data, size_t length, unsigned columnsMask)
{
	const unsigned width = 16;

	const unsigned char* b = static_cast<const unsigned char*>(data);
	const unsigned char* const e = b + length;
	while (b < e) {
		const unsigned char* const b2 = b;
		const unsigned char* const e2 = b + width;

		if (columnsMask & HEXDMP_ADDR) {
			debug_print("%08x: ", (size_t)b);
		}
		if (columnsMask & HEXDMP_DATA) {
			while (b < e2) {
				if (b < e) {
					debug_print("%02X ", *b & 0xFF);
				} else {
					debug_print("   ");
				}
				b++;
			}
		}
		if (columnsMask & HEXDMP_CHARS) {
			debug_print("| ");
			b = b2;
			while (b < e2) {
				unsigned char c = *b & 0xFF;
				if (b >= e) {
					c = ' ';
				} else if (!isprint(c)) {
					c = '.';
				}
				debug_print("%c", c);
				b++;
			}
		}
		debug_print("\n");
	}
}

