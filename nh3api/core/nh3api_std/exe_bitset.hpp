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

#pragma pack(push, 8)
/// @brief Visual C++ 6.0 std::bitset implementation used by heroes3.exe
/// @tparam _N number of bits to store
template<size_t _N>
class exe_bitset
{
protected:
    typedef uint32_t _Ty;
    typedef exe_bitset<_N> this_type NH3API_NODEBUG;

public:
    typedef bool element_type;

    /// @brief proxy class representing a reference to a bit
    class reference
    {
    friend class exe_bitset<_N>;
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
        NH3API_CONSTEXPR reference(exe_bitset<_N>& _X, size_t _P) NH3API_NOEXCEPT
            : _Pbs(&_X), _Off(_P)
            {}
        exe_bitset<_N>* _Pbs;
        size_t _Off;
    };

    enum { bitset_size = _N };
    bool at(size_t _P) const
    {
        if (_N <= _P)
            _Xran();
        return test(_P);
    }

    reference at(size_t _P)
    {
        if (_N <= _P)
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
        for ( size_t i = 0; _X != 0 && i < _N; _X >>= 1, ++i )
            if ( _X & 1 )
                _A[i / _Bitsperword] |= (_Ty)1 << i % _Bitsperword;
    }

    template<typename _StringType>
    exe_bitset(const _StringType& _S,
               size_t _P = 0,
               size_t _L = _StringType::npos,
               typename _StringType::value_type _zero = typename _StringType::value_type('0'),
               typename _StringType::value_type _one = typename _StringType::value_type('1') )
    {
        size_t _I;
        if (_S.size() < _P)
            _Xran();
        if (_S.size() - _P < _L)
            _L = _S.size() - _P;
        if (_N < _L)
            _L = _N;
        _Tidy(), _P += _L;
        for (_I = 0; _I < _L; ++_I)
            if (_S[--_P] == _one)
                set(_I);
            else if (_S[_P] != _zero)
                _Xinv();
    }

    NH3API_FORCEINLINE
    exe_bitset(const nh3api::dummy_tag_t& tag) NH3API_NOEXCEPT
    { NH3API_IGNORE(_A); }

    NH3API_CONSTEXPR exe_bitset<_N>& operator&=(const exe_bitset<_N>& _R) NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] &= _R._W(_I);
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<_N>& operator|=(const exe_bitset<_N>& _R) NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] |= _R._W(_I);
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<_N>& operator^=(const exe_bitset<_N>& _R) NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] ^= _R._W(_I);
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<_N>& operator<<=(size_t _P) NH3API_NOEXCEPT
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

    NH3API_CONSTEXPR exe_bitset<_N>& operator>>=(size_t _P) NH3API_NOEXCEPT
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

    NH3API_CONSTEXPR exe_bitset<_N>& set() NH3API_NOEXCEPT
    {
        _Tidy(~(_Ty)0);
        return (*this);
    }

    exe_bitset<_N>& set(size_t _P, bool _X = true)
    {
        if (_N <= _P)
            _Xran();
        if (_X)
            _A[_P / _Bitsperword] |= (_Ty)1 << _P % _Bitsperword;
        else
            _A[_P / _Bitsperword] &= ~((_Ty)1 << _P % _Bitsperword);
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<_N>& reset() NH3API_NOEXCEPT
    {
        _Tidy();
        return *this;
    }

    NH3API_CONSTEXPR exe_bitset<_N>& reset(size_t _P)
    { return set(_P, 0); }

    NH3API_CONSTEXPR exe_bitset<_N> operator~() const NH3API_NOEXCEPT
    { return exe_bitset<_N>(*this).flip(); }

    NH3API_CONSTEXPR exe_bitset<_N>& flip() NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] = ~_A[_I];
        _Trim();
        return *this;
    }

    exe_bitset<_N>& flip(size_t _P)
    {
        if (_N <= _P)
            _Xran();
        _A[_P / _Bitsperword] ^= (_Ty)1 << _P % _Bitsperword;
        return (*this);
    }

    unsigned long to_ulong() const
    {
        NH3API_STATIC_ASSERT("", (sizeof (unsigned long) % sizeof (_Ty) == 0));
        int _I = _Words;
        for (; sizeof (unsigned long) / sizeof (_Ty) <= _I; --_I)
            if (_A[_I] != 0)
                _Xoflo();
        unsigned long _V = _A[_I];
        for (; 0 <= --_I; )
            _V = ((_V << (_Bitsperword - 1)) << 1) | _A[_I];
        return (_V);
    }

    unsigned long long to_ullong() const
    {
        NH3API_STATIC_ASSERT("", (sizeof (unsigned long long) % sizeof (_Ty) == 0));
        int _I = _Words;
        for (; sizeof (unsigned long long) / sizeof (_Ty) <= _I; --_I)
            if (_A[_I] != 0)
                _Xoflo();
        unsigned long long _V = _A[_I];
        for (; 0 <= --_I; )
            _V = ((_V << (_Bitsperword - 1)) << 1) | _A[_I];
        return (_V);
    }

    exe_string to_string(char _zero = '0', char _one = '1') const
    {
        exe_string _S(_N, _zero);
        // use const_char and c_str explicitly to avoid invoking exe_string::_Freeze
        // as an optimization
        char* _Data = const_cast<char*>(_S.c_str());
        for (size_t i = _N; 0 < i; ++_Data )
            *_Data = _At(--i) ? _one : _zero;
        return (_S);
    }

    /*
    NH3API_CONSTEXPR size_t count() const NH3API_NOEXCEPT
    {
        size_t _V = 0;
        for (int _I = _Words; 0 <= _I; --_I)
            for (_Ty _X = _A[_I]; _X != 0; _X >>= 4)
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
    { return _N; }

    NH3API_CONSTEXPR bool operator==(const exe_bitset<_N>& _R) const NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            if (_A[_I] != _R._W(_I))
                return false;
        return true;
    }

    NH3API_CONSTEXPR bool operator!=(const exe_bitset<_N>& _R) const NH3API_NOEXCEPT
    { return (!(*this == _R)); }

    bool test(size_t _P) const
    {
        if (_N <= _P)
            _Xran();
        return ((_A[_P / _Bitsperword] & ((_Ty)1 << _P % _Bitsperword)) != 0);
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

    NH3API_CONSTEXPR exe_bitset<_N> operator<<(size_t _R) const NH3API_NOEXCEPT
    { return exe_bitset<_N>(*this) <<= _R; }

    NH3API_CONSTEXPR exe_bitset<_N> operator>>(size_t _R) const NH3API_NOEXCEPT
    { return exe_bitset<_N>(*this) >>= _R; }

    NH3API_CONSTEXPR friend exe_bitset<_N> operator&(const exe_bitset<_N>& _L,
        const exe_bitset<_N>& _R) NH3API_NOEXCEPT
    { return exe_bitset<_N>(_L) &= _R; }

    NH3API_CONSTEXPR friend exe_bitset<_N> operator|(const exe_bitset<_N>& _L,
        const exe_bitset<_N>& _R) NH3API_NOEXCEPT
    { return exe_bitset<_N>(_L) |= _R; }

    NH3API_CONSTEXPR friend exe_bitset<_N> operator^(const exe_bitset<_N>& _L,
        const exe_bitset<_N>& _R) NH3API_NOEXCEPT
    { return exe_bitset<_N>(_L) ^= _R; }

protected:
    // array position
    NH3API_CONSTEXPR _Ty _W(size_t _I) const NH3API_NOEXCEPT
    { return (_A[_I]); }

    // used for iterations inside the bitset.
    // no need to check for bounds.
    NH3API_CONSTEXPR bool _At(size_t _P) const NH3API_NOEXCEPT
    { return ((_A[_P / _Bitsperword] & ((_Ty)1 << _P % _Bitsperword)) != 0); }

    enum : unsigned int
    {
        _Bitsperword = CHAR_BIT * sizeof (_Ty),
        _Words = _N == 0 ? 0 : (_N - 1) / _Bitsperword
    };

    NH3API_CONSTEXPR void _Tidy(_Ty _X = 0) NH3API_NOEXCEPT
    {
        for (int _I = _Words; 0 <= _I; --_I)
            _A[_I] = _X;
        if (_X != 0)
            _Trim();
    }

    NH3API_CONSTEXPR void _Trim() NH3API_NOEXCEPT
    {
        if (_N % _Bitsperword != 0)
            _A[_Words] &= ((_Ty)1 << _N % _Bitsperword) - 1;
    }

    NH3API_FORCEINLINE static void _Xinv() 
    { NH3API_THROW(std::invalid_argument, "invalid bitset char"); }

    NH3API_FORCEINLINE static void _Xoflo()
    { NH3API_THROW(std::overflow_error, "bitset conversion overflow"); }

    NH3API_FORCEINLINE static void _Xran()
    { NH3API_THROW(std::out_of_range, "invalid bitset position"); }

    size_t _Hash_code() NH3API_NOEXCEPT 
    { 
        nh3api::default_hash hasher;
        hasher.update(_A, _Words + 1);
        return hasher.digest();
    }

    // allow access to _Hash_code() for std::hash
    friend std::hash<this_type>;

    // bit array
    _Ty _A[_Words + 1];
};
#pragma pack(pop)

// std::hash support for exe_bitset
template<size_t N>
class std::hash< exe_bitset<N> >
{
    public:
        size_t operator()(const exe_bitset<N>& arg) NH3API_NOEXCEPT
        {
            return arg._Hash_code();
        }
};