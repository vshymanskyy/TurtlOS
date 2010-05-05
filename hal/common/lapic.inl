extern size_t lapicAddress;

inline
uint32_t
lapicRead(uint32_t reg)
{
	return *((volatile uint32_t*)(lapicGetBase()+reg));
}

inline
void
lapicWrite(uint32_t reg, uint32_t value)
{
	*((volatile uint32_t*)(lapicGetBase()+reg)) = value;
}

inline
uint32_t
lapicGetID()
{
	return lapicRead(0x20)>>24;
}
