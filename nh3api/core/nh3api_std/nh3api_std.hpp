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
        #if (_MSC_VER < 1400)
            #error NH3API requires at least Visual Studio 2005 to work.
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
            #if (__clang_major__ < 10)
                #error NH3API requires at least clang 10 to work.
            #endif
            #if (__cplusplus < 201103L)
                #error NH3API doesnt work on clang with -std=c++98
            #endif
            #define NH3API_COMPILER_NAME "Clang"
        #else
            #define NH3API_CHECK_CLANG (0)
            #define NH3API_CHECK_MINGW (1)
            #if (__GNUC__ < 5)
                #error NH3API requires at least GCC 5 to work.
            #endif
            #if (__cplusplus < 201103L)
                #error NH3API doesnt work on GCC with -std=c++98
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
        #if NH3API_CHECK_MSVC
            // Visual Studio 2015 supports all C++11 features since update 1
            #if (_MSC_FULL_VER >= 190023506L && _MSC_FULL_VER < 190024210L)
                #define NH3API_CPLUSPLUS (NH3API_CPLUSPLUS_11)
            #else 
                #define NH3API_CPLUSPLUS (__cplusplus)
            #endif
        #else
            #define NH3API_CPLUSPLUS (__cplusplus)
        #endif
    #endif
#endif

#ifndef NH3API_CHECK_CPP11
    #if NH3API_CPLUSPLUS >= NH3API_CPLUSPLUS_11
        #define NH3API_CHECK_CPP11 (1)
    #else
        #define NH3API_CHECK_CPP11 (0)
    #endif

    #if NH3API_CPLUSPLUS >= NH3API_CPLUSPLUS_14
        #define NH3API_CHECK_CPP14 (1)
    #else
        #define NH3API_CHECK_CPP14 (0)
    #endif

    #if NH3API_CPLUSPLUS >= NH3API_CPLUSPLUS_17
        #define NH3API_CHECK_CPP17 (1)
    #else
        #define NH3API_CHECK_CPP17 (0)
    #endif

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
    #define NH3API_MSVC_VERSION_MIN  NH3API_MSVC_VERSION_2005 // Minimum supported MSVC version
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
    #define NH3API_MSVC_STL_VERSION_MIN  NH3API_MSVC_STL_VERSION_2005 // Minimum supported version of MSVC STL
#endif

#ifndef NH3API_VS2010
    #if NH3API_CHECK_MSVC
        #define NH3API_VS2010 (_MSC_VER >= 1600 && _MSC_FULL_VER < 190023506L) // Visual Studio 2010 has partial C++11 support.
        #define NH3API_VS2010_ONLY (_MSC_VER >= 1600 && _MSC_VER < 1700)
        #define NH3API_VS2012_2013 ((_MSC_VER == 1700) || (_MSC_VER == 1800))
    #else
        #define NH3API_VS2010 (0)
        #define NH3API_VS2010_ONLY (0)
        #define NH3API_VS2012_2013 (0)
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
        #define NH3API_HAS_CPP_ATTRIBUTE(ATTRIBUTE_TO_CHECK) __has_cpp_attribute(ATTRIBUTE_TO_CHECK)
    #else
        #define NH3API_HAS_CPP_ATTRIBUTE(ATTRIBUTE_TO_CHECK) (0)
    #endif
#endif

#ifndef NH3API_DEPRECATED
    #if NH3API_CHECK_MSVC_DRIVER
        #if NH3API_CHECK_CPP14
            #define NH3API_DEPRECATED(msg) [[deprecated(msg)]]
        #else
            #define NH3API_DEPRECATED(msg) __declspec(deprecated(msg))
        #endif
    #else
        #if NH3API_HAS_CPP_ATTRIBUTE(deprecated)
            #define NH3API_DEPRECATED(msg) [[deprecated(msg)]]
        #else
            #define NH3API_DEPRECATED(msg) // __attrubute__((deprecated)) // can not apply to pure virtual function...
        #endif
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
        #define NH3API_PRAGMA(...) __pragma(__VA_ARGS__)
    #else
        #define NH3API_PRAGMA(...) _Pragma(#__VA_ARGS__)
    #endif
#endif

#ifndef NH3API_DISABLE_WARNING
    #if NH3API_CHECK_MSVC
        #define NH3API_DISABLE_WARNING_BEGIN(GCC_CLANG, MSVC) NH3API_PRAGMA(warning(push)) \
                                                              NH3API_PRAGMA(warning(disable : MSVC))

        #define NH3API_DISABLE_WARNING_END   NH3API_PRAGMA(warning(pop))
        #define NH3API_DISABLE_WARNING(WARNING) NH3API_PRAGMA(warning(disable : WARNING))
    #elif defined(__clang__)
        #define NH3API_DISABLE_WARNING_BEGIN(GCC_CLANG, MSVC) NH3API_PRAGMA(clang diagnostic push) \
                                                              NH3API_PRAGMA(clang diagnostic ignored GCC_CLANG)
        #define NH3API_DISABLE_WARNING_END   NH3API_PRAGMA(clang diagnostic pop)
        #define NH3API_DISABLE_WARNING(WARNING) NH3API_PRAGMA(clang diagnostic ignored WARNING)
    #elif NH3API_CHECK_MINGW
        #define NH3API_DISABLE_WARNING_BEGIN(GCC_CLANG, MSVC) NH3API_PRAGMA(GCC diagnostic push) \
                                                              NH3API_PRAGMA(GCC diagnostic ignored GCC_CLANG)
        #define NH3API_DISABLE_WARNING_END   NH3API_PRAGMA(GCC diagnostic pop)
        #define NH3API_DISABLE_WARNING(WARNING) NH3API_PRAGMA(clang diagnostic ignored WARNING)
    #endif
#endif

#ifndef NH3API_DISABLE_MSVC_WARNING_BEGIN
    #if NH3API_CHECK_MSVC 
        #define NH3API_DISABLE_MSVC_WARNING_BEGIN(WARNING_NUMBER) NH3API_PRAGMA(warning(push)) \
                                                  NH3API_PRAGMA(warning(disable : WARNING_NUMBER))
        #define NH3API_DISABLE_MSVC_WARNING_END   NH3API_PRAGMA(warning(pop))                              
    #else 
        #define NH3API_DISABLE_MSVC_WARNING_BEGIN(WARNING_NUMBER)
        #define NH3API_DISABLE_MSVC_WARNING_END
    #endif
#endif

#ifndef NH3API_STD_MOVE_SEMANTICS
    #define NH3API_STD_MOVE_SEMANTICS (NH3API_VS2012_2013 || NH3API_CHECK_CPP11)
#endif // NH3API_STD_MOVE_SEMANTICS

// disable useless C++ warnings
#if NH3API_CHECK_MSVC
#if !NH3API_STD_MOVE_SEMANTICS
// Qualifying an enumerator by the enumeration name is an C++98 Microsoft extension 
// standardized since C++11
NH3API_DISABLE_WARNING(4482)
#endif
// Nonstandard extension used: nameless struct/union
NH3API_DISABLE_WARNING(4201)
#else
// GCC/Clang warnings
NH3API_DISABLE_WARNING("-Wc++98-compat") // C++98 compatibility
NH3API_DISABLE_WARNING("-Wc++98-compat-pedantic") // C++98 compatibility
NH3API_DISABLE_WARNING("-Wold-style-cast") // C-style cast
NH3API_DISABLE_WARNING("-Wreserved-identifier") // name that starts with '_'
NH3API_DISABLE_WARNING("-Wpadded") // padded types
NH3API_DISABLE_WARNING("-Wnon-virtual-dtor") // no virtual destructor(NH3API uses scalar_deleting_destructor)
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

    #ifndef NH3API_CONSTEXPR_CPP_17
        #ifdef _GLIBCXX17_CONSTEXPR
            #define NH3API_CONSTEXPR_CPP_17 _GLIBCXX17_CONSTEXPR
        #else
            #define NH3API_CONSTEXPR_CPP_17
        #endif
    #endif

    #ifndef NH3API_CONSTEXPR_CPP_14
        #ifdef _GLIBCXX14_CONSTEXPR
            #define NH3API_CONSTEXPR_CPP_14 _GLIBCXX14_CONSTEXPR
        #else
            #define NH3API_CONSTEXPR_CPP_14
        #endif
    #endif

    #ifndef NH3API_NODISCARD
        #if NH3API_HAS_CPP_ATTRIBUTE(nodiscard)
            #define NH3API_NODISCARD [[nodiscard]]
        #else
            #define NH3API_NODISCARD
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

    #ifndef NH3API_CONSTEXPR_CPP_17
        #ifdef _LIBCPP_CONSTEXPR_SINCE_CXX17
            #define NH3API_CONSTEXPR_CPP_17 _LIBCPP_CONSTEXPR_SINCE_CXX17
        #else
            #define NH3API_CONSTEXPR_CPP_17
        #endif
    #endif

    #ifndef NH3API_CONSTEXPR_CPP_14
        #ifdef _LIBCPP_CONSTEXPR_SINCE_CXX14
            #define NH3API_CONSTEXPR_CPP_14 _LIBCPP_CONSTEXPR_SINCE_CXX14
        #else
            #define NH3API_CONSTEXPR_CPP_14
        #endif
    #endif

    #ifndef NH3API_NODISCARD
        #if NH3API_HAS_CPP_ATTRIBUTE(nodiscard)
            #define NH3API_NODISCARD [[nodiscard]]
        #else
            #define NH3API_NODISCARD
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

    #ifndef NH3API_CONSTEXPR_CPP_17
        #if NH3API_CHECK_CPP17 && defined(_CONSTEXPR17)
            #define NH3API_CONSTEXPR_CPP_17 constexpr
        #else
            #define NH3API_CONSTEXPR_CPP_17
        #endif
    #endif

    #ifndef NH3API_CONSTEXPR_CPP_14
        #ifdef __cpp_constexpr
            #if __cpp_constexpr >= 201304L
                #define NH3API_CONSTEXPR_CPP_14 constexpr
            #else
                #define NH3API_CONSTEXPR_CPP_14
            #endif
        #else
            #define NH3API_CONSTEXPR_CPP_14
        #endif
    #endif

    #ifndef NH3API_NODISCARD
        #ifdef _NODISCARD
            #define NH3API_NODISCARD _NODISCARD
        #else
            #define NH3API_NODISCARD
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
    #define NH3API_CONSTEXPR_CPP_14
#endif

#ifndef NH3API_CONSTEXPR_CPP_17 
    #define NH3API_CONSTEXPR_CPP_17
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

    #if !defined(NH3API_FLAG_OPTIMIZE_FOR_SIZE)
        #ifndef NH3API_FLATTEN
            #if NH3API_HAS_CPP_ATTRIBUTE(msvc::flatten)
                #define NH3API_FLATTEN [[msvc::flatten]]
            #else
                #define NH3API_FLATTEN
            #endif
        #endif
    #else
        #if !defined(NH3API_FLATTEN)
            #define NH3API_FLATTEN __declspec(noinline)
        #endif
    #endif
#else
    #ifndef NH3API_NO_OPT_BEGIN
        #if defined(__clang__)
            #define NH3API_NO_OPT_BEGIN NH3API_PRAGMA(clang optimize off)
            #define NH3API_NO_OPT_END   NH3API_PRAGMA(clang optimize on)
        #else
            #define NH3API_NO_OPT_BEGIN NH3API_PRAGMA(GCC push_options) \
                                        NH3API_PRAGMA(GCC optimize("O0"))
            #define NH3API_NO_OPT_END   NH3API_PRAGMA(GCC pop_options)
        #endif
    #endif

    #ifndef NH3API_FORCEINLINE 
        #define NH3API_FORCEINLINE inline __attribute__((__always_inline__))
    #endif

    #ifndef NH3API_NOINLINE
        #define NH3API_NOINLINE __attribute__((__noinline__))
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

// <cstdint> header for the pre-C++0x compiler
#if NH3API_CHECK_CPP11 || NH3API_VS2010
    #include <cstdint>
#else
    #if NH3API_CHECK_MSVC

    typedef signed __int8      int8_t;
    typedef signed __int16     int16_t;
    typedef signed __int32     int32_t;
    typedef signed __int64     int64_t;
    typedef unsigned __int8    uint8_t;
    typedef unsigned __int16   uint16_t;
    typedef unsigned __int32   uint32_t;
    typedef unsigned __int64   uint64_t;

    typedef int8_t             int_least8_t;
    typedef int16_t            int_least16_t;
    typedef int32_t            int_least32_t;
    typedef int64_t            int_least64_t;
    typedef uint8_t            uint_least8_t;
    typedef uint16_t           uint_least16_t;
    typedef uint32_t           uint_least32_t;
    typedef uint64_t           uint_least64_t;

    typedef int8_t             int_fast8_t;
    typedef int32_t            int_fast16_t;
    typedef int32_t            int_fast32_t;
    typedef int64_t            int_fast64_t;
    typedef uint8_t            uint_fast8_t;
    typedef uint32_t           uint_fast16_t;
    typedef uint32_t           uint_fast32_t;
    typedef uint64_t           uint_fast64_t;

    typedef int64_t            intmax_t;
    typedef uint64_t           uintmax_t;
    typedef int32_t            intptr_t;
    typedef uint32_t           uintptr_t;
    namespace std
    {
        using ::int8_t;
        using ::int16_t;
        using ::int32_t;
        using ::int64_t;
        using ::uint8_t;
        using ::uint16_t;
        using ::uint32_t;
        using ::uint64_t;

        using ::int_least8_t;
        using ::int_least16_t;
        using ::int_least32_t;
        using ::int_least64_t;
        using ::uint_least8_t;
        using ::uint_least16_t;
        using ::uint_least32_t;
        using ::uint_least64_t;

        using ::int_fast8_t;
        using ::int_fast16_t;
        using ::int_fast32_t;
        using ::int_fast64_t;
        using ::uint_fast8_t;
        using ::uint_fast16_t;
        using ::uint_fast32_t;
        using ::uint_fast64_t;

        using ::intptr_t;
        using ::uintptr_t;
        using ::intmax_t;
        using ::uintmax_t;
    }

    #ifndef INT8_MIN
        #define INT8_MIN         (-127i8 - 1)
        #define INT16_MIN        (-32767i16 - 1)
        #define INT32_MIN        (-2147483647i32 - 1)
        #define INT64_MIN        (-9223372036854775807i64 - 1)
        #define INT8_MAX         127i8
        #define INT16_MAX        32767i16
        #define INT32_MAX        2147483647i32
        #define INT64_MAX        9223372036854775807i64
        #define UINT8_MAX        0xffui8
        #define UINT16_MAX       0xffffui16
        #define UINT32_MAX       0xffffffffui32
        #define UINT64_MAX       0xffffffffffffffffui64
    #endif

    #ifndef INT_LEAST8_MIN
        #define INT_LEAST8_MIN   INT8_MIN
        #define INT_LEAST16_MIN  INT16_MIN
        #define INT_LEAST32_MIN  INT32_MIN
        #define INT_LEAST64_MIN  INT64_MIN
        #define INT_LEAST8_MAX   INT8_MAX
        #define INT_LEAST16_MAX  INT16_MAX
        #define INT_LEAST32_MAX  INT32_MAX
        #define INT_LEAST64_MAX  INT64_MAX
        #define UINT_LEAST8_MAX  UINT8_MAX
        #define UINT_LEAST16_MAX UINT16_MAX
        #define UINT_LEAST32_MAX UINT32_MAX
        #define UINT_LEAST64_MAX UINT64_MAX

        #define INT_FAST8_MIN    INT8_MIN
        #define INT_FAST16_MIN   INT32_MIN
        #define INT_FAST32_MIN   INT32_MIN
        #define INT_FAST64_MIN   INT64_MIN
        #define INT_FAST8_MAX    INT8_MAX
        #define INT_FAST16_MAX   INT32_MAX
        #define INT_FAST32_MAX   INT32_MAX
        #define INT_FAST64_MAX   INT64_MAX
        #define UINT_FAST8_MAX   UINT8_MAX
        #define UINT_FAST16_MAX  UINT32_MAX
        #define UINT_FAST32_MAX  UINT32_MAX
        #define UINT_FAST64_MAX  UINT64_MAX
    #endif

    #ifndef INTPTR_MIN
        #define INTPTR_MIN      INT32_MIN
        #define INTPTR_MAX      INT32_MAX
        #define UINTPTR_MAX     UINT32_MAX
    #endif

    #ifndef INTMAX_MIN
        #define INTMAX_MIN       INT64_MIN
        #define INTMAX_MAX       INT64_MAX
        #define UINTMAX_MAX      UINT64_MAX
    #endif

    #ifndef PTRDIFF_MIN
        #define PTRDIFF_MIN      INTPTR_MIN
        #define PTRDIFF_MAX      INTPTR_MAX
    #endif

    #ifndef SIZE_MAX
        #define SIZE_MAX        UINTPTR_MAX
    #endif

    #ifndef SIG_ATOMIC_MIN
        #define SIG_ATOMIC_MIN   INT32_MIN
        #define SIG_ATOMIC_MAX   INT32_MAX
    #endif

    #ifndef WCHAR_MIN
        #define WCHAR_MIN        0x0000
        #define WCHAR_MAX        0xffff
    #endif

    #ifndef WINT_MIN
        #define WINT_MIN         0x0000
        #define WINT_MAX         0xffff
    #endif

    #ifndef INT8_C
        #define INT8_C(x)    (x)
        #define INT16_C(x)   (x)
        #define INT32_C(x)   (x)
        #define INT64_C(x)   (x ## LL)

        #define UINT8_C(x)   (x)
        #define UINT16_C(x)  (x)
        #define UINT32_C(x)  (x ## U)
        #define UINT64_C(x)  (x ## ULL)

        #define INTMAX_C(x)  INT64_C(x)
        #define UINTMAX_C(x) UINT64_C(x)
    #endif

    #else // NH3API_CHECK_MSVC
    #include <stdint.h>
    #endif // NH3API_CHECK_MSVC
#endif // NH3API_CHECK_CPP11
typedef uint32_t bool32_t;

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

#ifndef NH3API_FLAG_ENUM 
    #if NH3API_HAS_CPP_ATTRIBUTE(clang::flag_enum)
        #define NH3API_FLAG_ENUM enum [[clang::flag_enum]]
    #else 
        #define NH3API_FLAG_ENUM enum
    #endif
#endif

// cringeworthy C++11 macros for old MSVC compiler(s)
// Usage:
// NH3API_CONSTEXPR before function
// NH3API_NOEXCEPT after function
// NH3API_DELETED_FUNCTION after function
// NH3API_STATIC_ASSERT(expr) for a pre-C++11 compiler

#if NH3API_VS2010_ONLY
    #if !defined(NH3API_CONSTEXPR)
        #define NH3API_CONSTEXPR
    #endif // NH3API_CONSTEXPR

    #ifndef NH3API_IF_CONSTEXPR
        #define NH3API_IF_CONSTEXPR if
    #endif

    #ifndef NH3API_CONSTEXPR_VAR
        #define NH3API_CONSTEXPR_VAR const
    #endif

    #if !defined(NH3API_STATIC_ASSERT)
        #define NH3API_STATIC_ASSERT(msg, ...) static_assert(__VA_ARGS__, msg)
    #endif // NH3API_STATIC_ASSERT

    #if !defined(NH3API_NOEXCEPT)
        #define NH3API_NOEXCEPT throw()
    #endif // NH3API_NOEXCEPT

    #if !defined (NH3API_MAYBE_UNUSED)
        #define NH3API_MAYBE_UNUSED
    #endif

    #if !defined(NH3API_NOEXCEPT_EXPR)
        #define NH3API_NOEXCEPT_EXPR(...)
    #endif

    #if !defined(NH3API_DELETED_FUNCTION)
        #define NH3API_DELETED_FUNCTION {assert("Error: Call to deleted function."&&0);}
    #endif // NH3API_DELETED_FUNCTION
#endif

#if NH3API_CHECK_CPP11
    #if !defined(NH3API_CONSTEXPR)
        #define NH3API_CONSTEXPR constexpr
    #endif // NH3API_CONSTEXPR

    #ifndef NH3API_CONSTEXPR_VAR
        #define NH3API_CONSTEXPR_VAR constexpr
    #endif

    #ifndef NH3API_IF_CONSTEXPR
        #ifdef __cpp_if_constexpr
            #define NH3API_IF_CONSTEXPR if constexpr
        #else
            #define NH3API_IF_CONSTEXPR if
        #endif
    #endif

    #if !defined(NH3API_STATIC_ASSERT)
        #define NH3API_STATIC_ASSERT(msg, ...) static_assert(__VA_ARGS__, msg)
    #endif // NH3API_STATIC_ASSERT

    #if !defined(NH3API_NOEXCEPT)
        #define NH3API_NOEXCEPT noexcept
    #endif // NH3API_NOEXCEPT

    #if !defined (NH3API_MAYBE_UNUSED)
        #if NH3API_CHECK_CPP17
            #define NH3API_MAYBE_UNUSED [[maybe_unused]]
        #else
            #ifdef __has_attribute
                #if __has_attribute(unused)
                    #define NH3API_MAYBE_UNUSED __attribute__((unused))
                #else
                    #define NH3API_MAYBE_UNUSED
                #endif
            #else
                #define NH3API_MAYBE_UNUSED
            #endif
        #endif
    #endif

    #if !defined(NH3API_NOEXCEPT_EXPR)
    #if defined(NH3API_FLAG_NO_CPP_EXCEPTIONS)
        #define NH3API_NOEXCEPT_EXPR(...) noexcept
    #else
        #define NH3API_NOEXCEPT_EXPR(...) noexcept(noexcept(__VA_ARGS__))
    #endif // defined(NH3API_FLAG_NO_CPP_EXCEPTIONS)
    #endif // NH3API_NOEXCEPT_EXPR

    #if !defined(NH3API_DELETED_FUNCTION)
        #define NH3API_DELETED_FUNCTION =delete;
    #endif // NH3API_DELETED_FUNCTION
#else //NH3API_CHECK_CPP11
    #if !defined(NH3API_CONSTEXPR)
        #define NH3API_CONSTEXPR // the "closest" pre-C++11 alternative is either 'const' or 'inline'...
    #endif // NH3API_CONSTEXPR

    #ifndef NH3API_IF_CONSTEXPR
        #define NH3API_IF_CONSTEXPR if
    #endif

    #ifndef NH3API_CONSTEXPR_VAR
        #define NH3API_CONSTEXPR_VAR const
    #endif

    #if _MSC_VER < 1600
        #define nullptr NULL
    #endif

    #if NH3API_MSVC_VERSION < NH3API_MSVC_VERSION_2010
        template <bool condition> struct STATIC_ASSERTION_FAILURE;
        template <> struct STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };

        #if !defined(NH3API_STATIC_ASSERT)
            //#define NH3API_CONCAT_LINE_FILE "static assertion failed: " __FILE__ " at line: " NH3API_LINE_AS_STRING(__LINE__)
            #define NH3API_STATIC_ASSERT(msg, ...) \
            enum { NH3API_JOIN_STRING(static_assert_failure_, __COUNTER__) \
                = sizeof(STATIC_ASSERTION_FAILURE< (bool)( __VA_ARGS__ ) >) }
        #endif // NH3API_STATIC_ASSERT
    #else // __cpp_static_assert
        #define NH3API_STATIC_ASSERT(msg, ...) static_assert(__VA_ARGS__, msg)
    #endif

    // use MSVC STL _NOEXCEPT
    #if !defined(NH3API_NOEXCEPT)
        #if defined(_NOEXCEPT)
            #define NH3API_NOEXCEPT _NOEXCEPT
        #else
            #define NH3API_NOEXCEPT throw()
        #endif
    #endif // NH3API_NOEXCEPT

    #if !defined(NH3API_MAYBE_UNUSED)
        #define NH3API_MAYBE_UNUSED
    #endif

    #if !defined(NH3API_NOEXCEPT_EXPR)
        #if defined(NH3API_FLAG_NO_CPP_EXCEPTIONS)
            #define NH3API_NOEXCEPT_EXPR(...) NH3API_NOEXCEPT
        #else
            #define NH3API_NOEXCEPT_EXPR(...)
        #endif // defined(NH3API_FLAG_NO_CPP_EXCEPTIONS)
    #endif // NH3API_NOEXCEPT_EXPR

    #if !defined(NH3API_DELETED_FUNCTION)
        #if (_MSC_VER >= 1800)
            #define NH3API_DELETED_FUNCTION =delete;
        #else
            #define NH3API_DELETED_FUNCTION {assert("Error: Call to deleted function."&&0);}
        #endif // vs2013
    #endif // NH3API_DELETED_FUNCTION
#endif //NH3API_CHECK_CPP11

// C++11 features test

#ifndef NH3API_STD_DELEGATING_CONSTRUCTORS
    #if NH3API_CHECK_CPP11 || defined(__cpp_delegating_constructors)
        #define NH3API_STD_DELEGATING_CONSTRUCTORS (1)
    #else
        #define NH3API_STD_DELEGATING_CONSTRUCTORS (0)
    #endif
#endif

#ifndef NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS
// default template arguments for standalone functions support
    #ifdef _MSC_FULL_VER
        #if NH3API_CHECK_CPP11 || (_MSC_FULL_VER > 170051025) // Visual Studio 2012 November CTP
            #define NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS (1)
        #else
            #define NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS (0)
        #endif
    #else
        #if NH3API_CHECK_CPP11
            #define NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS (1)
        #else
            #define NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS (0)
        #endif
    #endif
#endif

#ifndef NH3API_STD_INLINE_VARIABLES
    #ifdef __cpp_inline_variables
        #define NH3API_STD_INLINE_VARIABLES (1)
    #else
        #define NH3API_STD_INLINE_VARIABLES (0)
    #endif
#endif

#ifndef NH3API_STD_VARIADIC_ARGUMENTS_FULL_SUPPORT
    #if NH3API_CHECK_MSVC
        #if _MSC_FULL_VER >= 170051025 // full support since MSVC 2012 November CTP
            #define NH3API_STD_VARIADIC_ARGUMENTS_FULL_SUPPORT (1)
        #else
            #define NH3API_STD_VARIADIC_ARGUMENTS_FULL_SUPPORT (0)
        #endif
    #elif NH3API_CHECK_CPP11
        #define NH3API_STD_VARIADIC_ARGUMENTS_FULL_SUPPORT (1)
    #else
        #define NH3API_STD_VARIADIC_ARGUMENTS_FULL_SUPPORT (0)
    #endif
#endif

#ifndef NH3API_STD_INITIALIZER_LIST
    #if NH3API_CHECK_CPP11 || defined(__cpp_initializer_lists)
        #define NH3API_STD_INITIALIZER_LIST (1)
    #else 
        #define NH3API_STD_INITIALIZER_LIST (0)
    #endif  
#endif

#ifndef NH3API_STD_RELAXED_CONSTEXPR
    #ifdef __cpp_constexpr
        #if __cpp_constexpr >= 201304L
            #define NH3API_STD_RELAXED_CONSTEXPR (1)
        #endif
    #endif
#endif

#ifndef NH3API_STD_RELAXED_CONSTEXPR
    #define NH3API_STD_RELAXED_CONSTEXPR (0)
#endif

namespace nh3api
{

struct dummy_tag_t
{}
#if NH3API_STD_RELAXED_CONSTEXPR
constexpr
#endif
#if NH3API_STD_INLINE_VARIABLES
inline
#endif
// no-op constructor tag, used to optimize low-level manipulations for objects
const dummy_tag;
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
#if NH3API_STD_RELAXED_CONSTEXPR
constexpr
#endif
#if NH3API_STD_INLINE_VARIABLES
inline
#endif
// omit vftable pointer assign at base
const omit_base_vftable_tag;
// example:
// NH3API_VIRTUAL_CLASS derived : public base
// {
// derived() : base(nh3api::omit_base_vftable_tag)
// { NH3API_SET_VFTABLE(); }
// };
// derived:
// assign derived::vftable to this, omit assignment to base in base constructor
}

#ifndef NH3API_DEFAULT_DESTRUCTOR
    #if NH3API_CHECK_CPP11 
        #define NH3API_DEFAULT_DESTRUCTOR(CLASS_NAME) NH3API_FORCEINLINE ~CLASS_NAME() NH3API_NOEXCEPT = default;
    #else   
        #define NH3API_DEFAULT_DESTRUCTOR(CLASS_NAME) NH3API_FORCEINLINE ~CLASS_NAME() NH3API_NOEXCEPT {}
    #endif
#endif

#ifndef NH3API_DELEGATE_DUMMY
    #if NH3API_STD_DELEGATING_CONSTRUCTORS
        #define NH3API_DELEGATE_DUMMY(CLASS_NAME) : CLASS_NAME(::nh3api::dummy_tag)
    #else
        #define NH3API_DELEGATE_DUMMY(CLASS_NAME)
    #endif
#endif

#ifndef NH3API_DELEGATE_DUMMY_OR_BASE
    #if NH3API_STD_DELEGATING_CONSTRUCTORS
        #define NH3API_DELEGATE_DUMMY_OR_BASE(CLASS_NAME, BASE) : CLASS_NAME(::nh3api::dummy_tag)
    #else
        #define NH3API_DELEGATE_DUMMY_OR_BASE(CLASS_NAME, BASE) : BASE(::nh3api::dummy_tag)
    #endif
#endif

// final keyword
#ifndef NH3API_FINAL
    #if NH3API_CHECK_CPP11
        #define NH3API_FINAL final
    #else
        #define NH3API_FINAL
    #endif
#endif // NH3API_FINAL

// explicit for C++11
#ifndef NH3API_EXPLICIT_CPP11
    #if NH3API_CHECK_CPP11
        #define NH3API_EXPLICIT_CPP11 explicit
    #else
        #define NH3API_EXPLICIT_CPP11
    #endif
#endif

// std::array<T, N> support
#ifndef NH3API_STD_ARRAY
    #if NH3API_CHECK_CPP11 || NH3API_VS2010
        #define NH3API_STD_ARRAY (1)
    #else
        #define NH3API_STD_ARRAY (0)
    #endif
#endif

#ifndef NH3API_STD_STRING_VIEW
    #ifdef __cpp_lib_string_view
        #define NH3API_STD_STRING_VIEW (1)
    #else
        #define NH3API_STD_STRING_VIEW (0)
    #endif
#endif

#ifndef NH3API_STD_HASH 
    #if NH3API_CHECK_CPP11 || (NH3API_MSVC_STL_VERSION > NH3API_MSVC_STL_VERSION_2010)
        #define NH3API_STD_HASH (1)
    #else 
        #define NH3API_STD_HASH (0)
    #endif
#endif

// ranges library support for containers
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

#ifndef NH3API_HAS_INCLUDE
    #ifdef __has_include
        #define NH3API_HAS_INCLUDE(ARG) __has_include(ARG)
    #else
        #define NH3API_HAS_INCLUDE(ARG) (0)
    #endif
#endif

// check if compiler has builtins (MSVC since 2015)
#ifndef NH3API_HAS_BUILTINS
    #ifdef __has_builtin
        #define NH3API_HAS_BUILTINS (1)
        #define NH3API_HAS_BUILTIN(BUILTIN_TO_CHECK) __has_builtin(BUILTIN_TO_CHECK)
    #else
        #define NH3API_HAS_BUILTINS                      (0)
        #define NH3API_HAS_BUILTIN(BUILTIN_TO_CHECK)     (0)
    #endif
#endif

#ifndef NH3API_HAS_IS_CONSTANT_EVALUATED
    #if NH3API_HAS_BUILTIN(__builtin_is_constant_evaluated)
        #define NH3API_HAS_IS_CONSTANT_EVALUATED (1)
        namespace nh3api
        {
        inline constexpr bool is_constant_evaluated() noexcept
        { return __builtin_is_constant_evaluated(); }
        }
    #elif NH3API_CHECK_MSVC && NH3API_STD_RELAXED_CONSTEXPR
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

#ifndef NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
    #if NH3API_HAS_IS_CONSTANT_EVALUATED && NH3API_STD_RELAXED_CONSTEXPR
        #define NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED constexpr
    #else
        #define NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
    #endif
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
    #if NH3API_STD_MOVE_SEMANTICS
        #define NH3API_SFINAE_BEGIN(...) ,typename nh3api::tt::enable_if<__VA_ARGS__, bool>::type = false
    #else
        #define NH3API_SFINAE_BEGIN(...)
    #endif
#endif

// as for the C++98 mode, use function argument to implement SFINAE via std::enable_if
#ifndef NH3API_SFINAE_END
    #if !NH3API_STD_MOVE_SEMANTICS
        #define NH3API_SFINAE_END(...) ,typename nh3api::tt::enable_if<__VA_ARGS__, bool>::type = false
    #else
        #define NH3API_SFINAE_END(...)
    #endif
#endif

#ifndef NH3API_REQUIRES
    #if NH3API_STD_MOVE_SEMANTICS
        #ifdef __cpp_concepts
            #define NH3API_REQUIRES(...) > requires(__VA_ARGS__) // concept form is more readable
        #else
            #define NH3API_REQUIRES(...) ,typename std::enable_if<__VA_ARGS__, bool>::type = false>
        #endif
    #else
        #define NH3API_REQUIRES(...) >
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
    #ifdef NH3API_FLAG_INLINE_HEADERS
        #if NH3API_STD_INLINE_VARIABLES
            #define NH3API_INLINE_OR_EXTERN inline
        #else 
            #define NH3API_INLINE_OR_EXTERN
            #error 'NH3API_FLAG_INLINE_HEADERS' requires C++17
        #endif
    #else
        #define NH3API_INLINE_OR_EXTERN extern
    #endif
#endif

#ifndef NH3API_INLINE_OR_EXTERN_INIT
    #ifdef NH3API_FLAG_INLINE_HEADERS
        #define NH3API_INLINE_OR_EXTERN_INIT(...) = __VA_ARGS__
    #else
        #define NH3API_INLINE_OR_EXTERN_INIT(...)
    #endif
#endif

#ifndef NH3API_INLINE_STATIC_VARIABLE
    #ifdef NH3API_FLAG_INLINE_HEADERS
        #if NH3API_STD_INLINE_VARIABLES
            #define NH3API_INLINE_STATIC_VARIABLE static inline
        #else 
            #define NH3API_INLINE_STATIC_VARIABLE
            #error 'NH3API_FLAG_INLINE_HEADERS' requires C++17
        #endif
    #else  
        #define NH3API_INLINE_STATIC_VARIABLE static
    #endif
#endif

#ifndef NH3API_INLINE_STATIC_VARIABLE_INIT
    #ifdef NH3API_FLAG_INLINE_HEADERS
        #define NH3API_INLINE_STATIC_VARIABLE_INIT(...) = __VA_ARGS__
    #else
        #define NH3API_INLINE_STATIC_VARIABLE_INIT(...)
    #endif
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
    #define NH3API_NOEXCEPT_DESTRUCT NH3API_NOEXCEPT_EXPR(::nh3api::tt::is_nothrow_destructible<value_type>::value)
#endif

namespace nh3api
{

// NH3API settings as C++ constants
namespace flags
{
enum : unsigned char
{

    #if NH3API_CHECK_MSVC 
    use_msvc = true,
    use_gcc = false,
    use_clang = false, 
    use_clang_cl = false,
    #elif NH3API_CHECK_MINGW
    use_msvc = false,
    use_gcc = true,
    use_clang = false, 
    use_clang_cl = false,
    #elif NH3API_CHECK_CLANG 
    use_msvc = false,
    use_gcc = false,
    use_clang = true, 
    use_clang_cl = false,
    #elif NH3API_CHECK_CLANG_CL 
    use_msvc = false,
    use_gcc = false,
    use_clang = false, 
    use_clang_cl = true,
    #endif 

    // see 'NH3API_FLAG_NO_CPP_EXCEPTIONS' flag
    no_exceptions =
    #ifdef NH3API_FLAG_NO_CPP_EXCEPTIONS
        true
    #else
        false
    #endif
    ,

    // Use optional ERA 3 SDK module
    use_era = 
    #ifdef NH3API_FLAG_USE_ERA
        true
    #else 
        false
    #endif

};

} // namespace nh3api::flags

} // namespace nh3api

#ifndef NH3API_MAJOR_VERSION
    #define NH3API_MAJOR_VERSION 1
    #define NH3API_MINOR_VERSION 1
    #define NH3API_VERSION_STRING "NH3API v1.1"
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
        #if NH3API_HAS_BUILTIN(__builtin_assume)
            #define NH3API_ASSUME(...) do { __builtin_assume(__VA_ARGS__); } while(0)
        #elif __has_attribute(__assume__)
            #define NH3API_ASSUME(...) __attribute__((__assume__(__VA_ARGS__)))
        #elif NH3API_HAS_BUILTIN(__builtin_unreachable)
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
    // IntelliSense is broken before VS2015, it uses EDG compiler as a backend and has different ABI, showing error
    // but MSVC compiles that without errors, so we just turn off this while IntelliSense is on, but turn back in when it's off.
    #if defined(__INTELLISENSE__) && (NH3API_MSVC_VERSION < NH3API_MSVC_VERSION_2015) && (NH3API_MSVC_VERSION >= NH3API_MSVC_VERSION_MIN)
        #define NH3API_SIZE_ASSERT(EXPECTED_SIZE, ...) 
    #else
        #define NH3API_SIZE_ASSERT(EXPECTED_SIZE, ...) NH3API_STATIC_ASSERT("size mismatch", sizeof(__VA_ARGS__) == EXPECTED_SIZE)
    #endif
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
#if NH3API_HAS_BUILTIN(__builtin_constant_p)
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
template<class T> NH3API_NODISCARD NH3API_FORCEINLINE
typename T::vftable_t* get_vftable(T* ptr) NH3API_NOEXCEPT
#if NH3API_HAS_BUILTIN(__builtin_launder)
{ return *reinterpret_cast<typename T::vftable_t**>(__builtin_launder(ptr)); }
#else 
{ return *reinterpret_cast<typename T::vftable_t**>(ptr); }
#endif

// requires T::vftable_t
template<class T> NH3API_NODISCARD NH3API_FORCEINLINE
const typename T::vftable_t* get_vftable(const T* ptr) NH3API_NOEXCEPT
#if NH3API_HAS_BUILTIN(__builtin_get_vtable_pointer)
{ return *reinterpret_cast<const typename T::vftable_t* const*>(__builtin_get_vtable_pointer(ptr)); }
#else 
{ return *reinterpret_cast<const typename T::vftable_t* const*>(ptr); }
#endif

// is supposed to be specialized for each .exe-s polymorphic class
template<class T>
struct vftable_address
{ 
static const
#if NH3API_STD_INLINE_VARIABLES
inline constexpr
#endif
uintptr_t address = 0; 
};

template<typename T> NH3API_NODISCARD NH3API_FORCEINLINE
const typename T::vftable_t* get_type_vftable() NH3API_NOEXCEPT
{ return reinterpret_cast<const typename T::vftable_t*>(vftable_address<T>::address); }

template<typename T> NH3API_NODISCARD NH3API_FORCEINLINE
const typename T::vftable_t* get_type_vftable(const T*) NH3API_NOEXCEPT
{ return reinterpret_cast<const typename T::vftable_t*>(vftable_address<T>::address); }

// for constructors
template<typename T>
void set_vftable(T* ptr)
{
    NH3API_MEMSHIELD_BEGIN
    //#ifdef __cpp_lib_launder
    //*reinterpret_cast<void**>(std::launder(ptr)) = get_type_vftable(ptr);
    //#else
    *reinterpret_cast<const void**>(ptr) = get_type_vftable(ptr);
    //#endif
    NH3API_MEMSHIELD_END
}

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
#if NH3API_HAS_CPP_ATTRIBUTE(clang::no_specializations)
[[clang::no_specializations("Specialize vftable_address<T> instead.")]]
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
