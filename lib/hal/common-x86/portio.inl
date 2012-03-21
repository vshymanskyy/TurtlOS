INLINE
uint8_t
inb(const uint16_t port)
{
	uint8_t data;
    asm ( "inb %1, %0" : "=a"(data) : "Nd"(port) );
    //debug_print("inb\t0x%x\t0x%x\t(0b%b)\n", port, data, data);
    return data;
}

INLINE
uint16_t
inw(const uint16_t port)
{
	uint16_t data;
    asm ( "inw %1, %0" : "=a"(data) : "Nd"(port) );
    return data;
}

INLINE
uint32_t
inl(const uint16_t port)
{
	uint32_t data;
    asm ( "inl %1, %0" : "=a"(data) : "Nd"(port) );
    return data;
}

INLINE
void
outb(const uint16_t port, const uint8_t data)
{
	//debug_print("outb\t0x%x\t0x%x\t(0b%b)\n", port, data, data);
    asm ( "outb %0, %1" : : "a"(data), "Nd"(port) );
}

INLINE
void
outw(const uint16_t port, const uint16_t data)
{
    asm ( "outw %0, %1" : : "a"(data), "Nd"(port) );
}

INLINE
void
outl(const uint16_t port, const uint32_t data)
{
    asm ( "outl %0, %1" : : "a"(data), "Nd"(port) );
}

INLINE
void
io_wait(void)
{
    asm ( "outb %%al, $0x80" : : "a"(0) );
}
