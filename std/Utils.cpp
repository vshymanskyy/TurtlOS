#include <std/Utils.h>

#include <ctype.h>
#include <string.h>

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


char*
printsize(const unsigned long long bytes, char* buff, size_t len)
{
	static const char titles[] = {'K','M','G','T','P','E','Z','Y'};
	static const float ratio = 1024.f;

	if (bytes < 1024) {
		snprintf(buff, len, "%llu byte%s", bytes, (bytes==1)?"":"s");
	} else {
		float size = float(bytes)/ratio;
		for(int i = 0; i < 8; i++){
			const float tmp = size/ratio;
			if (tmp > 1.0f) {
				size=tmp;
			} else {
				snprintf(buff, len, "%.2f %cB", size, titles[i]);
				break;
			}
		}
	}
	return buff;
}
