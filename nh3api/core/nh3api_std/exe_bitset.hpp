//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2026
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

#include <functional> // std::hash
#include <stdexcept>  // std::invalid_argument, std::overflow_error, std::out_of_range
#include <string>     // std::string, std::string_view, std::char_traits

#include "stl_extras.hpp"        // in_range
#include "exe_string.hpp"        // exe_string
#include "intrin.hpp"            // bitpopcnt
#include "nh3api_exceptions.hpp" // nh3api::throw_exception

template<size_t N>
class exe_bitset;
template<size_t N>
struct std::hash<exe_bitset<N>>;

template<size_t N>
struct nh3api::private_accessor<exe_bitset<N>>;

#pragma pack(push, 8)
// Visual C++ 6.0 std::bitset implementation used by heroes3.exe
template<size_t _Bits>
class NH3API_CLANG_TRIVIAL_ABI exe_bitset
{
protected:
    using word_type = uint32_t;

public:
    using element_type = bool;

    // proxy class representing a reference to a bit inside bitset
    class NH3API_CLANG_TRIVIAL_ABI reference
    {
    friend class exe_bitset<_Bits>;
    public:
        reference(const reference&) = default;

        constexpr reference& operator=(bool _Value) noexcept
        {
            _parent._Set_unchecked(_pos, _Value);
            return *this;
        }

        constexpr reference& operator=(const reference& _Other) noexcept
        {
            _parent._Set_unchecked(_pos, static_cast<bool>(_Other));
            return *this;
        }

        constexpr reference& flip() noexcept
        {
            _parent._Flip_unchecked(_pos);
            return *this;
        }

        constexpr bool operator~() const noexcept
        { return !_parent._Subscript(_pos); }

        [[nodiscard]] constexpr explicit operator bool() const noexcept
        { return _parent._Subscript(_pos); }

        ~reference()                                                = default;
        reference()                                                 = delete;
        inline constexpr reference(reference&&) noexcept            = default;
        inline constexpr reference& operator=(reference&&) noexcept = default;

    private:
        constexpr reference(exe_bitset<_Bits>& _Parent, size_t _Pos) noexcept
            : _parent(_Parent), _pos(_Pos)
        {}

        exe_bitset<_Bits>& _parent;
        size_t _pos;
    };

public:
    [[nodiscard]] constexpr bool operator[](const size_t _Pos) const
#if NH3API_DEBUG
    noexcept(false)
#else
    noexcept
#endif
    {
    #if NH3API_DEBUG
        if ( _Pos >= _Bits )
            _Throw_invalid_subscript();
    #endif

        return _Subscript(_Pos);
    }

    constexpr reference operator[](const size_t _Pos)
#if NH3API_DEBUG
    noexcept(false)
#else
    noexcept
#endif
    {
        #if NH3API_DEBUG
        if ( _Pos >= _Bits )
            _Throw_invalid_subscript();
        #endif

        return reference { *this, _Pos };
    }

    // construct with all false values
    constexpr exe_bitset() noexcept
        : _Array{}
    {}

    inline explicit constexpr exe_bitset(const uint64_t _Value) noexcept
        : _Array{static_cast<word_type>(_Need_mask ? _Value & _Mask : _Value)}
    {}

    template<class CharT, class CharTraits, class Allocator> explicit
    exe_bitset(const std::basic_string<CharT, CharTraits, Allocator>& _String,
               typename std::basic_string<CharT, CharTraits, Allocator>::size_type _Pos = 0,
               typename std::basic_string<CharT, CharTraits, Allocator>::size_type _Count = std::basic_string<CharT, CharTraits, Allocator>::npos,
               CharT _Zero = CharT('0'),
               CharT _One  = CharT('1'))
    {
        if (_String.empty())
            return;

        if (_String.size() < _Pos)
            _Throw_invalid_subscript();

        if (_String.size() - _Pos < _Count)
            _Count = _String.size() - _Pos;

        _Construct_from_string<CharTraits>(_String.data() + _Pos, static_cast<size_t>(_Count), _Zero, _One);
    }

    template<class CharT, class CharTraits> explicit
    exe_bitset(const std::basic_string_view<CharT, CharTraits>& _String,
               typename std::basic_string_view<CharT, CharTraits>::size_type _Pos = 0,
               typename std::basic_string_view<CharT, CharTraits>::size_type _Count = std::basic_string_view<CharT, CharTraits>::npos,
               CharT _Zero = CharT('0'),
               CharT _One  = CharT('1'))
    {
        if ( _Pos > _String.size() )
            _Throw_invalid_subscript();

        const size_t _Length = std::min<size_t>(static_cast<size_t>(_Count), static_cast<size_t>(_String.size() - _Pos));
        _Construct_from_string<CharTraits>(_String.data() + _Pos, _Length, _Zero, _One);
    }

    template<typename = char>
    explicit exe_bitset(const char* _String,
                        size_t _Count = std::string_view::npos,
                        const char _Zero = '0',
                        const char _One  = '1')
    {
        if ( _String == nullptr )
            return;

        if ( _Count == std::string::npos )
            _Count = __builtin_strlen(_String);

        _Construct_from_string<std::char_traits<char>>(_String, _Count, _Zero, _One);
    }

    template<typename = wchar_t>
    explicit exe_bitset(const wchar_t* _String,
                        size_t _Count = std::wstring_view::npos,
                        wchar_t _Zero = L'0',
                        wchar_t _One  = L'1')
    {
        if ( _String == nullptr )
            return;

        #if NH3API_HAS_BUILTIN(__builtin_wcslen)
        if ( _Count == std::string::npos )
            _Count = __builtin_wcslen(_String);
        #else
        if ( _Count == std::string::npos )
            _Count = std::char_traits<wchar_t>::length(_String);
        #endif

        _Construct_from_string<std::char_traits<wchar_t>>(_String, _Count, _Zero, _One);
    }

    inline exe_bitset(const nh3api::dummy_tag_t&) noexcept
    {}

    inline constexpr exe_bitset<_Bits>& operator&=(const exe_bitset<_Bits>& _Other) noexcept
    {
        for (int i = _Words; 0 <= i; --i)
            _Array[i] &= _Other._Array[i];

        return *this;
    }

    inline constexpr exe_bitset<_Bits>& operator|=(const exe_bitset<_Bits>& _Other) noexcept
    {
        for (int i = _Words; 0 <= i; --i)
            _Array[i] |= _Other._Array[i];

        return *this;
    }

    inline constexpr exe_bitset<_Bits>& operator^=(const exe_bitset<_Bits>& _Other) noexcept
    {
        for (int i = _Words; 0 <= i; --i)
            _Array[i] ^= _Other._Array[i];

        return *this;
    }

    // shift left by _Shift, first by words then by bits
    inline constexpr exe_bitset<_Bits>& operator<<=(size_t _Shift) noexcept
    {
        const auto _Wordshift = static_cast<ptrdiff_t>(_Shift / _Bitsperword);
        if (_Wordshift != 0)
            for (ptrdiff_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
                _Array[_Wpos] = _Wordshift <= _Wpos ? _Array[_Wpos - _Wordshift] : 0;

        // 0 < pos < _Bitsperword, shift by bits
        if ((_Shift %= _Bitsperword) != 0)
        {
            for (ptrdiff_t _Wpos = _Words; 0 < _Wpos; --_Wpos)
                _Array[_Wpos] = (_Array[_Wpos] << _Shift) | (_Array[_Wpos - 1] >> (_Bitsperword - _Shift));

            _Array[0] <<= _Shift;
        }

        _Trim();
        return *this;
    }

    // shift right by _Shift, first by words then by bits
    inline constexpr exe_bitset<_Bits>& operator>>=(size_t _Shift) noexcept
    {
        const auto _Wordshift = static_cast<ptrdiff_t>(_Shift / _Bitsperword);
        if (_Wordshift != 0)
            for (ptrdiff_t _Wpos = 0; _Wpos <= _Words; ++_Wpos)
                _Array[_Wpos] = _Wordshift <= _Words - _Wpos ? _Array[_Wpos + _Wordshift] : 0;

        // 0 < pos < _Bitsperword, shift by bits
        if ((_Shift %= _Bitsperword) != 0)
        {
            for (ptrdiff_t _Wpos = 0; _Wpos < _Words; ++_Wpos)
                _Array[_Wpos] = (_Array[_Wpos] >> _Shift) | (_Array[_Wpos + 1] << (_Bitsperword - _Shift));

            _Array[_Words] >>= _Shift;
        }

        return *this;
    }

    // set all bits true
    constexpr exe_bitset<_Bits>& set() noexcept
    {
        NH3API_IF_CONSTEVAL
        {
            for ( word_type& word : _Array )
                word = static_cast<word_type>(-1);
        }
        else
        {
            #if NH3API_HAS_BUILTIN(__builtin_memset_inline)
            __builtin_memset_inline(&_Array, 0xFF, sizeof(_Array));
            #else
            std::memset(&_Array, 0xFF, sizeof(_Array));
            #endif
        }
        _Trim();
        return *this;
    }

    // set bit at pos to value
    inline constexpr exe_bitset<_Bits>& set(const size_t _Pos, const bool _Value = true)
    {
        if ( _Bits <= _Pos )
            _Throw_invalid_subscript();
        return _Set_unchecked(_Pos, _Value);
    }

    // set all bits false
    inline constexpr exe_bitset<_Bits>& reset() noexcept
    {
        NH3API_IF_CONSTEVAL
        {
            for ( word_type& word : _Array )
                word = 0;
        }
        else
        {
            #if NH3API_HAS_BUILTIN(__builtin_memset_inline)
            __builtin_memset_inline(&_Array, 0, sizeof(_Array));
            #else
            std::memset(&_Array, 0, sizeof(_Array));
            #endif
        }
        return *this;
    }

    // set bit at pos to false
    constexpr exe_bitset<_Bits>& reset(size_t _Pos)
    { return set(_Pos, 0); }

    // flip all bits
    constexpr exe_bitset<_Bits> operator~() const noexcept
    {
        exe_bitset<_Bits> result = *this;
        result.flip();
        return result;
    }

    // flip all bits
    constexpr exe_bitset<_Bits>& flip() noexcept
    {
        for (size_t _Wpos = 0; _Wpos <= _Words; ++_Wpos)
            _Array[_Wpos] = ~_Array[_Wpos];

        _Trim();
        return *this;
    }

    // flip bit at pos
    constexpr exe_bitset<_Bits>& flip(size_t _Pos)
    {
        if (_Bits <= _Pos)
            _Throw_invalid_subscript();

        return _Flip_unchecked(_Pos);
    }

    // extract underlying unsigned long integer from the bitset
    [[nodiscard]] constexpr unsigned long to_ulong() const noexcept(_Bits <= 32)
    {
        static_assert(sizeof(unsigned long) % sizeof(word_type) == 0);
        constexpr bool _Bits_zero  = _Bits == 0;
        constexpr bool _Bits_small = _Bits <= 32;
        constexpr bool _Bits_large = _Bits > 64;
        if constexpr (_Bits_zero)
        {
            return 0;
        }
        else if constexpr (_Bits_small)
        {
            return static_cast<unsigned long>(_Array[0]);
        }
        else
        {
            if constexpr (_Bits_large)
                for (size_t _Idx = 1; _Idx <= _Words; ++_Idx)
                    if (_Array[_Idx] != 0)
                        _Throw_ulong_conversion_overflow(); // fail if any high-order words are nonzero

            if (!nh3api::in_range<unsigned long>(_Array[0]))
                _Throw_ulong_conversion_overflow();

            return static_cast<unsigned long>(_Array[0]);
        }
    }

    // extract underlying unsigned long long integer from the bitset
    [[nodiscard]] constexpr unsigned long long to_ullong() const noexcept(_Bits <= 64)
    {
        constexpr bool _Bits_zero  = _Bits == 0;
        constexpr bool _Bits_large = _Bits > 64;
        if constexpr (_Bits_zero)
        {
            return 0;
        }
        else
        {
            if constexpr (_Bits_large)
                for (size_t _Idx = 1; _Idx <= _Words; ++_Idx)
                    if (_Array[_Idx] != 0)
                        _Throw_ullong_conversion_overflow(); // fail if any high-order words are nonzero

            return _Array[0];
        }
    }

    [[nodiscard]] exe_string to_string(const char _Zero = '0', const char _One = '1') const
    {
        exe_string result(_Bits, _Zero);
        char* dst = result.data();
        for (size_t i = _Bits; 0 < i; ++dst )
            *dst = _Subscript(--i) ? _One : _Zero;
        return result;
    }

    #ifdef __clang__
    NH3API_WARNING(push)
    NH3API_WARNING_GNUC_DISABLE("-Wbit-int-extension")
    #endif
    [[nodiscard]] constexpr size_t count() const noexcept
    {
        if constexpr ( _Bits == 0 )
        {
            return 0;
        }
        else if constexpr (_Bits <= _Bitsperword)
        {
            #if defined(__BITINT_MAXWIDTH__) && NH3API_HAS_BUILTIN(__builtin_popcountg)
                return static_cast<size_t>(__builtin_popcountg(static_cast<unsigned _BitInt(_Bits)>(_Array[0])));
            #elif NH3API_HAS_BUILTIN(__builtin_popcount)
                return static_cast<size_t>(__builtin_popcount(_Array[0]));
            #else
                return static_cast<size_t>(bitpopcnt(_Array[0]));
            #endif
        }
        else
        {
            size_t result = 0;
            for (size_t i = 0; i < _Words + 1; ++i)
            #if NH3API_HAS_BUILTIN(__builtin_popcount)
                result += static_cast<size_t>(__builtin_popcount(_Array[i]));
            #else
                result += static_cast<size_t>(bitpopcnt(_Array[i]));
            #endif

            return result;
        }
    }
    #ifdef __clang__
    NH3API_WARNING(pop)
    #endif

    [[nodiscard]] constexpr size_t size() const noexcept
    { return _Bits; }

    constexpr bool operator==(const exe_bitset<_Bits>& _Other) const noexcept
    {
        NH3API_IF_CONSTEVAL
        {
            for (int i = _Words; 0 <= i; --i)
                if (_Array[i] != _Other._Array[i])
                    return false;
            return true;
        }
        else
        {
            return __builtin_memcmp(&_Array[0], &_Other._Array[0], sizeof(_Array)) == 0;
        }
    }

    constexpr bool operator!=(const exe_bitset<_Bits>& _Other) const noexcept
    { return !(*this == _Other); }

    [[nodiscard]] constexpr bool test(const size_t _Pos) const
    {
        if ( _Bits <= _Pos )
            _Throw_invalid_subscript();

        return _Subscript(_Pos);
    }

    [[nodiscard]] constexpr bool any() const noexcept
    {
        for (int i = _Words; 0 <= i; --i)
            if (_Array[i] != 0)
                return true;
        return false;
    }

    [[nodiscard]] constexpr bool all() const noexcept
    {
        constexpr bool _Zero_length = _Bits == 0;

        // must test for this, otherwise would count one full word
        if constexpr (_Zero_length)
            return true;

        constexpr bool _No_padding = _Bits % _Bitsperword == 0;
        for (size_t _Wpos = 0; _Wpos < _Words + _No_padding; ++_Wpos)
            if (_Array[_Wpos] != ~static_cast<word_type>(0))
                return false;

        return _No_padding || _Array[_Words] == (static_cast<word_type>(1) << (_Bits % _Bitsperword)) - 1;
    }

    [[nodiscard]] constexpr bool none() const noexcept
    { return !any(); }

    constexpr exe_bitset<_Bits> operator<<(size_t _Shift) const noexcept
    {
        exe_bitset<_Bits> result = *this;
        result <<= _Shift;
        return result;
    }

    constexpr exe_bitset<_Bits> operator>>(size_t _Shift) const noexcept
    {
        exe_bitset<_Bits> result = *this;
        result >>= _Shift;
        return result;
    }

protected:
    // used for iterations inside the bitset.
    // no need to check for bounds.
    [[nodiscard]] constexpr bool _Subscript(size_t _Pos) const noexcept
    { return (_Array[_Pos / _Bitsperword] & (word_type{1} << _Pos % _Bitsperword)) != 0; }

    static constexpr ptrdiff_t _Bitsperword = CHAR_BIT * sizeof(word_type);
    static constexpr ptrdiff_t _Words       = _Bits == 0 ? 0 : (_Bits - 1) / _Bitsperword; // NB: number of words - 1
    static constexpr bool      _Need_mask   = _Bits < CHAR_BIT * sizeof(uint64_t);
    static constexpr uint64_t  _Mask        = (1ULL << (_Need_mask ? _Bits : 0)) - 1ULL;

    template<class TraitsType, class CharType = typename TraitsType::char_type>
    inline
#ifdef __cpp_lib_constexpr_string
            constexpr
#endif
    void _Construct_from_string(const CharType*                                                  _String,
                                size_t _Count,
                                const CharType                                                   _Zero,
                                const CharType                                                   _One)
    {
        if ( _Count > _Bits )
        {
            for ( size_t _Idx = _Bits; _Idx < _Count; ++_Idx )
            {
                const auto _Ch = _String[_Idx];
                if ( !TraitsType::eq(_One, _Ch) && !TraitsType::eq(_Zero, _Ch) )
                    _Throw_invalid_char();
            }

            _Count = _Bits;
        }

        size_t _Wpos = 0;
        if ( _Count != 0 )
        {
            size_t    _Bits_used_in_word = 0;
            auto      _Last              = _String + _Count;
            word_type _This_word         = 0;
            do
            {
                --_Last;
                const auto _Ch  = *_Last;
                _This_word     |= static_cast<word_type>(TraitsType::eq(_One, _Ch)) << _Bits_used_in_word;
                if ( !TraitsType::eq(_One, _Ch) && !TraitsType::eq(_Zero, _Ch) )
                    _Throw_invalid_char();

                if ( ++_Bits_used_in_word == _Bitsperword )
                {
                    _Array[_Wpos] = _This_word;
                    ++_Wpos;
                    _This_word         = 0;
                    _Bits_used_in_word = 0;
                }
            }
            while ( _String != _Last );

            if ( _Bits_used_in_word != 0 )
            {
                _Array[_Wpos] = _This_word;
                ++_Wpos;
            }
        }

        for ( ; _Wpos <= _Words; ++_Wpos )
            _Array[_Wpos] = 0;
    }

    // clear any trailing bits in last word
    constexpr void _Trim() noexcept
    {
        constexpr bool _Work_to_do = _Bits == 0 || _Bits % _Bitsperword != 0;
        if constexpr (_Work_to_do)
            _Array[_Words] &= (word_type{1} << _Bits % _Bitsperword) - 1;
    }

    // set bit at _Pos to _Val, no checking
    constexpr exe_bitset& _Set_unchecked(const size_t _Pos, const bool _Value) noexcept
    {
        auto& _Selected_word = _Array[_Pos / _Bitsperword];
        const auto _Bit      = word_type{1} << _Pos % _Bitsperword;
        if (_Value)
            _Selected_word |= _Bit;
        else
            _Selected_word &= ~_Bit;

        return *this;
    }

    // flip bit at _Pos, no checking
    constexpr exe_bitset& _Flip_unchecked(const size_t _Pos) noexcept
    {
        _Array[_Pos / _Bitsperword] ^= word_type{1} << _Pos % _Bitsperword;
        return *this;
    }

    [[noreturn]] NH3API_FORCEINLINE static void _Throw_invalid_char() noexcept(false)
    { nh3api::throw_exception<std::invalid_argument>("bitset construction failure: invalid bitset char"); }

    [[noreturn]] NH3API_FORCEINLINE static void _Throw_ulong_conversion_overflow() noexcept(false)
    { nh3api::throw_exception<std::overflow_error>("bitset::to_ulong conversion overflow"); }

    [[noreturn]] NH3API_FORCEINLINE static void _Throw_ullong_conversion_overflow() noexcept(false)
    { nh3api::throw_exception<std::overflow_error>("bitset::to_ullong conversion overflow"); }

    [[noreturn]] NH3API_FORCEINLINE static void _Throw_invalid_subscript() noexcept(false)
    { nh3api::throw_exception<std::out_of_range>("invalid bitset position"); }

    [[nodiscard]] constexpr size_t _Hash_code() noexcept
    {
        nh3api::default_hash hasher;
        hasher.update(&_Array[0], _Words + 1);
        return hasher.digest();
    }

    // allow access to _Hash_code() for std::hash
    friend std::hash< exe_bitset<_Bits> >;

    template<typename StringT> NH3API_FORCEINLINE
    StringT _To_std_string(
        typename StringT::value_type _Zero = typename StringT::value_type('0'),
        typename StringT::value_type _One = typename StringT::value_type('1')) const
    {
        StringT result(_Bits, _Zero);
        typename StringT::value_type* dst = const_cast<typename StringT::value_type*>(result.c_str());
        for (size_t i = _Bits; 0 < i; ++dst )
            *dst = _Subscript(--i) ? _One : _Zero;
        return result;
    }

    friend nh3api::private_accessor<exe_bitset<_Bits>>;

    // bit array
    word_type _Array[_Words + 1];
};
#pragma pack(pop)

template<size_t N>
inline constexpr exe_bitset<N> operator&(const exe_bitset<N>& lhs, const exe_bitset<N>& rhs) noexcept
{
    exe_bitset<N> result = lhs;
    result &= rhs;
    return result;
}

template<size_t N>
inline constexpr exe_bitset<N> operator|(const exe_bitset<N>& lhs, const exe_bitset<N>& rhs) noexcept
{
    exe_bitset<N> result = lhs;
    result |= rhs;
    return result;
}

template<size_t N>
inline constexpr exe_bitset<N> operator^(const exe_bitset<N>& lhs, const exe_bitset<N>& rhs) noexcept
{
    exe_bitset<N> result = lhs;
    result ^= rhs;
    return result;
}
// std::hash support for exe_bitset
template<size_t N>
struct std::hash< exe_bitset<N> >
{
    public:
    #if defined(__cpp_static_call_operator) && NH3API_CHECK_CPP23
        static
    #endif
        size_t operator()(const exe_bitset<N>& arg)
    #if !(defined(__cpp_static_call_operator) && NH3API_CHECK_CPP23)
        const
    #endif
        noexcept
        { return arg._Hash_code(); }
};

template<size_t N>
struct nh3api::private_accessor<exe_bitset<N>>
{
    template<typename StringT> NH3API_FORCEINLINE
    static StringT convert(const exe_bitset<N>& arg, char zero = '0', char one = '1')
    { return arg.template _To_std_string<StringT>(zero, one); }
};

template<size_t N>
std::string to_std_string(const exe_bitset<N>& arg, char zero = '0', char one = '1')
{ return nh3api::private_accessor<exe_bitset<N>>::template convert<std::string>(arg, zero, one); }

template<size_t N>
std::wstring to_std_wstring(const exe_bitset<N>& arg, wchar_t zero = L'0', wchar_t one = L'1')
{ return nh3api::private_accessor<exe_bitset<N>>::template convert<std::wstring>(arg, zero, one); }

template<size_t N>
exe_string to_exe_string(const exe_bitset<N>& arg, char zero = '0', char one = '1')
{ return arg.to_string(zero, one); }
