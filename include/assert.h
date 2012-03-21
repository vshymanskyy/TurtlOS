#ifndef INC_ASSERT_H
#define INC_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

void debug_print(const char* format, ...);
void debug_puts (const char* s);
void debug_putc (const char s);

void __attribute__((noreturn)) assert_failed (const char *msg, const char *file, const int line, const char *func);
void __attribute__((noreturn)) fatal_soft (const char *msg, const char *file, const int line, const char *func);

#define fatal(msg)		fatal_soft(msg, __FILE__, __LINE__, __FUNC__);

#ifdef DEBUG
	#define assert(msg) {													\
		if (!(msg)) {														\
			assert_failed(__string(msg), __FILE__, __LINE__, __FUNC__);		\
		}																	\
	}
#else
	#define assert(msg)
#endif


#ifdef __cplusplus
}
#endif

#endif /* INC_ASSERT_H */
