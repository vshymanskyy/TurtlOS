#pragma once

void picSendEOI(uint8_t irq);

void picRemap(uint8_t masterOffset, uint8_t slaveOffset);

void picSetMask(uint8_t IRQline);

void picClearMask(uint8_t IRQline);
