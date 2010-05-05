#include <std/klib.h>

void
swab(const void* src, void* dest, size_t n)
{
	const char* s = (const char*)src;
	char* d = (char*)dest;
	n&=~1;
	for(size_t i=0; i<n; i+=2) {
		d[i]=s[i+1];
		d[i+1]=s[i];
	}
}

char
memsum(void* const s, size_t n)
{
	char result = 0;
	char* d = (char*)s;
	while(n--)
		result = static_cast<char>(result + *(d++));
	return result;
}

void*
memcpy(void* dest, const void* src, size_t count)
{
	size_t* dq = (size_t*)dest;
	const size_t* sx = (const size_t*)src;
	while(count >= sizeof(size_t)) {
		*dq++ = *sx++;
		count -= sizeof(size_t);
	}
	uint8_t* db = (uint8_t*)dq;
	const uint8_t* sb = (const uint8_t*)sx;
	while(count--) {
		*db++ = *sb++;
	}
	return dest;
}

void*
memset(void* dest, uint8_t val, size_t count)
{
	size_t vx = val;
	for(unsigned i=8; i<sizeof(size_t)*8; i*=2) {
		vx |= (vx << i);
	}
	size_t* dq = (size_t*)dest;
	while(count >= sizeof(size_t)) {
		*dq++ = vx;
		count -= sizeof(size_t);
	}
	uint8_t* db = (uint8_t*)dq;
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
	size_t* dx = (size_t*)dest;
	while(count >= sizeof(size_t)) {
		*dx++ = 0;
		count -= sizeof(size_t);
	}
	uint8_t* db = (uint8_t*)dx;
	while(count--) {
		*db++ = 0;
	}
	return dest;
}

void*
memmove(void* dest, const void* src, size_t count)
{
	if (dest < src) {
		return memcpy(dest, src, count);
	}else if (dest > src) {
		size_t* dq = (size_t*)((char*)dest + count);
		const size_t* sx = (const size_t*)((const char*)src + count);
		while(count >= sizeof(size_t)) {
			*--dq = *--sx;
			count -= sizeof(size_t);
		}
		uint8_t* db = (uint8_t*)dq;
		const uint8_t* sb = (const uint8_t*)sx;
		while(count--) {
			*--db = *--sb;
		}
		return dest;
	}
	return NULL; //to make gcc happy
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

