//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// This file contains definitions used by the Hex-Rays decompiler output.
// It has type definitions and convenience macros to make the
// output more readable.
// Copyright (c) 2007 Hex-Rays

#pragma once

#include <intrin.h>
#include "type_traits.hpp"

// Partially defined types. They are used when the decompiler does not know
// anything about the type except its size.

// 8 bits
typedef uint8_t  _BYTE;
// 16 bits
typedef uint16_t _WORD;
// 32 bits
typedef uint32_t _DWORD;
// 64 bits
typedef uint64_t _QWORD;
// 128 bits SSE2
typedef __m128i  _OWORD;

// Non-standard boolean types. They are used when the decompiler can not use
// the standard "bool" type because of the size mistmatch but the possible
// values are only 0 and 1. See also 'BOOL' type below.

// 8 bits
typedef int8_t _BOOL1;
// 16 bits
typedef int16_t _BOOL2;
typedef int32_t _BOOL4;

#ifndef __pure
    #if NH3API_CHECK_MSVC_DRIVER
        #define __pure  __declspec(noalias) // not quite the same but similiar
    #else
        #define __pure  __attribute__((__pure__))
    #endif
#endif

#ifndef __noreturn
    #if NH3API_CHECK_MSVC_DRIVER
        #define __noreturn  __declspec(noreturn)
    #else
        #define __noreturn  __attribute__((__noreturn__))
    #endif
#endif

#ifndef __cppobj
    #define __cppobj
#endif

#ifndef LAST_IND
    // Some convenience macros to make partial accesses nicer
    #define LAST_IND(x,part_type)  (sizeof(x)/sizeof(part_type) - 1)
    #define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
    #define LOW_IND(x,part_type)   0
#endif

#ifndef BYTEn
    // first unsigned macros:
    #define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
    #define WORDn(x, n)   (*((_WORD*)&(x)+n))
    #define DWORDn(x, n)  (*((_DWORD*)&(x)+n))
#endif

#ifndef LOBYTE
    #define LOBYTE(x)  BYTEn(x,LOW_IND(x,_BYTE))
#endif
#ifndef LOWORD
    #define LOWORD(x)  WORDn(x,LOW_IND(x,_WORD))
#endif
#ifndef LODWORD
#define LODWORD(x) DWORDn(x,LOW_IND(x,_DWORD))
#endif
#ifndef HIBYTE
    #define HIBYTE(x)  BYTEn(x,HIGH_IND(x,_BYTE))
#endif
#ifndef HIWORD
    #define HIWORD(x)  WORDn(x,HIGH_IND(x,_WORD))
#endif
#ifndef HIDWORD
    #define HIDWORD(x) DWORDn(x,HIGH_IND(x,_DWORD))
#endif

#ifndef BYTE1
    #define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
    #define BYTE2(x)   BYTEn(x,  2)
    #define BYTE3(x)   BYTEn(x,  3)
    #define BYTE4(x)   BYTEn(x,  4)
    #define BYTE5(x)   BYTEn(x,  5)
    #define BYTE6(x)   BYTEn(x,  6)
    #define BYTE7(x)   BYTEn(x,  7)
    #define BYTE8(x)   BYTEn(x,  8)
    #define BYTE9(x)   BYTEn(x,  9)
    #define BYTE10(x)  BYTEn(x, 10)
    #define BYTE11(x)  BYTEn(x, 11)
    #define BYTE12(x)  BYTEn(x, 12)
    #define BYTE13(x)  BYTEn(x, 13)
    #define BYTE14(x)  BYTEn(x, 14)
    #define BYTE15(x)  BYTEn(x, 15)
#endif
#ifndef WORD1
    #define WORD1(x)   WORDn(x,  1)
    #define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
    #define WORD3(x)   WORDn(x,  3)
    #define WORD4(x)   WORDn(x,  4)
    #define WORD5(x)   WORDn(x,  5)
    #define WORD6(x)   WORDn(x,  6)
    #define WORD7(x)   WORDn(x,  7)
#endif

#ifndef SBYTEn
    // now signed macros (the same but with sign extension)
    #define SBYTEn(x, n)   (*((int8_t*)&(x)+n))
    #define SWORDn(x, n)   (*((int16_t*)&(x)+n))
    #define SDWORDn(x, n)  (*((int32_t*)&(x)+n))
#endif

#ifndef SLOBYTE
    #define SLOBYTE(x)  SBYTEn(x,LOW_IND(x,int8_t))
    #define SLOWORD(x)  SWORDn(x,LOW_IND(x,int16_t))
    #define SLODWORD(x) SDWORDn(x,LOW_IND(x,int32_t))
    #define SHIBYTE(x)  SBYTEn(x,HIGH_IND(x,int8_t))
    #define SHIWORD(x)  SWORDn(x,HIGH_IND(x,int16_t))
    #define SHIDWORD(x) SDWORDn(x,HIGH_IND(x,int32_t))
    #define SBYTE1(x)   SBYTEn(x,  1)
    #define SBYTE2(x)   SBYTEn(x,  2)
    #define SBYTE3(x)   SBYTEn(x,  3)
    #define SBYTE4(x)   SBYTEn(x,  4)
    #define SBYTE5(x)   SBYTEn(x,  5)
    #define SBYTE6(x)   SBYTEn(x,  6)
    #define SBYTE7(x)   SBYTEn(x,  7)
    #define SBYTE8(x)   SBYTEn(x,  8)
    #define SBYTE9(x)   SBYTEn(x,  9)
    #define SBYTE10(x)  SBYTEn(x, 10)
    #define SBYTE11(x)  SBYTEn(x, 11)
    #define SBYTE12(x)  SBYTEn(x, 12)
    #define SBYTE13(x)  SBYTEn(x, 13)
    #define SBYTE14(x)  SBYTEn(x, 14)
    #define SBYTE15(x)  SBYTEn(x, 15)
    #define SWORD1(x)   SWORDn(x,  1)
    #define SWORD2(x)   SWORDn(x,  2)
    #define SWORD3(x)   SWORDn(x,  3)
    #define SWORD4(x)   SWORDn(x,  4)
    #define SWORD5(x)   SWORDn(x,  5)
    #define SWORD6(x)   SWORDn(x,  6)
    #define SWORD7(x)   SWORDn(x,  7)
#endif

#ifndef NH3API_IDA_INTRIN
    #define NH3API_IDA_INTRIN NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE __pure
#endif

template<typename T>
NH3API_IDA_INTRIN
bool is_mul_ok(T count, T elsize) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_unsigned<T>::value);
    if ( elsize  == 0 || count == 0 )
        return true;
    return count <= ((T)(-1)) / elsize;
}

// multiplication that saturates (yields the biggest value) instead of overflowing
// such a construct is useful in "operator new[]"
template<class T>
NH3API_IDA_INTRIN
bool saturated_mul(T count, T elsize) NH3API_NOEXCEPT
{ return is_mul_ok(count, elsize) ? count * elsize : T(-1); }

#if NH3API_CHECK_MSVC
#pragma intrinsic(__movsb)
#endif

// memcpy() with determined behavoir: it always copies
// from the start to the end of the buffer
// note: it copies byte by byte, so it is not equivalent to, for example, rep movsd
NH3API_FORCEINLINE void* qmemcpy(void* dst, const void* src, size_t count) NH3API_NOEXCEPT
{
    __movsb(static_cast<unsigned char*>(dst), static_cast<const unsigned char*>(src), count);
    return dst;
}

#if NH3API_CHECK_MSVC
#pragma intrinsic(__stosd)
#endif

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)

NH3API_FORCEINLINE void memset32(unsigned long* dst, const unsigned long value, size_t count) NH3API_NOEXCEPT
{ __stosd(dst, value, count); }

NH3API_FORCEINLINE void memset_0(void* ptr, size_t size) NH3API_NOEXCEPT
{ std::memset(ptr, 0, size); }

template<typename T>
NH3API_FORCEINLINE void memset_0(T* ptr) NH3API_NOEXCEPT
{ std::memset(ptr, 0, sizeof(T)); }

NH3API_DISABLE_WARNING_END

#define CHECK_T_CASSERT NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value)

// Generate a reference to pair of operands
template<class T> NH3API_IDA_INTRIN int16_t __PAIR__ ( int8_t  high, T low ) NH3API_NOEXCEPT { CHECK_T_CASSERT; return (( static_cast<int16_t>(high)) << sizeof(high)*8) | static_cast<int16_t>(low);  }
template<class T> NH3API_IDA_INTRIN int32_t __PAIR__ ( int16_t high, T low ) NH3API_NOEXCEPT { CHECK_T_CASSERT; return (( static_cast<int32_t>(high)) << sizeof(high)*8) | static_cast<int32_t>(low);  }
template<class T> NH3API_IDA_INTRIN int64_t __PAIR__ ( int32_t high, T low ) NH3API_NOEXCEPT { CHECK_T_CASSERT; return (( static_cast<int64_t>(high)) << sizeof(high)*8) | static_cast<int64_t>(low);  }
template<class T> NH3API_IDA_INTRIN uint16_t __PAIR__(uint8_t  high, T low ) NH3API_NOEXCEPT { CHECK_T_CASSERT; return ((static_cast<uint16_t>(high)) << sizeof(high)*8) | static_cast<uint16_t>(low); }
template<class T> NH3API_IDA_INTRIN uint32_t __PAIR__(uint16_t high, T low ) NH3API_NOEXCEPT { CHECK_T_CASSERT; return ((static_cast<uint32_t>(high)) << sizeof(high)*8) | static_cast<uint32_t>(low); }
template<class T> NH3API_IDA_INTRIN uint64_t __PAIR__(uint32_t high, T low ) NH3API_NOEXCEPT { CHECK_T_CASSERT; return ((static_cast<uint64_t>(high)) << sizeof(high)*8) | static_cast<uint64_t>(low); }

NH3API_IDA_INTRIN uint16_t __PAIR16__ (uint8_t  high, uint8_t  low) NH3API_NOEXCEPT { return static_cast<uint16_t>((( static_cast<uint16_t>(high)) << UINT16_C(8))  | static_cast<uint16_t>(low)); }
NH3API_IDA_INTRIN uint32_t __PAIR32__ (uint16_t high, uint16_t low) NH3API_NOEXCEPT { return (( static_cast<uint32_t>(high)) << UINT32_C(16)) | static_cast<uint32_t>(low); }
NH3API_IDA_INTRIN uint64_t __PAIR64__ (uint32_t high, uint32_t low) NH3API_NOEXCEPT { return (( static_cast<uint64_t>(high)) << UINT64_C(32)) | static_cast<uint64_t>(low); }
NH3API_IDA_INTRIN int16_t  __SPAIR16__(int8_t   high, int8_t   low) NH3API_NOEXCEPT { return static_cast<int16_t>((( static_cast<int16_t>(high))  << INT16_C(8))   | static_cast<int16_t>(low)); }
NH3API_IDA_INTRIN int32_t  __SPAIR32__(int16_t  high, int16_t  low) NH3API_NOEXCEPT { return (( static_cast<int32_t>(high))  << INT32_C(16))  | static_cast<int32_t>(low); }
NH3API_IDA_INTRIN int64_t  __SPAIR64__(int32_t  high, int32_t  low) NH3API_NOEXCEPT { return (( static_cast<int64_t>(high))  << INT64_C(32))  | static_cast<int64_t>(low); }

#undef CHECK_T_CASSERT

#if NH3API_HAS_BUILTIN(__builtin_rotateleft8) && NH3API_HAS_BUILTIN(__builtin_rotateright8)

NH3API_IDA_INTRIN uint8_t  __ROL1__(uint8_t  value, uint8_t count)  NH3API_NOEXCEPT { return __builtin_rotateleft8(value, count); }
NH3API_IDA_INTRIN uint16_t __ROL2__(uint16_t value, uint16_t count) NH3API_NOEXCEPT { return __builtin_rotateleft16(value, count); }
NH3API_IDA_INTRIN uint32_t __ROL4__(uint32_t value, uint32_t count) NH3API_NOEXCEPT { return __builtin_rotateleft32(value, count); }
NH3API_IDA_INTRIN uint64_t __ROL8__(uint64_t value, uint64_t count) NH3API_NOEXCEPT { return __builtin_rotateleft64(value, count); }
NH3API_IDA_INTRIN uint8_t  __ROR1__(uint8_t  value, uint8_t count)  NH3API_NOEXCEPT { return __builtin_rotateright8(value, count); }
NH3API_IDA_INTRIN uint16_t __ROR2__(uint16_t value, uint16_t count) NH3API_NOEXCEPT { return __builtin_rotateright16(value, count); }
NH3API_IDA_INTRIN uint32_t __ROR4__(uint32_t value, uint32_t count) NH3API_NOEXCEPT { return __builtin_rotateright32(value, count); }
NH3API_IDA_INTRIN uint64_t __ROR8__(uint64_t value, uint64_t count) NH3API_NOEXCEPT { return __builtin_rotateright64(value, count); }

#else

// rotate left
template<class T> NH3API_IDA_INTRIN
T __ROL__(T value, size_t count) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value);
    const uint32_t nbits = sizeof(T) * 8;

    count %= nbits;
    T high = value >> (nbits - count);
    NH3API_IF_CONSTEXPR ( nh3api::tt::is_signed<T>::value )
        high &= ~((T(-1) << count));
    value <<= count;
    value |= high;

    return value;
}

// rotate right
template<class T> NH3API_IDA_INTRIN
T __ROR__(T value, size_t count) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value);
    const uint32_t nbits = sizeof(T) * 8;
    count %= nbits;
    T low = value << (nbits - count);
    value >>= count;
    value |= low;
    return value;
}

NH3API_IDA_INTRIN uint8_t  __ROL1__(uint8_t  value, size_t count) NH3API_NOEXCEPT { return __ROL__<uint8_t>(static_cast<uint8_t> (value),  count); }
NH3API_IDA_INTRIN uint16_t __ROL2__(uint16_t value, size_t count) NH3API_NOEXCEPT { return __ROL__<uint16_t>(static_cast<uint16_t>(value),  count); }
NH3API_IDA_INTRIN uint32_t __ROL4__(uint32_t value, size_t count) NH3API_NOEXCEPT { return __ROL__<uint32_t>(static_cast<uint32_t>(value),  count); }
NH3API_IDA_INTRIN uint64_t __ROL8__(uint64_t value, size_t count) NH3API_NOEXCEPT { return __ROL__<uint64_t>(static_cast<uint64_t>(value),  count); }
NH3API_IDA_INTRIN uint8_t  __ROR1__(uint8_t  value, size_t count) NH3API_NOEXCEPT { return __ROR__<uint8_t>(static_cast<uint8_t> (value), count); }
NH3API_IDA_INTRIN uint16_t __ROR2__(uint16_t value, size_t count) NH3API_NOEXCEPT { return __ROR__<uint16_t>(static_cast<uint16_t>(value), count); }
NH3API_IDA_INTRIN uint32_t __ROR4__(uint32_t value, size_t count) NH3API_NOEXCEPT { return __ROR__<uint32_t>(static_cast<uint32_t>(value), count); }
NH3API_IDA_INTRIN uint64_t __ROR8__(uint64_t value, size_t count) NH3API_NOEXCEPT { return __ROR__<uint64_t>(static_cast<uint64_t>(value), count); }

#endif

// carry flag of left shift
template<class T> NH3API_IDA_INTRIN
int8_t __MKCSHL__(T value, uint32_t count) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value);
    const uint32_t nbits = sizeof(T) * 8;
    count %= nbits;
    return (value >> (nbits-count)) & 1;
}

// carry flag of right shift
template<class T> NH3API_IDA_INTRIN
int8_t __MKCSHR__(T value, uint32_t count) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value);
    return (value >> (count-1)) & 1;
}

// sign flag
template<class T> NH3API_IDA_INTRIN
int8_t __SETS__(T x) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value);
    switch ( sizeof(T) )
    {
        case 1:
            return static_cast<int8_t>(x) < 0;
            break;
        case 2:
            return static_cast<int16_t>(x) < 0;
            break;
        case 4:
            return static_cast<int32_t>(x) < 0;
            break;
        case 8:
            return static_cast<int64_t>(x) < 0;
            break;
        default:
            return 0;
            break;
    }
}

// overflow flag of subtraction (x-y)
template<class T, class U> NH3API_IDA_INTRIN
int8_t __OFSUB__(T x, U y) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value && nh3api::tt::is_integral<U>::value);
    NH3API_IF_CONSTEXPR ( sizeof(T) < sizeof(U) )
    {
        U x2 = x;
        int8_t sx = __SETS__(x2);
        return (sx ^ __SETS__(y)) & (sx ^ __SETS__(x2-y));
    }
    else
    {
        T y2 = y;
        int8_t sx = __SETS__(x);
        return (sx ^ __SETS__(y2)) & (sx ^ __SETS__(x-y2));
    }
}

// overflow flag of addition (x+y)
template<class T, class U> NH3API_IDA_INTRIN
int8_t __OFADD__(T x, U y) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value && nh3api::tt::is_integral<U>::value);
    NH3API_IF_CONSTEXPR ( sizeof(T) < sizeof(U) )
    {
        U x2 = x;
        int8_t sx = __SETS__(x2);
        return ((1 ^ sx) ^ __SETS__(y)) & (sx ^ __SETS__(x2+y));
    }
    else
    {
        T y2 = y;
        int8_t sx = __SETS__(x);
        return ((1 ^ sx) ^ __SETS__(y2)) & (sx ^ __SETS__(x+y2));
    }
}

// carry flag of subtraction (x-y)
template<class T, class U> NH3API_IDA_INTRIN
int8_t __CFSUB__(T x, U y) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value && nh3api::tt::is_integral<U>::value);
    const size_t size = sizeof(T) > sizeof(U) ? sizeof(T) : sizeof(U);
    switch ( size )
    {
        case 1:
            return static_cast<uint8_t>(x) < static_cast<uint8_t>(y);
            break;
        case 2:
            return static_cast<uint16_t>(x) < static_cast<uint16_t>(y);
            break;
        case 4:
            return static_cast<uint32_t>(x) < static_cast<uint32_t>(y);
            break;
        case 8:
            return static_cast<uint64_t>(x) < static_cast<uint64_t>(y);
            break;
        default:
            return 0;
            break;
    }
}

// carry flag of addition (x+y)
template<class T, class U> NH3API_IDA_INTRIN
int8_t __CFADD__(T x, U y) NH3API_NOEXCEPT
{
    NH3API_STATIC_ASSERT("", nh3api::tt::is_integral<T>::value && nh3api::tt::is_integral<U>::value);
    const size_t size = sizeof(T) > sizeof(U) ? sizeof(T) : sizeof(U);
    switch (size)
    {
        case 1:
            return static_cast<uint8_t>(x) > static_cast<uint8_t>(x+y);
            break;
        case 2:
            return static_cast<uint16_t>(x) > static_cast<uint16_t>(x+y);
            break;
        case 4:
            return static_cast<uint32_t>(x) > static_cast<uint32_t>(x+y);
            break;
        case 8:
            return static_cast<uint64_t>(x) > static_cast<uint64_t>(x+y);
            break;
        default:
            return 0;
            break;
    }

}

NH3API_IDA_INTRIN int8_t  abs8 (int8_t  x) NH3API_NOEXCEPT { return x >= 0 ? x : static_cast<int8_t>(-x); }
NH3API_IDA_INTRIN int16_t abs16(int16_t x) NH3API_NOEXCEPT { return x >= 0 ? x : static_cast<int16_t>(-x); }
NH3API_IDA_INTRIN int32_t abs32(int32_t x) NH3API_NOEXCEPT { return x >= 0 ? x : -x; }
NH3API_IDA_INTRIN int64_t abs64(int64_t x) NH3API_NOEXCEPT { return x >= 0 ? x : -x; }

#ifndef COERCE_FLOAT
    #define COERCE_FLOAT(v)          ::nh3api::bit_cast<float>(v)
    #define COERCE_DOUBLE(v)         ::nh3api::bit_cast<double>(v)
    #define COERCE_LONG_DOUBLE(v)    ::nh3api::bit_cast<long double>(v)
    #define COERCE_UNSIGNED_INT(v)   ::nh3api::bit_cast<unsigned int>(v)
    #define COERCE_UNSIGNED_INT64(v) ::nh3api::bit_cast<uint64_t>(v)
#endif

#ifndef CONTAINING_RECORD
#define CONTAINING_RECORD(address, type, field) ((type *)((_BYTE*)(address) - offsetof(type, field)))
#endif
