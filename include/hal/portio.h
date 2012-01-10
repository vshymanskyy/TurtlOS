#ifndef _INC_PORTIO_H
#define _INC_PORTIO_H

#include <global.h>

#ifdef __cplusplus
extern "C" {
#endif

	uint8_t  inb(const uint16_t port);
	uint16_t inw(const uint16_t port);
	uint32_t inportl(const uint16_t port);
	void inportsb(const uint16_t port, void* buffer, const int count);
	void inportsw(const uint16_t port, void* buffer, const int count);
	void inportsl(const uint16_t port, void* buffer, const int count);

	void outb(const uint16_t port, const uint8_t data);
	void outportw(const uint16_t port, const uint16_t data);
	void outportl(const uint16_t port, const uint32_t data);
	void outportsb(const uint16_t port, const void* buffer, const int count);
	void outportsw(const uint16_t port, const void* buffer, const int count);
	void outportsl(const uint16_t port, const void* buffer, const int count);

#ifdef __cplusplus
}
#endif

#include INCLUDE_COMMON(../lib/hal,portio.inl)

#endif /*_INC_PORTIO_H */
