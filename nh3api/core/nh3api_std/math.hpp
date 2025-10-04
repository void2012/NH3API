//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
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
#if NH3API_CHECK_MSVC
#include <intrin.h>
#endif
#ifndef NH3API_FLAG_NO_CPP_EXCEPTIONS
#include <stdexcept>
#endif

namespace nh3api
{

// integer math

#if !NH3API_CHECK_MSVC
// integer logarithm of base 2
constexpr NH3API_FORCEINLINE uint32_t ilog2(uint32_t x) NH3API_NOEXCEPT
{ return (static_cast<uint32_t>(__builtin_clz(x))) ^ 31u; }
#else
NH3API_CONST NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
// integer logarithm of base 2
NH3API_FORCEINLINE uint32_t ilog2(uint32_t x) NH3API_NOEXCEPT
{
    #if NH3API_HAS_IS_CONSTANT_EVALUATED
    if constexpr (is_constant_evaluated())
    {
        // implementation from MSVC STL
        uint32_t _Yx = 0;

        unsigned int _Nx = sizeof(uint32_t) * CHAR_BIT;
        unsigned int _Cx = (sizeof(uint32_t) * CHAR_BIT) / 2;
        do {
            _Yx = static_cast<uint32_t>(x >> _Cx);
            if (_Yx != 0) {
                _Nx -= _Cx;
                x = _Yx;
            }
            _Cx >>= 1;
        } while (_Cx != 0);
        return static_cast<uint32_t>(static_cast<int>(_Nx) - static_cast<int>(x));
    }
    else  
    #endif
    {
        unsigned long result;
        _BitScanReverse(&result, x);
        return result;
    }
}
#endif

NH3API_CONST
// count the amount of digits in an integer
#if NH3API_CHECK_MSVC 
NH3API_CONSTEXPR_IF_HAS_IF_CONSTANT_EVALUATED
#else 
constexpr
#endif
NH3API_FORCEINLINE uint32_t count_digits(uint32_t x) NH3API_NOEXCEPT
{
    NH3API_CONSTEXPR_VAR uint32_t digits_table[] = {9, 99, 999, 9999, 99999,
    999999, 9999999, 99999999, 999999999};
    uint32_t y = (9 * ilog2(x)) >> 5;
    NH3API_ASSUME(y < 9);
    y += x > digits_table[y];
    return y + 1;
}

}
