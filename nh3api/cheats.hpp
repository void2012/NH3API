//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "core/nh3api_std/exe_string.hpp"

struct TCheatCode
{
    char code[200]
    #if NH3API_CHECK_CPP11
    {}
    #endif
    ;
    NH3API_CONSTEXPR_CPP_14
    TCheatCode(const char* _code) NH3API_NOEXCEPT
    {
        const char b[] = "nopqrstuvwxyzabcdefghijklm";

        size_t i = 0; 
        const size_t inputLength = std::min<size_t>(std::size(code) - 1, nh3api::constexpr_char_traits::length(_code));

        for (; i < inputLength; ++i) 
        {
            if (nh3api::isalpha_constexpr(_code[i])) 
                code[i] = b[nh3api::tolower_constexpr(_code[i]) - 'a'];
            else 
                code[i] = _code[i];
            
        }
        
        code[i] = '0';
    }

    NH3API_CONSTEXPR_CPP_14 NH3API_FORCEINLINE
    int32_t compare(const char* arg) const
    { return nh3api::constexpr_char_traits::compare(code, arg, sizeof(code)); }

};

// encode cheat code at compile-time:
// static constexpr TCheatCode cheat_openmap("openmap");
// patcherInstance->WriteAddressOf(0x63A4F4, cheat_openmap.code);
