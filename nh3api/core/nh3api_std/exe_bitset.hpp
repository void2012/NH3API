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

#include "exe_string.hpp" // exe_string, exceptions
#include "intrin.hpp" // bitpopcnt

template<size_t N>
struct exe_bitset_to_string_helper;

#if NH3API_STD_HASH 
template<size_t N>
class exe_bitset;
template<size_t N>
class std::hash< exe_bitset<N> >;
#endif

#pragma pack(push, 8)
/// @brief Visual C++ 6.0 std::bitset implementation used by heroes3.exe
/// @tparam N number of bits to store
template<size_t N>
class exe_bitset
{
protected:
    typedef uint32_t underlying_type;
    typedef exe_bitset<N> this_type NH3API_NODEBUG;

public:
    typedef bool element_type;

    /// @brief proxy class representing a reference to a bit
    class reference
    {
    friend class exe_bitset<N>;
    public:
        NH3API_CONSTEXPR reference& operator=(bool _X) NH3API_NOEXCEPT
        {
            _Pbs->set(_Off, _X);
            return (*this);
        }

        NH3API_CONSTEXPR reference& operator=(const reference& _Bs) NH3API_NOEXCEPT
        {
            _Pbs->set(_Off, bool(_Bs));
            return (*this);
        }

        NH3API_CONSTEXPR reference& flip()  NH3API_NOEXCEPT
        {
            _Pbs->flip(_Off);
            return (*this);
        }

        NH3API_CONSTEXPR bool operator~() const NH3API_NOEXCEPT
        { return (!_Pbs->test(_Off)); }

        NH3API_CONSTEXPR operator bool() const NH3API_NOEXCEPT
        { return (_Pbs->test(_Off)); }

    private:
        NH3API_CONSTEXPR reference(exe_bitset<N>& _X, size_t _P) NH3API_NOEXCEPT
            : _Pbs(&_X), _Off(_P)
            {}
        exe_bitset<N>* _Pbs;
        size_t _Off;
    };

    bool at(size_t _P) const
    {
        if (N <= _P)
            _Xran();
        return test(_P);
    }

    reference at(size_t _P)
    {
        if (N <= _P)
            _Xran();
        return reference(*this, _P);
    }

public:
    NH3API_CONSTEXPR bool operator[](size_t _P) const
    { return test(_P); }

    NH3API_CONSTEXPR reference operator[](size_t _P)
    NH3API_NOEXCEPT
    { return reference(*this, _P); }

    NH3API_CONSTEXPR exe_bitset()
    NH3API_NOEXCEPT
    { _Tidy(); }

    explicit NH3API_CONSTEXPR exe_bitset(unsigned long _X)
    NH3API_NOEXCEPT
    {
        _Tidy();
        for ( size_t i = 0; _X != 0 && i < N; _X >>= 1, ++i )
            if ( _X & 1 )
                _A[i / _Bitsperword] |= (underlying_type)1 << i % _Bitsperword;
    }

    template<class T, class CharTraits, class Allocator>
    exe_bitset(const std::basic_string<T, CharTraits, Allocator>& str,
               size_t pos = 0,
               size_t n = std::basic_string<T, CharTraits, Allocator>::npos,
               typename std::basic_string<T, CharTraits, Allocator>::value_type zero = typename std::basic_string<T, CharTraits, Allocator>::value_type('0'),
               typename std::basic_string<T, CharTraits, Allocator>::value_type one  = typename std::basic_string<T, CharTraits, Allocator>::value_type('1') )
    {
        size_t _I;
        if (str.size() < pos)
            _Xran();
        if (str.size() - pos < n)
            n = str.size() - pos;
        if (N < n)
            n = N;
        _Tidy(), pos += n;
        for (_I = 0; _I < n; ++_I)
            if (str[--pos] == one)
                set(_I);
            else if (str[pos] != zero)
                _Xinv();
    }

    template<class T, class CharTraits, class Allocator>
    exe_bitset(const exe_basic_string<T, CharTraits, Allocator>& str,
               size_t pos = 0,
               size_t n = exe_basic_string<T, CharTraits, Allocator>::npos,
               typename exe_basic_string<T, CharTraits, Allocator>::value_type zero = typename exe_basic_string<T, CharTraits, Allocator>::value_type('0'),
               typename exe_basic_string<T, CharTraits, Allocator>::value_type one  = typename exe_basic_string<T, CharTraits, Allocator>::value_type('1') )
    {
        size_t _I;
        if (str.size() < pos)
            _Xran();
        if (str.size() - pos < n)
            n = str.size() - pos;
        if (N < n)
            n = N;
        _Tidy(), pos += n;
        for (_I = 0; _I < n; ++_I)
            if (str[--pos] == one)
                set(_I);
            else if (str[pos] != zero)
                _Xinv();
    }

    NH3API_FORCEINLINE
    exe_bitset(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
    { NH3API_IGNORE(_A); }

    NH3API_CONSTEXPR exe_bitset<N>& operator&=(const exe_bitset<N>& _R) NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] &= _R._W(_I);
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<N>& operator|=(const exe_bitset<N>& _R) NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] |= _R._W(_I);
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<N>& operator^=(const exe_bitset<N>& _R) NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] ^= _R._W(_I);
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<N>& operator<<=(size_t _P) NH3API_NOEXCEPT
    {
        const size_t _D = _P / _Bitsperword;
        if (_D != 0)
            for (int _I = _Words; 0 <= _I; --_I)
                _A[_I] = _D <= _I ? _A[_I - _D] : 0;
        if ((_P %= _Bitsperword) != 0)
        {
            for (int _I = _Words; 0 < _I; --_I)
                _A[_I] = (_A[_I] << _P) | (_A[_I - 1] >> (_Bitsperword - _P));
            _A[0] <<= _P, _Trim();
        }
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<N>& operator>>=(size_t _P) NH3API_NOEXCEPT
    {
        const size_t _D = _P / _Bitsperword;
        if (_D != 0)
            for (size_t _I = 0; _I <= _Words; ++_I)
                _A[_I] = _D <= _Words - _I ? _A[_I + _D] : 0;
        if ((_P %= _Bitsperword) != 0)
            {for (size_t _I = 0; _I < _Words; ++_I)
                _A[_I] = (_A[_I] >> _P)
                    | (_A[_I + 1] << (_Bitsperword - _P));
            _A[_Words] >>= _P; }
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<N>& set() NH3API_NOEXCEPT
    {
        _Tidy(~(underlying_type)0);
        return (*this);
    }

    exe_bitset<N>& set(size_t _P, bool _X = true)
    {
        if (N <= _P)
            _Xran();
        if (_X)
            _A[_P / _Bitsperword] |= (underlying_type)1 << _P % _Bitsperword;
        else
            _A[_P / _Bitsperword] &= ~((underlying_type)1 << _P % _Bitsperword);
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<N>& reset() NH3API_NOEXCEPT
    {
        _Tidy();
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<N>& reset(size_t _P)
    { return set(_P, 0); }

    NH3API_CONSTEXPR exe_bitset<N> operator~() const NH3API_NOEXCEPT
    { return exe_bitset<N>(*this).flip(); }

    NH3API_CONSTEXPR exe_bitset<N>& flip() NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] = ~_A[_I];
        _Trim();
        return *this;
    }

    exe_bitset<N>& flip(size_t _P)
    {
        if (N <= _P)
            _Xran();
        _A[_P / _Bitsperword] ^= (underlying_type)1 << _P % _Bitsperword;
        return (*this);
    }

    unsigned long to_ulong() const
    {
        NH3API_STATIC_ASSERT("", (sizeof (unsigned long) % sizeof (underlying_type) == 0));
        int _I = _Words;
        for (; sizeof (unsigned long) / sizeof (underlying_type) <= _I; --_I)
            if (_A[_I] != 0)
                _Xoflo();
        unsigned long _V = _A[_I];
        for (; 0 <= --_I; )
            _V = ((_V << (_Bitsperword - 1)) << 1) | _A[_I];
        return (_V);
    }

    unsigned long long to_ullong() const
    {
        NH3API_STATIC_ASSERT("", (sizeof (unsigned long long) % sizeof (underlying_type) == 0));
        int _I = _Words;
        for (; sizeof (unsigned long long) / sizeof (underlying_type) <= _I; --_I)
            if (_A[_I] != 0)
                _Xoflo();
        unsigned long long _V = _A[_I];
        for (; 0 <= --_I; )
            _V = ((_V << (_Bitsperword - 1)) << 1) | _A[_I];
        return (_V);
    }

    #if NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS
    template<typename CharT = char, typename CharTraits = std::char_traits<CharT>, typename Allocator = exe_allocator<CharT>> 
    exe_basic_string<CharT, CharTraits, Allocator> 
    to_string(CharT zero = CharT('0'), CharT one = CharT('1')) const
    { return _To_exe_string<CharT, CharTraits, Allocator>(); }

    #else
    template<typename CharT, typename CharTraits, typename Allocator> 
    exe_basic_string<CharT, CharTraits, Allocator> 
    to_string(CharT zero = CharT('0'), CharT one = CharT('1')) const
    { return _To_exe_string<CharT, CharTraits, Allocator>(); }

    template<typename CharT, typename CharTraits> 
    exe_basic_string<CharT, CharTraits> 
    to_string(CharT zero = CharT('0'), CharT one = CharT('1')) const
    { return _To_exe_string<CharT, CharTraits>(); }

    template<typename CharT> 
    exe_basic_string<CharT> 
    to_string(CharT zero = CharT('0'), CharT one = CharT('1')) const
    { return _To_exe_string<CharT>(); }

    exe_string to_string(char zero = '0', char one = '1') const
    { return _To_exe_string<char>(zero, one); }
    #endif

    /*
    NH3API_CONSTEXPR size_t count() const NH3API_NOEXCEPT
    {
        size_t _V = 0;
        for (int _I = _Words; 0 <= _I; --_I)
            for (underlying_type _X = _A[_I]; _X != 0; _X >>= 4)
                _V += "\0\1\1\2\1\2\2\3"
                    "\1\2\2\3\2\3\3\4"[_X & 0xF];
        return (_V);
    }
    */

    NH3API_CONSTEXPR size_t count() const NH3API_NOEXCEPT
    {
        size_t _V = 0;
        for (size_t _I = 0; _I < _Words + 1; ++_I)
            _V += bitpopcnt(_A[_I]);

        return _V;
    }

    NH3API_CONSTEXPR size_t size() const NH3API_NOEXCEPT
    { return N; }

    NH3API_CONSTEXPR bool operator==(const exe_bitset<N>& _R) const NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            if (_A[_I] != _R._W(_I))
                return false;
        return true;
    }

    NH3API_CONSTEXPR bool operator!=(const exe_bitset<N>& _R) const NH3API_NOEXCEPT
    { return (!(*this == _R)); }

    bool test(size_t _P) const
    {
        if (N <= _P)
            _Xran();
        return ((_A[_P / _Bitsperword] & ((underlying_type)1 << _P % _Bitsperword)) != 0);
    }

    NH3API_CONSTEXPR bool any() const NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            if (_A[_I] != 0)
                return true;
        return false;
    }

    NH3API_CONSTEXPR bool none() const NH3API_NOEXCEPT
    { return !any(); }

    NH3API_CONSTEXPR exe_bitset<N> operator<<(size_t _R) const NH3API_NOEXCEPT
    { return exe_bitset<N>(*this) <<= _R; }

    NH3API_CONSTEXPR exe_bitset<N> operator>>(size_t _R) const NH3API_NOEXCEPT
    { return exe_bitset<N>(*this) >>= _R; }

    NH3API_CONSTEXPR friend exe_bitset<N> operator&(const exe_bitset<N>& _L,
        const exe_bitset<N>& _R) NH3API_NOEXCEPT
    { return exe_bitset<N>(_L) &= _R; }

    NH3API_CONSTEXPR friend exe_bitset<N> operator|(const exe_bitset<N>& _L,
        const exe_bitset<N>& _R) NH3API_NOEXCEPT
    { return exe_bitset<N>(_L) |= _R; }

    NH3API_CONSTEXPR friend exe_bitset<N> operator^(const exe_bitset<N>& _L,
        const exe_bitset<N>& _R) NH3API_NOEXCEPT
    { return exe_bitset<N>(_L) ^= _R; }

protected:
    // array position
    NH3API_CONSTEXPR underlying_type _W(size_t _I) const NH3API_NOEXCEPT
    { return (_A[_I]); }

    // used for iterations inside the bitset.
    // no need to check for bounds.
    NH3API_CONSTEXPR bool _At(size_t _P) const NH3API_NOEXCEPT
    { return ((_A[_P / _Bitsperword] & ((underlying_type)1 << _P % _Bitsperword)) != 0); }

    enum : unsigned int
    {
        _Bitsperword = CHAR_BIT * sizeof (underlying_type),
        _Words = N == 0 ? 0 : (N - 1) / _Bitsperword
    };

    NH3API_CONSTEXPR void _Tidy(underlying_type _X = 0) NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] = _X;
        if (_X != 0)
            _Trim();
    }

    NH3API_CONSTEXPR void _Trim() NH3API_NOEXCEPT
    {
        if (N % _Bitsperword != 0)
            _A[_Words] &= ((underlying_type)1 << N % _Bitsperword) - 1;
    }

    NH3API_FORCEINLINE static void _Xinv() 
    { NH3API_THROW(std::invalid_argument, "invalid bitset char"); }

    NH3API_FORCEINLINE static void _Xoflo()
    { NH3API_THROW(std::overflow_error, "bitset conversion overflow"); }

    NH3API_FORCEINLINE static void _Xran()
    { NH3API_THROW(std::out_of_range, "invalid bitset position"); }

    #if NH3API_STD_HASH
    NH3API_CONSTEXPR size_t _Hash_code() NH3API_NOEXCEPT 
    { 
        nh3api::default_hash hasher;
        hasher.update(&_A[0], _Words + 1);
        return hasher.digest();
    }

    // allow access to _Hash_code() for std::hash
    friend std::hash<this_type>;
    #endif

    template<class CharT
    #if NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS
    , class CharTraits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>
    #endif
    > NH3API_FORCEINLINE
    std::basic_string<CharT,
    #if NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS
    CharTraits, Allocator
    #else 
    std::char_traits<CharT>, std::allocator<CharT>
    #endif
    > _To_std_string(CharT zero = CharT('0'), CharT one = CharT('1')) const
    {
        std::basic_string<CharT,
        #if NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS
        CharTraits, Allocator
        #else 
        std::char_traits<CharT>, std::allocator<CharT>
        #endif
        >
        result(N, zero);
        CharT* dst = const_cast<CharT*>(result.c_str());
        for (size_t i = N; 0 < i; ++dst )
            *dst = _At(--i) ? one : zero;
        return result;
    }

    template<class CharT
    #if NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS
    , class CharTraits = std::char_traits<CharT>, class Allocator = exe_allocator<CharT>
    #endif
    > NH3API_FORCEINLINE
    exe_basic_string<CharT,
    #if NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS
    CharTraits, Allocator
    #else 
    std::char_traits<CharT>, exe_allocator<CharT>
    #endif
    > _To_exe_string(CharT zero = CharT('0'), CharT one = CharT('1')) const
    {
        exe_basic_string<CharT,
        #if NH3API_STD_DEFAULT_TEMPLATE_ARGUMENTS
        CharTraits, Allocator
        #else 
        std::char_traits<CharT>, exe_allocator<CharT>
        #endif
        >
        result(N, zero);
        // use const_char and c_str explicitly to avoid invoking exe_string::_Freeze
        // as an optimization
        CharT* dst = const_cast<CharT*>(result.c_str());
        for (size_t i = N; 0 < i; ++dst )
            *dst = _At(--i) ? one : zero;
        return result;
    }
    friend exe_bitset_to_string_helper<N>;

    // bit array
    underlying_type _A[_Words + 1];
};
#pragma pack(pop)

#if NH3API_STD_HASH
// std::hash support for exe_bitset
template<size_t N>
class std::hash< exe_bitset<N> >
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
    static StringT to_std_basic_string(const exe_bitset<N>& arg, char zero = '0', char one = '1')
    { return arg._To_std_string(zero, one); }

    template<typename StringT> NH3API_FORCEINLINE
    static StringT to_exe_basic_string(const exe_bitset<N>& arg, char zero = '0', char one = '1')
    { return arg._To_exe_string(zero, one); }
};

template<size_t N>
std::string to_std_string(const exe_bitset<N>& arg, char zero = '0', char one = '1')
{ return exe_bitset_to_string_helper<N>::template to_std_basic_string<std::string>(arg, zero, one); }

template<size_t N>
std::wstring to_std_wstring(const exe_bitset<N>& arg, wchar_t zero = L'0', wchar_t one = L'1')
{ return exe_bitset_to_string_helper<N>::template to_std_basic_string<std::wstring>(arg, zero, one); }

template<size_t N>
exe_string to_exe_string(const exe_bitset<N>& arg, char zero = '0', char one = '1')
{ return exe_bitset_to_string_helper<N>::template to_exe_basic_string<exe_string>(arg, zero, one); }

template<size_t N>
exe_wstring to_exe_wstring(const exe_bitset<N>& arg, wchar_t zero = L'0', wchar_t one = L'1')
{ return exe_bitset_to_string_helper<N>::template to_exe_basic_string<exe_wstring>(arg, zero, one); }
