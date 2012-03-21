#ifndef INC_PLATFORM_H
#define INC_PLATFORM_H

#if _MSC_VER > 1000
	#pragma once
#endif

#define TARGET_X86 1
#define TARGET_X86_64 2
#define TARGET_ARM 3

#define COMPILER_GCC 1
#define COMPILER_ICC 2
#define COMPILER_MSVC 3

/*********************************************************
 * TARGET PLATFORM
 *********************************************************/
#if defined(__x86)
#  define TARGET TARGET_X86
#  define TARGET_PATH x86
#  define COMMON_PATH common-x86
#elif defined(__x86_64)
#  define TARGET TARGET_X86_64
#  define TARGET_PATH x64
#  define COMMON_PATH common-x86
#elif defined(__arm)
#  define TARGET TARGET_ARM
#  define TARGET_PATH arm
#  define COMMON_PATH mach-versatilepb
#else
#  error "Target platform not recognized"
#endif

#define INCLUDE_COMMON(PATH,FILENAME) <PATH/COMMON_PATH/FILENAME>
#define INCLUDE_TARGET(PATH,FILENAME) <PATH/TARGET_PATH/FILENAME>

/*********************************************************
 * COMPILER
 *********************************************************/
#if defined(__GNUC__)
#  define COMPILER COMPILER_GCC
#  define __COMPILER__ "GNU C++"
#  define __COMPILER_VER__ "xxx"
#  define FORCE_INLINE __attribute__((always_inline))
#  define PACKED __attribute__((packed))
#  define BEGIN_PACKED_REGION
#  define END_PACKED_REGION
#  define __FUNC__ __FUNCTION__
#elif defined(_MSC_VER)
#  define FORCE_INLINE __forceinline
#  define BEGIN_PACKED_REGION __pragma(pack(push, 1))
#  define END_PACKED_REGION __pragma(pack(pop))
#  define PACKED
#  if defined(__ICL)
#    define COMPILER COMPILER_ICC
#    define __COMPILER__ "Intel C++"
#    define __COMPILER_VER__ "xxx"
#  else
#    define COMPILER COMPILER_MSVC
#    define __COMPILER__ "MSVC"
#    define __COMPILER_VER__ "xxx"
#  endif
#else
#  error "Compiler not recognized"
#endif

#define INLINE __inline__ static
#define NAKED __declspec((naked))
#define NORETURN __declspec((noreturn))
#define NOINLINE __declspec((noinline))

#endif  /* INC_PLATFORM_H */
