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

#include <cstdint>
#include <cstring>     // std::memcpy, std::memset
#include <limits>      // std::numeric_limits
#include <type_traits> // std::common_type_t, std::enable_if_t, std::make_unsigned_t
#if (__cplusplus >= 202002L) || defined(__cpp_lib_concepts)
#include <concepts>    // std::integral
#endif

#include "nh3api_std.hpp"

// Partially defined types. They are used when the decompiler does not know
// anything about the type except its size.

// 8 bits
using _BYTE = uint8_t;
// 16 bits
using _WORD = uint16_t;
// 32 bits
using _DWORD = uint32_t;
// 64 bits
using _QWORD = uint64_t;

// Non-standard boolean types. They are used when the decompiler can not use
// the standard "bool" type because of the size mistmatch but the possible
// values are only 0 and 1. See also 'BOOL' type below.

// 8 bits bool
using _BOOL1 = uint8_t;
// 16 bits bool
using _BOOL2 = uint16_t;
// 32 bits bool
using _BOOL4 = uint32_t;

#ifndef __noreturn
    #if NH3API_CHECK_MSVC_DRIVER
        #define __noreturn __declspec(noreturn)
    #else
        #define __noreturn __attribute__((__noreturn__))
    #endif
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
    #define SBYTEn(x, n)   (*((int8_t*)&(x)+(n)))
    #define SWORDn(x, n)   (*((int16_t*)&(x)+(n)))
    #define SDWORDn(x, n)  (*((int32_t*)&(x)+(n)))
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
    #if NH3API_CHECK_MSVC
        #define NH3API_IDA_INTRIN inline constexpr __declspec(noalias)
    #else
        #define NH3API_IDA_INTRIN [[__gnu__::__pure__]] inline constexpr
    #endif
#endif

#ifndef NH3API_IDA_INTEGRAL_TEMPLATE
    #ifdef __cpp_lib_concepts
        #define NH3API_IDA_INTEGRAL_TEMPLATE(T) std::integral T
        #define NH3API_IDA_INTEGRAL_TEMPLATE_2(T, U) std::integral T, std::integral U
    #else
        #define NH3API_IDA_INTEGRAL_TEMPLATE(T) typename T,std::enable_if_t<std::is_integral_v<T>,bool> =false
        #define NH3API_IDA_INTEGRAL_TEMPLATE_2(T, U) typename T, typename U, std::enable_if_t<std::is_integral_v<T> && std::is_integral_v<U>,bool> =false
    #endif
#endif

template<NH3API_IDA_INTEGRAL_TEMPLATE(T)>
NH3API_IDA_INTRIN bool is_mul_ok(T count, T elsize) noexcept
{
    if ( elsize == 0 || count == 0 )
        return true;

    #if NH3API_HAS_BUILTIN(__builtin_mul_overflow)
    T result;
    return !__builtin_mul_overflow(count, elsize, &result);
    #else
    return count <= (std::numeric_limits<T>::max)() / elsize;
    #endif
}

// multiplication that saturates (yields the biggest value) instead of overflowing
// such a construct is useful in "operator new[]"
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)>
NH3API_IDA_INTRIN T saturated_mul(T count, T elsize) noexcept
{
    #if NH3API_HAS_BUILTIN(__builtin_mul_overflow)
    T result;
    if ( __builtin_mul_overflow(count, elsize, &result) )
        return (std::numeric_limits<T>::max)();

    return result;
    #else
    return is_mul_ok(count, elsize) ? count * elsize : (std::numeric_limits<T>::max)();
    #endif
}

#if NH3API_CHECK_MSVC
void __movsb(unsigned char*, unsigned char const*, size_t);
#endif

// memcpy() with determined behavior: it always copies
// from the start to the end of the buffer
// note: it copies byte by byte, so it is not equivalent to, for example, rep movsd
NH3API_FORCEINLINE void* qmemcpy(void* dst, const void* src, size_t count) noexcept
{
    #if NH3API_CHECK_MSVC
    __movsb(static_cast<unsigned char*>(dst), static_cast<const unsigned char*>(src), count);
    #else
    __asm__ __volatile__("xchg {%%esi, %1|%1, esi}\n"
                         "rep movsb\n"
                         "xchg {%%esi, %1|%1, esi}"
                         : "+D"(dst), "+r"(src), "+c"(count)
                         :
                         : "memory");
    #endif
    return dst;
}

#if NH3API_CHECK_MSVC
void __stosd(unsigned long*, unsigned long, size_t);
#endif

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wattributes")
NH3API_WARNING_MSVC_DISABLE(4714)

NH3API_FORCEINLINE void memset32(void* dst, unsigned long value, size_t count) noexcept
{
    #if NH3API_CHECK_MSVC
    __stosd(reinterpret_cast<unsigned long*>(dst), static_cast<unsigned long>(value), count);
    #else
    __asm__ __volatile__("rep stos{l|d}"
                       : "+D"(dst), "+c"(count)
                       : "a"(value)
                       : "memory");
    #endif
}

inline void memset_0(void* ptr, size_t size) noexcept
{ std::memset(ptr, 0, size); }

template<typename T>
inline void memset_0(T* ptr) noexcept
{ std::memset(ptr, 0, sizeof(T)); }

NH3API_WARNING(pop)

// Generate a reference to pair of operands
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)>
NH3API_IDA_INTRIN int16_t __PAIR__(int8_t  high, T low) noexcept
{ return ((static_cast<int16_t>(high)) << static_cast<int16_t>(sizeof(high) * 8U)) | static_cast<int16_t>(low); }

template<NH3API_IDA_INTEGRAL_TEMPLATE(T)>
NH3API_IDA_INTRIN int32_t __PAIR__(int16_t high, T low) noexcept
{ return ((static_cast<int32_t>(high)) << static_cast<int32_t>(sizeof(high) * 8U)) | static_cast<int32_t>(low); }

template<NH3API_IDA_INTEGRAL_TEMPLATE(T)>
NH3API_IDA_INTRIN int64_t __PAIR__(int32_t high, T low) noexcept
{ return ((static_cast<int64_t>(high)) << static_cast<int64_t>(sizeof(high) * 8U)) | static_cast<int64_t>(low); }

template<NH3API_IDA_INTEGRAL_TEMPLATE(T)>
NH3API_IDA_INTRIN uint16_t __PAIR__(uint8_t high, T low) noexcept
{ return ((static_cast<uint16_t>(high)) << static_cast<uint16_t>(sizeof(high) * 8U)) | static_cast<uint16_t>(low); }

template<NH3API_IDA_INTEGRAL_TEMPLATE(T)>
NH3API_IDA_INTRIN uint32_t __PAIR__(uint16_t high, T low) noexcept
{ return ((static_cast<uint32_t>(high)) << sizeof(high) * 8U) | static_cast<uint32_t>(low); }

template<NH3API_IDA_INTEGRAL_TEMPLATE(T)>
NH3API_IDA_INTRIN uint64_t __PAIR__(uint32_t high, T low) noexcept
{ return ((static_cast<uint64_t>(high)) << sizeof(high) * 8U) | static_cast<uint64_t>(low); }

NH3API_IDA_INTRIN uint16_t __PAIR16__ (uint8_t  high, uint8_t  low) noexcept { return static_cast<uint16_t>((( static_cast<uint16_t>(high)) << UINT16_C(8))  | static_cast<uint16_t>(low)); }
NH3API_IDA_INTRIN uint32_t __PAIR32__ (uint16_t high, uint16_t low) noexcept { return (( static_cast<uint32_t>(high)) << UINT32_C(16)) | static_cast<uint32_t>(low); }
NH3API_IDA_INTRIN uint64_t __PAIR64__ (uint32_t high, uint32_t low) noexcept { return (( static_cast<uint64_t>(high)) << UINT64_C(32)) | static_cast<uint64_t>(low); }
NH3API_IDA_INTRIN int16_t  __SPAIR16__(int8_t   high, int8_t   low) noexcept { return static_cast<int16_t>((( static_cast<int16_t>(high))  << INT16_C(8))   | static_cast<int16_t>(low)); }
NH3API_IDA_INTRIN int32_t  __SPAIR32__(int16_t  high, int16_t  low) noexcept { return (( static_cast<int32_t>(high))  << INT32_C(16))  | static_cast<int32_t>(low); }
NH3API_IDA_INTRIN int64_t  __SPAIR64__(int32_t  high, int32_t  low) noexcept { return (( static_cast<int64_t>(high))  << INT64_C(32))  | static_cast<int64_t>(low); }

#if NH3API_HAS_BUILTIN(__builtin_rotateleft8) && NH3API_HAS_BUILTIN(__builtin_rotateright8)

// rotate left
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)> NH3API_IDA_INTRIN
T __ROL__(T value, size_t count) noexcept
{
    if constexpr (sizeof(T) == 1)
        return __builtin_rotateleft8(value, count);
    else if constexpr (sizeof(T) == 2)
        return __builtin_rotateleft16(value, count);
    else if constexpr (sizeof(T) == 4)
        return __builtin_rotateleft32(value, count);
    else if constexpr (sizeof(T) == 8)
        return __builtin_rotateleft64(value, count);
#if NH3API_HAS_BUILTIN(__builtin_unreachable)
    else
        __builtin_unreachable();
#endif
}

// rotate left
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)> NH3API_IDA_INTRIN
T __ROR__(T value, size_t count) noexcept
{
    if constexpr (sizeof(T) == 1)
        return __builtin_rotateright8(value, count);
    else if constexpr (sizeof(T) == 2)
        return __builtin_rotateright16(value, count);
    else if constexpr (sizeof(T) == 4)
        return __builtin_rotateright32(value, count);
    else if constexpr (sizeof(T) == 8)
        return __builtin_rotateright64(value, count);
#if NH3API_HAS_BUILTIN(__builtin_unreachable)
    else
        __builtin_unreachable();
#endif
}

NH3API_IDA_INTRIN uint8_t  __ROL1__(uint8_t  value, uint8_t count)  noexcept { return __builtin_rotateleft8(value, count); }
NH3API_IDA_INTRIN uint16_t __ROL2__(uint16_t value, uint16_t count) noexcept { return __builtin_rotateleft16(value, count); }
NH3API_IDA_INTRIN uint32_t __ROL4__(uint32_t value, uint32_t count) noexcept { return __builtin_rotateleft32(value, count); }
NH3API_IDA_INTRIN uint64_t __ROL8__(uint64_t value, uint64_t count) noexcept { return __builtin_rotateleft64(value, count); }
NH3API_IDA_INTRIN uint8_t  __ROR1__(uint8_t  value, uint8_t count)  noexcept { return __builtin_rotateright8(value, count); }
NH3API_IDA_INTRIN uint16_t __ROR2__(uint16_t value, uint16_t count) noexcept { return __builtin_rotateright16(value, count); }
NH3API_IDA_INTRIN uint32_t __ROR4__(uint32_t value, uint32_t count) noexcept { return __builtin_rotateright32(value, count); }
NH3API_IDA_INTRIN uint64_t __ROR8__(uint64_t value, uint64_t count) noexcept { return __builtin_rotateright64(value, count); }

#else

// rotate left
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)> NH3API_IDA_INTRIN
T __ROL__(T value, size_t count) noexcept
{
    const uint32_t nbits = sizeof(T) * 8;

    count %= nbits;
    T high = value >> (nbits - count);
    if constexpr ( std::is_signed_v<T> )
        high &= ~((T(-1) << count));
    value <<= count;
    value |= high;

    return value;
}

// rotate right
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)> NH3API_IDA_INTRIN
T __ROR__(T value, size_t count) noexcept
{
    const uint32_t nbits = sizeof(T) * 8;
    count %= nbits;
    T low = value << (nbits - count);
    value >>= count;
    value |= low;
    return value;
}

NH3API_IDA_INTRIN uint8_t  __ROL1__(uint8_t  value, size_t count) noexcept { return __ROL__<uint8_t>(static_cast<uint8_t> (value),  count); }
NH3API_IDA_INTRIN uint16_t __ROL2__(uint16_t value, size_t count) noexcept { return __ROL__<uint16_t>(static_cast<uint16_t>(value),  count); }
NH3API_IDA_INTRIN uint32_t __ROL4__(uint32_t value, size_t count) noexcept { return __ROL__<uint32_t>(static_cast<uint32_t>(value),  count); }
NH3API_IDA_INTRIN uint64_t __ROL8__(uint64_t value, size_t count) noexcept { return __ROL__<uint64_t>(static_cast<uint64_t>(value),  count); }
NH3API_IDA_INTRIN uint8_t  __ROR1__(uint8_t  value, size_t count) noexcept { return __ROR__<uint8_t>(static_cast<uint8_t> (value), count); }
NH3API_IDA_INTRIN uint16_t __ROR2__(uint16_t value, size_t count) noexcept { return __ROR__<uint16_t>(static_cast<uint16_t>(value), count); }
NH3API_IDA_INTRIN uint32_t __ROR4__(uint32_t value, size_t count) noexcept { return __ROR__<uint32_t>(static_cast<uint32_t>(value), count); }
NH3API_IDA_INTRIN uint64_t __ROR8__(uint64_t value, size_t count) noexcept { return __ROR__<uint64_t>(static_cast<uint64_t>(value), count); }

#endif

// carry flag of left shift
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)> NH3API_IDA_INTRIN
int8_t __MKCSHL__(T value, uint32_t count) noexcept
{
    const uint32_t nbits = sizeof(T) * 8;
    count %= nbits;
    return (value >> (nbits-count)) & 1;
}

// carry flag of right shift
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)> NH3API_IDA_INTRIN
int8_t __MKCSHR__(T value, uint32_t count) noexcept
{ return (value >> (count-1)) & 1; }

// sign flag
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)> NH3API_IDA_INTRIN
int8_t __SETS__(T x) noexcept
{ return x < 0; }

// parity flag
template<NH3API_IDA_INTEGRAL_TEMPLATE(T)> NH3API_IDA_INTRIN
int32_t __SETP__(T x, T y)
{
#if NH3API_HAS_BUILTIN(__builtin_parityg)
        return __builtin_parityg(x - y);
#elif NH3API_HAS_BUILTIN(__builtin_parity) && NH3API_HAS_BUILTIN(__builtin_parityll)
    if constexpr (sizeof(T) == 8)
        return __builtin_parityll(x - y);
    else if constexpr (sizeof(T) == 4)
        return __builtin_parity(x - y);
    else if constexpr (sizeof(T) == 1 || sizeof(T) == 2)
        return __builtin_parity(static_cast<uint32_t>(x - y));
#if NH3API_HAS_BUILTIN(__builtin_unreachable)
    else
        __builtin_unreachable();
#endif
#else
        T diff = x - y;
        int32_t parity = 0;
        for (size_t i = 0; i < sizeof(T) * 8; ++i)
            parity ^= (diff >> i) & 1;
        return parity;
#endif
}

// overflow flag of subtraction (x-y)
template<NH3API_IDA_INTEGRAL_TEMPLATE_2(T, U)> NH3API_IDA_INTRIN
int8_t __OFSUB__(T x, U y) noexcept
{
#if NH3API_HAS_BUILTIN(__builtin_sub_overflow)
    using common_type = std::common_type_t<T, U>;
    common_type result;
    return __builtin_sub_overflow(static_cast<common_type>(x), static_cast<common_type>(y), &result);
#else
    if constexpr ( sizeof(T) < sizeof(U) )
    {
        const U x2 = x;
        const int8_t sx = __SETS__(x2);
        return (sx ^ __SETS__(y)) & (sx ^ __SETS__(x2-y));
    }
    else
    {
        const T y2 = y;
        const int8_t sx = __SETS__(x);
        return (sx ^ __SETS__(y2)) & (sx ^ __SETS__(x-y2));
    }
#endif
}

// overflow flag of addition (x+y)
template<NH3API_IDA_INTEGRAL_TEMPLATE_2(T, U)> NH3API_IDA_INTRIN
int8_t __OFADD__(T x, U y) noexcept
{
#if NH3API_HAS_BUILTIN(__builtin_add_overflow)
    using common_type = std::common_type_t<T, U>;
    common_type result;
    return __builtin_add_overflow(static_cast<common_type>(x), static_cast<common_type>(y), &result);
#else
    if constexpr ( sizeof(T) < sizeof(U) )
    {
        const U x2 = x;
        const int8_t sx = __SETS__(x2);
        return ((1 ^ sx) ^ __SETS__(y)) & (sx ^ __SETS__(x2+y));
    }
    else
    {
        const T y2 = y;
        const int8_t sx = __SETS__(x);
        return ((1 ^ sx) ^ __SETS__(y2)) & (sx ^ __SETS__(x+y2));
    }
#endif
}

// carry flag of subtraction (x-y)
template<NH3API_IDA_INTEGRAL_TEMPLATE_2(T, U)> NH3API_IDA_INTRIN
int8_t __CFSUB__(T x, U y) noexcept
{
    using common_type = std::make_unsigned_t<std::common_type_t<T, U>>;
#if NH3API_HAS_CONSTEXPR_BUILTIN(__builtin_subcb) && \
    NH3API_HAS_CONSTEXPR_BUILTIN(__builtin_subcs) && \
    NH3API_HAS_CONSTEXPR_BUILTIN(__builtin_subc) && \
    NH3API_HAS_CONSTEXPR_BUILTIN(__builtin_subcll)
    const common_type ux = static_cast<common_type>(x);
    const common_type uy = static_cast<common_type>(y);
    common_type carryout;
    if constexpr ( sizeof(common_type) == 1 )
        __builtin_subcb(ux, uy, 0, &carryout);
    else if constexpr ( sizeof(common_type) == 2 )
        __builtin_subcs(ux, uy, 0, &carryout);
    else if constexpr ( sizeof(common_type) == 4 )
        __builtin_subc(ux, uy, 0, &carryout);
    else if constexpr ( sizeof(common_type) == 8 )
        __builtin_subcll(ux, uy, 0, &carryout);
    else
        return ux < uy;
    return static_cast<int8_t>(carryout);
#else
    return (static_cast<common_type>(x) < static_cast<common_type>(y));
#endif
}

// carry flag of addition (x+y)
template<NH3API_IDA_INTEGRAL_TEMPLATE_2(T, U)> NH3API_IDA_INTRIN
int8_t __CFADD__(T x, U y) noexcept
{
    using common_type = std::make_unsigned_t<std::common_type_t<T, U>>;
#if NH3API_HAS_CONSTEXPR_BUILTIN(__builtin_addcb) && \
    NH3API_HAS_CONSTEXPR_BUILTIN(__builtin_addcs) && \
    NH3API_HAS_CONSTEXPR_BUILTIN(__builtin_addc) && \
    NH3API_HAS_CONSTEXPR_BUILTIN(__builtin_addcll)
    common_type ux = static_cast<common_type>(x);
    common_type uy = static_cast<common_type>(y);
    common_type carryout;
    if constexpr (sizeof(common_type) == 1)
        __builtin_addcb(ux, uy, 0, &carryout);
    else if constexpr (sizeof(common_type) == 2)
        __builtin_addcs(ux, uy, 0, &carryout);
    else if constexpr (sizeof(common_type) == 4)
        __builtin_addc(ux, uy, 0, &carryout);
    else if constexpr (sizeof(common_type) == 8)
        __builtin_addcll(ux, uy, 0, &carryout);
    else
    {
        // fallback: emulate using addition and comparison
        common_type sum = ux + uy;
        return (sum < ux || sum < uy);
    }
    return static_cast<int8_t>(carryout);
#else
    common_type sum = static_cast<common_type>(x) + static_cast<common_type>(y);
    return (sum < static_cast<common_type>(x) || sum < static_cast<common_type>(y));
#endif
}

NH3API_IDA_INTRIN int8_t abs8(int8_t  x) noexcept
{ return x >= 0 ? x : static_cast<int8_t>(-x); }

NH3API_IDA_INTRIN int16_t abs16(int16_t x) noexcept
{ return x >= 0 ? x : static_cast<int16_t>(-x); }

NH3API_IDA_INTRIN int32_t abs32(int32_t x) noexcept
{
#if NH3API_HAS_BUILTIN(__builtin_abs)
    return __builtin_abs(x);
#else
    return x >= 0 ? x : -x;
#endif
}

NH3API_IDA_INTRIN int64_t abs64(int64_t x) noexcept
{
#if NH3API_HAS_BUILTIN(__builtin_llabs)
    return __builtin_llabs(x);
#else
    return x >= 0 ? x : -x;
#endif
}

#ifndef COERCE_FLOAT
    #if NH3API_HAS_BUILTIN_BIT_CAST
        #define COERCE_FLOAT(...)          (__builtin_bit_cast(float, (__VA_ARGS__)))
        #define COERCE_DOUBLE(...)         (__builtin_bit_cast(double, (__VA_ARGS__)))
        #define COERCE_LONG_DOUBLE(...)    (__builtin_bit_cast(long double, (__VA_ARGS__)))
        #define COERCE_UNSIGNED_INT(...)   (__builtin_bit_cast(uint32_t, (__VA_ARGS__)))
        #define COERCE_UNSIGNED_INT64(...) (__builtin_bit_cast(uint64_t, (__VA_ARGS__)))
    #else
        #define COERCE_FLOAT(...)          (::nh3api::bit_cast<float>((__VA_ARGS__)))
        #define COERCE_DOUBLE(...)         (::nh3api::bit_cast<double>((__VA_ARGS__)))
        #define COERCE_LONG_DOUBLE(...)    (::nh3api::bit_cast<long double>((__VA_ARGS__)))
        #define COERCE_UNSIGNED_INT(...)   (::nh3api::bit_cast<uint32_t>((__VA_ARGS__)))
        #define COERCE_UNSIGNED_INT64(...) (::nh3api::bit_cast<uint64_t>((__VA_ARGS__)))
    #endif
#endif

#ifndef CONTAINING_RECORD
    #define CONTAINING_RECORD(address, type, field) ((type *)((_BYTE*)(address) - offsetof(type, field)))
#endif

#ifndef JUMPOUT
    #define JUMPOUT(address) (((__noreturn void (*)(void))(address))())
#endif

#ifndef ADJ
    #ifdef __widberg__
        #define ADJ(ptr) ((__parentof(ptr) *)((char *)(ptr) - __deltaof(ptr)))
    #else
        #define ADJ(ptr) (static_assert(0, "__shifted pointers are not supported by the current compiler. You may want to use LLVM with Widberg extensions to use this feature."))
    #endif
#endif
