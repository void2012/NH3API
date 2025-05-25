//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

/*
 * This file is derived from software bearing the following
 * restrictions:
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Hewlett-Packard Company makes no representations about the
 * suitability of this software for any purpose. It is provided
 * "as is" without express or implied warranty.
 */
#pragma once

#include <string>        // std::char_traits, and optionally std::basic_string
#include <limits>        // std::numeric_limits
#include <utility>       // std::swap, std::forward
#ifdef __cpp_lib_ranges 
#include <ranges> // std::ranges::enable_view, std::ranges::enable_borrowed_range
#endif
#include "algorithm.hpp" // constexpr algorithms
#include "intrin.hpp"    // constexpr string functions
#include "math.hpp"      // nh3api::fpclassify, nh3api::count_digits, float classification macros
#include "memory.hpp"    // allocators
#include "iterator.hpp"  // std::reverse_iterator, tt::is_iterator
#include "nh3api_exceptions.hpp" // exceptions
#if NH3API_MSVC_STL_VERSION > NH3API_MSVC_STL_VERSION_2010 || NH3API_CHECK_CPP11
#include "hash.hpp" // nh3api::default_hash
#endif

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)
NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

template
<
typename CharT,
typename CharTraits = std::char_traits<CharT>,
typename Allocator = exe_allocator<CharT>
>
struct exe_string_helper
{
    public:
        typedef Allocator allocator_type;
        typedef CharTraits traits_type;

    #if NH3API_CHECK_CPP11
    protected:
        typedef std::allocator_traits<allocator_type> alloc_traits;

    public:
        typedef typename alloc_traits::size_type       size_type;
        typedef typename alloc_traits::difference_type difference_type;
        typedef typename traits_type::char_type        value_type;
        typedef typename traits_type::char_type        char_type;
        typedef typename traits_type::int_type         int_type;
        typedef typename traits_type::off_type         off_type;
        typedef typename traits_type::state_type       state_type;
        typedef value_type&                            reference;
        typedef const value_type&                      const_reference;
        typedef typename alloc_traits::pointer         pointer;
        typedef typename alloc_traits::const_pointer   const_pointer;

        typedef typename alloc_traits::propagate_on_container_copy_assignment propagate_on_container_copy_assignment;
        typedef typename alloc_traits::propagate_on_container_move_assignment propagate_on_container_move_assignment;
        typedef typename alloc_traits::propagate_on_container_swap propagate_on_container_swap;
        typedef typename alloc_traits::is_always_equal is_always_equal;

    #else
        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;
        typedef typename traits_type::char_type          value_type;
        typedef typename traits_type::char_type          char_type;
        typedef typename traits_type::int_type           int_type;
        typedef typename traits_type::off_type           off_type;
        typedef typename traits_type::state_type         state_type;
        typedef value_type&                              reference;
        typedef const value_type&                        const_reference;
        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;

        typedef nh3api::tt::false_type propagate_on_container_copy_assignment;
        typedef nh3api::tt::false_type propagate_on_container_move_assignment;
        typedef nh3api::tt::false_type propagate_on_container_swap;
        typedef nh3api::tt::is_empty<allocator_type> is_always_equal;
    #endif

    public:
        NH3API_CONSTEXPR_CPP_20
        exe_string_helper()
            NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_default_constructible<allocator_type>::value)
            : alloc()
        {}

        NH3API_CONSTEXPR_CPP_20
        explicit exe_string_helper(const allocator_type& allocator)
            : alloc(allocator)
        {}

        #if NH3API_STD_MOVE_SEMANTICS
        NH3API_CONSTEXPR_CPP_20
        exe_string_helper(allocator_type&& other)
            : alloc(std::move(other))
        {}

        NH3API_CONSTEXPR_CPP_20
        exe_string_helper(exe_string_helper&& other)
            : alloc(std::move(other.alloc))
        {}

        exe_string_helper& operator=(exe_string_helper&& other)
        {
            this->alloc = std::move(other.alloc);
            return *this;
        }
        #endif

        NH3API_CONSTEXPR_CPP_20
        allocator_type select_on_container_copy_construction() const
        {
            #if NH3API_CHECK_CPP11
            return alloc_traits::select_on_container_copy_construction(alloc);
            #else
            return alloc;
            #endif
        }

        NH3API_CONSTEXPR
        exe_string_helper(const nh3api::dummy_tag_t& tag)
        { NH3API_IGNORE(alloc); }

    // string operations
    public:
        static NH3API_CONSTEXPR_CPP_17
        void assign(char_type& c1, const char_type& c2) NH3API_NOEXCEPT
        { traits_type::assign(c1, c1); }

        static NH3API_CONSTEXPR_CPP_17
        bool eq(char_type c1, char_type c2) NH3API_NOEXCEPT
        { return traits_type::eq(c1, c1); }

        static NH3API_CONSTEXPR_CPP_20
        bool lt(char_type c1, char_type c2) NH3API_NOEXCEPT
        { return traits_type::eq(c1, c1); }

        static NH3API_CONSTEXPR_CPP_17
        size_t length(const char_type* str)
        { return traits_type::length(str); }

        static NH3API_CONSTEXPR_CPP_17
        int_type compare(const char_type* left, const char_type* right, size_t count) NH3API_NOEXCEPT
        { return traits_type::compare(left, right, count); }

        static NH3API_CONSTEXPR_CPP_20
        char_type* move(char_type* dst, const char_type* src, size_t n) NH3API_NOEXCEPT
        { return traits_type::move(dst, src, n); }

        static NH3API_CONSTEXPR_CPP_20
        char_type* copy(char_type* dst, const char_type* src, size_t n) NH3API_NOEXCEPT
        { return traits_type::copy(dst, src, n); }

        static NH3API_CONSTEXPR_CPP_20
        char_type* assign(char_type* str, size_t n, char_type value) NH3API_NOEXCEPT
        { return traits_type::assign(str, n, value); }

        static NH3API_CONSTEXPR_CPP_17
        const char_type* find(const char_type* ptr, size_t count, const char_type& ch)
        { return traits_type::find(ptr, count, ch); }

    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        size_type max_size() const NH3API_NOEXCEPT
        {
            #if NH3API_CHECK_CPP11
            return alloc_traits::max_size(alloc);
            #else
            return alloc.max_size();
            #endif
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        pointer allocate(size_type num, const void* ptr)
        {
            return alloc.allocate(num, ptr);
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        pointer allocate(size_type num)
        {
            return alloc.allocate(num, nullptr);
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_20
        void deallocate(void* ptr, size_type num)
        NH3API_NOEXCEPT
        {
            alloc.deallocate(ptr, num);
        }

    public:
        allocator_type& alloc;
};

template<>
struct exe_string_helper<char, std::char_traits<char>, exe_allocator<char> >
{
    public:
        typedef exe_allocator<char>    allocator_type;
        typedef std::char_traits<char> traits_type;
        typedef size_t                 size_type;
        typedef ptrdiff_t              difference_type;
        typedef char                   char_type;
        typedef char_type              value_type;
        typedef value_type*            pointer;
        typedef const value_type*      const_pointer;
        typedef value_type&            reference;
        typedef const value_type&      const_reference;

        typedef nh3api::tt::false_type propagate_on_container_copy_assignment;
        typedef nh3api::tt::false_type propagate_on_container_move_assignment;
        typedef nh3api::tt::false_type propagate_on_container_swap;
        typedef nh3api::tt::true_type is_always_equal;

    public:
        NH3API_CONSTEXPR
        exe_string_helper() NH3API_NOEXCEPT
            : alloc()
        {}

        NH3API_CONSTEXPR
        exe_string_helper(const allocator_type& allocator) NH3API_NOEXCEPT
            : alloc(allocator)
        {}

        #if NH3API_STD_MOVE_SEMANTICS
        NH3API_CONSTEXPR
        exe_string_helper(allocator_type&& other) NH3API_NOEXCEPT
            : alloc(std::move(other))
        {}

        NH3API_CONSTEXPR
        exe_string_helper(exe_string_helper&& other) NH3API_NOEXCEPT
            : alloc(std::move(other.alloc))
        {}

        exe_string_helper& operator=(exe_string_helper&& other) NH3API_NOEXCEPT
        {
            return *this;
        }
        #endif

        NH3API_CONSTEXPR
        allocator_type select_on_container_copy_construction() const
        { return alloc; }

        NH3API_CONSTEXPR
        exe_string_helper(const nh3api::dummy_tag_t& tag)
        { NH3API_IGNORE(alloc); }

    // string operations
    public:
        static NH3API_CONSTEXPR
        void assign(char& c1, const char& c2) NH3API_NOEXCEPT
        { c1 = c2; }

        static NH3API_CONSTEXPR
        bool eq(char c1, char c2) NH3API_NOEXCEPT
        { return c1 == c2; }

        static NH3API_CONSTEXPR
        bool lt(char c1, char c2) NH3API_NOEXCEPT
        { return (unsigned char)c1 < (unsigned char)c2; }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        size_t length(const char* str)
        {
            #if NH3API_CHECK_CPP14
            return nh3api::strlen_constexpr<char>(str);
            #else
            return nh3api::str_func_chooser<char>::_strlen(str);
            #endif
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        int compare(const char* left, const char* right, size_t count) NH3API_NOEXCEPT
        {
            #if NH3API_CHECK_CPP14
            return nh3api::memcmp_constexpr<char>(left, right, count);
            #else
            return nh3api::str_func_chooser<char>::_memcmp(left, right, count);
            #endif
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        char* move(char* dst, const char* src, size_t n) NH3API_NOEXCEPT
        {
            #if NH3API_CHECK_CPP14
            return nh3api::memmove_constexpr<char, const char>(dst, src, n);
            #else
            return nh3api::str_func_chooser<char>::_memmove(dst, src, n);
            #endif
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        char* copy(char* dst, const char* src, size_t n) NH3API_NOEXCEPT
        {
            #if NH3API_CHECK_CPP14
            return nh3api::memmove_constexpr<char, const char>(dst, src, n);
            #else
            return nh3api::str_func_chooser<char>::_memmove(dst, src, n);
            #endif
        }

        static NH3API_CONSTEXPR_CPP_14
        char* assign(char* str, size_t n, char value) NH3API_NOEXCEPT
        {
            return nh3api::fill_n_constexpr(str, n, value);
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        const char* find(const char* ptr, size_t count, const char& ch)
        {
            if ( count == 0 )
                return nullptr;
            #if NH3API_CHECK_CPP14
            return nh3api::memchr_constexpr<char>(ptr, ch, count);
            #else
            return nh3api::str_func_chooser<char>::_memchr(ptr, ch, count);
            #endif
        }

    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        static size_t max_size() NH3API_NOEXCEPT
        {
            return (size_t)(-1);
        }

        NH3API_FORCEINLINE
        static pointer allocate(size_t num, const void*)
        NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
        {
            #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
                return (char*)::operator new(num, exe_heap);
            #else
                return (char*)::operator new(num, exe_heap, std::nothrow);
            #endif
        }

        NH3API_FORCEINLINE
        static pointer allocate(size_t num)
        NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
        {
            #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
                return (char*)::operator new(num, exe_heap);
            #else
                return (char*)::operator new(num, exe_heap, std::nothrow);
            #endif
        }

        NH3API_FORCEINLINE
        static void deallocate(void* ptr, size_t)
        NH3API_NOEXCEPT
        { exe_delete(ptr); }

    public:
        allocator_type alloc;
};

template<>
struct exe_string_helper<wchar_t, std::char_traits<wchar_t>, exe_allocator<wchar_t> >
{
    public:
        typedef exe_allocator<wchar_t>         allocator_type;
        typedef std::char_traits<wchar_t>      traits_type;
        typedef size_t                         size_type;
        typedef ptrdiff_t                      difference_type;
        typedef wchar_t                        char_type;
        typedef char_type                      value_type;
        typedef value_type*                    pointer;
        typedef const value_type*              const_pointer;
        typedef value_type&                    reference;
        typedef const value_type&              const_reference;

        typedef nh3api::tt::false_type propagate_on_container_copy_assignment;
        typedef nh3api::tt::false_type propagate_on_container_move_assignment;
        typedef nh3api::tt::false_type propagate_on_container_swap;
        typedef nh3api::tt::true_type is_always_equal;

    public:
        NH3API_CONSTEXPR
        exe_string_helper() NH3API_NOEXCEPT
            : alloc()
        {}

        NH3API_CONSTEXPR
        exe_string_helper(const allocator_type& allocator) NH3API_NOEXCEPT
            : alloc(allocator)
        {}

        #if NH3API_STD_MOVE_SEMANTICS
        NH3API_CONSTEXPR
        exe_string_helper(allocator_type&& other) NH3API_NOEXCEPT
            : alloc(std::move(other))
        {}

        NH3API_CONSTEXPR
        exe_string_helper(exe_string_helper&& other) NH3API_NOEXCEPT
            : alloc(std::move(other.alloc))
        {}

        exe_string_helper& operator=(exe_string_helper&& other) NH3API_NOEXCEPT
        {
            return *this;
        }
        #endif

        NH3API_CONSTEXPR
        allocator_type select_on_container_copy_construction() const
        { return alloc; }

        NH3API_CONSTEXPR
        exe_string_helper(const nh3api::dummy_tag_t& tag)
        { NH3API_IGNORE(alloc); }

    // string operations
    public:
        static NH3API_CONSTEXPR
        void assign(wchar_t& c1, const wchar_t& c2) NH3API_NOEXCEPT
        { c1 = c2; }

        static NH3API_CONSTEXPR
        bool eq(wchar_t c1, wchar_t c2) NH3API_NOEXCEPT
        { return c1 == c2; }

        static NH3API_CONSTEXPR
        bool lt(wchar_t c1, wchar_t c2) NH3API_NOEXCEPT
        { return (wint_t)c1 < (wint_t)c2; }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        size_t length(const wchar_t* str)
        {
            #if NH3API_CHECK_CPP14
            return nh3api::strlen_constexpr<wchar_t>(str);
            #else
            return nh3api::str_func_chooser<wchar_t>::_strlen(str);
            #endif
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        int compare(const wchar_t* left, const wchar_t* right, size_t count) NH3API_NOEXCEPT
        {
            #if NH3API_CHECK_CPP14
            return nh3api::memcmp_constexpr<wchar_t>(left, right, count);
            #else
            return nh3api::str_func_chooser<wchar_t>::_memcmp(left, right, count);
            #endif
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        wchar_t* move(wchar_t* dst, const wchar_t* src, size_t n) NH3API_NOEXCEPT
        {
            #if NH3API_CHECK_CPP14
            return nh3api::memmove_constexpr<wchar_t, const wchar_t>(dst, src, n);
            #else
            return nh3api::str_func_chooser<wchar_t>::_memmove(dst, src, n);
            #endif
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        wchar_t* copy(wchar_t* dst, const wchar_t* src, size_t n) NH3API_NOEXCEPT
        {
            #if NH3API_CHECK_CPP14
            return nh3api::memmove_constexpr<wchar_t, const wchar_t>(dst, src, n);
            #else
            return nh3api::str_func_chooser<wchar_t>::_memmove(dst, src, n);
            #endif
        }

        static NH3API_CONSTEXPR_CPP_14
        wchar_t* assign(wchar_t* str, size_t n, wchar_t value) NH3API_NOEXCEPT
        {
            return nh3api::fill_n_constexpr(str, n, value);
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        const wchar_t* find(const wchar_t* ptr, size_t count, const wchar_t& ch)
        {
            if ( count == 0 )
                return nullptr;
            #if NH3API_CHECK_CPP14
            return nh3api::memchr_constexpr<wchar_t>(ptr, ch, count);
            #else
            return nh3api::str_func_chooser<wchar_t>::_memchr(ptr, ch, count);
            #endif
        }

    public:
        NH3API_FORCEINLINE NH3API_CONSTEXPR
        static size_t max_size() NH3API_NOEXCEPT
        {
            return (size_t)(-1) / 2;
        }

        NH3API_FORCEINLINE
        static pointer allocate(size_t num, const void*)
        NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
        {
            #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
                return (wchar_t*)::operator new(num * sizeof(wchar_t), exe_heap);
            #else
                return (wchar_t*)::operator new(num * sizeof(wchar_t), exe_heap, std::nothrow);
            #endif
        }

        NH3API_FORCEINLINE
        static pointer allocate(size_t num)
        NH3API_NOEXCEPT_EXPR(nh3api::flags::no_exceptions)
        {
            #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
                return (wchar_t*)::operator new(num * sizeof(wchar_t), exe_heap);
            #else
                return (wchar_t*)::operator new(num * sizeof(wchar_t), exe_heap, std::nothrow);
            #endif
        }

        NH3API_FORCEINLINE
        static void deallocate(void* ptr, size_t)
        NH3API_NOEXCEPT
        { exe_delete(ptr); }

    public:
        allocator_type alloc;
};

// Implementation from libc++
namespace nh3api 
{

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_find(const CharT* str, SizeT size, CharT c, SizeT pos) NH3API_NOEXCEPT
{
    if ( pos >= size )
        return npos;
    const CharT* found = ::exe_string_helper<CharT, CharTraits>::find(str + pos, size - pos, c);
    if ( found == nullptr )
        return npos;
    return static_cast<SizeT>(found - str);
}

template <class CharT, class CharTraits>
NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_14 const CharT* 
search_substring(const CharT* first1, 
                 const CharT* last1, 
                 const CharT* first2, 
                 const CharT* last2) NH3API_NOEXCEPT 
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
    CharT f2 = *first2;
    while (true) 
    {
        len1 = last1 - first1;
        // Check whether first1 still has at least len2 bytes.
        if (len1 < len2)
            return last1;

        // Find f2 the first byte matching in first1.
        first1 = ::exe_string_helper<CharT, CharTraits>::find(first1, len1 - len2 + 1, f2);
        if (first1 == nullptr)
            return last1;

        // It is faster to compare from the first byte of first1 even if we
        // already know that it matches the first byte of first2: this is because
        // first2 is most likely aligned, as it is user's "pattern" string, and
        // first1 + 1 is most likely not aligned, as the match is in the middle of
        // the string.
        if (::exe_string_helper<CharT, CharTraits>::compare(first1, first2, len2) == 0)
            return first1;

        ++first1;
    }
}

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_find(const CharT* str, SizeT size, const CharT* needle, SizeT pos, SizeT n) NH3API_NOEXCEPT 
{
    if (pos > size)
        return npos;

    if (n == 0) // There is nothing to search, just return pos.
        return pos;

    const CharT* found = search_substring<CharT>(str + pos, str + size, needle, needle + n);

    if (found == str + size)
        return npos;
    return static_cast<SizeT>(found - str);
}

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_rfind(const CharT* str, SizeT size, CharT c, SizeT pos) NH3API_NOEXCEPT 
{
    if (size < 1)
        return npos;

    if (pos < size)
        ++pos;
    else
        pos = size;

    for (const CharT* i = str + pos; i != str;) 
    {
        if (::exe_string_helper<CharT, CharTraits>::eq(*--i, c))
            return static_cast<SizeT>(i - str);
    }
    return npos;
}

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_rfind(const CharT* str, SizeT size, const CharT* needle, SizeT pos, SizeT n) NH3API_NOEXCEPT 
{
    pos = std::min<SizeT>(pos, size);
    if (n < size - pos)
        pos += n;
    else
        pos = size;
  
    if (n == 0) 
        return (pos <= size) ? pos : size;

    if (size == 0 || n > size)
        return npos;

    for (SizeT i = pos - n; ; --i) 
    {
        bool match = true;
        for (SizeT j = 0; j < n; ++j) 
        {
            if ( !::exe_string_helper<CharT, CharTraits>::eq(str[i + j], needle[j]) ) 
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

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_find_first_of(const CharT* str, 
                  SizeT size, 
                  const CharT* 
                  needle, 
                  SizeT pos, 
                  SizeT n) NH3API_NOEXCEPT 
{
    if (pos >= size || n == 0)
        return npos;
    const CharT* found = find_first_of_constexpr(str + pos, 
                                                 str + size, 
                                                 needle, 
                                                 needle + n, 
                                                 ::exe_string_helper<CharT, CharTraits>::eq);
    if (found == str + size)
        return npos;
    return static_cast<SizeT>(found - str);
}

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_find_last_of(const CharT* str, SizeT size, const CharT* needle, SizeT pos, SizeT n) NH3API_NOEXCEPT 
{
    if (n != 0) 
    {
        if (pos < size)
            ++pos;
        else
            pos = size;
        for (const CharT* curr = str + pos; curr != str;) 
        {
            const CharT* found = ::exe_string_helper<CharT, CharTraits>::find(needle, n, *--curr);
            if (found)
                return static_cast<SizeT>(curr - str);
        }
    }
    return npos;
}

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_find_first_not_of(const CharT* str, SizeT size, const CharT* needle, SizeT pos, SizeT n) NH3API_NOEXCEPT 
{
    if (pos < size) 
    {
        const CharT* shifted = str + size;
        for (const CharT* curr = str + pos; curr != shifted; ++curr)
            if (::exe_string_helper<CharT, CharTraits>::find(needle, n, *curr) == nullptr)
                return static_cast<SizeT>(curr - str);
    }
  return npos;
}

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_find_first_not_of(const CharT* str, SizeT size, CharT c, SizeT pos) NH3API_NOEXCEPT 
{
    if (pos < size) 
    {
        const CharT* shifted = str + size;
        for (const CharT* curr = str + pos; curr != shifted; ++curr)
            if (!::exe_string_helper<CharT, CharTraits>::eq(*curr, c))
                return static_cast<SizeT>(curr - str);
    }
    return npos;
}

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_find_last_not_of(const CharT* str, SizeT size, const CharT* needle, SizeT pos, SizeT n) NH3API_NOEXCEPT 
{
    if (pos < size)
        ++pos;
    else
        pos = size;
    for (const CharT* curr = str + pos; curr != str;)
        if (::exe_string_helper<CharT, CharTraits>::find(needle, n, *--curr) == nullptr)
            return static_cast<SizeT>(curr - str);
    return npos;
}

template <class CharT, class SizeT, SizeT npos, class CharTraits>
NH3API_FORCEINLINE SizeT NH3API_CONSTEXPR_CPP_14 
str_find_last_not_of(const CharT* str, SizeT size, CharT c, SizeT pos) NH3API_NOEXCEPT 
{
    if (pos < size)
        ++pos;
    else
        pos = size;
    for (const CharT* curr = str + pos; curr != str;)
        if (!::exe_string_helper<CharT, CharTraits>::eq(*--curr, c))
            return static_cast<SizeT>(curr - str);
    return npos;
}

template<typename CharT = char, typename CharTraits = std::char_traits<char> >
class basic_string_view
{
    public:
        typedef CharTraits    traits_type;
        typedef CharT         value_type;
        typedef CharT*        pointer;
        typedef CharT const*  const_pointer;
        typedef CharT&        reference;
        typedef CharT const&  const_reference;
        typedef const_pointer iterator;
        typedef const_pointer const_iterator;
        typedef std::reverse_iterator<const_iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef size_t        size_type;
        typedef ptrdiff_t     difference_type;
    
    protected:
        typedef ::exe_string_helper<CharT, CharTraits> helper_type;

    // constructors
    public:
        NH3API_CONSTEXPR basic_string_view() NH3API_NOEXCEPT
            : _data(nullptr), _size(0)
        {}

        #if NH3API_CHECK_CPP11
        NH3API_CONSTEXPR basic_string_view(const basic_string_view& other) NH3API_NOEXCEPT = default;
        #else 
        basic_string_view(const basic_string_view& other) NH3API_NOEXCEPT
            : _data(other._data), _size(other._size)
        {}
        #endif

        NH3API_CONSTEXPR basic_string_view(const CharT* str, size_type count) NH3API_NOEXCEPT
            : _data(str), _size(count)
        {}

        NH3API_CONSTEXPR_CPP_14 basic_string_view(const CharT* str) NH3API_NOEXCEPT
            : _data(str), _size(helper_type::length(str))
        {}

        #if NH3API_CHECK_CPP11
        basic_string_view(std::nullptr_t) = delete;
        #endif

        #if NH3API_CHECK_CPP20
        template <std::contiguous_iterator IterT, std::sized_sentinel_for<IterT> EndIterT>
            requires(std::is_same_v<std::iter_value_t<IterT>, CharT> && !std::is_convertible_v<EndIterT, size_type>)
        constexpr basic_string_view(IterT first, EndIterT last)
            : _data(std::to_address(first)), _size(last - first)
        { nh3api::verify_range(first, last); }
        #endif

        #if NH3API_CHECK_CPP23 
        template <class RangeT>
            requires(!std::is_same_v<std::remove_cvref_t<RangeT>, basic_string_view> && std::ranges::contiguous_range<RangeT> &&
                    std::ranges::sized_range<RangeT> && std::is_same_v<std::ranges::range_value_t<RangeT>, CharT> &&
                    !std::is_convertible_v<RangeT, const CharT*> &&
                    (!requires(std::remove_cvref_t<RangeT>& d) { d.operator basic_string_view<CharT, CharTraits>(); }))
        constexpr explicit basic_string_view(RangeT&& range)
            : _data(std::ranges::data(range)), _size(std::ranges::size(range)) {}
        #endif

    // assignment
    public:
        #if NH3API_CHECK_CPP11
        NH3API_CONSTEXPR_CPP_14 basic_string_view& operator=(const basic_string_view& other) NH3API_NOEXCEPT = default;
        #else 
        basic_string_view& operator=(const basic_string_view& other) NH3API_NOEXCEPT
        {
            _data = other._data;
            _size = other._size;
            return *this;
        }
        #endif
    
    // access
    public:
        NH3API_CONSTEXPR const_iterator begin() const NH3API_NOEXCEPT 
        { return _data; }
        
        NH3API_CONSTEXPR const_iterator end() const NH3API_NOEXCEPT 
        { return _data + _size; }

        NH3API_CONSTEXPR const_iterator cbegin() const NH3API_NOEXCEPT 
        { return begin(); }

        NH3API_CONSTEXPR const_iterator cend() const NH3API_NOEXCEPT 
        { return end(); }

        NH3API_CONSTEXPR const_reverse_iterator rbegin() const NH3API_NOEXCEPT 
        { return const_reverse_iterator( end() ); }

        NH3API_CONSTEXPR const_reverse_iterator rend() const NH3API_NOEXCEPT 
        { return const_reverse_iterator( begin() ); }

        NH3API_CONSTEXPR const_reverse_iterator crbegin() const NH3API_NOEXCEPT 
        { return rbegin(); }

        NH3API_CONSTEXPR const_reverse_iterator crend() const NH3API_NOEXCEPT 
        { return rend(); }

        NH3API_CONSTEXPR bool empty() const NH3API_NOEXCEPT 
        { return _size == 0; }

        NH3API_CONSTEXPR size_type size() const NH3API_NOEXCEPT 
        { return _size; }

        NH3API_CONSTEXPR size_type length() const NH3API_NOEXCEPT 
        { return _size; }
        
        NH3API_CONSTEXPR size_type max_size() const NH3API_NOEXCEPT 
        { return (std::numeric_limits<size_type>::max)(); }

        NH3API_CONSTEXPR const_reference operator[]( size_type pos ) const NH3API_NOEXCEPT
        { return _data[pos]; }

        NH3API_CONSTEXPR_CPP_14 const_reference at( size_type pos ) const
        {
            if ( pos >= size() )
            {
                NH3API_THROW(std::out_of_range, "string_view::at out of range");
            }
            return _data[pos];
        }

        NH3API_CONSTEXPR const_reference front() const NH3API_NOEXCEPT 
        { return _data[0]; }

        NH3API_CONSTEXPR const_reference back() const NH3API_NOEXCEPT 
        { return _data[size() - 1]; }

        NH3API_CONSTEXPR const_pointer data() const NH3API_NOEXCEPT 
        { return _data; }

        NH3API_CONSTEXPR_CPP_14 void remove_prefix(size_type n) NH3API_NOEXCEPT
        {
            assert(n <= size());
            _data += n;
            _size -= n;
        }

        NH3API_CONSTEXPR_CPP_14 void remove_suffix( size_type n ) NH3API_NOEXCEPT
        {
            assert(n <= size());
            _size -= n;
        }

        NH3API_CONSTEXPR_CPP_14 void swap( basic_string_view& other ) NH3API_NOEXCEPT
        {
            const basic_string_view tmp(other);
            other = *this;
            *this = tmp;
        }
        
        NH3API_CONSTEXPR_CPP_14
        size_type copy( CharT* dest, size_type n, size_type pos = 0 ) const
        {
            if ( pos > size() )
            {
                NH3API_THROW(std::out_of_range, "string_view::copy out of range");
            }
            const size_type rlen = std::min<size_type>( n, size() - pos );
            (void) helper_type::copy( dest, data() + pos, rlen );
            return rlen;
        }

        NH3API_CONSTEXPR_CPP_14
        basic_string_view substr( size_type pos = 0, size_type n = npos ) const
        {
            if ( pos > size() )
            {
                NH3API_THROW(std::out_of_range, "string_view::substr out of range");
            }

            return basic_string_view( data() + pos, std::min<size_type>( n, size() - pos ) );
        }

        NH3API_CONSTEXPR_CPP_14
        int compare(basic_string_view other) const NH3API_NOEXCEPT
        {
            int result = helper_type::compare(data(), other.data(), std::min<size_type>(size(), other.size()));
            if ( result == 0 )
                result = size() == other.size() ? 0 : (size() < other.size() ? -1 : 1);
            return result;
        }

        NH3API_CONSTEXPR_CPP_14 int compare( size_type pos1, size_type n1, basic_string_view other ) const 
        { return substr( pos1, n1 ).compare( other ); }

        NH3API_CONSTEXPR_CPP_14 int compare( size_type pos1, size_type n1, basic_string_view other, size_type pos2, size_type n2 ) const 
        { return substr( pos1, n1 ).compare( other.substr( pos2, n2 ) ); }

        NH3API_CONSTEXPR_CPP_14 int compare( CharT const* s ) const NH3API_NOEXCEPT
        { return compare( basic_string_view( s ) ); }

        NH3API_CONSTEXPR_CPP_14 int compare( size_type pos1, size_type n1, CharT const* s ) const 
        { return substr( pos1, n1 ).compare( basic_string_view( s ) ); }

        NH3API_CONSTEXPR_CPP_14 int compare( size_type pos1, size_type n1, CharT const* s, size_type n2 ) const
        { return substr( pos1, n1 ).compare( basic_string_view( s, n2 ) ); }

        NH3API_CONSTEXPR_CPP_14 bool starts_with( basic_string_view other ) const NH3API_NOEXCEPT
        { return size() >= other.size() && compare( 0, other.size(), other ) == 0; }

        NH3API_CONSTEXPR_CPP_14 bool starts_with( CharT c ) const NH3API_NOEXCEPT
        { return !empty() && helper_type::eq(front(), c); }

        NH3API_CONSTEXPR_CPP_14 bool starts_with( CharT const* str ) const NH3API_NOEXCEPT
        { return starts_with(basic_string_view(str)); }

        NH3API_CONSTEXPR_CPP_14 bool ends_with( basic_string_view other ) const NH3API_NOEXCEPT 
        { return size() >= other.size() && compare( size() - other.size(), npos, other ) == 0; }

        NH3API_CONSTEXPR_CPP_14 bool ends_with( CharT c ) const NH3API_NOEXCEPT
        { return !empty() && helper_type::eq(back(), c); }

        NH3API_CONSTEXPR_CPP_14 bool ends_with( CharT const* str ) const NH3API_NOEXCEPT
        { return starts_with(basic_string_view(str)); }

        NH3API_CONSTEXPR_CPP_14 size_type find( basic_string_view other, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( other._data != nullptr && other._size != 0 );
            return nh3api::str_find<CharT, size_type, npos, traits_type>(data(), size(), other.data(), pos, other.size());
        }

        NH3API_CONSTEXPR_CPP_14 size_type find( CharT c, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            return nh3api::str_find<CharT, size_type, npos, traits_type>(data(), size(), c, pos);
        }

        NH3API_CONSTEXPR_CPP_14 size_type find( const CharT* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::str_find<CharT, size_type, npos, traits_type>(data(), size(), str, pos, n);
        }

        NH3API_CONSTEXPR_CPP_14 size_type find( const CharT* str, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::str_find<CharT, size_type, npos, traits_type>(data(), size(), str, pos, helper_type::length(str));
        }

        NH3API_CONSTEXPR_CPP_14 size_type rfind( basic_string_view other, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( other._data != nullptr && other._size != 0 );
            return nh3api::str_rfind<CharT, size_type, npos, traits_type>(data(), size(), other.data(), pos, other.size());
        }

        NH3API_CONSTEXPR_CPP_14 size_type rfind( CharT c, size_type pos = 0 ) const NH3API_NOEXCEPT
        { return nh3api::str_rfind<CharT, size_type, npos, traits_type>(data(), size(), c, pos); }

        NH3API_CONSTEXPR_CPP_14 size_type rfind( const CharT* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::str_rfind<CharT, size_type, npos, traits_type>(data(), size(), str, pos, n);
        }

        NH3API_CONSTEXPR_CPP_14 size_type rfind( const CharT* str, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::str_rfind<CharT, size_type, npos, traits_type>(data(), size(), str, pos, helper_type::length(str));
        }

        NH3API_CONSTEXPR_CPP_14 size_type find_first_of( basic_string_view other, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( other._data != nullptr && other._size != 0 );
            return nh3api::str_find_first_of<CharT, size_type, npos, traits_type>(data(), size(), other.data(), pos, other.size());
        }

        NH3API_CONSTEXPR_CPP_14 size_type find_first_of( CharT c, size_type pos = 0 ) const NH3API_NOEXCEPT
        { return find(c, pos); }

        NH3API_CONSTEXPR_CPP_14 size_type find_first_of( const CharT* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::str_find_first_of<CharT, size_type, npos, traits_type>(data(), size(), str, pos, n);
        }

        NH3API_CONSTEXPR_CPP_14 size_type find_first_of( const CharT* str, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::str_find_first_of<CharT, size_type, npos, traits_type>(data(), size(), str, pos, helper_type::length(str));
        }

        NH3API_CONSTEXPR_CPP_14 size_type find_last_of( basic_string_view other, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( other._data != nullptr && other._size != 0 );
            return nh3api::str_find_last_of<CharT, size_type, npos, traits_type>(data(), size(), other.data(), pos, other.size());
        }

        NH3API_CONSTEXPR_CPP_14 size_type find_last_of( CharT c, size_type pos = 0 ) const NH3API_NOEXCEPT
        { return rfind(c, pos); }

        NH3API_CONSTEXPR_CPP_14 size_type find_last_of( const CharT* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::str_find_last_of<CharT, size_type, npos, traits_type>(data(), size(), str, pos, n);
        }

        NH3API_CONSTEXPR_CPP_14 size_type find_last_of( const CharT* str, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::str_find_last_of<CharT, size_type, npos, traits_type>(data(), size(), str, pos, helper_type::length(str));
        }

        NH3API_CONSTEXPR_CPP_14 size_type find_first_not_of( basic_string_view other, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( other._data != nullptr && other._size != 0 );
            return nh3api::str_find_first_not_of<CharT, size_type, npos, traits_type>(data(), size(), other.data(), pos, other.size());
        }

        NH3API_CONSTEXPR_CPP_14 size_type find_first_not_of( CharT c, size_type pos = 0 ) const NH3API_NOEXCEPT
        { return nh3api::str_find_first_not_of<CharT, size_type, npos, traits_type>(data(), size(), c, pos); }

        NH3API_CONSTEXPR_CPP_14 size_type find_first_not_of( const CharT* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::str_find_first_not_of<CharT, size_type, npos, traits_type>(data(), size(), str, pos, n);
        }

        NH3API_CONSTEXPR_CPP_14 size_type find_first_not_of( const CharT* str, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::str_find_first_not_of<CharT, size_type, npos, traits_type>(data(), size(), str, pos, helper_type::length(str));
        }

        NH3API_CONSTEXPR_CPP_14 bool contains(basic_string_view other) const NH3API_NOEXCEPT
        { return find(other) != npos; }

        NH3API_CONSTEXPR_CPP_14 bool contains(CharT c) const NH3API_NOEXCEPT
        { return find(c) != npos; }

        NH3API_CONSTEXPR_CPP_14 bool contains(const CharT* str) const
        { return find(str) != npos; }
    
    public:
        #if NH3API_STD_INLINE_VARIABLES
            static inline constexpr size_type npos = size_type(-1);
        #else 
            static const size_type npos;
        #endif

    protected:
        const_pointer   _data;
        size_type       _size;

};

typedef basic_string_view<char> string_view;
typedef basic_string_view<wchar_t> wstring_view;

} // namespace nh3api

#if !NH3API_STD_INLINE_VARIABLES
template<typename CharT, typename CharTraits>
const typename nh3api::basic_string_view<CharT, CharTraits>::size_type 
nh3api::basic_string_view<CharT, CharTraits>::npos 
= typename nh3api::basic_string_view<CharT, CharTraits>::size_type(-1);
#endif

template< class CharT, class CharTraits >
NH3API_CONSTEXPR bool operator== (
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

template< class CharT, class CharTraits >
NH3API_CONSTEXPR bool operator!= (
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return !( lhs == rhs ); }

template< class CharT, class CharTraits >
NH3API_CONSTEXPR bool operator< (
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) < 0; }

template< class CharT, class CharTraits >
NH3API_CONSTEXPR bool operator<= (
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) <= 0; }

template< class CharT, class CharTraits >
NH3API_CONSTEXPR bool operator> (
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) > 0; }

template< class CharT, class CharTraits >
NH3API_CONSTEXPR bool operator>= (
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) >= 0; }

#ifdef __cpp_user_defined_literals
namespace exe_string_literals
{
NH3API_CONSTEXPR nh3api::string_view operator""_exe_sv(const char* str, size_t len) NH3API_NOEXCEPT 
{ return nh3api::string_view{ str, len }; }

NH3API_CONSTEXPR nh3api::wstring_view operator""_exe_sv(const wchar_t* str, size_t len) NH3API_NOEXCEPT 
{ return nh3api::wstring_view{ str, len }; }   

}
#endif

#ifdef __cpp_lib_ranges
template<class CharT, class CharTraits>
inline constexpr bool std::ranges::enable_view<nh3api::basic_string_view<CharT, CharTraits>> = true;
template<class CharT, class CharTraits>
inline constexpr bool std::ranges::enable_borrowed_range<nh3api::basic_string_view<CharT, CharTraits>> = true;
#endif

#pragma pack(push, 4)

// Visual C++ 6.0 implementation of std::basic_string, 
// which uses unsafe copy-on-write semantics with 8-bit reference counting.
// Use only for binary compatibility with the program. 
// Consider using std::basic_string or exe_std_string/exe_std_wstring which use .exe allocator
template
<
typename CharT,
typename CharTraits = std::char_traits<CharT>,
typename Allocator = exe_allocator<CharT>
>
class exe_basic_string
{
    protected:
        typedef exe_basic_string<CharT, CharTraits, Allocator> this_type;
        template<class IterT>
            struct is_cptr : public nh3api::tt::integral_constant<bool,
                nh3api::tt::is_same<IterT, const CharT* const>::value
                || nh3api::tt::is_same<IterT, CharT* const>::value
                || nh3api::tt::is_same<IterT, const CharT*>::value
                || nh3api::tt::is_same<IterT, CharT*>::value>
            {};

    protected:
        typedef exe_string_helper<CharT, CharTraits, Allocator> helper_type NH3API_NODEBUG;
        typedef typename helper_type::propagate_on_container_copy_assignment propagate_on_container_copy_assignment;
        typedef typename helper_type::propagate_on_container_move_assignment propagate_on_container_move_assignment;
        typedef typename helper_type::propagate_on_container_swap propagate_on_container_swap;

    // external typedefs
    public:
        typedef Allocator allocator_type;
        typedef CharTraits traits_type;
        typedef typename helper_type::size_type       size_type;
        typedef typename helper_type::difference_type difference_type;
        typedef typename helper_type::reference       reference;
        typedef typename helper_type::const_reference const_reference;
        typedef typename helper_type::value_type      value_type;
        typedef typename helper_type::pointer         pointer;
        typedef typename helper_type::const_pointer   const_pointer;

        typedef pointer iterator;
        typedef const_pointer const_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef std::reverse_iterator<iterator>       reverse_iterator;

    // exceptions
    protected:
        #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
        NH3API_FORCEINLINE
        static void _throw_length_exception()
        {
            NH3API_IF_CONSTEXPR ( nh3api::tt::is_same<value_type, wchar_t>::value )
                NH3API_THROW(std::length_error, "wstring too long");
            else
                NH3API_THROW(std::length_error, "string too long");
        }
        #else
        NH3API_FORCEINLINE
        static void _throw_length_exception()
        {
            NH3API_THROW(0, 0);
        }
        #endif

        #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS 
        NH3API_FORCEINLINE
        static void _throw_out_of_range_exception()
        {
            NH3API_IF_CONSTEXPR ( nh3api::tt::is_same<value_type, wchar_t>::value )
                NH3API_THROW(std::out_of_range, "invalid wstring position");
            else
                NH3API_THROW(std::out_of_range, "invalid string position");
        }
        #else
        NH3API_FORCEINLINE
        static void _throw_out_of_range_exception()
        {
            NH3API_THROW(0, 0);
        }
        #endif

        // we use the exceptions function like this:
        // return _throw_xxx(), yyy;
        // to help compiler optimize branches

    public:
        explicit exe_basic_string( const allocator_type& _Al = allocator_type() )
        NH3API_NOEXCEPT_EXPR(nh3api::tt::is_nothrow_copy_constructible<allocator_type>::value)
        : helper(_Al), _Ptr(nullptr), _Len(0), _Res(0)
        {
        }

        exe_basic_string(const exe_basic_string& other)
            : helper( other.helper.select_on_container_copy_construction() ),
            _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(other, 0, npos);
        }

        exe_basic_string(const exe_basic_string& other, size_type pos, size_type _M,
                        const allocator_type& _Al = allocator_type() )
            : helper( _Al ), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign( other, pos, _M );
        }

        exe_basic_string( const value_type* _S, size_type _N,
                        const allocator_type& _Al = allocator_type() )
            : helper( _Al ), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(_S, _N);
        }

        exe_basic_string( const value_type* _S, const allocator_type& _Al = allocator_type() )
            : helper(_Al), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(_S);
        }

        exe_basic_string(size_type _N, value_type _C)
            : helper(), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(_N, _C);
        }

        exe_basic_string(size_type _N, value_type _C, const allocator_type& _Al)
            : helper(_Al), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(_N, _C);
        }

        template <class IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        exe_basic_string( IterT _F, IterT _L, const allocator_type& _Al = allocator_type()
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
            : helper(_Al), _Ptr(nullptr), _Len(0), _Res(0)
        {
            nh3api::verify_range(_F, _L);
            _Construct(nh3api::unfancy(_F),
                    nh3api::unfancy(_L),
                    nh3api::iter_category<IterT>());
        }

        protected:
        NH3API_FORCEINLINE
        void _Nullify() NH3API_NOEXCEPT
        {
            if ( nh3api::tt::is_empty<allocator_type>::value)
            {
                #if NH3API_CHECK_MSVC
                    *reinterpret_cast<uint32_t*>(&helper) = 0;
                    _Ptr = nullptr;
                    _Len = 0;
                    _Res = 0;
                #elif __has_builtin(__builtin_memset_inline)
                    __builtin_memset_inline(this, 0, sizeof(*this));
                #elif __has_builtin(__builtin_memset_chk)
                    __builtin_memset_chk(this, 0, sizeof(*this));
                #elif __has_builtin(__builtin_memset)
                    __builtin_memset(this, 0, sizeof(*this));
                #else
                    *reinterpret_cast<uint32_t*>(&helper) = 0;
                    _Ptr = nullptr;
                    _Len = 0;
                    _Res = 0;
                #endif
            }
            else
            {
                _Ptr = nullptr;
                _Len = 0;
                _Res = 0;
            }
        }

        #if NH3API_STD_MOVE_SEMANTICS
        NH3API_FORCEINLINE
        void _Move_nullify(exe_basic_string* other) NH3API_NOEXCEPT
        {
            this->helper = std::move(other->helper);
            this->_Ptr   = nh3api::exchange(other->_Ptr, nullptr);
            this->_Len   = nh3api::exchange(other->_Len, 0);
            this->_Res   = nh3api::exchange(other->_Res, 0);
        }
        #endif

        template <class IterT>
        void _Construct(IterT _F, IterT _L, std::input_iterator_tag)
        {
            NH3API_TRY
            {
                for (; _F != _L; ++_F)
                    push_back(static_cast<CharT>(*_F));
            }
            NH3API_CATCH(...)
            {
                _Tidy_deallocate();
                NH3API_RETHROW
            }
        }

        template <class IterT>
        void _Construct(IterT _F, IterT _L, std::forward_iterator_tag)
        {
            const size_type count = static_cast<size_type>(std::distance(_F, _L));
            reserve(count);
            _Construct(_F, _L, std::input_iterator_tag());
        }

        void _Construct(CharT* const _F, CharT* const _L, std::random_access_iterator_tag)
        {
            if (_F != _L)
                assign(_F, static_cast<size_type>(_L - _F));
        }

        void _Construct(const CharT* const _F, const CharT* const _L, std::random_access_iterator_tag)
        {
            if (_F != _L)
                assign(_F, static_cast<size_type>(_L - _F));
        }

        public:
        #if NH3API_STD_MOVE_SEMANTICS
        exe_basic_string(exe_basic_string&& other)
            NH3API_NOEXCEPT_EXPR(helper_type::is_always_equal::value)
            : helper(std::move(other.helper))
        {
            // assign(std::move(other));
            // I decided to copy 95% of assign(exe_basic_string &&other)
            // implementation to optimize for constructor

            if (this == &other)
            { // arguments are same, do nothing
                return;
            }
            else if (this->helper.alloc != other.helper.alloc)
            {
                // allocators are not same, we have to make a copy.
                size_type _Ns = other._Len | _MIN_SIZE;
                if (max_size() < _Ns)
                    _Ns = other._Len;
                // allocate: refcnt + string + null terminator = _Len + 2
                value_type *_S = helper.allocate(_Ns + 2, nullptr);
                this->_Ptr = _S + 1;
                // copy string and null terminator
                helper_type::copy(_Ptr, other._Ptr, other.size() + 1);
                unsigned char refcnt = other._Refcnt(other._Ptr);
                this->_Refcnt(this->_Ptr) = refcnt;
                this->_Len = other._Len;
                this->_Res = other._Res;
                other._Tidy();
            }
            else
            {
                _Move_nullify(&other);
            }
        }

        exe_basic_string& operator=(exe_basic_string&& other)
            NH3API_NOEXCEPT_EXPR(helper_type::is_always_equal::value)
        { return assign(std::forward<exe_basic_string>(other)); }

        NH3API_INLINE_LARGE
        exe_basic_string& assign(exe_basic_string&& other)
            NH3API_NOEXCEPT_EXPR(helper_type::is_always_equal::value)
        {
            if (this == &other)
            { // arguments are same, do nothing
                return *this;
            }
            else if ( this->helper.alloc != other.helper.alloc )
            {
                // allocators are not same, we have to make a copy.
                size_type _Ns = other._Len | _MIN_SIZE;
                if (max_size() < _Ns)
                    _Ns = other._Len;
                // allocate: refcnt + string + null terminator = _Len + 2
                value_type *_S = helper.allocate(_Ns + 2, nullptr);
                this->_Ptr = _S + 1;
                // copy string and null terminator
                helper_type::copy(_Ptr, other._Ptr, other.size() + 1);
                unsigned char refcnt = other._Refcnt(other._Ptr);
                this->_Refcnt(this->_Ptr) = refcnt;
                this->_Len = other._Len;
                this->_Res = other._Res;
                other._Tidy();
            }
            else
            {
                this->_Tidy_deallocate();
                _Move_nullify(&other);
            }
            return *this;
        }

        #endif

        exe_basic_string(const nh3api::dummy_tag_t& tag)
            : helper(tag)
        {
            NH3API_IGNORE(_Ptr, _Len, _Res);
        }

        NH3API_FLATTEN
        ~exe_basic_string()
        {
            _Tidy_deallocate();
        }

    protected:
        enum _Mref : unsigned char { _FROZEN = 255 };

    public:
        #if NH3API_STD_INLINE_VARIABLES
            static inline constexpr size_type npos = size_type(-1);
        #else 
            static const size_type npos;
        #endif
        exe_basic_string& operator=( const exe_basic_string& other )
        {
            return assign( other );
        }
        exe_basic_string& operator=( const value_type* _S )
        {
            return assign( _S );
        }
        exe_basic_string& operator=( value_type _C )
        {
            return assign( 1, _C );
        }

        #ifdef NH3API_FLAG_STD_CONTAINERS_RELATIONS
        // std::string assign
        template<typename OtherAllocator>
        exe_basic_string& operator=( const std::basic_string<value_type, OtherAllocator> & _S )
        {
            return assign( _S.c_str() );
        }
        #endif
        NH3API_INLINE_LARGE
        exe_basic_string& operator+=( const exe_basic_string& other )
        {
            return append( other );
        }
        NH3API_INLINE_LARGE
        exe_basic_string& operator+=( const value_type* _S )
        {
            return append( _S );
        }
        NH3API_INLINE_LARGE
        exe_basic_string& operator+=( value_type _C )
        {
            return append( 1, _C );
        }

        exe_basic_string& append( const exe_basic_string& other )
        {
            return append( other, 0, npos );
        }
        exe_basic_string &append(const exe_basic_string &other, size_type pos, size_type _M = npos)
        {
            if (other.size() < pos)
            {
                return _throw_out_of_range_exception(), *this;
            }
            size_type _N = other.size() - pos;
            if (_N < _M)
                _M = _N;
            if (npos - _Len <= _M)
            {
                return _throw_length_exception(), *this;
            }
            if (0 < _M && _Grow(_N = _Len + _M))
            {
                helper_type::copy(_Ptr + _Len, &other.c_str()[pos], _M);
                _Eos(_N);
            }
            return (*this);
        }
        exe_basic_string& append( const value_type* _S, size_type _M )
        {
            if (npos - _Len <= _M)
            {
                return _throw_length_exception(), *this;
            }
            size_type _N;
            if ( 0 < _M && _Grow( _N = _Len + _M ) )
            {
                helper_type::copy( _Ptr + _Len, _S, _M );
                _Eos( _N );
            }
            return (*this);
        }
        exe_basic_string& append( const value_type* _S )
        {
            return append(_S, helper_type::length(_S));
        }
        exe_basic_string& append( size_type _M, value_type _C )
        {
            if (npos - _Len <= _M)
            {
                return _throw_length_exception(), *this;
            }
            size_type _N;
            if (0 < _M && _Grow(_N = _Len + _M))
            {
                helper_type::assign(_Ptr + _Len, _M, _C);
                _Eos(_N);
            }
            return (*this);
        }

        template<class IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        exe_basic_string& append(IterT _F, IterT _L
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        {
            return replace(end(), end(), _F, _L);
        }

        void push_back(value_type _C)
        {
            append(1, _C);
        }

        exe_basic_string& assign( const exe_basic_string& other )
        {
            return (assign( other, 0, npos ));
        }
        NH3API_INLINE_LARGE
        exe_basic_string& assign( const exe_basic_string& other, size_type pos, size_type _M )
        {
            if ( other.size() < pos )
            {
                return _throw_out_of_range_exception(), *this;
            }
            size_type _N = other.size() - pos;
            if ( _M < _N )
                _N = _M;
            if ( this == &other )
            {
                erase(static_cast<size_type>(pos + _N));
                erase(0, pos);
            }
            else if ( 0 < _N && _N == other.size()
                    && _Refcnt( other.c_str() ) < _FROZEN - 1
                    && (helper.alloc == other.helper.alloc) )
            {
                _Tidy( true );
                _Ptr = other._Ptr;
                _Len = other.size();
                _Res = other.capacity();
                ++_Refcnt( _Ptr );
            }
            else if ( _Grow( _N, true ) )
            {
                helper_type::copy( _Ptr, &other.c_str()[pos], _N );
                _Eos( _N );
            }
            return (*this);
        }
        exe_basic_string& assign( const value_type* _S, size_type _N )
        {
            if ( _Grow( _N, true ) )
            {
                helper_type::copy( _Ptr, _S, _N );
                _Eos( _N );
            }
            return (*this);
        }
        exe_basic_string& assign( const value_type* _S )
        {
            return (assign( _S, helper_type::length( _S ) ));
        }
        exe_basic_string& assign( size_type _N, value_type _C )
        {
            if ( _N == npos )
            {
                return _throw_length_exception(), *this;
            }
            if ( _Grow( _N, true ) )
            {
                helper_type::assign( _Ptr, _N, _C );
                _Eos( _N );
            }
            return (*this);
        }
        template<typename IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        exe_basic_string& assign( IterT _F, IterT _L
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        { return replace( begin(), end(), _F, _L ); }

        exe_basic_string& insert( size_type _P0, const exe_basic_string& other )
        {
            return (insert( _P0, other, 0, npos ));
        }
        exe_basic_string& insert( size_type _P0, const exe_basic_string& other, size_type pos,
                    size_type _M )
        {
            if ( _Len < _P0 || other.size() < pos )
            {
                return _throw_out_of_range_exception(), *this;
            }
            size_type _N = other.size() - pos;
            if ( _N < _M )
                _M = _N;
            if (npos - _Len <= _M)
            {
                return _throw_length_exception(), *this;
            }
            if (0 < _M && _Grow(_N = _Len + _M))
            {
                helper_type::move( _Ptr + _P0 + _M, _Ptr + _P0, _Len - _P0 );
                helper_type::copy( _Ptr + _P0, &other.c_str()[pos], _M );
                _Eos( _N );
            }
            return (*this);
        }
        exe_basic_string& insert( size_type _P0, const value_type* _S, size_type _M )
        {
            if ( _Len < _P0 )
            {
                return _throw_out_of_range_exception(), *this;
            }
            if (npos - _Len <= _M)
            {
                return _throw_length_exception(), *this;
            }
            size_type _N;
            if ( 0 < _M && _Grow( _N = _Len + _M ) )
            {
                helper_type::move( _Ptr + _P0 + _M, _Ptr + _P0, _Len - _P0 );
                helper_type::copy( _Ptr + _P0, _S, _M );
                _Eos( _N );
            }
            return (*this);
        }
        exe_basic_string& insert( size_type _P0, const value_type* _S )
        {
            return (insert( _P0, _S, helper_type::length( _S ) ));
        }
        exe_basic_string& insert( size_type _P0, size_type _M, value_type _C )
        {
            if ( _Len < _P0 )
            {
                return _throw_out_of_range_exception(), *this;
            }
            if ( npos - _Len <= _M )
            {
                return _throw_length_exception(), *this;
            }
            size_type _N;
            if ( 0 < _M && _Grow( _N = _Len + _M ) )
            {
                helper_type::move( _Ptr + _P0 + _M, _Ptr + _P0, _Len - _P0 );
                helper_type::assign( _Ptr + _P0, _M, _C );
                _Eos( _N );
            }
            return (*this);
        }
        iterator insert(const const_iterator pos, const value_type _C )
        {
            size_type _P0 = _Pdif( pos, begin() );
            insert( _P0, 1, _C );
            return (begin() + _P0);
        }
        iterator insert(const const_iterator pos, const size_type _M, const value_type _C)
        {
            size_type _P0 = _Pdif( pos, begin() );
            insert( _P0, _M, _C );
            return begin() + _P0;
        }
        template<class IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        iterator insert( const const_iterator pos, IterT _F, IterT _L
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        {
            size_type _P0 = _Pdif( pos, begin() );
            replace(pos, pos, _F, _L);
            return begin() + _P0;
        }

        NH3API_FLATTEN
        void pop_back()
        { erase(end() - 1); }

        void shrink_to_fit()
        { exe_basic_string(*this).swap(*this); }

        NH3API_INLINE_LARGE
        exe_basic_string& erase( size_type _P0 = 0, size_type _M = npos )
        {
            if (_Len < _P0)
            {
                return _throw_out_of_range_exception(), *this;
            }
            _Split();
            if (_Len - _P0 < _M)
                _M = _Len - _P0;
            if (0 < _M)
            {
                helper_type::move(_Ptr + _P0, _Ptr + _P0 + _M, _Len - _P0 - _M);
                size_type _N = _Len - _M;
                if (_Grow(_N))
                    _Eos(_N);
            }
            return *this;
        }
        iterator erase( const const_iterator pos )
        {
            size_t _M = _Pdif( pos, begin() );
            erase( _M, 1 );
            return (_Psum( _Ptr, _M ));
        }
        iterator erase( const const_iterator _F, const const_iterator _L )
        {
            size_t _M = _Pdif( _F, begin() );
            erase( _M, _Pdif( _L, _F ) );
            return _Psum( _Ptr, _M );
        }

        void clear() NH3API_NOEXCEPT
        { _Eos(0); }

        exe_basic_string& replace(const size_type _P0, const size_type _N0, const exe_basic_string& other )
        {
            return replace(_P0, _N0, other, 0, npos);
        }

        NH3API_INLINE_LARGE
        exe_basic_string& replace(const size_type _P0, size_type _N0, const exe_basic_string& other,
                    const size_type pos, size_type _M = npos )
        {
            if (_Len < _P0 || other.size() < pos)
            {
                return _throw_out_of_range_exception(), *this;
            }
            if (_Len - _P0 < _N0)
                _N0 = _Len - _P0;
            size_type _N = other.size() - pos;
            if (_N < _M)
                _M = _N;
            if (npos - _M <= _Len - _N0)
            {
                return _throw_length_exception(), *this;
            }
            _Split();
            size_type _Nm = _Len - _N0 - _P0;
            if (_M < _N0)
                helper_type::move(_Ptr + _P0 + _M, _Ptr + _P0 + _N0, _Nm);
            if ((0 < _M || 0 < _N0) && _Grow(_N = _Len + _M - _N0))
            {
                if (_N0 < _M)
                    helper_type::move(_Ptr + _P0 + _M, _Ptr + _P0 + _N0, _Nm);
                helper_type::copy(_Ptr + _P0, &other.c_str()[pos], _M);
                _Eos(_N);
            }
            return *this;
        }

        NH3API_INLINE_LARGE
        exe_basic_string& replace(const size_type _P0, size_type _N0, const value_type* const _S,
                    const size_type _M )
        {
            if (_Len < _P0)
            {
                return _throw_out_of_range_exception(), *this;
            }
            if (_Len - _P0 < _N0)
                _N0 = _Len - _P0;
            if (npos - _M <= _Len - _N0)
            {
                return _throw_length_exception(), *this;
            }
            _Split();
            size_type _Nm = _Len - _N0 - _P0;
            if (_M < _N0)
                helper_type::move(_Ptr + _P0 + _M, _Ptr + _P0 + _N0, _Nm);
            size_type _N;
            if ((0 < _M || 0 < _N0) && _Grow(_N = _Len + _M - _N0))
            {
                if (_N0 < _M)
                    helper_type::move(_Ptr + _P0 + _M, _Ptr + _P0 + _N0, _Nm);
                helper_type::copy(_Ptr + _P0, _S, _M);
                _Eos(_N);
            }
            return (*this);
        }

        exe_basic_string& replace( size_type _P0, size_type _N0, const value_type* _S )
        {
            return (replace( _P0, _N0, _S, helper_type::length( _S ) ));
        }

        NH3API_INLINE_LARGE
        exe_basic_string& replace( size_type _P0, size_type _N0,
                    size_type _M, value_type _C )
        {
            if (_Len < _P0)
            {
                return _throw_out_of_range_exception(), *this;
            }
            if (_Len - _P0 < _N0)
                _N0 = _Len - _P0;
            if (npos - _M <= _Len - _N0)
            {
                return _throw_length_exception(), *this;
            }
            _Split();
            size_type _Nm = _Len - _N0 - _P0;
            if (_M < _N0)
                helper_type::move(_Ptr + _P0 + _M, _Ptr + _P0 + _N0, _Nm);
            size_type _N;
            if ((0 < _M || 0 < _N0) && _Grow(_N = _Len + _M - _N0))
            {
                if (_N0 < _M)
                    helper_type::move(_Ptr + _P0 + _M, _Ptr + _P0 + _N0, _Nm);
                helper_type::assign(_Ptr + _P0, _M, _C);
                _Eos(_N);
            }
            return *this;
        }

        exe_basic_string& replace(const const_iterator _F, const const_iterator _L, const exe_basic_string& other )
        {
            return replace(_Pdif( _F, begin() ), _Pdif( _L, _F ), other );
        }

        exe_basic_string& replace(const const_iterator _F,
                        const const_iterator _L,
                        const value_type* const _S,
                        size_type _M)
        {
            return replace(_Pdif( _F, begin() ), _Pdif( _L, _F ), _S, _M );
        }

        exe_basic_string& replace(const const_iterator _F, const const_iterator _L, const value_type* const _S)
        {
            return replace(_Pdif( _F, begin() ), _Pdif( _L, _F ), _S );
        }

        exe_basic_string& replace( const const_iterator _F,
                            const const_iterator _L,
                            const size_type _M,
                            const value_type _C )
        {
            return replace(_Pdif( _F, begin() ), _Pdif( _L, _F ), _M, _C );
        }

        template<typename IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        exe_basic_string& replace(const const_iterator _F1,
                        const const_iterator _L1,
                        const IterT _F2,
                        const IterT _L2
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        {
            #if NH3API_STD_MOVE_SEMANTICS
            nh3api::verify_range(_F2, _L2);
            const auto _UF2 = nh3api::unfancy(_F2);
            const auto _UL2 = nh3api::unfancy(_L2);
            return _ReplaceRange(_F1,
                                _L1,
                                _UF2,
                                _UL2,
                                is_cptr<decltype(_UF2)>());
            #else // Visual Studio 2005..2008 have no decltype()
            typedef typename _STD _Checked_iterator_base_helper<IterT>::_Checked_iterator_base_type
            unchecked_iterator_type;
            return _ReplaceRange(_F1,
                                _L1,
                                nh3api::unfancy(_F2),
                                nh3api::unfancy(_L2),
                                is_cptr<unchecked_iterator_type>());
            #endif
        }

        protected:
        template<class IterT>
        exe_basic_string& _ReplaceRange(const const_iterator _F1,
                                const const_iterator _L1,
                                const IterT _F2,
                                const IterT _L2,
                                nh3api::tt::true_type)
        {
            return replace(_Pdif(_F1, cbegin()), _Pdif(_L1, _F2), _F2, _Pdif(_L2, _F2));
        }

        template<class IterT>
        exe_basic_string& _ReplaceRange(const const_iterator _F1,
                                const const_iterator _L1,
                                const IterT _F2,
                                const IterT _L2,
                                nh3api::tt::false_type)
        {
            const exe_basic_string temp(_F2, _L2, get_allocator());
            replace(_F1, _L2, temp);
            return *this;
        }

    public:
        reference front()
        {
            _Freeze();
            return _Ptr[0];
        }

        const_reference front() const
        { return _Ptr[0]; }

        reference back() 
        {
            _Freeze();
            return _Ptr[_Len - 1];
        }

        const_reference back() const
        { return _Ptr[_Len - 1]; }

        iterator begin()
        {
            _Freeze();
            return (_Ptr);
        }
        const_iterator begin() const
        {
            return (_Ptr);
        }
        const_iterator cbegin() const
        {
            return (_Ptr);
        }
        iterator end()
        {
            _Freeze();
            return ((iterator)_Psum( _Ptr, _Len ));
        }
        const_iterator end() const
        {
            return ((const_iterator)_Psum( _Ptr, _Len ));
        }
        const_iterator cend() const
        {
            return ((const_iterator)_Psum( _Ptr, _Len ));
        }
        reverse_iterator rbegin()
        {
            return (reverse_iterator( end() ));
        }
        const_reverse_iterator rbegin() const
        {
            return (const_reverse_iterator( end() ));
        }
        const_reverse_iterator crbegin() const
        {
            return (const_reverse_iterator( end() ));
        }
        reverse_iterator rend()
        {
            return (reverse_iterator( begin() ));
        }
        const_reverse_iterator rend() const
        {
            return (const_reverse_iterator( begin() ));
        }
        const_reverse_iterator crend() const
        {
            return (const_reverse_iterator( begin() ));
        }
        reference at( size_type _P0 )
        {
            if ( _Len <= _P0 )
            {
                return _throw_out_of_range_exception(), _Ptr[_P0];
            }
            _Freeze();
            return _Ptr[_P0];
        }
        const_reference at( size_type _P0 ) const
        {
            if ( _Len <= _P0 )
            {
                return _throw_out_of_range_exception(), _Ptr[_P0];
            }
            return _Ptr[_P0];
        }
        reference operator[]( size_type _P0 )
        {
        #if !NH3API_DEBUG
            _Freeze();
            return (_Ptr[_P0]);
        #else
            return at(_P0);
        #endif
        }
        const_reference operator[]( size_type _P0 ) const
        {
        #if !NH3API_DEBUG
            return (_Ptr[_P0]);
        #else
            return at(_P0);
        #endif
        }
        const value_type* c_str() const
        {
            return _Ptr;
        }
        const value_type* data() const
        {
            return _Ptr;
        }
        value_type* data()
        {
            _Freeze();
            return _Ptr;
        }
        size_type length() const
        {
            return (_Len);
        }
        size_type size() const
        {
            return (_Len);
        }
        size_type max_size() const
        {
            const size_type _N = helper.max_size();
            return (_N <= 2 ? 1 : _N - 2);
        }

        void resize(const size_type _N, const value_type _C = value_type())
        {
            const size_type old_size = size();
            if (_N <= old_size)
            {
                _Eos(_N);
            }
            else
            {
                append(_N - old_size, _C);
            }
        }

        size_type capacity() const
        {
            return (_Res);
        }
        void reserve( size_type _N = 0 )
        {
            if ( _Res < _N )
                _Grow( _N );
        }
        bool empty() const
        {
            return (_Len == 0);
        }
        size_type copy( value_type* _S, size_type _N, size_type _P0 = 0 ) const
        {
            if ( _Len < _P0 )
            {
                return _throw_out_of_range_exception(), 0;
            }
            if ( _Len - _P0 < _N )
                _N = _Len - _P0;
            if ( 0 < _N )
                helper_type::copy( _S, _Ptr + _P0, _N );
            return (_N);
        }
        void swap( exe_basic_string& other )
        NH3API_NOEXCEPT_EXPR(helper_type::propagate_on_container_swap::value
                                || helper_type::is_always_equal::value)
        {
            if ( this->helper.alloc == other.helper.alloc )
            {
                std::swap( _Ptr, other._Ptr );
                std::swap( _Len, other._Len );
                std::swap( _Res, other._Res );
            }
            else
            {
                exe_basic_string _Ts = *this;
                *this = other;
                other = _Ts;
            }
        }

        bool contains(value_type c) const 
        {
            for ( size_type i = 0; i < _Len; ++i )
            {
                if ( helper_type::eq(_Ptr[i], c) )
                    return true;
            }
            return false;
        }

        bool contains(const_pointer str) const 
        { return find(str) != npos; }

        bool starts_with(value_type c) const 
        { return !empty() && helper_type::eq(front(), c); }

        bool ends_with(value_type c) const 
        { return !empty() && helper_type::eq(back(), c); }

        // TODO: Implement other overloads

        size_type find( const exe_basic_string& other, size_type pos = 0 ) const
        {
            return (find( other.c_str(), pos, other.size() ));
        }
        size_type find( const value_type* _S, size_type pos,
                        size_type _N ) const
        {
            if (!_S)
                return npos;

            if ( _N == 0 && pos <= _Len )
                return (pos);
            size_type _Nm;
            if ( pos < _Len && _N <= (_Nm = _Len - pos) )
            {
                const value_type* _U, * _V;
                for ( _Nm -= _N - 1, _V = _Ptr + pos;
                    (_U = helper_type::find( _V, _Nm, *_S )) != 0;
                    _Nm -= _U - _V + 1, _V = _U + 1 )
                    if ( helper_type::compare( _U, _S, _N ) == 0 )
                        return (_U - _Ptr);
            }
            return npos;
        }
        size_type find( const value_type* _S, size_type pos = 0 ) const
        {
            return (find( _S, pos, helper_type::length( _S ) ));
        }
        size_type find( value_type _C, size_type pos = 0 ) const
        {
            return (find( static_cast<const value_type*>(&_C), pos, 1));
        }
        size_type rfind( const exe_basic_string& other, size_type pos = npos ) const
        {
            return (rfind( other.c_str(), pos, other.size() ));
        }
        size_type rfind( const value_type* _S, size_type pos,
                        size_type _N ) const
        {
            if (_N == 0)
                return (pos < _Len ? pos : _Len);

            if (_N <= _Len)
            {
                for (const value_type *_U = _Ptr + +(pos < _Len - _N ? pos : _Len - _N);; --_U)
                {
                    if (helper_type::eq(*_U, *_S) && helper_type::compare(_U, _S, _N) == 0)
                        return (_U - _Ptr);
                    else if (_U == _Ptr)
                        break;
                }
            }
            return (npos);
        }
        size_type rfind( const value_type* _S, size_type pos = npos ) const
        {
            return (rfind( _S, pos, helper_type::length( _S ) ));
        }
        size_type rfind( value_type _C, size_type pos = npos ) const
        {
            return (rfind( static_cast<const value_type*>(&_C), pos, 1));
        }
        size_type find_first_of( const exe_basic_string& other,
                                size_type pos = 0 ) const
        {
            return (find_first_of( other.c_str(), pos, other.size() ));
        }
        size_type find_first_of( const value_type* _S, size_type pos,
                                size_type _N ) const
        {
            if ( 0 < _N && pos < _Len )
            {
                const value_type* const _V = _Ptr + _Len;
                for ( const value_type* _U = _Ptr + pos; _U < _V; ++_U )
                    if ( helper_type::find( _S, _N, *_U ) != 0 )
                        return (_U - _Ptr);
            }
            return (npos);
        }
        size_type find_first_of( const value_type* _S, size_type pos = 0 ) const
        {
            return (find_first_of( _S, pos, helper_type::length( _S ) ));
        }
        size_type find_first_of( value_type _C, size_type pos = 0 ) const
        {
            return (find( static_cast<const value_type*>(&_C), pos, 1));
        }
        size_type find_last_of( const exe_basic_string& other,
                                size_type pos = npos ) const
        {
            return (find_last_of( other.c_str(), pos, other.size() ));
        }
        size_type find_last_of( const value_type* _S, size_type pos,
                                size_type _N ) const
        {
            if (0 < _N && 0 < _Len)
            {
                for (const value_type *_U = _Ptr + (pos < _Len ? pos : _Len - 1);; --_U)
                {
                    if (helper_type::find(_S, _N, *_U) != 0)
                        return (_U - _Ptr);
                    else if (_U == _Ptr)
                        break;
                }
            }
            return (npos);
        }
        size_type find_last_of( const value_type* _S,
                                size_type pos = npos ) const
        {
            return (find_last_of( _S, pos, helper_type::length( _S ) ));
        }
        size_type find_last_of( value_type _C, size_type pos = npos ) const
        {
            return (rfind( static_cast<const value_type*>(&_C), pos, 1 ));
        }
        size_type find_first_not_of( const exe_basic_string& other,
                                    size_type pos = 0 ) const
        {
            return (find_first_not_of( other.c_str(), pos,
                    other.size() ));
        }
        size_type find_first_not_of( const value_type* _S, size_type pos,
                                    size_type _N ) const
        {
            if ( pos < _Len )
            {
                const value_type* const _V = _Ptr + _Len;
                for ( const value_type* _U = _Ptr + pos; _U < _V; ++_U )
                    if ( helper_type::find( _S, _N, *_U ) == 0 )
                        return (_U - _Ptr);
            }
            return (npos);
        }
        size_type find_first_not_of( const value_type* _S,
                                    size_type pos = 0 ) const
        {
            return (find_first_not_of( _S, pos, helper_type::length( _S ) ));
        }
        size_type find_first_not_of( value_type _C, size_type pos = 0 ) const
        {
            return (find_first_not_of( static_cast<const value_type*>(&_C), pos, 1 ));
        }
        size_type find_last_not_of( const exe_basic_string& other,
                                    size_type pos = npos ) const
        {
            return (find_last_not_of( other.c_str(), pos, other.size() ));
        }
        size_type find_last_not_of( const value_type* _S, size_type pos,
                                    size_type _N ) const
        {
            if (0 < _Len)
            {
                for (const value_type *_U = _Ptr + (pos < _Len ? pos : _Len - 1);; --_U)
                {
                    if (helper_type::find(_S, _N, *_U) == 0)
                        return (_U - _Ptr);
                    else if (_U == _Ptr)
                        break;
                }
            }
            return (npos);
        }
        size_type find_last_not_of( const value_type* _S,
                                    size_type pos = npos ) const
        {
            return (find_last_not_of( _S, pos, helper_type::length( _S ) ));
        }
        size_type find_last_not_of( value_type _C, size_type pos = npos ) const
        {
            return (find_last_not_of( static_cast<const value_type*>(&_C), pos, 1 ));
        }
        exe_basic_string substr( size_type pos = 0, size_type _M = npos ) const
        {
            return (exe_basic_string( *this, pos, _M ));
        }
        int compare( const exe_basic_string& other ) const
        {
            return (compare( 0, _Len, other.c_str(), other.size() ));
        }
        int compare( size_type _P0, size_type _N0,
                    const exe_basic_string& other ) const
        {
            return (compare( _P0, _N0, other, 0, npos ));
        }
        int compare( size_type _P0, size_type _N0, const exe_basic_string& other,
                    size_type pos, size_type _M ) const
        {
            if ( other.size() < pos )
            {
                return _throw_out_of_range_exception(), this->size();
            }
            if ( other._Len - pos < _M )
                _M = other._Len - pos;
            return (compare( _P0, _N0, other.c_str() + pos, _M ));
        }
        int compare( const value_type* _S ) const
        {
            return (compare( 0, _Len, _S, helper_type::length( _S ) ));
        }
        int compare( size_type _P0, size_type _N0, const value_type* _S ) const
        {
            return (compare( _P0, _N0, _S, helper_type::length( _S ) ));
        }
        int compare( size_type _P0, size_type _N0, const value_type* _S,
                    size_type _M ) const
        {
            if ( _Len < _P0 )
            {
                return _throw_out_of_range_exception(), this->size();
            }
            if ( _Len - _P0 < _N0 )
                _N0 = _Len - _P0;
            size_type _Ans = helper_type::compare( _Psum( _Ptr, _P0 ), _S,
                                        _N0 < _M ? _N0 : _M );
            return (_Ans != 0 ? _Ans : _N0 < _M ? -1
                    : _N0 == _M ? 0 : +1);
        }

        allocator_type get_allocator() const
        { return helper.alloc; }

    private:
        enum { _MIN_SIZE = sizeof( value_type ) <= 32 ? 31 : 7 };

        // copy _Oldlen elements to newly allocated buffer
        NH3API_INLINE_LARGE
        void _Copy( size_type _N ) NH3API_NOEXCEPT
        {
            size_type _Ns = _N | _MIN_SIZE;
            if ( max_size() < _Ns )
                _Ns = _N;
            value_type* _S;

            NH3API_IF_CONSTEXPR ( nh3api::tt::allocator_may_throw<allocator_type>::value )
            {
                NH3API_TRY
                {
                    _S = helper.allocate( _Ns + 2, nullptr );
                }
                NH3API_CATCH(...)
                {
                    _Ns = _N;
                    _S = helper.allocate( _Ns + 2, nullptr );
                }
            }
            else
            {
                _S = helper.allocate( _Ns + 2, nullptr );
            }

            if ( 0 < _Len )
                helper_type::copy( _S + 1, _Ptr, _Len > _Ns ? _Ns : _Len );
            size_type _Olen = _Len;
            _Tidy( true );
            _Ptr = _S + 1;
            _Refcnt( _Ptr ) = 0;
            _Res = _Ns;
            _Eos( _Olen > _Ns ? _Ns : _Olen );
        }

        // set new length and null terminator
        void _Eos( size_type _N )
        {
            helper_type::assign( _Ptr[_Len = _N], value_type(0) );
        }

        void _Freeze()
        {
            if ( _Ptr != nullptr
                && _Refcnt( _Ptr ) != 0 && _Refcnt( _Ptr ) != _FROZEN )
                _Grow( _Len );
            if ( _Ptr != nullptr )
                _Refcnt( _Ptr ) = _FROZEN;
        }

        NH3API_INLINE_LARGE
        // ensure buffer is big enough, trim to size if _Trim is true
        bool _Grow( size_type _N, bool _Trim = false )
        {
            if ( max_size() < _N )
            {
                return _throw_length_exception(), false;
            }
            if ( _Ptr != nullptr
                && _Refcnt( _Ptr ) != 0 && _Refcnt( _Ptr ) != _FROZEN )
                {
                    if ( _N == 0 )
                    {
                        --_Refcnt( _Ptr );
                        _Tidy();
                        return (false);
                    }
                    else
                    {
                        _Copy( _N );
                        return (true);
                    }
                }

            if ( _N == 0 )
            {
                if ( _Trim )
                    _Tidy( true );
                else if ( _Ptr != nullptr )
                    _Eos( 0 );
                return (false);
            }
            else
            {
                if ( _Trim && (_MIN_SIZE < _Res || _Res < _N) )
                {
                    _Tidy( true );
                    _Copy( _N );
                }
                else if ( !_Trim && _Res < _N )
                    _Copy( _N );
                return (true);
            }
        }

        static size_type _Pdif( const_pointer _P2, const_pointer _P1 )
        {
            return (_P2 == nullptr ? 0 : _P2 - _P1);
        }

        static const_pointer _Psum( const_pointer pos, size_type _N )
        {
            return (pos == nullptr ? nullptr : pos + _N);
        }

        static pointer _Psum( pointer pos, size_type _N )
        {
            return (pos == nullptr ? nullptr : pos + _N);
        }

        NH3API_FORCEINLINE
        unsigned char& _Refcnt( const value_type* _U )
        {
            return ((unsigned char*)(_U))[-1];
        }

        void _Split()
        {
            if ( _Ptr != nullptr && _Refcnt( _Ptr ) != 0 && _Refcnt( _Ptr ) != _FROZEN )
            {
                const value_type* _Temp = _Ptr;
                _Tidy( true );
                assign( _Temp );
            }
        }

        NH3API_FLATTEN
        // initialize buffer, deallocating any storage
        void _Tidy( bool _Built = false)
        {
            if ( !_Built || _Ptr == nullptr )
                ;
            else if ( _Refcnt( _Ptr ) == 0 || _Refcnt( _Ptr ) == _FROZEN )
                helper.deallocate( _Ptr - 1, _Res + 2 );
            else
                --_Refcnt( _Ptr );
            _Nullify();
        }

        NH3API_FORCEINLINE
        void _Tidy_deallocate()
        {
            if ( _Ptr == nullptr )
                ;
            else if ( _Refcnt( _Ptr ) == 0 || _Refcnt( _Ptr ) == _FROZEN )
                helper.deallocate( _Ptr - 1, _Res + 2 );
            else
                --_Refcnt( _Ptr );
        }

    #if NH3API_CHECK_MSVC_DRIVER
    protected:
        // tell the ref count for natvis
        unsigned char _natvis_RefCount()
        { return _Refcnt( _Ptr ); }

    #endif

    protected:
        helper_type helper;
        value_type* _Ptr; // data()
        size_type _Len; // size()
        size_type _Res; // capacity()
};
#pragma pack(pop)

#if !NH3API_STD_INLINE_VARIABLES
template<class CharT, class CharTraits, class Allocator>
    const typename exe_basic_string<CharT, CharTraits, Allocator>::size_type
    exe_basic_string<CharT, CharTraits, Allocator>::npos 
    = typename exe_basic_string<CharT, CharTraits, Allocator>::size_type(-1);
#endif

//} // namespace nh3api

#if !NH3API_STD_MOVE_SEMANTICS
template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
void swap(const exe_basic_string<CharT, CharTraits, Allocator>& lhs,
          const exe_basic_string<CharT, CharTraits, Allocator>& rhs) // ADL swap
{ lhs.swap(rhs); }
#endif

typedef exe_basic_string<char, std::char_traits<char>, exe_allocator<char> > exe_string;
typedef exe_basic_string<wchar_t, std::char_traits<wchar_t>, exe_allocator<wchar_t> > exe_wstring;
// NH3API_SIZE_TEST(0x10, exe_string);

// concatenation

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
exe_basic_string<CharT, CharTraits, Allocator> operator+(
    const exe_basic_string<CharT, CharTraits, Allocator>& _L,
    const exe_basic_string<CharT, CharTraits, Allocator>& _R)
{
    exe_basic_string<CharT, CharTraits, Allocator> result;
    result.reserve(_L.size() + _R.size());
    result += _L;
    result += _R;
    return result;
}

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
exe_basic_string<CharT, CharTraits, Allocator> operator+(const CharT* const _L,
                                        const exe_basic_string<CharT, CharTraits, Allocator>& _R)
{
    exe_basic_string<CharT, CharTraits, Allocator> result;
    result.reserve(exe_string_helper<CharT, CharTraits, Allocator>::length(_L) + _R.size());
    result += _L;
    result += _R;
    return result;
}

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
exe_basic_string<CharT, CharTraits, Allocator> operator+(
    const CharT _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R)
{
    exe_basic_string<CharT, CharTraits, Allocator> result;
    result.reserve(1 + _R.size());
    result += _L;
    result += _R;
    return result;
}

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
exe_basic_string<CharT, CharTraits, Allocator> operator+(
    const exe_basic_string<CharT, CharTraits, Allocator>& _L,
    const CharT* const _R)
{
    exe_basic_string<CharT, CharTraits, Allocator> result;
    result.reserve(_L.size() + exe_string_helper<CharT, CharTraits, Allocator>::length(_R));
    result += _L;
    result += _R;
    return result;
}

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
exe_basic_string<CharT, CharTraits, Allocator> operator+(
    const exe_basic_string<CharT, CharTraits, Allocator>& _L, const CharT _R)
{
    exe_basic_string<CharT, CharTraits, Allocator> result;
    result.reserve(_L.size() + 1);
    result += _L;
    result += _R;
    return result;
}

#if NH3API_STD_MOVE_SEMANTICS

template <class CharT, class CharTraits, class Allocator>
NH3API_FORCEINLINE exe_basic_string<CharT, CharTraits, Allocator>
operator+(const exe_basic_string<CharT, CharTraits, Allocator> &_L, exe_basic_string<CharT, CharTraits, Allocator>&& _R)
{ return std::move(_R.insert(0, _L)); }

template <class CharT, class CharTraits, class Allocator>
NH3API_FORCEINLINE exe_basic_string<CharT, CharTraits, Allocator>
operator+(exe_basic_string<CharT, CharTraits, Allocator>&& _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R)
{ return std::move(_L.append(_R)); }

template <class CharT, class CharTraits, class Allocator>
NH3API_FORCEINLINE exe_basic_string<CharT, CharTraits, Allocator>
operator+(exe_basic_string<CharT, CharTraits, Allocator>&& _L, exe_basic_string<CharT, CharTraits, Allocator>&& _R)
{
    if (_R.size() <= _L.capacity() - _L.size() || _R.capacity() - _R.size() < _L.size())
        return std::move(_L.append(_R));
    else
        return std::move(_R.insert(0, _L));
}

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
exe_basic_string<CharT, CharTraits, Allocator> operator+(const CharT* const _L,
                                        exe_basic_string<CharT, CharTraits, Allocator>&& _R)
{ return std::move(_R.insert(0, _L)); }

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
exe_basic_string<CharT, CharTraits, Allocator> operator+(
    const CharT _L, exe_basic_string<CharT, CharTraits, Allocator>&& _R)
{
    typedef typename Allocator::size_type size_type;
    return std::move(_R.insert(static_cast<size_type>(0), static_cast<size_type>(1), _L));
}

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
exe_basic_string<CharT, CharTraits, Allocator> operator+(
    exe_basic_string<CharT, CharTraits, Allocator>&& _L,
    const CharT* const _R)
{ return std::move(_L.append(_R)); }

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
exe_basic_string<CharT, CharTraits, Allocator> operator+(
    exe_basic_string<CharT, CharTraits, Allocator>&& _L, const CharT _R)
{
    _L.push_back(_R);
    return std::move(_L);
}

#endif

// relational operators

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator==(const exe_basic_string<CharT, CharTraits, Allocator>& _L,
                const exe_basic_string<CharT, CharTraits, Allocator>& _R)
{ return (_L.compare(_R) == 0); }

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator==(const CharT* _L,
                const exe_basic_string<CharT, CharTraits, Allocator>& _R)
{ return (_R.compare(_L) == 0); }

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator==(const exe_basic_string<CharT, CharTraits, Allocator>& _L,
                const CharT* _R)
{ return (_L.compare(_R) == 0); }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator<(
    const exe_basic_string<CharT, CharTraits, Allocator>& _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R) NH3API_NOEXCEPT
{ return _L.compare(_R) < 0; }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator<(const CharT* const _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R) NH3API_NOEXCEPT
{ return _R.compare(_L) > 0; }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator<(const exe_basic_string<CharT, CharTraits, Allocator>& _L, const CharT* const _R) NH3API_NOEXCEPT
{ return _L.compare(_R) < 0; }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator>(
    const exe_basic_string<CharT, CharTraits, Allocator>& _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R) NH3API_NOEXCEPT
{ return _R < _L; }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator>( const CharT* const _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R) NH3API_NOEXCEPT
{ return _R < _L; }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator>(const exe_basic_string<CharT, CharTraits, Allocator>& _L, const CharT* const _R) NH3API_NOEXCEPT
{ return _R < _L; }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator<=(
    const exe_basic_string<CharT, CharTraits, Allocator>& _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R) NH3API_NOEXCEPT
{ return !(_R < _L); }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator<=(const CharT* const _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R) NH3API_NOEXCEPT
{ return !(_R < _L); }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator<=(const exe_basic_string<CharT, CharTraits, Allocator>& _L, const CharT* const _R) NH3API_NOEXCEPT
{ return !(_R < _L); }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator>=(
    const exe_basic_string<CharT, CharTraits, Allocator>& _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R) NH3API_NOEXCEPT
{ return !(_L < _R); }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator>=(const CharT* const _L, const exe_basic_string<CharT, CharTraits, Allocator>& _R) NH3API_NOEXCEPT
{ return !(_L < _R); }

template <class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator>=(const exe_basic_string<CharT, CharTraits, Allocator>& _L, const CharT* const _R) NH3API_NOEXCEPT
{ return !(_L < _R); }

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator!=(const exe_basic_string<CharT, CharTraits, Allocator>& _L,
                const exe_basic_string<CharT, CharTraits, Allocator>& _R)
{ return (!(_L == _R)); }

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator!=(const CharT* _L,
                const exe_basic_string<CharT, CharTraits, Allocator>& _R)
{ return (!(_L == _R)); }

template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
bool operator!=(const exe_basic_string<CharT, CharTraits, Allocator>& _L,
                const CharT* _R)
{ return (!(_L == _R)); }

#if !NH3API_CHECK_CPP11 || (NH3API_MSVC_VERSION >= NH3API_MSVC_VERSION_2010 && NH3API_MSVC_VERSION < NH3API_MSVC_VERSION_2015)
template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator==(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    CharT const * rhs ) NH3API_NOEXCEPT
{ return lhs.size() == exe_string_helper<CharT, CharTraits>::length( rhs ) && lhs.compare( rhs ) == 0; }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator==(
    CharT const * lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return exe_string_helper<CharT, CharTraits>::length( lhs ) == rhs.size() && rhs.compare( lhs ) == 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator==(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    exe_basic_string<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator==(
    exe_basic_string<CharT, CharTraits> rhs,
    nh3api::basic_string_view<CharT, CharTraits> lhs ) NH3API_NOEXCEPT
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator!=(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    CharT const * rhs ) NH3API_NOEXCEPT
{ return !( lhs == rhs ); }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator!=(
    CharT const * lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return !( lhs == rhs ); }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator!=(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    exe_basic_string<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return !( lhs == rhs ); }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator!=(
    exe_basic_string<CharT, CharTraits> rhs,
    nh3api::basic_string_view<CharT, CharTraits> lhs ) NH3API_NOEXCEPT
{ return !( lhs == rhs ); }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator<(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    CharT const * rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) < 0; }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator<(
    CharT const * lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return rhs.compare( lhs ) > 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator<(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    exe_basic_string<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) < 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator<(
    exe_basic_string<CharT, CharTraits> rhs,
    nh3api::basic_string_view<CharT, CharTraits> lhs ) NH3API_NOEXCEPT
{ return rhs.compare( lhs ) > 0; }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator<=(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    CharT const * rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) <= 0; }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator<=(
    CharT const * lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return rhs.compare( lhs ) >= 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator<=(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    exe_basic_string<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) <= 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator<=(
    exe_basic_string<CharT, CharTraits> rhs,
    nh3api::basic_string_view<CharT, CharTraits> lhs ) NH3API_NOEXCEPT
{ return rhs.compare( lhs ) >= 0; }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator>(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    CharT const * rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) > 0; }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator>(
    CharT const * lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return rhs.compare( lhs ) < 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator>(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    exe_basic_string<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) > 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator>(
    exe_basic_string<CharT, CharTraits> rhs,
    nh3api::basic_string_view<CharT, CharTraits> lhs ) NH3API_NOEXCEPT
{ return rhs.compare( lhs ) < 0; }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator>=(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    CharT const * rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) >= 0; }

template< class CharT, class CharTraits>
NH3API_CONSTEXPR bool operator>=(
    CharT const * lhs,
    nh3api::basic_string_view<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return rhs.compare( lhs ) <= 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator>=(
    nh3api::basic_string_view<CharT, CharTraits> lhs,
    exe_basic_string<CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) >= 0; }

template< class CharT, class CharTraits>
NH3API_FORCEINLINE bool operator>=(
    exe_basic_string<CharT, CharTraits> rhs,
    nh3api::basic_string_view<CharT, CharTraits> lhs ) NH3API_NOEXCEPT
{ return rhs.compare( lhs ) <= 0; }
#else 
template< class CharT, class CharTraits, int=1 >
NH3API_CONSTEXPR bool operator==(
         nh3api::basic_string_view  <CharT, CharTraits> lhs,
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type rhs ) NH3API_NOEXCEPT
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

template< class CharT, class CharTraits, int=2 >
NH3API_CONSTEXPR bool operator==(
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type lhs,
         nh3api::basic_string_view  <CharT, CharTraits> rhs ) NH3API_NOEXCEPT
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

template< class CharT, class CharTraits, int=1 >
NH3API_CONSTEXPR bool operator!= (
         nh3api::basic_string_view  < CharT, CharTraits > lhs,
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type rhs ) NH3API_NOEXCEPT
{ return !( lhs == rhs ); }

template< class CharT, class CharTraits, int=2 >
NH3API_CONSTEXPR bool operator!= (
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type lhs,
         nh3api::basic_string_view  < CharT, CharTraits > rhs ) NH3API_NOEXCEPT
{ return !( lhs == rhs ); }

template< class CharT, class CharTraits, int=1 >
NH3API_CONSTEXPR bool operator< (
         nh3api::basic_string_view  < CharT, CharTraits > lhs,
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) < 0; }

template< class CharT, class CharTraits, int=2 >
NH3API_CONSTEXPR bool operator< (
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type lhs,
         nh3api::basic_string_view  < CharT, CharTraits > rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) < 0; }

template< class CharT, class CharTraits, int=1 >
NH3API_CONSTEXPR bool operator<= (
         nh3api::basic_string_view  < CharT, CharTraits > lhs,
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) <= 0; }

template< class CharT, class CharTraits, int=2 >
NH3API_CONSTEXPR bool operator<= (
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type lhs,
         nh3api::basic_string_view  < CharT, CharTraits > rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) <= 0; }

template< class CharT, class CharTraits, int=1 >
NH3API_CONSTEXPR bool operator> (
         nh3api::basic_string_view  < CharT, CharTraits > lhs,
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) > 0; }

template< class CharT, class CharTraits, int=2 >
NH3API_CONSTEXPR bool operator> (
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type lhs,
         nh3api::basic_string_view  < CharT, CharTraits > rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) > 0; }

template< class CharT, class CharTraits, int=1 >
NH3API_CONSTEXPR bool operator>= (
         nh3api::basic_string_view  < CharT, CharTraits > lhs,
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) >= 0; }

template< class CharT, class CharTraits, int=2 >
NH3API_CONSTEXPR bool operator>= (
		 typename std::decay< nh3api::basic_string_view<CharT, CharTraits> >::type lhs,
         nh3api::basic_string_view  < CharT, CharTraits > rhs ) NH3API_NOEXCEPT
{ return lhs.compare( rhs ) >= 0; }
#endif

#ifdef __cpp_user_defined_literals
namespace exe_string_literals
{
NH3API_FORCEINLINE
exe_string operator""_exe( const char* str, size_t len )
{ return exe_string{str, len}; }

NH3API_FORCEINLINE
exe_wstring operator""_exe( const wchar_t* str, size_t len )
{ return exe_wstring{str, len}; }    
}
#endif

namespace nh3api
{

// get exe_basic_string reference count
// if you need it for whatever reason
template<class CharT, class CharTraits, class Allocator> NH3API_FORCEINLINE
uint8_t refcount(const exe_basic_string<CharT, CharTraits, Allocator>& str)
{ return *(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(str.c_str())) - 1); }

NH3API_CONSTEXPR inline
// constexpr atoi that uses no locale
int32_t fast_atoi(const char* p) NH3API_NOEXCEPT
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

// convert single digit to char
NH3API_FORCEINLINE NH3API_CONSTEXPR
char digit_to_char(int32_t digit, char) NH3API_NOEXCEPT
{ return '0' + static_cast<char>(digit); }

// convert single digit to wide char
NH3API_FORCEINLINE NH3API_CONSTEXPR
wchar_t digit_to_char(int32_t digit, wchar_t) NH3API_NOEXCEPT
{ return L'0' + static_cast<wchar_t>(digit); }

// convert letter character to lowercase
NH3API_CONST NH3API_FORCEINLINE NH3API_CONSTEXPR 
char fast_tolower(const char c) NH3API_NOEXCEPT
{ return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c; }

struct case_insensitive_traits : public ::std::char_traits<char>
{
    static NH3API_CONST NH3API_CONSTEXPR bool eq(char c1, char c2) NH3API_NOEXCEPT
    {
        return fast_tolower(c1) == fast_tolower(c2);
    }

    static NH3API_CONST NH3API_CONSTEXPR bool ne(char c1, char c2) NH3API_NOEXCEPT
    {
        return fast_tolower(c1) != fast_tolower(c2);
    }

    static NH3API_CONST NH3API_CONSTEXPR bool lt(char c1, char c2) NH3API_NOEXCEPT
    {
        return fast_tolower(c1) < fast_tolower(c2);
    }

    static NH3API_CONSTEXPR int compare(const char* s1, const char* s2, size_t n) NH3API_NOEXCEPT
    {
        while (n-- != 0)
        {
            if (fast_tolower(*s1) < fast_tolower(*s2))
                return -1;
            if (fast_tolower(*s1) > fast_tolower(*s2))
                return 1;
            ++s1; 
            ++s2;
        }
        return 0;
    }

    static NH3API_CONSTEXPR const char* find(const char* s, int n, char a) NH3API_NOEXCEPT
    {
        while ( n-- > 0)
        {
            if (fast_tolower(*s) == fast_tolower(a))
            {
                return s;
            }
            ++s;
        }
        return nullptr;
    }
};

NH3API_CONSTEXPR bool isalpha_constexpr(const char c) NH3API_NOEXCEPT
{
    const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "abcdefghijklmnopqrstuvwxyz";
    const char *letter = alphabet;
    while(*letter != '\0' && *letter != c)
        ++letter;
    if (*letter)
        return true;
    return false;
}

// fast algorithm by void_17
template<class StringT>
static StringT print_int(int32_t x)
{
    typedef typename StringT::value_type CharT;
    if ( x == 0 )
        return StringT(1, CharT('0'));
    int32_t minusSymbol = (x < 0);
    x = ::std::abs(x);
    const size_t numSymbols = minusSymbol + nh3api::count_digits(x);
    // hint the compiler for small string optimization for std::basic_string
    NH3API_ASSUME(numSymbols <= 10);
    StringT str(numSymbols, CharT('\0'));
    if ( minusSymbol )
        str[0] = '-';
    CharT* ptr = const_cast<CharT*>(str.c_str()) + str.size()-1;
    while (x > 0)
    {
        int32_t digit = x % 10;
        *ptr-- = digit_to_char(digit, CharT());
        x /= 10;
    }
    return str;
}

// fast algorithm by void_17
template<class StringT>
static StringT print_uint(uint32_t x)
{
    typedef typename StringT::value_type CharT;
    if ( x == 0 )
        return StringT(1, CharT('0'));
    const size_t numSymbols = nh3api::count_digits(x);
    // hint the compiler for small string optimization for std::basic_string
    NH3API_ASSUME(numSymbols <= 11);
    StringT str(numSymbols, CharT('\0'));
    CharT* ptr = const_cast<CharT*>(str.c_str()) + str.size()-1;
    while (x > 0)
    {
        uint32_t digit = x % 10;
        *ptr-- = digit_to_char(digit, CharT());
        x /= 10;
    }
    return str;
}

NH3API_FORCEINLINE NH3API_CONSTEXPR
const char* get_inf_string(char) NH3API_NOEXCEPT
{ return "INF"; }

NH3API_FORCEINLINE NH3API_CONSTEXPR
const wchar_t* get_inf_string(wchar_t) NH3API_NOEXCEPT
{ return L"INF"; }

NH3API_FORCEINLINE NH3API_CONSTEXPR
const char* get_nan_string(char) NH3API_NOEXCEPT
{ return "NaN"; }

NH3API_FORCEINLINE NH3API_CONSTEXPR
const wchar_t* get_nan_string(wchar_t) NH3API_NOEXCEPT
{ return L"NaN"; }

NH3API_FORCEINLINE NH3API_CONSTEXPR
const char* get_error_fp_string(char) NH3API_NOEXCEPT
{ return "ERROR"; }

NH3API_FORCEINLINE NH3API_CONSTEXPR
const wchar_t* get_error_fp_string(wchar_t) NH3API_NOEXCEPT
{ return L"ERROR"; }

template<class StringT, typename CharT = typename StringT::value_type>
struct fp_finite_printer; // undefined

// I HAVE to repeat the same code again
// and put it in the struct
// to make it compile under C++98...

template<class StringT>
struct fp_finite_printer<StringT, char>
{
    static StringT print(float x, size_t precision)
    {
        const size_t buffer_size = static_cast<size_t>(_scprintf("%.*f", precision, x));
        StringT result(buffer_size, '\0');
        sprintf_s(const_cast<char*>(result.c_str()), buffer_size + 1, "%.*f", precision, x);
        return result;
    }

    static StringT print(double x, size_t precision)
    {
        const size_t buffer_size = static_cast<size_t>(_scprintf("%.*f", precision, x));
        StringT result(buffer_size, '\0');
        sprintf_s(const_cast<char*>(result.c_str()), buffer_size + 1, "%.*f", precision, x);
        return result;
    }
};

template<class StringT>
struct fp_finite_printer<StringT, wchar_t>
{
    static StringT print(float x, size_t precision)
    {
        const size_t buffer_size = static_cast<size_t>(_scwprintf(L"%.*f", precision, x));
        StringT result(buffer_size, L'\0');
        swprintf_s(const_cast<wchar_t*>(result.c_str()), buffer_size + 1, L"%.*f", precision, x);
        return result;
    }

    static StringT print(double x, size_t precision)
    {
        const size_t buffer_size = static_cast<size_t>(_scwprintf(L"%.*f", precision, x));
        StringT result(buffer_size, L'\0');
        swprintf_s(const_cast<wchar_t*>(result.c_str()), buffer_size + 1, L"%.*f", precision, x);
        return result;
    }
};

template<class StringT>
static StringT print_float(float x, size_t precision)
{
    typedef typename StringT::value_type CharT;
    switch (nh3api::fpclassify(x))
    {
        case FP_INFINITE:
        {
            return StringT(get_inf_string(CharT()), 3);
        }
        case FP_NAN:
        {
            return StringT(get_nan_string(CharT()), 3);
        }
        // isfinite
        case FP_NORMAL:
        case FP_SUBNORMAL:
        case FP_ZERO:
        {
            return fp_finite_printer<StringT>::print(x, precision);
        }
        default:
            return StringT(get_error_fp_string(CharT()), 5);
    }
}

template<class StringT>
static StringT print_double(double x, size_t precision)
{
    typedef typename StringT::value_type     CharT;
    switch (nh3api::fpclassify(x))
    {
        case FP_INFINITE:
        {
            return StringT(get_inf_string(CharT()), 3);
        }
        case FP_NAN:
        {
            return StringT(get_nan_string(CharT()), 3);
        }
        // isfinite
        case FP_NORMAL:
        case FP_SUBNORMAL:
        case FP_ZERO:
        {
            return fp_finite_printer<StringT>::print(x, precision);
        }
        default:
            return StringT(get_error_fp_string(CharT()), 5);
    }
}

} // namespace nh3api

NH3API_FORCEINLINE
// write signed integer as string
exe_string to_exe_string(int32_t x)
{ return nh3api::print_int<exe_string>(x); }

NH3API_FORCEINLINE
// write unsigned integer as string
exe_string to_exe_string(uint32_t x)
{ return nh3api::print_uint<exe_string>(x); }

NH3API_FORCEINLINE
// write double as string
exe_string to_exe_string(double x, size_t precision = 8)
{ return nh3api::print_double<exe_string>(x, precision); }

NH3API_FORCEINLINE
// write float as string
exe_string to_exe_string(float x, size_t precision = 4)
{ return nh3api::print_float<exe_string>(x, precision); }

NH3API_FORCEINLINE
// write signed integer as wide string
exe_wstring to_exe_wstring(int32_t x)
{ return nh3api::print_int<exe_wstring>(x);}

NH3API_FORCEINLINE
// write unsigned integer as wide string
exe_wstring to_exe_wstring(uint32_t x)
{ return nh3api::print_uint<exe_wstring>(x); }

NH3API_FORCEINLINE
// write double as wide string
exe_wstring to_exe_wstring(double x, size_t precision = 8)
{ return nh3api::print_double<exe_wstring>(x, precision); }

NH3API_FORCEINLINE
// write float as wide string
exe_wstring to_exe_wstring(float x, size_t precision = 4)
{ return nh3api::print_float<exe_wstring>(x, precision); }

NH3API_FORCEINLINE
// write signed integer as string
std::string to_std_string(int32_t x)
{ return nh3api::print_int<std::string>(x); }

NH3API_FORCEINLINE
// write unsigned integer as string
std::string to_std_string(uint32_t x)
{ return nh3api::print_uint<std::string>(x); }

NH3API_FORCEINLINE
// write double as string
std::string to_std_string(double x, size_t precision = 8)
{ return nh3api::print_double<std::string>(x, precision); }

NH3API_FORCEINLINE
// write float as string
std::string to_std_string(float x, size_t precision = 4)
{ return nh3api::print_float<std::string>(x, precision); }

NH3API_FORCEINLINE
// write signed integer as wide string
std::wstring to_std_wstring(int32_t x)
{ return nh3api::print_int<std::wstring>(x); }

NH3API_FORCEINLINE
// write unsigned integer as wide string
std::wstring to_std_wstring(uint32_t x)
{ return nh3api::print_uint<std::wstring>(x); }

NH3API_FORCEINLINE
// write double as wide string
std::wstring to_std_wstring(double x, size_t precision = 8)
{ return nh3api::print_double<std::wstring>(x, precision); }

NH3API_FORCEINLINE
// write float as wide string
std::wstring to_std_wstring(float x, size_t precision = 4)
{ return nh3api::print_float<std::wstring>(x, precision); }

#if NH3API_MSVC_STL_VERSION > NH3API_MSVC_STL_VERSION_2010 || NH3API_CHECK_CPP11
// std::hash support for exe_basic_string
template<typename CharT, typename TraitsT, typename AllocatorT>
class std::hash< exe_basic_string<CharT, TraitsT, AllocatorT> >
{
    public:
        size_t operator()(const exe_basic_string<CharT, TraitsT, AllocatorT>& str) NH3API_NOEXCEPT
        {
            nh3api::default_hash hasher;
            hasher.update(str.data(), str.size());
            return hasher.digest();
        }
};
#endif

// std::string which uses exe_allocator<char>
typedef std::basic_string<char, std::char_traits<char>, exe_allocator<char> > exe_std_string;
// std::wstring which uses exe_allocator<wchar_t>
typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, exe_allocator<wchar_t> > exe_std_wstring;

NH3API_DISABLE_WARNING_END
