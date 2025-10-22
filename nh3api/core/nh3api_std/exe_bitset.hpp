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

#include "exe_string.hpp" // exe_string, exceptions
#include "intrin.hpp" // bitpopcnt

template<size_t N>
struct exe_bitset_to_string_helper;

template<size_t N>
class exe_bitset;
template<size_t N>
struct std::hash<exe_bitset<N>>;

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

        constexpr reference& operator=(bool value) noexcept
        {
            _parent->_Set_unchecked(_pos, value);
            return *this;
        }

        constexpr reference& operator=(const reference& other) noexcept
        {
            _parent->_Set_unchecked(_pos, static_cast<bool>(other));
            return *this;
        }

        constexpr reference& flip() noexcept
        {
            _parent->_Flip_unchecked(_pos);
            return *this;
        }

        constexpr bool operator~() const noexcept
        { return !_parent->_Subscript(_pos); }

        [[nodiscard]] constexpr operator bool() const noexcept
        { return _parent->_Subscript(_pos); }

        ~reference() = default;

        constexpr reference() noexcept NH3API_DELETED_FUNCTION

    private:
        constexpr reference(exe_bitset<_Bits>* value, size_t pos) noexcept
            : _parent(value), _pos(pos)
        {}

        exe_bitset<_Bits>* _parent;
        size_t _pos;
    };

public:
    [[nodiscard]] constexpr bool operator[](size_t pos) const noexcept(nh3api::flags::nodebug)
    {
        #if NH3API_DEBUG
        if ( pos >= _Bits )
            _Throw_invalid_subscript();
        #endif

        return _Subscript(pos);
    }

    constexpr reference operator[](size_t pos) noexcept(nh3api::flags::nodebug)
    {
        #if NH3API_DEBUG
        if ( pos >= _Bits )
            _Throw_invalid_subscript();
        #endif

        return reference(*this, pos);
    }

    // construct with all false values
    constexpr exe_bitset() noexcept
        : _Array()
    {}

    explicit constexpr exe_bitset(unsigned long long value) noexcept
        : _Array{static_cast<word_type>(_Need_mask ? value & _Mask : value)}
    {}

    template<class CharT, class CharTraits, class Allocator> explicit
    exe_bitset(const std::basic_string<CharT, CharTraits, Allocator>& str,
               typename std::basic_string<CharT, CharTraits, Allocator>::size_type pos = 0,
               typename std::basic_string<CharT, CharTraits, Allocator>::size_type count = std::basic_string<CharT, CharTraits, Allocator>::npos,
               CharT zero = CharT('0'),
               CharT one  = CharT('1'))
    {
        if (str.empty())
            return;

        if (str.size() < pos)
            _Throw_invalid_subscript();

        if (str.size() - pos < count)
            count = str.size() - pos;

        _Construct<CharTraits>(str.data() + pos, count, zero, one);
    }

    template<class CharT, class CharTraits> explicit
    exe_bitset(const std::basic_string_view<CharT, CharTraits>& str,
               typename std::basic_string_view<CharT, CharTraits>::size_type pos = 0,
               typename std::basic_string_view<CharT, CharTraits>::size_type count = std::basic_string_view<CharT, CharTraits>::npos,
               CharT zero = CharT('0'),
               CharT one  = CharT('1'))
    {
        if ( pos > str.size() )
            _Throw_invalid_subscript();

        const size_t length = (std::min)(count, str.size() - pos);
        _Construct<CharTraits>(str.data() + pos, length, zero, one);
    }

    template<typename = char> explicit
    exe_bitset(const char* str,
               typename std::string_view::size_type count = std::string_view::npos,
               char zero = '0',
               char one  = '1')
    {
        if ( str == nullptr )
            return;

        if ( count == std::string::npos )
        #if __has_builtin(__builtin_strlen)
            count = __builtin_strlen(str);
        #else
            count = ::std::strlen(str);
        #endif

        _Construct<
        #ifdef __cpp_lib_constexpr_string
        std::char_traits<char>
        #else
        nh3api::constexpr_char_traits
        #endif
        >(str, count, zero, one);
    }

    template<typename = wchar_t> explicit
    exe_bitset(const wchar_t* str,
               typename std::wstring_view::size_type count = std::wstring_view::npos,
               wchar_t zero = L'0',
               wchar_t one  = L'1')
    {
        if ( str == nullptr )
            return;

        if ( count == std::string::npos )
        #if __has_builtin(__builtin_wcslen)
            count = __builtin_wcslen(str);
        #else
            count = ::std::wcslen(str);
        #endif

        _Construct<std::char_traits<wchar_t>>(str, count, zero, one);
    }

    NH3API_FORCEINLINE
    exe_bitset(const ::nh3api::dummy_tag_t&) noexcept
    {}

    constexpr exe_bitset<_Bits>& operator&=(const exe_bitset<_Bits>& other) noexcept
    {
        for (int i = _Words; 0 <= i; --i)
            _Array[i] &= other._Array[i];

        return *this;
    }

    constexpr exe_bitset<_Bits>& operator|=(const exe_bitset<_Bits>& other) noexcept
    {
        for (int i = _Words; 0 <= i; --i)
            _Array[i] |= other._Array[i];

        return *this;
    }

    constexpr exe_bitset<_Bits>& operator^=(const exe_bitset<_Bits>& other) noexcept
    {
        for (int i = _Words; 0 <= i; --i)
            _Array[i] ^= other._Array[i];

        return *this;
    }

    // shift left by pos, first by words then by bits
    constexpr exe_bitset<_Bits>& operator<<=(size_t pos) noexcept
    {
        const auto _Wordshift = static_cast<ptrdiff_t>(pos / _Bitsperword);
        if (_Wordshift != 0)
            for (ptrdiff_t _Wpos = _Words; 0 <= _Wpos; --_Wpos)
                _Array[_Wpos] = _Wordshift <= _Wpos ? _Array[_Wpos - _Wordshift] : 0;

        // 0 < pos < _Bitsperword, shift by bits
        if ((pos %= _Bitsperword) != 0)
        {
            for (ptrdiff_t _Wpos = _Words; 0 < _Wpos; --_Wpos)
                _Array[_Wpos] = (_Array[_Wpos] << pos) | (_Array[_Wpos - 1] >> (_Bitsperword - pos));

            _Array[0] <<= pos;
        }

        _Trim();
        return *this;
    }

    // shift right by pos, first by words then by bits
    constexpr exe_bitset<_Bits>& operator>>=(size_t pos) noexcept
    {
        const auto _Wordshift = static_cast<ptrdiff_t>(pos / _Bitsperword);
        if (_Wordshift != 0)
            for (ptrdiff_t _Wpos = 0; _Wpos <= _Words; ++_Wpos)
                _Array[_Wpos] = _Wordshift <= _Words - _Wpos ? _Array[_Wpos + _Wordshift] : 0;

        // 0 < pos < _Bitsperword, shift by bits
        if ((pos %= _Bitsperword) != 0)
        {
            for (ptrdiff_t _Wpos = 0; _Wpos < _Words; ++_Wpos)
                _Array[_Wpos] = (_Array[_Wpos] >> pos) | (_Array[_Wpos + 1] << (_Bitsperword - pos));

            _Array[_Words] >>= pos;
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
            #if __has_builtin(__builtin_memset_inline)
            __builtin_memset_inline(&_Array, 0xFF, sizeof(_Array));
            #else
            ::std::memset(&_Array, 0xFF, sizeof(_Array));
            #endif
        }
        _Trim();
        return *this;
    }

    // set bit at pos to value
    constexpr exe_bitset<_Bits>& set(size_t pos, bool value = true)
    {
        if (_Bits <= pos)
            _Throw_invalid_subscript();
        return _Set_unchecked(pos, value);
    }

    // set all bits false
    constexpr exe_bitset<_Bits>& reset() noexcept
    {
        NH3API_IF_CONSTEVAL
        {
            for ( word_type& word : _Array )
                word = 0;
        }
        else
        {
            #if __has_builtin(__builtin_memset_inline)
            __builtin_memset_inline(&_Array, 0, sizeof(_Array));
            #else
            ::std::memset(&_Array, 0, sizeof(_Array));
            #endif
        }
        return *this;
    }

    // set bit at pos to false
    constexpr exe_bitset<_Bits>& reset(size_t pos)
    { return set(pos, 0); }

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
    constexpr exe_bitset<_Bits>& flip(size_t pos)
    {
        if (_Bits <= pos)
            _Throw_invalid_subscript();

        return _Flip_unchecked(pos);
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

    [[nodiscard]] exe_string to_string(char zero = '0', char one = '1') const
    {
        exe_string result(_Bits, zero);
        char* dst = result.data();
        for (size_t i = _Bits; 0 < i; ++dst )
            *dst = _Subscript(--i) ? one : zero;
        return result;
    }

    [[nodiscard]] constexpr size_t count() const noexcept
    {
        if constexpr ( _Bits == 0 )
        {
            return 0;
        }
        else if constexpr (_Bits <= _Bitsperword)
        {
            #if defined (__BITINT_MAXWIDTH__) && __has_builtin(__builtin_popcountg)
                return __builtin_popcountg(static_cast<unsigned _BitInt(_Bits)>(_Array[0]));
            #elif __has_builtin(__builtin_popcount)
                return __builtin_popcount(_Array[0]);
            #else
                return bitpopcnt(_Array[0]);
            #endif
        }
        else
        {
            size_t result = 0;
            for (size_t i = 0; i < _Words + 1; ++i)
            #if __has_builtin(__builtin_popcount)
                result += __builtin_popcount(_Array[i]);
            #else
                result += bitpopcnt(_Array[i]);
            #endif

            return result;
        }
    }

    [[nodiscard]] constexpr size_t size() const noexcept
    { return _Bits; }

    constexpr bool operator==(const exe_bitset<_Bits>& other) const noexcept
    {
        NH3API_IF_CONSTEVAL
        {
            for (int i = _Words; 0 <= i; --i)
                if (_Array[i] != other._Array[i])
                    return false;
            return true;
        }
        else
        {
            #if __has_builtin(__builtin_memcmp)
            return __builtin_memcmp(&_Array[0], &other._Array[0], sizeof(_Array)) == 0;
            #else
            return ::std::memcmp(&_Array[0], &other._Array[0], sizeof(_Array)) == 0;
            #endif
        }
    }

    constexpr bool operator!=(const exe_bitset<_Bits>& other) const noexcept
    { return !(*this == other); }

    [[nodiscard]] bool test(size_t pos) const
    {
        if (_Bits <= pos)
            _Throw_invalid_subscript();
        return _Subscript(pos);
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

    constexpr exe_bitset<_Bits> operator<<(size_t n) const noexcept
    {
        exe_bitset<_Bits> result = *this;
        result <<= n;
        return result;
    }

    constexpr exe_bitset<_Bits> operator>>(size_t n) const noexcept
    {
        exe_bitset<_Bits> result = *this;
        result >>= n;
        return result;
    }

protected:
    // used for iterations inside the bitset.
    // no need to check for bounds.
    [[nodiscard]] constexpr bool _Subscript(size_t pos) const noexcept
    { return (_Array[pos / _Bitsperword] & (word_type{1} << pos % _Bitsperword)) != 0; }

    static constexpr ptrdiff_t _Bitsperword   = CHAR_BIT * sizeof(word_type);
    static constexpr ptrdiff_t _Words         = _Bits == 0 ? 0 : (_Bits - 1) / _Bitsperword; // NB: number of words - 1
    static constexpr bool _Need_mask          = _Bits < CHAR_BIT * sizeof(unsigned long long);
    static constexpr unsigned long long _Mask = (1ULL << (_Need_mask ? _Bits : 0)) - 1ULL;

    template<class TraitsType, class CharType> NH3API_FORCEINLINE
    void _Construct(const CharType* ptr,
                    typename std::basic_string_view<CharType, TraitsType>::size_type count,
                    const CharType zero,
                    const CharType one)
    {
        if ( count > _Bits )
        {
            for ( size_t _Idx = _Bits; _Idx < count; ++_Idx )
            {
                const auto _Ch = ptr[_Idx];
                if ( !TraitsType::eq(one, _Ch) && !TraitsType::eq(zero, _Ch) )
                    _Throw_invalid_char();
            }

            count = _Bits;
        }

        size_t _Wpos = 0;
        if ( count != 0 )
        {
            size_t    _Bits_used_in_word = 0;
            auto      _Last              = ptr + count;
            word_type _This_word         = 0;
            do
            {
                --_Last;
                const auto _Ch  = *_Last;
                _This_word     |= static_cast<word_type>(TraitsType::eq(one, _Ch)) << _Bits_used_in_word;
                if ( !TraitsType::eq(one, _Ch) && !TraitsType::eq(zero, _Ch) )
                    _Throw_invalid_char();

                if ( ++_Bits_used_in_word == _Bitsperword )
                {
                    _Array[_Wpos] = _This_word;
                    ++_Wpos;
                    _This_word         = 0;
                    _Bits_used_in_word = 0;
                }
            }
            while ( ptr != _Last );

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
    constexpr exe_bitset& _Set_unchecked(const size_t _Pos, const bool _Val) noexcept
    {
        auto& _Selected_word = _Array[_Pos / _Bitsperword];
        const auto _Bit      = word_type{1} << _Pos % _Bitsperword;
        if (_Val)
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
    { NH3API_THROW(std::invalid_argument, "bitset construction failure: invalid bitset char"); }

    [[noreturn]] NH3API_FORCEINLINE static void _Throw_ulong_conversion_overflow() noexcept(false)
    { NH3API_THROW(std::overflow_error, "bitset::to_ulong conversion overflow"); }

    [[noreturn]] NH3API_FORCEINLINE static void _Throw_ullong_conversion_overflow() noexcept(false)
    { NH3API_THROW(std::overflow_error, "bitset::to_ullong conversion overflow"); }

    [[noreturn]] NH3API_FORCEINLINE static void _Throw_invalid_subscript() noexcept(false)
    { NH3API_THROW(std::out_of_range, "invalid bitset position"); }

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
        typename StringT::value_type zero = typename StringT::value_type('0'),
        typename StringT::value_type one = typename StringT::value_type('1')) const
    {
        StringT result(_Bits, zero);
        typename StringT::value_type* dst = const_cast<typename StringT::value_type*>(result.c_str());
        for (size_t i = _Bits; 0 < i; ++dst )
            *dst = _Subscript(--i) ? one : zero;
        return result;
    }

    friend exe_bitset_to_string_helper<_Bits>;

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

#if NH3API_STD_HASH
// std::hash support for exe_bitset
template<size_t N>
struct std::hash< exe_bitset<N> >
{
    public:
        size_t operator()(const exe_bitset<N>& arg) noexcept
        { return arg._Hash_code(); }
};
#endif

template<size_t N>
struct exe_bitset_to_string_helper
{
    template<typename StringT> NH3API_FORCEINLINE
    static StringT convert(const exe_bitset<N>& arg, char zero = '0', char one = '1')
    { return arg.template _To_std_string<StringT>(zero, one); }
};

template<size_t N>
std::string to_std_string(const exe_bitset<N>& arg, char zero = '0', char one = '1')
{ return exe_bitset_to_string_helper<N>::template convert<std::string>(arg, zero, one); }

template<size_t N>
std::wstring to_std_wstring(const exe_bitset<N>& arg, wchar_t zero = L'0', wchar_t one = L'1')
{ return exe_bitset_to_string_helper<N>::template convert<std::wstring>(arg, zero, one); }

template<size_t N>
exe_string to_exe_string(const exe_bitset<N>& arg, char zero = '0', char one = '1')
{ return arg.to_string(zero, one); }
