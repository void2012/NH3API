//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// NH3API 2024
// Helpful Macros, some extra stuff...
// See README
#pragma once

#include <cassert>
#include <cstring>
#include <cstddef>

#ifndef _WIN32
    #error NH3API targets only windows
#endif

#pragma once
#if (defined(_WIN64) || defined(__x86_64__)) || (!defined(_M_IX86) && !defined(__i386__))
    #error Heroes III is a 32-bit game. Please switch your compiler to x86 mode.
#endif

#if (defined(_MSC_VER)) || (defined(__MINGW32__)) || (defined(__clang__))
    // pass
#else
    #error Unsupported compiler. NH3API supports MSVC, GCC and Clang only
#endif

#define NH3API_MIN(x,y) ((x) < (y) ? (x) : (y))
#define NH3API_MAX(x,y) ((x) > (y) ? (x) : (y))

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef NH3API_JOIN_STRING
    #define NH3API_JOIN_STRING_HELPER(X, Y) X##Y
    #define NH3API_JOIN_STRING(X, Y) NH3API_JOIN_STRING_HELPER(X, Y)
#endif

#define NH3API_WRAP_STRINGIFY(X) #X
#define NH3API_STRINGIFY(X) NH3API_WRAP_STRINGIFY(X)

#ifndef NH3API_CHECK_CLANG_CL
    #if defined(_MSC_VER) && defined(__clang__)
        #define NH3API_CHECK_CLANG_CL (1)
    #else
        #define NH3API_CHECK_CLANG_CL (0)
    #endif
#endif

// Visual Studio didn't update __cplusplus up until Visual Studio 2017 version 15.7
// And ever since it is required to have the '/Zc:__cplusplus' flag to update it
#if defined(_MSC_VER) && !defined(__clang__)
    #if (_MSC_VER < 1400)
        #error NH3API requires at least Visual Studio 2005 to work.
    #endif

    #ifndef _CPPLIB_VER
        #error NH3API supports only MSVC STL with MSVC
    #else
        #define NH3API_MSVC_STL (1)
    #endif

    #if defined(_MSC_FULL_VER)
        #if _MSC_FULL_VER >= 191426428L // Visual Studio 2017 version 15.7
            #if __cplusplus < 201103L
            #error missing flag '/Zc:__cplusplus'
            #else
                // C++ versions macros check
                #ifndef NH3API_CHECK_CPP11
                    #define NH3API_CHECK_CPP11 (__cplusplus>=201103L)
                #endif // NH3API_CHECK_CPP11

                #ifndef NH3API_CHECK_CPP14
                    #define NH3API_CHECK_CPP14 (__cplusplus>=201402L)
                #endif // NH3API_CHECK_CPP14

                #ifndef NH3API_CHECK_CPP17
                    #define NH3API_CHECK_CPP17 (__cplusplus>=201703L)
                #endif // NH3API_CHECK_CPP17

                #ifndef NH3API_CHECK_CPP20
                    #define NH3API_CHECK_CPP20 (__cplusplus>=202002L)
                #endif // NH3API_CHECK_CPP20

                #ifndef NH3API_CHECK_CPP23
                    #define NH3API_CHECK_CPP23 (__cplusplus>=202302L)
                #endif // NH3API_CHECK_CPP23
            #endif
        #elif (_MSC_FULL_VER >= 190023506L) // Visual Studio 2015 supports all C++11 features since update 1
            #define NH3API_CHECK_CPP11 (1)
            #define NH3API_CHECK_CPP14 (0)
            #define NH3API_CHECK_CPP17 (0)
            #define NH3API_CHECK_CPP20 (0)
            #define NH3API_CHECK_CPP23 (0)
        #endif
    #else
        #define NH3API_CHECK_CPP11 (0)
        #define NH3API_CHECK_CPP14 (0)
        #define NH3API_CHECK_CPP17 (0)
        #define NH3API_CHECK_CPP20 (0)
        #define NH3API_CHECK_CPP23 (0)
    #endif
#else

// C++ versions macros check
#ifndef NH3API_CHECK_CPP11
    #define NH3API_CHECK_CPP11 (__cplusplus>=201103L)
#endif // NH3API_CHECK_CPP11

#ifndef NH3API_CHECK_CPP14
    #define NH3API_CHECK_CPP14 (__cplusplus>=201402L)
#endif // NH3API_CHECK_CPP14

#ifndef NH3API_CHECK_CPP17
    #define NH3API_CHECK_CPP17 (__cplusplus>=201702L)
#endif // NH3API_CHECK_CPP17

#ifndef NH3API_CHECK_CPP20
    #define NH3API_CHECK_CPP20 (__cplusplus>=202002L)
#endif // NH3API_CHECK_CPP20

#ifndef NH3API_CHECK_CPP23
    #define NH3API_CHECK_CPP23 (__cplusplus>=202302L)
#endif // NH3API_CHECK_CPP20

#endif // defined(_MSC_VER)

// MSVC compiler macro check
#ifndef NH3API_CHECK_MSVC
    #if defined(_MSC_VER) && !defined(__clang__)
        #define NH3API_CHECK_MSVC (1)
    #else
        #define NH3API_CHECK_MSVC (0)
    #endif
#endif

#if NH3API_CHECK_MSVC
    #if NH3API_CHECK_CPP14
        #define NH3API_DEPRECATED(msg) [[deprecated(msg)]]
    #else
        #define NH3API_DEPRECATED(msg) __declspec(deprecated(msg))
    #endif
#else
    #if __has_attribute(deprecated)
        #define NH3API_DEPRECATED(msg) [[deprecated(msg)]]
    #else
        #define NH3API_DEPRECATED(msg) // __attrubute__((deprecated)) // can not apply to pure virtual function...
    #endif
#endif

#if defined(_MSC_VER) && !defined(__clang__)
    #define NH3API_VS2010 (_MSC_VER >= 1600 && _MSC_FULL_VER < 190023506L) // Visual Studio 2010 has partial C++11 support.
#else
    #define NH3API_VS2010 (0)
#endif

#if defined(_MSC_VER) && !defined(__clang__)
    #define NH3API_VS2012_2013 ((_MSC_VER == 1700) || (_MSC_VER == 1800))
#else
    #define NH3API_VS2012_2013 (0)
#endif

#if defined(_MSC_VER) && !defined(__clang__)
    #define NH3API_VS2010_ONLY (_MSC_VER >= 1600 && _MSC_VER < 1700) // Visual Studio 2010 has partial C++11 support.
#else
    #define NH3API_VS2010_ONLY (0)
#endif

#ifndef NH3API_ARGS_DOTS
    #if NH3API_VS2010_ONLY
        #define NH3API_ARGS_DOTS
    #else
        #define NH3API_ARGS_DOTS ...
    #endif
#endif

#if defined (__clang__) && !defined(_MSC_VER)
    #define NH3API_CHECK_CLANG (1)
#else
    #define NH3API_CHECK_CLANG (0)
#endif

#if defined (__GNUC__) && defined(__MINGW32__) && !defined(__clang__)
#define NH3API_CHECK_MINGW (1)
#else
#define NH3API_CHECK_MINGW (0)
#endif

#ifndef NH3API_CHECK_MINGW_CLANG
    #define NH3API_CHECK_MINGW_CLANG (NH3API_CHECK_MINGW || NH3API_CHECK_CLANG)
#endif // NH3API_CHECK_MINGW_CLANG

#if (!NH3API_CHECK_CLANG) && (!NH3API_CHECK_MINGW) && (!NH3API_CHECK_MSVC) && (!NH3API_CHECK_CLANG_CL)
    #error Unsupported compiler
#endif

#if NH3API_CHECK_MINGW
    #if (__GNUC__ < 5)
        #error NH3API requires at least GCC 5 to work.
    #endif
    #if (__cplusplus < 201103L)
        #error NH3API doesnt work on GCC with -std=c++98
    #endif
#endif

#if NH3API_CHECK_CLANG
    #if (__clang_major__ < 10)
        #error NH3API requires at least clang 10 to work.
    #endif
    #if (__cplusplus < 201103L)
        #error NH3API doesnt work on clang with -std=c++98
    #endif
#endif

#ifndef NH3API_DEBUG
    #ifndef NDEBUG
        #define NH3API_DEBUG (1)
    #else
        #define NH3API_DEBUG (0)
    #endif
#endif

// improving the debug mode compilation time
#if NH3API_DEBUG
    // some standalone functions use this for both optimization and
    // to not mess with the linker, so we still need to keep the inline keyword
    // but forceinline is more aggressive and takes more compile time
    #define NH3API_FORCEINLINE inline
    #define NH3API_FLATTEN
    #define NH3API_INLINE_LARGE
#endif

#ifndef NH3API_DLLEXPORT
    #if NH3API_CHECK_MSVC || NH3API_CHECK_CLANG_CL
        #define NH3API_DLLEXPORT __declspec(dllexport)
    #else
        #define NH3API_DLLEXPORT __attribute__((__dllexport__))
    #endif
#endif

#ifndef NH3API_DLLIMPORT
    #if NH3API_CHECK_MSVC || NH3API_CHECK_CLANG_CL
        #define NH3API_DLLIMPORT __declspec(dllimport)
    #else
        #define NH3API_DLLIMPORT __attribute__((__dllimport__))
    #endif
#endif

// Compiler-specific pragma wrapper
#if NH3API_CHECK_MSVC || NH3API_CHECK_CLANG_CL
    #if !defined(NH3API_PRAGMA)
        #define NH3API_PRAGMA(S) __pragma(S)
    #endif
#else
    #if !defined(NH3API_PRAGMA)
        #define NH3API_PRAGMA(S) _Pragma(#S)
    #endif
#endif // NH3API_PRAGMA

// STL vendor check

#if defined(__GLIBCXX__) // GNU GCC libstdc++
    #ifndef NDEBUG
        #define _GLIBCXX_DEBUG 1
    #include <debug/debug.h>
    #endif

    #ifndef NH3API_GCC_STL
        #define NH3API_GCC_STL (1)
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

#elif defined(_LIBCPP_VERSION) // clang libc++
    #ifndef NDEBUG
        #define _LIBCPP_DEBUG 1
    #endif

    #ifndef NH3API_CLANG_STL
        #define NH3API_CLANG_STL (1)
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

#elif defined(_CPPLIB_VER) // MSVC STL
// MSVC defines macros for release and debug version automatically

    #ifndef NH3API_MSVC_STL
        #define NH3API_MSVC_STL (1)
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

#else
    #error Unknown STL vendor
#endif


#ifndef NH3API_MSVC_STL_VERSION 
    #ifdef _CPPLIB_VER
        #define NH3API_MSVC_STL_VERSION (_CPPLIB_VER)
    #else 
        #define NH3API_MSVC_STL_VERSION (0)
    #endif
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



// 1) use
// NH3API_NO_OPT_BEGIN
// ...
// NH3API_NO_OPT_END
// to wrap code that must have no optimizations
// 2) NH3API_FORCEINLINE forces function to be inline
// 3) NH3API_NOINLINE forces function to not to be inlined in any scenario

#if NH3API_CHECK_MSVC || NH3API_CHECK_CLANG_CL
    #ifndef NH3API_NO_OPT
        #define NH3API_NO_OPT_BEGIN NH3API_PRAGMA(optimize("",off))
        #define NH3API_NO_OPT_END   NH3API_PRAGMA(optimize("",on))
        #define NH3API_NO_OPT
    #endif // NH3API_NO_OPT

    #ifndef NH3API_FORCEINLINE
        #define NH3API_FORCEINLINE __forceinline
    #endif // NH3API_FORCEINLINE

    #ifndef NH3API_NOINLINE
        #define NH3API_NOINLINE __declspec(noinline)
    #endif // NH3API_NOINLINE

    #ifndef NH3API_MALLOC
        #ifdef _VCRT_ALLOCATOR
            #define NH3API_MALLOC(...) _VCRT_ALLOCATOR __declspec(restrict)
        #else
            #define NH3API_MALLOC(...) __declspec(restrict)
        #endif
    #endif

    #ifndef NH3API_NODISCARD
        #ifdef _NODISCARD
            #define NH3API_NODISCARD _NODISCARD
        #else
            #define NH3API_NODISCARD
        #endif
    #endif

    #if !defined(NH3API_FLAG_OPTIMIZE_FOR_SIZE)
        #ifndef NH3API_FLATTEN
            #ifdef __has_cpp_attribute
                #if __has_cpp_attribute(msvc::flatten)
                    #define NH3API_FLATTEN [[msvc::flatten]]
                #else
                    #define NH3API_FLATTEN
                #endif
            #else
                #define NH3API_FLATTEN
            #endif
        #endif
    #else
        #if !defined(NH3API_FLATTEN)
            #define NH3API_FLATTEN __declspec(noinline)
        #endif
    #endif
#endif // NH3API_CHECK_MSVC

#if NH3API_CHECK_CLANG
    #ifndef NH3API_NO_OPT
        #define NH3API_NO_OPT_BEGIN NH3API_PRAGMA(clang optimize off)
        #define NH3API_NO_OPT_END   NH3API_PRAGMA(clang optimize on)
        #define NH3API_NO_OPT
    #endif // NH3API_NO_OPT

    #ifndef NH3API_FORCEINLINE
        #define NH3API_FORCEINLINE __attribute__((__always_inline__))
    #endif

    #ifndef NH3API_NOINLINE
        #define NH3API_NOINLINE __attribute__((__noinline__))
    #endif

    #ifndef NH3API_MALLOC
        #define NH3API_MALLOC(...) __attribute__((__returns_nonnull__, __malloc__, __alloc_size__(__VA_ARGS__)))
    #endif

    #ifndef NH3API_NODISCARD
        #if defined(_LIBCPP_NODISCARD)
            #define NH3API_NODISCARD _LIBCPP_NODISCARD
        #elif defined(_NODISCARD)
            #define NH3API_NODISCARD _NODISCARD
        #else
            #define NH3API_NODISCARD
        #endif
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

#if defined(__GNUC__)
    #ifndef NH3API_NO_OPT
        #define NH3API_NO_OPT_BEGIN NH3API_PRAGMA(GCC push_options) \
                                    NH3API_PRAGMA(GCC optimize("O0"))
        #define NH3API_NO_OPT_END   NH3API_PRAGMA(GCC pop_options)
        #define NH3API_NO_OPT
    #endif // NH3API_NO_OPT
    #ifndef NH3API_FORCEINLINE
        #define NH3API_FORCEINLINE __attribute__((__always_inline__))
    #endif

    #ifndef NH3API_NOINLINE
        #define NH3API_NOINLINE __attribute__((__noinline__))
    #endif

    #ifndef NH3API_MALLOC
        #define NH3API_MALLOC(...) __attribute__((__returns_nonnull__, __malloc__, __alloc_size__(__VA_ARGS__)))
    #endif

    #ifndef NH3API_NODISCARD
        #define NH3API_NODISCARD [[nodiscard]]
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

/*
// force compiler to treat key warnings as errors
#if NH3API_CHECK_MSVC
    #pragma warning(error : 4716) // function must always return
#elif NH3API_CHECK_MINGW
    #pragma GCC diagnostic error "-Wreturn-type"
#elif NH3API_CHECK_CLANG || NH3API_CHECK_CLANG_CL
    #pragma clang diagnostic error "-Wreturn-type"
#endif
*/

#ifndef NH3API_HOT
    #ifdef __has_attribute
        #if __has_attribute(__hot__)
            #define NH3API_HOT __attribute__((__hot__))
        #endif
    #endif
#endif

#ifndef NH3API_HOT
    #define NH3API_HOT
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

#if NH3API_CHECK_MSVC && !NH3API_CHECK_CLANG_CL
    #if !defined(NH3API_DISABLE_WARNING)
        #define NH3API_DISABLE_WARNING_BEGIN(GCC_CLANG, MSVC) NH3API_PRAGMA(warning(push)) \
                                                              NH3API_PRAGMA(warning(disable : MSVC))

        #define NH3API_DISABLE_WARNING_END   NH3API_PRAGMA(warning(pop))
        #define NH3API_DISABLE_WARNING
    #endif // NH3API_DISABLE_WARNING
#endif // NH3API_CHECK_MSVC

#if NH3API_CHECK_CLANG || NH3API_CHECK_CLANG_CL
    #if !defined(NH3API_DISABLE_WARNING)
        #define NH3API_DISABLE_WARNING_BEGIN(GCC_CLANG, MSVC) NH3API_PRAGMA(clang diagnostic push) \
                                                              NH3API_PRAGMA(clang diagnostic ignored GCC_CLANG )
        #define NH3API_DISABLE_WARNING_END   NH3API_PRAGMA(clang diagnostic pop)
        #define NH3API_DISABLE_WARNING
    #endif // NH3API_DISABLE_WARNING
#endif // __clang__

#if defined(__GNUC__)
    #if !defined(NH3API_DISABLE_WARNING)
        #define NH3API_DISABLE_WARNING_BEGIN(GCC_CLANG, MSVC) NH3API_PRAGMA(GCC diagnostic push) \
                                                              NH3API_PRAGMA(GCC diagnostic ignored GCC_CLANG )
        #define NH3API_DISABLE_WARNING_END   NH3API_PRAGMA(GCC diagnostic pop)
        #define NH3API_DISABLE_WARNING
    #endif // NH3API_DISABLE_WARNING
#endif // __GNUC__

#ifndef NH3API_WEAK
    #if NH3API_CHECK_MSVC || NH3API_CHECK_CLANG_CL
        #define NH3API_WEAK __declspec(selectany)
    #else
        #define NH3API_WEAK __attribute__((__weak__))
    #endif
#endif

#ifndef NH3API_MSVC_INTRIN
    #ifdef _MSVC_INTRINSIC
        #define NH3API_MSVC_INTRIN _MSVC_INTRINSIC
    #else
        #define NH3API_MSVC_INTRIN
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
    #else // NH3API_CHECK_MSVC
    #include <stdint.h>
    #endif // NH3API_CHECK_MSVC
#endif // NH3API_CHECK_CPP11
typedef uint32_t bool32_t;

// make POSIX compiler follow the MSVC struct layout
#ifndef NH3API_MSVC_LAYOUT
    #if NH3API_CHECK_MSVC || NH3API_CHECK_CLANG_CL
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
#ifndef NH3API_NO_VFTABLE_ASSIGN
    #if NH3API_CHECK_MSVC
        #define NH3API_NO_VFTABLE_ASSIGN __declspec(novtable)
    #else
        #define NH3API_NO_VFTABLE_ASSIGN
    #endif
#endif

// see NH3API_NO_VFTABLE_ASSIGN
#ifndef NH3API_VIRTUAL_CLASS
    #if NH3API_CHECK_MSVC
        #define NH3API_VIRTUAL_CLASS class __declspec(novtable)
    #else
        #if __has_attribute(novtable)
            #define NH3API_VIRTUAL_CLASS __attribute__((novtable))
        #else
            #define NH3API_VIRTUAL_CLASS class
        #endif
    #endif
#endif

#ifndef NH3API_CONST
    #if !NH3API_CHECK_MSVC
        #if __has_attribute(__const__)
            #define NH3API_CONST __attribute__((__const__))
        #else
            #define NH3API_CONST
        #endif
    #else
        #define NH3API_CONST
    #endif
#endif

// see NH3API_NO_VFTABLE_ASSIGN
#ifndef NH3API_VIRTUAL_STRUCT
    #if NH3API_CHECK_MSVC
        #define NH3API_VIRTUAL_STRUCT struct __declspec(novtable)
    #else
        #define NH3API_VIRTUAL_STRUCT struct
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
        #if NH3API_CHECK_CPP17
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
    #include <yvals.h>
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

    template <bool condition> struct STATIC_ASSERTION_FAILURE;
    template <> struct STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };

    #if !defined(NH3API_STATIC_ASSERT)
        //#define NH3API_CONCAT_LINE_FILE "static assertion failed: " __FILE__ " at line: " NH3API_LINE_AS_STRING(__LINE__)
        #define NH3API_STATIC_ASSERT(msg, ...) \
         enum { NH3API_JOIN_STRING(static_assert_failure_, __COUNTER__) \
            = sizeof(STATIC_ASSERTION_FAILURE< (bool)( __VA_ARGS__ ) >) }
    #endif // NH3API_STATIC_ASSERT

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
    #if NH3API_CHECK_CPP11
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
    #define NH3API_STD_INITIALIZER_LIST NH3API_STD_MOVE_SEMANTICS || NH3API_VS2012_2013
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

#ifndef NH3API_STD_MOVE_SEMANTICS
    #define NH3API_STD_MOVE_SEMANTICS (NH3API_VS2010 || NH3API_CHECK_CPP11)
#endif // NH3API_STD_MOVE_SEMANTICS

namespace nh3api
{
struct dummy_tag_t
{}
// no-op constructor tag, used to optimize low-level manipulations for objects
const dummy_tag;
// example:
// exe_string get_string() {
// exe_string result(nh3api::dummy_tag);
// (void) FASTCALL_1(0x404040, exe_string*, &result);
// return result; }
// without dummy_tag, string will default-initialize its fields to zero
// which is useless because it is overriden in another function anyway
// with dummy_tag, no default initialization occurs

struct omit_base_vftable_tag_t
{}
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

#ifndef NH3API_DELEGATE_DUMMY
    #if NH3API_STD_DELEGATING_CONSTRUCTORS
        #define NH3API_DELEGATE_DUMMY(CLASS_NAME) : CLASS_NAME(nh3api::dummy_tag)
    #else
        #define NH3API_DELEGATE_DUMMY(CLASS_NAME)
    #endif
#endif

#ifndef NH3API_DELEGATE_DUMMY_COMMA
    #if NH3API_STD_DELEGATING_CONSTRUCTORS
        #define NH3API_DELEGATE_DUMMY_COMMA(CLASS_NAME) ,CLASS_NAME(nh3api::dummy_tag)
    #else
        #define NH3API_DELEGATE_DUMMY_COMMA(CLASS_NAME)
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

#ifndef NH3API_STD_ARRAY
    // std::array<T, N> support
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

// check if compiler has builtins (MSVC since 2015)
#ifndef NH3API_HAS_BUILTINS
    #ifdef __has_builtin
        #define NH3API_HAS_BUILTINS (1)
    #else
        #define NH3API_HAS_BUILTINS (0)
    #endif
#endif

#ifndef NH3API_HAS_CHECK_INCLUDE
    #ifdef __has_include
        #define NH3API_HAS_CHECK_INCLUDE (1)
    #else
        #define NH3API_HAS_CHECK_INCLUDE (0)
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_ADDRESSOF
    #if NH3API_HAS_BUILTINS
        #if __has_builtin(__builtin_addressof)
            #define NH3API_HAS_BUILTIN_ADDRESSOF (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_ADDRESSOF
    #define NH3API_HAS_BUILTIN_ADDRESSOF (0)
#endif

#ifndef NH3API_HAS_BUILTIN_CONSTANT_P
    #if NH3API_HAS_BUILTINS
        #if __has_builtin(__builtin_constant_p)
            #define NH3API_HAS_BUILTIN_CONSTANT_P (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_CONSTANT_P
    #define NH3API_HAS_BUILTIN_CONSTANT_P (0)
#endif

#ifndef NH3API_HAS_BUILTIN_BIT_CAST
    #if NH3API_HAS_BUILTINS
        #if __has_builtin(__builtin_bit_cast)
            #define NH3API_HAS_BUILTIN_BIT_CAST (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_BIT_CAST
    #define NH3API_HAS_BUILTIN_BIT_CAST (0)
#endif

#ifndef NH3API_HAS_BUILTIN_STRLEN
    #ifdef __has_builtin // check if compiler has builtins (MSVC since 2015)
        #if __has_builtin(__builtin_strlen)
            #define NH3API_HAS_BUILTIN_STRLEN (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_STRLEN
    #define NH3API_HAS_BUILTIN_STRLEN (0)
#endif

#ifndef NH3API_HAS_BUILTIN_WCSLEN
    #ifdef __has_builtin // check if compiler has builtins (MSVC since 2015)
        #if __has_builtin(__builtin_wcslen)
            #define NH3API_HAS_BUILTIN_WCSLEN (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_WCSLEN
    #define NH3API_HAS_BUILTIN_WCSLEN (0)
#endif

#ifndef NH3API_HAS_BUILTIN_MEMCMP
    #ifdef __has_builtin // check if compiler has builtins (MSVC since 2015)
        #if __has_builtin(__builtin_memcmp)
            #define NH3API_HAS_BUILTIN_MEMCMP (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_MEMCMP
    #define NH3API_HAS_BUILTIN_MEMCMP (0)
#endif

#ifndef NH3API_HAS_BUILTIN_WMEMCMP
    #ifdef __has_builtin // check if compiler has builtins (MSVC since 2015)
        #if __has_builtin(__builtin_wmemcmp)
            #define NH3API_HAS_BUILTIN_WMEMCMP (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_WMEMCMP
    #define NH3API_HAS_BUILTIN_WMEMCMP (0)
#endif

#ifndef NH3API_HAS_BUILTIN_MEMCHR
    #ifdef __has_builtin // check if compiler has builtins (MSVC since 2015)
        #if __has_builtin(__builtin_memchr)
            #define NH3API_HAS_BUILTIN_MEMCHR (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_MEMCHR
    #define NH3API_HAS_BUILTIN_MEMCHR (0)
#endif

#ifndef NH3API_HAS_BUILTIN_CHAR_MEMCHR
    #ifdef __has_builtin // check if compiler has builtins (MSVC since 2015)
        #if __has_builtin(__builtin_char_memchr)
            #define NH3API_HAS_BUILTIN_CHAR_MEMCHR (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_CHAR_MEMCHR
    #define NH3API_HAS_BUILTIN_CHAR_MEMCHR (0)
#endif

#ifndef NH3API_HAS_BUILTIN_WMEMCHR
    #ifdef __has_builtin // check if compiler has builtins (MSVC since 2015)
        #if __has_builtin(__builtin_wmemchr)
            #define NH3API_HAS_BUILTIN_WMEMCHR (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_WMEMCHR
    #define NH3API_HAS_BUILTIN_WMEMCHR (0)
#endif

#ifndef NH3API_HAS_BUILTIN_MEMCPY
    #ifdef __has_builtin // check if compiler has builtins (MSVC since 2015)
        #if __has_builtin(__builtin_memcpy)
            #define NH3API_HAS_BUILTIN_MEMCPY (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_MEMCPY
    #define NH3API_HAS_BUILTIN_MEMCPY (0)
#endif

#ifndef NH3API_HAS_BUILTIN_MEMMOVE
    #ifdef __has_builtin // check if compiler has builtins (MSVC since 2015)
        #if __has_builtin(__builtin_memmove)
            #define NH3API_HAS_BUILTIN_MEMMOVE (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_MEMMOVE
    #define NH3API_HAS_BUILTIN_MEMMOVE (0)
#endif

#ifndef NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED
    #if NH3API_HAS_BUILTINS
        #if __has_builtin(__builtin_is_constant_evaluated)
            #define NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED (1)
        #endif
    #endif
#endif

#ifndef NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED
    #define NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED (0)
#endif

#ifndef NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
    #if NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED
        #define NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED constexpr
    #else 
        #define NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
    #endif
#endif

#ifndef NH3API_HAS_IS_CONSTANT_EVALUATED
    #if NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED
        #define NH3API_HAS_IS_CONSTANT_EVALUATED (1)
    #elif NH3API_CHECK_MSVC && NH3API_STD_RELAXED_CONSTEXPR
        #define NH3API_HAS_IS_CONSTANT_EVALUATED (1) // hacky soultion for VS 2017
    #else
        #define NH3API_HAS_IS_CONSTANT_EVALUATED (0)
    #endif
#endif

// noreturn attribute
#if !defined(NH3API_NORETURN)
    #if NH3API_CHECK_CPP11
        #define NH3API_NORETURN [[noreturn]]
    #else // old MSVC
        #define NH3API_NORETURN __declspec(noreturn)
    #endif
#endif

// use the C++11's default template function argument feature to implement SFINAE via std::enable_if
#ifndef NH3API_SFINAE_BEGIN
    #if NH3API_STD_MOVE_SEMANTICS
        #define NH3API_SFINAE_BEGIN(...) ,typename std::enable_if<__VA_ARGS__, bool>::type = false
    #else
        #define NH3API_SFINAE_BEGIN(...)
    #endif
#endif

// as for the C++98 mode, use function argument to implement SFINAE via std::enable_if
#ifndef NH3API_SFINAE_END
    #if !NH3API_STD_MOVE_SEMANTICS
        #define NH3API_SFINAE_END(...) ,typename nh3api::tt::enable_if<__VA_ARGS__, void>::type* = 0
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
// - 'NH3API_FLAG_NOTHROW_NEW':
// operator new (exe_heap) will not throw std::bad_alloc and check for nullptr
// operator new[] (exe_heap) will not throw std::bad_array_new_length and check for nullptr
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
// - 'NH3API_FLAG_STD_CONTAINERS_RELATIONS'
// provide support of NH3API's exe_ containers
// for STL implementations
// for example, exe_vector from std::vector and so on.

// indicate function or variable inline
// use NH3API_FORCEINLINE for functions code inlining as
// this macro is for inline variables too
// which aren't available before C++17
#ifndef NH3API_INLINE
    #ifdef NH3API_FLAG_INLINE_HEADERS
        #if NH3API_STD_INLINE_VARIABLES
            #define NH3API_INLINE inline
        #else
            #error 'NH3API_FLAG_INLINE_HEADERS' requires C++17
        #endif
    #else
        #define NH3API_INLINE
    #endif
#endif

#ifndef NH3API_INLINE_OR_EXTERN
    #ifdef NH3API_FLAG_INLINE_HEADERS
        #define NH3API_INLINE_OR_EXTERN inline
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

#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    #if NH3API_CHECK_MINGW_CLANG
        #ifndef __EXCEPTIONS
            #define NH3API_FLAG_NO_CPP_EXCEPTIONS
        #endif
    #endif
    #if NH3API_CHECK_MSVC
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

#ifndef NH3API_NOEXCEPT_ALLOC
    #define NH3API_NOEXCEPT_ALLOC NH3API_NOEXCEPT_EXPR(nh3api::tt::allocator_may_throw<allocator_type>::value)
#endif

#ifndef NH3API_NOEXCEPT_DESTRUCT
    #define NH3API_NOEXCEPT_DESTRUCT NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_destructible<value_type>::value)
#endif

namespace nh3api
{
namespace flags
{
enum : unsigned char
{
    // see 'NH3API_FLAG_NO_CPP_EXCEPTIONS' flag
    no_exceptions =
    #ifdef NH3API_FLAG_NO_CPP_EXCEPTIONS
        true,
    #else
        false,
    #endif

};

}

}

#ifndef NH3API_THROW
    #ifdef NH3API_FLAG_NO_CPP_EXCEPTIONS
        // Raise C++ exception via the WinAPI, no compiler specific exception handling
        #define NH3API_THROW(EXCEPTION) RaiseException(0xE06D7363u, EXCEPTION_NONCONTINUABLE, 0, nullptr)
    #else
        #define NH3API_THROW(EXCEPTION) throw EXCEPTION
    #endif // NH3API_FLAG_NO_CPP_EXCEPTIONS
#endif // NH3API_THROW

#ifndef NH3API_UNREACHABLE
    #if NH3API_CHECK_MSVC
        #define NH3API_UNREACHABLE() __assume(false)
    #else
        #define NH3API_UNREACHABLE() __builtin_unreachable()
    #endif
#endif

#ifndef NH3API_ASSUME
    #if NH3API_CHECK_MSVC
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

// Debug expression macro
#if NH3API_DEBUG
    #define NH3API_DEBUG_SWITCH(debug_expr, release_expr) (release_expr)
#else // NH3API_DEBUG
    #define NH3API_DEBUG_SWITCH(debug_expr, release_expr) (debug_expr)
#endif // NH3API_DEBUG


NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)

// include as less windows stuff as possible to improve compilation time
#if NH3API_HAS_CHECK_INCLUDE
    #if __has_include(<debugapi.h>)
        #include <debugapi.h>
    #else
        #include <winbase.h>
    #endif
#else
    #include <winbase.h>
#endif

// https://en.cppreference.com/w/cpp/header/debugging

// int 3 breakpoint
inline void breakpoint() NH3API_NOEXCEPT
{
#if NH3API_CHECK_MSVC || NH3API_CHECK_CLANG_CL
    __debugbreak();
#else
    __asm__ __volatile__("int {$}3" ::: "memory");
#endif
}
inline bool is_debugger_present() NH3API_NOEXCEPT
{ return IsDebuggerPresent() != 0; }
inline void breakpoint_if_debugging() NH3API_NOEXCEPT
{
    if ( is_debugger_present() )
        breakpoint();
}

NH3API_DISABLE_WARNING_END

#ifndef NH3API_SIZE_TEST
    #define NH3API_SIZE_TEST(EXPECTED_SIZE, ...) NH3API_STATIC_ASSERT("size mismatch", sizeof(__VA_ARGS__) == EXPECTED_SIZE)
#endif


#ifndef NH3API_IGNORE
    #if NH3API_CHECK_MSVC
        #define NH3API_IGNORE(...) __noop(__VA_ARGS__)
    #else
        //imlementation by https://ds.me/

        namespace nh3api
        {
        inline void ignore() { }
        template <typename T0, typename... T>
        void ignore(const T0& args, const T&... further_args) { (void)args; ignore(further_args...); }
        }

        #define NH3API_IGNORE(...) (void)([&](){nh3api::ignore(__VA_ARGS__);})
    #endif
#endif

#ifndef NH3API_MAJOR_VERSION
    #define NH3API_MAJOR_VERSION 1
    #define NH3API_MINOR_VERSION 0
    #define NH3API_VERSION_STRING "NH3API v1.0"
#endif

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