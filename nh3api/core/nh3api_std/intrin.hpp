//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "ida.hpp"

template<typename T>
T* get_ptr(uint32_t address) NH3API_NOEXCEPT
{ return reinterpret_cast<T*>(address); }

// Memory shield does not let the optimizing compiler discard statements
// It is recommended to use it inside your init() function
// like this:
// void init()
// {
// NH3API_MEMSHIELD_BEGIN
// ...
// NH3API_MEMSHIELD_END
// }

#ifdef _MSC_VER
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

// this is a hack designed to enable global variables addresses to be backed into binaries
// rather that using global variables which is double indirection
// whole program optimization doesn't always fix this issue though.
#if NH3API_HAS_BUILTIN(__builtin_constant_p)
    #define get_global_var_ptr(address,...) (__builtin_constant_p(reinterpret_cast<__VA_ARGS__*>(address)) ? reinterpret_cast<__VA_ARGS__*>(address) : reinterpret_cast<__VA_ARGS__*>(address))  // use '-fwhole-program' or '-flto'('-O1' on clang) flag to make it constexpr
    #define get_global_var_ref(address,...) *(__builtin_constant_p(reinterpret_cast<__VA_ARGS__*>(address)) ? reinterpret_cast<__VA_ARGS__*>(address) : reinterpret_cast<__VA_ARGS__*>(address)) // use '-fwhole-program' or '-flto'('-O1' on clang) flag to make it constexpr
#else
    #define get_global_var_ptr(address,...) (reinterpret_cast<__VA_ARGS__*>(address))
    #define get_global_var_ref(address,...) (*reinterpret_cast<__VA_ARGS__*>(address))
#endif

#if NH3API_CHECK_MSVC
    #define NH3API_INTRIN_FUNCTION NH3API_FORCEINLINE
#else
    #define NH3API_INTRIN_FUNCTION NH3API_IDA_INTRIN
#endif

NH3API_INTRIN_FUNCTION
// Reverses the order of bytes in 16-bit <x> /
// Меняет порядок бит на обратный в 16-битной области памяти <x>.
uint16_t byteswap16(uint16_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Reverses the order of bytes in 32-bit <x> /
// Меняет порядок бит на обратный в 32-битной области памяти <x>.
uint32_t byteswap32(uint32_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Reverses the order of bytes in 64-bit <x> /
// Меняет порядок бит на обратный в 64-битной области памяти <x>.
uint64_t byteswap64(uint64_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Count leading zeros. Returns 1 if x == 0 /
// Посчитать количество нулей до первого бита = 1 начиная с самого старшего бита. Возвращает 1 если x равно нулю.
uint32_t bitclz(uint32_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Count leading zeros. Returns 1 if x == 0 /
// Посчитать количество нулей до первого бита = 1 начиная с самого старшего бита. Возвращает 1 если x равно нулю.
uint32_t bitclz64(uint64_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Count trailing zeros /
// Посчитать количество нулей до первого бита = 1 начиная с самого младшего бита.
uint32_t bitctz(uint32_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Count trailing zeros /
// Посчитать количество нулей до первого бита = 1 начиная с самого младшего бита.
uint32_t bitctz64(uint64_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Find first set bit /
// Найти первый бит == 1.
uint32_t bitffs(uint32_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Find first set bit /
// Найти первый бит == 1.
uint32_t bitffs64(uint64_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Find highest set bit /
// Найти последний бит == 1.
uint32_t bitfhs(uint32_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Find highest set bit /
// Найти последний бит == 1.
uint32_t bitfhs64(uint64_t) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Count number of set bits /
// Посчитать кол-во бит, == 1.
uint32_t bitpopcnt(uint32_t) NH3API_NOEXCEPT;

// Count number of set bits /
// Посчитать кол-во бит, == 1.
NH3API_INTRIN_FUNCTION
uint32_t bitpopcnt64(uint64_t x) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Bit rotate left /
// Побитовый поворот влево.
uint32_t bitrotl(uint32_t n, uint32_t c) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Bit rotate right /
// Побитовый поворот вправо.
uint32_t bitrotr(uint32_t n, uint32_t c) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Bit rotate left /
// Побитовый поворот влево.
uint64_t bitrotl64(uint64_t n, uint32_t c) NH3API_NOEXCEPT;

NH3API_INTRIN_FUNCTION
// Bit rotate right /
// Побитовый поворот вправо.
uint64_t bitrotr64(uint64_t n, uint32_t c) NH3API_NOEXCEPT;

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
    uint16_t byteswap16(uint16_t x) NH3API_NOEXCEPT
    { return _byteswap_ushort(x); }

    NH3API_INTRIN_FUNCTION
    uint32_t byteswap32(uint32_t x) NH3API_NOEXCEPT
    { return _byteswap_ulong(x); }

    NH3API_INTRIN_FUNCTION
    uint64_t byteswap64(uint64_t x) NH3API_NOEXCEPT
    { return _byteswap_uint64(x); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitctz(uint32_t x) NH3API_NOEXCEPT
    { unsigned long result; _BitScanForward(&result, x); return result; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitctz64(uint64_t x) NH3API_NOEXCEPT
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
    uint32_t bitclz(uint32_t x) NH3API_NOEXCEPT
    { unsigned long result; _BitScanReverse(&result, x); return result ^ 31; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitclz64(uint64_t x) NH3API_NOEXCEPT
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
    uint32_t bitffs(uint32_t x) NH3API_NOEXCEPT
    { return bitctz(x) + 1; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitffs64(uint64_t x) NH3API_NOEXCEPT
    { return bitctz64(x) + 1; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitfhs(uint32_t x) NH3API_NOEXCEPT
    { unsigned long result; _BitScanReverse(&result, x); return result; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitfhs64(uint64_t x) NH3API_NOEXCEPT
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
    uint32_t bitpopcnt(uint32_t v) NH3API_NOEXCEPT
    {
        v = v - ((v >> 1) & 0x55555555);
        v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
        return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitpopcnt64(uint64_t x) NH3API_NOEXCEPT
    { return bitpopcnt((uint32_t)(x)) + bitpopcnt((uint32_t)(x >> 32)); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitrotl(uint32_t n, uint32_t c) NH3API_NOEXCEPT
    { return _rotl(n, c); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitrotr(uint32_t n, uint32_t c) NH3API_NOEXCEPT
    { return _rotr(n, c); }

    NH3API_INTRIN_FUNCTION
    uint64_t bitrotl64(uint64_t n, uint32_t c) NH3API_NOEXCEPT
    { return _rotl64(n, c); }

    NH3API_INTRIN_FUNCTION
    uint64_t bitrotr64(uint64_t n, uint32_t c) NH3API_NOEXCEPT
    { return _rotr64(n, c); }

#else
    NH3API_INTRIN_FUNCTION
    uint16_t byteswap16(uint16_t x) NH3API_NOEXCEPT
    { 
    #if NH3API_HAS_BUILTIN(__builtin_bswap16)
        return __builtin_bswap16(x); 
    #else 
        return (x >> 8) | (x << 8);
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint32_t byteswap32(uint32_t x) NH3API_NOEXCEPT
    { 
    #if NH3API_HAS_BUILTIN(__builtin_bswap32)
        return __builtin_bswap32(x); 
    #else 
        return (x >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | (x << 24);
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint64_t byteswap64(uint64_t x) NH3API_NOEXCEPT
    {
    #if NH3API_HAS_BUILTIN(__builtin_bswap64)
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
    uint32_t bitclz(uint32_t x) NH3API_NOEXCEPT
    { return __builtin_clz(x); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitclz64(uint64_t x) NH3API_NOEXCEPT
    { return __builtin_clzll(x); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitctz(uint32_t x) NH3API_NOEXCEPT
    { return __builtin_ctz(x); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitctz64(uint64_t x) NH3API_NOEXCEPT
    { return __builtin_ctzll(x); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitffs(uint32_t x) NH3API_NOEXCEPT
    { 
    #if NH3API_HAS_BUILTIN(__builtin_ffs)
        return __builtin_ffs(x); 
    #else 
        return bitctz(x) + 1;
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitffs64(uint64_t x) NH3API_NOEXCEPT
    { 
    #if NH3API_HAS_BUILTIN(__builtin_ffsll)
        return __builtin_ffsll(x); 
    #else 
        return bitctz64(x) + 1;
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitfhs(uint32_t x) NH3API_NOEXCEPT
    { return __builtin_clz(x) ^ 31; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitfhs64(uint64_t x) NH3API_NOEXCEPT
    { return __builtin_clzll(x) ^ 63; }

    NH3API_INTRIN_FUNCTION
    uint32_t bitpopcnt(uint32_t v) NH3API_NOEXCEPT
    { return __builtin_popcount(v); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitpopcnt64(uint64_t x) NH3API_NOEXCEPT
    { return __builtin_popcountll(x); }

    NH3API_INTRIN_FUNCTION
    uint32_t bitrotl(uint32_t n, uint32_t c) NH3API_NOEXCEPT
    {
        const uint32_t mask = (8 * sizeof(n) - 1);
        c &= mask;
        return (n << c) | (n >> ((-c) & mask));
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitrotr(uint32_t n, uint32_t c) NH3API_NOEXCEPT
    {
        const uint32_t mask = (8 * sizeof(n) - 1);
        c &= mask;
        return (n >> c) | (n << ((-c) & mask));
    }

    NH3API_INTRIN_FUNCTION
    uint64_t bitrotl64(uint64_t n, uint32_t c) NH3API_NOEXCEPT
    {
        c &= (8 * sizeof(n) - 1);
        if (!c)
            return n;
        return (n >> (64 - c)) | (n << c);
    }

    NH3API_INTRIN_FUNCTION
    uint64_t bitrotr64(uint64_t n, uint32_t c) NH3API_NOEXCEPT
    {
        c &= (8 * sizeof(n) - 1);
        if (!c)
            return n;
        return (n << (64 - c)) | (n >> c);
    }

#endif
