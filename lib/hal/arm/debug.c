volatile unsigned int* const UART0DR = (unsigned int *)0x10009000;

void
debug_putc (const char c)
{
	*UART0DR = (unsigned int)(c);
}
