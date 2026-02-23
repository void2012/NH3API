//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "ida.hpp" // abs32

namespace nh3api
{

/*
struct constexpr_char_traits
{
public:
    using char_type = char;
    using int_type = int;
    using state_type = ::std::mbstate_t;
    #ifdef __cpp_lib_three_way_comparison
    using comparison_category = ::std::strong_ordering;
    #endif

private:
    enum : uint32_t { npos = (uint32_t)(-1) };

public:
    static inline constexpr void assign(char& c1, const char& c2) noexcept
    { c1 = c2; }

    static inline constexpr bool eq(char c1, char c2) noexcept
    { return c1 == c2; }

    static inline constexpr bool lt(char c1, char c2) noexcept
    { return static_cast<uint8_t>(c1) < static_cast<uint8_t>(c2); }

    static constexpr size_t length(const char* str) noexcept
    {
        if ( str == nullptr )
            return 0;

        return __builtin_strlen(str);
    }

    static constexpr int compare(const char* left, const char* right, size_t count) noexcept
    {
        if ( left == nullptr || right == nullptr || count == 0 )
            return 0;

        return __builtin_memcmp(left, right, count);
    }

    #if defined(__cpp_lib_constexpr_string) && (__cpp_lib_constexpr_string >= 201907L)
    static constexpr char* move(char* dst, const char* src, size_t count) noexcept
    {
        #if NH3API_DEBUG
        if ( dst == nullptr || src == nullptr )
            return nullptr;
        #endif

        return ::std::char_traits<char>::move(dst, src, count);
    }
    #else
    static constexpr char* move(char* dst, const char* src, size_t count) noexcept
    {
        #if NH3API_DEBUG
        if ( dst == nullptr || src == nullptr )
            return nullptr;
        #endif

        #if NH3API_HAS_BUILTIN(__builtin_memmove)
        __builtin_memmove(dst, src, count);
        return dst;
        #else
        NH3API_IF_CONSTEVAL
        {
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
        }
        else
        {
            ::std::memmove(dst, src, count);
            return dst;
        }
        #endif // NH3API_HAS_BUILTIN(__builtin_memmove)
    }
    #endif

    #if defined(__cpp_lib_constexpr_string) && (__cpp_lib_constexpr_string >= 201907L)
    static constexpr char* copy(char* dst, const char* src, size_t count) noexcept
    {
        #if NH3API_DEBUG
        if ( dst == nullptr || src == nullptr )
            return nullptr;
        #endif

        return ::std::char_traits<char>::copy(dst, src, count);
    }
    #else
    static constexpr char* copy(char* dst, const char* src, size_t count) noexcept
    {
        if ( dst == nullptr || src == nullptr )
            return nullptr;

        #if NH3API_HAS_BUILTIN(__builtin_memcpy)
        __builtin_memcpy(dst, src, count);
        return dst;
        #else
        NH3API_IF_CONSTEVAL
        {
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
        }
        else
        {
            ::std::memcpy(dst, src, count);
            return dst;
        }
        #endif // NH3API_HAS_BUILTIN(__builtin_memcpy)
    }
    #endif

    #if defined(__cpp_lib_constexpr_string) && (__cpp_lib_constexpr_string >= 201907L)
    static constexpr char* assign(char* str, size_t count, char value) noexcept
    {
        #if NH3API_DEBUG
        if ( str == nullptr )
            return nullptr;
        #endif
        return ::std::char_traits<char>::assign(str, count, value);
    }
    #else
    static constexpr char* assign(char* str, size_t count, char value) noexcept
    {
        #if NH3API_DEBUG
        if ( str == nullptr )
            return nullptr;
        #endif
        #if NH3API_HAS_BUILTIN(__builtin_memset)
        __builtin_memset(str, value, count);
        return str;
        #else
            NH3API_IF_CONSTEVAL
            {
                for (size_t i = 0; i < count; i++)
                    *str++ = value;
                return str;
            }
            else
            {
                ::std::memset(str, value, count);
                return str;
            }
        #endif
    }
    #endif

    #if defined(__cpp_lib_constexpr_string)
    static constexpr const char* find(const char* str, size_t count, const char& ch) noexcept
    { return ::std::char_traits<char>::find(str, count, ch); }
    #else
    static constexpr const char* find(const char* str, size_t count, const char& ch) noexcept
    {
        #if NH3API_DEBUG
        if ( count == 0 || str == nullptr)
            return nullptr;
        #endif

        return __builtin_char_memchr(str, ch, count);
    }
    #endif

};
*/

// safe strlen which is bound to max_size
inline constexpr size_t safe_strlen(const char* str, const size_t max_size) noexcept
{
    if ( str == nullptr )
            return 0;

    NH3API_IF_CONSTEVAL
    {
        size_t len = 0;
        while ( len < max_size && str[len] != '\0' )
            ++len;

        return len;
    }
    else
    {
        return ::strnlen(str, max_size);
    }
}

// safe strlen which is bound to max_size
inline constexpr size_t safe_strlen(const wchar_t* str, const size_t max_size) noexcept
{
    if ( str == nullptr )
            return 0;

    NH3API_IF_CONSTEVAL
    {
        size_t len = 0;
        while (len < max_size && str[len] != L'\0')
            ++len;

        return len;
    }
    else
    {
        return ::wcsnlen(str, max_size);
    }
}
// constexpr atoi that uses no locale
inline constexpr int32_t constexpr_atoi(const char* p) noexcept
{
    int32_t x   = 0;
    bool    neg = false;

    if ( *p == '-' )
    {
        neg = true;
        ++p;
    }

    while ( *p >= '0' && *p <= '9' )
    {
        x = (x * 10) + (*p - '0');
        ++p;
    }

    if ( neg )
        x = -x;

    return x;
}

// convert letter character to lowercase
[[nodiscard]] inline constexpr char tolower_constexpr(const char c) noexcept
{ return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c; }

/*
struct case_insensitive_traits
{
    using char_type = char;
    using int_type = int;
    using state_type = ::std::mbstate_t;
    #ifdef __cpp_lib_three_way_comparison
    using comparison_category = ::std::weak_ordering;
    #endif

    static constexpr char* move(char* dst, const char* src, size_t count) noexcept
    { return constexpr_char_traits::move(dst, src, count); }

    static constexpr bool eq(char c1, char c2) noexcept
    { return tolower_constexpr(c1) == tolower_constexpr(c2); }

    static constexpr bool ne(char c1, char c2) noexcept
    { return tolower_constexpr(c1) != tolower_constexpr(c2); }

    static constexpr bool lt(char c1, char c2) noexcept
    { return tolower_constexpr(c1) < tolower_constexpr(c2); }

    static constexpr int compare(const char* s1, const char* s2, size_t n) noexcept
    {
        if ( s1 == nullptr || s2 == nullptr )
            return 0;

        while (n-- != 0)
        {
            if ( tolower_constexpr(*s1) < tolower_constexpr(*s2) )
                return -1;
            if ( tolower_constexpr(*s1) > tolower_constexpr(*s2) )
                return 1;
            ++s1;
            ++s2;
        }
        return 0;
    }

    static constexpr const char* find(const char* s, int n, char a) noexcept
    {
        if ( s == nullptr )
            return nullptr;

        while ( n-- > 0)
        {
            if ( tolower_constexpr(*s) == tolower_constexpr(a) )
            {
                return s;
            }
            ++s;
        }
        return nullptr;
    }

    static inline constexpr void assign(char& c1, const char& c2) noexcept
    { c1 = tolower_constexpr(c2); }

    static constexpr char* copy(char* dst, const char* src, size_t count) noexcept
    { return constexpr_char_traits::copy(dst, src, count); }

    static constexpr char* assign(char* str, size_t count, char value) noexcept
    { return constexpr_char_traits::assign(str, count, tolower_constexpr(value)); }

    static constexpr size_t length(const char* str) noexcept
    {
        if ( str == nullptr )
            return 0;

        return __builtin_strlen(str);
    }
};
*/

inline constexpr bool isalpha_constexpr(const char c) noexcept
{
    constexpr char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz";
    const char*    letter     = &alphabet[0];

    while ( *letter != '\0' && *letter != c )
        ++letter;

    if ( *letter )
        return true;

    return false;
}

} // namespace nh3api
