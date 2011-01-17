#include <memory.h>
#include <stdint.h>

void*
memrev(void* b, void* e)
{
	char* beg = (char*)b;
	char* end = (char*)e;
	while (end > beg) {
		char tmp = *end;
		*end-- = *beg;
		*beg++ = tmp;
	}
	return b;
}

void
swab(const void* src, void* dest, size_t n)
{
	size_t i;
	const char* s = (const char*)src;
	char* d = (char*)dest;
	n = n & (size_t)(~1);
	for(i = 0; i < n; i += 2) {
		d[i]=s[i+1];
		d[i+1]=s[i];
	}
}

char
memsum(void* const s, size_t n)
{
	char result = 0;
	char* d = (char*)s;
	while (n) {
		n -= 1;
		result = (char)(result + *(d++));
	}
	return result;
}

void*
memcpy(void* dest, const void* src, size_t count)
{
	size_t* dq;
	uint8_t* db;
	const size_t* sx;
	const uint8_t* sb;

	dq = (size_t*)dest;
	sx = (const size_t*)src;
	while(count >= sizeof(size_t)) {
		*dq++ = *sx++;
		count -= sizeof(size_t);
	}
	db = (uint8_t*)dq;
	sb = (const uint8_t*)sx;
	while(count--) {
		*db++ = *sb++;
	}
	return dest;
}

void*
memset(void* dest, uint8_t val, size_t count)
{
	size_t* dq;
	uint8_t* db;
	size_t vx = val;
	unsigned i;
	for(i=8; i<sizeof(size_t)*8; i*=2) {
		vx |= (vx << i);
	}
	dq = (size_t*)dest;
	while(count >= sizeof(size_t)) {
		*dq++ = vx;
		count -= sizeof(size_t);
	}
	db = (uint8_t*)dq;
	while(count--) {
		*db++ = val;
	}
	return dest;
}

void*
memsetw(void* dest, uint16_t val, size_t count)
{
	uint16_t* d = (uint16_t*)dest;
	while(count--) {
		*d++ = val;
	}
	return dest;
}

void*
memsetd(void* dest, uint32_t val, size_t count)
{
	uint32_t* d = (uint32_t*)dest;
	while(count--) {
		*d++ = val;
	}
	return dest;
}

void*
memzero(void* dest, size_t count)
{
	size_t* dx;
	uint8_t* db;
	dx = (size_t*)dest;
	while(count >= sizeof(size_t)) {
		*dx++ = 0;
		count -= sizeof(size_t);
	}
	db = (uint8_t*)dx;
	while(count--) {
		*db++ = 0;
	}
	return dest;
}

void*
memmove(void* dest, const void* src, size_t count)
{
	size_t* dq;
	const size_t* sx;
	uint8_t* db;
	const uint8_t* sb;

	if (dest < src) {
		return memcpy(dest, src, count);
	}else if (dest > src) {
		dq = (size_t*)((char*)dest + count);
		sx = (const size_t*)((const char*)src + count);
		while(count >= sizeof(size_t)) {
			*--dq = *--sx;
			count -= sizeof(size_t);
		}
		db = (uint8_t*)dq;
		sb = (const uint8_t*)sx;
		while(count--) {
			*--db = *--sb;
		}
		return dest;
	}
	return NULL; /*to make gcc happy*/
}

int
memcmp(const void* s1, const void* s2, size_t n)
{
	const unsigned char* p1 = (const unsigned char*) s1;
	const unsigned char* p2 = (const unsigned char*) s2;
	while(n--) {
		if (*p1 != *p2)
			return *p1 - *p2;
		++p1;
		++p2;
	}
	return 0;
}
