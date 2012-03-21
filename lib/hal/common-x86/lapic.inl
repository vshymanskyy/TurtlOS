#pragma once

extern size_t lapicAddress;

INLINE
uint32_t
lapicRead(uint32_t reg)
{
	return *((volatile uint32_t*)(lapicGetBase()+reg));
}

INLINE
void
lapicWrite(uint32_t reg, uint32_t value)
{
	*((volatile uint32_t*)(lapicGetBase()+reg)) = value;
}

INLINE
uint32_t
lapicGetID()
{
	return lapicRead(0x20)>>24;
}
