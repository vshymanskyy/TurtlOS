#ifndef INC_GLOBAL_H
#define INC_GLOBAL_H

#if _MSC_VER > 1000
	#pragma once
#endif

#include <platform.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <assert.h>
#include <new.h>

typedef void     (*pproc_t)(void);
typedef int      (*pfunc_t)(void);
typedef void*    ptr_t;

#define __concat(x,y) x ## y		/*  */
#define __string(x) #x				/*  */
#define __string_def(x) __string(x)	/*  */

#endif /* INC_GLOBAL_H */

