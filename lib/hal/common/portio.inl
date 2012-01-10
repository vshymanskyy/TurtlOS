#include <hal/portio.h>

inline
uint8_t
inb(const uint16_t port)
{
	uint8_t data;
    asm ( "inb %1, %0" : "=a"(data) : "Nd"(port) );
    return data;
}

inline
uint16_t
inw(const uint16_t port)
{
	uint16_t data;
    asm ( "inw %1, %0" : "=a"(data) : "Nd"(port) );
    return data;
}

inline
uint32_t
inl(const uint16_t port)
{
	uint32_t data;
    asm ( "inl %1, %0" : "=a"(data) : "Nd"(port) );
    return data;
}

inline
void
outb(const uint16_t port, const uint8_t data)
{
    asm ( "outb %0, %1" : : "a"(data), "Nd"(port) );
}

inline
void
outw(const uint16_t port, const uint16_t data)
{
    asm ( "outw %0, %1" : : "a"(data), "Nd"(port) );
}

inline
void
outl(const uint16_t port, const uint32_t data)
{
    asm ( "outl %0, %1" : : "a"(data), "Nd"(port) );
}

inline
void
io_wait(void)
{
    asm ( "outb %%al, $0x80" : : "a"(0) );
}
