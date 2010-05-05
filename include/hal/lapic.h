#ifndef _INC_LAPIC_H
#define _INC_LAPIC_H

void lapicInit();
void lapicStart();
void lapicStartCpu(uint32_t id, uint32_t entry);

void lapicClearErrorStatus();
void lapicSetBase(size_t addr);
size_t lapicGetBase();

uint32_t lapicGetID();
uint32_t lapicReadErrorStatus();

#include <../hal/common/lapic.inl>

#endif // _INC_LAPIC_H
