//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// NH3API 2024
// Helpful Macros, some extra stuff...
// See README
#pragma once

#ifndef __cplusplus
    #error NH3API is a C++ only library
#endif

#ifndef _WIN32
    #error NH3API targets only windows
#endif

#if (defined(_WIN64) || defined(__x86_64__)) || (!defined(_M_IX86) && !defined(__i386__))
    #error Heroes III is a 32-bit game. Please switch your compiler to x86 mode.
#endif

#if (defined(_MSC_VER)) || (defined(__MINGW32__)) || (defined(__clang__))
    // pass
#else
    #error Unsupported compiler. NH3API supports MSVC, GCC, Clang-CL and Clang only
#endif

#define NH3API_MIN(x,y) ((x) < (y) ? (x) : (y))
#define NH3API_MAX(x,y) ((x) > (y) ? (x) : (y))

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <cassert>
#include <cstring>
#include <cstddef>
#include <windows.h>

#ifndef NH3API_FLAG_INLINE_HEADERS
    #define NH3API_FLAG_INLINE_HEADERS (1)
#endif

#ifndef NH3API_JOIN_STRING
    #define NH3API_JOIN_STRING_HELPER(X, Y) X##Y
    #define NH3API_JOIN_STRING(X, Y) NH3API_JOIN_STRING_HELPER(X, Y)
#endif

#define NH3API_WRAP_STRINGIFY(X) #X
#define NH3API_STRINGIFY(X) NH3API_WRAP_STRINGIFY(X)

// NH3API_CHECK_MSVC_DRIVER = MSVC, Clang-CL compilers
// NH3API_CHECK_GNU_DRIVER = MinGW GCC, Clang (GNU aka posix) compilers

#ifndef NH3API_COMPILER_NAME
    // some old MinGW GCC versions defines _MSC_VER, here is a workaround
    #if (defined(_MSC_VER) && !defined(__GNUC__))
        #if (_MSC_VER < 1910)
            #error NH3API v1.2 requires at least Visual Studio 2017 to work. Use NH3API v1.1 if you want to use it with older Visual Studio.
        #endif

        #define NH3API_CHECK_MSVC_DRIVER (1)
        #define NH3API_CHECK_GNU_DRIVER  (0)
        #define NH3API_CHECK_CLANG       (0)
        #define NH3API_CHECK_MINGW       (0)

        #if defined(__clang__)
            #define NH3API_CHECK_CLANG_CL (1)
            #define NH3API_CHECK_MSVC     (0)
            #define NH3API_COMPILER_NAME   "Clang-CL"
        #else
            #define NH3API_CHECK_CLANG_CL (0)
            #define NH3API_CHECK_MSVC     (1)
            #define NH3API_COMPILER_NAME   "MSVC"
        #endif
    #else
        #define NH3API_CHECK_GNU_DRIVER  (1)
        #define NH3API_CHECK_MSVC_DRIVER (0)
        #define NH3API_CHECK_CLANG_CL    (0)
        #define NH3API_CHECK_MSVC        (0)

        #if defined(__clang__)
            #define NH3API_CHECK_CLANG (1)
            #define NH3API_CHECK_MINGW (0)
            #if (__clang_major__ < 9)
                #error NH3API requires at least clang 9 to work.
            #endif
            #define NH3API_COMPILER_NAME "Clang"
        #else
            #define NH3API_CHECK_CLANG (0)
            #define NH3API_CHECK_MINGW (1)
            #if (__GNUC__ < 9)
                #error NH3API requires at least GCC 9 to work.
            #endif
            #define NH3API_COMPILER_NAME "GCC"
        #endif
    #endif
#endif

#if (!NH3API_CHECK_GNU_DRIVER) && (!NH3API_CHECK_MSVC_DRIVER)
    #error Unsupported compiler
#endif

#ifndef NH3API_CPLUSPLUS_11
    #define NH3API_CPLUSPLUS_98 (199711L)
    #define NH3API_CPLUSPLUS_11 (201103L)
    #define NH3API_CPLUSPLUS_14 (201402L)
    #define NH3API_CPLUSPLUS_17 (201703L)
    #define NH3API_CPLUSPLUS_20 (202002L)
    #define NH3API_CPLUSPLUS_23 (202302L)
#endif

// Portable version of the __cplusplus macro
#ifndef NH3API_CPLUSPLUS
    // Visual Studio 2015 Update 3 introduces an _MSVC_LANG macro as a substitute for __cplusplus
    #ifdef _MSVC_LANG
        #define NH3API_CPLUSPLUS (_MSVC_LANG)
    #else
        #define NH3API_CPLUSPLUS (__cplusplus)
    #endif
#endif

#if NH3API_CPLUSPLUS < NH3API_CPLUSPLUS_17
    #error NH3API v1.2 requires at least C++17. Please switch your compiler to C++17 version, or use NH3API v1.1 for C++98/11/14 compatibility
#endif

#ifndef NH3API_CHECK_CPP11
    #define NH3API_CHECK_CPP11 (1)
    #define NH3API_CHECK_CPP14 (1)
    #define NH3API_CHECK_CPP17 (1)

    #if NH3API_CPLUSPLUS >= NH3API_CPLUSPLUS_20
        #define NH3API_CHECK_CPP20 (1)
    #else
        #define NH3API_CHECK_CPP20 (0)
    #endif

    #if NH3API_CPLUSPLUS >= NH3API_CPLUSPLUS_23
        #define NH3API_CHECK_CPP23 (1)
    #else
        #define NH3API_CHECK_CPP23 (0)
    #endif
#endif

#ifndef NH3API_CHECK_RTTI
    #if NH3API_CHECK_GNU_DRIVER
        #ifdef __GXX_RTTI
            #define NH3API_CHECK_RTTI (1)
        #else
            #define NH3API_CHECK_RTTI (0)
        #endif
    #elif NH3API_CHECK_MSVC_DRIVER
        #ifdef _CPPRTTI
            #define NH3API_CHECK_RTTI (1)
        #else
            #define NH3API_CHECK_RTTI (0)
        #endif
    #else
        #define NH3API_CHECK_RTTI (0)
    #endif
#endif

#ifndef NH3API_MSVC_VERSION
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_MSVC_VERSION _MSC_VER
    #else
        #define NH3API_MSVC_VERSION 0
    #endif
    #define NH3API_MSVC_VERSION_2005 1400
    #define NH3API_MSVC_VERSION_2008 1500
    #define NH3API_MSVC_VERSION_2010 1600
    #define NH3API_MSVC_VERSION_2012 1700
    #define NH3API_MSVC_VERSION_2013 1800
    #define NH3API_MSVC_VERSION_2015 1900
    #define NH3API_MSVC_VERSION_2017 1910
    #define NH3API_MSVC_VERSION_2017_UPDATE_3 1911
    #define NH3API_MSVC_VERSION_2017_UPDATE_5 1912
    #define NH3API_MSVC_VERSION_2017_UPDATE_6 1913
    #define NH3API_MSVC_VERSION_2017_UPDATE_8 1915
    #define NH3API_MSVC_VERSION_2017_UPDATE_9 1916
    #define NH3API_MSVC_VERSION_2019 1920
    #define NH3API_MSVC_VERSION_2022 1930
    // TODO: figure out correct Visual Studio 2026 _MSC_VER
    #define NH3API_MSVC_VERSION_MIN  NH3API_MSVC_VERSION_2017 // Minimum supported MSVC version
#endif

#ifndef NH3API_MSVC_STL_VERSION
    #ifdef _CPPLIB_VER
        #define NH3API_MSVC_STL_VERSION (_CPPLIB_VER)
    #else
        #define NH3API_MSVC_STL_VERSION (0)
    #endif
    #define NH3API_MSVC_STL_VERSION_2005 405
    #define NH3API_MSVC_STL_VERSION_2008 503
    #define NH3API_MSVC_STL_VERSION_2008_SP1 505
    #define NH3API_MSVC_STL_VERSION_2010 520
    #define NH3API_MSVC_STL_VERSION_2012 540
    #define NH3API_MSVC_STL_VERSION_2013 610
    #define NH3API_MSVC_STL_VERSION_2015_2022 650
    #define NH3API_MSVC_STL_VERSION_MIN  NH3API_MSVC_STL_VERSION_2015_2022 // Minimum supported version of MSVC STL
#endif

#ifndef NH3API_CRT_VERSION
    #define NH3API_CRT_MSVCR70  0x700 // Visual C++ 2002 Runtime
    #define NH3API_CRT_MSVCR71  0x701 // Visual C++ 2003 Runtime
    #define NH3API_CRT_MSVCR80  0x800 // Visual C++ 2005 Runtime
    #define NH3API_CRT_MSVCR90  0x900 // Visual C++ 2008 Runtime
    #define NH3API_CRT_MSVCR100 0xA00 // Visual C++ 2010 Runtime
    #define NH3API_CRT_MSVCR110 0xB00 // Visual C++ 2012 Runtime
    #define NH3API_CRT_MSVCR120 0xC00 // Visual C++ 2013 Runtime
    #define NH3API_CRT_MSVCRT   0x600 // Windows msvcrt.dll
    #define NH3API_CRT_UCRT     0xE00 // Universal C++ Runtime (2015+)

    #ifdef _UCRT
        #define NH3API_CRT_VERSION NH3API_CRT_UCRT
    #elif defined(__MSVCRT_VERSION__)
        #define NH3API_CRT_VERSION __MSVCRT_VERSION__
    #endif
#endif

#ifndef NH3API_DEBUG
    #ifndef NDEBUG
        #define NH3API_DEBUG (1)
        // improving the debug mode compilation time:
        // some standalone functions use this for both optimization and
        // to not mess with the linker, so we still need to keep the inline keyword
        // but forceinline is more aggressive and takes more compile time
        #define NH3API_FORCEINLINE inline
        #define NH3API_FLATTEN
        #define NH3API_INLINE_LARGE
    #else
        #define NH3API_DEBUG (0)
    #endif
#endif

#ifndef NH3API_HAS_CPP_ATTRIBUTE
    #ifdef __has_cpp_attribute
        #define NH3API_HAS_CPP_ATTRIBUTE(ATTRIBUTE) __has_cpp_attribute(ATTRIBUTE)
    #else
        #define NH3API_HAS_CPP_ATTRIBUTE(ATTRIBUTE) (0)
    #endif
#endif

#ifndef NH3API_DLLEXPORT
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_DLLEXPORT __declspec(dllexport)
        #define NH3API_DLLIMPORT __declspec(dllimport)
        #define NH3API_SELECTANY __declspec(selectany)
    #else
        // prefer C++11 [[attribute]] syntax
        #if NH3API_HAS_CPP_ATTRIBUTE(__gnu__::__dllexport__)
            #define NH3API_DLLEXPORT [[__gnu__::__dllexport__]]
            #define NH3API_DLLIMPORT [[__gnu__::__dllimport__]]
            #define NH3API_SELECTANY [[__gnu__::__selectany__]]
        #else
            #define NH3API_DLLEXPORT __attribute__((__dllexport__))
            #define NH3API_DLLIMPORT __attribute__((__dllimport__))
            #define NH3API_SELECTANY __attribute__((__selectany__))
        #endif
    #endif
#endif

#ifndef NH3API_PRAGMA
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_PRAGMA(X) __pragma(X)
    #else
        #define NH3API_PRAGMA(X) _Pragma(#X)
    #endif
#endif

#ifndef NH3API_DISABLE_WARNING
    #if NH3API_CHECK_MSVC
        #define NH3API_DISABLE_WARNING_BEGIN(GCC_CLANG, MSVC) __pragma(warning(push)) \
                                                              __pragma(warning(disable : MSVC))

        #define NH3API_DISABLE_WARNING_END      __pragma(warning(pop))
        #define NH3API_DISABLE_WARNING(WARNING) __pragma(warning(disable : WARNING))
    #elif defined(__clang__)
        #define NH3API_DISABLE_WARNING_BEGIN(GCC_CLANG, MSVC) NH3API_PRAGMA(clang diagnostic push) \
                                                              NH3API_PRAGMA(clang diagnostic ignored GCC_CLANG)
        #define NH3API_DISABLE_WARNING_END      NH3API_PRAGMA(clang diagnostic pop)
        #define NH3API_DISABLE_WARNING(WARNING) NH3API_PRAGMA(clang diagnostic ignored WARNING)
    #elif NH3API_CHECK_MINGW
        #define NH3API_DISABLE_WARNING_BEGIN(GCC_CLANG, MSVC) NH3API_PRAGMA(GCC diagnostic push) \
                                                              NH3API_PRAGMA(GCC diagnostic ignored GCC_CLANG)
        #define NH3API_DISABLE_WARNING_END      NH3API_PRAGMA(GCC diagnostic pop)
        #define NH3API_DISABLE_WARNING(WARNING) NH3API_PRAGMA(clang diagnostic ignored WARNING)
    #endif
#endif

#ifndef NH3API_STD_MOVE_SEMANTICS
    #define NH3API_STD_MOVE_SEMANTICS (1)
#endif // NH3API_STD_MOVE_SEMANTICS

// disable useless C++ warnings
#if NH3API_CHECK_MSVC
// Nonstandard extension used: nameless struct/union
NH3API_DISABLE_WARNING(4201)
#else
// GCC/Clang warnings
NH3API_DISABLE_WARNING("-Wc++98-compat")          // C++98 compatibility
NH3API_DISABLE_WARNING("-Wc++98-compat-pedantic") // C++98 compatibility
NH3API_DISABLE_WARNING("-Wc++11-compat")          // C++11 compatibility
NH3API_DISABLE_WARNING("-Wc++11-compat-pedantic") // C++11 compatibility
NH3API_DISABLE_WARNING("-Wc++14-compat")          // C++14 compatibility
NH3API_DISABLE_WARNING("-Wc++14-compat-pedantic") // C++14 compatibility
NH3API_DISABLE_WARNING("-Wold-style-cast")        // C-style cast
NH3API_DISABLE_WARNING("-Wreserved-identifier")   // name that starts with '_'
NH3API_DISABLE_WARNING("-Wpadded")                // padded types
NH3API_DISABLE_WARNING("-Wnon-virtual-dtor")      // no virtual destructor(NH3API uses scalar_deleting_destructor)
#endif

#ifndef NH3API_DISABLE_MSVC_WARNING_BEGIN
    #if NH3API_CHECK_MSVC
        #define NH3API_DISABLE_MSVC_WARNING_BEGIN(WARNING_NUMBER) __pragma(warning(push)) \
                                                  __pragma(warning(disable : WARNING_NUMBER))
        #define NH3API_DISABLE_MSVC_WARNING_END   __pragma(warning(pop))
    #else
        #define NH3API_DISABLE_MSVC_WARNING_BEGIN(WARNING_NUMBER)
        #define NH3API_DISABLE_MSVC_WARNING_END
    #endif
#endif

#ifndef NH3API_DISABLE_GCC_CLANG_WARNING_BEGIN
    #if NH3API_CHECK_MSVC
        #define NH3API_DISABLE_GCC_CLANG_WARNING_BEGIN(...)
        #define NH3API_DISABLE_GCC_CLANG_WARNING_END
    #else
        #ifdef __clang__
            #define NH3API_DISABLE_GCC_CLANG_WARNING_BEGIN(...) NH3API_PRAGMA(clang diagnostic push) \
                                                                NH3API_PRAGMA(clang diagnostic ignored __VA_ARGS__)
            #define NH3API_DISABLE_GCC_CLANG_WARNING_END        NH3API_PRAGMA(clang diagnostic pop)
        #else
            #define NH3API_DISABLE_GCC_CLANG_WARNING_BEGIN(...) NH3API_PRAGMA(GCC diagnostic push) \
                                                                NH3API_PRAGMA(GCC diagnostic ignored __VA_ARGS__)
            #define NH3API_DISABLE_GCC_CLANG_WARNING_END        NH3API_PRAGMA(GCC diagnostic pop)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_WARNING
    #if defined(__has_warning) && !NH3API_CHECK_MSVC
        #define NH3API_HAS_WARNING(...) __has_warning(__VA_ARGS__)
    #else
        #define NH3API_HAS_WARNING(...) (0)
    #endif
#endif

// STL vendor check

#if defined(__GLIBCXX__) // GNU GCC libstdc++
    #ifndef NH3API_GCC_STL
        #define NH3API_GCC_STL (1)
    #endif

    #ifndef NH3API_CONSTEXPR_CPP_23
        #ifdef _GLIBCXX23_CONSTEXPR
            #define NH3API_CONSTEXPR_CPP_23 _GLIBCXX23_CONSTEXPR
        #else
            #define NH3API_CONSTEXPR_CPP_23
        #endif
    #endif

    #ifndef NH3API_CONSTEXPR_CPP_20
        #ifdef _GLIBCXX20_CONSTEXPR
            #define NH3API_CONSTEXPR_CPP_20 _GLIBCXX20_CONSTEXPR
        #else
            #define NH3API_CONSTEXPR_CPP_20
        #endif
    #endif
#elif defined(_LIBCPP_VERSION) // Clang libc++
    #ifndef NH3API_CLANG_STL
        #define NH3API_CLANG_STL (1)
    #endif

    #ifndef NH3API_CONSTEXPR_CPP_23
        #ifdef _LIBCPP_CONSTEXPR_SINCE_CXX23
            #define NH3API_CONSTEXPR_CPP_23 _LIBCPP_CONSTEXPR_SINCE_CXX23
        #else
            #define NH3API_CONSTEXPR_CPP_23
        #endif
    #endif

    #ifndef NH3API_CONSTEXPR_CPP_20
        #ifdef _LIBCPP_CONSTEXPR_SINCE_CXX20
            #define NH3API_CONSTEXPR_CPP_20 _LIBCPP_CONSTEXPR_SINCE_CXX20
        #else
            #define NH3API_CONSTEXPR_CPP_20
        #endif
    #endif
#elif defined(_CPPLIB_VER) // MSVC STL
// MSVC defines macros for release and debug version automatically
    #ifndef NH3API_MSVC_STL
        #define NH3API_MSVC_STL (1)
    #endif

    #ifndef NH3API_CONSTEXPR_CPP_23
        #if NH3API_CHECK_CPP23 && defined(_CONSTEXPR23)
            #define NH3API_CONSTEXPR_CPP_23 constexpr
        #else
            #define NH3API_CONSTEXPR_CPP_23
        #endif
    #endif

    #ifndef NH3API_CONSTEXPR_CPP_20
        #if NH3API_CHECK_CPP20 && defined(_CONSTEXPR20)
            #define NH3API_CONSTEXPR_CPP_20 constexpr
        #else
            #define NH3API_CONSTEXPR_CPP_20
        #endif
    #endif

    #ifndef NH3API_MALLOC
        #ifdef _VCRT_ALLOCATOR
            #define NH3API_MALLOC(...) _VCRT_ALLOCATOR __declspec(restrict) __declspec(noalias)
        #else
            #define NH3API_MALLOC(...) __declspec(restrict) __declspec(noalias)
        #endif
    #endif
#else
    #error Unknown STL vendor
#endif

#ifndef NH3API_NODISCARD
    #define NH3API_NODISCARD [[nodiscard]]
#endif

#ifndef NH3API_GCC_STL
    #define NH3API_GCC_STL (0)
#endif

#ifndef NH3API_CLANG_STL
    #define NH3API_CLANG_STL (0)
#endif

#ifndef NH3API_MSVC_STL
    #define NH3API_MSVC_STL (0)
#endif

#ifndef NH3API_CONSTEXPR_CPP_14
    #define NH3API_CONSTEXPR_CPP_14 constexpr
#endif

#ifndef NH3API_CONSTEXPR_CPP_17
    #define NH3API_CONSTEXPR_CPP_17 constexpr
#endif

#ifndef NH3API_CONSTEXPR_CPP_20
    #define NH3API_CONSTEXPR_CPP_20
#endif

#ifndef NH3API_CONSTEXPR_CPP_23
    #define NH3API_CONSTEXPR_CPP_23
#endif

// 1) use
// NH3API_NO_OPT_BEGIN
// ...
// NH3API_NO_OPT_END
// to wrap code that must have no optimizations
// 2) NH3API_FORCEINLINE forces function to be inline
// 3) NH3API_NOINLINE forces function to not to be inlined in any scenario

#if defined(__OPTIMIZE_SIZE__) && !defined(NH3API_FLAG_OPTIMIZE_FOR_SPEED)
    #ifndef NH3API_FLAG_OPTIMIZE_FOR_SIZE
        #define NH3API_FLAG_OPTIMIZE_FOR_SIZE (1)
    #endif
#endif

#ifndef NH3API_CHECK_SSE
    #if defined(_M_IX86_FP) || defined(__SSE__)
        #define NH3API_CHECK_SSE (1)
    #else
        #define NH3API_CHECK_SSE (0)
    #endif
#endif

#ifndef NH3API_CHECK_SSE2
    #ifdef _M_IX86_FP
        #if _M_IX86_FP == 2
            #define NH3API_CHECK_SSE2 (1)
        #else
            #define NH3API_CHECK_SSE2 (0)
        #endif
    #elif defined(__SSE2__)
        #define NH3API_CHECK_SSE2 (1)
    #else
        #define NH3API_CHECK_SSE2 (0)
    #endif
#endif

#ifndef NH3API_CHECK_AVX
    #ifdef __AVX__
        #define NH3API_CHECK_AVX (1)
    #else
        #define NH3API_CHECK_AVX (0)
    #endif
#endif

#ifndef NH3API_CHECK_AVX2
    #ifdef __AVX2__
        #define NH3API_CHECK_AVX2 (1)
    #else
        #define NH3API_CHECK_AVX2 (0)
    #endif
#endif

#if NH3API_CHECK_MSVC_DRIVER
    #ifndef NH3API_NO_OPT
        #define NH3API_NO_OPT_BEGIN NH3API_PRAGMA(optimize("",off))
        #define NH3API_NO_OPT_END   NH3API_PRAGMA(optimize("",on))
        #define NH3API_NO_OPT
    #endif // NH3API_NO_OPT

    // fallback to old MSVC
    #ifndef NH3API_FORCEINLINE
        #define NH3API_FORCEINLINE __forceinline
    #endif // NH3API_FORCEINLINE

    #ifndef NH3API_NOINLINE
        #define NH3API_NOINLINE __declspec(noinline)
    #endif // NH3API_NOINLINE

    #ifndef NH3API_FLAG_OPTIMIZE_FOR_SIZE
        #ifndef NH3API_FLATTEN
            #if NH3API_HAS_CPP_ATTRIBUTE(clang::flatten)
                #define NH3API_FLATTEN [[clang::flatten]]
            #elif NH3API_HAS_CPP_ATTRIBUTE(msvc::flatten)
                #define NH3API_FLATTEN [[msvc::flatten]]
            #else
                #define NH3API_FLATTEN
            #endif
        #endif
    #else
        #ifndef NH3API_FLATTEN
            #define NH3API_FLATTEN __declspec(noinline)
        #endif
    #endif
#else
    #ifndef NH3API_NO_OPT_BEGIN
        #ifdef __clang__
            #define NH3API_NO_OPT_BEGIN NH3API_PRAGMA(clang optimize off)
            #define NH3API_NO_OPT_END   NH3API_PRAGMA(clang optimize on)
        #else
            #define NH3API_NO_OPT_BEGIN NH3API_PRAGMA(GCC push_options) \
                                        NH3API_PRAGMA(GCC optimize("O0"))
            #define NH3API_NO_OPT_END   NH3API_PRAGMA(GCC pop_options)
        #endif
    #endif

    #ifndef NH3API_FORCEINLINE
        #define NH3API_FORCEINLINE __attribute__((__always_inline__)) inline
    #endif

    #ifndef NH3API_NOINLINE
        #define NH3API_NOINLINE [[__gnu__::__noinline__]]
    #endif

    #ifndef NH3API_MALLOC
        #define NH3API_MALLOC(...) __attribute__((/*__returns_nonnull__,*/ __malloc__, __alloc_size__(__VA_ARGS__)))
    #endif

    #ifndef NH3API_FLAG_OPTIMIZE_FOR_SIZE
        #ifndef NH3API_FLATTEN
            #define NH3API_FLATTEN __attribute__((__flatten__))
        #endif
    #else
        #ifndef NH3API_FLATTEN
            #define NH3API_FLATTEN __attribute__((__noinline__))
        #endif
    #endif
#endif

#ifndef NH3API_CALLBACK
    #if NH3API_HAS_CPP_ATTRIBUTE(_Clang::__callback__)
        #define NH3API_CALLBACK(...) [[_Clang::__callback__(__VA_ARGS__)]]
    #else
        #define NH3API_CALLBACK(...)
    #endif
#endif

#ifndef NH3API_RETURNS_ALIGNED
    #if NH3API_HAS_CPP_ATTRIBUTE(__gnu__::__assume_aligned__)
        #define NH3API_RETURNS_ALIGNED(ALIGNMENT) [[__gnu__::__assume_aligned__(ALIGNMENT)]]
    #else
        #define NH3API_RETURNS_ALIGNED(ALIGNMENT)
    #endif
#endif

#ifndef NH3API_INLINE_LARGE
    #if defined(NH3API_FLAG_OPTIMIZE_FOR_SPEED) && !defined(NH3API_FLAG_OPTIMIZE_FOR_SIZE)
        #define NH3API_INLINE_LARGE NH3API_NOINLINE NH3API_FLATTEN
    #elif !defined(NH3API_FLAG_OPTIMIZE_FOR_SPEED) && defined(NH3API_FLAG_OPTIMIZE_FOR_SIZE)
        #define NH3API_INLINE_LARGE NH3API_NOINLINE
    #elif !defined(NH3API_FLAG_OPTIMIZE_FOR_SPEED) && !defined(NH3API_FLAG_OPTIMIZE_FOR_SIZE)
        #define NH3API_INLINE_LARGE
    #else
        #error pick only one size optimization flag
    #endif
#endif

#ifndef NH3API_MSVC_INTRIN
    #if NH3API_HAS_CPP_ATTRIBUTE(msvc::intrinsic)
        #define NH3API_MSVC_INTRIN [[msvc::intrinsic]]
    #else
        #define NH3API_MSVC_INTRIN
    #endif
#endif

#ifndef NH3API_MAX_HEAP_REQUEST
    #define NH3API_MAX_HEAP_REQUEST (0xFFFFFFE0L)
#endif

// indicates that the pointer returned by that function must be freed by deallocator function
#ifndef NH3API_DEALLOCATOR
    #if NH3API_CHECK_MINGW
        #define NH3API_DEALLOCATOR(FUNCTION, ARG) __attribute__ ((__malloc__, __malloc__(FUNCTION, ARG)))
    #else
        #define NH3API_DEALLOCATOR(FUNCTION, ARG)
    #endif
#endif

#include <cstdint>
using bool32_t = uint32_t;

// make POSIX compiler follow the MSVC struct layout
#ifndef NH3API_MSVC_LAYOUT
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_MSVC_LAYOUT
    #else
        #if __has_attribute(__ms_struct__)
            #define NH3API_MSVC_LAYOUT __attribute__((__ms_struct__))
        #else
            #define NH3API_MSVC_LAYOUT
        #endif
    #endif
#endif

// optimize vftable assign:
// we change the vftable anyway so for the sake of optimization,
// we ignore the locally-generated vftable.
#ifndef NH3API_NO_VFTABLE
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_NO_VFTABLE __declspec(novtable)
    #else
        #if __has_attribute(__novtable__)
            #define NH3API_NO_VFTABLE __attribute__((__novtable__))
        #else
            #define NH3API_NO_VFTABLE
        #endif
    #endif

    #define NH3API_VIRTUAL_CLASS  class  NH3API_NO_VFTABLE
    #define NH3API_VIRTUAL_STRUCT struct NH3API_NO_VFTABLE
#endif

#ifndef NH3API_CONST
    #if NH3API_HAS_CPP_ATTRIBUTE(__gnu__::__const__)
        #define NH3API_CONST [[__gnu__::__const__]]
    #else
        #if NH3API_CHECK_MSVC_DRIVER
            #define NH3API_CONST __declspec(noalias)
        #else
            #define NH3API_CONST
        #endif
    #endif
#endif

#ifndef NH3API_PURE
#if NH3API_HAS_CPP_ATTRIBUTE(__gnu__::__pure__)
        #define NH3API_PURE [[__gnu__::__pure__]]
    #else
        #if NH3API_CHECK_MSVC_DRIVER
            #define NH3API_PURE __declspec(noalias)
        #else
            #define NH3API_PURE
        #endif
    #endif
#endif

#ifndef NH3API_CLANG_TRIVIAL_ABI
    #if NH3API_HAS_CPP_ATTRIBUTE(_Clang::__trivial_abi__)
        #define NH3API_CLANG_TRIVIAL_ABI [[_Clang::__trivial_abi__]]
    #else
        #define NH3API_CLANG_TRIVIAL_ABI
    #endif
#endif

#ifndef NH3API_INLINE_LAMBDA
    #if NH3API_HAS_CPP_ATTRIBUTE(_Clang::__always_inline__)
        #define NH3API_INLINE_LAMBDA [[_Clang::__always_inline__]]
    #elif NH3API_HAS_CPP_ATTRIBUTE(__gnu__::__always_inline__)
        #define NH3API_INLINE_LAMBDA [[__gnu__::__always_inline__]]
    #elif NH3API_HAS_CPP_ATTRIBUTE(msvc::always_inline)
        #define NH3API_INLINE_LAMBDA [[msvc::always_inline]]
    #else
        #define NH3API_INLINE_LAMBDA
    #endif
#endif

#ifndef NH3API_FLAG_ENUM
    #if NH3API_HAS_CPP_ATTRIBUTE(_Clang::__flag_enum__)
        #define NH3API_FLAG_ENUM enum [[_Clang::__flag_enum__]]
    #else
        #define NH3API_FLAG_ENUM enum
    #endif
#endif

#ifndef NH3API_STATIC_ASSERT
    #define NH3API_STATIC_ASSERT(msg, ...) static_assert(__VA_ARGS__, msg)
#endif // NH3API_STATIC_ASSERT

#ifndef NH3API_NOEXCEPT
    #define NH3API_NOEXCEPT noexcept
#endif // NH3API_NOEXCEPT

#ifndef NH3API_MAYBE_UNUSED
    #define NH3API_MAYBE_UNUSED [[maybe_unused]]
#endif

#ifndef NH3API_NOEXCEPT_EXPR
    #ifdef NH3API_FLAG_NO_CPP_EXCEPTIONS
        #define NH3API_NOEXCEPT_EXPR(...) noexcept
    #else
        #define NH3API_NOEXCEPT_EXPR(...) noexcept(noexcept(__VA_ARGS__))
    #endif // defined(NH3API_FLAG_NO_CPP_EXCEPTIONS)
#endif // NH3API_NOEXCEPT_EXPR

#ifndef NH3API_DELETED_FUNCTION
    #define NH3API_DELETED_FUNCTION =delete;
#endif // NH3API_DELETED_FUNCTION

#ifndef NH3API_STD_DELEGATING_CONSTRUCTORS
    #define NH3API_STD_DELEGATING_CONSTRUCTORS (1)
    #define NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS (1)
    #define NH3API_STD_INLINE_VARIABLES (1)
    #define NH3API_STD_VARIADIC_ARGUMENTS_FULL_SUPPORT (1)
    #define NH3API_STD_INITIALIZER_LIST (1)
    #define NH3API_STD_RELAXED_CONSTEXPR (1)
    #define NH3API_STD_ARRAY (1)
    #define NH3API_STD_STRING_VIEW (1)
    #define NH3API_STD_HASH (1)
#endif

#ifndef NH3API_STD_RANGES_CONTAINERS
    #if NH3API_CHECK_CPP20
        #if __has_include(<ranges>)
            #ifdef __cpp_lib_containers_ranges
                #define NH3API_STD_RANGES_CONTAINERS (1)
            #endif
        #endif
    #endif
#endif

#ifndef NH3API_STD_RANGES_CONTAINERS
    #define NH3API_STD_RANGES_CONTAINERS (0)
#endif

#ifndef NH3API_STD_STATIC_SUBSCRIPT_OPERATOR
    #ifdef __cpp_multidimensional_subscript
        #if __cpp_multidimensional_subscript >= 202211L
            #define NH3API_STD_STATIC_SUBSCRIPT_OPERATOR (1)
        #endif
    #endif
#endif

#ifndef NH3API_STD_STATIC_SUBSCRIPT_OPERATOR
    #define NH3API_STD_STATIC_SUBSCRIPT_OPERATOR (0)
#endif

namespace nh3api
{

struct dummy_tag_t
{}
// no-op constructor tag, used to optimize low-level manipulations for objects
inline constexpr dummy_tag;
// example:
// exe_string get_string() {
// exe_string result(::nh3api::dummy_tag);
// (void) FASTCALL_1(0x404040, exe_string*, &result);
// return result; }
// without dummy_tag, string will default-initialize its fields to zero
// which is useless because it is overriden in another function anyway
// with dummy_tag, no default initialization occurs

struct omit_base_vftable_tag_t
{}
// omit vftable pointer assign at base
inline constexpr omit_base_vftable_tag;
// example:
// NH3API_VIRTUAL_CLASS derived : public base
// {
// derived() : base(nh3api::omit_base_vftable_tag)
// { NH3API_SET_VFTABLE(); }
// };
// derived:
// assign derived::vftable to this, omit assignment to base in base constructor
}

#ifndef NH3API_UNCOPYABLE
    #define NH3API_UNCOPYABLE(CLASS_NAME) CLASS_NAME(const CLASS_NAME&) = delete; CLASS_NAME& operator=(const CLASS_NAME&) = delete;
    #define NH3API_IMMOVABLE(CLASS_NAME)  CLASS_NAME(CLASS_NAME&&) = delete; CLASS_NAME&& operator=(CLASS_NAME&&) = delete;
    #define NH3API_SINGLETON(CLASS_NAME) NH3API_UNCOPYABLE(CLASS_NAME) NH3API_IMMOVABLE(CLASS_NAME)
#endif

#ifndef NH3API_DEFAULT_DESTRUCTOR
    #define NH3API_DEFAULT_DESTRUCTOR(CLASS_NAME) NH3API_FORCEINLINE ~CLASS_NAME() noexcept = default;
#endif

#ifndef NH3API_DELEGATE_DUMMY
    #define NH3API_DELEGATE_DUMMY(CLASS_NAME) : CLASS_NAME(::nh3api::dummy_tag)
#endif

#ifndef NH3API_DELEGATE_DUMMY_BASE
    #if NH3API_STD_DELEGATING_CONSTRUCTORS
        #define NH3API_DELEGATE_DUMMY_BASE(CLASS_NAME) : CLASS_NAME(::nh3api::dummy_tag)
    #else
        #define NH3API_DELEGATE_DUMMY_BASE(CLASS_NAME) : BASE(::nh3api::dummy_tag)
    #endif
#endif

// final keyword
#ifndef NH3API_FINAL
    #define NH3API_FINAL final
#endif // final

// explicit for C++11
#ifndef NH3API_EXPLICIT_CPP11
    #define NH3API_EXPLICIT_CPP11 explicit
#endif

// ranges library support for containers

#ifndef NH3API_HAS_INCLUDE
    #ifdef __has_include
        #define NH3API_HAS_INCLUDE(...) (__has_include(__VA_ARGS__))
    #else
        #define NH3API_HAS_INCLUDE(...) (0)
    #endif
#endif

// check if compiler has builtins (MSVC has builtins since 2015)
#ifndef NH3API_HAS_BUILTINS
    #define NH3API_HAS_BUILTINS (1)
    #if !NH3API_CHECK_MSVC
        #define NH3API_HAS_BUILTIN(BUILTIN_TO_CHECK) (__has_builtin(BUILTIN_TO_CHECK))
        #define NH3API_HAS_BUILTIN_BIT_CAST (__has_builtin(__builtin_bit_cast))
        #define NH3API_HAS_BUILTIN_ASSUME_ALIGNED (__has_builtin(__builtin_assume_aligned))
        #define NH3API_HAS_BUILTIN_CONSTANT_P (__has_builtin(__builtin_constant_p))
        #define NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED (__has_builtin(__builtin_is_constant_evaluated))
        #ifdef __has_constexpr_builtin
            #define NH3API_HAS_CONSTEXPR_BUILTIN(BUILTIN_TO_CHECK) (__has_constexpr_builtin(BUILTIN_TO_CHECK))
        #else
            #define NH3API_HAS_CONSTEXPR_BUILTIN(BUILTIN_TO_CHECK) (0)
        #endif
    #else // MSVC has no __has_builtin
        #define NH3API_HAS_BUILTIN(BUILTIN_TO_CHECK) (0)
        // MSVC supports __builtin_bit_cast since MSVC v19.26 (Visual Studio 2019)
        #if NH3API_MSVC_VERSION >= 1926
            #define NH3API_HAS_BUILTIN_BIT_CAST (1)
        #else
            #define NH3API_HAS_BUILTIN_BIT_CAST (0)
        #endif

        // MSVC supports __builtin_assume_aligned since MSVC v19.26 (Visual Studio 2019)
        #if NH3API_MSVC_VERSION >= 1928
            #define NH3API_HAS_BUILTIN_ASSUME_ALIGNED (1)
        #else
            #define NH3API_HAS_BUILTIN_ASSUME_ALIGNED (0)
        #endif

        // I doubt MSVC will ever support __builtin_constant_p
        #define NH3API_HAS_BUILTIN_CONSTANT_P (0)

        // MSVC supports __builtin_is_constant_evaluated since MSVC v19.25 (Visual Studio 2019)
        #if NH3API_MSVC_VERSION >= 1925
            #define NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED (1)
        #else
            #define NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED (0)
        #endif

        // MSVC has no support for __has_builtin in the first place
        #define NH3API_HAS_CONSTEXPR_BUILTIN(BUILTIN_TO_CHECK) (0)
    #endif
#endif

#ifndef NH3API_HAS_IS_CONSTANT_EVALUATED
    #if NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED
        #define NH3API_HAS_IS_CONSTANT_EVALUATED (1)
        namespace nh3api
        {
        inline constexpr bool is_constant_evaluated() noexcept
        { return __builtin_is_constant_evaluated(); }
        }
    #elif NH3API_CHECK_MSVC
        #define NH3API_HAS_IS_CONSTANT_EVALUATED (1)
        // hacky soultion for VS 2017
        namespace nh3api
        {
        struct msvc_is_constant_evaluated_helper_base {};
        struct msvc_is_constant_evaluated_helper1 : msvc_is_constant_evaluated_helper_base { int test; };
        struct msvc_is_constant_evaluated_helper2 : msvc_is_constant_evaluated_helper_base { int test; };
        inline constexpr bool is_constant_evaluated() noexcept
        {
            return &msvc_is_constant_evaluated_helper1::test !=
                static_cast<int msvc_is_constant_evaluated_helper_base::*>(&msvc_is_constant_evaluated_helper2::test);
        }
        }
    #else
        #define NH3API_HAS_IS_CONSTANT_EVALUATED (0)
    #endif
#endif

#ifndef NH3API_IF_CONSTEVAL
    #ifdef __cpp_if_consteval
        #define NH3API_IF_CONSTEVAL if consteval
    #elif NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED
        #define NH3API_IF_CONSTEVAL if (__builtin_is_constant_evaluated())
    #else
        #define NH3API_IF_CONSTEVAL if (::nh3api::is_constant_evaluated())
    #endif
#endif

#ifndef NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
    #define NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED constexpr
#endif

// noreturn attribute
#ifndef NH3API_NORETURN
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_NORETURN __declspec(noreturn)
    #else // old MSVC
        #define NH3API_NORETURN __attribute__((noreturn))
    #endif
#endif

// use the C++11's default template function argument feature to implement SFINAE via std::enable_if
#ifndef NH3API_SFINAE_BEGIN
    #define NH3API_SFINAE_BEGIN(...) ,typename ::std::enable_if<__VA_ARGS__, bool>::type = false
#endif

// as for the C++98 mode, use function argument to implement SFINAE via std::enable_if
#ifndef NH3API_SFINAE_END
    #define NH3API_SFINAE_END(...)
#endif

#ifndef NH3API_REQUIRES
    #ifdef __cpp_concepts
        #define NH3API_REQUIRES(...) > requires(__VA_ARGS__) // concept form is more readable
    #else
        #define NH3API_REQUIRES(...) ,typename std::enable_if<__VA_ARGS__, bool>::type = false>
    #endif
#endif

// NH3API flags passed to the compiler:
// - 'NH3API_FLAG_NO_CPP_EXCEPTIONS':
// is useful in combination with '-fno-exceptions' (or '/EHsc' on MSVC) flag
// disables the generation of C++ compiler-specific
// exceptions code and throws exception via the winAPI's
// RaiseException, raising the E06D7363 code (Microsoft C++ exception)
// which can only be catched by the exception filter
// if exception.what() is non-empty, a pointer to the .what() string
// is passed as an argument to the exception filter.
// - 'NH3API_FLAG_INLINE_HEADERS':
// include the NH3API without the build system
// (yes, it is possible)
// makes static/global variables inline
// hence this flag is supported only for the C++17-compliant compiler
// the downside is that the code size will grow heavily
// so this flag is advised to use when you don't
// - 'NH3API_FLAG_OPTIMIZE_FOR_SPEED'
// optimize nh3api functions for size,
// making some effort into smart inlining rather
// blindly forceinlining everything
// - 'NH3API_FLAG_OPTIMIZE_FOR_SIZE'
// optimize aggressively for size

#ifndef NH3API_INLINE_OR_EXTERN
    #define NH3API_INLINE_OR_EXTERN inline
#endif

#ifndef NH3API_INLINE_OR_EXTERN_INIT
    #define NH3API_INLINE_OR_EXTERN_INIT(...) = __VA_ARGS__
#endif

#ifndef NH3API_INLINE_STATIC_VARIABLE
    #define NH3API_INLINE_STATIC_VARIABLE static inline
#endif

#ifndef NH3API_INLINE_STATIC_VARIABLE_INIT
    #define NH3API_INLINE_STATIC_VARIABLE_INIT(...) = __VA_ARGS__
#endif

#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    #if NH3API_CHECK_GNU_DRIVER
        #if !defined(__EXCEPTIONS) && !defined(__cpp_exceptions)
            #define NH3API_FLAG_NO_CPP_EXCEPTIONS
        #endif
    #endif
    #if NH3API_MSVC_STL
        #if !_HAS_EXCEPTIONS
            #define NH3API_FLAG_NO_CPP_EXCEPTIONS
        #endif
    #endif
#endif

#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    #ifndef NH3API_TRY
        #define NH3API_TRY try
    #endif

    #ifndef NH3API_CATCH
        #define NH3API_CATCH(...) catch(__VA_ARGS__)
    #endif

    #ifndef NH3API_RETHROW
        #define NH3API_RETHROW throw;
    #endif
#else
    #ifndef NH3API_TRY
        #define NH3API_TRY if (1)
    #endif

    #ifndef NH3API_CATCH
        #define NH3API_CATCH(...) if (0)
    #endif

    #ifndef NH3API_RETHROW
        #define NH3API_RETHROW ((void)0);
    #endif
#endif

#ifndef NH3API_NOEXCEPT_DESTRUCT
    #define NH3API_NOEXCEPT_DESTRUCT NH3API_NOEXCEPT_EXPR(::std::is_nothrow_destructible_v<value_type>)
#endif

// NH3API settings as C++ constants
namespace nh3api::flags
{

inline constexpr bool msvc_driver
#if NH3API_CHECK_MSVC_DRIVER
 = true;
#else
 = false;
#endif

inline constexpr bool gnu_driver
#if NH3API_CHECK_GNU_DRIVER
 = true;
#else
 = false;
#endif

inline constexpr bool use_msvc
#if NH3API_CHECK_MSVC
 = true;
#else
 = false;
#endif

inline constexpr bool use_gcc
#if NH3API_CHECK_MINGW
 = true;
#else
 = false;
#endif

inline constexpr bool use_clang
#if NH3API_CHECK_CLANG
 = true;
#else
 = false;
#endif

inline constexpr bool use_clang_cl
#if NH3API_CHECK_CLANG_CL
 = true;
#else
 = false;
#endif

inline constexpr bool no_exceptions
#ifdef NH3API_FLAG_NO_CPP_EXCEPTIONS
 = true;
#else
 = false;
#endif

inline constexpr bool debug
#if NH3API_DEBUG
 = true;
#else
 = false;
#endif

inline constexpr bool nodebug = !debug;

inline constexpr bool use_era
#ifdef NH3API_FLAG_USE_ERA
 = true;
#else
 = false;
#endif

} // namespace nh3api::flags

#ifndef NH3API_MAJOR_VERSION
    #define NH3API_MAJOR_VERSION 1
    #define NH3API_MINOR_VERSION 2
    #define NH3API_VERSION_STRING "NH3API v1.2"
#endif

#ifndef NH3API_COMPILER_MAJOR_VERSION
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_COMPILER_MAJOR_VERSION (_MSC_VER / 100)
        #define NH3API_COMPILER_MINOR_VERSION (((_MSC_VER % 100) - (_MSC_VER % 10))/10)
        #define NH3API_COMPILER_PATCH_LEVEL   (_MSC_VER % 10)
    #elif NH3API_CHECK_GNU_DRIVER
        #ifdef __clang__
            #define NH3API_COMPILER_MAJOR_VERSION (__clang_major__)
            #define NH3API_COMPILER_MINOR_VERSION (__clang_minor__)
            #define NH3API_COMPILER_PATCH_LEVEL   (__clang_patchlevel__)
        #else
            #define NH3API_COMPILER_MAJOR_VERSION (__GNUC__)
            #define NH3API_COMPILER_MINOR_VERSION (__GNUC_MINOR__)
            #define NH3API_COMPILER_PATCH_LEVEL   (__GNUC_PATCHLEVEL__)
        #endif
    #endif
#endif

#ifndef NH3API_UNREACHABLE
    #if NH3API_CHECK_MSVC
        #define NH3API_UNREACHABLE() __assume(false)
    #else
        #define NH3API_UNREACHABLE() __builtin_unreachable()
    #endif
#endif

#ifndef NH3API_ASSUME
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_ASSUME(...) do { __assume(__VA_ARGS__); } while(0)
    #else
        #if __has_builtin(__builtin_assume)
            #define NH3API_ASSUME(...) do { __builtin_assume(__VA_ARGS__); } while(0)
        #elif __has_attribute(__assume__)
            #define NH3API_ASSUME(...) __attribute__((__assume__(__VA_ARGS__)))
        #elif __has_builtin(__builtin_unreachable)
            #define NH3API_ASSUME(...) do { if (!bool(__VA_ARGS__)) __builtin_unreachable(); } while(0)
        #endif
    #endif
#endif

// no appropriate implementation
#ifndef NH3API_ASSUME
    #define NH3API_ASSUME(...)
#endif

// inline assembly macro
// to use, every statement must be wrapped in NH3API_ASM()
#ifndef NH3API_ASM
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_ASM(...) __asm {__VA_ARGS__}
    #else
        #define NH3API_ASM(...) __asm__ __volatile__ (#__VA_ARGS__)
    #endif
#endif

#ifndef NH3API_NAKED
    #if NH3API_CHECK_MSVC_DRIVER
        #define NH3API_NAKED __declspec(naked)
    #else
        #if __has_attribute(__naked__)
            #define NH3API_NAKED __attribute__((__naked__))
        #else
            #define NH3API_NAKED
        #endif
    #endif
#endif

// Debug expression macro
#if NH3API_DEBUG
    #define NH3API_DEBUG_SWITCH(debug_expr, release_expr) (release_expr)
#else // NH3API_DEBUG
    #define NH3API_DEBUG_SWITCH(debug_expr, release_expr) (debug_expr)
#endif // NH3API_DEBUG

#ifndef NH3API_SIZE_ASSERT
    #define NH3API_SIZE_ASSERT(EXPECTED_SIZE, ...) NH3API_STATIC_ASSERT("size mismatch", sizeof(__VA_ARGS__) == EXPECTED_SIZE)
#endif

// Memory shield does not let the optimizing compiler discard statements
// like this:
// void foo()
// {
// NH3API_MEMSHIELD_BEGIN
// ... // will not be optimized away
// NH3API_MEMSHIELD_END
// }
#if NH3API_CHECK_MSVC
    #ifndef NH3API_MEMSHIELD_BEGIN
        #define NH3API_MEMSHIELD_BEGIN _ReadWriteBarrier();
        #define NH3API_MEMSHIELD_END   _ReadWriteBarrier();
        #define NH3API_MEMSHIELD
    #endif // NH3API_MEMSHIELD_BEGIN
#else
    #ifndef NH3API_MEMSHIELD_BEGIN
        #define NH3API_MEMSHIELD_BEGIN __asm__ volatile("" ::: "memory");
        #define NH3API_MEMSHIELD_END   __asm__ volatile("" ::: "memory");
        #define NH3API_MEMSHIELD
    #endif // NH3API_MEMSHIELD_BEGIN
#endif

// this is a hack designed to enable global variables addresses to be baked into binaries
// rather that using global variables which is double indirection
// whole program optimization doesn't always fix this issue though.
#if NH3API_HAS_BUILTIN_CONSTANT_P
    #define get_global_var_ptr(address,...) (__builtin_constant_p(reinterpret_cast<__VA_ARGS__* const>(address)) ? reinterpret_cast<__VA_ARGS__* const>(address) : reinterpret_cast<__VA_ARGS__* const>(address))  // use '-fwhole-program' or '-flto'('-O1' on clang) flag to make it constexpr
    #define get_global_var_ref(address,...) (*((__builtin_constant_p(reinterpret_cast<__VA_ARGS__* const>(address)) ? reinterpret_cast<__VA_ARGS__* const>(address) : reinterpret_cast<__VA_ARGS__* const>(address)))) // use '-fwhole-program' or '-flto'('-O1' on clang) flag to make it constexpr
#else
    #define get_global_var_ptr(address,...) (reinterpret_cast<__VA_ARGS__* const>(address))
    #define get_global_var_ref(address,...) (*reinterpret_cast<__VA_ARGS__* const>(address))
#endif

#ifndef NH3API_SCALAR_DELETING_DESTRUCTOR
#define NH3API_SCALAR_DELETING_DESTRUCTOR virtual void __thiscall scalar_deleting_destructor(uint8_t flag) \
                                          { get_vftable(this)->scalar_deleting_destructor(this, flag); }
#endif

// requires T::vftable_t
template<class T> [[nodiscard]] NH3API_FORCEINLINE
typename T::vftable_t* get_vftable(T* ptr) noexcept
{ return *reinterpret_cast<typename T::vftable_t**>(__builtin_launder(ptr)); }


// requires T::vftable_t
template<class T> [[nodiscard]] NH3API_FORCEINLINE
const typename T::vftable_t* get_vftable(const T* ptr) noexcept
{ return *reinterpret_cast<const typename T::vftable_t* const*>(ptr); }

// is supposed to be specialized for each .exe-s polymorphic class
template<class T>
struct vftable_address
{ static inline constexpr uintptr_t address = 0; };

template<typename T> [[nodiscard]] NH3API_FORCEINLINE
const typename T::vftable_t* get_type_vftable() noexcept
{ return reinterpret_cast<const typename T::vftable_t*>(vftable_address<T>::address); }

template<typename T> [[nodiscard]] NH3API_FORCEINLINE
const typename T::vftable_t* get_type_vftable(const T*) noexcept
{ return reinterpret_cast<const typename T::vftable_t*>(vftable_address<T>::address); }

// for constructors
template<typename T>
void set_vftable(T* ptr)
{
    NH3API_MEMSHIELD_BEGIN
    *reinterpret_cast<const void**>(__builtin_launder(ptr)) = get_type_vftable(ptr);
    NH3API_MEMSHIELD_END
}

// undocumented MSVC intrinsic: __vtable_initialize(type) sets vftable pointer for this of type*

#ifndef NH3API_SET_VFTABLE
    #define NH3API_SET_VFTABLE() do { NH3API_MEMSHIELD_BEGIN set_vftable(this); NH3API_MEMSHIELD_END } while(0)
#endif

#if NH3API_STD_INLINE_VARIABLES

#ifndef NH3API_SPECIALIZE_TYPE_VFTABLE
#define NH3API_SPECIALIZE_TYPE_VFTABLE(ADDRESS, ...) \
template<> struct vftable_address<__VA_ARGS__> \
{ static inline constexpr uintptr_t address = ADDRESS; };
#endif

template<typename T>
#if NH3API_HAS_CPP_ATTRIBUTE(_Clang::__no_specializations__)
[[_Clang::__no_specializations__("Specialize vftable_address<T> instead.")]]
#endif
inline constexpr uintptr_t vftable_address_v = vftable_address<T>::address;

#else // NH3API_STD_INLINE_VARIABLES

#ifndef NH3API_SPECIALIZE_TYPE_VFTABLE
#define NH3API_SPECIALIZE_TYPE_VFTABLE(ADDRESS, ...) \
template<> struct vftable_address<__VA_ARGS__> \
{ static const uintptr_t address = ADDRESS; };
#endif

#endif // NH3API_STD_INLINE_VARIABLES

#ifndef NH3API_COMPILER_VERSION_STRING
    #if NH3API_CHECK_MSVC
        #define NH3API_COMPILER_VERSION_STRING "MSVC v" NH3API_STRINGIFY(_MSC_FULL_VER)
    #elif NH3API_CHECK_MINGW
        #define NH3API_COMPILER_VERSION_STRING "GCC v" NH3API_STRINGIFY(__GNUC__) "." NH3API_STRINGIFY(__GNUC_MINOR__) "." NH3API_STRINGIFY(__GNUC_PATCHLEVEL__)
    #elif NH3API_CHECK_CLANG
        #define NH3API_COMPILER_VERSION_STRING "Clang v" NH3API_STRINGIFY(__clang_major__) "." NH3API_STRINGIFY(__clang_minor__) "." NH3API_STRINGIFY(__clang_patchlevel__)
    #elif NH3API_CHECK_CLANG_CL
        #define NH3API_COMPILER_VERSION_STRING "Clang-cl v" NH3API_STRINGIFY(__clang_major__) "." NH3API_STRINGIFY(__clang_minor__) "." NH3API_STRINGIFY(__clang_patchlevel__)
    #endif
#endif

#if NH3API_CHECK_MSVC
//#ifndef NH3API_PRAGMA_COMMENT_USER_USED
//#define NH3API_PRAGMA_COMMENT_USER_USED
#pragma comment(user, "Built with " NH3API_VERSION_STRING " by " NH3API_COMPILER_VERSION_STRING " on " __DATE__ " at " __TIME__ )
//#endif // NH3API_PRAGMA_COMMENT_USER_USED
#endif
