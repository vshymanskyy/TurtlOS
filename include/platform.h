#ifndef _INC_PLATFORM_H
#define _INC_PLATFORM_H

#define I386 1
#define X86_64 2

/// Recognize target
#if defined(__i386)
#  define TARGET I386
#  define CURTARGET "i386"
#elif defined(__x86_64)
#  define TARGET X86_64
#  define CURTARGET "x86-64"
#else
#  error "Target platform not recognized."
#endif

/// Attempt to recognize compiler
#if defined(__GNUC__)
#  define COMPILER GCC
#  define COMPILERSTR "Gnu C++ Compiler"
#  define PACKED __attribute__((packed))
#  define BEGIN_PACKED_REGION
#  define END_PACKED_REGION
#  define NAKED __declspec(naked)
#  define NORETURN __declspec(noreturn)
#  define NOINLINE __declspec(noinline)
#elif defined(_MSC_VER)
#  if defined(__MWERKS__)
#    define COMPILER MWERCS
#  elif defined(__VECTOR_C)
#    define COMPILER VECTOR
#  elif defined(__ICL)
#    define COMPILER INTEL
#    define COMPILERSTR "Intel C++ Compiler"
#    define PACKED
#    define BEGIN_PACKED_REGION __pragma(pack(push, 1))
#    define END_PACKED_REGION __pragma(pack(pop))
#    define NAKED __declspec(naked)
#    define NORETURN __declspec(noreturn)
#    define NOINLINE __declspec(noinline)
#  elif defined(__BORLANDC__)
#    define COMPILER BORLAND
#  else
#    define COMPILER MSVC
#    define COMPILERSTR "Microsoft Visual C++ Compiler"
#    define PACKED
#    define BEGIN_PACKED_REGION __pragma(pack(push, 1))
#    define END_PACKED_REGION __pragma(pack(pop))
#    define NAKED __declspec(naked)
#    define NORETURN __declspec(noreturn)
#    define NOINLINE __declspec(noinline)
#  endif
#else
#  error "Compiler not recognized."
#endif

/// Compiler-specific defines
#if COMPILER == GCC
#  define __FUNC__ __FUNCTION__
#elif COMPILER == MSVC
//#  define __FUNC__ __FUNC__
#endif

#endif  // _INC_PLATFORM_H
