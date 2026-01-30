//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
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

#include <algorithm>             // std::copy, std::copy_n, std::max, std::min, std::remove
#include <array>                 // std::array
#include <cassert>               // assert
#include <cmath>                 // std::fpclassify
#include <initializer_list>      // std::initializer_list
#include <utility>               // std::swap, std::forward
#if ( __cplusplus >= 202002L ) || defined(__cpp_lib_ranges)
#include <ranges>                // std::ranges::begin, std::ranges::contiguous_range, std::ranges::data, std::ranges::forward_range, std::from_range
#endif
#include <stdexcept>             // std::length_error, std::out_of_range
#include <string>                // std::string, std::wstring, std::char_traits

#include "nh3api_exceptions.hpp" // nh3api::throw_exception
#include "hash.hpp"              // nh3api::default_hash
#include "intrin.hpp"            // constexpr string functions, std::fpclassify, nh3api::count_digits, float classification macros
#include "iterator.hpp"          // std::reverse_iterator, tt::is_iterator
#include "memory.hpp"            // allocators

NH3API_WARNING(push)
NH3API_WARNING_GNUC_DISABLE("-Wattributes")
NH3API_WARNING_GNUC_DISABLE("-Wuninitialized")
NH3API_WARNING_MSVC_DISABLE(4714)
NH3API_WARNING_MSVC_DISABLE(26495)

namespace nh3api
{
#ifdef __cpp_lib_ranges_contains
template <class Range>
concept contiguous_chars_range =
(std::ranges::contiguous_range<Range>) && std::same_as<std::remove_cvref_t<std::ranges::range_reference_t<Range>>, char>;
#endif

} // namespace nh3api

class exe_string;

template<>
struct nh3api::private_accessor<exe_string>;

// tag to select constructors used by basic_string's concatenation operators (operator+)
struct exe_string_constructor_concat_tag
{ inline explicit exe_string_constructor_concat_tag() = default; };

#pragma pack(push, 4)
// Visual C++ 6.0 implementation of std::string,
// which uses unsafe copy-on-write semantics with 8-bit reference counting(in this library, CoW is not implemented on copy, it is preserved for compatibility).
// Use only for binary compatibility with the game.
// Consider using std::basic_string or exe_std_string/exe_std_wstring which use .exe allocator
class exe_string
{
    // external typedefs
    public:
        using value_type      = char;
        using allocator_type  = exe_allocator<char>;
        using traits_type     = std::char_traits<char>;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = value_type*;
        using const_pointer   = const value_type*;
        using reference       = value_type&;
        using const_reference = const value_type&;

        using iterator               = pointer;
        using const_iterator         = const_pointer;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator       = std::reverse_iterator<iterator>;

    protected:
        template<typename _Iter>
        using is_elem_cptr = std::bool_constant<nh3api::tt::is_any_of_v<_Iter, const char* const,
                                                                               char* const,
                                                                               const char*,
                                                                               char*>>;

        template<typename _Iter>
        using is_elem_ucptr = std::bool_constant<nh3api::tt::is_any_of_v<_Iter, const unsigned char* const,
                                                                                unsigned char* const,
                                                                                const unsigned char*,
                                                                                unsigned char*>>;

        [[noreturn]] inline static void _Throw_length_exception() noexcept(false)
        { nh3api::throw_exception<std::length_error>("string too long"); }

        [[noreturn]] inline static void _Throw_out_of_range_exception() noexcept(false)
        { nh3api::throw_exception<std::out_of_range>("invalid string position"); }

    public:
        inline constexpr exe_string() noexcept
            : _Myptr  { nullptr },
              _Mysize { 0 },
              _Myres  { 0 }
        {}

        exe_string(const exe_string& _Other)
        { _Construct<_Construct_strategy::_From_string>(_Other._Myptr, _Other._Mysize); }

        exe_string(const exe_string& _Other, const size_t _Offset)
        {
            _Other._Check_offset(_Offset);
            _Construct<_Construct_strategy::_From_ptr>(_Other._Myptr + _Offset, _Other._Clamp_suffix_size(_Offset, npos));
        }

        exe_string(const exe_string& _Other, const size_t _Offset, const size_t _Count)
        {
            _Other._Check_offset(_Offset);
            _Construct<_Construct_strategy::_From_ptr>(_Other._Myptr + _Offset, _Other._Clamp_suffix_size(_Offset, _Count));
        }

        exe_string(exe_string&& _Other, const size_t _Offset)
        { _Move_construct_from_substr(_Other, _Offset, npos); }

        exe_string(exe_string&& _Other, const size_t _Offset, const size_t _Count)
        { _Move_construct_from_substr(_Other, _Offset, _Count); }

        // construct from const char* and size
        exe_string(const char* const _String, const size_t _Count)
        {
            if ( _String && _Count )
                _Construct<_Construct_strategy::_From_ptr>(_String, _Count);
            else
                _Construct_empty();
        }

        template<size_t _Size>
        inline exe_string(const char (&_String)[_Size])
            : _Mysize { _Size - 1 }
        {
            static_assert(_Size > 1);
            constexpr size_t _New_capacity = _Calculate_growth(_Size - 1, 0, max_size());
            this->_Myres                   = _New_capacity;
            char* const _New_ptr           = _Allocate_for_capacity(_New_capacity);
            this->_Myptr                   = _New_ptr + 1; // reference count is one byte before the string
            _Refcnt(this->_Myptr)          = 0;            // set reference count to zero on construction
            traits_type::copy(_Myptr, _String, _Size - 1);
            _Myptr[_Size - 1] = '\0';
        }

        // construct from const char* and calculate its length
        inline exe_string(const char* const _String)
        {
            if ( _String ) NH3API_LIKELY
                _Construct<_Construct_strategy::_From_ptr>(_String, __builtin_strlen(_String));
            else
                _Construct_empty();
        }

        exe_string(std::nullptr_t) = delete;

        // fill string with the same character n times
        exe_string(size_t _Count, char _Character)
        { _Construct<_Construct_strategy::_From_char>(_Character, _Count); }

    #if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
        template<nh3api::tt::iterator_for_container _Iter>
    #else
        template<class _Iter, std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
    #endif
        exe_string(const _Iter _First, const _Iter _Last)
        {
            nh3api::verify_range(_First, _Last);
            auto _UFirst = nh3api::unfancy(_First);
            auto _ULast  = nh3api::unfancy(_Last);
            if ( _UFirst == _ULast )
                _Construct_empty();
            else if constexpr ( is_elem_cptr<decltype(_UFirst)>::value )
                _Construct<_Construct_strategy::_From_ptr>(_UFirst, static_cast<size_t>(_ULast - _UFirst));
            else if constexpr ( is_elem_ucptr<decltype(_UFirst)>::value )
                _Construct<_Construct_strategy::_From_ptr>(reinterpret_cast<const char* const>(_UFirst), static_cast<size_t>(_ULast - _UFirst));
            else if constexpr ( nh3api::is_cpp17_fwd_iter_v<_Iter> )
                _Construct_from_iter(std::move(_UFirst), std::move(_ULast), static_cast<size_t>(std::distance(_UFirst, _ULast)));
            else
                _Construct_from_iter(std::move(_UFirst), std::move(_ULast));
        }

    #ifdef __cpp_lib_containers_ranges
    #if defined(__clang__) && defined(__INTELLISENSE__)
        template<typename _Range>
    #else
        template<nh3api::tt::container_compatible_range<char> _Range>
    #endif
        exe_string(std::from_range_t, _Range _Rng)
        {
            if constexpr (std::ranges::sized_range<_Range> || std::ranges::forward_range<_Range>)
            {
                const size_t _Count = static_cast<size_t>(std::ranges::distance(_Rng));
                if constexpr ( nh3api::contiguous_chars_range<_Range> )
                    _Construct<_Construct_strategy::_From_ptr>(std::ranges::data(_Rng), _Count);
                else
                    _Construct_from_iter(nh3api::unfancy_begin(_Rng), nh3api::unfancy_end(_Rng), _Count);
            }
            else
            {
                _Construct_from_iter(nh3api::unfancy_begin(_Rng), nh3api::unfancy_end(_Rng));
            }
        }
    #endif

        // Construct from initializer_list of chars
        exe_string(std::initializer_list<char> _Initializer_list)
        { _Construct<_Construct_strategy::_From_ptr>(_Initializer_list.begin(), _Initializer_list.size()); }

        exe_string(exe_string&& _Other) noexcept
        { _Take_contents(_Other); }

        exe_string(exe_string_constructor_concat_tag,
                   const char* const _Left_ptr,
                   const size_t      _Left_size,
                   const char* const _Right_ptr,
                   const size_t      _Right_size)
        {
            assert(_Left_size < max_size() && _Right_size < max_size());
            // we have checked for the nullptrs in the concatenation operator before
            NH3API_ASSUME(_Left_ptr != nullptr && _Right_ptr != nullptr);

            const size_t _New_size     = _Left_size + _Right_size;
            size_t       _New_capacity = _MIN_SIZE;
            _New_capacity              = _Calculate_growth(_New_size, _MIN_SIZE, max_size());
            _Myptr                     = _Allocate_for_capacity(_New_capacity);
            _Refcnt(_Myptr)            = 0;
            _Mysize                    = _New_size;
            _Myres                     = _New_capacity;
            traits_type::copy(_Myptr, _Left_ptr, _Left_size);
            traits_type::copy(_Myptr + static_cast<ptrdiff_t>(_Left_size), _Right_ptr, _Right_size);
            _Myptr[_New_size] = '\0';
        }

        exe_string(exe_string_constructor_concat_tag,
                   exe_string& _Left,
                   exe_string& _Right)
        {
            const size_t _New_size = _Left._Mysize + _Right._Mysize;
            reserve(_New_size);
            traits_type::copy(_Myptr, _Left._Myptr, _Left._Mysize);
            traits_type::copy(_Myptr + _Left._Mysize, _Right._Myptr, _Right._Mysize);
            _Eos(_New_size);
        }

        exe_string(const std::string_view _Other)
        { _Construct<_Construct_strategy::_From_ptr>(_Other.data(), _Other.size()); }

        exe_string(const std::string_view _Other, const size_t offset, const size_t count)
        {
            const std::string_view _Subview = _Other.substr(offset, count);
            _Construct<_Construct_strategy::_From_ptr>(_Subview.data(), _Subview.size());
        }

        exe_string& operator=(exe_string&& _Other) noexcept
        {
            if ( this == &_Other ) NH3API_UNLIKELY
                return *this;

            _Tidy_deallocate();
            _Take_contents(_Other);
            return *this;
        }

        inline exe_string(const nh3api::dummy_tag_t&) noexcept
        {}

        NH3API_FLATTEN ~exe_string() noexcept
        { _Tidy_deallocate(); }

    protected:
        inline static constexpr uint8_t _FROZEN = 255;

    public:
        inline static constexpr size_t npos = size_t(-1);

    public:
        exe_string& assign(exe_string&& _Other) noexcept
        {
            if ( this == &_Other ) NH3API_UNLIKELY
                return *this;

            _Tidy_deallocate();
            _Take_contents(_Other);
            return *this;
        }

        exe_string& operator=(std::initializer_list<char> _Initializer_list)
        { assign(_Initializer_list.begin(), _Initializer_list.size()); return *this; }

        exe_string& operator+=(std::initializer_list<char> _Initializer_list)
        { return append(_Initializer_list.begin(), _Initializer_list.size()); }

        exe_string& assign(std::initializer_list<char> _Initializer_list)
        { return assign(_Initializer_list.begin(), _Initializer_list.size()); }

        exe_string& append(std::initializer_list<char> _Initializer_list)
        { return append(_Initializer_list.begin(), _Initializer_list.size()); }

        iterator insert(const_pointer _Where, const std::initializer_list<char> _Initializer_list)
        {
            const size_t _Offset = static_cast<size_t>(_Where - _Myptr);
            insert(_Offset, _Initializer_list.begin(), _Initializer_list.size());
            return begin() + static_cast<ptrdiff_t>(_Offset);
        }

        exe_string& replace(const_iterator _First, const_iterator _Last, const std::initializer_list<char> _Initializer_list)
        {
            nh3api::verify_range(_First, _Last);
            const size_t _Offset = static_cast<size_t>(_First - _Myptr);
            const size_t _Length = static_cast<size_t>(_Last - _First);
            return replace(_Offset, _Length, _Initializer_list.begin(), _Initializer_list.size());
        }

        exe_string& operator=(const exe_string& _Other)
        {
            if ( this != &_Other ) NH3API_LIKELY
				assign(_Other);

			return *this;
        }

        exe_string& operator=(const std::string_view _String)
        {
            assign(_String);
            return *this;
        }

        template<typename CharPtr>
        exe_string& operator=(const CharPtr _String)
        {
            assign(_String);
            return *this;
        }

        exe_string& operator=(std::nullptr_t) = delete;

        template<size_t _Size>
        exe_string& operator=(const char (&_String)[_Size])
        {
            static_assert(_Size >= 1);
            auto        _Buf     = _Get_buffer(_Size - 1);
            char* const _New_ptr = _Buf.get();
            traits_type::copy(_New_ptr, _String, _Size - 1);
            _Buf.set(this->_Myptr);
            this->_Myptr        = _New_ptr;
            this->_Myptr[_Size - 1] = '\0';
            return *this;
        }

        exe_string& operator=(char _Character)
        {
            auto        _Buf     = _Get_buffer(1);
            char* const _New_ptr = _Buf.get() + 1;
            _New_ptr[0]          = _Character;
            _New_ptr[1]          = '\0';
            _Buf.set(this->_Myptr);
            this->_Myptr = _New_ptr;
            return *this;
        }

        exe_string& operator+=(const exe_string& _Other)
        { return append(_Other); }

        exe_string& operator+=(const char* const _String)
        { return append(_String); }

        exe_string& operator+=(std::nullptr_t) = delete;

        template<size_t _Size>
        exe_string& operator+=(const char (&_String)[_Size])
        {
            if constexpr ( _Size == 0 || _Size == 1 || _Size >= max_size() )
                return *this;

            return _Reallocate_grow_by(
                _Size - 1,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size, const char* const _Src,
                   const size_t _Src_size)
                {
                    traits_type::copy(_New_ptr, _Old_ptr, _Old_size);
                    traits_type::copy(_New_ptr + _Old_size, _Src, _Src_size);
                },
                _String, _Size - 1);
        }

        exe_string& operator+=(const std::string_view _String)
        { return append(_String); }

        exe_string& operator+=(char _Character)
        {
            push_back(_Character);
            return *this;
        }

        exe_string& append(const exe_string& _Other)
        { return append(_Other._Myptr, _Other._Mysize); }

        exe_string& append(const exe_string& _Other, size_t _Offset, size_t _Count = npos)
        {
            _Other._Check_offset(_Offset);
            _Count = _Other._Clamp_suffix_size(_Offset, _Count);
            return append(_Other._Myptr + _Offset, _Count);
        }

        exe_string& append(const std::string_view _String)
        { return append(_String.data(), _String.size()); }

        exe_string& append(const std::string_view _String, size_t _Offset, size_t _Count = npos)
        { return append(_String.substr(_Offset, _Count)); }

        exe_string& append(const char* const _String, const size_t _Count)
        {
            if ( _String == nullptr || _Count == 0 ) NH3API_UNLIKELY
                return *this;

            return _Reallocate_grow_by(
                _Count,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size,
                    const char* const _Src, const size_t _Src_count)
                {
                    NH3API_ASSUME(_Src);
                    NH3API_ASSUME(_Src_count);
                    traits_type::copy(_New_ptr, _Old_ptr, _Old_size);
                    traits_type::copy(_New_ptr + _Old_size, _Src, _Src_count);
                },
                _String, _Count);
        }

        exe_string& append(std::nullptr_t) = delete;

        template<size_t _Size>
        exe_string& append(const char (&_String)[_Size])
        {
            if constexpr ( _Size == 0 || _Size == 1 || _Size >= max_size() )
                return *this;

            return _Reallocate_grow_by(
                _Size - 1,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size, const char* const _Src,
                   const size_t _Src_size)
                {
                    NH3API_ASSUME(_Src);
                    NH3API_ASSUME(_Src_size);
                    traits_type::copy(_New_ptr, _Old_ptr, _Old_size);
                    traits_type::copy(_New_ptr + _Old_size, _Src, _Src_size);
                },
                _String, _Size - 1);
        }

        exe_string& append(const char* const _String)
        {
            if ( _String == nullptr ) NH3API_UNLIKELY
                return *this;

            const size_t _Count = __builtin_strlen(_String);
            if ( _Count == 0 ) NH3API_UNLIKELY
                return *this;

            return _Reallocate_grow_by(
                _Count,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size,
                    const char* const _Src, const size_t _Src_count)
                {
                    NH3API_ASSUME(_Src);
                    NH3API_ASSUME(_Src_count);
                    traits_type::copy(_New_ptr, _Old_ptr, _Old_size);
                    traits_type::copy(_New_ptr + _Old_size, _Src, _Src_count);
                },
                _String, _Count);
        }

        exe_string& append(size_t _Count, char _Character)
        {
            if ( _Count == 0 ) NH3API_UNLIKELY
                return *this;

            return _Reallocate_grow_by(
                _Count,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size, const size_t _Character_num,
                   const char _Ch)
                {
                    NH3API_ASSUME(_Character_num);
                    traits_type::copy(_New_ptr, _Old_ptr, _Old_size);
                    traits_type::assign(_New_ptr + _Old_size, _Character_num, _Ch);
                },
                _Count, _Character);
        }

    #if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
        template<nh3api::tt::iterator_for_container _Iter>
    #else
        template<class _Iter, std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
    #endif
        exe_string& append(const _Iter _First, const _Iter _Last)
        {
            nh3api::verify_range(_First, _Last);
            const size_t _Appended_size = std::distance(_First, _Last);
            if ( _Appended_size == 0 ) NH3API_UNLIKELY
                return *this;

            const auto _UFirst = nh3api::unfancy(_First);
            const auto _ULast = nh3api::unfancy(_Last);
            if constexpr (is_elem_cptr<decltype(_UFirst)>::value)
            {
                return append(_UFirst, static_cast<size_t>(_ULast - _UFirst));
            }
            else
            {
                return _Reallocate_grow_by(
                _Appended_size,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size,
                    auto _Src_first, auto _Src_last)
                {
                    NH3API_ASSUME(_Old_size);
                    traits_type::copy(_New_ptr, _Old_ptr, _Old_size);
                    std::copy(_Src_first, _Src_last, _New_ptr + _Old_size);
                },
                _First, _Last);
            }
        }

    #ifdef __cpp_lib_containers_ranges
    #if defined(__clang__) && defined(__INTELLISENSE__)
        template<typename _Range>
    #else
        template<nh3api::tt::container_compatible_range<char> _Range>
    #endif
        exe_string& append_range(_Range&& _Rng)
        {
            const size_t _Appended_size = std::ranges::size(_Rng);
            if ( _Appended_size == 0 ) NH3API_UNLIKELY
                return *this;

            if constexpr ( std::ranges::sized_range<_Range> && nh3api::contiguous_chars_range<_Range>)
            {
                return append(std::ranges::data(_Rng), _Appended_size);
            }
            else
            {
                return _Reallocate_grow_by(
                _Appended_size,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size,
                const auto _Rng_begin, const size_t _Rng_size)
                {
                    NH3API_ASSUME(_Old_size);
                    traits_type::copy(_New_ptr, _Old_ptr, _Old_size);
                    std::ranges::copy_n(_Rng_begin, _Rng_size, _New_ptr + _Old_size);
                },
                std::ranges::begin(_Rng), _Appended_size);
            }
        }
    #endif // C++23

        exe_string& assign(const exe_string& _Other)
        {
            if ( this == &_Other ) NH3API_UNLIKELY
                return *this;

            return assign(_Other._Myptr, _Other._Mysize);
        }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral SizeType = size_t>
    #else
        template<typename SizeType = size_t, std::enable_if_t<std::is_integral_v<SizeType> && !std::is_same_v<SizeType,bool>, int> = 0 >
    #endif
        exe_string& assign(const exe_string& _Other, const SizeType _Offset, SizeType _Count)
        {
            _Other._Check_offset(_Offset);
            _Count = _Other._Clamp_suffix_size(_Offset, _Count);
            return assign(_Other._Myptr + _Offset, _Count);
        }

        exe_string& assign(const std::string_view _String)
        { return assign(_String.data(), _String.size()); }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral SizeType = size_t>
    #else
        template<typename SizeType = size_t, std::enable_if_t<std::is_integral_v<SizeType> && !std::is_same_v<SizeType,bool>, int> = 0 >
    #endif
        exe_string& assign(const std::string_view _String, const SizeType _Offset, const SizeType _Count = npos)
        { return assign(_String.substr(_Offset, _Count)); }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral SizeType = size_t>
    #else
        template<typename SizeType = size_t, std::enable_if_t<std::is_integral_v<SizeType> && !std::is_same_v<SizeType,bool>, int> = 0 >
    #endif
        exe_string& assign(const char* _String, const SizeType _Count)
        {
            if ( _String == nullptr || _Count == 0 ) NH3API_UNLIKELY
                return *this;

            return _Reallocate_for(_Count,
            NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const size_t _Src_count, const char* const _Src)
            {
                NH3API_ASSUME(_Src);
                NH3API_ASSUME(_Src_count);
                traits_type::copy(_New_ptr, _Src, _Src_count);
            }, _String);
        }

        exe_string& assign(std::nullptr_t) = delete;

        template<size_t _Size>
        exe_string& assign(const char (&_String)[_Size])
        {
            if constexpr ( _Size == 0 || _Size == 1 || _Size >= max_size() )
                return *this;

            return _Reallocate_for(_Size - 1,
            NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const size_t _Count, const char* const _Src)
            {
                NH3API_ASSUME(_Src);
                traits_type::copy(_New_ptr, _Src, _Count);
            }, _String);
        }

        exe_string& assign(const char* const _String)
        {
            if ( _String == nullptr ) NH3API_UNLIKELY
                return *this;

            const size_t _Length = __builtin_strlen(_String);

            return _Reallocate_for(_Length,
            NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const size_t _Count, const char* const _Src)
            {
                NH3API_ASSUME(_Src);
                traits_type::copy(_New_ptr, _Src, _Count);
            }, _String);
        }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral SizeType = size_t>
    #else
        template<typename SizeType = size_t, std::enable_if_t<std::is_integral_v<SizeType> && !std::is_same_v<SizeType,bool>, int> = 0 >
    #endif
        exe_string& assign(SizeType _Count, char _Character)
        {
            if ( _Count == 0 ) NH3API_UNLIKELY
                return *this;

            return _Reallocate_for(
            _Count,
            NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const size_t _Characters_num, const char _Ch)
            {
                NH3API_ASSUME(_Characters_num);
                traits_type::assign(_New_ptr, _Characters_num, _Ch);
            },
            _Character);
        }

    #if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
        template<nh3api::tt::iterator_for_container _Iter>
    #else
        template<class _Iter, std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
    #endif
        exe_string& assign(const _Iter _First, const _Iter _Last)
        {
            nh3api::verify_range(_First, _Last);
            const size_t _New_size = std::distance(_First, _Last);
            if ( _New_size == 0 ) NH3API_UNLIKELY
                return *this;

            auto _UFirst = nh3api::unfancy(_First);
            auto _ULast  = nh3api::unfancy(_Last);
            if constexpr ( is_elem_cptr<decltype(_UFirst)>::value )
            {
                return assign(_UFirst, _New_size);
            }
            else if constexpr ( is_elem_ucptr<decltype(_UFirst)>::value )
            {
                return assign(reinterpret_cast<const char* const>(_UFirst), _New_size);
            }
            else
            {
                return _Reallocate_for(_New_size,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const size_t _Count, const auto _Src)
                {
                    std::copy_n(_Src, _Count, _New_ptr);
                }, _First);
            }
        }

    #ifdef __cpp_lib_containers_ranges
    #if defined(__clang__) && defined(__INTELLISENSE__)
        template<typename _Range>
    #else
        template<nh3api::tt::container_compatible_range<char> _Range>
    #endif
        exe_string& assign_range(_Range&& _Rng)
        {
            const size_t _Count = std::ranges::size(_Rng);
            if ( _Count == 0 ) NH3API_UNLIKELY
                return *this;

            if constexpr ( std::ranges::sized_range<_Range> && nh3api::contiguous_chars_range<_Range> )
                return assign(std::ranges::data(_Rng), _Count);
            else
                return _Reallocate_for(_Count,
                       NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const size_t _Rng_size, const auto _Rng_begin)
                       {
                           std::ranges::copy_n(_Rng_begin, _Rng_size, _New_ptr);
                       },
                       std::ranges::begin(_Rng));
        }
    #endif // C++23

        exe_string& insert(const size_t _Offset, const exe_string& _Other)
        { return insert(_Offset, _Other._Myptr, _Other._Mysize); }

        exe_string& insert(const size_t _Offset, const exe_string& _Other, size_t _OtherOffset, size_t _Count = npos)
        {
            _Other._Check_offset(_OtherOffset);
            _Count = _Other._Clamp_suffix_size(_OtherOffset, _Count);
            return insert(_Offset, _Other._Myptr + _OtherOffset, _Count);
        }

        exe_string& insert(const size_t _Offset, const std::string_view _String)
        { return insert(_Offset, _String.data(), _String.size()); }

        exe_string& insert(const size_t _Offset, const std::string_view _String, size_t _OtherOffset, size_t _Count = npos)
        { return insert(_Offset, _String.substr(_OtherOffset, _Count)); }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral SizeType = size_t>
    #else
        template<typename SizeType = size_t, std::enable_if_t<std::is_integral_v<SizeType> && !std::is_same_v<SizeType,bool>, int> = 0 >
    #endif
        // insert [_String, _String + _Count) at _Offset
        exe_string& insert(const SizeType _Offset, const char* const _String, const size_t _Count)
        {
            _Check_offset(static_cast<size_t>(_Offset));
            if ( _String == nullptr || _Count == 0 ) NH3API_UNLIKELY
                return *this;

            return _Reallocate_grow_by(
            _Count,
            NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size, const size_t _Src_offset,
                const char* const _Src_ptr, const size_t _Src_count)
            {
                NH3API_ASSUME(_Src_ptr);
                NH3API_ASSUME(_Src_count);
                traits_type::copy(_New_ptr, _Old_ptr, _Src_offset);
                traits_type::copy(_New_ptr + _Src_offset, _Src_ptr, _Src_count);
                traits_type::copy(_New_ptr + _Src_offset + _Src_count, _Old_ptr + _Src_offset, _Old_size - _Src_offset + 1);
            },
            static_cast<size_t>(_Offset), _String, _Count);
        }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral SizeType = size_t>
    #else
        template<typename SizeType = size_t, std::enable_if_t<std::is_integral_v<SizeType> && !std::is_same_v<SizeType,bool>, int> = 0 >
    #endif
        exe_string& insert(const SizeType _Offset, const char* const _String)
        {
            _Check_offset(static_cast<size_t>(_Offset));
            if ( _String == nullptr ) NH3API_UNLIKELY
                return *this;

            const size_t _Length = __builtin_strlen(_String);
            if ( _Length == 0 ) NH3API_UNLIKELY
                return *this;

            return _Reallocate_grow_by(
            _Length,
            NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size, const size_t _Off,
                const char* const _Src, const size_t _Src_count)
            {
                NH3API_ASSUME(_Src);
                NH3API_ASSUME(_Src_count);
                traits_type::copy(_New_ptr, _Old_ptr, _Off);
                traits_type::copy(_New_ptr + _Off, _Src, _Src_count);
                traits_type::copy(_New_ptr + _Off + _Src_count, _Old_ptr + _Off, _Old_size - _Off + 1);
            },
            static_cast<size_t>(_Offset), _String, _Length);
        }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral SizeType = size_t>
    #else
        template<typename SizeType = size_t, std::enable_if_t<std::is_integral_v<SizeType> && !std::is_same_v<SizeType,bool>, int> = 0 >
    #endif
        exe_string& insert(const SizeType _Offset, const size_t _Count, char _Character)
        {
            if ( _Count == 0 ) NH3API_UNLIKELY
                return *this;

            _Check_offset(static_cast<size_t>(_Offset));
            return _Reallocate_grow_by(
            _Count,
            NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size, const size_t _Off,
                const size_t _Characters_num, const char _Ch)
            {
                NH3API_ASSUME(_Characters_num);
                traits_type::copy(_New_ptr, _Old_ptr, _Off);
                traits_type::assign(_New_ptr + _Off, _Characters_num, _Ch);
                traits_type::copy(_New_ptr + _Off + _Characters_num, _Old_ptr + _Off, _Old_size - _Off + 1);
            },
            static_cast<size_t>(_Offset), _Count, _Character);
        }

        iterator insert(const const_iterator _Where, char _Character)
        {
            const size_t _Offset = (_Inside(_Where)) ? static_cast<size_t>(_Where - cbegin()) : 0;
            if ( _Offset )
                insert( _Offset, 1, _Character );
            return begin() + static_cast<ptrdiff_t>(_Offset);
        }

        iterator insert(const const_iterator _Where, const size_t _Count, char _Character)
        {
            const size_t _Offset = (_Inside(_Where)) ? static_cast<size_t>(_Where - cbegin()) : 0;
            if ( _Offset )
                insert( _Offset, _Count, _Character );
            return begin() + static_cast<ptrdiff_t>(_Offset);
        }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<nh3api::tt::iterator_for_container _Iter>
    #else
        template<class _Iter, std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
    #endif
        iterator insert(const const_iterator _Where, const _Iter _First, const _Iter _Last)
        {
            nh3api::verify_range(_First, _Last);
            const size_t _Offset = (_Inside(_Where)) ? static_cast<size_t>(_Where - cbegin()) : 0;
            if ( _Offset )
            {
                const auto _UFirst = nh3api::unfancy(_First);
                const auto _ULast  = nh3api::unfancy(_Last);
                if constexpr ( is_elem_cptr<decltype(_UFirst)>::value )
                {
                    const size_t _Count = static_cast<size_t>(_ULast - _UFirst);
                    if ( _Count == 0 )
                        return begin() + static_cast<ptrdiff_t>(_Offset);

                    insert(_Offset, _UFirst, _Count);
                }
                else
                {
                    const size_t _Count = std::distance(_First, _Last);
                    if ( _Count == 0 )
                        return begin() + static_cast<ptrdiff_t>(_Offset);

                    _Reallocate_grow_by(
                        _Count,
                        NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size, const size_t _Src_offset,
                                                  const _Iter _Src_begin, const size_t _Src_count)
                        {
                            NH3API_ASSUME(_Src_count);
                            traits_type::copy(_New_ptr, _Old_ptr, _Src_offset);
                            std::copy_n(_Src_begin, _Src_count, _New_ptr + _Src_offset);
                            traits_type::copy(_New_ptr + _Src_offset + _Src_count, _Old_ptr + _Src_offset, _Old_size - _Src_offset + 1);
                        },
                        _Offset, _First, _Count);
                }
            }
            return begin() + static_cast<ptrdiff_t>(_Offset);
        }

    #ifdef __cpp_lib_containers_ranges
    #if defined(__clang__) && defined(__INTELLISENSE__)
        template<typename _Range>
    #else
        template<nh3api::tt::container_compatible_range<char> _Range>
    #endif
        iterator insert_range(const const_iterator _Where, _Range&& _Rng)
        {
            const size_t _Offset = (_Inside(_Where)) ? static_cast<size_t>(_Where - cbegin()) : 0;
            const size_t _Count = std::ranges::size(_Rng);
            if ( _Count == 0 ) NH3API_UNLIKELY
                return begin() + static_cast<difference_type>(_Offset);

            if constexpr ( std::ranges::sized_range<_Range> && nh3api::contiguous_chars_range<_Range>)
            {
                insert(_Offset, std::ranges::data(_Rng), _Count);
                return begin() + static_cast<difference_type>(_Offset);
            }
            else
            {
                _Reallocate_grow_by(
                _Count,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size, const size_t _Src_offset,
                                            const auto _Src_begin, const size_t _Src_count)
                {
                    NH3API_ASSUME(_Src_count);
                    traits_type::copy(_New_ptr, _Old_ptr, _Src_offset);
                    std::ranges::copy_n(_Src_begin, _Src_count, _New_ptr + _Src_offset);
                    traits_type::copy(_New_ptr + _Src_offset + _Src_count, _Old_ptr + _Src_offset, _Old_size - _Src_offset + 1);
                },
                _Offset, std::ranges::begin(_Rng), _Count);
                return begin() + static_cast<ptrdiff_t>(_Offset);
            }
        }
    #endif // C++23

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral _Size_type>
    #else
        template<class _Size_type, std::enable_if_t<std::is_integral_v<_Size_type>, bool> = false>
    #endif
        exe_string& erase(const _Size_type _Offset)
        {
            if ( !empty() )
            {
                _Check_offset(_Offset);
                if ( !_Is_shared() ) NH3API_LIKELY
                {
                    _Eos(_Offset);
                }
                else
                {
                    const size_t _Leftover = _Mysize - _Offset;
                    auto         _Buf      = _Get_fresh_buffer(_Leftover);
                    char* const  _New_ptr  = _Buf.get();
                    traits_type::copy(_New_ptr, _Myptr, _Leftover);
                    _Buf.set(this->_Myptr);
                    this->_Myptr            = _New_ptr;
                    this->_Myptr[_Leftover] = '\0';
                }
            }

            return *this;
        }

    protected:
        exe_string& _Erase_without_checks(const size_t _Offset, size_t _Count)
        {
            NH3API_ASSUME(_Myptr != nullptr && _Mysize != 0);
            if ( _Count == 0 )
                return *this;

            _Count                 = _Clamp_suffix_size(_Offset, _Count);
            const size_t _New_size = _Mysize - _Count;
            if ( !_Is_shared() ) NH3API_LIKELY
            {
                char* const _Erase_at = _Myptr + _Offset;
                // move suffix + null up
                traits_type::move(_Erase_at, _Erase_at + _Count, _New_size - _Offset + 1);
                this->_Mysize = _New_size;
            }
            else
            {
                auto        _Buf      = _Get_fresh_buffer(_New_size);
                char* const _New_ptr  = _Buf.get();
                char* const _Erase_at = _New_ptr + _Offset;
                // copy characters before the offset
                traits_type::copy(_New_ptr, _Myptr, _Offset);
                // move suffix + null up
                traits_type::copy(_Erase_at, _Myptr + _Offset, _New_size - _Offset + 1);
                _Buf.set(this->_Myptr);
                this->_Myptr = _New_ptr;
            }
            return *this;
        }

    public:
    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral _Size_type>
    #else
        template<class _Size_type, std::enable_if_t<std::is_integral_v<_Size_type>, bool> = false>
    #endif
        exe_string& erase(const _Size_type _Offset, _Size_type _Count)
        {
            if ( empty() ) NH3API_UNLIKELY
                return *this;

            _Check_offset(_Offset);
            return _Erase_without_checks(_Offset, _Count);
        }

        iterator erase(const const_iterator _Where)
        {
            if ( empty() ) NH3API_UNLIKELY
                return _Myptr;

            if ( _Inside(_Where) )
            {
                const size_t _Offset = static_cast<size_t>(_Where - begin());
                _Erase_without_checks(_Offset, 1);
                return _Myptr + static_cast<ptrdiff_t>(_Offset);
            }

            return _Myptr;
        }

        iterator erase(const const_iterator _First, const const_iterator _Last)
        {
            if ( empty() ) NH3API_UNLIKELY
                return _Myptr;

            nh3api::verify_range(_First, _Last);
            if ( (_First < _Last) && _Inside(_First) && _Inside_exclusive(_Last) )
            {
                const size_t _Offset = static_cast<size_t>(_First - cbegin());
                _Erase_without_checks(_Offset, static_cast<size_t>(_Last - _First));
                return _Myptr + static_cast<ptrdiff_t>(_Offset);
            }

            return _Myptr;
        }

        void clear()
        {
            if ( empty() ) NH3API_UNLIKELY
                return;

            if ( _Is_shared() ) NH3API_UNLIKELY
            {
                auto        _Buf     = _Get_fresh_buffer(1);
                char* const _New_ptr = _Buf.get();
                _Buf.set(this->_Myptr);
                this->_Myptr = _New_ptr;
            }
            else
            {
                this->_Mysize = 0;
            }

            traits_type::assign(_Myptr, _Mysize, '\0');
        }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral _Size_type>
    #else
        template<class _Size_type, std::enable_if_t<std::is_integral_v<_Size_type>, bool> = false>
    #endif
        exe_string& replace(const _Size_type  _Offset,
                            const _Size_type  _Nx,
                            const exe_string& _Other)
        { return replace(_Offset, _Nx, _Other._Myptr, _Other._Mysize); }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral _Size_type>
    #else
        template<class _Size_type, std::enable_if_t<std::is_integral_v<_Size_type>, bool> = false>
    #endif
        exe_string& replace(const _Size_type  _Offset,
                            _Size_type        _Nx,
                            const exe_string& _Other,
                            const _Size_type  _Other_offset,
                            _Size_type        _Count = npos)
        {
            if ( empty() ) NH3API_UNLIKELY
                return *this;

            _Other._Check_offset(_Other_offset);
            _Count = _Other._Clamp_suffix_size(_Other_offset, _Count);
            return replace(_Offset, _Nx, _Other._Myptr + _Other_offset, _Count);
        }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral _Size_type>
    #else
        template<class _Size_type, std::enable_if_t<std::is_integral_v<_Size_type>, bool> = false>
    #endif
        exe_string& replace(const _Size_type  _Offset,
                            _Size_type        _Nx,
                            const char* const _String,
                            const _Size_type  _Count)
        {
            if ( empty() || _String == nullptr || _Count == 0 ) NH3API_UNLIKELY
                return *this;

            _Check_offset(_Offset);
            _Nx                      = _Clamp_suffix_size(_Offset, _Nx);
            const size_t _Growth     = static_cast<size_t>(_Count - _Nx);
            const bool   _Use_existing_buffer = !empty() && !_Is_shared();

            if ( _Use_existing_buffer && _Nx == _Count )
            {
                traits_type::move(_Myptr + _Offset, _String, _Count);
                return *this;
            }

            const size_t    _Old_size    = _Mysize;
            const size_t _Suffix_size = _Old_size - _Nx - _Offset + 1;
            // suffix shifts backwards; we don't have to move anything out of the way
            if ( _Use_existing_buffer && _Count < _Nx )
            {
                char* const _Insert_at = _Myptr + _Offset;
                traits_type::move(_Insert_at, _String, _Count);
                traits_type::move(_Insert_at + _Count, _Insert_at + _Nx, _Suffix_size);
                _Mysize = _Old_size - (_Nx - _Count);
                return *this;
            }

            if ( _Use_existing_buffer && (_Growth <= _Myres - _Old_size) )
            {
                _Mysize                        = _Old_size + _Growth;
                char* const _Old_ptr           = _Myptr;
                char* const _Insert_at         = _Old_ptr + _Offset;
                char* const _Suffix_at         = _Insert_at + _Nx;
                size_t      _Ptr_shifted_after = 0;
                if ( _String + _Count <= _Insert_at || _String > _Old_ptr + _Old_size )
                    _Ptr_shifted_after = _Count;
                else if ( _Suffix_at <= _String )
                    _Ptr_shifted_after = 0;
                else
                    _Ptr_shifted_after = static_cast<size_t>(_Suffix_at - _String);

                traits_type::move(_Suffix_at + _Growth, _Suffix_at, _Suffix_size);
                // next case must be move, in case _Ptr begins before _Insert_at and contains part of the hole;
                // this case doesn't occur in insert because the new content must come from outside the removed
                // content there (because in insert there is no removed content)
                traits_type::move(_Insert_at, _String, _Ptr_shifted_after);
                // the next case can be copy, because it comes from the chunk moved out of the way in the
                // first move, and the hole we're filling can't alias the chunk we moved out of the way
                traits_type::copy(_Insert_at + _Ptr_shifted_after, _String + _Growth + _Ptr_shifted_after, _Count - _Ptr_shifted_after);
                return *this;
            }

            return _Reallocate_grow_by(
                _Growth,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_str, const size_t _Old_str_size, const size_t _Off,
                   const size_t _Num, const char* const _Src, const size_t _Src_count)
                {
                    NH3API_ASSUME(_Src);
                    NH3API_ASSUME(_Src_count);
                    traits_type::copy(_New_ptr, _Old_str, _Off);
                    traits_type::copy(_New_ptr + _Off, _Src, _Src_count);
                    traits_type::copy(_New_ptr + _Off + _Src_count, _Old_str + _Off + _Num, _Old_str_size - _Num - _Off + 1);
                },
                _Offset, _Nx, _String, _Count);
        }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral _Size_type>
    #else
        template<class _Size_type, std::enable_if_t<std::is_integral_v<_Size_type>, bool> = false>
    #endif
        exe_string& replace(const _Size_type _Offset, const _Size_type _Nx, const char* const _String)
        {
            if ( !this->empty() && _String != nullptr ) NH3API_LIKELY
                return replace(_Offset, _Nx, _String, __builtin_strlen(_String));

            return *this;
        }

    #if defined(__cpp_lib_concepts) && !defined(__INTELLISENSE__)
        template<std::integral _Size_type>
    #else
        template<class _Size_type, std::enable_if_t<std::is_integral_v<_Size_type>, bool> = false>
    #endif
        exe_string& replace(const _Size_type _Offset,
                            _Size_type       _Nx,
                            const _Size_type _Count,
                            char             _Character)
        {
            if ( empty() || _Count == 0 ) NH3API_UNLIKELY
                return *this;

            _Check_offset(_Offset);
            _Nx = _Clamp_suffix_size(_Offset, _Nx);
            const bool _Use_existing_buffer = !empty() && !_Is_shared();

            if ( _Use_existing_buffer && _Count == _Nx )
            {
                traits_type::assign(_Myptr + _Offset, _Count, _Character);
                return *this;
            }

            const size_t _Old_size = _Mysize;
            if ( _Use_existing_buffer && (_Count < _Nx || (_Count - _Nx <= _Myres - _Old_size)) )
            {
                const size_t _New_size = _Old_size + _Count - _Nx;
                _Mysize                = _New_size;
                char* const _Insert_at = _Myptr + _Offset;
                traits_type::move(_Insert_at + _Count, _Insert_at + _Nx, _Old_size - _Nx - _Offset + 1);
                traits_type::assign(_Insert_at, _Count, _Character);
                return *this;
            }

            return _Reallocate_grow_by(
                _Count - _Nx,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_str, const size_t _Old_str_size, const size_t _Off,
                   const size_t _Number, const size_t _Character_num, const char _Ch)
                {
                    NH3API_ASSUME(_Character_num);
                    traits_type::copy(_New_ptr, _Old_str, _Off);
                    traits_type::assign(_New_ptr + _Off, _Character_num, _Ch);
                    traits_type::copy(_New_ptr + _Off + _Character_num, _Old_str + _Off + _Number, _Old_str_size - _Number - _Off + 1);
                },
                _Offset, _Nx, _Count, _Character);
        }

        exe_string& replace(const const_iterator _First,
                            const const_iterator _Last,
                            const exe_string&    _Other)
        {
            if ( empty() ) NH3API_UNLIKELY
                return *this;

            nh3api::verify_range(_First, _Last);
            const size_t _Difference = (_First < _Last) ? static_cast<size_t>(_Last - _First) : 0;
            const size_t _Offset     = (_Inside(_First)) ? static_cast<size_t>(_First - _Myptr) : 0;
            if ( _Offset && _Difference )
                return replace(_Offset, _Difference, _Other);

            return *this;
        }

        exe_string& replace(const const_iterator    _First,
                            const const_iterator    _Last,
                            const std::string_view _Other)
        {
            if ( empty() ) NH3API_UNLIKELY
                return *this;

            nh3api::verify_range(_First, _Last);
            const size_t _Difference = (_First < _Last) ? static_cast<size_t>(_Last - _First) : 0;
            const size_t _Offset     = (_Inside(_First)) ? static_cast<size_t>(_First - _Myptr) : 0;
            if ( _Offset && _Difference )
                return replace(_Offset, _Difference, _Other.data(), _Other.size());

            return *this;
        }

        exe_string& replace(const const_iterator _First,
                            const const_iterator _Last,
                            const char* const    _String,
                            const size_t         _Count)
        {
            if ( empty() ) NH3API_UNLIKELY
                return *this;

            nh3api::verify_range(_First, _Last);
            const size_t _Difference = (_First < _Last) ? static_cast<size_t>(_Last - _First) : 0;
            const size_t _Offset     = (_Inside(_First)) ? static_cast<size_t>(_First - begin()) : 0;
            if ( _Offset && _Difference && _String )
                return replace(_Offset, _Difference, _String, _Count);

            return *this;
        }

        exe_string& replace(const const_iterator _First,
                            const const_iterator _Last,
                            const char* const    _String)
        {
            nh3api::verify_range(_First, _Last);
            const size_t _Difference = (_First < _Last) ? static_cast<size_t>(_Last - _First) : 0;
            const size_t _Offset     = (_Inside(_First)) ? static_cast<size_t>(_First - begin()) : 0;
            if ( _Offset && _Difference && _String )
                return replace(_Offset, _Difference, _String);

            return *this;
        }

        exe_string& replace(const const_iterator _First,
                            const const_iterator _Last,
                            const size_t         _Count,
                            char                 _Character)
        {
            nh3api::verify_range(_First, _Last);
            const size_t _Difference = (_First < _Last) ? static_cast<size_t>(_Last - _First) : 0;
            const size_t _Offset     = (_Inside(_First)) ? static_cast<size_t>(_First - begin()) : 0;
            if ( _Offset && _Difference )
                return replace(_Offset, _Difference, _Count, _Character);

            return *this;
        }

    #if defined(__cpp_concepts) && !defined(__INTELLISENSE__)
        template<nh3api::tt::iterator_for_container _Iter>
    #else
        template<class _Iter, std::enable_if_t<nh3api::tt::is_iterator_v<_Iter>, bool> = false>
    #endif
        exe_string& replace(const const_iterator _First,
                            const const_iterator _Last,
                            const _Iter          _First2,
                            const _Iter          _Last2)
        {
            nh3api::verify_range(_First, _Last);
            if ( !_Inside(_First) || !_Inside_exclusive(_Last) )
                return *this;

            const size_t _Offset = static_cast<size_t>(_First - _Myptr);
            const size_t _Length = static_cast<size_t>(_Last - _First);
            nh3api::verify_range(_First2, _Last2);
            const auto _UFirst2 = nh3api::unfancy(_First2);
            const auto _ULast2  = nh3api::unfancy(_Last2);
            if constexpr ( is_elem_cptr<decltype(_UFirst2)>::value )
            {
                return replace(_Offset, _Length, _UFirst2, static_cast<size_t>(_ULast2 - _UFirst2));
            }
            else
            {
                const exe_string _Temp(_UFirst2, _ULast2);
                return replace(_Offset, _Length, _Temp._Myptr, _Temp._Mysize);
            }
        }

    #ifdef __cpp_lib_containers_ranges
        template<nh3api::tt::container_compatible_range<char> _Range>
        exe_string& replace_with_range(const const_iterator _First,
                                       const const_iterator _Last,
                                       _Range&&             _Rng)
        {
            nh3api::verify_range(_First, _Last);
            if ( !_Inside(_First) || !_Inside_exclusive(_Last) )
                return *this;

            const size_t _Offset = static_cast<size_t>(_First - _Myptr);
            const size_t _Length = static_cast<size_t>(_Last - _First);
            if constexpr (std::ranges::sized_range<_Range> || std::ranges::forward_range<_Range>)
            {
                const size_t _Count = static_cast<size_t>(std::ranges::distance(_Rng));
                return replace(_Offset, _Length, std::ranges::data(_Rng), _Count);
            }
            else
            {
                const exe_string _Temp(std::from_range, _Rng);
                return replace(_Offset, _Length, _Temp._Myptr, _Temp._Mysize);
            }
        }
    #endif

    public:
        [[nodiscard]] reference front() noexcept
        {
            _Freeze();
            return _Myptr[0];
        }

        [[nodiscard]] const_reference front() const noexcept
        { return _Myptr[0]; }

        [[nodiscard]] reference back() noexcept
        {
            _Freeze();
            return _Myptr[_Mysize - 1];
        }

        [[nodiscard]] const_reference back() const noexcept
        { return _Myptr[_Mysize - 1]; }

        [[nodiscard]] iterator begin() noexcept
        {
            _Freeze();
            return _Myptr;
        }

        [[nodiscard]] const_iterator begin() const noexcept
        { return _Myptr; }

        [[nodiscard]] const_iterator cbegin() const noexcept
        { return _Myptr; }

        [[nodiscard]] iterator end() noexcept
        {
            _Freeze();
            return _Myptr + _Mysize;
        }

        [[nodiscard]] const_iterator end() const noexcept
        { return static_cast<const_iterator>(_Myptr + _Mysize); }

        [[nodiscard]] const_iterator cend() const noexcept
        { return static_cast<const_iterator>(_Myptr + _Mysize); }

        [[nodiscard]] reverse_iterator rbegin() noexcept
        { return reverse_iterator(end()); }

        [[nodiscard]] const_reverse_iterator rbegin() const noexcept
        { return const_reverse_iterator(end()); }

        [[nodiscard]] const_reverse_iterator crbegin() const noexcept
        { return const_reverse_iterator(end()); }

        [[nodiscard]] reverse_iterator rend() noexcept
        { return reverse_iterator(begin()); }

        [[nodiscard]] const_reverse_iterator rend() const noexcept
        { return const_reverse_iterator(begin()); }

        [[nodiscard]] const_reverse_iterator crend() const noexcept
        {  return const_reverse_iterator(begin()); }

        // reduce capacity
        void shrink_to_fit()
        {
            if ( _Myptr == nullptr || _Mysize == _Myres ) NH3API_UNLIKELY
                return;

            char* const _New_ptr = _Allocate_for_capacity(_Mysize);
            traits_type::copy(_New_ptr, _Myptr, _Mysize);
            _Tidy_deallocate();
            this->_Myptr          = _New_ptr;
            this->_Myptr[_Mysize] = '\0';
            this->_Myres          = _Mysize;
        }

        reference at(const size_t _Offset)
        {
            _Check_offset_exclusive(_Offset);
            _Freeze();
            return _Myptr[_Offset];
        }

        [[nodiscard]] const_reference at(const size_t _Offset) const
        {
            _Check_offset_exclusive(_Offset);
            return _Myptr[_Offset];
        }

        [[nodiscard]] reference operator[](size_t _Offset) noexcept
        {
            _Freeze();
            return _Myptr[_Offset];
        }

        [[nodiscard]] const_reference operator[](size_t _Offset) const
        { return _Myptr[_Offset]; }

        operator std::string_view() const noexcept
        { return {_Myptr, _Mysize}; }

        void push_back(char _Character)
        {
            if ( !_Is_shared() && _Mysize < _Myres ) NH3API_LIKELY
            {
                const size_t _Old_size = _Mysize;
                _Mysize                = _Old_size + 1;
                _Myptr[_Old_size]      = _Character;
                _Myptr[_Old_size + 1]  = '\0';
            }
            else
            {
                if ( max_size() == _Mysize ) NH3API_UNLIKELY
                    _Throw_length_exception();

                const size_t _Old_size = this->_Mysize;
                const size_t _New_size = _Old_size + 1;
                auto         _Buf      = _Get_fresh_buffer(_New_size);
                char* const  _New_ptr  = _Buf.get();
                char* const  _Old_ptr  = this->_Myptr;
                traits_type::copy(_New_ptr, _Old_ptr, _Old_size);
                _New_ptr[_Old_size] = _Character;
                _Buf.set(_Old_ptr);
                this->_Myptr            = _New_ptr;
                this->_Myptr[_New_size] = '\0';
            }
        }

        void pop_back()
        {
            if ( empty() ) NH3API_UNLIKELY
                return;

            const size_t _Leftover = _Mysize - 1;
            if ( !_Is_shared() ) NH3API_LIKELY
            {
                _Eos(_Leftover);
            }
            else
            {
                auto        _Buf     = _Get_fresh_buffer(_Leftover);
                char* const _New_ptr = _Buf.get();
                traits_type::copy(_New_ptr, _Myptr, _Leftover);
                _Buf.set(this->_Myptr);
                this->_Myptr              = _New_ptr;
                this->_Myptr[_Leftover] = '\0';
            }
        }

        [[nodiscard]] const char* c_str() const noexcept
        { return _Myptr ? _Myptr : ""; }

        [[nodiscard]] const char* data() const noexcept
        { return _Myptr; }

        [[nodiscard]] char* data() noexcept
        {
            _Freeze();
            return _Myptr;
        }

        [[nodiscard]] size_t length() const noexcept
        { return _Mysize; }

        [[nodiscard]] size_t size() const noexcept
        { return _Mysize; }

        [[nodiscard]] inline constexpr static size_t max_size() noexcept
        { return size_t(~0U); }

        void resize(const size_t _New_size, char _Character = '\0')
        {
            const size_t _Old_size = _Mysize;
            if ( _New_size <= _Old_size )
            {
                if ( _Is_shared() ) NH3API_UNLIKELY
                {
                    auto        _Buf     = _Get_fresh_buffer(_New_size);
                    char* const _New_ptr = _Buf.get();
                    traits_type::copy(_New_ptr, _Myptr, _New_size);
                    _Buf.set(this->_Myptr);
                    this->_Myptr = _New_ptr;
                }

                _Eos(_New_size);
            }
            else
            {
                append(_New_size - _Old_size, _Character);
            }
        }

        template<class _Operation>
        void resize_and_overwrite(const size_t _New_size, _Operation _Op)
        {
            // if capacity is insufficient -> reallocate/grow (keeps old contents)
            if ( _Myres < _New_size )
            {
                // _Reallocate_grow_by will set _Mysize == _New_size and place null terminator
                _Reallocate_grow_by(
                    _New_size - _Mysize,
                    NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_ptr, const size_t _Old_size)
                    {
                        // copy old contents including null terminator
                        traits_type::copy(_New_ptr, _Old_ptr, _Old_size + 1);
                    });
            }
            else if ( _Is_shared() ) NH3API_UNLIKELY
            {
                auto _Buf = _Get_fresh_buffer(_New_size);
                char* const _New_ptr = _Buf.get();
                const size_t _To_copy = std::min<size_t>(_Mysize, _New_size);
                traits_type::copy(_New_ptr, _Myptr, _To_copy);
                _Buf.set(this->_Myptr);
                this->_Myptr = _New_ptr;
                this->_Myptr[_New_size] = '\0';
                this->_Mysize = _New_size;
            }
            else
            {
                this->_Mysize = _New_size;
                this->_Myptr[_New_size] = '\0';
            }

            char* _Arg_ptr = this->_Myptr;
            size_t _Arg_size = _New_size;
            const size_t _Result_size = static_cast<size_t>(_Op(_Arg_ptr, _Arg_size)); // op returns resulting size

            _Eos(_Result_size);
        }

        [[nodiscard]] size_t capacity() const noexcept
        { return _Myres; }

        // determine new minimum length of allocated storage
        void reserve(const size_t _Newcap
        #if !NH3API_CHECK_CPP20
        = 0
        #endif
        )
        {
            if ( empty() )
            {
				if ( _Newcap == 0 )
					return;

                const size_t _New_capacity  = _Calculate_growth(_Newcap);
                char* const  _Allocated_ptr = _Allocate_for_capacity(_New_capacity);
                char* const  _New_ptr       = _Allocated_ptr + 1;
                _Refcnt(_New_ptr)           = 0;
                this->_Myptr                = _New_ptr;
                this->_Myptr[0]             = '\0';
                this->_Mysize               = 0;
                this->_Myres                = _New_capacity;
                return;
            }

            // requested capacity is not large enough for current size, ignore
            if ( _Mysize > _Newcap )
                return;

            const size_t _Old_size = _Mysize;

            _Reallocate_grow_by(
                _Newcap - _Old_size,
                NH3API_CAPTURELESS_LAMBDA(char* const _New_ptr, const char* const _Old_str, const size_t _Old_str_size)
                {
                    traits_type::copy(_New_ptr, _Old_str, _Old_str_size + 1);
                });

            _Eos(_Old_size);
        }

        [[nodiscard]] inline bool empty() const noexcept
        { return _Myptr == nullptr || _Mysize == 0; }

        size_t copy(char* _Destination, size_t _Count, const size_t _Offset = 0) const
        {
            if ( _Destination == nullptr || _Count == 0 )
                return 0;

            _Check_offset(_Offset);
            _Count = _Clamp_suffix_size(_Offset, _Count);
            if ( _Count )
                traits_type::copy(_Destination, _Myptr + static_cast<ptrdiff_t>(_Offset), _Count);

            return _Count;
        }

        void swap(exe_string& _Right) noexcept
        {
            auto& _Lhs = reinterpret_cast<std::array<uint32_t, 4>&>(*this);
            auto& _Rhs = reinterpret_cast<std::array<uint32_t, 4>&>(_Right);
            std::swap(_Lhs, _Rhs);
        }

        [[nodiscard]] bool contains(char _Character) const noexcept
        {
            if ( empty() )
                return false;

        #if NH3API_CHECK_MINGW // GCC doesn't support __builtin_char_memchr
            return __builtin_memchr(_Myptr, _Character, _Mysize) != nullptr;
        #else
            return __builtin_char_memchr(_Myptr, _Character, _Mysize) != nullptr;
        #endif
        }

        [[nodiscard]] bool contains(const char* const _String) const noexcept
        { return find(_String) != npos; }

        [[nodiscard]] bool contains(const std::string_view _String) const noexcept
        { return find(_String) != npos; }

        [[nodiscard]] bool starts_with(char _Character) const noexcept
        { return !empty() && (front() == _Character); }

        [[nodiscard]] bool starts_with(const char* const _String) const noexcept
        {
            if ( _String )
            {
                const size_t _Rightsize = __builtin_strlen(_String);
                if ( _Mysize < _Rightsize )
                    return false;
                return __builtin_memcmp(_Myptr, _String, _Rightsize) == 0;
            }

            return false;
        }

        [[nodiscard]] bool starts_with(const std::string_view _String) const noexcept
        {
            const size_t _Rightsize = _String.size();
            if ( _String.data() && _Rightsize )
            {
                if ( _Mysize < _Rightsize )
                    return false;
                return __builtin_memcmp(_Myptr, _String.data(), _Rightsize) == 0;
            }

            return false;
        }

        [[nodiscard]] bool ends_with(char _Character) const
        { return !empty() && (back() == _Character); }

        [[nodiscard]] bool ends_with(const char* const _String) const noexcept
        {
            if ( _String )
            {
                const size_t _Rightsize = __builtin_strlen(_String);
                if ( _Mysize < _Rightsize )
                    return false;
                return __builtin_memcmp(_Myptr + (_Mysize - _Rightsize), _String, _Rightsize) == 0;
            }

            return false;
        }

        [[nodiscard]] bool ends_with(const std::string_view _String) const noexcept
        {
            const size_t _Rightsize = _String.size();
            if ( _String.data() && _Rightsize )
            {
                if ( _Mysize < _Rightsize )
                    return false;
                return __builtin_memcmp(_Myptr + (_Mysize - _Rightsize), _String.data(), _Rightsize) == 0;
            }

            return false;
        }

        [[nodiscard]] size_t find(const exe_string& _Other, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find(_Other._Myptr, _Offset, _Other._Mysize); }

        [[nodiscard]] size_t find(const std::string_view _Other, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find(_Other.data(), _Offset, _Other.size()); }

        [[nodiscard]] size_t find(char _Character, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find(_Character, _Offset); }

        [[nodiscard]] size_t find(const char* const _String, const size_t _Offset, const size_t _Count) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find(_String, _Offset, _Count) : npos; }

        [[nodiscard]] size_t find(const char* const _String, const size_t _Offset = 0) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find(_String, _Offset, __builtin_strlen(_String)) : npos; }

        [[nodiscard]] size_t rfind(const exe_string& _Other, const size_t _Offset = npos) const noexcept
        { return std::string_view{_Myptr, _Mysize}.rfind(_Other._Myptr, _Offset, _Other._Mysize); }

        [[nodiscard]] size_t rfind(const std::string_view _String, const size_t _Offset = npos) const noexcept
        { return std::string_view{_Myptr, _Mysize}.rfind(_String.data(), _Offset, _String.size()); }

        [[nodiscard]] size_t rfind(char _Character, const size_t _Offset = npos) const noexcept
        { return std::string_view{_Myptr, _Mysize}.rfind(_Character, _Offset); }

        [[nodiscard]] size_t rfind(const char* const _String, const size_t _Offset, const size_t _Count) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.rfind(_String, _Offset, _Count) : npos; }

        [[nodiscard]] size_t rfind(const char* const _String, const size_t _Offset = npos) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.rfind(_String, _Offset, __builtin_strlen(_String)) : npos; }

        [[nodiscard]] size_t find_first_of(const exe_string& _Other, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_first_of(_Other._Myptr, _Offset, _Other._Mysize); }

        [[nodiscard]] size_t find_first_of(const std::string_view _Other, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_first_of(_Other.data(), _Offset, _Other.size()); }

        [[nodiscard]] size_t find_first_of(char _Character, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_first_of(_Character, _Offset); }

        [[nodiscard]] size_t find_first_of(const char* const _String, const size_t _Offset, const size_t _Count) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find_first_of(_String, _Offset, _Count) : npos; }

        [[nodiscard]] size_t find_first_of(const char* const _String, const size_t _Offset = 0) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find_first_of(_String, _Offset, __builtin_strlen(_String)) : npos; }

        [[nodiscard]] size_t find_last_of(const exe_string& _Other, const size_t _Offset = npos) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_last_of(_Other._Myptr, _Offset, _Other._Mysize); }

        [[nodiscard]] size_t find_last_of(const std::string_view _Other, const size_t _Offset = npos) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_last_of(_Other.data(), _Offset, _Other.size()); }

        [[nodiscard]] size_t find_last_of(char _Character, const size_t _Offset = npos) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_last_of(_Character, _Offset); }

        [[nodiscard]] size_t find_last_of(const char* const _String, const size_t _Offset, const size_t _Count) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find_last_of(_String, _Offset, _Count) : npos; }

        [[nodiscard]] size_t find_last_of(const char* const _String, const size_t _Offset = npos) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find_last_of(_String, _Offset, __builtin_strlen(_String)) : npos; }

        [[nodiscard]] size_t find_first_not_of(const exe_string& _Other, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_first_not_of(_Other._Myptr, _Offset, _Other._Mysize); }

        [[nodiscard]] size_t find_first_not_of(const std::string_view _Other, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_first_not_of(_Other.data(), _Offset, _Other.size()); }

        [[nodiscard]] size_t find_first_not_of(char _Character, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_first_not_of(_Character, _Offset); }

        [[nodiscard]] size_t find_first_not_of(const char* const _String, size_t _Offset, const size_t _Count) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find_first_not_of(_String, _Offset, _Count) : npos; }

        [[nodiscard]] size_t find_first_not_of(const char* const _String, const size_t _Offset = 0) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find_first_not_of(_String, _Offset, __builtin_strlen(_String)) : npos; }

        [[nodiscard]] size_t find_last_not_of(const exe_string& _Other, const size_t _Offset = npos) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_last_not_of(_Other._Myptr, _Offset, _Other._Mysize); }

        [[nodiscard]] size_t find_last_not_of(const std::string_view _Other, const size_t _Offset = npos) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_last_not_of(_Other.data(), _Offset, _Other.size()); }

        [[nodiscard]] size_t find_last_not_of(char _Character, const size_t _Offset = 0) const noexcept
        { return std::string_view{_Myptr, _Mysize}.find_last_not_of(_Character, _Offset); }

        [[nodiscard]] size_t find_last_not_of(const char* const _String, const size_t _Offset, const size_t _Count) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find_last_not_of(_String, _Offset, _Count) : npos; }

        [[nodiscard]] size_t find_last_not_of(const char* const _String, const size_t _Offset = npos) const noexcept
        { return _String ? std::string_view{_Myptr, _Mysize}.find_last_not_of(_String, _Offset, __builtin_strlen(_String)) : npos; }

        [[nodiscard]] exe_string substr(const size_t _Offset, const size_t _Count = npos) const&
        { return { *this, _Offset, _Count }; }

        [[nodiscard]] exe_string substr(const size_t _Offset, const size_t _Count = npos) &&
        { return { std::move(*this), _Offset, _Count }; }

        [[nodiscard]] exe_string substr() const&
        { return { *this }; }

        [[nodiscard]] exe_string substr() && noexcept
        { return { std::move(*this) }; }

        [[nodiscard]] int compare(const std::string_view _String) const noexcept
        { return _Compare(this->_Myptr, this->_Mysize, _String.data(), _String.size()); }

        [[nodiscard]] int compare(const size_t _Offset, const size_t _Nx, const std::string_view _String) const
        {
            _Check_offset(_Offset);
            return _Compare(this->_Myptr + _Offset, this->_Clamp_suffix_size(_Offset, _Nx), _String.data(), _String.size());
        }

        [[nodiscard]] int compare(const size_t           _Offset,
                                  const size_t           _Nx,
                                  const std::string_view _String,
                                  const size_t           _Right_offset,
                                  const size_t           _Count = npos) const
        {
            _Check_offset(_Offset);
            const std::string_view _Subview = _String.substr(_Right_offset, _Count);
            return _Compare(this->_Myptr + _Offset, this->_Clamp_suffix_size(_Offset, _Nx), _Subview.data(), _Subview.size());
        }

        [[nodiscard]] int compare(const exe_string& _Other) const noexcept
        { return _Compare(this->_Myptr, this->_Mysize, _Other._Myptr, _Other._Mysize); }

        [[nodiscard]] int compare(const size_t _Offset, const size_t _Nx, const exe_string& _Other ) const
        {
            _Check_offset(_Offset);
            return _Compare(this->_Myptr + _Offset, this->_Clamp_suffix_size(_Offset, _Nx), _Other._Myptr, _Other._Mysize);
        }

        [[nodiscard]] int compare(const size_t      _Offset,
                                  const size_t      _Nx,
                                  const exe_string& _Other,
                                  const size_t      _Right_offset,
                                  const size_t      _Count = npos) const
        {
            this->_Check_offset(_Offset);
            _Other._Check_offset(_Right_offset);

            return _Compare(this->_Myptr + _Offset,
                            this->_Clamp_suffix_size(_Offset, _Nx),
                            _Other._Myptr + _Right_offset,
                            _Other._Clamp_suffix_size(_Right_offset, _Count));
        }

        int compare(const char* const _String) const noexcept
        {
            if ( _String )
                return _Compare(this->_Myptr, this->_Mysize, _String, __builtin_strlen(_String));

            // compare to null string: if this string is empty, return 0, because null strings are equal
            // if string is not empty, it is always larger than the null string, so we return 1(the left string, or this string, is larger)
            return empty() ? 0 : 1;
        }

        int compare(const size_t _Offset, const size_t _Nx, const char* const _String) const
        {
            _Check_offset(_Offset);
            if ( _String )
                return _Compare(this->_Myptr + _Offset, _Clamp_suffix_size(_Offset, _Nx), _String, __builtin_strlen(_String));

            // the same logic as in int compare(const char*), but if there are any symbols left after the offset
            return (_Mysize - _Offset == 0) ? 0 : 1;
        }

        int compare(const size_t      _Offset,
                    const size_t      _Nx,
                    const char* const _String,
                    const size_t      _Count) const
        {
            _Check_offset(_Offset);
            if ( _String && _Count )
                return _Compare(this->_Myptr + _Offset, _Clamp_suffix_size(_Offset, _Nx), _String, std::min<size_t>(_Count, __builtin_strlen(_String)));

            // the same logic as in int compare(const char*), but if there are any symbols left after the offset
            return (_Mysize - _Offset == 0) ? 0 : 1;
        }

        [[nodiscard]] inline static constexpr allocator_type get_allocator() noexcept
        { return {}; }

    private:
        inline static constexpr size_t _MIN_SIZE { 31 };

        // assign by stealing _Right's buffer
        // pre: this != &_Right
        // pre: *this owns no memory
        inline void _Take_contents(exe_string& _Right) noexcept
        {
            auto& _Dst = reinterpret_cast<std::array<uint32_t, 4>&>(*this);
            auto& _Src = reinterpret_cast<std::array<uint32_t, 4>&>(_Right);
            _Dst = std::exchange(_Src, {});
        }

        void _Move_construct_from_substr(exe_string& _Other, const size_t _Offset, const size_t _Size_max)
        {
            _Other._Check_offset(_Offset);
            const size_t _Result_size = _Other._Clamp_suffix_size(_Offset, _Size_max);
            char* const  _Other_ptr   = _Other._Myptr;

            // erase characters after offset and move the characters
            if ( _Offset != 0 )
                traits_type::move(_Other_ptr, _Other_ptr + _Offset, _Result_size);

            _Other._Eos(_Result_size);
            this->_Take_contents(_Other);
        }

        inline void _Construct_empty() noexcept
        { reinterpret_cast<std::array<uint32_t, 4>&>(*this) = {}; }

        enum class _Construct_strategy : uint8_t
        {
            _From_char,
            _From_ptr,
            _From_string
        };

        template<_Construct_strategy _Strat, class _Char_or_ptr>
        void _Construct(const _Char_or_ptr _Arg, const size_t _Count)
        {
            if constexpr ( _Strat == _Construct_strategy::_From_char )
                static_assert(std::is_same_v<_Char_or_ptr, char>);
            else
                static_assert(is_elem_cptr<_Char_or_ptr>::value);

            if ( _Count > max_size() )
                _Throw_length_exception();

            const size_t _New_capacity  = _Calculate_growth(_Count, 0, max_size());
            this->_Mysize               = _Count;
            this->_Myres                = _New_capacity;

            if constexpr ( _Strat != _Construct_strategy::_From_string )
            {
                char* const  _New_ptr       = _Allocate_for_capacity(_New_capacity);
                this->_Myptr                = _New_ptr + 1; // reference count is one byte before the string
                _Refcnt(this->_Myptr) = 0;            // set reference count to zero on construction

                if constexpr ( _Strat == _Construct_strategy::_From_char )
                {
                    traits_type::assign(_Myptr, _Count, _Arg);
                    _Myptr[_Count] = '\0';
                }
                else /*if constexpr ( _Strat == _Construct_strategy::_From_ptr )*/
                {
                    traits_type::copy(_Myptr, _Arg, _Count);
                    _Myptr[_Count] = '\0';
                }
            }
            // _Strat == _Construct_strategy::_From_string
            else
            {
                // NOTE: for performance reason and memory safety reasons,
                // we do not implement copy-on-write in this implementation reference counting
                // implement copy-on-write
            #if 0
                if ( _Refcnt(_Arg) < _FROZEN - 1 )
                {
                    // do not copy, just share the same pointer
                    _Myptr = _Arg;
                    // and increment reference count
                    ++_Refcnt(_Myptr);
                }
                else
            #endif
                {
                    // allocate and copy
                    char* const _New_ptr        = _Allocate_for_capacity(_New_capacity);
                    this->_Myptr                = _New_ptr + 1; // reference count is one byte before the string
                    _Refcnt(this->_Myptr) = 0;            // set reference count to zero on construction
                    traits_type::copy(_Myptr, _Arg, _Count);
                    _Myptr[_Count] = '\0';
                }
            }
        }

        template<class _Iter, class _Size = std::nullptr_t>
        void _Construct_from_iter(_Iter _First, const _Iter _Last, _Size _Count = {})
        {
            if constexpr ( std::is_same_v<_Size, size_t> )
            {
                if ( _Count > max_size() )
                    _Throw_length_exception();

                const size_t _New_capacity = _Calculate_growth(_Count);
                char* const  _New_ptr      = _Allocate_for_capacity(_New_capacity);
                this->_Myptr               = _New_ptr + 1;
                _Refcnt(this->_Myptr)      = 0;
                this->_Mysize              = _Count;
                this->_Myres               = _New_capacity;
                traits_type::copy(this->_Myptr, _First, _Count);
                _Myptr[_Count] = '\0';
            }
            else
            {
                const size_t _New_size = static_cast<size_t>(std::distance(_First, _Last));
                if ( _New_size > max_size() )
                    _Throw_length_exception();

                const size_t _New_capacity = _Calculate_growth(_New_size);
                char* const  _New_ptr      = _Allocate_for_capacity(_New_capacity);
                this->_Myptr               = _New_ptr + 1;
                _Refcnt(this->_Myptr)      = 0;
                this->_Mysize              = _New_size;
                this->_Myres               = _New_capacity;
                std::copy(_First, _Last, _Myptr);
                _Myptr[_New_size] = '\0';
            }
        }

        [[nodiscard]] NH3API_MALLOC(1)
        static inline char* _Allocate_for_capacity(const size_t _Requested_size)
        {
            // we allocate one additional byte before the string for ref counting
            // and another one after the string, for null terminator
            char* const _Allocated_ptr = static_cast<char*>(::operator new(_Requested_size + 2, exe_heap, std::nothrow));
            if ( _Allocated_ptr != nullptr ) NH3API_LIKELY
                return _Allocated_ptr;

            nh3api::throw_exception<std::bad_alloc>();
        }

        static inline void _Deallocate(char* _Ptr) noexcept
        { ::operator delete(_Ptr, exe_heap); }

        [[nodiscard]] static inline constexpr size_t _Calculate_growth(const size_t _Requested, const size_t _Old, const size_t _Max) noexcept
        {
            size_t _Masked = _Requested | _MIN_SIZE;

            // the mask overflows, settle for max_size()
            if ( _Masked > _Max )
                return _Max;

            // similarly, geometric overflows
            if ( _Old > _Max - _Old / 2 )
                return _Max;

            return std::max<size_t>(_Masked, _Old + _Old / 2);
        }

        [[nodiscard]] inline size_t _Calculate_growth(const size_t _Requested) const noexcept
        { return _Calculate_growth(_Requested, capacity(), max_size()); }

        // reallocate to store exactly _New_size elements, new buffer prepared by
        // _Fn(_New_ptr, _New_size, _Args...)
        template<class _Function, class... _FuncArgs>
        NH3API_FORCEINLINE exe_string& _Reallocate_for(const size_t _New_size, _Function _Fn, _FuncArgs... _Args)
        {
            _Check_size(_New_size);

            auto        _Buf     = _Get_buffer(_New_size);
            char* const _New_ptr = _Buf.get();
            _Fn(_New_ptr, _New_size, _Args...);
            _Buf.set(this->_Myptr);
            this->_Myptr            = _New_ptr;
            this->_Myptr[_New_size] = '\0';
            return *this;
        }

        // reallocate to increase size by _Size_increase elements, new buffer prepared by
        // _Fn(_New_ptr, _Old_ptr, _Old_size, _Args...)
        template<class _Function, class... _FuncArgs>
        NH3API_FORCEINLINE exe_string& _Reallocate_grow_by(const size_t _Size_increase, _Function _Fn, _FuncArgs... _Args)
        {
            if ( max_size() - _Mysize < _Size_increase ) NH3API_UNLIKELY
                _Throw_length_exception();

            const size_t _Old_size = this->_Mysize;
            const size_t _New_size = _Old_size + _Size_increase;
            auto         _Buf      = _Get_buffer(_New_size);
            char* const  _New_ptr  = _Buf.get();
            char* const  _Old_ptr  = this->_Myptr;
            _Fn(_New_ptr, _Old_ptr, _Old_size, _Args...);
            _Buf.set(_Old_ptr);
            this->_Myptr            = _New_ptr;
            this->_Myptr[_New_size] = '\0';
            return *this;
        }

        // set new length and null terminator
        inline void _Eos(const size_t _New_size) noexcept
        {
            _Mysize = _New_size;
            if ( _Myptr )
                _Myptr[_New_size] = '\0';
        }

        [[nodiscard]] inline bool _Inside(const const_iterator _Iterator) const noexcept
        { return cbegin() <= _Iterator && _Iterator < cend(); }

        [[nodiscard]] inline bool _Inside_exclusive(const const_iterator _Iterator) const noexcept
        { return cbegin() <= _Iterator && _Iterator <= cend(); }

    #if defined(__GNUC__) || defined(__clang__)
        [[__gnu__::__hot__]]
    #endif
        // Prepare function for modification: "freeze" the string
        // This is similar to _Split(), but here we are
        // 1. do not change the size of the string or its contents, we just split the existing string to a unique one
        // 2. prepare the string for user modification.
        void _Freeze() noexcept
        {
            if ( _Myptr != nullptr )
            {
                if ( _Refcnt(_Myptr) != 0 && _Refcnt(_Myptr) != _FROZEN ) NH3API_UNLIKELY
                    _Split();
                _Refcnt(_Myptr) = _FROZEN;
            }
        }

        // Get the reference counter one byte before the pointer
        [[nodiscard]] static NH3API_FORCEINLINE uint8_t& _Refcnt(const char* _Ptr) noexcept
        {
            assert(_Ptr);
            NH3API_ASSUME(_Ptr != nullptr);
            return reinterpret_cast<uint8_t*>(const_cast<char*>(_Ptr))[-1];
        }

        // Split string into a new unique copy of string to prepare it for modification(append(), erase(), etc.):
        // 1. allocate a new buffer of size = min(capacity(), _New_size)
        // 2. copy the old string into a newly allocated buffer
        // 3. assign the newly allocated buffer pointer to the string pointer (if refcount is zero, deallocate the old buffer)
        void _Split()
        {
            char* const  _New_ptr      = _Allocate_for_capacity(_Myres);
            traits_type::copy(_New_ptr + 1, this->_Myptr, this->_Mysize + 1);
            _Tidy_deallocate();
            this->_Myptr          = _New_ptr + 1;
            _Refcnt(this->_Myptr) = 0;
        }

        // RAII char[] buffer that releases the array and the reference counting on destruction
        struct _Refcnt_buffer
        {
            public:
                _Refcnt_buffer(const _Refcnt_buffer&)            = delete;
                _Refcnt_buffer(_Refcnt_buffer&&)                 = delete;
                _Refcnt_buffer& operator=(const _Refcnt_buffer&) = delete;
                _Refcnt_buffer& operator=(_Refcnt_buffer&&)      = delete;

                NH3API_FORCEINLINE _Refcnt_buffer(char* ptr_, bool need_free_) noexcept
                    : _Ptr { ptr_ }, _Need_free { need_free_ }
                {}

                [[nodiscard]] NH3API_FORCEINLINE char* get() noexcept
                { return _Ptr; }

                [[nodiscard]] NH3API_FORCEINLINE const char* get() const noexcept
                { return _Ptr; }

                NH3API_FORCEINLINE void set(char* const _New_ptr) noexcept
                { _Ptr = _New_ptr; }

                NH3API_FORCEINLINE ~_Refcnt_buffer() noexcept
                {
                    if ( _Need_free && _Ptr )
                    {
                        if ( _Refcnt(_Ptr) == 0 || _Refcnt(_Ptr) == _FROZEN ) NH3API_LIKELY
                            _Deallocate(&_Ptr[-1]);
                        else
                            --_Refcnt(_Ptr);
                    }
                }

            private:
                char* _Ptr;
                bool _Need_free;
        };

        // _Refcnt_buffer but without local array check, similar to std::unique_ptr
        struct _Refcnt_fresh_buffer
        {
            public:
                _Refcnt_fresh_buffer(const _Refcnt_fresh_buffer&)            = delete;
                _Refcnt_fresh_buffer(_Refcnt_fresh_buffer&&)                 = delete;
                _Refcnt_fresh_buffer& operator=(const _Refcnt_fresh_buffer&) = delete;
                _Refcnt_fresh_buffer& operator=(_Refcnt_fresh_buffer&&)      = delete;

                NH3API_FORCEINLINE explicit _Refcnt_fresh_buffer(char* ptr_) noexcept
                    : _Ptr { ptr_ }
                {}

                [[nodiscard]] NH3API_FORCEINLINE char* get() noexcept
                { return _Ptr; }

                [[nodiscard]] NH3API_FORCEINLINE const char* get() const noexcept
                { return _Ptr; }

                NH3API_FORCEINLINE void set(char* const _New_ptr) noexcept
                { _Ptr = _New_ptr; }

                NH3API_FORCEINLINE ~_Refcnt_fresh_buffer() noexcept
                {
					if ( _Ptr )
					{
						if ( _Refcnt(_Ptr) == 0 || _Refcnt(_Ptr) == _FROZEN ) NH3API_LIKELY
							_Deallocate(&_Ptr[-1]);
						else
							--_Refcnt(_Ptr);
					}
                }

            private:
                char* _Ptr;
        };

        // Get the buffer to the string of size _Requested_size:
        // if the string reference counting is in shared mode, then we split the string and allocate a new buffer
        // otherwise, use the existing buffer
        NH3API_FORCEINLINE _Refcnt_buffer _Get_buffer(const size_t _Requested_size)
        {
            this->_Mysize = _Requested_size;
            if ( _Is_shared() || _Myres < _Requested_size ) NH3API_UNLIKELY
            {
                const size_t _New_capacity = _Calculate_growth(_Requested_size);
                char* const  _New_ptr      = _Allocate_for_capacity(_New_capacity) + 1;
                _Refcnt(_New_ptr)          = 0;
                this->_Myres               = _New_capacity;
                return { _New_ptr, true };
            }

            return { this->_Myptr, false };
        }

        // Split the string and allocate a new buffer no matter whether or not the string reference counting is in shared mode
        NH3API_FORCEINLINE _Refcnt_fresh_buffer _Get_fresh_buffer(const size_t _Requested_size)
        {
            const size_t _New_capacity = _Calculate_growth(_Requested_size);
            char* const  _New_ptr      = _Allocate_for_capacity(_New_capacity) + 1;
            _Refcnt(_New_ptr)          = 0;
            this->_Mysize              = _Requested_size;
            this->_Myres               = _New_capacity;
            return _Refcnt_fresh_buffer { _New_ptr };
        }

        inline bool _Is_shared() noexcept
        { return _Myptr && _Refcnt(_Myptr) != 0 && _Refcnt(_Myptr) != _FROZEN; }

    #if defined(__GNUC__) || defined(__clang__)
        [[__gnu__::__hot__]]
    #endif
        void _Tidy_deallocate() noexcept
        {
            if ( _Myptr ) NH3API_LIKELY
            {
                if ( _Is_shared() ) NH3API_UNLIKELY
                    --_Refcnt(_Myptr);
                else
                    _Deallocate(&_Myptr[-1]);
                _Myptr = nullptr;
            }
        }

        inline constexpr static bool _Equal(const char*  _Left,
                                            const size_t _Left_size,
                                            const char*  _Right,
                                            const size_t _Right_size) noexcept
        {
            if ( _Left_size != _Right_size )
                return false;

            if ( _Left_size == 0 )
                return true;

            return __builtin_memcmp(_Left, _Right, _Left_size) == 0;
        }

        friend nh3api::private_accessor<exe_string>;

        inline constexpr static int _Compare(const char*  _Left,
                                             const size_t _Left_size,
                                             const char*  _Right,
                                             const size_t _Right_size) noexcept
        {
            const int _Ans = __builtin_memcmp(_Left, _Right, std::min<size_t>(_Left_size, _Right_size));
            if ( _Ans != 0 )
                return _Ans;

            if ( _Left_size < _Right_size )
                return -1;

            if ( _Left_size > _Right_size )
                return 1;

            return 0;
        }

        inline void _Check_size(const size_t _Requested_size) const
        {
            if ( _Requested_size > max_size() ) NH3API_UNLIKELY
                _Throw_length_exception();
        }

        // checks whether _Off is in the bounds of [0, size()]
        inline void _Check_offset(const size_t _Offset) const
        {
            if ( size() < _Offset ) NH3API_UNLIKELY
                _Throw_out_of_range_exception();
        }

        // checks whether _Off is in the bounds of [0, size())
        inline void _Check_offset_exclusive(const size_t _Offset) const
        {
            if ( size() <= _Offset ) NH3API_UNLIKELY
                _Throw_out_of_range_exception();
        }

        // trims _Size to the longest it can be assuming a string at/after _Off
        [[nodiscard]] inline size_t _Clamp_suffix_size(const size_t _Off, const size_t _Size) const noexcept
        { return std::min<size_t>(_Size, size() - _Off); }

    protected:
        uint32_t : 32;  // padding
        char*  _Myptr;  // data()
        size_t _Mysize; // size()
        size_t _Myres;  // capacity()
};
#pragma pack(pop)

template<>
struct nh3api::private_accessor<exe_string>
{
    inline static bool equal(const exe_string& _Left, const exe_string& _Right) noexcept
    { return exe_string::_Equal(_Left.c_str(), _Left.size(), _Right.c_str(), _Right.size()); }

    inline static bool equal(const exe_string& _Left, const char* const _Right_string) noexcept
    {
        if ( _Right_string )
            return exe_string::_Equal(_Left.c_str(), _Left.size(), _Right_string, __builtin_strlen(_Right_string));

        return _Left.empty();
    }

    inline static bool equal(const char* const _Left_string, const exe_string& _Right) noexcept
    {
        if ( _Left_string )
            return exe_string::_Equal(_Left_string, __builtin_strlen(_Left_string), _Right.c_str(), _Right.size());

        return _Right.empty();
    }

    inline static void erase_unchecked(exe_string& _String, const size_t _Count)
    { _String._Erase_without_checks(0, _Count); }

    inline static void freeze(exe_string& _String) noexcept
    { return _String._Freeze(); }

};

//} // namespace nh3api

NH3API_SIZE_ASSERT(0x10, exe_string);

inline void swap(exe_string& _Left, exe_string& _Right) noexcept
{ _Left.swap(_Right); }

// concatenation

inline exe_string operator+(const exe_string& _Left, const exe_string& _Right)
{
    const size_t      _Left_size   = _Left.size();
    const size_t      _Right_size  = _Right.size();
    const char* const _Left_ptr    = _Left.data();
    const char* const _Right_ptr   = _Right.data();
    const bool        _Left_empty  = _Left_size == 0;
    const bool        _Right_empty = _Right_size == 0;

    if ( (_Left_ptr == nullptr && _Right_ptr == nullptr) || (_Left_empty && _Right_empty) )
        return {};

    if ( _Right_ptr == nullptr || _Right_empty )
        return _Left;

    if ( _Left_ptr == nullptr || _Left_empty )
        return _Right;

    if ( _Left.max_size() - _Left_size < _Right_size )
        nh3api::throw_exception<std::length_error>("string too long");

    return { exe_string_constructor_concat_tag{}, _Left_ptr, _Left_size, _Right_ptr, _Right_size };
}

inline exe_string operator+(const char* const _Left_string, const exe_string& _Right)
{
    if ( _Left_string == nullptr )
        return _Right;

    const char* const _Right_ptr = _Right.data();
    const size_t _Right_size     = _Right.size();
    if ( _Right_ptr == nullptr || _Right_size == 0 )
        return exe_string { _Left_string };

    const size_t _Left_size = __builtin_strlen(_Left_string);
    if ( _Right.max_size() - _Left_size < _Right_size )
        nh3api::throw_exception<std::length_error>("string too long");

    return { exe_string_constructor_concat_tag{}, _Left_string, _Left_size, _Right_ptr, _Right_size };
}

inline exe_string operator+(char _Left_character, const exe_string& _Right)
{
    const size_t      _Right_size = _Right.size();
    const char* const _Right_ptr  = _Right.data();
    if ( _Right_ptr == nullptr || _Right_size == 0 )
        return { _Left_character };

    if ( _Right.max_size() == _Right_size )
        nh3api::throw_exception<std::length_error>("string too long");

    return { exe_string_constructor_concat_tag {}, &_Left_character, 1U, _Right.c_str(), _Right_size };
}

inline exe_string operator+(const exe_string& _Left, const char* const _Right_string)
{
    if ( _Right_string == nullptr )
        return exe_string { _Right_string };

    const char* const _Left_ptr  = _Left.data();
    const size_t      _Left_size = _Left.size();
    if ( _Left_ptr == nullptr || _Left_size == 0 )
        return exe_string { _Right_string };

    const size_t _Right_size = __builtin_strlen(_Right_string);
    if ( _Left.max_size() - _Right_size < _Left_size )
        nh3api::throw_exception<std::length_error>("string too long");

    return { exe_string_constructor_concat_tag {}, _Right_string, _Right_size, _Left_ptr, _Left_size };
}

inline exe_string operator+(const exe_string& _Left, char _Right_character)
{
    const size_t      _Left_size = _Left.size();
    const char* const _Left_ptr  = _Left.data();
    if ( _Left_ptr == nullptr || _Left_size == 0 )
        return { _Right_character };

    if ( _Left.max_size() == _Left_size )
        nh3api::throw_exception<std::length_error>("string too long");

    return { exe_string_constructor_concat_tag {}, _Left_ptr, _Left_size, &_Right_character, 1 };
}

inline exe_string operator+(const exe_string& _Left, exe_string&& _Right)
{ return std::move(_Right.insert(0U, _Left)); }

inline exe_string operator+(exe_string&& _Left, const exe_string& _Right)
{ return std::move(_Left.append(_Right)); }

inline exe_string operator+(exe_string&& _Left, exe_string&& _Right)
{
    if ( (_Left.data() == nullptr && _Right.data() == nullptr) || (_Left.empty() && _Right.empty()) )
        return {};

    if ( _Right.data() == nullptr || _Right.empty() )
        return std::move(_Left);

    if ( _Left.data() == nullptr || _Left.empty() )
        return std::move(_Right);

    return { exe_string_constructor_concat_tag{}, _Left, _Right };
}

inline exe_string operator+(const char* const _Left_string, exe_string&& _Right)
{ return std::move(_Right.insert(0U, _Left_string)); }

inline exe_string operator+(char _Left, exe_string&& _Right)
{ return std::move(_Right.insert(0U, 1U, _Left)); }

inline exe_string operator+(exe_string&& _Left, const char* const _Right_string)
{ return std::move(_Left.append(_Right_string)); }

inline exe_string operator+(exe_string&& _Left, char _Right_character)
{
    _Left.push_back(_Right_character);
    return std::move(_Left);
}

// relational operators

inline bool operator==(const exe_string& _Left, const exe_string& _Right) noexcept
{ return nh3api::private_accessor<exe_string>::equal(_Left, _Right); }

inline bool operator==(const char* const _Left_string, const exe_string& _Right) noexcept
{ return nh3api::private_accessor<exe_string>::equal(_Left_string, _Right); }

#ifdef __cpp_lib_three_way_comparison
inline std::strong_ordering operator<=>(const exe_string& _Left, const exe_string& _Right) noexcept
{ return static_cast<std::strong_ordering>(_Left.compare(_Right) <=> 0); }

inline std::strong_ordering operator<=>(const exe_string& _Left, const char* const _Right_string) noexcept
{ return static_cast<std::strong_ordering>(_Left.compare(_Right_string) <=> 0); }

inline std::strong_ordering operator<=>(const char* const _Left_string, const exe_string& _Right) noexcept
{ return static_cast<std::strong_ordering>(_Right.compare(_Left_string) <=> 0); }
#else
inline bool operator==(const exe_string& _Left, const char* const _Right_string) noexcept
{ return nh3api::private_accessor<exe_string>::equal(_Left, _Right_string); }

inline bool operator<(const exe_string& _Left, const exe_string& _Right) noexcept
{ return _Left.compare(_Right) < 0; }

inline bool operator<(const char* const _Left_string, const exe_string& _Right) noexcept
{ return _Right.compare(_Left_string) > 0; }

inline bool operator<(const exe_string& _Left, const char* const _Right) noexcept
{ return _Left.compare(_Right) < 0; }

inline bool operator>(const exe_string& _Left, const exe_string& _Right) noexcept
{ return _Right < _Left; }

inline bool operator>(const char* const _Left_string, const exe_string& _Right) noexcept
{ return _Right < _Left_string; }

inline bool operator>(const exe_string& _Left, const char* const _Right_string) noexcept
{ return _Right_string < _Left; }

inline bool operator<=(const exe_string& _Left, const exe_string& _Right) noexcept
{ return !(_Right < _Left); }

inline bool operator<=(const char* const _Left_string, const exe_string& _Right) noexcept
{ return !(_Right < _Left_string); }

inline bool operator<=(const exe_string& _Left, const char* const _Right_string) noexcept
{ return !(_Right_string < _Left); }

inline bool operator>=(const exe_string& _Left, const exe_string& _Right) noexcept
{ return !(_Left < _Right); }

inline bool operator>=(const char* const _Left_string, const exe_string& _Right) noexcept
{ return !(_Left_string < _Right); }

inline bool operator>=(const exe_string& _Left, const char* const _Right_string) noexcept
{ return !(_Left < _Right_string); }

inline bool operator!=(const exe_string& _Left, const exe_string& _Right) noexcept
{ return (!(_Left == _Right)); }

inline bool operator!=(const char* _Left_string, const exe_string& _Right) noexcept
{ return (!(_Left_string == _Right)); }

inline bool operator!=(const exe_string& _Left, const char* _Right_string) noexcept
{ return (!(_Left == _Right_string)); }
#endif

namespace nh3api
{

// get exe_string reference count
// if you need it for whatever reason
inline uint8_t refcount(const ::exe_string& _String) noexcept
{ return static_cast<uint8_t>(_String.c_str()[-1]); }

// optimization hint: assume that the current exe_string variable is not shared
// use it ONLY when you are 100% certain.
inline void assume_not_shared(const ::exe_string& _String) noexcept
{
    if ( _String.c_str() )
        if ( (refcount(_String) != 0 && refcount(_String) != 255) )
            NH3API_UNREACHABLE();
}

// fast algorithm by void_17
template<class StringT>
static StringT int_to_string(int32_t _Value)
{
    using CharT = typename StringT::value_type;

    if ( _Value == 0 )
        return StringT(1, CharT('0'));
    bool32_t _Minus_symbol = (_Value < 0);
    _Value = ::abs32(_Value);
    const size_t _Num_symbols = _Minus_symbol + count_digits(static_cast<uint32_t>(_Value));
    // hint the compiler for small string optimization for std::basic_string
    NH3API_ASSUME(_Num_symbols <= 10);
    StringT _Result(_Num_symbols, CharT('\0'));
    if ( _Minus_symbol )
        _Result[0] = '-';
    CharT* _Ptr = const_cast<CharT*>(_Result.c_str()) + _Result.size()-1;
    while (_Value > 0)
    {
        uint8_t _Digit = static_cast<uint8_t>(_Value % 10);
        if constexpr ( ::std::is_same_v<CharT, char> )
            *_Ptr-- = static_cast<char>(('0' + _Digit) & INT8_MAX);
        else if constexpr ( ::std::is_same_v<CharT, wchar_t> )
            *_Ptr-- = static_cast<wchar_t>((L'0' + _Digit) & INT16_MAX);
        _Value /= 10;
    }
    return _Result;
}

// fast algorithm by void_17
template<class StringT>
static StringT uint_to_string(uint32_t _Value)
{
    using CharT = typename StringT::value_type;
    if ( _Value == 0 )
        return StringT(1, CharT('0'));
    const size_t _Num_symbols = count_digits(_Value);
    // hint the compiler for small string optimization for std::basic_string
    NH3API_ASSUME(_Num_symbols <= 11);
    StringT _Result(_Num_symbols, CharT('\0'));
    CharT* _Ptr = const_cast<CharT*>(_Result.c_str()) + _Result.size()-1;
    while (_Value > 0)
    {
        uint8_t _Digit = static_cast<uint8_t>(_Value % 10);
        if constexpr ( ::std::is_same_v<CharT, char> )
            *_Ptr-- = static_cast<char>(('0' + _Digit) & INT8_MAX);
        else if constexpr ( ::std::is_same_v<CharT, wchar_t> )
            *_Ptr-- = static_cast<wchar_t>((L'0' + _Digit) & INT16_MAX);
        _Value /= 10;
    }
    return _Result;
}

inline constexpr const char* get_inf_string(char) noexcept
{ return "INF"; }

inline constexpr const wchar_t* get_inf_string(wchar_t) noexcept
{ return L"INF"; }

inline constexpr const char* get_nan_string(char) noexcept
{ return "NaN"; }

inline constexpr const wchar_t* get_nan_string(wchar_t) noexcept
{ return L"NaN"; }

inline constexpr const char* get_error_fp_string(char) noexcept
{ return "ERROR"; }

inline constexpr const wchar_t* get_error_fp_string(wchar_t) noexcept
{ return L"ERROR"; }

template<class StringT>
static StringT double_to_string(double _Value, int32_t _Precision)
{
    using CharT = typename StringT::value_type;
    if ( _Precision < 1 || _Precision > 18 )
        return StringT(get_error_fp_string(CharT()), 5);

    switch (::std::fpclassify(_Value))
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
            // 32 is more than enough.
            std::array<CharT, 32> buffer{};

            if constexpr (::std::is_same_v<CharT, char>)
            {
                sprintf_s(const_cast<char*>(buffer.data()), buffer.size(), "%.*f", _Precision, _Value);
            }
            else if (::std::is_same_v<CharT, wchar_t>)
            {
                swprintf_s(const_cast<wchar_t*>(buffer.data()), buffer.size(), L"%.*f", _Precision, _Value);
            }
            else
            {}

            return StringT { buffer.data() };
        }
        default:
            return StringT (get_error_fp_string(CharT()), 5);
    }
}

} // namespace nh3api


// write signed integer as string
inline exe_string to_exe_string(int32_t _Value)
{ return nh3api::int_to_string<exe_string>(_Value); }

// write unsigned integer as string
inline exe_string to_exe_string(uint32_t _Value)
{ return nh3api::uint_to_string<exe_string>(_Value); }

// write double as string
inline exe_string to_exe_string(double _Value, int32_t _Precision = 8)
{ return nh3api::double_to_string<exe_string>(_Value, _Precision); }

// write float as string
inline exe_string to_exe_string(float _Value, int32_t _Precision = 4)
{ return nh3api::double_to_string<exe_string>(static_cast<double>(_Value), _Precision); }

// write signed integer as string
inline std::string to_std_string(int32_t _Value)
{ return nh3api::int_to_string<std::string>(_Value); }

// write unsigned integer as string
inline std::string to_std_string(uint32_t _Value)
{ return nh3api::uint_to_string<std::string>(_Value); }

// write double as string
inline std::string to_std_string(double _Value, int32_t _Precision = 8)
{ return nh3api::double_to_string<std::string>(_Value, _Precision); }

// write float as string
inline std::string to_std_string(float _Value, int32_t _Precision = 4)
{ return nh3api::double_to_string<std::string>(static_cast<double>(_Value), _Precision); }

// write signed integer as wide string
inline std::wstring to_std_wstring(int32_t _Value)
{ return nh3api::int_to_string<std::wstring>(_Value); }

// write unsigned integer as wide string
inline std::wstring to_std_wstring(uint32_t _Value)
{ return nh3api::uint_to_string<std::wstring>(_Value); }

// write double as wide string
inline std::wstring to_std_wstring(double _Value, int32_t _Precision = 8)
{ return nh3api::double_to_string<std::wstring>(_Value, _Precision); }

// write float as wide string
inline std::wstring to_std_wstring(float _Value, int32_t _Precision = 4)
{ return nh3api::double_to_string<std::wstring>(static_cast<double>(_Value), _Precision); }

namespace nh3api
{

inline size_t hash_string(const ::exe_string& str) noexcept
{
    default_hash hasher;
    hasher.update(str.c_str(), str.size());
    return hasher.digest();
}

inline size_t hash_string(const ::std::string& str) noexcept
{
    default_hash hasher;
    hasher.update(str.c_str(), str.size());
    return hasher.digest();
}

inline size_t hash_string(const ::std::wstring& str) noexcept
{
    default_hash hasher;
    hasher.update(str.c_str(), str.size());
    return hasher.digest();
}

} // namespace nh3api

// std::hash support for exe_string
template<>
struct std::hash<exe_string>
{
    public:
    #ifdef __cpp_static_call_operator
        static
    #endif
        size_t operator()(const exe_string& str)
    #ifndef __cpp_static_call_operator
        const
    #endif
        noexcept
        {
            nh3api::default_hash hasher;
            hasher.update(str.data(), str.size());
            return hasher.digest();
        }
};

// std::string which uses exe_allocator<char>
using exe_std_string = std::basic_string<char, std::char_traits<char>, exe_allocator<char>>;
// std::wstring which uses exe_allocator<wchar_t>
using exe_std_wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, exe_allocator<wchar_t>>;

NH3API_WARNING(pop)
