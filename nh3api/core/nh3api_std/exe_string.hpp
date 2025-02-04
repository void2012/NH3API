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
#include <utility>       // std::swap, std::forward
#include "algorithm.hpp" // constexpr algorithms
#include "intrin.hpp"    // constexpr string functions
#include "math.hpp"      // nh3api::fpclassify, float classification macros
#include "memory.hpp"    // allocators
#include "iterator.hpp"  // std::reverse_iterator
#include "nh3api_std.hpp"

#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
#include <stdexcept>     // std::length_error, std::out_of_range
#endif

NH3API_DISABLE_WARNING_BEGIN("-Wattributes", 4714)
NH3API_DISABLE_WARNING_BEGIN("-Wuninitialized", 26495)

template
<
typename _E,
typename _Tr = std::char_traits<_E>,
typename _A = exe_allocator<_E>
>
struct exe_string_helper
{
    public:
        typedef _A allocator_type;
        typedef _Tr traits_type;

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
            nh3api::str_func_chooser<char>::_memmove(dst, src, n);
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
            nh3api::str_func_chooser<wchar_t>::_memmove(dst, src, n);
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

//namespace nh3api
//{
#pragma pack(push, 4)
template
<
typename _E,
typename _Tr = std::char_traits<_E>,
typename _A = exe_allocator<_E>
>
class exe_basic_string
{
protected:
    typedef exe_basic_string<_E, _Tr, _A> this_type;
    template<class IterT>
        struct is_cptr : public nh3api::tt::integral_constant<bool,
               nh3api::tt::is_same<IterT, const _E* const>::value
            || nh3api::tt::is_same<IterT, _E* const>::value
            || nh3api::tt::is_same<IterT, const _E*>::value
            || nh3api::tt::is_same<IterT, _E*>::value>
        {};

    protected:
    typedef exe_string_helper<_E, _Tr, _A> helper_type;
    typedef typename helper_type::propagate_on_container_copy_assignment propagate_on_container_copy_assignment;
    typedef typename helper_type::propagate_on_container_move_assignment propagate_on_container_move_assignment;
    typedef typename helper_type::propagate_on_container_swap propagate_on_container_swap;

    // external typedefs
    public:
        typedef _A allocator_type;
        typedef _Tr traits_type;
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
    static std::length_error _length_exception()
    {
        NH3API_IF_CONSTEXPR ( nh3api::tt::is_same<value_type, wchar_t>::value )
            return std::length_error("wstring too long");
        else
            return std::length_error("string too long");
    }
    static void _throw_length_exception()
    {
        NH3API_THROW(_length_exception());
    }
    #else
    static void _throw_length_exception()
    {
        NH3API_THROW(0);
    }
    #endif

    #ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
    NH3API_FORCEINLINE
    static std::out_of_range _out_of_range_exception()
    {
        NH3API_IF_CONSTEXPR ( nh3api::tt::is_same<value_type, wchar_t>::value )
            return std::out_of_range("invalid wstring position");
        else
            return std::out_of_range("invalid string position");
    }
    static void _throw_out_of_range_exception()
    {
        NH3API_THROW(_out_of_range_exception());
    }
    #else
    static void _throw_out_of_range_exception()
    {
        NH3API_THROW(0);
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

    exe_basic_string(const exe_basic_string& _X)
        : helper( _X.helper.select_on_container_copy_construction() ),
        _Ptr(nullptr), _Len(0), _Res(0)
    {
        assign(_X, 0, exe_basic_string::npos);
    }

    exe_basic_string(const exe_basic_string& _X, size_type _P, size_type _M,
                    const allocator_type& _Al = allocator_type() )
        : helper( _Al ), _Ptr(nullptr), _Len(0), _Res(0)
    {
        assign( _X, _P, _M );
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
            // GCC and Clang optimizers cause segfault on this
            *reinterpret_cast<__m128i*>(this) = _mm_setzero_si128();
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
        if ( nh3api::tt::is_empty<allocator_type>::value )
        {
            __m128i* _this_m128i = reinterpret_cast<__m128i*>(this);
            __m128i* _other_m128i = reinterpret_cast<__m128i*>(other);
            *_this_m128i = *_other_m128i;
            *_other_m128i = _mm_setzero_si128();
        }
        else
        {
            this->helper = std::move(other->helper);
            this->_Ptr   = nh3api::exchange(other->_Ptr, nullptr);
            this->_Len   = nh3api::exchange(other->_Len, 0);
            this->_Res   = nh3api::exchange(other->_Res, 0);
        }
    }
    #endif

    template <class IterT>
    void _Construct(IterT _F, IterT _L, std::input_iterator_tag)
    {
        NH3API_TRY
        {
            for (; _F != _L; ++_F)
                push_back(static_cast<_E>(*_F));
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

    void _Construct(_E* const _F, _E* const _L, std::random_access_iterator_tag)
    {
        if (_F != _L)
            assign(_F, static_cast<size_type>(_L - _F));
    }

    void _Construct(const _E* const _F, const _E* const _L, std::random_access_iterator_tag)
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

    exe_basic_string(const nh3api::dummy_tag_t& tag)
        : helper(tag)
    {
        NH3API_IGNORE(_Ptr, _Len, _Res);
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

    NH3API_FLATTEN
    ~exe_basic_string()
    {
        _Tidy_deallocate();
    }

protected:
    enum _Mref : unsigned char { _FROZEN = 255 };

public:
    static const size_type npos;
    exe_basic_string& operator=( const exe_basic_string& _X )
    {
        return assign( _X );
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
    exe_basic_string& operator+=( const exe_basic_string& _X )
    {
        return append( _X );
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

    exe_basic_string& append( const exe_basic_string& _X )
    {
        return append( _X, 0, exe_basic_string::npos );
    }
    exe_basic_string &append(const exe_basic_string &_X, size_type _P, size_type _M = exe_basic_string::npos)
    {
        if (_X.size() < _P)
        {
            return _throw_out_of_range_exception(), *this;
        }
        size_type _N = _X.size() - _P;
        if (_N < _M)
            _M = _N;
        if (exe_basic_string::npos - _Len <= _M)
        {
            return _throw_length_exception(), *this;
        }
        if (0 < _M && _Grow(_N = _Len + _M))
        {
            helper_type::copy(_Ptr + _Len, &_X.c_str()[_P], _M);
            _Eos(_N);
        }
        return (*this);
    }
    exe_basic_string& append( const value_type* _S, size_type _M )
    {
        if (exe_basic_string::npos - _Len <= _M)
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
        if (exe_basic_string::npos - _Len <= _M)
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

    exe_basic_string& assign( const exe_basic_string& _X )
    {
        return (assign( _X, 0, exe_basic_string::npos ));
    }
    NH3API_INLINE_LARGE
    exe_basic_string& assign( const exe_basic_string& _X, size_type _P, size_type _M )
    {
        if ( _X.size() < _P )
        {
            return _throw_out_of_range_exception(), *this;
        }
        size_type _N = _X.size() - _P;
        if ( _M < _N )
            _N = _M;
        if ( this == &_X )
        {
            erase(static_cast<size_type>(_P + _N));
            erase(0, _P);
        }
        else if ( 0 < _N && _N == _X.size()
                && _Refcnt( _X.c_str() ) < _FROZEN - 1
                && (helper.alloc == _X.helper.alloc) )
        {
            _Tidy( true );
            _Ptr = _X._Ptr;
            _Len = _X.size();
            _Res = _X.capacity();
            ++_Refcnt( _Ptr );
        }
        else if ( _Grow( _N, true ) )
        {
            helper_type::copy( _Ptr, &_X.c_str()[_P], _N );
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
        if ( _N == exe_basic_string::npos )
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

    exe_basic_string& insert( size_type _P0, const exe_basic_string& _X )
    {
        return (insert( _P0, _X, 0, exe_basic_string::npos ));
    }
    exe_basic_string& insert( size_type _P0, const exe_basic_string& _X, size_type _P,
                size_type _M )
    {
        if ( _Len < _P0 || _X.size() < _P )
        {
            return _throw_out_of_range_exception(), *this;
        }
        size_type _N = _X.size() - _P;
        if ( _N < _M )
            _M = _N;
        if (exe_basic_string::npos - _Len <= _M)
        {
            return _throw_length_exception(), *this;
        }
        if (0 < _M && _Grow(_N = _Len + _M))
        {
            helper_type::move( _Ptr + _P0 + _M, _Ptr + _P0, _Len - _P0 );
            helper_type::copy( _Ptr + _P0, &_X.c_str()[_P], _M );
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
        if (exe_basic_string::npos - _Len <= _M)
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
        if ( exe_basic_string::npos - _Len <= _M )
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
    iterator insert(const const_iterator _P, const value_type _C )
    {
        size_type _P0 = _Pdif( _P, begin() );
        insert( _P0, 1, _C );
        return (begin() + _P0);
    }
    iterator insert(const const_iterator _P, const size_type _M, const value_type _C)
    {
        size_type _P0 = _Pdif( _P, begin() );
        insert( _P0, _M, _C );
        return begin() + _P0;
    }
    template<class IterT
    NH3API_SFINAE_BEGIN(nh3api::tt::is_iterator<IterT>::value)>
    iterator insert( const const_iterator _P, IterT _F, IterT _L
    NH3API_SFINAE_END(nh3api::tt::is_iterator<IterT>::value))
    {
        size_type _P0 = _Pdif( _P, begin() );
        replace(_P, _P, _F, _L);
        return begin() + _P0;
    }

    NH3API_FLATTEN
    void pop_back()
    { erase(end() - 1); }

    NH3API_INLINE_LARGE
    exe_basic_string& erase( size_type _P0 = 0, size_type _M = exe_basic_string::npos )
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
    iterator erase( const const_iterator _P )
    {
        size_t _M = _Pdif( _P, begin() );
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

    exe_basic_string& replace(const size_type _P0, const size_type _N0, const exe_basic_string& _X )
    {
        return replace(_P0, _N0, _X, 0, exe_basic_string::npos);
    }

    NH3API_INLINE_LARGE
    exe_basic_string& replace(const size_type _P0, size_type _N0, const exe_basic_string& _X,
                const size_type _P, size_type _M = exe_basic_string::npos )
    {
        if (_Len < _P0 || _X.size() < _P)
        {
            return _throw_out_of_range_exception(), *this;
        }
        if (_Len - _P0 < _N0)
            _N0 = _Len - _P0;
        size_type _N = _X.size() - _P;
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
            helper_type::copy(_Ptr + _P0, &_X.c_str()[_P], _M);
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
        if (exe_basic_string::npos - _M <= _Len - _N0)
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
        if (exe_basic_string::npos - _M <= _Len - _N0)
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

    exe_basic_string& replace(const const_iterator _F, const const_iterator _L, const exe_basic_string& _X )
    {
        return replace(_Pdif( _F, begin() ), _Pdif( _L, _F ), _X );
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
    void swap( exe_basic_string& _X )
    NH3API_NOEXCEPT_EXPR(helper_type::propagate_on_container_swap::value
                             || helper_type::is_always_equal::value)
    {
        if ( this->helper.alloc == _X.helper.alloc )
        {
            std::swap( _Ptr, _X._Ptr );
            std::swap( _Len, _X._Len );
            std::swap( _Res, _X._Res );
        }
        else
        {
            exe_basic_string _Ts = *this;
            *this = _X;
            _X = _Ts;
        }
    }

    size_type find( const exe_basic_string& _X, size_type _P = 0 ) const
    {
        return (find( _X.c_str(), _P, _X.size() ));
    }
    size_type find( const value_type* _S, size_type _P,
                    size_type _N ) const
    {
        if ( _N == 0 && _P <= _Len )
            return (_P);
        size_type _Nm;
        if ( _P < _Len && _N <= (_Nm = _Len - _P) )
        {
            const value_type* _U, * _V;
            for ( _Nm -= _N - 1, _V = _Ptr + _P;
                (_U = helper_type::find( _V, _Nm, *_S )) != 0;
                _Nm -= _U - _V + 1, _V = _U + 1 )
                if ( helper_type::compare( _U, _S, _N ) == 0 )
                    return (_U - _Ptr);
        }
        return (exe_basic_string::npos);
    }
    size_type find( const value_type* _S, size_type _P = 0 ) const
    {
        return (find( _S, _P, helper_type::length( _S ) ));
    }
    size_type find( value_type _C, size_type _P = 0 ) const
    {
        return (find( static_cast<const value_type*>(&_C), _P, 1));
    }
    size_type rfind( const exe_basic_string& _X, size_type _P = exe_basic_string::npos ) const
    {
        return (rfind( _X.c_str(), _P, _X.size() ));
    }
    size_type rfind( const value_type* _S, size_type _P,
                    size_type _N ) const
    {
        if (_N == 0)
            return (_P < _Len ? _P : _Len);

        if (_N <= _Len)
        {
            for (const value_type *_U = _Ptr + +(_P < _Len - _N ? _P : _Len - _N);; --_U)
            {
                if (helper_type::eq(*_U, *_S) && helper_type::compare(_U, _S, _N) == 0)
                    return (_U - _Ptr);
                else if (_U == _Ptr)
                    break;
            }
        }
        return (exe_basic_string::npos);
    }
    size_type rfind( const value_type* _S, size_type _P = exe_basic_string::npos ) const
    {
        return (rfind( _S, _P, helper_type::length( _S ) ));
    }
    size_type rfind( value_type _C, size_type _P = exe_basic_string::npos ) const
    {
        return (rfind( static_cast<const value_type*>(&_C), _P, 1));
    }
    size_type find_first_of( const exe_basic_string& _X,
                            size_type _P = 0 ) const
    {
        return (find_first_of( _X.c_str(), _P, _X.size() ));
    }
    size_type find_first_of( const value_type* _S, size_type _P,
                            size_type _N ) const
    {
        if ( 0 < _N && _P < _Len )
        {
            const value_type* const _V = _Ptr + _Len;
            for ( const value_type* _U = _Ptr + _P; _U < _V; ++_U )
                if ( helper_type::find( _S, _N, *_U ) != 0 )
                    return (_U - _Ptr);
        }
        return (exe_basic_string::npos);
    }
    size_type find_first_of( const value_type* _S, size_type _P = 0 ) const
    {
        return (find_first_of( _S, _P, helper_type::length( _S ) ));
    }
    size_type find_first_of( value_type _C, size_type _P = 0 ) const
    {
        return (find( static_cast<const value_type*>(&_C), _P, 1));
    }
    size_type find_last_of( const exe_basic_string& _X,
                            size_type _P = exe_basic_string::npos ) const
    {
        return (find_last_of( _X.c_str(), _P, _X.size() ));
    }
    size_type find_last_of( const value_type* _S, size_type _P,
                            size_type _N ) const
    {
        if (0 < _N && 0 < _Len)
        {
            for (const value_type *_U = _Ptr + (_P < _Len ? _P : _Len - 1);; --_U)
            {
                if (helper_type::find(_S, _N, *_U) != 0)
                    return (_U - _Ptr);
                else if (_U == _Ptr)
                    break;
            }
        }
        return (exe_basic_string::npos);
    }
    size_type find_last_of( const value_type* _S,
                            size_type _P = exe_basic_string::npos ) const
    {
        return (find_last_of( _S, _P, helper_type::length( _S ) ));
    }
    size_type find_last_of( value_type _C, size_type _P = exe_basic_string::npos ) const
    {
        return (rfind( static_cast<const value_type*>(&_C), _P, 1 ));
    }
    size_type find_first_not_of( const exe_basic_string& _X,
                                size_type _P = 0 ) const
    {
        return (find_first_not_of( _X.c_str(), _P,
                _X.size() ));
    }
    size_type find_first_not_of( const value_type* _S, size_type _P,
                                size_type _N ) const
    {
        if ( _P < _Len )
        {
            const value_type* const _V = _Ptr + _Len;
            for ( const value_type* _U = _Ptr + _P; _U < _V; ++_U )
                if ( helper_type::find( _S, _N, *_U ) == 0 )
                    return (_U - _Ptr);
        }
        return (exe_basic_string::npos);
    }
    size_type find_first_not_of( const value_type* _S,
                                size_type _P = 0 ) const
    {
        return (find_first_not_of( _S, _P, helper_type::length( _S ) ));
    }
    size_type find_first_not_of( value_type _C, size_type _P = 0 ) const
    {
        return (find_first_not_of( static_cast<const value_type*>(&_C), _P, 1 ));
    }
    size_type find_last_not_of( const exe_basic_string& _X,
                                size_type _P = exe_basic_string::npos ) const
    {
        return (find_last_not_of( _X.c_str(), _P, _X.size() ));
    }
    size_type find_last_not_of( const value_type* _S, size_type _P,
                                size_type _N ) const
    {
        if (0 < _Len)
        {
            for (const value_type *_U = _Ptr + (_P < _Len ? _P : _Len - 1);; --_U)
            {
                if (helper_type::find(_S, _N, *_U) == 0)
                    return (_U - _Ptr);
                else if (_U == _Ptr)
                    break;
            }
        }
        return (exe_basic_string::npos);
    }
    size_type find_last_not_of( const value_type* _S,
                                size_type _P = exe_basic_string::npos ) const
    {
        return (find_last_not_of( _S, _P, helper_type::length( _S ) ));
    }
    size_type find_last_not_of( value_type _C, size_type _P = exe_basic_string::npos ) const
    {
        return (find_last_not_of( static_cast<const value_type*>(&_C), _P, 1 ));
    }
    exe_basic_string substr( size_type _P = 0, size_type _M = exe_basic_string::npos ) const
    {
        return (exe_basic_string( *this, _P, _M ));
    }
    int compare( const exe_basic_string& _X ) const
    {
        return (compare( 0, _Len, _X.c_str(), _X.size() ));
    }
    int compare( size_type _P0, size_type _N0,
                const exe_basic_string& _X ) const
    {
        return (compare( _P0, _N0, _X, 0, exe_basic_string::npos ));
    }
    int compare( size_type _P0, size_type _N0, const exe_basic_string& _X,
                size_type _P, size_type _M ) const
    {
        if ( _X.size() < _P )
        {
            return _throw_out_of_range_exception(), this->size();
        }
        if ( _X._Len - _P < _M )
            _M = _X._Len - _P;
        return (compare( _P0, _N0, _X.c_str() + _P, _M ));
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

    static const_pointer _Psum( const_pointer _P, size_type _N )
    {
        return (_P == nullptr ? nullptr : _P + _N);
    }

    static pointer _Psum( pointer _P, size_type _N )
    {
        return (_P == nullptr ? nullptr : _P + _N);
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

#if defined(_MSC_VER)
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

template<class _Elem,
    class _Traits,
    class _Alloc>
    const typename exe_basic_string<_Elem, _Traits, _Alloc>::size_type
    exe_basic_string<_Elem, _Traits, _Alloc>::npos = -1;

//} // namespace nh3api

namespace std
{

#if !NH3API_STD_MOVE_SEMANTICS
template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
void swap(const exe_basic_string<_E, _Tr, _A>& _L,
          const exe_basic_string<_E, _Tr, _A>& _R)
{
    _L.swap(_R);
}
#endif

}

typedef exe_basic_string<char, std::char_traits<char>, exe_allocator<char> > exe_string;
typedef exe_basic_string<wchar_t, std::char_traits<wchar_t>, exe_allocator<wchar_t> > exe_wstring;
// NH3API_SIZE_TEST(0x10, exe_string);

// concatenation

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
exe_basic_string<_E, _Tr, _A> operator+(
    const exe_basic_string<_E, _Tr, _A>& _L,
    const exe_basic_string<_E, _Tr, _A>& _R)
{
    exe_basic_string<_E, _Tr, _A> result;
    result.reserve(_L.size() + _R.size());
    result += _L;
    result += _R;
    return result;
}

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
exe_basic_string<_E, _Tr, _A> operator+(const _E* const _L,
                                        const exe_basic_string<_E, _Tr, _A>& _R)
{
    exe_basic_string<_E, _Tr, _A> result;
    exe_string_helper<_E, _Tr, _A> helper;
    result.reserve(helper.length(_L) + _R.size());
    result += _L;
    result += _R;
    return result;
}

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
exe_basic_string<_E, _Tr, _A> operator+(
    const _E _L, const exe_basic_string<_E, _Tr, _A>& _R)
{
    exe_basic_string<_E, _Tr, _A> result;
    result.reserve(1 + _R.size());
    result += _L;
    result += _R;
    return result;
}

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
exe_basic_string<_E, _Tr, _A> operator+(
    const exe_basic_string<_E, _Tr, _A>& _L,
    const _E* const _R)
{
    exe_basic_string<_E, _Tr, _A> result;
    exe_string_helper<_E, _Tr, _A> helper;
    result.reserve(_L.size() + helper.length(_R));
    result += _L;
    result += _R;
    return result;
}

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
exe_basic_string<_E, _Tr, _A> operator+(
    const exe_basic_string<_E, _Tr, _A>& _L, const _E _R)
{
    exe_basic_string<_E, _Tr, _A> result;
    result.reserve(_L.size() + 1);
    result += _L;
    result += _R;
    return result;
}

#if NH3API_STD_MOVE_SEMANTICS

template <class _E, class _Tr, class _A>
NH3API_FORCEINLINE exe_basic_string<_E, _Tr, _A>
operator+(const exe_basic_string<_E, _Tr, _A> &_L, exe_basic_string<_E, _Tr, _A>&& _R)
{ return std::move(_R.insert(0, _L)); }

template <class _E, class _Tr, class _A>
NH3API_FORCEINLINE exe_basic_string<_E, _Tr, _A>
operator+(exe_basic_string<_E, _Tr, _A>&& _L, const exe_basic_string<_E, _Tr, _A>& _R)
{ return std::move(_L.append(_R)); }

template <class _E, class _Tr, class _A>
NH3API_FORCEINLINE exe_basic_string<_E, _Tr, _A>
operator+(exe_basic_string<_E, _Tr, _A>&& _L, exe_basic_string<_E, _Tr, _A>&& _R)
{
    if (_R.size() <= _L.capacity() - _L.size() || _R.capacity() - _R.size() < _L.size())
        return std::move(_L.append(_R));
    else
        return std::move(_R.insert(0, _L));
}

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
exe_basic_string<_E, _Tr, _A> operator+(const _E* const _L,
                                        exe_basic_string<_E, _Tr, _A>&& _R)
{ return std::move(_R.insert(0, _L)); }

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
exe_basic_string<_E, _Tr, _A> operator+(
    const _E _L, exe_basic_string<_E, _Tr, _A>&& _R)
{
    typedef typename _A::size_type size_type;
    return std::move(_R.insert(static_cast<size_type>(0), static_cast<size_type>(1), _L));
}

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
exe_basic_string<_E, _Tr, _A> operator+(
    exe_basic_string<_E, _Tr, _A>&& _L,
    const _E* const _R)
{ return std::move(_L.append(_R)); }

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
exe_basic_string<_E, _Tr, _A> operator+(
    exe_basic_string<_E, _Tr, _A>&& _L, const _E _R)
{
    _L.push_back(_R);
    return std::move(_L);
}

#endif

// relational operators

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator==(const exe_basic_string<_E, _Tr, _A>& _L,
                const exe_basic_string<_E, _Tr, _A>& _R)
{ return (_L.compare(_R) == 0); }

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator==(const _E* _L,
                const exe_basic_string<_E, _Tr, _A>& _R)
{ return (_R.compare(_L) == 0); }

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator==(const exe_basic_string<_E, _Tr, _A>& _L,
                const _E* _R)
{ return (_L.compare(_R) == 0); }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator<(
    const exe_basic_string<_E, _Tr, _A>& _L, const exe_basic_string<_E, _Tr, _A>& _R) NH3API_NOEXCEPT
{ return _L.compare(_R) < 0; }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator<(const _E* const _L, const exe_basic_string<_E, _Tr, _A>& _R) NH3API_NOEXCEPT
{ return _R.compare(_L) > 0; }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator<(const exe_basic_string<_E, _Tr, _A>& _L, const _E* const _R) NH3API_NOEXCEPT
{ return _L.compare(_R) < 0; }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator>(
    const exe_basic_string<_E, _Tr, _A>& _L, const exe_basic_string<_E, _Tr, _A>& _R) NH3API_NOEXCEPT
{ return _R < _L; }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator>( const _E* const _L, const exe_basic_string<_E, _Tr, _A>& _R) NH3API_NOEXCEPT
{ return _R < _L; }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator>(const exe_basic_string<_E, _Tr, _A>& _L, const _E* const _R) NH3API_NOEXCEPT
{ return _R < _L; }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator<=(
    const exe_basic_string<_E, _Tr, _A>& _L, const exe_basic_string<_E, _Tr, _A>& _R) NH3API_NOEXCEPT
{ return !(_R < _L); }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator<=(const _E* const _L, const exe_basic_string<_E, _Tr, _A>& _R) NH3API_NOEXCEPT
{ return !(_R < _L); }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator<=(const exe_basic_string<_E, _Tr, _A>& _L, const _E* const _R) NH3API_NOEXCEPT
{ return !(_R < _L); }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator>=(
    const exe_basic_string<_E, _Tr, _A>& _L, const exe_basic_string<_E, _Tr, _A>& _R) NH3API_NOEXCEPT
{ return !(_L < _R); }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator>=(const _E* const _L, const exe_basic_string<_E, _Tr, _A>& _R) NH3API_NOEXCEPT
{ return !(_L < _R); }

template <class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator>=(const exe_basic_string<_E, _Tr, _A>& _L, const _E* const _R) NH3API_NOEXCEPT
{ return !(_L < _R); }

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator!=(const exe_basic_string<_E, _Tr, _A>& _L,
                const exe_basic_string<_E, _Tr, _A>& _R)
{ return (!(_L == _R)); }

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator!=(const _E* _L,
                const exe_basic_string<_E, _Tr, _A>& _R)
{ return (!(_L == _R)); }

template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
bool operator!=(const exe_basic_string<_E, _Tr, _A>& _L,
                const _E* _R)
{ return (!(_L == _R)); }

namespace nh3api
{

// get exe_basic_string reference count
// if you need it for whatever reason
template<class _E, class _Tr, class _A> NH3API_FORCEINLINE
uint8_t refcount(const exe_basic_string<_E, _Tr, _A>& str)
{ return *(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(str.c_str())) - 1); }

NH3API_CONSTEXPR inline
// constexpr atoi that uses no locale
int32_t atoi_constexpr(const char *p) NH3API_NOEXCEPT
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

// hash support for exe_string
#if NH3API_VS2012_2013 || NH3API_CHECK_CPP11
#include "hash.hpp"

namespace std 
{
    template<typename CharT, typename TraitsT, typename AllocatorT>
    class hash< exe_basic_string<CharT, TraitsT, AllocatorT> >
    {
        public:
            size_t operator()(const exe_basic_string<CharT, TraitsT, AllocatorT>& str) NH3API_NOEXCEPT
            {
                ::nh3api::default_hash hasher;
                hasher.update(str.data(), str.size());
                return hasher.digest();
            }
    };
}
#endif

NH3API_DISABLE_WARNING_END
