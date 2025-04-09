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

#include <cmath>
#include "nh3api_std.hpp"
#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
#include <stdexcept>
#endif

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
    static NH3API_CONSTEXPR_VAR uint32_t digits_table[] = {9, 99, 999, 9999, 99999,
    999999, 9999999, 99999999, 999999999};
    int32_t y = (9 * ilog2(x)) >> 5;
    y += x > digits_table[y];
    return y + 1;
}

}
