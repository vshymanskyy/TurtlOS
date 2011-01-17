#ifndef INC_LINKER_H
#define INC_LINKER_H

#if _MSC_VER > 1000
	#pragma once
#endif

/* kernel */
extern size_t __kernelStart;
extern size_t __kernelEnd;

/* sections */
extern size_t __textSectStart;
extern size_t __textSectEnd;

extern size_t __dataSectStart;
extern size_t __dataSectEnd;

extern size_t __bssSectStart;
extern size_t __bssSectEnd;

/* text section */
extern size_t __loaderStart;
extern size_t __loaderEnd;

extern size_t __textStart;
extern size_t __textEnd;

extern size_t __rodataStart;
extern size_t __rodataEnd;

/* data section */
extern size_t __ctorsStart;
extern size_t __ctorsEnd;

extern size_t __dtorsStart;
extern size_t __dtorsEnd;

extern size_t __ehframeStart;
extern size_t __ehframeEnd;

/* bss section */
extern size_t __bssStart;
extern size_t __bssEnd;

extern size_t __commonStart;
extern size_t __commonEnd;

#endif /* INC_LINKER_H */
