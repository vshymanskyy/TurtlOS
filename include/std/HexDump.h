#pragma once

enum HEXDMP_COLUMN {
	HEXDMP_ADDR = 1
	,HEXDMP_DATA = 2
	,HEXDMP_CHARS = 4
};

void hexdump (const void* data, size_t length, unsigned columnsMask = HEXDMP_ADDR | HEXDMP_DATA | HEXDMP_CHARS);

template <typename T>
void hexdump (const T* data, unsigned count = 1, unsigned columnsMask = HEXDMP_ADDR | HEXDMP_DATA | HEXDMP_CHARS) {
	//for (int i=0; i<count; i++) {
	hexdump((const void*)(data), count*sizeof(T), columnsMask);
	//}
}
