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

namespace nh3api 
{

struct constexpr_char_traits
{
    public:
        typedef char char_type;
        typedef int  int_type;
        typedef ::std::streamoff off_type;
        typedef ::std::streampos pos_type;
        typedef ::std::mbstate_t state_type;
        #if NH3API_CHECK_CPP20
        using comparison_category = ::std::strong_ordering;
        #endif

    private:
        enum : uint32_t { npos = (uint32_t)(-1) };

    public:
        static NH3API_CONSTEXPR NH3API_FORCEINLINE
        void assign(char& c1, const char& c2) NH3API_NOEXCEPT
        { c1 = c2; }

        static NH3API_CONSTEXPR NH3API_FORCEINLINE
        bool eq(char c1, char c2) NH3API_NOEXCEPT
        { return c1 == c2; }

        static NH3API_CONSTEXPR NH3API_FORCEINLINE
        bool lt(char c1, char c2) NH3API_NOEXCEPT
        { return (uint8_t)c1 < (uint8_t)c2; }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        size_t length(const char* str) NH3API_NOEXCEPT
        {
            if ( str == nullptr )
                return 0;

            #if NH3API_HAS_BUILTIN(__builtin_strlen)
            return __builtin_strlen(str);
            #else // __has_builtin(__builtin_strlen)
                #if NH3API_STD_RELAXED_CONSTEXPR
                #if NH3API_HAS_IS_CONSTANT_EVALUATED
                if (is_constant_evaluated())
                {
                    const char* ptr = str;
                    while (*ptr != '\0')
                        ++ptr;
                    return ptr - str;

                    return 0;
                }
                else  
                {
                #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
                    return ::strlen(str);
                #if NH3API_HAS_IS_CONSTANT_EVALUATED
                }
                #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
                #else // NH3API_STD_RELAXED_CONSTEXPR
                return ::strlen(str);
                #endif // NH3API_STD_RELAXED_CONSTEXPR
            #endif // __has_builtin(__builtin_strlen)
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        int compare(const char* left, const char* right, size_t count) NH3API_NOEXCEPT
        {
            if ( left == nullptr || right == nullptr || count == 0 )
                return 0;
            
            #if NH3API_HAS_BUILTIN(__builtin_memcmp)
            return __builtin_memcmp(left, right, count);
            #else // __has_builtin(__builtin_memcmp)
            #if NH3API_STD_RELAXED_CONSTEXPR
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            if (is_constant_evaluated())
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
            #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
                return ::memcmp(left, right, count);
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            }
            #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
            #else // NH3API_STD_RELAXED_CONSTEXPR
            return ::memcmp(left, right, count);
            #endif // NH3API_STD_RELAXED_CONSTEXPR
            #endif // __has_builtin(__builtin_memcmp)
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        char* move(char* dst, const char* src, size_t count) NH3API_NOEXCEPT
        {
            #if NH3API_HAS_BUILTIN(__builtin_memmove)
            __builtin_memmove(dst, src, count);
            return dst;
            #else
            #if NH3API_STD_RELAXED_CONSTEXPR
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            if (is_constant_evaluated())
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
            #endif
                ::memmove(dst, src, count);
                return dst;
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            }
            #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
            #else // NH3API_STD_RELAXED_CONSTEXPR
            ::memmove(dst, src, count);
            return dst;
            #endif // NH3API_STD_RELAXED_CONSTEXPR
            #endif // __has_builtin(__builtin_memmove)
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        char* copy(char* dst, const char* src, size_t count) NH3API_NOEXCEPT
        {
            if ( dst == nullptr || src == nullptr )
                return nullptr;
            #if NH3API_HAS_BUILTIN(__builtin_memcpy)
            __builtin_memcpy(dst, src, count);
            return dst;
            #else
            #if NH3API_STD_RELAXED_CONSTEXPR
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            if (is_constant_evaluated())
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
            #endif
                ::memcpy(dst, src, count);
                return dst;
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            }
            #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
            #else // NH3API_STD_RELAXED_CONSTEXPR
            ::memcpy(dst, src, count);
            return dst;
            #endif // NH3API_STD_RELAXED_CONSTEXPR
            #endif // __has_builtin(__builtin_memcpy)
        }

        static NH3API_CONSTEXPR_CPP_14
        char* assign(char* str, size_t count, char value) NH3API_NOEXCEPT
        {
            if ( str == nullptr )
                return nullptr;
            #if NH3API_STD_RELAXED_CONSTEXPR
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            if (is_constant_evaluated())
            {
                for (size_t i = 0; i < count; i++)
                    *str++ = value;
                return str;
            }
            else
            {
            #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
                ::memset(str, value, count);
                return str;
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            }
            #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
            #else 
            ::memset(str, value, count);
            return str;
            #endif // NH3API_STD_RELAXED_CONSTEXPR
        }

        static NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
        const char* find(const char* str, size_t count, const char& ch)
        {
            if ( count == 0 || str == nullptr)
                return nullptr;
            #if NH3API_HAS_BUILTIN(__builtin_char_memchr)
            return __builtin_char_memchr(str, ch, count);
            #else // __has_builtin(__builtin_char_memchr)
            #if NH3API_STD_RELAXED_CONSTEXPR
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            if (is_constant_evaluated())
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
            #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
                return static_cast<const char*>(::memchr(str, ch, count));
            #if NH3API_HAS_IS_CONSTANT_EVALUATED
            }
            #endif // NH3API_HAS_IS_CONSTANT_EVALUATED
            #else // NH3API_STD_RELAXED_CONSTEXPR
            return static_cast<const char*>(::memchr(str, ch, count));
            #endif // NH3API_STD_RELAXED_CONSTEXPR
            #endif // __has_builtin(__builtin_char_memchr)
        }

        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_find(const char* str, size_t size, char c, size_t pos) NH3API_NOEXCEPT
        {
            if ( pos >= size )
                return npos;
            const char* found = find(str + pos, size - pos, c);
            if ( found == nullptr )
                return npos;
            return static_cast<size_t>(found - str);
        }

        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_14
        static const char* search_substring(const char* first1, 
                                            const char* last1, 
                                            const char* first2, 
                                            const char* last2) NH3API_NOEXCEPT 
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
                first1 = find(first1, len1 - len2 + 1, f2);
                if (first1 == nullptr)
                    return last1;

                // It is faster to compare from the first byte of first1 even if we
                // already know that it matches the first byte of first2: this is because
                // first2 is most likely aligned, as it is user's "pattern" string, and
                // first1 + 1 is most likely not aligned, as the match is in the middle of
                // the string.
                if (compare(first1, first2, len2) == 0)
                    return first1;

                ++first1;
            }
        }

        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_find(const char* str, size_t size, const char* needle, size_t pos, size_t n) NH3API_NOEXCEPT 
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

        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_rfind(const char* str, size_t size, char c, size_t pos) NH3API_NOEXCEPT 
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

        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_rfind(const char* str, size_t size, const char* needle, size_t pos, size_t n) NH3API_NOEXCEPT 
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
        NH3API_FORCEINLINE NH3API_CONSTEXPR_CPP_14
        static const char* find_first_of_impl(const char* first1,
                                              const char* last1,
                                              const char* first2,
                                              const char* last2) NH3API_NOEXCEPT
        {
            for (; first1 != last1; ++first1)
                for (const char* i = first2; i != last2; ++i)
                    if (eq(*first1, *i))
                        return first1;
            return last1;
        }

    public:
        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_find_first_of(const char* str, 
                          size_t size, 
                          const char* needle, 
                          size_t pos, 
                          size_t n) NH3API_NOEXCEPT 
        {
            if (pos >= size || n == 0)
                return npos;
            const char* found = find_first_of_impl(str + pos, str + size, needle, needle + n);
            if (found == str + size)
                return npos;
            return static_cast<size_t>(found - str);
        }

        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_find_last_of(const char* str, size_t size, const char* needle, size_t pos, size_t n) NH3API_NOEXCEPT 
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

        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_find_first_not_of(const char* str, size_t size, const char* needle, size_t pos, size_t n) NH3API_NOEXCEPT 
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

        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_find_first_not_of(const char* str, size_t size, char c, size_t pos) NH3API_NOEXCEPT 
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

        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_find_last_not_of(const char* str, size_t size, const char* needle, size_t pos, size_t n) NH3API_NOEXCEPT 
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

        NH3API_FORCEINLINE size_t NH3API_CONSTEXPR_CPP_14 
        static str_find_last_not_of(const char* str, size_t size, char c, size_t pos) NH3API_NOEXCEPT 
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

} // namespace nh3api

#pragma pack(push, 4)
// Visual C++ 6.0 implementation of std::basic_string, 
// which uses unsafe copy-on-write semantics with 8-bit reference counting.
// Use only for binary compatibility with the program. 
// Consider using std::basic_string or exe_std_string/exe_std_wstring which use .exe allocator
class exe_string
{
    protected:
        template<class IterT>
            struct is_cptr : public nh3api::tt::integral_constant<bool,
                nh3api::tt::is_same<IterT, const char* const>::value
                || nh3api::tt::is_same<IterT, char* const>::value
                || nh3api::tt::is_same<IterT, const char*>::value
                || nh3api::tt::is_same<IterT, char*>::value>
            {};

    // external typedefs
    public:
        typedef char value_type;
        typedef exe_allocator<char>    allocator_type;
        typedef std::char_traits<char> traits_type;
        typedef size_t            size_type;
        typedef ptrdiff_t         difference_type;
        typedef value_type*       pointer;
        typedef const value_type* const_pointer;
        typedef value_type&       reference;
        typedef const value_type& const_reference;

        typedef pointer iterator;
        typedef const_pointer const_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef std::reverse_iterator<iterator>       reverse_iterator;

    // exceptions
    protected:
        #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
        NH3API_FORCEINLINE
        static void _Throw_length_exception()
        {
            NH3API_THROW(std::length_error, "string too long");
        }
        #else
        NH3API_FORCEINLINE
        static void _Throw_length_exception()
        {
            NH3API_THROW(0, 0);
        }
        #endif

        #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS 
        NH3API_FORCEINLINE
        static void _Throw_out_of_range_exception()
        {
            NH3API_THROW(std::out_of_range, "invalid string position");
        }
        #else
        NH3API_FORCEINLINE
        static void _Throw_out_of_range_exception()
        {
            NH3API_THROW(0, 0);
        }
        #endif

        // we use the exceptions function like this:
        // return _Throw_xxx(), yyy;
        // to help compiler optimize branches

    public:
        exe_string() NH3API_NOEXCEPT
            : _Dummy(0), _Ptr(nullptr), _Len(0), _Res(0)
        {}

        explicit exe_string( const allocator_type& _Al ) NH3API_NOEXCEPT
            : _Dummy(0), _Ptr(nullptr), _Len(0), _Res(0)
        {}

        explicit exe_string(const exe_string& other)
            : _Dummy(0), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(other, 0, npos);
        }

        exe_string(const exe_string& other, size_type pos, size_type n,
                        const allocator_type& _Al = allocator_type() )
            : _Dummy(0), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign( other, pos, n );
        }

        exe_string( const value_type* str, size_type _N,
                        const allocator_type& _Al = allocator_type() )
            : _Dummy(0), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(str, _N);
        }

        exe_string( const value_type* str, const allocator_type& _Al = allocator_type() )
            : _Dummy(0), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(str);
        }

        exe_string(size_type _N, value_type c)
            : _Dummy(0), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(_N, c);
        }

        exe_string(size_type _N, value_type c, const allocator_type& _Al)
            : _Dummy(0), _Ptr(nullptr), _Len(0), _Res(0)
        {
            assign(_N, c);
        }

        template <class IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        exe_string( IterT _F, IterT lhs, const allocator_type& _Al = allocator_type()
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
            : _Dummy(0), _Ptr(nullptr), _Len(0), _Res(0)
        {
            nh3api::verify_range(_F, lhs);
            _Construct(nh3api::unfancy(_F),
                       nh3api::unfancy(lhs),
                       nh3api::iter_category<IterT>());
        }

        protected:
        NH3API_FORCEINLINE void _Move_nullify(exe_string* other) NH3API_NOEXCEPT
        { nh3api::trivial_move<sizeof(*this)>(other, this); }

        template <class IterT>
        void _Construct(IterT _F, IterT lhs, std::input_iterator_tag)
        {
            NH3API_TRY
            {
                for (; _F != lhs; ++_F)
                    push_back(static_cast<char>(*_F));
            }
            NH3API_CATCH(...)
            {
                _Tidy_deallocate();
                NH3API_RETHROW
            }
        }

        template <class IterT>
        void _Construct(IterT _F, IterT lhs, std::forward_iterator_tag)
        {
            const size_type count = static_cast<size_type>(std::distance(_F, lhs));
            reserve(count);
            _Construct(_F, lhs, std::input_iterator_tag());
        }

        void _Construct(char* const _F, char* const lhs, std::random_access_iterator_tag)
        {
            if (_F != lhs)
                assign(_F, static_cast<size_type>(lhs - _F));
        }

        void _Construct(const char* const _F, const char* const lhs, std::random_access_iterator_tag)
        {
            if (_F != lhs)
                assign(_F, static_cast<size_type>(lhs - _F));
        }

        public:
        #if NH3API_STD_MOVE_SEMANTICS
        exe_string(exe_string&& other) NH3API_NOEXCEPT
        {
            if (this == &other)
            { // arguments are same, do nothing
                return;
            }
            else
            {
                _Move_nullify(&other);
            }
        }

        exe_string& operator=(exe_string&& other) NH3API_NOEXCEPT
        { return assign(std::forward<exe_string>(other)); }

        NH3API_INLINE_LARGE
        exe_string& assign(exe_string&& other) NH3API_NOEXCEPT
        {
            if (this == &other)
            { // arguments are same, do nothing
                return *this;
            }
            else
            {
                this->_Tidy_deallocate();
                _Move_nullify(&other);
            }
            return *this;
        }

        #endif

        exe_string(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
        {
            NH3API_IGNORE(_Dummy, _Ptr, _Len, _Res);
        }

        NH3API_FLATTEN
        ~exe_string() NH3API_NOEXCEPT
        { _Tidy_deallocate(); }

    protected:
        enum _Mref : uint8_t { _FROZEN = 255 };

    public:
        #if NH3API_STD_INLINE_VARIABLES
            static inline constexpr size_type npos = size_type(-1);
        #else 
            static const size_type npos;
        #endif
        exe_string& operator=( const exe_string& other )
        {
            return assign( other );
        }
        exe_string& operator=( const value_type* str )
        {
            return assign( str );
        }
        exe_string& operator=( value_type c )
        {
            return assign( 1, c );
        }

        NH3API_INLINE_LARGE
        exe_string& operator+=( const exe_string& other )
        {
            return append( other );
        }
        NH3API_INLINE_LARGE
        exe_string& operator+=( const value_type* str )
        {
            return append( str );
        }
        NH3API_INLINE_LARGE
        exe_string& operator+=( value_type c )
        {
            return append( 1, c );
        }

        exe_string& append( const exe_string& other )
        {
            return append( other, 0, npos );
        }
        exe_string &append(const exe_string &other, size_type pos, size_type n = npos)
        {
            if (other.size() < pos)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            size_type _N = other.size() - pos;
            if (_N < n)
                n = _N;
            if (npos - _Len <= n)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            if (0 < n && _Grow<false>(_N = _Len + n))
            {
                nh3api::constexpr_char_traits::copy(_Ptr + _Len, &other.c_str()[pos], n);
                _Eos(_N);
            }
            return (*this);
        }
        exe_string& append( const value_type* str, size_type n )
        {
            if (npos - _Len <= n)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            size_type _N;
            // we use noexcept version of _Grow because it is nearly impossible to get the string literal to overflow
            if ( 0 < n && _Grow<true>( _N = _Len + n ) )
            {
                nh3api::constexpr_char_traits::copy( _Ptr + _Len, str, n );
                _Eos( _N );
            }
            return (*this);
        }
        exe_string& append( const value_type* str )
        {
            return append(str, nh3api::constexpr_char_traits::length(str));
        }
        exe_string& append( size_type n, value_type c )
        {
            if (npos - _Len <= n)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            size_type _N;
            if (0 < n && _Grow<false>(_N = _Len + n))
            {
                nh3api::constexpr_char_traits::assign(_Ptr + _Len, n, c);
                _Eos(_N);
            }
            return (*this);
        }

        template<class IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        exe_string& append(IterT _F, IterT lhs
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        {
            return replace(end(), end(), _F, lhs);
        }

        void push_back(value_type c)
        {
            append(1, c);
        }

        exe_string& assign( const exe_string& other )
        {
            return (assign( other, 0, npos ));
        }
        NH3API_INLINE_LARGE
        exe_string& assign( const exe_string& other, size_type pos, size_type n )
        {
            if ( other.size() < pos )
            {
                return _Throw_out_of_range_exception(), *this;
            }
            size_type _N = other.size() - pos;
            if ( n < _N )
                _N = n;
            if ( this == &other )
            {
                erase(static_cast<size_type>(pos + _N));
                erase(0, pos);
            }
            else if ( 0 < _N && _N == other.size() && _Refcnt( other.c_str() ) < _FROZEN - 1)
            {
                _Tidy_deallocate();
                _Ptr = other._Ptr;
                _Len = other.size();
                _Res = other.capacity();
                ++_Refcnt( _Ptr );
            }
            else if ( _Grow<false>( _N, true ) )
            {
                nh3api::constexpr_char_traits::copy( _Ptr, &other.c_str()[pos], _N );
                _Eos( _N );
            }
            return (*this);
        }
        exe_string& assign( const value_type* str, size_type _N )
        {
            if ( _Grow<false>( _N, true ) )
            {
                nh3api::constexpr_char_traits::copy( _Ptr, str, _N );
                _Eos( _N );
            }
            return (*this);
        }
        exe_string& assign( const value_type* str )
        {
            return (assign( str, nh3api::constexpr_char_traits::length( str ) ));
        }
        exe_string& assign( size_type _N, value_type c )
        {
            if ( _N == npos )
            {
                return _Throw_out_of_range_exception(), *this;
            }
            if ( _Grow<false>( _N, true ) )
            {
                nh3api::constexpr_char_traits::assign( _Ptr, _N, c );
                _Eos( _N );
            }
            return (*this);
        }
        template<typename IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        exe_string& assign( IterT _F, IterT lhs
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        { return replace( begin(), end(), _F, lhs ); }

        exe_string& insert( size_type pos1, const exe_string& other )
        {
            return (insert( pos1, other, 0, npos ));
        }
        exe_string& insert( size_type pos1, const exe_string& other, size_type pos,
                    size_type n )
        {
            if ( _Len < pos1 || other.size() < pos )
            {
                return _Throw_out_of_range_exception(), *this;
            }
            size_type _N = other.size() - pos;
            if ( _N < n )
                n = _N;
            if (npos - _Len <= n)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            if (0 < n && _Grow<false>(_N = _Len + n))
            {
                nh3api::constexpr_char_traits::move( _Ptr + pos1 + n, _Ptr + pos1, _Len - pos1 );
                nh3api::constexpr_char_traits::copy( _Ptr + pos1, &other.c_str()[pos], n );
                _Eos( _N );
            }
            return (*this);
        }
        exe_string& insert( size_type pos1, const value_type* str, size_type n )
        {
            if ( _Len < pos1 )
            {
                return _Throw_out_of_range_exception(), *this;
            }
            if (npos - _Len <= n)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            size_type _N;
            if ( 0 < n && _Grow<false>( _N = _Len + n ) )
            {
                nh3api::constexpr_char_traits::move( _Ptr + pos1 + n, _Ptr + pos1, _Len - pos1 );
                nh3api::constexpr_char_traits::copy( _Ptr + pos1, str, n );
                _Eos( _N );
            }
            return (*this);
        }
        exe_string& insert( size_type pos1, const value_type* str )
        {
            return (insert( pos1, str, nh3api::constexpr_char_traits::length( str ) ));
        }
        exe_string& insert( size_type pos1, size_type n, value_type c )
        {
            if ( _Len < pos1 )
            {
                return _Throw_out_of_range_exception(), *this;
            }
            if ( npos - _Len <= n )
            {
                return _Throw_out_of_range_exception(), *this;
            }
            size_type _N;
            if ( 0 < n && _Grow<false>( _N = _Len + n ) )
            {
                nh3api::constexpr_char_traits::move( _Ptr + pos1 + n, _Ptr + pos1, _Len - pos1 );
                nh3api::constexpr_char_traits::assign( _Ptr + pos1, n, c );
                _Eos( _N );
            }
            return (*this);
        }
        iterator insert(const const_iterator pos, const value_type c )
        {
            size_type pos1 = _Pdif( pos, begin() );
            insert( pos1, 1, c );
            return (begin() + pos1);
        }
        iterator insert(const const_iterator pos, const size_type n, const value_type c)
        {
            size_type pos1 = _Pdif( pos, begin() );
            insert( pos1, n, c );
            return begin() + pos1;
        }
        template<class IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        iterator insert( const const_iterator pos, IterT _F, IterT lhs
        NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
        {
            size_type pos1 = _Pdif( pos, begin() );
            replace(pos, pos, _F, lhs);
            return begin() + pos1;
        }

        NH3API_FLATTEN
        void pop_back()
        { erase(end() - 1); }

        void shrink_to_fit()
        { exe_string(*this).swap(*this); }

        NH3API_INLINE_LARGE
        exe_string& erase( size_type pos1 = 0, size_type n = npos )
        {
            if (_Len < pos1)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            _Split();
            if (_Len - pos1 < n)
                n = _Len - pos1;
            if (0 < n)
            {
                nh3api::constexpr_char_traits::move(_Ptr + pos1, _Ptr + pos1 + n, _Len - pos1 - n);
                size_type _N = _Len - n;
                if (_Grow<false>(_N))
                    _Eos(_N);
            }
            return *this;
        }
        iterator erase( const const_iterator pos )
        {
            size_t n = _Pdif( pos, begin() );
            erase( n, 1 );
            return (_Psum( _Ptr, n ));
        }
        iterator erase( const const_iterator _F, const const_iterator lhs )
        {
            size_t n = _Pdif( _F, begin() );
            erase( n, _Pdif( lhs, _F ) );
            return _Psum( _Ptr, n );
        }

        void clear() NH3API_NOEXCEPT
        { _Eos(0); }

        exe_string& replace(const size_type pos1, const size_type n1, const exe_string& other )
        {
            return replace(pos1, n1, other, 0, npos);
        }

        NH3API_INLINE_LARGE
        exe_string& replace(const size_type pos1, size_type n1, const exe_string& other,
                    const size_type pos, size_type n = npos )
        {
            if (_Len < pos1 || other.size() < pos)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            if (_Len - pos1 < n1)
                n1 = _Len - pos1;
            size_type _N = other.size() - pos;
            if (_N < n)
                n = _N;
            if (npos - n <= _Len - n1)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            _Split();
            size_type _Nm = _Len - n1 - pos1;
            if (n < n1)
                nh3api::constexpr_char_traits::move(_Ptr + pos1 + n, _Ptr + pos1 + n1, _Nm);
            if ((0 < n || 0 < n1) && _Grow<false>(_N = _Len + n - n1))
            {
                if (n1 < n)
                    nh3api::constexpr_char_traits::move(_Ptr + pos1 + n, _Ptr + pos1 + n1, _Nm);
                nh3api::constexpr_char_traits::copy(_Ptr + pos1, &other.c_str()[pos], n);
                _Eos(_N);
            }
            return *this;
        }

        NH3API_INLINE_LARGE
        exe_string& replace(const size_type pos1, size_type n1, const value_type* const str,
                    const size_type n )
        {
            if (_Len < pos1)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            if (_Len - pos1 < n1)
                n1 = _Len - pos1;
            if (npos - n <= _Len - n1)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            _Split();
            size_type _Nm = _Len - n1 - pos1;
            if (n < n1)
                nh3api::constexpr_char_traits::move(_Ptr + pos1 + n, _Ptr + pos1 + n1, _Nm);
            size_type _N;
            if ((0 < n || 0 < n1) && _Grow<false>(_N = _Len + n - n1))
            {
                if (n1 < n)
                    nh3api::constexpr_char_traits::move(_Ptr + pos1 + n, _Ptr + pos1 + n1, _Nm);
                nh3api::constexpr_char_traits::copy(_Ptr + pos1, str, n);
                _Eos(_N);
            }
            return (*this);
        }

        exe_string& replace( size_type pos1, size_type n1, const value_type* str )
        {
            return (replace( pos1, n1, str, nh3api::constexpr_char_traits::length( str ) ));
        }

        NH3API_INLINE_LARGE
        exe_string& replace( size_type pos1, size_type n1,
                    size_type n, value_type c )
        {
            if (_Len < pos1)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            if (_Len - pos1 < n1)
                n1 = _Len - pos1;
            if (npos - n <= _Len - n1)
            {
                return _Throw_out_of_range_exception(), *this;
            }
            _Split();
            size_type _Nm = _Len - n1 - pos1;
            if (n < n1)
                nh3api::constexpr_char_traits::move(_Ptr + pos1 + n, _Ptr + pos1 + n1, _Nm);
            size_type _N;
            if ((0 < n || 0 < n1) && _Grow<false>(_N = _Len + n - n1))
            {
                if (n1 < n)
                    nh3api::constexpr_char_traits::move(_Ptr + pos1 + n, _Ptr + pos1 + n1, _Nm);
                nh3api::constexpr_char_traits::assign(_Ptr + pos1, n, c);
                _Eos(_N);
            }
            return *this;
        }

        exe_string& replace(const const_iterator _F, const const_iterator lhs, const exe_string& other )
        {
            return replace(_Pdif( _F, begin() ), _Pdif( lhs, _F ), other );
        }

        exe_string& replace(const const_iterator _F,
                        const const_iterator lhs,
                        const value_type* const str,
                        size_type n)
        {
            return replace(_Pdif( _F, begin() ), _Pdif( lhs, _F ), str, n );
        }

        exe_string& replace(const const_iterator _F, const const_iterator lhs, const value_type* const str)
        {
            return replace(_Pdif( _F, begin() ), _Pdif( lhs, _F ), str );
        }

        exe_string& replace( const const_iterator _F,
                            const const_iterator lhs,
                            const size_type n,
                            const value_type c )
        {
            return replace(_Pdif( _F, begin() ), _Pdif( lhs, _F ), n, c );
        }

        template<typename IterT
        NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
        exe_string& replace(const const_iterator _F1,
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
        exe_string& _ReplaceRange(const const_iterator _F1,
                                const const_iterator _L1,
                                const IterT _F2,
                                const IterT _L2,
                                nh3api::tt::true_type)
        {
            return replace(_Pdif(_F1, cbegin()), _Pdif(_L1, _F2), _F2, _Pdif(_L2, _F2));
        }

        template<class IterT>
        exe_string& _ReplaceRange(const const_iterator _F1,
                                const const_iterator _L1,
                                const IterT _F2,
                                const IterT _L2,
                                nh3api::tt::false_type)
        {
            const exe_string temp(_F2, _L2, get_allocator());
            replace(_F1, _L2, temp);
            return *this;
        }

    public:
        reference front() NH3API_NOEXCEPT
        {
            _Freeze();
            return _Ptr[0];
        }

        const_reference front() const NH3API_NOEXCEPT
        { return _Ptr[0]; }

        reference back() NH3API_NOEXCEPT
        {
            _Freeze();
            return _Ptr[_Len - 1];
        }

        const_reference back() const NH3API_NOEXCEPT
        { return _Ptr[_Len - 1]; }

        iterator begin() NH3API_NOEXCEPT
        {
            _Freeze();
            return (_Ptr);
        }
        const_iterator begin() const NH3API_NOEXCEPT
        {
            return (_Ptr);
        }
        const_iterator cbegin() const NH3API_NOEXCEPT
        {
            return (_Ptr);
        }
        iterator end() NH3API_NOEXCEPT
        {
            _Freeze();
            return ((iterator)_Psum( _Ptr, _Len ));
        }
        const_iterator end() const NH3API_NOEXCEPT
        {
            return ((const_iterator)_Psum( _Ptr, _Len ));
        }
        const_iterator cend() const NH3API_NOEXCEPT
        {
            return ((const_iterator)_Psum( _Ptr, _Len ));
        }
        reverse_iterator rbegin() NH3API_NOEXCEPT
        {
            return (reverse_iterator( end() ));
        }
        const_reverse_iterator rbegin() const NH3API_NOEXCEPT
        {
            return (const_reverse_iterator( end() ));
        }
        const_reverse_iterator crbegin() const NH3API_NOEXCEPT
        {
            return (const_reverse_iterator( end() ));
        }
        reverse_iterator rend() NH3API_NOEXCEPT
        {
            return (reverse_iterator( begin() ));
        }
        const_reverse_iterator rend() const NH3API_NOEXCEPT
        {
            return (const_reverse_iterator( begin() ));
        }
        const_reverse_iterator crend() const NH3API_NOEXCEPT
        {
            return (const_reverse_iterator( begin() ));
        }
        reference at( size_type pos1 )
        {
            if ( _Len <= pos1 )
            {
                return _Throw_out_of_range_exception(), _Ptr[pos1];
            }
            _Freeze();
            return _Ptr[pos1];
        }
        const_reference at( size_type pos1 ) const
        {
            if ( _Len <= pos1 )
            {
                return _Throw_out_of_range_exception(), _Ptr[pos1];
            }
            return _Ptr[pos1];
        }
        reference operator[]( size_type pos1 )
        #if !NH3API_DEBUG
        NH3API_NOEXCEPT
        #endif
        {
        #if !NH3API_DEBUG
            _Freeze();
            return (_Ptr[pos1]);
        #else
            return at(pos1);
        #endif
        }
        const_reference operator[]( size_type pos1 ) const
        #if !NH3API_DEBUG
        NH3API_NOEXCEPT
        #endif
        {
        #if !NH3API_DEBUG
            return (_Ptr[pos1]);
        #else
            return at(pos1);
        #endif
        }
        const value_type* c_str() const NH3API_NOEXCEPT
        {
            return _Ptr;
        }
        const value_type* data() const NH3API_NOEXCEPT
        {
            return _Ptr;
        }
        value_type* data() NH3API_NOEXCEPT
        {
            _Freeze();
            return _Ptr;
        }

        size_type length() const NH3API_NOEXCEPT
        {
            return (_Len);
        }

        size_type size() const NH3API_NOEXCEPT
        {
            return (_Len);
        }

        NH3API_NODISCARD NH3API_FORCEINLINE NH3API_CONSTEXPR
        size_type max_size() const NH3API_NOEXCEPT
        { return NH3API_MAX_HEAP_REQUEST; }

        void resize(const size_type _N, const value_type c = value_type())
        {
            const size_type old_size = size();
            if (_N <= old_size)
            {
                _Eos(_N);
            }
            else
            {
                append(_N - old_size, c);
            }
        }

        size_type capacity() const NH3API_NOEXCEPT
        {
            return _Res;
        }

        void reserve( size_type _N = 0 )
        {
            if ( _Res < _N )
                _Grow<false>( _N );
        }

        bool empty() const
        {
            return (_Len == 0);
        }
        
        size_type copy( value_type* str, size_type _N, size_type pos1 = 0 ) const
        {
            if ( _Len < pos1 )
            {
                return _Throw_out_of_range_exception(), 0;
            }
            if ( _Len - pos1 < _N )
                _N = _Len - pos1;
            if ( 0 < _N )
                nh3api::constexpr_char_traits::copy( str, _Ptr + pos1, _N );
            return (_N);
        }

        void swap( exe_string& other ) NH3API_NOEXCEPT
        {
            nh3api::trivial_swap<sizeof(*this)>(this, &other);
        }

        bool contains(value_type c) const NH3API_NOEXCEPT
        {
            for ( size_type i = 0; i < _Len; ++i )
            {
                if ( nh3api::constexpr_char_traits::eq(_Ptr[i], c) )
                    return true;
            }
            return false;
        }

        bool contains(const_pointer str) const 
        { return find(str) != npos; }

        bool starts_with(value_type c) const 
        { return !empty() && nh3api::constexpr_char_traits::eq(front(), c); }

        bool ends_with(value_type c) const 
        { return !empty() && nh3api::constexpr_char_traits::eq(back(), c); }

        size_type find(const exe_string& other, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( other.data() != nullptr && other.size() != 0 );
            return nh3api::constexpr_char_traits::str_find(data(), size(), other.data(), pos, other.size());
        }

        size_type find( char c, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            return nh3api::constexpr_char_traits::str_find(data(), size(), c, pos);
        }

        size_type find( const char* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::constexpr_char_traits::str_find(data(), size(), str, pos, n);
        }

        size_type find( const char* str, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::constexpr_char_traits::str_find(data(), size(), str, pos, nh3api::constexpr_char_traits::length(str));
        }

        size_type rfind(const exe_string& other, size_type pos = npos ) const NH3API_NOEXCEPT
        {
            assert( other.data() != nullptr && other.size() != 0 );
            return nh3api::constexpr_char_traits::str_rfind(data(), size(), other.data(), pos, other.size());
        }

        size_type rfind( char c, size_type pos = npos ) const NH3API_NOEXCEPT
        { return nh3api::constexpr_char_traits::str_rfind(data(), size(), c, pos); }

        size_type rfind( const char* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::constexpr_char_traits::str_rfind(data(), size(), str, pos, n);
        }

        size_type rfind( const char* str, size_type pos = npos ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::constexpr_char_traits::str_rfind(data(), size(), str, pos, nh3api::constexpr_char_traits::length(str));
        }

        size_type find_first_of(const exe_string& other, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( other.data() != nullptr && other.size() != 0 );
            return nh3api::constexpr_char_traits::str_find_first_of(data(), size(), other.data(), pos, other.size());
        }

        size_type find_first_of( char c, size_type pos = 0 ) const NH3API_NOEXCEPT
        { return find(c, pos); }

        size_type find_first_of( const char* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::constexpr_char_traits::str_find_first_of(data(), size(), str, pos, n);
        }

        size_type find_first_of( const char* str, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::constexpr_char_traits::str_find_first_of(data(), size(), str, pos, nh3api::constexpr_char_traits::length(str));
        }

        size_type find_last_of(const exe_string& other, size_type pos = npos ) const NH3API_NOEXCEPT
        {
            assert( other.data() != nullptr && other.size() != 0 );
            return nh3api::constexpr_char_traits::str_find_last_of(data(), size(), other.data(), pos, other.size());
        }

        size_type find_last_of( char c, size_type pos = npos ) const NH3API_NOEXCEPT
        { return rfind(c, pos); }

        size_type find_last_of( const char* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::constexpr_char_traits::str_find_last_of(data(), size(), str, pos, n);
        }

        size_type find_last_of( const char* str, size_type pos = npos ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::constexpr_char_traits::str_find_last_of(data(), size(), str, pos, nh3api::constexpr_char_traits::length(str));
        }

        size_type find_first_not_of(const exe_string& other, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( other.data() != nullptr && other.size() != 0 );
            return nh3api::constexpr_char_traits::str_find_first_not_of(data(), size(), other.data(), pos, other.size());
        }

        size_type find_first_not_of( char c, size_type pos = 0 ) const NH3API_NOEXCEPT
        { return nh3api::constexpr_char_traits::str_find_first_not_of(data(), size(), c, pos); }

        size_type find_first_not_of( const char* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::constexpr_char_traits::str_find_first_not_of(data(), size(), str, pos, n);
        }

        size_type find_first_not_of( const char* str, size_type pos = 0 ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::constexpr_char_traits::str_find_first_not_of(data(), size(), str, pos, nh3api::constexpr_char_traits::length(str));
        }

        size_type find_last_not_of(const exe_string& other, size_type pos = npos ) const NH3API_NOEXCEPT
        {
            assert( other.data() != nullptr && other.size() != 0 );
            return nh3api::constexpr_char_traits::str_find_last_not_of(data(), size(), other.data(), pos, other.size());
        }

        size_type find_last_not_of( char c, size_type pos = 0 ) const NH3API_NOEXCEPT
        { return nh3api::constexpr_char_traits::str_find_last_not_of(data(), size(), c, pos); }

        size_type find_last_not_of( const char* str, size_type pos, size_type n ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr && n != 0 );
            return nh3api::constexpr_char_traits::str_find_last_not_of(data(), size(), str, pos, n);
        }

        size_type find_last_not_of( const char* str, size_type pos = npos ) const NH3API_NOEXCEPT
        {
            assert( str != nullptr );
            return nh3api::constexpr_char_traits::str_find_last_not_of(data(), size(), str, pos, nh3api::constexpr_char_traits::length(str));
        }
        
        #if NH3API_CHECK_CPP23

        exe_string substr( size_type pos = 0, size_type n = npos ) const&
        { return exe_string( *this, pos, n ); }

        exe_string substr( size_type pos = 0, size_type n = npos ) &&
        { return exe_string( std::move(*this), pos, n ); }

        #else

        exe_string substr( size_type pos = 0, size_type n = npos ) const
        { return exe_string( *this, pos, n ); }

        #endif

        int compare( const exe_string& other ) const
        {
            return (compare( 0, _Len, other.c_str(), other.size() ));
        }
        int compare( size_type pos1, size_type n1,
                    const exe_string& other ) const
        {
            return (compare( pos1, n1, other, 0, npos ));
        }
        int compare( size_type pos1, size_type n1, const exe_string& other,
                    size_type pos, size_type n ) const
        {
            if ( other.size() < pos )
            {
                return _Throw_out_of_range_exception(), this->size();
            }
            if ( other._Len - pos < n )
                n = other._Len - pos;
            return (compare( pos1, n1, other.c_str() + pos, n ));
        }
        int compare( const value_type* str ) const
        {
            return (compare( 0, _Len, str, nh3api::constexpr_char_traits::length( str ) ));
        }
        int compare( size_type pos1, size_type n1, const value_type* str ) const
        {
            return (compare( pos1, n1, str, nh3api::constexpr_char_traits::length( str ) ));
        }
        int compare( size_type pos1, size_type n1, const value_type* str,
                    size_type n ) const
        {
            if ( _Len < pos1 )
            {
                return _Throw_out_of_range_exception(), this->size();
            }
            if ( _Len - pos1 < n1 )
                n1 = _Len - pos1;
            size_type _Ans = nh3api::constexpr_char_traits::compare( _Psum( _Ptr, pos1 ), str, n1 < n ? n1 : n );
            return (_Ans != 0 ? _Ans : n1 < n ? -1
                    : n1 == n ? 0 : +1);
        }

        allocator_type get_allocator() const
        { return allocator_type(); }

    private:
        enum : uint32_t { _MIN_SIZE = 31 };

        // copy _Oldlen elements to newly allocated buffer
        NH3API_INLINE_LARGE
        void _Copy( size_type _Newsize ) NH3API_NOEXCEPT
        {
            size_type _Clamped_newsize = _Newsize | _MIN_SIZE;
            if ( max_size() < _Clamped_newsize )
                _Clamped_newsize = _Newsize;
            value_type* str = static_cast<value_type*>(::operator new( _Clamped_newsize + 2, exe_heap, std::nothrow ));

            if ( 0 < _Len )
                nh3api::constexpr_char_traits::copy( str + 1, _Ptr, _Len > _Clamped_newsize ? _Clamped_newsize : _Len );
            size_type _Old_length = _Len;
            _Tidy_deallocate();
            _Ptr = str + 1;
            _Refcnt( _Ptr ) = 0;
            _Res = _Clamped_newsize;
            _Eos( _Old_length > _Clamped_newsize ? _Clamped_newsize : _Old_length );
        }

        // set new length and null terminator
        void _Eos( size_type _N ) NH3API_NOEXCEPT
        {
            nh3api::constexpr_char_traits::assign( _Ptr[_Len = _N], value_type(0) );
        }

        void _Freeze() NH3API_NOEXCEPT
        {
            if ( _Ptr != nullptr
                && _Refcnt( _Ptr ) != 0 && _Refcnt( _Ptr ) != _FROZEN )
                _Grow<true>( _Len );
            if ( _Ptr != nullptr )
                _Refcnt( _Ptr ) = _FROZEN;
        }

        template<bool is_noexcept>
        NH3API_INLINE_LARGE
        // ensure buffer is big enough, trim to size if _Trim is true
        bool _Grow( size_type _N, bool _Trim = false )
        NH3API_NOEXCEPT_EXPR(is_noexcept)
        {
            NH3API_IF_CONSTEXPR (!is_noexcept)
            {
            if ( max_size() < _N )
            {
                return _Throw_out_of_range_exception(), false;
            }    
            }
            if ( _Ptr != nullptr
                && _Refcnt( _Ptr ) != 0 && _Refcnt( _Ptr ) != _FROZEN )
                {
                    if ( _N == 0 )
                    {
                        --_Refcnt( _Ptr );
                        _Tidy();
                        return false;
                    }
                    else
                    {
                        _Copy( _N );
                        return true;
                    }
                }

            if ( _N == 0 )
            {
                if ( _Trim )
                    _Tidy_deallocate();
                else if ( _Ptr != nullptr )
                    _Eos( 0 );
                return false;
            }
            else
            {
                if ( _Trim && (_MIN_SIZE < _Res || _Res < _N) )
                {
                    _Tidy_deallocate();
                    _Copy( _N );
                }
                else if ( !_Trim && _Res < _N )
                    _Copy( _N );
                return true;
            }
        }

        static size_type _Pdif( const_pointer _P2, const_pointer _P1 ) NH3API_NOEXCEPT
        {
            return (_P2 == nullptr ? 0 : _P2 - _P1);
        }

        static const_pointer _Psum( const_pointer pos, size_type _N ) NH3API_NOEXCEPT
        {
            return (pos == nullptr ? nullptr : pos + _N);
        }

        static pointer _Psum( pointer pos, size_type _N ) NH3API_NOEXCEPT
        {
            return (pos == nullptr ? nullptr : pos + _N);
        }

        NH3API_FORCEINLINE
        uint8_t& _Refcnt( const value_type* _U ) NH3API_NOEXCEPT
        {
            return ((uint8_t*)(_U))[-1];
        }

        void _Split()
        {
            if ( _Ptr != nullptr && _Refcnt( _Ptr ) != 0 && _Refcnt( _Ptr ) != _FROZEN )
            {
                const value_type* _Temp = _Ptr;
                _Tidy_deallocate();
                assign( _Temp );
            }
        }

        NH3API_FLATTEN
        // initialize buffer, deallocating any storage
        void _Tidy( bool _Built = false) NH3API_NOEXCEPT
        {
            if ( !_Built || _Ptr == nullptr )
                ;
            else if ( _Refcnt( _Ptr ) == 0 || _Refcnt( _Ptr ) == _FROZEN )
                ::operator delete( _Ptr - 1, exe_heap );
            else
                --_Refcnt( _Ptr );

            nh3api::trivial_zero<sizeof(*this)>(this);
        }

        NH3API_FORCEINLINE
        void _Tidy_deallocate() NH3API_NOEXCEPT
        {
            if ( _Ptr == nullptr )
                ;
            else if ( _Refcnt( _Ptr ) == 0 || _Refcnt( _Ptr ) == _FROZEN )
                ::operator delete( _Ptr - 1, exe_heap );
            else
                --_Refcnt( _Ptr );
        }

    #if NH3API_CHECK_MSVC_DRIVER

    protected:
        // tell the ref count for natvis
        uint8_t _natvis_RefCount()
        { return _Refcnt( _Ptr ); }

    #endif

    protected:
        uint32_t _Dummy;
        value_type* _Ptr; // data()
        size_type _Len; // size()
        size_type _Res; // capacity()
};
#pragma pack(pop)

#if !NH3API_STD_INLINE_VARIABLES
const typename exe_string::size_type exe_string::npos = typename exe_string::size_type(-1);
#endif

//} // namespace nh3api

#if !NH3API_STD_MOVE_SEMANTICS
NH3API_FORCEINLINE
void swap(const exe_string& lhs,
          const exe_string& rhs) // ADL swap
{ lhs.swap(rhs); }
#endif

NH3API_SIZE_ASSERT(0x10, exe_string);

// concatenation

NH3API_FORCEINLINE
exe_string operator+(const exe_string& lhs, const exe_string& rhs)
{
    exe_string result;
    result.reserve(lhs.size() + rhs.size());
    result += lhs;
    result += rhs;
    return result;
}

NH3API_FORCEINLINE
exe_string operator+(const char* const lhs, const exe_string& rhs)
{
    exe_string result;
    result.reserve(nh3api::constexpr_char_traits::length(lhs) + rhs.size());
    result += lhs;
    result += rhs;
    return result;
}

NH3API_FORCEINLINE
exe_string operator+(const char lhs, const exe_string& rhs)
{
    exe_string result;
    result.reserve(1 + rhs.size());
    result += lhs;
    result += rhs;
    return result;
}

NH3API_FORCEINLINE
exe_string operator+(const exe_string& lhs, const char* const rhs)
{
    exe_string result;
    result.reserve(lhs.size() + nh3api::constexpr_char_traits::length(rhs));
    result += lhs;
    result += rhs;
    return result;
}

NH3API_FORCEINLINE
exe_string operator+(const exe_string& lhs, const char rhs)
{
    exe_string result;
    result.reserve(lhs.size() + 1);
    result += lhs;
    result += rhs;
    return result;
}

#if NH3API_STD_MOVE_SEMANTICS

NH3API_FORCEINLINE exe_string
operator+(const exe_string &lhs, exe_string&& rhs)
{ return std::move(rhs.insert(0, lhs)); }

NH3API_FORCEINLINE exe_string
operator+(exe_string&& lhs, const exe_string& rhs)
{ return std::move(lhs.append(rhs)); }

NH3API_FORCEINLINE exe_string
operator+(exe_string&& lhs, exe_string&& rhs)
{
    if (rhs.size() <= lhs.capacity() - lhs.size() || rhs.capacity() - rhs.size() < lhs.size())
        return std::move(lhs.append(rhs));
    else
        return std::move(rhs.insert(0, lhs));
}

NH3API_FORCEINLINE
exe_string operator+(const char* const lhs, exe_string&& rhs)
{ return std::move(rhs.insert(0, lhs)); }

NH3API_FORCEINLINE
exe_string operator+(const char lhs, exe_string&& rhs)
{
    return std::move(rhs.insert(static_cast<size_t>(0), static_cast<size_t>(1), lhs));
}

NH3API_FORCEINLINE
exe_string operator+(exe_string&& lhs, const char* const rhs)
{ return std::move(lhs.append(rhs)); }

NH3API_FORCEINLINE
exe_string operator+(exe_string&& lhs, const char rhs)
{
    lhs.push_back(rhs);
    return std::move(lhs);
}

#endif

// relational operators

NH3API_FORCEINLINE
bool operator==(const exe_string& lhs, const exe_string& rhs)
{ return (lhs.compare(rhs) == 0); }

NH3API_FORCEINLINE
bool operator==(const char* lhs, const exe_string& rhs)
{ return (rhs.compare(lhs) == 0); }

NH3API_FORCEINLINE
bool operator==(const exe_string& lhs, const char* rhs)
{ return (lhs.compare(rhs) == 0); }

NH3API_FORCEINLINE
bool operator<(const exe_string& lhs, const exe_string& rhs)
{ return lhs.compare(rhs) < 0; }

NH3API_FORCEINLINE
bool operator<(const char* const lhs, const exe_string& rhs)
{ return rhs.compare(lhs) > 0; }

NH3API_FORCEINLINE
bool operator<(const exe_string& lhs, const char* const rhs)
{ return lhs.compare(rhs) < 0; }

NH3API_FORCEINLINE
bool operator>(const exe_string& lhs, const exe_string& rhs)
{ return rhs < lhs; }

NH3API_FORCEINLINE
bool operator>(const char* const lhs, const exe_string& rhs)
{ return rhs < lhs; }

NH3API_FORCEINLINE
bool operator>(const exe_string& lhs, const char* const rhs)
{ return rhs < lhs; }

NH3API_FORCEINLINE
bool operator<=(const exe_string& lhs, const exe_string& rhs)
{ return !(rhs < lhs); }

NH3API_FORCEINLINE
bool operator<=(const char* const lhs, const exe_string& rhs)
{ return !(rhs < lhs); }

NH3API_FORCEINLINE
bool operator<=(const exe_string& lhs, const char* const rhs) 
{ return !(rhs < lhs); }

NH3API_FORCEINLINE
bool operator>=(const exe_string& lhs, const exe_string& rhs) 
{ return !(lhs < rhs); }

NH3API_FORCEINLINE
bool operator>=(const char* const lhs, const exe_string& rhs) 
{ return !(lhs < rhs); }

NH3API_FORCEINLINE
bool operator>=(const exe_string& lhs, const char* const rhs) 
{ return !(lhs < rhs); }

NH3API_FORCEINLINE
bool operator!=(const exe_string& lhs, const exe_string& rhs)
{ return (!(lhs == rhs)); }

NH3API_FORCEINLINE
bool operator!=(const char* lhs, const exe_string& rhs)
{ return (!(lhs == rhs)); }

NH3API_FORCEINLINE
bool operator!=(const exe_string& lhs, const char* rhs)
{ return (!(lhs == rhs)); }

#ifdef __cpp_user_defined_literals
namespace exe_string_literals
{
NH3API_FORCEINLINE
exe_string operator""_exe( const char* str, size_t len )
{ return exe_string{str, len}; }
}
#endif

namespace nh3api
{

// get exe_string reference count
// if you need it for whatever reason
NH3API_FORCEINLINE uint8_t refcount(const ::exe_string& str) NH3API_NOEXCEPT
{ return *(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(str.c_str())) - 1); }

NH3API_FORCEINLINE NH3API_CONSTEXPR size_t 
safe_strlen(const char* str, const size_t max_size) NH3API_NOEXCEPT
{
    size_t len = 0;
    while (len < max_size && str[len] != '\0') 
        ++len;
    
    return len;
}

NH3API_FORCEINLINE NH3API_CONSTEXPR size_t 
safe_strlen(const wchar_t* str, const size_t max_size) NH3API_NOEXCEPT
{
    size_t len = 0;
    while (len < max_size && str[len] != L'\0') 
        ++len;
    
    return len;
}

NH3API_CONSTEXPR NH3API_FORCEINLINE
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
char tolower_constexpr(const char c) NH3API_NOEXCEPT
{ return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c; }

struct case_insensitive_traits
{
    typedef char char_type;
    typedef int  int_type;
    typedef ::std::streamoff off_type;
    typedef ::std::streampos pos_type;
    typedef ::std::mbstate_t state_type;
    #if NH3API_CHECK_CPP20
    using comparison_category = ::std::strong_ordering;
    #endif

    static NH3API_CONST NH3API_CONSTEXPR bool eq(char c1, char c2) NH3API_NOEXCEPT
    {
        return tolower_constexpr(c1) == tolower_constexpr(c2);
    }

    static NH3API_CONST NH3API_CONSTEXPR bool ne(char c1, char c2) NH3API_NOEXCEPT
    {
        return tolower_constexpr(c1) != tolower_constexpr(c2);
    }

    static NH3API_CONST NH3API_CONSTEXPR bool lt(char c1, char c2) NH3API_NOEXCEPT
    {
        return tolower_constexpr(c1) < tolower_constexpr(c2);
    }

    static NH3API_CONSTEXPR int compare(const char* s1, const char* s2, size_t n) NH3API_NOEXCEPT
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

    static NH3API_CONSTEXPR const char* find(const char* s, int n, char a) NH3API_NOEXCEPT
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
struct float_printer; // undefined

// I HAVE to repeat the same code again
// and put it in the struct
// to make it compile under C++98...

template<class StringT>
struct float_printer<StringT, char>
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
struct float_printer<StringT, wchar_t>
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
    switch (::nh3api::fpclassify(x))
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
            return float_printer<StringT>::print(x, precision);
        }
        default:
            return StringT(get_error_fp_string(CharT()), 5);
    }
}

template<class StringT>
static StringT print_double(double x, size_t precision)
{
    typedef typename StringT::value_type CharT;
    switch (::nh3api::fpclassify(x))
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
            return float_printer<StringT>::print(x, precision);
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

namespace nh3api 
{

NH3API_FORCEINLINE size_t hash_string(const ::exe_string& str) NH3API_NOEXCEPT 
{
    default_hash hasher;
    hasher.update(str.c_str(), str.size());
    return hasher.digest();
} 

NH3API_FORCEINLINE size_t hash_string(const ::std::string& str) NH3API_NOEXCEPT 
{
    default_hash hasher;
    hasher.update(str.c_str(), str.size());
    return hasher.digest();
} 

NH3API_FORCEINLINE size_t hash_string(const ::std::wstring& str) NH3API_NOEXCEPT 
{
    default_hash hasher;
    hasher.update(str.c_str(), str.size());
    return hasher.digest();
} 

} // namespace nh3api

#if NH3API_STD_HASH
// std::hash support for exe_string
template<>
struct std::hash<exe_string>
{
    public:
        size_t operator()(const exe_string& str) NH3API_NOEXCEPT
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
