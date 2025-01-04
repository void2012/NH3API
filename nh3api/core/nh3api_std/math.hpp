//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//

// fdtest and ddtest implementation from MSVC STL
// https://github.com/microsoft/STL
// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include "nh3api_std.hpp"
#include <cmath>
#include <stdexcept>

namespace nh3api
{

// integer math

#if !NH3API_CHECK_MSVC
// integer logarithm of base 2
NH3API_CONSTEXPR inline uint32_t ilog2(uint32_t x)
{ return __builtin_clz(x) ^ 31; }
#else
#include <intrin.h>

// integer logarithm of base 2
inline uint32_t ilog2(uint32_t x)
{
unsigned long result;
_BitScanReverse(&result, x);
return result;
}
#endif
// count the amount of digits in an integer
NH3API_FORCEINLINE
int32_t count_digits(uint32_t x)
{
    static uint32_t digits_table[] = {9, 99, 999, 9999, 99999,
    999999, 9999999, 99999999, 999999999};
    int32_t y = (9 * ilog2(x)) >> 5;
    y += x > digits_table[y];
    return y + 1;
}

// floating-point math

// older versions of MSVC STL
#if !NH3API_CHECK_CPP11

#define INFINITY ((float)(1e+300 * 1e+300))
#define NAN (-(float)(NH3API_INFINITY * 0.0F))
#define FP_INFINITE  1
#define FP_NAN       2
#define FP_NORMAL    (-1)
#define FP_SUBNORMAL (-2)
#define FP_ZERO      0

union fval_t
{ // pun floating type as integer array
    uint16_t _Sh[8];
    float _Val;
};

union dval_t
{ // pun floating type as integer array
    uint16_t _Sh[8];
    double _Val;
};

// categorize *px
inline short fdtest(const float* const px)
{
    const fval_t* ps = reinterpret_cast<const fval_t*>(px);
    if ((ps->_Sh[1] & 32640u) == 255u << 7)
        return (ps->_Sh[1] & 127u) != 0 || ps->_Sh[0] != 0 ? FP_NAN : FP_INFINITE;
    else if ((ps->_Sh[1] & ~0x8000u) != 0 || ps->_Sh[0] != 0)
        return (ps->_Sh[1] & 32640u) == 0 ? FP_SUBNORMAL : FP_NORMAL;
    else
        return 0;
}

// categorize *px
inline short ddtest(const double* const px)
{
    const dval_t* ps = reinterpret_cast<const dval_t*>(px);
    if ((ps->_Sh[3] & 32752u) == 2047u << 4)
        return (ps->_Sh[3] & 15u) != 0 || ps->_Sh[2] != 0 || ps->_Sh[1] != 0 || ps->_Sh[0] != 0 ? FP_NAN : FP_INFINITE;
    else if ((ps->_Sh[3] & ~0x8000) != 0 || ps->_Sh[2] != 0 || ps->_Sh[1] != 0 || ps->_Sh[0] != 0)
        return (ps->_Sh[3] & 32752u) == 0 ? FP_SUBNORMAL : FP_NORMAL;
    else
        return 0;
}

#endif

} // namespace nh3api

// older versions of MSVC STL
#if !NH3API_CHECK_CPP11
namespace nh3api
{

inline int fpclassify(float num) NH3API_NOEXCEPT
{ return fdtest(&num); }

inline int fpclassify(double num) NH3API_NOEXCEPT
{ return ddtest(&num); }

inline bool isfinite(float num) NH3API_NOEXCEPT
{ return fdtest(&num) <= 0; }

inline bool isinf(float num) NH3API_NOEXCEPT
{ return fdtest(&num) == 1; }

inline bool isnan(float num) NH3API_NOEXCEPT
{ return fdtest(&num) == 2; }

inline bool isnormal(float num) NH3API_NOEXCEPT
{ return fdtest(&num) == (-1); }

inline bool isfinite(double num) NH3API_NOEXCEPT
{ return ddtest(&num) <= 0; }

inline bool isinf(double num) NH3API_NOEXCEPT
{ return ddtest(&num) == 1; }

inline bool isnan(double num) NH3API_NOEXCEPT
{ return ddtest(&num) == 2; }

inline bool isnormal(double num) NH3API_NOEXCEPT
{ return ddtest(&num) == (-1); }

inline double trunc(double num)
{ return (num > 0) ? floor(num) : ceil(num); }
inline float trunc(float num)
{ return (num > 0) ? floorf(num) : ceilf(num); }

inline bool signbit(double num) NH3API_NOEXCEPT
{ return !!(*reinterpret_cast<const uint64_t*>(&num) & (1ull << 63ull)); }
inline bool signbit(float num) NH3API_NOEXCEPT
{ return !!(*reinterpret_cast<const uint32_t*>(&num) & (1ul << 31ul)); }

} // namespace nh3api
#else
namespace nh3api
{

using ::std::fpclassify;
using ::std::isfinite;
using ::std::isinf;
using ::std::isnan;
using ::std::isnormal;
using ::std::trunc;
using ::std::signbit;

}
#endif