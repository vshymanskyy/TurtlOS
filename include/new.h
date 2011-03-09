#ifndef INC_NEW_H
#define INC_NEW_H

#ifdef __cplusplus
#include <stddef.h>

void* operator new (size_t size);
void* operator new (size_t size, void* at);
void* operator new[] (size_t size);
void operator delete (void* p);
void operator delete[] (void* p);

#endif /* __cplusplus */

#endif /* INC_NEW_H */
