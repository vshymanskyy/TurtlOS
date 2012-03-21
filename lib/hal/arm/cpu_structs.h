#pragma once
#include <platform.h>

#define MAX_INTERRUPTS		256

BEGIN_PACKED_REGION

struct RegisterFrame {
	uint32_t interrupt, error;
} PACKED;

END_PACKED_REGION
