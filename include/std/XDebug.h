#ifndef _X_DEBUG_H_
#define _X_DEBUG_H_

#include <XPlat.h>

#if defined DEBUG || defined _DEBUG
	#define X_DBG(expr) expr
#else
	#define X_DBG(expr)
#endif

#if defined (TARGET_OS_WINDOWS)
	#include <Windows.h>
	#include <stdio.h>
	
	#define X_DBG_BREAK() DebugBreak();
	#define X_DBG_PRINT(...) { 	char buff[1024]; snprintf(buff, 1024, __VA_ARGS__); OutputDebugString(buff); }
#elif defined (TARGET_OS_UNIX)
	#include <stdio.h>
	#include <string.h>
	#include <errno.h>
	#include <signal.h>
	
	#define X_DBG_BREAK() raise(SIGTRAP);
	#define X_DBG_PRINT(...) { fprintf(stderr, __VA_ARGS__); }
#else
	#define X_DBG_BREAK() *(char*)(NULL) = 0xFF; // SEGV!!!
	#define X_DBG_PRINT(...)
#endif

#define X_FATAL(...) { X_DBG_PRINT("Fatal error: "); X_DBG_PRINT(__VA_ARGS__); X_DBG_PRINT(" at %s:%d\n", __FILE__, __LINE__); X_DBG_BREAK(); }

#define X_ASSERT(expr) { if (!(expr)) { X_DBG_PRINT("Assertion '" #expr "' failed at %s:%d", __FILE__, __LINE__); X_DBG_BREAK(); } }
#define X_ASSERT_EQ(expr, val, fmt) { if (!((expr) == (val))) { X_DBG_PRINT("Assertion '" #expr " == " #val "' failed ( " fmt " != " fmt " ) at %s:%d\n", (expr), (val), __FILE__, __LINE__); X_DBG_BREAK(); } }
#define X_ASSERT_NE(expr, val, fmt) { if (!((expr) != (val))) { X_DBG_PRINT("Assertion '" #expr " != " #val "' failed ( " fmt " == " fmt " ) at %s:%d\n", (expr), (val), __FILE__, __LINE__); X_DBG_BREAK(); } }
#define X_ASSERT_GT(expr, val, fmt) { if (!((expr) >  (val))) { X_DBG_PRINT("Assertion '" #expr " > "  #val "' failed ( " fmt " <= " fmt " ) at %s:%d\n", (expr), (val), __FILE__, __LINE__); X_DBG_BREAK(); } }
#define X_ASSERT_GE(expr, val, fmt) { if (!((expr) >= (val))) { X_DBG_PRINT("Assertion '" #expr " >= " #val "' failed ( " fmt " < "  fmt " ) at %s:%d\n", (expr), (val), __FILE__, __LINE__); X_DBG_BREAK(); } }
#define X_ASSERT_LT(expr, val, fmt) { if (!((expr) <  (val))) { X_DBG_PRINT("Assertion '" #expr " < "  #val "' failed ( " fmt " >= " fmt " ) at %s:%d\n", (expr), (val), __FILE__, __LINE__); X_DBG_BREAK(); } }
#define X_ASSERT_LE(expr, val, fmt) { if (!((expr) <= (val))) { X_DBG_PRINT("Assertion '" #expr " <= " #val "' failed ( " fmt " > "  fmt " ) at %s:%d\n", (expr), (val), __FILE__, __LINE__); X_DBG_BREAK(); } }

#endif /* _X_DEBUG_H_ */
