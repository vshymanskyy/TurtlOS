#include <string.h>
#include <ctype.h>
#include <memory.h>

#ifdef TEST
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#endif

static const char* symb = "0123456789abcdefghijklmnopqrstuvwxyz";

static const signed char vals[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	-1, -1, -1, -1, -1, -1, -1,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
	-1, -1, -1, -1, -1, -1,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
};

char*
itoa(int value, char* buf, int base)
{
	char* p = buf;
	unsigned int ud;
	if (base == 10 && value < 0) {
		*p++ = '-';
		buf++;
		ud = -value;
	} else {
		ud = value;
	}
	do {
		*p++ = symb[ud%base];
	} while (ud /= base);
	*p = '\0';
	return (char*)memrev(buf, p - 1);
}

long
strtol (const char* buf, char**  endptr, int base)
{
	assert(buf != NULL);
	assert((base>=2 && base<=36) || base==0);

	while (isspace(*buf))
		buf++;

	bool n = (*buf == '-');
	if (n || *buf == '+')
		buf++;

	if (*buf == '0' && *(buf+1) == 'x') {
		buf += 2;
		base = 16;
	} else if (*buf == '0') {
		buf += 1;
		base = 8;
	} else if (base == 0) {
		base = 10;
	}

	if (*buf == '\0')
		return 0;

	long result = 0;
	signed char val;
	while(((val = vals[static_cast<int>(*buf++)])>=0) && (val < base))
		result = result*base + val;
	if (endptr)
		*endptr = ((char*)buf)-1;
	if (n)
		return -result;
	return result;
}

#ifdef TEST

#define __string(x) #x
#define TEST_CASE(msg) if (!(msg)) { result++; printf("%s:%d: test failed: %s\n", __FILE__, __LINE__, __string(msg)); }

int main() {
	int result = 0;
	TEST_CASE(strtol("1024", NULL, 10) == 1024);
	TEST_CASE(strtol("0x1024", NULL, 10) == 0x1024);
	TEST_CASE(strtol("01024", NULL, 10) == 01024);
	TEST_CASE(strtol("1024", NULL, 16) == 0x1024);

	const char* str = "   10010010210100";
	char* end = NULL;
	strtol(str, &end, 2);
	TEST_CASE(end == &str[11]);
	return result;
}

#endif

