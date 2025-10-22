//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "ida.hpp"

#if NH3API_CHECK_MSVC
    #define NH3API_INTRIN_FUNCTION NH3API_FORCEINLINE
#else
    #define NH3API_INTRIN_FUNCTION NH3API_IDA_INTRIN
#endif

namespace nh3api
{

// integer math

#if !NH3API_CHECK_MSVC
// integer logarithm of base 2
NH3API_CONST constexpr NH3API_FORCEINLINE uint32_t ilog2(uint32_t x) noexcept
{ return (static_cast<uint32_t>(__builtin_clz(x))) ^ 31u; }
#else
NH3API_CONST constexpr
// integer logarithm of base 2
NH3API_FORCEINLINE uint32_t ilog2(uint32_t x) noexcept
{
    NH3API_IF_CONSTEVAL
    {
        // implementation from MSVC STL
        uint32_t _Yx = 0;

        unsigned int _Nx = sizeof(uint32_t) * CHAR_BIT;
        unsigned int _Cx = (sizeof(uint32_t) * CHAR_BIT) / 2;
        do {
            _Yx = static_cast<uint32_t>(x >> _Cx);
            if (_Yx != 0) {
                _Nx -= _Cx;
                x = _Yx;
            }
            _Cx >>= 1;
        } while (_Cx != 0);
        return static_cast<uint32_t>(static_cast<int>(_Nx) - static_cast<int>(x));
    }
    else
    {
        unsigned long result;
        _BitScanReverse(&result, x);
        return result;
    }
}
#endif

NH3API_CONST
// count the amount of digits in an integer
constexpr NH3API_FORCEINLINE uint32_t count_digits(uint32_t x) noexcept
{
    constexpr uint32_t digits_table[] = {9, 99, 999, 9999, 99999,
    999999, 9999999, 99999999, 999999999};
    uint32_t y = (9 * ilog2(x)) >> 5;
    NH3API_ASSUME(y < 9);
    y += x > digits_table[y];
    return y + 1;
}

} // namespace nh3api

NH3API_INTRIN_FUNCTION
// Reverses the order of bytes in 16-bit <x> /
// Меняет порядок бит на обратный в 16-битной области памяти <x>.
uint16_t byteswap16(uint16_t) noexcept;

NH3API_INTRIN_FUNCTION
// Reverses the order of bytes in 32-bit <x> /
// Меняет порядок бит на обратный в 32-битной области памяти <x>.
uint32_t byteswap32(uint32_t) noexcept;

NH3API_INTRIN_FUNCTION
// Reverses the order of bytes in 64-bit <x> /
// Меняет порядок бит на обратный в 64-битной области памяти <x>.
uint64_t byteswap64(uint64_t) noexcept;

NH3API_INTRIN_FUNCTION
// Count leading zeros. Returns 1 if x == 0 /
// Посчитать количество нулей до первого бита = 1 начиная с самого старшего бита. Возвращает 1 если x равно нулю.
uint32_t bitclz(uint32_t) noexcept;

NH3API_INTRIN_FUNCTION
// Count leading zeros. Returns 1 if x == 0 /
// Посчитать количество нулей до первого бита = 1 начиная с самого старшего бита. Возвращает 1 если x равно нулю.
uint32_t bitclz64(uint64_t) noexcept;

NH3API_INTRIN_FUNCTION
// Count trailing zeros /
// Посчитать количество нулей до первого бита = 1 начиная с самого младшего бита.
uint32_t bitctz(uint32_t) noexcept;

NH3API_INTRIN_FUNCTION
// Count trailing zeros /
// Посчитать количество нулей до первого бита = 1 начиная с самого младшего бита.
uint32_t bitctz64(uint64_t) noexcept;

NH3API_INTRIN_FUNCTION
// Find first set bit /
// Найти первый бит == 1.
uint32_t bitffs(uint32_t) noexcept;

NH3API_INTRIN_FUNCTION
// Find first set bit /
// Найти первый бит == 1.
uint32_t bitffs64(uint64_t) noexcept;

NH3API_INTRIN_FUNCTION
// Find highest set bit /
// Найти последний бит == 1.
uint32_t bitfhs(uint32_t) noexcept;

NH3API_INTRIN_FUNCTION
// Find highest set bit /
// Найти последний бит == 1.
uint32_t bitfhs64(uint64_t) noexcept;

NH3API_INTRIN_FUNCTION
// Count number of set bits /
// Посчитать кол-во бит, == 1.
uint32_t bitpopcnt(uint32_t) noexcept;

// Count number of set bits /
// Посчитать кол-во бит, == 1.
NH3API_INTRIN_FUNCTION
uint32_t bitpopcnt64(uint64_t x) noexcept;

NH3API_INTRIN_FUNCTION
// Bit rotate left /
// Побитовый поворот влево.
uint32_t bitrotl(uint32_t n, uint32_t c) noexcept;

NH3API_INTRIN_FUNCTION
// Bit rotate right /
// Побитовый поворот вправо.
uint32_t bitrotr(uint32_t n, uint32_t c) noexcept;

NH3API_INTRIN_FUNCTION
// Bit rotate left /
// Побитовый поворот влево.
uint64_t bitrotl64(uint64_t n, uint32_t c) noexcept;

NH3API_INTRIN_FUNCTION
// Bit rotate right /
// Побитовый поворот вправо.
uint64_t bitrotr64(uint64_t n, uint32_t c) noexcept;

#if NH3API_CHECK_CLANG_CL && !NH3API_HAS_BUILTINS 
#pragma comment(lib, "clang_rt.builtins-i386.lib")
extern int __builtin_clz(unsigned int) noexcept;
extern int __builtin_clzll(unsigned long long) noexcept;
extern int __builtin_ffs(int) noexcept;
extern int __builtin_ffsll(long long) noexcept; 
extern int __builtin_ctz(unsigned int) noexcept;
extern int __builtin_ctzll(unsigned long long) noexcept;
extern int __builtin_popcount(unsigned int) noexcept;
extern int __builtin_popcountll(unsigned long long) noexcept;
#endif 

#if NH3API_CHECK_MSVC
    NH3API_INTRIN_FUNCTION
    uint16_t byteswap16(uint16_t x) noexcept
    { return _byteswap_ushort(x); }

    NH3API_INTRIN_FUNCTION
    uint32_t byteswap32(uint32_t x) noexcept
    { return _byteswap_ulong(x); }

    NH3API_INTRIN_FUNCTION
    uint64_t byteswap64(uint64_t x) noexcept
    { return _byteswap_uint64(x); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitctz(uint32_t x) noexcept
    { unsigned long result; _BitScanForward(&result, x); return result; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitctz64(uint64_t x) noexcept
    {
        unsigned long result = 0;
        if ( !_BitScanForward(&result, (uint32_t)x) &&
            _BitScanForward(&result, (uint32_t)(x >> 32)) )
        {
            result += 32;
        }
        return result;
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitclz(uint32_t x) noexcept
    { unsigned long result; _BitScanReverse(&result, x); return result ^ 31; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitclz64(uint64_t x) noexcept
    {
        unsigned long result = 0;
        if ( !_BitScanReverse(&result, (uint32_t)x) &&
            _BitScanReverse(&result, (uint32_t)(x >> 32)) )
        {
            result += 32;
        }
        return result ^ 63;
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitffs(uint32_t x) noexcept
    { return bitctz(x) + 1; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitffs64(uint64_t x) noexcept
    { return bitctz64(x) + 1; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitfhs(uint32_t x) noexcept
    { unsigned long result; _BitScanReverse(&result, x); return result; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitfhs64(uint64_t x) noexcept
    {
        unsigned long result = 0;
        if ( !_BitScanReverse(&result, (uint32_t)x) &&
            _BitScanReverse(&result, (uint32_t)(x >> 32)) )
        {
            result += 32;
        }
        return result;
    }

    // MSVC has no x86 architecture switches(they are broken)
    // In GCC and clang you can set any specific like i686, pentium4, haswell, etc.
    // so for the compability no popcnt instruction for the MSVC!
    NH3API_INTRIN_FUNCTION
    uint32_t bitpopcnt(uint32_t v) noexcept
    {
        v = v - ((v >> 1) & 0x55555555);
        v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
        return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitpopcnt64(uint64_t x) noexcept
    { return bitpopcnt((uint32_t)(x)) + bitpopcnt((uint32_t)(x >> 32)); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitrotl(uint32_t n, uint32_t c) noexcept
    { return _rotl(n, static_cast<int32_t>(c)); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitrotr(uint32_t n, uint32_t c) noexcept
    { return _rotr(n, static_cast<int32_t>(c)); }

    NH3API_INTRIN_FUNCTION
    uint64_t bitrotl64(uint64_t n, uint32_t c) noexcept
    { return _rotl64(n, static_cast<int32_t>(c)); }

    NH3API_INTRIN_FUNCTION
    uint64_t bitrotr64(uint64_t n, uint32_t c) noexcept
    { return _rotr64(n, static_cast<int32_t>(c)); }

#else
    NH3API_INTRIN_FUNCTION
    uint16_t byteswap16(uint16_t x) noexcept
    { 
    #if __has_builtin(__builtin_bswap16)
        return __builtin_bswap16(x); 
    #else 
        return (x >> 8) | (x << 8);
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint32_t byteswap32(uint32_t x) noexcept
    { 
    #if __has_builtin(__builtin_bswap32)
        return __builtin_bswap32(x); 
    #else 
        return (x >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | (x << 24);
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint64_t byteswap64(uint64_t x) noexcept
    {
    #if __has_builtin(__builtin_bswap64)
        return __builtin_bswap64(x); 
    #else 
        return (x >> 56) |
           ((x & 0x00FF000000000000) >> 40) |
           ((x & 0x0000FF0000000000) >> 24) |
           ((x & 0x000000FF00000000) >> 8) |
           ((x & 0x00000000FF000000) << 8) |
           ((x & 0x0000000000FF0000) << 24) |
           ((x & 0x000000000000FF00) << 40) |
           (x << 56);
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitclz(uint32_t x) noexcept
    { return static_cast<uint32_t>(__builtin_clz(x)); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitclz64(uint64_t x) noexcept
    { return static_cast<uint32_t>(__builtin_clzll(x)); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitctz(uint32_t x) noexcept
    { return static_cast<uint32_t>(__builtin_ctz(x)); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitctz64(uint64_t x) noexcept
    { return static_cast<uint32_t>(__builtin_ctzll(x)); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitffs(uint32_t x) noexcept
    { 
    #if __has_builtin(__builtin_ffs)
        return static_cast<uint32_t>(__builtin_ffs(static_cast<int32_t>(x))); 
    #else 
        return bitctz(x) + 1;
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitffs64(uint64_t x) noexcept
    { 
    #if __has_builtin(__builtin_ffsll)
        return static_cast<uint32_t>(__builtin_ffsll(static_cast<int64_t>(x))); 
    #else 
        return bitctz64(x) + 1;
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitfhs(uint32_t x) noexcept
    { return static_cast<uint32_t>(__builtin_clz(x) ^ 31); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitfhs64(uint64_t x) noexcept
    { return static_cast<uint32_t>(__builtin_clzll(x) ^ 63); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitpopcnt(uint32_t v) noexcept
    { return static_cast<uint32_t>(__builtin_popcount(v)); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitpopcnt64(uint64_t x) noexcept
    { return static_cast<uint32_t>(__builtin_popcountll(x)); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitrotl(uint32_t n, uint32_t c) noexcept
    {
        const uint32_t mask = (8 * sizeof(n) - 1);
        c &= mask;
        return (n << c) | (n >> ((-c) & mask));
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitrotr(uint32_t n, uint32_t c) noexcept
    {
        const uint32_t mask = (8 * sizeof(n) - 1);
        c &= mask;
        return (n >> c) | (n << ((-c) & mask));
    }

    NH3API_INTRIN_FUNCTION
    uint64_t bitrotl64(uint64_t n, uint32_t c) noexcept
    {
        c &= (8 * sizeof(n) - 1);
        if (!c)
            return n;
        return (n >> (64 - c)) | (n << c);
    }

    NH3API_INTRIN_FUNCTION
    uint64_t bitrotr64(uint64_t n, uint32_t c) noexcept
    {
        c &= (8 * sizeof(n) - 1);
        if (!c)
            return n;
        return (n << (64 - c)) | (n >> c);
    }

#endif
