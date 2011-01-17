#pragma once

inline
uint8_t
inportb(const uint16_t port)
{
	uint8_t val;
	asm (
			"inb %1, %0"
			: "=a" (val)
			: "d" (port)
		);
	return val;
}

inline
uint16_t
inportw(const uint16_t port)
{
	uint16_t val;
	asm (
			"inw %1, %0"
			: "=a" (val)
			: "d" (port)
		);
	return val;
}

inline
void
outportb(const uint16_t port, const uint8_t data)
{
	asm (
			"outb %1, %0"
			:
			: "d" (port),
			"a" (data)
		);
}

inline
void
outportw(const uint16_t port, const uint16_t data)
{
	asm (
			"outw %1, %0"
			:
			: "d" (port),
			"a" (data)
		);
}

/*
inline
uint32_t
inportl(const uint16_t port)
{
	uint32_t val;
	asm (
			"inl %1, %0"
			: "=a" (val)
			: "d" (port)
		);
	return val;
}

inline
void
inportsb(const uint16_t port, void* buffer, const int count)
{
	asm (
			"cld; rep; insb"
			:
			: "d" (port),
			"D" (buffer),
			"c" (count)
		);
}

inline
void
inportsw(const uint16_t port, void* buffer, const int count)
{
	asm (
			"cld; rep; insw"
			:
			: "d" (port),
			"D" (buffer),
			"c" (count)
		);
}

inline
void
inportsl(const uint16_t port, void* buffer, const int count)
{
	asm (
			"cld; rep; insl"
			:
			: "d" (port),
			"D" (buffer),
			"c" (count)
		);
}



inline
void
outportl(const uint16_t port, const uint32_t data)
{
	asm (
			"outl %1, %0"
			:
			: "d" (port),
			"a" (data)
		);
}

inline
void
outportsb(const uint16_t port, const void* buffer, const int count)
{
	asm (
			"cld; rep; outsb"
			:
			: "d" (port),
			"S" (buffer),
			"c" (count)
		);
}

inline
void
outportsw(const uint16_t port, const void* buffer, const int count)
{
	asm (
			"cld; rep; outsw"
			:
			: "d" (port),
			"S" (buffer),
			"c" (count)
		);
}

inline
void
outportsl(const uint16_t port, const void* buffer, const int count)
{
	asm (
			"cld; rep; outsl"
			:
			: "d" (port),
			"S" (buffer),
			"c" (count)
		);
}

inline
uint16_t inportw(const uint16_t port) {
	uint16_t res=0;
	asm {
		mov dx, port;
		in	ax, dx;
		mov [res], ax;
	}
	return res;
}

inline
uint32_t inportl(const uint16_t port) {
	uint32_t res=0;
	asm {
		mov dx, port;
		in	eax, dx;
		mov [res], eax;
	}
	return res;
}

inline
void outportw(const uint16_t port, const uint16_t value) {
	asm {
		mov	dx, [port];
		mov	ax, [value];
		out	dx, ax;
	}
}

inline
void outportl(const uint16_t port, const uint32_t value) {
	asm {
		mov	dx, [port];
		mov	eax, [value];
		out	dx, eax;
	}
}

inline
void inportsb(const uint16_t port, const void* buffer, const int n){
	asm {
		mov	dx, [port];
		mov xdi, [buffer];
		mov	ecx, [n];
		rep insb;
	}
}

inline
void inportsw(const uint16_t port, const uint16_t* buffer, const int n){
	asm {
		mov	dx, [port];
		mov xdi, [buffer];
		mov	ecx, [n];
		cld;
		rep insw;
	}
}

inline
void inportsl(const uint16_t port, const uint32_t* buffer, const int n){
	asm {
		mov	dx,	[port];
		mov xdi, [buffer];
		mov	ecx, [n];
		cld;
		rep insd;
	}
}

inline
void outportsb(const uint8_t* buffer, const int n, const uint16_t port){
	asm {
		mov	dx, [port];
		mov xsi, [buffer];
		mov	ecx, [n];
		cld;
		rep outsb;
	}
}

inline
void outportsw(const uint16_t* buffer, const int n, const uint16_t port){
	asm {
		mov	dx, [port];
		mov xsi, [buffer];
		mov	ecx, [n];
		cld;
		rep outsw;
	}
}

inline
void outportsl(const uint32_t* buffer, const int n, const uint16_t port){
	asm {
		mov	dx, [port];
		mov xsi, [buffer];
		mov	ecx, [n];
		cld;
		rep outsd;
	}
}
*/