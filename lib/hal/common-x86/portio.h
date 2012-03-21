#ifndef _INC_PORTIO_H
#define _INC_PORTIO_H

#include <global.h>



INLINE void io_wait(void);

INLINE uint8_t  inb(const uint16_t port);
INLINE uint16_t inw(const uint16_t port);
INLINE uint32_t inl(const uint16_t port);

INLINE void outb(const uint16_t port, const uint8_t data);
INLINE void outw(const uint16_t port, const uint16_t data);
INLINE void outl(const uint16_t port, const uint32_t data);



#include "portio.inl"

#endif /*_INC_PORTIO_H */
