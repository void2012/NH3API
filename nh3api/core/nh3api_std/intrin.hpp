//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "ida.hpp"

// this is a hack designed to enable global variables addresses to be backed into binaries
// rather that using global variables which is double indirection
// whole program optimization doesn't always fix this issue though.
#if NH3API_HAS_BUILTIN_CONSTANT_P
    #define constexpr_rcast_ptr(x, ...) (__builtin_constant_p(reinterpret_cast<__VA_ARGS__*>(x)) ? reinterpret_cast<__VA_ARGS__*>(x) : reinterpret_cast<__VA_ARGS__*>(x))
    #define constexpr_rcast(x, ...) (__builtin_constant_p(reinterpret_cast<__VA_ARGS__>(x)) ? reinterpret_cast<__VA_ARGS__>(x) : reinterpret_cast<__VA_ARGS__>(x))
#endif

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

#if NH3API_HAS_BUILTIN_CONSTANT_P
    #define get_global_var_ptr(address,...) constexpr_rcast_ptr(address, __VA_ARGS__)  // use '-fwhole-program' or '-flto'('-O1' on clang) flag to make it constexpr
    #define get_global_var_ref(address,...) *constexpr_rcast_ptr(address, __VA_ARGS__) // use '-fwhole-program' or '-flto'('-O1' on clang) flag to make it constexpr
#else
    #define get_global_var_ptr(address,...) reinterpret_cast<__VA_ARGS__*>(address)
    #define get_global_var_ref(address,...) *reinterpret_cast<__VA_ARGS__*>(address)
#endif

#if NH3API_CHECK_MSVC
    #define NH3API_INTRIN_FUNCTION NH3API_FORCEINLINE
#else
    #define NH3API_INTRIN_FUNCTION NH3API_IDA_INTRIN
#endif

namespace nh3api
{

#if NH3API_CHECK_CPP14
template <class T1, class T2>
constexpr T1* memmove_constexpr_impl(T1* dst, T2* src, uint32_t count);

template <> constexpr
char* memmove_constexpr_impl<char, const char>(char* dst, const char* src, uint32_t count)
{
#if NH3API_HAS_BUILTIN_MEMMOVE
    __builtin_memmove(dst, src, count);
    return dst;
#else
    if ( dst == src )
        return dst;
    bool loop_forward = true;

    for (const char* ptr = src; ptr != src + count; ++ptr)
    {
        if (dst == ptr)
        {
            loop_forward = false;
            break;
        }
    }
    if (loop_forward)
    {
        for (size_t i = 0; i < count; ++i)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        for (size_t i = 0; i < count; ++i)
        {
            dst[count - 1 - i] = src[count - 1 - i];
        }
    }
    return dst;
#endif
}

template <> constexpr
wchar_t* memmove_constexpr_impl<wchar_t, const wchar_t>(wchar_t* dst, const wchar_t* src, uint32_t count)
{
#if NH3API_HAS_BUILTIN_MEMMOVE
    __builtin_memmove(dst, src, count * sizeof(wchar_t));
    return dst;
#else
    if ( dst == src )
        return dst;
    bool loop_forward = true;

    for (const wchar_t *ptr = src; ptr != src + count; ++ptr)
    {
        if (dst == ptr)
        {
            loop_forward = false;
            break;
        }
    }
    if (loop_forward)
    {
        for (size_t i = 0; i < count; ++i)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        for (size_t i = 0; i < count; ++i)
        {
            dst[count - 1 - i] = src[count - 1 - i];
        }
    }
    return dst;
#endif
}

#endif // C++14

template<typename CharT>
struct str_func_chooser;

template<>
struct str_func_chooser<char>
{
    #if NH3API_HAS_BUILTIN_STRLEN && NH3API_CHECK_CPP11
    static constexpr size_t _strlen(const char* str) NH3API_NOEXCEPT
    {  return __builtin_strlen(str); }
    #else
    static size_t _strlen(const char* str) NH3API_NOEXCEPT
    { return ::std::strlen(str); }
    #endif

    #if NH3API_HAS_BUILTIN_MEMCMP && NH3API_CHECK_CPP11
    static constexpr int32_t _memcmp(const char* s1, const char* s2, size_t count) NH3API_NOEXCEPT
    { return __builtin_memcmp(s1, s2, count); }

    #else
    static int32_t _memcmp(const char* s1, const char* s2, size_t count) NH3API_NOEXCEPT
    { return ::std::memcmp(s1, s2, count); }
    #endif

    #if NH3API_HAS_BUILTIN_CHAR_MEMCHR && NH3API_CHECK_CPP11
    static constexpr char* _memchr(const char* haystack, char needle, size_t count)
    {
        return __builtin_char_memchr(haystack, needle, count);
    }
    #else
    static char* _memchr(const char* haystack, char needle, size_t count)
    { return reinterpret_cast<char*>(const_cast<void*>(::std::memchr(haystack, needle, count))); }
    #endif

    #if NH3API_HAS_BUILTIN_MEMMOVE && NH3API_CHECK_CPP11
    static constexpr char* _memmove(char* dst, const char* src, size_t count)
    {
        __builtin_memmove(dst, src, count);
        return dst;
    }
    #else
    static char* _memmove(char* dst, const char* src, size_t count)
    {
        ::std::memmove(dst, src, count);
        return dst;
    }
    #endif
};

template<>
struct str_func_chooser<wchar_t>
{
    #if NH3API_HAS_BUILTIN_WCSLEN
    static constexpr size_t _strlen(const wchar_t* str) NH3API_NOEXCEPT
    {  return __builtin_wcslen(str); }
    #else
    static size_t _strlen(const wchar_t* str) NH3API_NOEXCEPT
    { return ::std::wcslen(str); }
    #endif

    #if NH3API_HAS_BUILTIN_WMEMCMP
    static constexpr int32_t _memcmp(const wchar_t* s1, const wchar_t* s2, size_t count) NH3API_NOEXCEPT
    { return __builtin_wmemcmp(s1, s2, count); }

    #else
    static int32_t _memcmp(const wchar_t* s1, const wchar_t* s2, size_t count) NH3API_NOEXCEPT
    { return ::std::wmemcmp(s1, s2, count); }
    #endif

    #if NH3API_HAS_BUILTIN_WMEMCHR
    static constexpr const wchar_t* _memchr(const wchar_t* haystack, wchar_t needle, size_t count)
    { return __builtin_wmemchr(haystack, needle, count); }
    #else
    static const wchar_t* _memchr(const wchar_t* haystack, wchar_t needle, size_t count)
    { return ::std::wmemchr(haystack, needle, count); }
    #endif

    #if NH3API_HAS_BUILTIN_MEMMOVE && NH3API_CHECK_CPP11
    static constexpr wchar_t* _memmove(wchar_t* dst, const wchar_t* src, size_t count)
    {
        __builtin_memmove(dst, src, count * sizeof(wchar_t));
        return dst;
    }
    #else
    static wchar_t* _memmove(wchar_t* dst, const wchar_t* src, size_t count)
    {
        ::std::wmemmove(dst, src, count);
        return dst;
    }
    #endif
};

template<typename CharT> NH3API_CONSTEXPR_CPP_14
size_t strlen_constexpr_impl(const CharT* str) NH3API_NOEXCEPT
{
    if ( str )
    {
        const CharT* ptr = str;
        while (*ptr != CharT(0))
            ++ptr;
        return ptr - str;
    }
    else
    {
        return 0;
    }
}

template<typename CharT> NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
size_t strlen_constexpr(const CharT* str) NH3API_NOEXCEPT
{
    #if NH3API_HAS_IS_CONSTANT_EVALUATED
    if ( is_constant_evaluated() )
        return strlen_constexpr_impl(str);
    else
        return str_func_chooser<CharT>::_strlen(str);
    #else
    return str_func_chooser<CharT>::_strlen(str);
    #endif
}

template <class T1, class T2> NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
T1* memmove_constexpr(T1* dst, T2* src, uint32_t count)
{
    #if NH3API_HAS_IS_CONSTANT_EVALUATED
    if ( is_constant_evaluated() )
        return memmove_constexpr_impl(dst, src, count);
    else
        return str_func_chooser<T1>::_memmove(dst, src, count);
    #else
    return str_func_chooser<T1>::_memmove(dst, src, count);
    #endif
}

template<typename CharT> NH3API_CONSTEXPR_CPP_14
int32_t memcmp_constexpr_impl(const CharT* s1, const CharT* s2, size_t count)
{
    while ( count-- != 0 )
    {
        if ( *s1 < *s2 )
            return -1;
        if ( *s1 > *s2 )
            return +1;
        ++s1;
        ++s2;
    }
    return 0;
}

//memcmp_constexpr_impl(s1, s2, count);
template<typename CharT> NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
int32_t memcmp_constexpr(const CharT* s1, const CharT* s2, size_t count)
{
#if NH3API_HAS_BUILTIN_IS_CONSTANT_EVALUATED
    if ( is_constant_evaluated() )
        return memcmp_constexpr_impl(s1, s2, count);
    else
        return str_func_chooser<CharT>::_memcmp(s1, s2, count);
#else
    return str_func_chooser<CharT>::_memcmp(s1, s2, count);
#endif
}

template<typename T, typename CharT> NH3API_CONSTEXPR_CPP_14
T* memchr_constexpr_impl(T* haystack, CharT needle, size_t count)
{
    for ( ; count; --count )
    {
        if ( *haystack == needle )
        {
            return haystack;
        }
        ++haystack;
    }
    return nullptr;
}

template<typename CharT> NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
const CharT* memchr_constexpr(const CharT* haystack, CharT needle, size_t count)
{
    NH3API_CONSTEXPR_VAR bool is_char_char  = tt::is_same<CharT, char>::value;
    NH3API_CONSTEXPR_VAR bool is_char_wchar = tt::is_same<CharT, wchar_t>::value;
    #if NH3API_HAS_BUILTIN_MEMCHR && NH3API_HAS_IS_CONSTANT_EVALUATED
    if ( is_constant_evaluated() )
    {
        return memchr_constexpr_impl(haystack, needle, count);
    }
    else
    {
        NH3API_IF_CONSTEXPR ( is_char_char || is_char_wchar )
            return str_func_chooser<CharT>::_memchr(haystack, needle, count);
        else
            return memchr_constexpr_impl(haystack, needle, count);
    }
    #else
    if ( is_char_char || is_char_wchar )
        return str_func_chooser<CharT>::_memchr(haystack, needle, count);
    else
        return memchr_constexpr_impl(haystack, needle, count);
    #endif
}

}

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


#ifndef retaddr
    #if NH3API_HAS_BUILTINS
        #if __has_builtin(__builtin_return_address)
            // Get the address of the instruction in the calling function that will be executed after control returns to the caller /
            // Возвращает адрес инструкции в вызывающей функции,
            // которая будет выполняться после возврата элемента управления вызывающей функции.
            #define retaddr() (__builtin_return_address(0))
        #endif
    #endif
#endif

#ifndef retaddr
    // Get the address of the instruction in the calling function that will be executed after control returns to the caller /
    // Возвращает адрес инструкции в вызывающей функции,
    // которая будет выполняться после возврата элемента управления вызывающей функции.
    #define retaddr() (_ReturnAddress())
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
    #if __has_builtin(__builtin_bswap16)
        return __builtin_bswap16(x); 
    #else 
        return (x >> 8) | (x << 8);
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint32_t byteswap32(uint32_t x) NH3API_NOEXCEPT
    { 
    #if __has_builtin(__builtin_bswap32)
        return __builtin_bswap32(x); 
    #else 
        return (x >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | (x << 24);
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint64_t byteswap64(uint64_t x) NH3API_NOEXCEPT
    {
    #if __has_builtin(__builtin_bswap32)
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
    #if __has_builtin(__builtin_ffsll)
        return __builtin_ffs(x); 
    #else 
        return bitctz(x) + 1;
    #endif
    }

    NH3API_INTRIN_FUNCTION
    uint32_t bitffs64(uint64_t x) NH3API_NOEXCEPT
    { 
    #if __has_builtin(__builtin_ffsll)
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
        c &= 63;
        if (!c)
            return n;
        return (n >> (64 - c)) | (n << c);
    }

    NH3API_INTRIN_FUNCTION
    uint64_t bitrotr64(uint64_t n, uint32_t c) NH3API_NOEXCEPT
    {
        c &= 63;
        if (!c)
            return n;
        return (n << (64 - c)) | (n >> c);
    }

#endif
