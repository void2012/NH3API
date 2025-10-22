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

#include <wchar.h>
#include <cstring> // size_t
#include <string>  // std::char_traits, and optionally std::basic_string
#include "nh3api_std.hpp"

namespace nh3api 
{
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
    static constexpr NH3API_FORCEINLINE
    void assign(char& c1, const char& c2) noexcept
    { c1 = c2; }

    static constexpr NH3API_FORCEINLINE
    bool eq(char c1, char c2) noexcept
    { return c1 == c2; }

    static constexpr NH3API_FORCEINLINE
    bool lt(char c1, char c2) noexcept
    { return (uint8_t)c1 < (uint8_t)c2; }

    #ifdef __cpp_lib_constexpr_string
    static constexpr size_t length(const char* str) noexcept
    { return ::std::char_traits<char>::length(str); }
    #else 
    static constexpr size_t length(const char* str) noexcept
    {
        if ( str == nullptr )
            return 0;

        #if __has_builtin(__builtin_strlen)
        return __builtin_strlen(str);
        #else // __has_builtin(__builtin_strlen)
            NH3API_IF_CONSTEVAL
            {
                const char* ptr = str;
                while (*ptr != '\0')
                    ++ptr;
                return ptr - str;

                return 0;
            }
            else  
            {
                return ::std::strlen(str);
            }
        #endif // __has_builtin(__builtin_strlen)
    }
    #endif
    
    #ifdef __cpp_lib_constexpr_string
    static constexpr int compare(const char* left, const char* right, size_t count) noexcept
    { return ::std::char_traits<char>::compare(left, right, count); }
    #else
    static constexpr int compare(const char* left, const char* right, size_t count) noexcept
    {
        if ( left == nullptr || right == nullptr || count == 0 )
            return 0;
        
        #if __has_builtin(__builtin_memcmp)
        return __builtin_memcmp(left, right, count);
        #else // __has_builtin(__builtin_memcmp)
        NH3API_IF_CONSTEVAL
        {
            while ( count-- != 0 )
            {
                if ( *left < *right )
                    return -1;
                if ( *left > *right )
                    return +1;
                ++left;
                ++right;
            }
            return 0;
        }
        else
        {
            return ::std::memcmp(left, right, count);
        }
        #endif // __has_builtin(__builtin_memcmp)
    }
    #endif

    #if defined(__cpp_lib_constexpr_string) && (__cpp_lib_constexpr_string >= 201907L)
    static constexpr char* move(char* dst, const char* src, size_t count) noexcept
    {
        return ::std::char_traits<char>::move(dst, src, count);
    }
    #else 
    static constexpr char* move(char* dst, const char* src, size_t count) noexcept
    {
        #if __has_builtin(__builtin_memmove)
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
        #endif // __has_builtin(__builtin_memmove)
    }
    #endif

    #if defined(__cpp_lib_constexpr_string) && (__cpp_lib_constexpr_string >= 201907L)
    static constexpr char* copy(char* dst, const char* src, size_t count) noexcept
    { return ::std::char_traits<char>::copy(dst, src, count); }
    #else 
    static constexpr char* copy(char* dst, const char* src, size_t count) noexcept
    {
        if ( dst == nullptr || src == nullptr )
            return nullptr;
        #if __has_builtin(__builtin_memcpy)
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
        #endif // __has_builtin(__builtin_memcpy)
    }
    #endif

    #if defined(__cpp_lib_constexpr_string) && (__cpp_lib_constexpr_string >= 201907L)
    static constexpr char* assign(char* str, size_t count, char value) noexcept
    { return ::std::char_traits<char>::assign(str, count, value); }
    #else
    static constexpr char* assign(char* str, size_t count, char value) noexcept
    {
        if ( str == nullptr )
            return nullptr;
        #if __has_builtin(__builtin_memset)
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
        if ( count == 0 || str == nullptr)
            return nullptr;
        #if __has_builtin(__builtin_char_memchr)
        return __builtin_char_memchr(str, ch, count);
        #else // __has_builtin(__builtin_char_memchr)
        NH3API_IF_CONSTEVAL
        {
            for ( ; count; --count )
            {
                if ( *str == ch )
                {
                    return str;
                }
                ++str;
            }
            return nullptr;
        }
        else 
        {
            return static_cast<const char*>(::std::memchr(str, ch, count));
        }
        #endif // __has_builtin(__builtin_char_memchr)
    }
    #endif

    NH3API_FORCEINLINE size_t constexpr 
    static str_find(const char* str, size_t size, char c, size_t pos) noexcept
    {
        if ( pos >= size )
            return npos;
        const char* found = find(str + pos, size - pos, c);
        if ( found == nullptr )
            return npos;
        return static_cast<size_t>(found - str);
    }

    NH3API_FORCEINLINE constexpr
    static const char* search_substring(const char* first1, 
                                        const char* last1, 
                                        const char* first2, 
                                        const char* last2) noexcept 
    {
        // Take advantage of knowing source and pattern lengths.
        // Stop short when source is smaller than pattern.
        const ptrdiff_t len2 = last2 - first2;
        if (len2 == 0)
            return first1;

        ptrdiff_t len1 = last1 - first1;
        if (len1 < len2)
            return last1;

        // First element of first2 is loop invariant.
        const char f2 = *first2;
        while (true) 
        {
            len1 = last1 - first1;
            // Check whether first1 still has at least len2 bytes.
            if (len1 < len2)
                return last1;

            // Find f2 the first byte matching in first1.
            first1 = find(first1, static_cast<size_t>(len1 - len2 + 1), f2);
            if (first1 == nullptr)
                return last1;

            // It is faster to compare from the first byte of first1 even if we
            // already know that it matches the first byte of first2: this is because
            // first2 is most likely aligned, as it is user's "pattern" string, and
            // first1 + 1 is most likely not aligned, as the match is in the middle of
            // the string.
            if (compare(first1, first2, static_cast<const size_t>(len2)) == 0)
                return first1;

            ++first1;
        }
    }

    NH3API_FORCEINLINE size_t constexpr 
    static str_find(const char* str, size_t size, const char* needle, size_t pos, size_t n) noexcept 
    {
        if (pos > size)
            return npos;

        if (n == 0) // There is nothing to search, just return pos.
            return pos;

        const char* found = search_substring(str + pos, str + size, needle, needle + n);

        if (found == str + size)
            return npos;
        return static_cast<size_t>(found - str);
    }

    NH3API_FORCEINLINE size_t constexpr 
    static str_rfind(const char* str, size_t size, char c, size_t pos) noexcept 
    {
        if (size < 1)
            return npos;

        if (pos < size)
            ++pos;
        else
            pos = size;

        for (const char* i = str + pos; i != str;) 
        {
            if (eq(*--i, c))
                return static_cast<size_t>(i - str);
        }
        return npos;
    }

    NH3API_FORCEINLINE size_t constexpr 
    static str_rfind(const char* str, size_t size, const char* needle, size_t pos, size_t n) noexcept 
    {
        pos = ::std::min<size_t>(pos, size);
        if (n < size - pos)
            pos += n;
        else
            pos = size;
    
        if (n == 0) 
            return (pos <= size) ? pos : size;

        if (size == 0 || n > size)
            return npos;

        for (size_t i = pos - n; ; --i) 
        {
            bool match = true;
            for (size_t j = 0; j < n; ++j) 
            {
                if ( !eq(str[i + j], needle[j]) ) 
                {
                    match = false;
                    break;
                }
            }
            if (match) 
                return i; 
            
            if (i == 0) 
                break; 
        }
        return npos; 
    }

protected:
    NH3API_FORCEINLINE constexpr
    static const char* find_first_of_impl(const char* first1,
                                            const char* last1,
                                            const char* first2,
                                            const char* last2) noexcept
    {
        for (; first1 != last1; ++first1)
            for (const char* i = first2; i != last2; ++i)
                if (eq(*first1, *i))
                    return first1;
        return last1;
    }

public:
    NH3API_FORCEINLINE size_t constexpr 
    static str_find_first_of(const char* str, 
                        size_t size, 
                        const char* needle, 
                        size_t pos, 
                        size_t n) noexcept 
    {
        if (pos >= size || n == 0)
            return npos;
        const char* found = find_first_of_impl(str + pos, str + size, needle, needle + n);
        if (found == str + size)
            return npos;
        return static_cast<size_t>(found - str);
    }

    NH3API_FORCEINLINE size_t constexpr 
    static str_find_last_of(const char* str, size_t size, const char* needle, size_t pos, size_t n) noexcept 
    {
        if (n != 0) 
        {
            if (pos < size)
                ++pos;
            else
                pos = size;
            for (const char* curr = str + pos; curr != str;) 
            {
                const char* const found = find(needle, n, *--curr);
                if (found)
                    return static_cast<size_t>(curr - str);
            }
        }
        return npos;
    }

    NH3API_FORCEINLINE size_t constexpr 
    static str_find_first_not_of(const char* str, size_t size, const char* needle, size_t pos, size_t n) noexcept 
    {
        if (pos < size) 
        {
            const char* shifted = str + size;
            for (const char* curr = str + pos; curr != shifted; ++curr)
                if (find(needle, n, *curr) == nullptr)
                    return static_cast<size_t>(curr - str);
        }
        return npos;
    }

    NH3API_FORCEINLINE size_t constexpr 
    static str_find_first_not_of(const char* str, size_t size, char c, size_t pos) noexcept 
    {
        if (pos < size) 
        {
            const char* const shifted = str + size;
            for (const char* curr = str + pos; curr != shifted; ++curr)
                if (!eq(*curr, c))
                    return static_cast<size_t>(curr - str);
        }
        return npos;
    }

    NH3API_FORCEINLINE size_t constexpr 
    static str_find_last_not_of(const char* str, size_t size, const char* needle, size_t pos, size_t n) noexcept 
    {
        if (pos < size)
            ++pos;
        else
            pos = size;
        for (const char* curr = str + pos; curr != str;)
            if (find(needle, n, *--curr) == nullptr)
                return static_cast<size_t>(curr - str);
        return npos;
    }

    NH3API_FORCEINLINE size_t constexpr 
    static str_find_last_not_of(const char* str, size_t size, char c, size_t pos) noexcept 
    {
        if (pos < size)
            ++pos;
        else
            pos = size;
        for (const char* curr = str + pos; curr != str;)
            if (!eq(*--curr, c))
                return static_cast<size_t>(curr - str);
        return npos;
    }
};

// safe strlen which is bound to max_size
constexpr NH3API_FORCEINLINE size_t safe_strlen(const char* str, const size_t max_size) noexcept
{
    if ( str == nullptr )
            return 0;

    NH3API_IF_CONSTEVAL 
    {
        size_t len = 0;
        while (len < max_size && str[len] != '\0') 
            ++len;
        
        return len;    
    }
    else  
    {
        return ::strnlen(str, max_size);
    }
}

// safe strlen which is bound to max_size
constexpr NH3API_FORCEINLINE size_t safe_strlen(const wchar_t* str, const size_t max_size) noexcept
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
constexpr NH3API_FORCEINLINE int32_t constexpr_atoi(const char* p) noexcept
{
    int32_t x = 0;
    bool neg = false;
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
    {
        x = -x;
    }
    return x;
}

// convert letter character to lowercase
NH3API_CONST NH3API_FORCEINLINE constexpr char tolower_constexpr(const char c) noexcept
{ return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c; }

struct case_insensitive_traits
{
    using char_type = char;
    using int_type = int;
    using state_type = ::std::mbstate_t;
    #ifdef __cpp_lib_three_way_comparison
    using comparison_category = ::std::strong_ordering;
    #endif

    static constexpr char* move(char* dst, const char* src, size_t count) noexcept
    { return constexpr_char_traits::move(dst, src, count); }

    NH3API_CONST static constexpr bool eq(char c1, char c2) noexcept
    { return tolower_constexpr(c1) == tolower_constexpr(c2); }

    NH3API_CONST static constexpr bool ne(char c1, char c2) noexcept
    { return tolower_constexpr(c1) != tolower_constexpr(c2); }

    NH3API_CONST static constexpr bool lt(char c1, char c2) noexcept
    { return tolower_constexpr(c1) < tolower_constexpr(c2); }

    static constexpr int compare(const char* s1, const char* s2, size_t n) noexcept
    {
        while (n-- != 0)
        {
            if (tolower_constexpr(*s1) < tolower_constexpr(*s2))
                return -1;
            if (tolower_constexpr(*s1) > tolower_constexpr(*s2))
                return 1;
            ++s1; 
            ++s2;
        }
        return 0;
    }

    static constexpr const char* find(const char* s, int n, char a) noexcept
    {
        while ( n-- > 0)
        {
            if (tolower_constexpr(*s) == tolower_constexpr(a))
            {
                return s;
            }
            ++s;
        }
        return nullptr;
    }

    static constexpr NH3API_FORCEINLINE
    void assign(char& c1, const char& c2) noexcept
    { c1 = tolower_constexpr(c2); }

    static constexpr char* copy(char* dst, const char* src, size_t count) noexcept
    { return constexpr_char_traits::copy(dst, src, count); }

    static constexpr char* assign(char* str, size_t count, char value) noexcept
    { return constexpr_char_traits::assign(str, count, tolower_constexpr(value)); }

    static constexpr size_t length(const char* str) noexcept
    { return constexpr_char_traits::length(str); }
};

constexpr NH3API_FORCEINLINE 
bool isalpha_constexpr(const char c) noexcept
{
    const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz";
    const char* letter = ::std::begin(alphabet);
    while(*letter != '\0' && *letter != c)
        ++letter;
    if (*letter)
        return true;
    return false;
}

inline constexpr void swap_chars_ranges(char* __restrict first1,
                                        char* __restrict last1,
                                        char* __restrict first2) noexcept
{
    const size_t n = last1 - first1;
    for ( size_t i = 0; i < n; ++i )
    {
        const char tmp = first1[i];
        first1[i]      = first2[i];
        first2[i]      = tmp;
    }
}

} // namespace nh3api