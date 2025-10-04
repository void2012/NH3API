//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright void_17, 2024-2025
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

#if NH3API_STD_HASH 
template<size_t N>
class exe_bitset;
template<size_t N>
struct std::hash< exe_bitset<N> >;
#endif

#pragma pack(push, 8)
/// @brief Visual C++ 6.0 std::bitset implementation used by heroes3.exe
/// @tparam N number of bits to store
template<size_t N>
class exe_bitset
{
protected:
    typedef uint32_t word_type;

public:
    typedef bool element_type;

    /// @brief proxy class representing a reference to a bit
    class reference
    {
    friend class exe_bitset<N>;
    public:
        NH3API_CONSTEXPR_CPP_14 reference& operator=(bool value) NH3API_NOEXCEPT
        {
            _parent.set(_pos, value);
            return *this;
        }

        NH3API_CONSTEXPR_CPP_14 reference& operator=(const reference& other) NH3API_NOEXCEPT
        {
            _parent.set(_pos, static_cast<bool>(other));
            return *this;
        }

        NH3API_CONSTEXPR_CPP_14 reference& flip()  NH3API_NOEXCEPT
        {
            _parent.flip(_pos);
            return *this;
        }

        NH3API_CONSTEXPR_CPP_14 bool operator~() const NH3API_NOEXCEPT
        { return !_parent.test(_pos); }

        NH3API_CONSTEXPR_CPP_14 operator bool() const NH3API_NOEXCEPT
        { return _parent.test(_pos); }

        #if NH3API_CHECK_CPP11
        ~reference() = default;
        #endif

        NH3API_CONSTEXPR_CPP_14 reference() NH3API_NOEXCEPT NH3API_DELETED_FUNCTION

    private:
        NH3API_CONSTEXPR_CPP_14 reference(exe_bitset<N>& value, size_t pos) NH3API_NOEXCEPT
            : _parent(value), _pos(pos)
            {}

        exe_bitset<N>& _parent;
        size_t _pos;
    };

    bool at(size_t pos) const
    {
        if (N <= pos)
            _Throw_invalid_subscript();
        return test(pos);
    }

    reference at(size_t pos)
    {
        if (N <= pos)
            _Throw_invalid_subscript();
        return reference(*this, pos);
    }

public:
    NH3API_CONSTEXPR_CPP_14 bool operator[](size_t pos) const
    { return test(pos); }

    NH3API_CONSTEXPR_CPP_14 reference operator[](size_t pos)
    NH3API_NOEXCEPT
    { return reference(*this, pos); }

    NH3API_CONSTEXPR_CPP_14 exe_bitset()
    NH3API_NOEXCEPT
    { _Tidy(); }

    explicit NH3API_CONSTEXPR_CPP_14 exe_bitset(unsigned long value)
    NH3API_NOEXCEPT
    {
        _Tidy();
        for ( size_t i = 0; value != 0 && i < N; value >>= 1, ++i )
            if ( value & 1 )
                _array[i / _Bitsperword] |= (word_type)1 << i % _Bitsperword;
    }

    template<class T, class CharTraits, class Allocator>
    exe_bitset(const std::basic_string<T, CharTraits, Allocator>& str,
               size_t pos = 0,
               size_t n = std::basic_string<T, CharTraits, Allocator>::npos,
               typename std::basic_string<T, CharTraits, Allocator>::value_type zero = typename std::basic_string<T, CharTraits, Allocator>::value_type('0'),
               typename std::basic_string<T, CharTraits, Allocator>::value_type one  = typename std::basic_string<T, CharTraits, Allocator>::value_type('1') )
    {
        if (str.size() < pos)
            _Throw_invalid_subscript();
        if (str.size() - pos < n)
            n = str.size() - pos;
        if (N < n)
            n = N;
        _Tidy();
        pos += n;
        for (size_t i = 0; i < n; ++i)
            if (str[--pos] == one)
                set(i);
            else if (str[pos] != zero)
                _Throw_invalid_char();
    }

    exe_bitset(const exe_string& str,
               size_t pos = 0,
               size_t n = exe_string::npos,
               typename exe_string::value_type zero = '0',
               typename exe_string::value_type one  = '1' )
    {
        if (str.size() < pos)
            _Throw_invalid_subscript();
        if (str.size() - pos < n)
            n = str.size() - pos;
        if (N < n)
            n = N;
        _Tidy();
        pos += n;
        for (size_t i = 0; i < n; ++i)
            if (str[--pos] == one)
                set(i);
            else if (str[pos] != zero)
                _Throw_invalid_char();
    }

    NH3API_FORCEINLINE
    exe_bitset(const ::nh3api::dummy_tag_t&) NH3API_NOEXCEPT
    {}

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N>& operator&=(const exe_bitset<N>& other) NH3API_NOEXCEPT
    {
        for (int i = _Words; 0 <= i; --i)
            _array[i] &= other._array[i];
        return *this;
    }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N>& operator|=(const exe_bitset<N>& other) NH3API_NOEXCEPT
    {
        for (int i = _Words; 0 <= i; --i)
            _array[i] |= other._array[i];
        return *this;
    }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N>& operator^=(const exe_bitset<N>& other) NH3API_NOEXCEPT
    {
        for (int i = _Words; 0 <= i; --i)
            _array[i] ^= other._array[i];
        return *this;
    }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N>& operator<<=(size_t pos) NH3API_NOEXCEPT
    {
        const size_t _D = pos / _Bitsperword;
        if (_D != 0)
            for (int i = _Words; 0 <= i; --i)
                _array[i] = _D <= i ? _array[i - _D] : 0;
        if ((pos %= _Bitsperword) != 0)
        {
            for (int i = _Words; 0 < i; --i)
                _array[i] = (_array[i] << pos) | (_array[i - 1] >> (_Bitsperword - pos));
            _array[0] <<= pos;
            _Trim();
        }
        return *this;
    }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N>& operator>>=(size_t pos) NH3API_NOEXCEPT
    {
        const size_t _D = pos / _Bitsperword;
        if (_D != 0)
            for (size_t i = 0; i <= _Words; ++i)
                _array[i] = _D <= _Words - i ? _array[i + _D] : 0;
        if ((pos %= _Bitsperword) != 0)
            {for (size_t i = 0; i < _Words; ++i)
                _array[i] = (_array[i] >> pos)
                    | (_array[i + 1] << (_Bitsperword - pos));
            _array[_Words] >>= pos; }
        return *this;
    }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N>& set() NH3API_NOEXCEPT
    {
        _Tidy(~(word_type)0);
        return (*this);
    }

    exe_bitset<N>& set(size_t pos, bool value = true)
    {
        if (N <= pos)
            _Throw_invalid_subscript();
        if (value)
            _array[pos / _Bitsperword] |= (word_type)1 << pos % _Bitsperword;
        else
            _array[pos / _Bitsperword] &= ~((word_type)1 << pos % _Bitsperword);
        return *this;
    }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N>& reset() NH3API_NOEXCEPT
    {
        _Tidy();
        return *this;
    }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N>& reset(size_t pos)
    { return set(pos, 0); }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N> operator~() const NH3API_NOEXCEPT
    { return exe_bitset<N>(*this).flip(); }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N>& flip() NH3API_NOEXCEPT
    {
        for (int i = _Words; 0 <= i; --i)
            _array[i] = ~_array[i];
        _Trim();
        return *this;
    }

    exe_bitset<N>& flip(size_t pos)
    {
        if (N <= pos)
            _Throw_invalid_subscript();
        _array[pos / _Bitsperword] ^= (word_type)1 << pos % _Bitsperword;
        return (*this);
    }

    unsigned long to_ulong() const
    {
        NH3API_STATIC_ASSERT("", (sizeof (unsigned long) % sizeof (word_type) == 0));
        int i = _Words;
        for (; sizeof (unsigned long) / sizeof (word_type) <= i; --i)
            if (_array[i] != 0)
                _Throw_ulong_conversion_overflow();
        unsigned long result = _array[i];
        for (; 0 <= --i; )
            result = ((result << (_Bitsperword - 1)) << 1) | _array[i];
        return (result);
    }

    unsigned long long to_ullong() const
    {
        NH3API_STATIC_ASSERT("", (sizeof (unsigned long long) % sizeof (word_type) == 0));
        int i = _Words;
        for (; sizeof (unsigned long long) / sizeof (word_type) <= i; --i)
            if (_array[i] != 0)
                _Throw_ullong_conversion_overflow();
        unsigned long long result = _array[i];
        for (; 0 <= --i; )
            result = ((result << (_Bitsperword - 1)) << 1) | _array[i];
        return (result);
    }

    exe_string to_string(char zero = '0', char one = '1') const
    {
        exe_string result(N, zero);
        char* dst = result.data();
        for (size_t i = N; 0 < i; ++dst )
            *dst = _At(--i) ? one : zero;
        return result;
    }

    /*
    NH3API_CONSTEXPR size_t count() const NH3API_NOEXCEPT
    {
        size_t result = 0;
        for (int i = _Words; 0 <= i; --i)
            for (word_type value = _array[i]; value != 0; value >>= 4)
                result += "\0\1\1\2\1\2\2\3"
                    "\1\2\2\3\2\3\3\4"[value & 0xF];
        return (result);
    }
    */

    NH3API_CONSTEXPR_CPP_14 size_t count() const NH3API_NOEXCEPT
    {
        size_t result = 0;
        for (size_t i = 0; i < _Words + 1; ++i)
            result += bitpopcnt(_array[i]);

        return result;
    }

    NH3API_CONSTEXPR_CPP_14 size_t size() const NH3API_NOEXCEPT
    { return N; }

    NH3API_CONSTEXPR_CPP_14 bool operator==(const exe_bitset<N>& other) const NH3API_NOEXCEPT
    {
        for (int i = _Words; 0 <= i; --i)
            if (_array[i] != other._array[i])
                return false;
        return true;
    }

    NH3API_CONSTEXPR_CPP_14 bool operator!=(const exe_bitset<N>& other) const NH3API_NOEXCEPT
    { return (!(*this == other)); }

    bool test(size_t pos) const
    {
        if (N <= pos)
            _Throw_invalid_subscript();
        return ((_array[pos / _Bitsperword] & ((word_type)1 << pos % _Bitsperword)) != 0);
    }

    NH3API_CONSTEXPR_CPP_14 bool any() const NH3API_NOEXCEPT
    {
        for (int i = _Words; 0 <= i; --i)
            if (_array[i] != 0)
                return true;
        return false;
    }

    NH3API_CONSTEXPR_CPP_14 bool none() const NH3API_NOEXCEPT
    { return !any(); }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N> operator<<(size_t other) const NH3API_NOEXCEPT
    { return exe_bitset<N>(*this) <<= other; }

    NH3API_CONSTEXPR_CPP_14 exe_bitset<N> operator>>(size_t other) const NH3API_NOEXCEPT
    { return exe_bitset<N>(*this) >>= other; }

    NH3API_CONSTEXPR_CPP_14 friend exe_bitset<N> operator&(const exe_bitset<N>& lhs,
        const exe_bitset<N>& other) NH3API_NOEXCEPT
    { return exe_bitset<N>(lhs) &= other; }

    NH3API_CONSTEXPR_CPP_14 friend exe_bitset<N> operator|(const exe_bitset<N>& lhs,
        const exe_bitset<N>& other) NH3API_NOEXCEPT
    { return exe_bitset<N>(lhs) |= other; }

    NH3API_CONSTEXPR_CPP_14 friend exe_bitset<N> operator^(const exe_bitset<N>& lhs,
        const exe_bitset<N>& other) NH3API_NOEXCEPT
    { return exe_bitset<N>(lhs) ^= other; }

protected:
    // used for iterations inside the bitset.
    // no need to check for bounds.
    NH3API_CONSTEXPR_CPP_14 bool _At(size_t pos) const NH3API_NOEXCEPT
    { return ((_array[pos / _Bitsperword] & ((word_type)1 << pos % _Bitsperword)) != 0); }

    enum : uint32_t
    {
        _Bitsperword = CHAR_BIT * sizeof (word_type),
        _Words = N == 0 ? 0 : (N - 1) / _Bitsperword
    };

    NH3API_CONSTEXPR_CPP_14 void _Tidy(word_type value = 0) NH3API_NOEXCEPT
    {
        for (int i = _Words; 0 <= i; --i)
            _array[i] = value;
        if (value != 0)
            _Trim();
    }

    NH3API_CONSTEXPR_CPP_14 void _Trim() NH3API_NOEXCEPT
    {
        NH3API_IF_CONSTEXPR (N % _Bitsperword != 0)
            _array[_Words] &= ((word_type)1 << N % _Bitsperword) - 1;
    }

    NH3API_FORCEINLINE static void _Throw_invalid_char() 
    { NH3API_THROW(std::invalid_argument, "bitset construction failure: invalid bitset char"); }

    NH3API_FORCEINLINE static void _Throw_ulong_conversion_overflow()
    { NH3API_THROW(std::overflow_error, "bitset::to_ulong conversion overflow"); }

    NH3API_FORCEINLINE static void _Throw_ullong_conversion_overflow()
    { NH3API_THROW(std::overflow_error, "bitset::to_ullong conversion overflow"); }

    NH3API_FORCEINLINE static void _Throw_invalid_subscript()
    { NH3API_THROW(std::out_of_range, "invalid bitset position"); }

    #if NH3API_STD_HASH
    NH3API_CONSTEXPR_CPP_14 size_t _Hash_code() NH3API_NOEXCEPT 
    { 
        nh3api::default_hash hasher;
        hasher.update(&_array[0], _Words + 1);
        return hasher.digest();
    }

    // allow access to _Hash_code() for std::hash
    friend std::hash< exe_bitset<N> >;
    #endif

    template<typename StringT> NH3API_FORCEINLINE
    StringT _To_std_string(
        typename StringT::value_type zero = typename StringT::value_type('0'), 
        typename StringT::value_type one = typename StringT::value_type('1')) const
    {
        StringT result(N, zero);
        typename StringT::value_type* dst = const_cast<typename StringT::value_type*>(result.c_str());
        for (size_t i = N; 0 < i; ++dst )
            *dst = _At(--i) ? one : zero;
        return result;
    }

    friend exe_bitset_to_string_helper<N>;

    // bit array
    word_type _array[_Words + 1];
};
#pragma pack(pop)

#if NH3API_STD_HASH
// std::hash support for exe_bitset
template<size_t N>
struct std::hash< exe_bitset<N> >
{
    public:
        size_t operator()(const exe_bitset<N>& arg) NH3API_NOEXCEPT
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
